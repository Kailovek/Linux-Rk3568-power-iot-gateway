#include "power_gateway/telemetry_queue.h"

bool pg_telemetry_queue_init(pg_telemetry_queue_t *queue,
                             pg_telemetry_t *storage,
                             size_t capacity) {
    if (queue == NULL || storage == NULL || capacity == 0u) {
        return false;
    }
    queue->storage = storage;
    queue->capacity = capacity;
    queue->head = 0u;
    queue->size = 0u;
    queue->overflow_count = 0u;
    return true;
}

bool pg_telemetry_queue_push(pg_telemetry_queue_t *queue,
                             const pg_telemetry_t *telemetry,
                             pg_telemetry_t *evicted) {
    size_t tail;
    if (queue == NULL || telemetry == NULL || queue->storage == NULL ||
        queue->capacity == 0u) {
        return false;
    }
    if (queue->size == queue->capacity) {
        if (evicted != NULL) {
            *evicted = queue->storage[queue->head];
        }
        queue->head = (queue->head + 1u) % queue->capacity;
        queue->size--;
        queue->overflow_count++;
    }
    tail = (queue->head + queue->size) % queue->capacity;
    queue->storage[tail] = *telemetry;
    queue->size++;
    return true;
}

bool pg_telemetry_queue_peek(const pg_telemetry_queue_t *queue,
                             pg_telemetry_t *telemetry) {
    if (queue == NULL || telemetry == NULL || queue->size == 0u) {
        return false;
    }
    *telemetry = queue->storage[queue->head];
    return true;
}

bool pg_telemetry_queue_pop(pg_telemetry_queue_t *queue,
                            pg_telemetry_t *telemetry) {
    if (!pg_telemetry_queue_peek(queue, telemetry)) {
        return false;
    }
    queue->head = (queue->head + 1u) % queue->capacity;
    queue->size--;
    return true;
}

size_t pg_telemetry_queue_size(const pg_telemetry_queue_t *queue) {
    return queue == NULL ? 0u : queue->size;
}
