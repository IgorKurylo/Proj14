//
// Created by Igork on 14/07/2020.
//

#ifndef PROJ14_MEMORYSNAPSHOT_H
#define PROJ14_MEMORYSNAPSHOT_H

#include "Constanst.h"
#include "DataStructers.h"
extern int dataSnapShotMemory[MAX_DATA];
extern int dataSize;
extern MachineMemory *machineMemory;
int *saveToSnapShotMemory(char *data,int directiveType,int *DC,int *deltaCounter,int *errorCounter,int lineNumber);
MachineMemory convertInstructionToMachineCode(int command,int funct,int sourceOperand,int sourceAddressType,int destOperand,int destAddressType);

#endif //PROJ14_MEMORYSNAPSHOT_H
