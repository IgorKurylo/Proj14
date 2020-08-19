//
// Created by Igork on 30/05/2020.
//
#include "Constanst.h"
#ifndef PROJECTMAMAN14_DATASTRUCTERS_H
typedef struct {
    int opCode;
    int funct;
    int numParams;
} commandMetaData;

typedef struct {
    char *key;
    commandMetaData value;
} Command;

typedef struct {
    char line[MAX_LINE_SIZE];
    int lineNumber;
    int lineSize;
    int isLabel;
} AsmFileContent;

#endif //PROJECTMAMAN14_DATASTRUCTERS_H
