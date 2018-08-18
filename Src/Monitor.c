#include "Monitor.h"
#include <stdint.h>
#include <stdio.h>
#include "unity.h"
#include "Packet.h"
#include "Stm32Functions.h"
#include "Stm32Uart.h"
#include "stm32f1xx_hal_def.h"

uint8_t *flashMemory = (uint8_t *)0x8000000;
uint32_t address, data;
uint8_t memoryPageArray[128];
uint8_t rxPacketBuffer[255+2];
uint8_t txPacketBuffer[255+2];

int uartReceivedPacket(Packet *packet){
 if((packet->length) != 0){
	 
	return 1;
 }
 else
    return 0; 
}

void handlePacketStateMachine() {
  Packet *packet;
  packet = (Packet*)rxPacketBuffer;
  int uartPacketReceived = uartReceivedPacket(packet);
  
  if(uartPacketReceived){
	switch(packet->type){
      case FLASH_MASS_ERASE:
      flash_massErase();
      break;
  
      case FLASH_PAGE_ERASE:   
	  printf("nothing")	;
      flash_pageErase(address);
      break;
  
      case FLASH_PROGRAM_WORD:     		
      flash_program_word(address, data);
      break;
      
      case FLASH_PROGRAM_HALF_WORD:		
      flash_program_halfword(address, data);
      break;
      
      case FLASH_PROGRAM_BYTE_WORD:		
      flash_program_byteword(address, data);
      break;
      
      case FALSH_UNLOCK:				
      flash_unlock();
      break;
      
      case FLASH_LOCK: 					
      flash_lock();
      break;
    }
  }
}

void initFlashPageInfo(){
  memset(memoryPageArray, 0, 128);
}

int hasPageBeenErased(int pageNumber){
  if(memoryPageArray[pageNumber]){
    return 1;
  }
  else
	return 0; 
}

void setPageArray(int pageNumber){
	memoryPageArray[pageNumber] = 1;
}

int getPageForAddress(uint32_t address){
	int pageNumber = ((address-0x8000000)/(0x400));
	
	return pageNumber;
}







