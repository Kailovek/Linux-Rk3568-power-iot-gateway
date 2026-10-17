#include "reference_adapter.h"

#include <string.h>

static bool publish_record(void *context, const pg_telemetry_t *telemetry) {
    pg_reference_adapter_t *adapter = (pg_reference_adapter_t *)context;
    if (adapter == NULL || telemetry == NULL || !adapter->broker_available) {
        return false;
    }
    adapter->last_published = *telemetry;
    adapter->publish_count++;
    return true;
}

static bool persist_record(void *context, const pg_telemetry_t *telemetry) {
    pg_reference_adapter_t *adapter = (pg_reference_adapter_t *)context;
    if (adapter == NULL || telemetry == NULL) {
        return false;
    }
    adapter->last_persisted = *telemetry;
    adapter->persist_count++;
    return true;
}

void pg_reference_adapter_init(pg_reference_adapter_t *adapter) {
    if (adapter != NULL) {
        memset(adapter, 0, sizeof(*adapter));
    }
}

void pg_reference_adapter_set_broker(pg_reference_adapter_t *adapter,
                                     bool available) {
    if (adapter != NULL) {
        adapter->broker_available = available;
    }
}

pg_delivery_port_t pg_reference_adapter_port(pg_reference_adapter_t *adapter) {
    pg_delivery_port_t port;
    port.context = adapter;
    port.publish = publish_record;
    port.persist = persist_record;
    return port;
}
