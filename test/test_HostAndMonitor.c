#include <windows.h>
#include <stdint.h>
#include "unity.h"
#include "Host.h"
#include "Monitor.h"
#include "mock_Serial.h"
//#include "mock_Stm32Uart.h"

#define K   1024

uint8_t host2MonitorSerialBuffer[4096];
uint8_t host2MonitorBufferIdx = 0;
uint8_t monitor2HostSerialBuffer[4096];
uint8_t monitor2HostBufferIdx = 0;

void setUp(void) {}

void tearDown(void) {}

HANDLE fake_initSerialComm(LPCSTR portname, DWORD baudrate, int NumCalls) {
  // Just ignore, but return an easily identifier for HANDLE 
  return (HANDLE)0xdeadbeef;
}

void fake_closeSerialPort(HANDLE hSerial, int NumCalls) {
  // Just ignore
}

DWORD fake_writeToSerialPort(HANDLE hSerial, uint8_t * data, int length, int NumCalls) {
  memcpy(&host2MonitorSerialBuffer[host2MonitorBufferIdx], data, length);
  host2MonitorBufferIdx += length;
  return length;  
}

DWORD fake_readFromSerialPort(HANDLE hSerial, uint8_t * buffer, int buffersize, int NumCalls) {
  int len = monitor2HostBufferIdx;
  memcpy(buffer, monitor2HostSerialBuffer, monitor2HostBufferIdx);
  monitor2HostBufferIdx = 0;
  return len;   
}

//// Header in Packet.h used by Host and Monitor ///////
#define FLASH_MASS_ERASE    0x10
#define FLASH_PAGE_ERASE    0x11
#define FLASH_PROGRAM       0x12
// ...
//// Header in Packet.h used by Host and Monitor ///////

///// Code in Host.c ///////////////
HANDLE hostUart;

/**
 * Send TLV packet. The format is:
 *   type | len | data 0 | data 1 | ... | data N
 */
void sendPacket(uint8_t type, uint8_t *data, uint8_t size) {
  writeToSerialPort(hostUart, &type, 1);
  writeToSerialPort(hostUart, &size, 1);
  writeToSerialPort(hostUart, data, size);  
}
///// Code in Host.c ///////////////

///// Code in Monitor.c for STM32 ///////////////
uint8_t *flashMemory = (uint8_t *)0x800000;

void handlePacketStateMachine() {
  // Your code to handle state machine in your STM32 that handles
  // flash mass erase, page erase, flash programming, etc.
}
///// Code in Monitor.c ///////////////

extern uint8_t *flashMemory;
uint8_t fakeStm32FlashMemory[64*K];

void setupFakeSerial() {
  host2MonitorBufferIdx = 0;
  monitor2HostBufferIdx = 0;

  flashMemory = fakeStm32FlashMemory;
  
  initSerialComm_StubWithCallback(fake_initSerialComm);
  closeSerialPort_StubWithCallback(fake_closeSerialPort);
  writeToSerialPort_StubWithCallback(fake_writeToSerialPort);
  readFromSerialPort_StubWithCallback(fake_readFromSerialPort);
} 

void test_host_sendPacket_FLASH_MASS_ERASE_(void) {
  int i;
  char errMsg[256];
    
  setupFakeSerial();
  
  sendPacket(FLASH_MASS_ERASE, NULL, 0);
  // State machine code in STM32
  handlePacketStateMachine();
  handlePacketStateMachine();
  handlePacketStateMachine();
  handlePacketStateMachine();
  handlePacketStateMachine();
  
  for(i = 0; i < 64*K; i++) {
    sprintf(errMsg, "Flash at address %d is not properly erased.", i);
    TEST_ASSERT_EQUAL_MESSAGE(0xff, flashMemory[i], errMsg);
  } 
}
