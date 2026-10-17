#ifndef POWER_GATEWAY_STORE_FORWARD_H
#define POWER_GATEWAY_STORE_FORWARD_H

#include "power_gateway/telemetry_queue.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef bool (*pg_publish_fn)(void *context, const pg_telemetry_t *telemetry);
typedef bool (*pg_persist_fn)(void *context, const pg_telemetry_t *telemetry);

typedef struct {
    void *context;
    pg_publish_fn publish;
    pg_persist_fn persist;
} pg_delivery_port_t;

typedef enum {
    PG_DELIVERY_PUBLISHED,
    PG_DELIVERY_BUFFERED,
    PG_DELIVERY_PERSISTED,
    PG_DELIVERY_DROPPED
} pg_delivery_result_t;

typedef struct {
    pg_telemetry_queue_t *outbox;
    pg_delivery_port_t port;
    uint32_t published_count;
    uint32_t persisted_count;
    uint32_t dropped_count;
} pg_store_forward_t;

bool pg_store_forward_init(pg_store_forward_t *service,
                           pg_telemetry_queue_t *outbox,
                           pg_delivery_port_t port);
pg_delivery_result_t pg_store_forward_submit(pg_store_forward_t *service,
                                             const pg_telemetry_t *telemetry,
                                             bool mqtt_online);
size_t pg_store_forward_flush(pg_store_forward_t *service,
                              bool mqtt_online,
                              size_t budget);

#endif
