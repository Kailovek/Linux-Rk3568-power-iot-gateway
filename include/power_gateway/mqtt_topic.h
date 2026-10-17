#ifndef POWER_GATEWAY_MQTT_TOPIC_H
#define POWER_GATEWAY_MQTT_TOPIC_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    PG_TOPIC_TELEMETRY,
    PG_TOPIC_STATE,
    PG_TOPIC_COMMAND,
    PG_TOPIC_REPLY
} pg_topic_kind_t;

bool pg_mqtt_topic_segment_is_valid(const char *segment);
bool pg_mqtt_topic_build(char *output,
                         size_t capacity,
                         const char *namespace_name,
                         const char *device_id,
                         pg_topic_kind_t kind);

#endif
