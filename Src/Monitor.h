#ifndef _MONITOR_H
#define _MONITOR_H
#include <stdint.h>
#include "unity.h"

void handlePacketStateMachine(uint8_t packetType);
void initFlashPageInfo();
int hasPageBeenErased(int pageNumber);
void setPageArray(int pageNumber);
int getPageForAddress(uint32_t address); 

#endif // _MONITOR_H
