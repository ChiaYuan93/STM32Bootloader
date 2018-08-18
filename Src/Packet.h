#ifndef __Packet_H__
#define __Packet_H__

#include <windows.h>
#include <stdint.h>

//// Header in Packet.h used by Host and Monitor ///////
#define FLASH_MASS_ERASE    		0x10
#define FLASH_PAGE_ERASE    		0x11
#define FLASH_PROGRAM_WORD     		0x12
#define FLASH_PROGRAM_HALF_WORD		0x13
#define FLASH_PROGRAM_BYTE_WORD		0x14
#define FALSH_UNLOCK				0x15
#define FLASH_LOCK 					0x16

typedef struct Packet Packet;

struct Packet{
  uint8_t type;
  uint8_t length;
  uint8_t data[0];
};

int uartReceivedPacket(Packet *packet);

#endif  // __Packet_H__