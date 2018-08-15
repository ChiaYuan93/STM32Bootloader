#ifndef _STRTONUMBER_H
#define _STRTONUMBER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

FILE *openHexFile(char *fileHandler);
char *readLine(FILE *fileHandler);
int verifyHexLine(char *line);
uint8_t *convertHexLineToStrOfNum(char *line);
int getHexLineLength(uint8_t *strOfNum);
int getHexLineAddress(uint8_t *strOfNum);
int getHexLineCode(uint8_t *strOfNum);
uint8_t *getHexLineDataByte(uint8_t *strOfNum, int *length);
void freeMemory(void *ptr);

#endif // _STRTONUMBER_H
