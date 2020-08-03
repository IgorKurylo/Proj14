//
// Created by Igork on 14/07/2020.
//

#ifndef PROJ14_MEMORYSNAPSHOT_H
#define PROJ14_MEMORYSNAPSHOT_H

#include "Constanst.h"

enum ARE {
    absolute = 4, relocatable = 2, external = 1
};

typedef struct {
    unsigned int opCode: 6;
    unsigned int srcRegister: 3;
    unsigned int srcAddress: 2;
    unsigned int destRegister: 3;
    unsigned int destAddress: 2;
    unsigned int funct: 5;
    unsigned int are: 3;

} instructions;

typedef struct {
    unsigned int dataValue:24;
} data;
typedef struct {
    unsigned int are:3;
    unsigned int extraWord:21;
} extraWord;
typedef struct {
    instructions instructions_t;
    data *dataValues;
} MachineCode;



extern int dataSnapShotMemory[MAX_DATA];
extern int dataSize;
extern MachineCode *machineCode;
extern int machineCodeSize;

int *saveToSnapShotMemory(char *data, int directiveType, int *DC, int *deltaCounter, int *errorCounter, int lineNumber);

MachineCode*
convertInstructionToMachineCode(unsigned int command, unsigned int funct, unsigned int sourceOperand, unsigned int sourceAddressType, unsigned int destOperand,
                                unsigned int destAddressType);

MachineCode *
convertExtraValueToMachineCode(MachineCode *code, int index, unsigned int value, unsigned int addressType, int isLabelExternal);

void updateMachineCode(int address, int offset, int IsExternalSymbol);

MachineCode getInstructionCounter(int offset);

void addDataToMachineCode(unsigned int value);

#endif //PROJ14_MEMORYSNAPSHOT_H
