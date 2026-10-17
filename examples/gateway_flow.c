#include "power_gateway/mqtt_session.h"
#include "power_gateway/mqtt_topic.h"
#include "power_gateway/store_forward.h"
#include "power_gateway/telemetry.h"
#include "power_gateway/telemetry_queue.h"
#include "reference_adapter.h"

#include <stdio.h>

int main(void) {
    pg_telemetry_t storage[4];
    pg_telemetry_t telemetry;
    pg_telemetry_queue_t outbox;
    pg_reference_adapter_t adapter;
    pg_store_forward_t delivery;
    pg_mqtt_session_t session;
    char topic[96];

    pg_telemetry_queue_init(&outbox, storage, 4u);
    pg_reference_adapter_init(&adapter);
    pg_store_forward_init(&delivery,
                          &outbox,
                          pg_reference_adapter_port(&adapter));
    pg_mqtt_session_init(&session, 1000u, 30000u, 60000u, 0u);

    pg_mqtt_topic_build(topic,
                        sizeof(topic),
                        "power-gateway",
                        "edge-node-01",
                        PG_TOPIC_TELEMETRY);
    pg_telemetry_init(&telemetry, "edge-node-01", 1000u);
    pg_telemetry_add(&telemetry, "temperature", 24.6, PG_QUALITY_GOOD);
    pg_telemetry_add(&telemetry, "voltage", 220.1, PG_QUALITY_GOOD);

    pg_store_forward_submit(&delivery, &telemetry, false);
    pg_reference_adapter_set_broker(&adapter, true);
    pg_mqtt_session_on_connected(&session, false, 2000u);
    pg_mqtt_session_on_subscribed(&session, 2100u);
    pg_store_forward_flush(&delivery,
                           pg_mqtt_session_is_online(&session),
                           4u);

    printf("MQTT topic: %s\n", topic);
    printf("Published records: %u\n", (unsigned)delivery.published_count);
    printf("Pending records: %u\n",
           (unsigned)pg_telemetry_queue_size(&outbox));
    return 0;
}
