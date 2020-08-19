//
// Created by Igork on 14/07/2020.
//

#ifndef PROJ14_MEMORYSNAPSHOT_H
#define PROJ14_MEMORYSNAPSHOT_H

#include "Constanst.h"

/* enum define ARE*/
enum ARE {
    absolute = 4, relocatable = 2, external = 1
};
/* struct define 24 bits of machine code of instruction*/
typedef struct {
    unsigned int are: 3;
    unsigned int funct: 5;
    unsigned int destRegister: 3;
    unsigned int destAddress: 2;
    unsigned int srcRegister: 3;
    unsigned int srcAddress: 2;
    unsigned int opCode: 6;
} Instruction;
/* struct define 24 bits of data*/
typedef struct {
    unsigned int dataValue: 24;
} Data;
/* struct define 21 bits of extra word snd 3 bits for are */
typedef struct {
    unsigned int are: 3;
    unsigned int extraWord: 21;
} Word;


/* array for hold data*/
extern int dataSnapShotMemory[MAX_DATA];
/* current size of data*/
extern int dataSize;
/* array for hold external labels*/
extern char **externalLabels;
/* current size of machine code*/
extern int machineCodeSize;
/* array for hold machine code*/
extern int *memoryCodeArray;

/* function for save inside data snap shot memory array*/
int *saveToSnapShotMemory(char *data, int directiveType, int *DC, int *deltaCounter, int *errorCounter, int lineNumber);

/* save instruction in machine code array*/
void
saveInstruction(unsigned int command, unsigned int funct, unsigned int sourceOperand, unsigned int sourceAddressType,
                unsigned int destOperand,
                unsigned int destAddressType);

/* save word in machine code array*/
void saveWord(unsigned int value, unsigned int addressType,
              int isLabelExternal);

/* save data in machine code array*/
void saveData(unsigned int value);

/* allocate machine memory array by size of machine code*/
int *initMachineMemoryCode(int sizeOfMachineCode);

/* init external labels by size of IC*/
char **initExternalLabels(int sizeOfIC);

/* add external label to array by ic and label*/
void addExternalLabel(int IC, char *label);


#endif //PROJ14_MEMORYSNAPSHOT_H
