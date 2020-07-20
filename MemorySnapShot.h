//
// Created by Igork on 14/07/2020.
//

#ifndef PROJ14_MEMORYSNAPSHOT_H
#define PROJ14_MEMORYSNAPSHOT_H

#include "Constanst.h"
#include "DataStructers.h"


extern int dataSnapShotMemory[MAX_DATA];
extern int dataSize;
extern MachineCode *machineCode;
int *saveToSnapShotMemory(char *data,int directiveType,int *DC,int *deltaCounter,int *errorCounter,int lineNumber);
MachineCode convertInstructionToMachineCode(int command,int funct,int sourceOperand,int sourceAddressType,int destOperand,int destAddressType);
MachineCode convertExtraValueToMachineCode(int value, int addressType, int isLabelExternal);

#endif //PROJ14_MEMORYSNAPSHOT_H
