#include "power_gateway/mqtt_topic.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool pg_mqtt_topic_segment_is_valid(const char *segment) {
    const unsigned char *cursor = (const unsigned char *)segment;
    if (cursor == NULL || *cursor == '\0') {
        return false;
    }
    while (*cursor != '\0') {
        if (!(isalnum(*cursor) || *cursor == '-' || *cursor == '_' ||
              *cursor == '.')) {
            return false;
        }
        cursor++;
    }
    return true;
}

bool pg_mqtt_topic_build(char *output,
                         size_t capacity,
                         const char *namespace_name,
                         const char *device_id,
                         pg_topic_kind_t kind) {
    static const char *const suffixes[] = {
        "telemetry", "state", "command", "reply"
    };
    int length;
    if (output == NULL || capacity == 0u ||
        !pg_mqtt_topic_segment_is_valid(namespace_name) ||
        !pg_mqtt_topic_segment_is_valid(device_id) ||
        kind < PG_TOPIC_TELEMETRY || kind > PG_TOPIC_REPLY) {
        return false;
    }
    length = snprintf(output,
                      capacity,
                      "%s/devices/%s/%s",
                      namespace_name,
                      device_id,
                      suffixes[kind]);
    return length > 0 && (size_t)length < capacity;
}
