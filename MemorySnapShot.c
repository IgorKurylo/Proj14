//
// Created by Igork on 14/07/2020.
//

#include "Constanst.h"
#include <string.h>
#include "MemorySnapShot.h"
#include "HelpersMethods.h"
#include <stdio.h>


int dataSnapShotMemory[MAX_DATA];
int dataSize;

int *
saveToSnapShotMemory(char *data, int directiveType, int *DC, int *deltaCounter, int *errorCounter, int lineNumber) {
    char *numberStr = NULL;
    int i = 0;
    int counterOfData = *DC;
    char *delim = ",";
    int value = 0;
    switch (directiveType) {
        case DATA_DIRECTIVE:
            numberStr = strtok(data, delim);
            while (numberStr) {
                if (numberValidation(numberStr, &value, lineNumber, errorCounter)) {
                    dataSnapShotMemory[counterOfData] = value;
                    ++counterOfData;
                    numberStr = strtok(NULL, delim);

                }
            }
            *deltaCounter = counterOfData;
            break;
        case STRING_DIRECTIVE:
            if (stringValidation(&data, lineNumber, errorCounter)) {
                while (i <= strlen(data)) {
                    dataSnapShotMemory[counterOfData] = (int) data[i];
                    ++counterOfData;
                    i++;
                }
                *deltaCounter = counterOfData;
            }
            break;
        default:
            break;

    }
    *DC = counterOfData;
    dataSize += counterOfData;
    return dataSnapShotMemory;
}

MachineMemory convertInstructionToMachineCode(int opcode, int funct, int sourceOperand, int sourceAddressType, int destOperand,
                                int destAddressType, char *dataMemory) {

    MachineMemory memory = {};
    memory.data.instructions.opCode = opcode;
    memory.data.instructions.funct = funct;
    memory.data.instructions.srcRegister = sourceOperand;
    memory.data.instructions.srcAddress = sourceAddressType;
    memory.data.instructions.destRegister = destOperand;
    memory.data.instructions.destAddress = destAddressType;
    memory.are = 2;

    if (dataMemory != NULL) {
        printf("%s", dataMemory);
        memory.data.extraWord.value=(int)dataMemory;
    }
    return memory;
}


