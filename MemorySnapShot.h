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
    unsigned int are: 3;
    int IC: 7;
    int wordLength: 2;
    unsigned int extraWordValue: 21;
    union {
        struct {
            unsigned int funct: 5;
            unsigned int opCode: 6;
            unsigned int srcRegister: 3;
            unsigned int destRegister: 3;
            unsigned int srcAddress: 2;
            unsigned int destAddress: 2;
        } instructions;
    } data;
} MachineCode;


extern int dataSnapShotMemory[MAX_DATA];
extern int dataSize;
extern MachineCode *machineCode;
extern int machineCodeSize;

int *saveToSnapShotMemory(char *data, int directiveType, int *DC, int *deltaCounter, int *errorCounter, int lineNumber);

MachineCode*
convertInstructionToMachineCode(unsigned int command, unsigned int funct, unsigned int sourceOperand, unsigned int sourceAddressType, unsigned int destOperand,
                                unsigned int destAddressType, int IC, int wordLength);

MachineCode *
convertExtraValueToMachineCode(MachineCode *code, int index, unsigned int value, unsigned int addressType, int isLabelExternal);

void updateMachineCode(int address, int offset, int IsExternalSymbol);

MachineCode getInstructionCounter(int offset);

void addDataToMachineCode(unsigned int value);

#endif //PROJ14_MEMORYSNAPSHOT_H
