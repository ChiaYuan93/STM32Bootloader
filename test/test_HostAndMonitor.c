#include <windows.h>
#include <stdint.h>
#include "unity.h"
#include "Host.h"
#include "Monitor.h"
#include "Packet.h"
#include "stm32f1xx_hal_def.h"
#include "mock_Serial.h"
#include "mock_Stm32Uart.h"
#include "mock_Stm32Functions.h"

#define K   			1024
#define BASEADDRESS		0x08000000

uint8_t host2MonitorSerialBuffer[4096];
uint8_t host2MonitorBufferIdx = 0;
uint8_t monitor2HostSerialBuffer[4096];
uint8_t monitor2HostBufferIdx = 0;

void setUp(void) {}

void tearDown(void) {}

HAL_StatusTypeDef fake_HAL_UART_Init(UART_HandleTypeDef *huart, int NumCalls) {
  return (HAL_StatusTypeDef)0xdeadbeef;	
}

HAL_StatusTypeDef fake_HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout, int NumCalls) {
  memcpy(&monitor2HostSerialBuffer[monitor2HostBufferIdx], pData, Size);
  monitor2HostBufferIdx += Size;
  return Size;
}

HAL_StatusTypeDef fake_HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout, int NumCalls) {
  int BuffSize = host2MonitorBufferIdx;
  memcpy(&Size, host2MonitorSerialBuffer, host2MonitorBufferIdx);
  monitor2HostBufferIdx = 0;
  return BuffSize;  
}

HANDLE fake_initSerialComm(LPCSTR portname, DWORD baudrate, int NumCalls) {
  // Just ignore, but return an easily identifier for HANDLE 
  return (HANDLE)0xdeadbeef;
}

void fake_closeSerialPort(HANDLE hSerial, int NumCalls) {
  // Just ignore
}

DWORD fake_writeToSerialPort(HANDLE hSerial, uint8_t *data, int length, int NumCalls) {
  memcpy(&host2MonitorSerialBuffer[host2MonitorBufferIdx], data, length);
  host2MonitorBufferIdx += length;
  return length;  
}

DWORD fake_readFromSerialPort(HANDLE hSerial, uint8_t *buffer, int buffersize, int NumCalls) {
  int len = monitor2HostBufferIdx;
  memcpy(buffer, monitor2HostSerialBuffer, monitor2HostBufferIdx);
  monitor2HostBufferIdx = 0;
  return len;   
}

extern uint8_t *flashMemory;
uint8_t fakeStm32FlashMemory[128*K];

void fake_flash_pageErase(uint32_t address, int NumCalls) {
  
  for(int i = 0; i < K; i ++){
	flashMemory[i]  = 0xFF;
  } 
}

void setupFakeSerial() {
  host2MonitorBufferIdx = 0;
  monitor2HostBufferIdx = 0;

  flashMemory = fakeStm32FlashMemory;
  
  initSerialComm_StubWithCallback(fake_initSerialComm);
  closeSerialPort_StubWithCallback(fake_closeSerialPort);
  writeToSerialPort_StubWithCallback(fake_writeToSerialPort);
  readFromSerialPort_StubWithCallback(fake_readFromSerialPort);
  HAL_UART_Init_StubWithCallback(fake_HAL_UART_Init);
  HAL_UART_Transmit_StubWithCallback(fake_HAL_UART_Transmit);
  HAL_UART_Receive_StubWithCallback(fake_HAL_UART_Receive);
  flash_pageErase_StubWithCallback(fake_flash_pageErase);
} 

void xtest_host_sendPacket_FLASH_MASS_ERASE_(void) {
  int i;
  char errMsg[256];
    
  setupFakeSerial();
  
  sendPacket(FLASH_MASS_ERASE, NULL, 0);
  // State machine code in STM32
  handlePacketStateMachine(FLASH_MASS_ERASE);
  handlePacketStateMachine(FLASH_MASS_ERASE);
  handlePacketStateMachine(FLASH_MASS_ERASE);
  handlePacketStateMachine(FLASH_MASS_ERASE);
  handlePacketStateMachine(FLASH_MASS_ERASE);
  
  for(i = 0; i < 64*K; i++) {
    sprintf(errMsg, "Flash at address %d is not properly erased.", i);
    TEST_ASSERT_EQUAL_MESSAGE(0xff, flashMemory[i], errMsg);
  } 
}

void test_host_sendPacket_FLASH_PAGE_ERASE_(void) {
  int i;
  uint32_t address = 0x4000;
  char errMsg[256];
  int pageNumber = getPageForAddress(address);
  uint32_t startEraseAddress = (400 * pageNumber);
    
  setupFakeSerial();
  
  sendPacket(FLASH_PAGE_ERASE, NULL, 0);
  handlePacketStateMachine(FLASH_PAGE_ERASE);
  
  for(i = 0; i < K; i++) {
    sprintf(errMsg, "Flash at address %d is not properly erased.", i);
    TEST_ASSERT_EQUAL_MESSAGE(0xff, flashMemory[i], errMsg);
  } 
}

void test_setPageArray_should_set_specific_page_after_page_erase(void){
  int pageNumber = 20;
  int pageInfo = 0;
  
  setPageArray(pageNumber);
  pageInfo = hasPageBeenErased(pageNumber);
  
  TEST_ASSERT_EQUAL(1, pageInfo);
}

void test_hasPageBeenErased_page_have_not_been_erased_should_return_0(void){
  int pageNumber = 0;
  int pageInfo = 1;
  
  pageInfo = hasPageBeenErased(pageNumber);
  
  TEST_ASSERT_EQUAL(0, pageInfo);  	
}

void test_hasPageBeenErased_page_has_been_erased_should_return_1(void){
  int pageNumber = 20;
  int pageInfo = 0;
  
  pageInfo = hasPageBeenErased(pageNumber);
  
  TEST_ASSERT_EQUAL(1, pageInfo);  	
}

void test_initFlashPageInfo_should_initailize_all_page_to_0(void){
  uint8_t expectedPageArray[128] = {0};
  uint8_t *PageInfo;
  PageInfo = malloc(128);
  
  initFlashPageInfo();
  for(int i = 0; i != 128; i++){
	int pageNumber = i;
    PageInfo[i] = hasPageBeenErased(pageNumber);
  }  
  
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedPageArray, PageInfo, 128);
  free (PageInfo);
}

void test_getPageForAddress_to_input_specific_address_should_return_page_number(void){
  uint32_t address = 0x0800A52D;
  int pageNumber;
  
  pageNumber = getPageForAddress(address);
  
  TEST_ASSERT_EQUAL(41, pageNumber);
}



















