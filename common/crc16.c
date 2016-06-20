#include <stdint.h>

static uint16_t crc16_add(uint16_t crc, uint8_t c, uint16_t poly)
{
  uint8_t  i;

  for (i = 0; i < 8; i++) {
    if ((crc ^ c) & 1)
      crc = (crc >> 1) ^ poly;
    else
      crc >>= 1;
    c >>= 1;
  }
  return crc;
}

uint16_t crc16(const uint8_t *data, uint32_t count,
               uint16_t init, uint32_t poly) {

  while (count--)
    init = crc16_add(init, *data++, poly);

  return init;
}
