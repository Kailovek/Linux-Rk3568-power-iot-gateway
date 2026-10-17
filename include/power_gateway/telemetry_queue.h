#ifndef POWER_GATEWAY_TELEMETRY_QUEUE_H
#define POWER_GATEWAY_TELEMETRY_QUEUE_H

#include "power_gateway/telemetry.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    pg_telemetry_t *storage;
    size_t capacity;
    size_t head;
    size_t size;
    uint32_t overflow_count;
} pg_telemetry_queue_t;

bool pg_telemetry_queue_init(pg_telemetry_queue_t *queue,
                             pg_telemetry_t *storage,
                             size_t capacity);
bool pg_telemetry_queue_push(pg_telemetry_queue_t *queue,
                             const pg_telemetry_t *telemetry,
                             pg_telemetry_t *evicted);
bool pg_telemetry_queue_peek(const pg_telemetry_queue_t *queue,
                             pg_telemetry_t *telemetry);
bool pg_telemetry_queue_pop(pg_telemetry_queue_t *queue,
                            pg_telemetry_t *telemetry);
size_t pg_telemetry_queue_size(const pg_telemetry_queue_t *queue);

#endif
