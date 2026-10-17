#include "power_gateway/crc16.h"

uint16_t pg_crc16_modbus(const uint8_t *data, size_t length) {
    uint16_t crc = 0xFFFFu;
    size_t index;
    unsigned bit;

    if (data == NULL && length != 0u) {
        return 0u;
    }
    for (index = 0u; index < length; ++index) {
        crc ^= data[index];
        for (bit = 0u; bit < 8u; ++bit) {
            crc = (crc & 1u) != 0u
                      ? (uint16_t)((crc >> 1u) ^ 0xA001u)
                      : (uint16_t)(crc >> 1u);
        }
    }
    return crc;
}
