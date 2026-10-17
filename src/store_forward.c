#include "power_gateway/store_forward.h"

#include <stddef.h>

bool pg_store_forward_init(pg_store_forward_t *service,
                           pg_telemetry_queue_t *outbox,
                           pg_delivery_port_t port) {
    if (service == NULL || outbox == NULL || port.publish == NULL) {
        return false;
    }
    service->outbox = outbox;
    service->port = port;
    service->published_count = 0u;
    service->persisted_count = 0u;
    service->dropped_count = 0u;
    return true;
}

pg_delivery_result_t pg_store_forward_submit(pg_store_forward_t *service,
                                             const pg_telemetry_t *telemetry,
                                             bool mqtt_online) {
    pg_telemetry_t evicted;
    bool queue_was_full;
    if (service == NULL || telemetry == NULL || service->outbox == NULL) {
        return PG_DELIVERY_DROPPED;
    }
    if (mqtt_online && service->port.publish(service->port.context, telemetry)) {
        service->published_count++;
        return PG_DELIVERY_PUBLISHED;
    }

    queue_was_full = pg_telemetry_queue_size(service->outbox) ==
                     service->outbox->capacity;
    if (!pg_telemetry_queue_push(service->outbox, telemetry, &evicted)) {
        service->dropped_count++;
        return PG_DELIVERY_DROPPED;
    }
    if (!queue_was_full) {
        return PG_DELIVERY_BUFFERED;
    }
    if (service->port.persist != NULL &&
        service->port.persist(service->port.context, &evicted)) {
        service->persisted_count++;
        return PG_DELIVERY_PERSISTED;
    }
    service->dropped_count++;
    return PG_DELIVERY_DROPPED;
}

size_t pg_store_forward_flush(pg_store_forward_t *service,
                              bool mqtt_online,
                              size_t budget) {
    pg_telemetry_t telemetry;
    size_t sent = 0u;
    if (service == NULL || !mqtt_online || service->port.publish == NULL) {
        return 0u;
    }
    while (sent < budget &&
           pg_telemetry_queue_peek(service->outbox, &telemetry)) {
        if (!service->port.publish(service->port.context, &telemetry)) {
            break;
        }
        pg_telemetry_queue_pop(service->outbox, &telemetry);
        service->published_count++;
        sent++;
    }
    return sent;
}
