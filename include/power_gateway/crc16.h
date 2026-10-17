#ifndef POWER_GATEWAY_CRC16_H
#define POWER_GATEWAY_CRC16_H

#include <stddef.h>
#include <stdint.h>

uint16_t pg_crc16_modbus(const uint8_t *data, size_t length);

#endif
