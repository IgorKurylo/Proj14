//
// Created by Igork on 14/07/2020.
//

#ifndef PROJ14_MEMORYSNAPSHOT_H
#define PROJ14_MEMORYSNAPSHOT_H

#include "Constanst.h"

/* check is empty line*/
enum ARE {
    absolute = 4, relocatable = 2, external = 1
};
/* check is empty line*/
typedef struct {
    unsigned int are: 3;
    unsigned int funct: 5;
    unsigned int destRegister: 3;
    unsigned int destAddress: 2;
    unsigned int srcRegister: 3;
    unsigned int srcAddress: 2;
    unsigned int opCode: 6;
} Instruction;
/* check is empty line*/
typedef struct {
    unsigned int dataValue: 24;
} Data;
/* check is empty line*/
typedef struct {
    unsigned int are: 3;
    unsigned int extraWord: 21;
} Word;



/* check is empty line*/
extern int dataSnapShotMemory[MAX_DATA];
/* check is empty line*/
extern int dataSize;
/* check is empty line*/
extern int machineCodeSize;
/* check is empty line*/
extern char **externalLabels;
/* check is empty line*/
extern int *memoryCodeArray;
/* check is empty line*/
int *saveToSnapShotMemory(char *data, int directiveType, int *DC, int *deltaCounter, int *errorCounter, int lineNumber);
/* check is empty line*/
void
saveInstruction(unsigned int command, unsigned int funct, unsigned int sourceOperand, unsigned int sourceAddressType,
                unsigned int destOperand,
                unsigned int destAddressType);
/* check is empty line*/
void saveWord(unsigned int value, unsigned int addressType,
              int isLabelExternal);
/* check is empty line*/
void saveData(unsigned int value);
/* check is empty line*/
int *initMachineMemoryCode(int sizeOfMachineCode);
/* check is empty line*/
char **initExternalLabels(int sizeOfIC);
/* check is empty line*/
void addExternalLabel(int IC, char *label);


#endif //PROJ14_MEMORYSNAPSHOT_H
