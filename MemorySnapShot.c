/* Created by Igork on 30/05/2020.*/


#include "Constanst.h"
#include <string.h>
#include "MemorySnapShot.h"
#include "HelperMethods.h"
#include <stdio.h>
#include <stdlib.h>

/* Array for Data snap shot saving & current size*/
int dataSnapShotMemory[MAX_DATA];
int dataSize;
/* Array of machine code memory saving & current size*/
int machineCodeSize = 0;
/* Array of external Labels */
char **externalLabels;
int *memoryCodeArray;

int *
saveToSnapShotMemory(char *data, int directiveType, int *DC, int *deltaCounter, int *errorCounter, int lineNumber) {
    char *numberStr = NULL;
    int i = 0;
    int counterOfData = *DC;
    int value = 0;
    switch (directiveType) {
        case DATA_DIRECTIVE:
            numberStr = strtok(data, DELIM);
            while (numberStr) {
                if (numberValidation(numberStr, MEMORY_WORD_SIZE, &value, lineNumber, errorCounter)) {
                    dataSnapShotMemory[counterOfData] = value;
                    ++counterOfData;
                    numberStr = strtok(NULL, DELIM);
                } else {
                    numberStr = strtok(NULL, DELIM);
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


int *initMachineMemoryCode(int sizeOfMachineCode) {
    memoryCodeArray = (int *) malloc(sizeof(int) * sizeOfMachineCode);
    if (memoryCodeArray == NULL) {
        printf("[ERROR] Machine code allocation fail\n");
        return NULL;
    } else {
        printf("[INFO] Machine code allocated successfully\n");
        return memoryCodeArray;
    }
}


void
saveInstruction(unsigned int opcode, unsigned int funct, unsigned int sourceOperand,
                unsigned int sourceAddressType, unsigned int destOperand,
                unsigned int destAddressType) {
    int index = machineCodeSize;
    int *machine_code_value;
    Instruction instruction = {0};
    instruction.opCode = opcode;
    instruction.funct = funct;
    instruction.srcRegister = sourceOperand;
    instruction.srcAddress = sourceAddressType;
    instruction.destRegister = destOperand;
    instruction.destAddress = destAddressType;
    instruction.are = absolute;
    machine_code_value = (int *) &instruction;
    memoryCodeArray[index] = *machine_code_value;
    machineCodeSize++;
}

void
saveWord(unsigned int value, unsigned int addressType,
         int isLabelExternal) {
    int index = 0, *value_word;
    index = machineCodeSize;
    Word word = {0};
    if (addressType != -1) {
        if (addressType == DIRECT_ADDRESSING) {
            if (isLabelExternal != -1) {
                word.are = isLabelExternal ? external : relocatable;
            }
        } else if (addressType == IMMEDIATE_ADDRESSING || addressType == RELATIVE_ADDRESSING) {
            word.are = absolute;
        }
    }
    word.extraWord = value;
    value_word = (int *) &word;
    memoryCodeArray[index] = *value_word;
    machineCodeSize++;

}

void saveData(unsigned int value) {
    int index = 0, *data_value;
    index = machineCodeSize;
    Data data = {0};
    data.dataValue = value;
    data_value = (int *) &data;
    memoryCodeArray[index] = *data_value;
    machineCodeSize++;
}

char **initExternalLabels(int sizeOfIC) {
    int i = 0;
    externalLabels = (char **) malloc(sizeOfIC * sizeof(char *));
    if (externalLabels != NULL) {
        for (i = 0; i < sizeOfIC; i++) {
            externalLabels[i] = (char *) malloc(MAX_SYMBOL_SIZE + 1);
        }
        return externalLabels;

    } else {
        return NULL;
    }
}

void addExternalLabel(int index, char *label) {
    if (externalLabels != NULL) {
        strcpy(externalLabels[index], label);
    }
}

void freeMemory() {
    if (memoryCodeArray != NULL) {
        machineCodeSize = 0;
        free(memoryCodeArray);
    }
    if (externalLabels != NULL) {
        free(externalLabels);
    }
}

