#include "power_gateway/mqtt_session.h"

#include <stddef.h>

static bool time_reached(uint32_t now_ms, uint32_t target_ms) {
    return (int32_t)(now_ms - target_ms) >= 0;
}

static uint32_t retry_delay(const pg_mqtt_session_t *session) {
    uint32_t delay = session->base_backoff_ms;
    uint8_t shift = session->failure_count == 0u
                        ? 0u
                        : (uint8_t)(session->failure_count - 1u);
    while (shift-- > 0u && delay < session->max_backoff_ms) {
        delay = delay > session->max_backoff_ms / 2u
                    ? session->max_backoff_ms
                    : delay * 2u;
    }
    return delay > session->max_backoff_ms ? session->max_backoff_ms : delay;
}

bool pg_mqtt_session_init(pg_mqtt_session_t *session,
                          uint32_t base_backoff_ms,
                          uint32_t max_backoff_ms,
                          uint32_t keepalive_ms,
                          uint32_t now_ms) {
    if (session == NULL || base_backoff_ms == 0u ||
        max_backoff_ms < base_backoff_ms || keepalive_ms == 0u) {
        return false;
    }
    session->state = PG_MQTT_OFFLINE;
    session->base_backoff_ms = base_backoff_ms;
    session->max_backoff_ms = max_backoff_ms;
    session->retry_at_ms = now_ms;
    session->keepalive_ms = keepalive_ms;
    session->last_activity_ms = now_ms;
    session->failure_count = 0u;
    session->session_present = false;
    session->flush_pending = false;
    return true;
}

pg_mqtt_action_t pg_mqtt_session_step(pg_mqtt_session_t *session,
                                      uint32_t now_ms) {
    if (session == NULL) {
        return PG_MQTT_ACTION_NONE;
    }
    if (session->state == PG_MQTT_OFFLINE ||
        (session->state == PG_MQTT_BACKOFF &&
         time_reached(now_ms, session->retry_at_ms))) {
        session->state = PG_MQTT_CONNECTING;
        return PG_MQTT_ACTION_CONNECT;
    }
    if (session->state == PG_MQTT_SUBSCRIBING) {
        return PG_MQTT_ACTION_SUBSCRIBE;
    }
    if (session->state == PG_MQTT_ONLINE && session->flush_pending) {
        session->flush_pending = false;
        return PG_MQTT_ACTION_FLUSH_OUTBOX;
    }
    if (session->state == PG_MQTT_ONLINE &&
        time_reached(now_ms, session->last_activity_ms + session->keepalive_ms)) {
        session->last_activity_ms = now_ms;
        return PG_MQTT_ACTION_PING;
    }
    return PG_MQTT_ACTION_NONE;
}

void pg_mqtt_session_on_connected(pg_mqtt_session_t *session,
                                  bool session_present,
                                  uint32_t now_ms) {
    if (session == NULL) {
        return;
    }
    session->session_present = session_present;
    session->failure_count = 0u;
    session->last_activity_ms = now_ms;
    session->state = session_present ? PG_MQTT_ONLINE : PG_MQTT_SUBSCRIBING;
    session->flush_pending = session_present;
}

void pg_mqtt_session_on_subscribed(pg_mqtt_session_t *session,
                                   uint32_t now_ms) {
    if (session != NULL && session->state == PG_MQTT_SUBSCRIBING) {
        session->state = PG_MQTT_ONLINE;
        session->last_activity_ms = now_ms;
        session->flush_pending = true;
    }
}

void pg_mqtt_session_on_activity(pg_mqtt_session_t *session,
                                 uint32_t now_ms) {
    if (session != NULL) {
        session->last_activity_ms = now_ms;
    }
}

void pg_mqtt_session_on_failure(pg_mqtt_session_t *session,
                                uint32_t now_ms) {
    if (session == NULL) {
        return;
    }
    if (session->failure_count < 31u) {
        session->failure_count++;
    }
    session->retry_at_ms = now_ms + retry_delay(session);
    session->state = PG_MQTT_BACKOFF;
    session->session_present = false;
    session->flush_pending = false;
}

bool pg_mqtt_session_is_online(const pg_mqtt_session_t *session) {
    return session != NULL && session->state == PG_MQTT_ONLINE;
}
