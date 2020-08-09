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
    unsigned int funct: 5;
    unsigned int destRegister: 3;
    unsigned int destAddress: 2;
    unsigned int srcRegister: 3;
    unsigned int srcAddress: 2;
    unsigned int opCode: 6;
} Instruction;

typedef struct {
    unsigned int dataValue: 24;
} Data;
typedef struct {
    unsigned int are: 3;
    unsigned int extraWord: 21;
} Word;
typedef struct {
    union {
        Instruction instruction_object;
        Word extraWord;
        Data data;
    };
} MachineCode;


extern int dataSnapShotMemory[MAX_DATA];
extern int dataSize;
extern MachineCode *machineCode;
extern int machineCodeSize;
extern char** externalLabels;

int *saveToSnapShotMemory(char *data, int directiveType, int *DC, int *deltaCounter, int *errorCounter, int lineNumber);

void
saveInstruction(unsigned int command, unsigned int funct, unsigned int sourceOperand, unsigned int sourceAddressType,
                unsigned int destOperand,
                unsigned int destAddressType);

void saveWord(unsigned int value, unsigned int addressType,
              int isLabelExternal);

void saveData(unsigned int value);

MachineCode *initMachineMemoryCode(int sizeOfMachineCode);

char **initExternalLabels(int sizeOfIC);
void addExternalLabel(int IC, char *label);


#endif //PROJ14_MEMORYSNAPSHOT_H
