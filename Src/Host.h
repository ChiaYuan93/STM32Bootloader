#ifndef _HOST_H
#define _HOST_H
#include <stdint.h>
#include "unity.h"

void sendPacket(uint8_t type, uint8_t *data, uint8_t size);

#endif // _HOST_H
