#include "Host.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "unity.h"
#include "Serial.h"

HANDLE hostUart;

/**
 * Send TLV packet. The format is:
 *   type | len | data 0 | data 1 | ... | data N
 */
void sendPacket(uint8_t type, uint8_t size,uint8_t *data ) {
  writeToSerialPort(hostUart, &type, 1);
  writeToSerialPort(hostUart, &size, 1);
  writeToSerialPort(hostUart, data, size);  
}
