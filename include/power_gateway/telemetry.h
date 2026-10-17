#ifndef POWER_GATEWAY_TELEMETRY_H
#define POWER_GATEWAY_TELEMETRY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PG_DEVICE_ID_CAPACITY 32u
#define PG_METRIC_NAME_CAPACITY 24u
#define PG_MAX_METRICS 8u

typedef enum {
    PG_QUALITY_GOOD = 0,
    PG_QUALITY_STALE,
    PG_QUALITY_INVALID
} pg_quality_t;

typedef struct {
    char name[PG_METRIC_NAME_CAPACITY];
    double value;
    pg_quality_t quality;
} pg_metric_t;

typedef struct {
    char device_id[PG_DEVICE_ID_CAPACITY];
    uint64_t timestamp_ms;
    pg_metric_t metrics[PG_MAX_METRICS];
    size_t metric_count;
} pg_telemetry_t;

bool pg_telemetry_init(pg_telemetry_t *telemetry,
                       const char *device_id,
                       uint64_t timestamp_ms);
bool pg_telemetry_add(pg_telemetry_t *telemetry,
                      const char *name,
                      double value,
                      pg_quality_t quality);

#endif
