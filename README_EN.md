# Power IoT Edge Gateway

> **Author: Hzhikai · Original Technical Portfolio · Open Learning · MIT Licensed**

An Embedded Linux gateway portfolio focused on MQTT telemetry delivery for power-facility monitoring. The original project separated serial-device acquisition and cloud communication with pthread workers and System V message queues, then combined MQTT, a bounded FIFO and SQLite-backed history for offline delivery.

The public code, architecture and documentation were independently organized and authored by **Hzhikai** from hands-on embedded development experience. They are shared for technical exchange, open learning and portfolio presentation.

## Public MQTT Design

- connection, subscription, keepalive and reconnect state machine
- bounded exponential backoff
- validated telemetry/state/command/reply topic construction
- fixed-capacity telemetry outbox
- store-and-forward policy with persistence boundary
- recovery-time backlog flushing with a per-cycle budget
- explicit telemetry quality states

The repository contains no production broker, credential, device identity, proprietary topic, customer data or third-party binary.

## Layout

```text
include/power_gateway/  Public MQTT and telemetry interfaces
src/                    Session, topic, queue and delivery policies
platform/               Reference platform adapter
examples/               Gateway data-flow entry point
docs/                   Publication and image notes
```

See [README.md](README.md) for the detailed architecture and [PUBLICATION_SCOPE.md](PUBLICATION_SCOPE.md) for the publication boundary.

## License

The code and documentation authored by **Hzhikai** in this repository are available under the [MIT License](LICENSE). This license does not apply to excluded production firmware, company material, customer data or third-party dependencies.
