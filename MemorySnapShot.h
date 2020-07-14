//
// Created by Igork on 14/07/2020.
//

#ifndef PROJ14_MEMORYSNAPSHOT_H
#define PROJ14_MEMORYSNAPSHOT_H

#include "Constanst.h"

extern int dataSnapShotMemory[MAX_DATA];
extern int dataSize;
int *saveToSnapShotMemory(char *data,int directiveType,int *DC,int *deltaCounter,int *errorCounter,int lineNumber);

#endif //PROJ14_MEMORYSNAPSHOT_H
