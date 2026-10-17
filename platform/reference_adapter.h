#ifndef POWER_GATEWAY_REFERENCE_ADAPTER_H
#define POWER_GATEWAY_REFERENCE_ADAPTER_H

#include "power_gateway/store_forward.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    bool broker_available;
    size_t publish_count;
    size_t persist_count;
    pg_telemetry_t last_published;
    pg_telemetry_t last_persisted;
} pg_reference_adapter_t;

void pg_reference_adapter_init(pg_reference_adapter_t *adapter);
void pg_reference_adapter_set_broker(pg_reference_adapter_t *adapter,
                                     bool available);
pg_delivery_port_t pg_reference_adapter_port(pg_reference_adapter_t *adapter);

#endif
