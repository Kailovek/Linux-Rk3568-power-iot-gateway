#ifndef POWER_GATEWAY_MQTT_SESSION_H
#define POWER_GATEWAY_MQTT_SESSION_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    PG_MQTT_OFFLINE,
    PG_MQTT_CONNECTING,
    PG_MQTT_SUBSCRIBING,
    PG_MQTT_ONLINE,
    PG_MQTT_BACKOFF
} pg_mqtt_state_t;

typedef enum {
    PG_MQTT_ACTION_NONE,
    PG_MQTT_ACTION_CONNECT,
    PG_MQTT_ACTION_SUBSCRIBE,
    PG_MQTT_ACTION_PING,
    PG_MQTT_ACTION_FLUSH_OUTBOX
} pg_mqtt_action_t;

typedef struct {
    pg_mqtt_state_t state;
    uint32_t base_backoff_ms;
    uint32_t max_backoff_ms;
    uint32_t retry_at_ms;
    uint32_t keepalive_ms;
    uint32_t last_activity_ms;
    uint8_t failure_count;
    bool session_present;
    bool flush_pending;
} pg_mqtt_session_t;

bool pg_mqtt_session_init(pg_mqtt_session_t *session,
                          uint32_t base_backoff_ms,
                          uint32_t max_backoff_ms,
                          uint32_t keepalive_ms,
                          uint32_t now_ms);
pg_mqtt_action_t pg_mqtt_session_step(pg_mqtt_session_t *session,
                                      uint32_t now_ms);
void pg_mqtt_session_on_connected(pg_mqtt_session_t *session,
                                  bool session_present,
                                  uint32_t now_ms);
void pg_mqtt_session_on_subscribed(pg_mqtt_session_t *session,
                                   uint32_t now_ms);
void pg_mqtt_session_on_activity(pg_mqtt_session_t *session,
                                 uint32_t now_ms);
void pg_mqtt_session_on_failure(pg_mqtt_session_t *session,
                                uint32_t now_ms);
bool pg_mqtt_session_is_online(const pg_mqtt_session_t *session);

#endif
