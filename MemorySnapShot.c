//
// Created by Igork on 14/07/2020.
//

#include "Constanst.h"
#include <string.h>
#include "MemorySnapShot.h"
#include "HelpersMethods.h"
#include <stdio.h>
#include <stdlib.h>

// Array for Data snap shot saving & current size
int dataSnapShotMemory[MAX_DATA];
int dataSize;
// Array of machine code memory saving & current size
MachineCode *machineCode;
int machineCodeSize = 0;

char **externalLabels;

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
                if (numberValidation(numberStr, &value, lineNumber, errorCounter)) {
                    dataSnapShotMemory[counterOfData] = value;
                    //addDataToMachineCode(value);
                    ++counterOfData;
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


MachineCode *initMachineMemoryCode(int sizeOfMachineCode) {
    machineCode = (MachineCode *) malloc(sizeof(MachineCode) * sizeOfMachineCode);
    if (machineCode == NULL) {
        printf("[ERROR] Machine code allocation fail\n");
        return NULL;
    } else {

        printf("[INFO] Machine code allocated successfully\n");
        return machineCode;
    }
}


void
saveInstruction(unsigned int opcode, unsigned int funct, unsigned int sourceOperand,
                unsigned int sourceAddressType, unsigned int destOperand,
                unsigned int destAddressType) {
    int index = machineCodeSize;
    machineCode[index].instruction_object.opCode = opcode;
    machineCode[index].instruction_object.funct = funct;
    machineCode[index].instruction_object.srcRegister = sourceOperand;
    machineCode[index].instruction_object.srcAddress = sourceAddressType;
    machineCode[index].instruction_object.destRegister = destOperand;
    machineCode[index].instruction_object.destAddress = destAddressType;
    machineCode[index].instruction_object.are = absolute;
    machineCodeSize++;
}

void
saveWord(unsigned int value, unsigned int addressType,
         int isLabelExternal) {
    int index = 0;
    index = machineCodeSize;
    if (addressType != -1) {
        if (addressType == DIRECT_ADDRESSING) {
            if (isLabelExternal != -1) {
                machineCode[index].extraWord.are = isLabelExternal ? external : relocatable;
            }
        } else if (addressType == IMMEDIATE_ADDRESSING || addressType == RELATIVE_ADDRESSING) {
            machineCode[index].extraWord.are = absolute;
        }
    }
    machineCode[index].extraWord.extraWord = value;
    machineCodeSize++;

}

void saveData(unsigned int value) {
    int index = 0;
    index = machineCodeSize;
    machineCode[index].data.dataValue = value;
    machineCodeSize++;
}


char **initExternalLabels(int sizeOfIC) {
    int index = 0;
    externalLabels = (char **) malloc(sizeof(char*) * sizeOfIC);
    if (externalLabels != NULL) {
        return externalLabels;
    } else {
        return NULL;
    }
}

void addExternalLabel(int IC, char *label) {
    if (externalLabels != NULL) {
        externalLabels[IC] = label;
    }
}


