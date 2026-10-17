#include "power_gateway/telemetry.h"

#include <string.h>

static bool copy_text(char *output, size_t capacity, const char *input) {
    size_t length;
    if (output == NULL || input == NULL || capacity == 0u) {
        return false;
    }
    length = strlen(input);
    if (length == 0u || length >= capacity) {
        return false;
    }
    memcpy(output, input, length + 1u);
    return true;
}

bool pg_telemetry_init(pg_telemetry_t *telemetry,
                       const char *device_id,
                       uint64_t timestamp_ms) {
    if (telemetry == NULL) {
        return false;
    }
    memset(telemetry, 0, sizeof(*telemetry));
    if (!copy_text(telemetry->device_id,
                   sizeof(telemetry->device_id),
                   device_id)) {
        return false;
    }
    telemetry->timestamp_ms = timestamp_ms;
    return true;
}

bool pg_telemetry_add(pg_telemetry_t *telemetry,
                      const char *name,
                      double value,
                      pg_quality_t quality) {
    pg_metric_t *metric;
    if (telemetry == NULL || telemetry->metric_count >= PG_MAX_METRICS) {
        return false;
    }
    metric = &telemetry->metrics[telemetry->metric_count];
    if (!copy_text(metric->name, sizeof(metric->name), name)) {
        return false;
    }
    metric->value = value;
    metric->quality = quality;
    telemetry->metric_count++;
    return true;
}
