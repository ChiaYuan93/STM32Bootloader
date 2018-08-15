#include "strtonumber.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


char buffer[1028];

FILE *openHexFile(char *path){
  FILE *fileHandler ;
  fileHandler = fopen(path, "r");
  return fileHandler;
}

char *readLine(FILE *fileHandler){  
  return fgets(buffer, 1028, fileHandler);
}

 // sscanf each byte and sum them up.
 // it should be zero.
 // Return 1 if zero; otherwise 0
int verifyHexLine(char *line){
  int hex;
  int storeHex = 0;
  int i = 0;
  char temp;

  while(1){
    sscanf(&line[i+1], "%2c", &temp);
    
    if(line[i+1] != '\n'){
      sscanf(&line[i+1], "%2x", &hex);
      i = i + 2;
      storeHex = storeHex + hex;
    }
    else{
      if((storeHex & 0xff) == 0){
        return 1;
        break;
      }
      else{
        return 0;
        break;
      }
    }
  }
}

  // sscanf to convert ascii digits to numbers
  // and return the buffer containing the result
uint8_t *convertHexLineToStrOfNum(char *line){
  int length = 0;
  char temp;
  uint8_t *resultBuffer;
  
  while(1){
    sscanf(&line[length+1], "%2c", &temp);
    
    if(line[length+1] != '\n'){
      length = length + 2;
    }
    else{
      resultBuffer = malloc(length/2);
      break;
    }
  }
  for(int i=0;i<length;i+=2){
    sscanf(&line [i+1], "%2x", &resultBuffer[i/2]);
  }
  return resultBuffer;
}

  // return the first byte of data from strOfNum
int getHexLineLength(uint8_t *strOfNum){
  int lineLength = strOfNum[0];
  return lineLength;
}

  // return the 16-bit data at byte 1 and 2
int getHexLineAddress(uint8_t *strOfNum){
  int lineAddress = (strOfNum[1]<<8) + strOfNum[2];
  
  return lineAddress;
}
  // return the 8-bit data at byte 3
int getHexLineCode(uint8_t *strOfNum){
  int dataType = strOfNum[3];
  
  return dataType;
}
  // return the data bytes
uint8_t *getHexLineDataByte(uint8_t *strOfNum, int *length){
  int byte = 0;
  uint8_t *dataByte;
  
  *length = getHexLineLength(strOfNum);
  dataByte = malloc(*length);
  if(*length == 0){
    return NULL;
  }
  else{
    for(int i = 0; i != *length; i++){
      dataByte[i] = strOfNum[i+4];
    }
  return dataByte;   
  }    
}
  //free malloc if ptr is not NULL
void freeMemory(void *ptr){
  if(ptr != NULL){
    free(ptr);
  }
}

