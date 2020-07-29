//
// Created by Igork on 14/07/2020.
//

#include "Constanst.h"
#include <string.h>
#include "MemorySnapShot.h"
#include "HelpersMethods.h"
#include <stdio.h>
#include <stdlib.h>

// Array for data snap shot saving & current size
int dataSnapShotMemory[MAX_DATA];
int dataSize;
// Array of machine code memory saving & current size
MachineCode *machineCode;
int machineCodeSize = 1;


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
                    convertExtraValueToMachineCode(value, -1, -1);
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
                    convertExtraValueToMachineCode(dataSnapShotMemory[counterOfData], -1, -1);
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


void initMachineMemoryCode() {
    machineCode = (MachineCode *) malloc(sizeof(MachineCode) * machineCodeSize);
    if (machineCode == NULL) {
        printf("[ERROR] Machine code allocation fail\n");
        return;
    } else {
        printf("[INFO] Machine code allocated successfully\n");
    }
}

MachineCode *resizeMachineMemoryCode() {
    MachineCode *machineCodeTemp;
    machineCodeTemp = (MachineCode *) realloc(machineCode, sizeof(MachineCode) * (machineCodeSize + 1));
    if (machineCodeTemp != NULL) {
        machineCode = machineCodeTemp;
    }
    return machineCode;
}

MachineCode
convertInstructionToMachineCode(int opcode, int funct, int sourceOperand, int sourceAddressType, int destOperand,
                                int destAddressType) {
    if (machineCode == NULL) {
        initMachineMemoryCode();
    } else {
        machineCode = resizeMachineMemoryCode();
    }
    MachineCode codeBlock = {};
    codeBlock.data.instructions.opCode = opcode;
    codeBlock.data.instructions.funct = funct;
    codeBlock.data.instructions.srcRegister = sourceOperand;
    codeBlock.data.instructions.srcAddress = sourceAddressType;
    codeBlock.data.instructions.destRegister = destOperand;
    codeBlock.data.instructions.destAddress = destAddressType;
    codeBlock.are = absolute;
    machineCode[machineCodeSize - 1] = codeBlock;
    machineCodeSize++;
    return codeBlock;
}

MachineCode convertExtraValueToMachineCode(int value, int addressType, int isLabelExternal) {
    if (machineCode == NULL) {
        initMachineMemoryCode();

    } else {
        machineCode = resizeMachineMemoryCode();
    }
    MachineCode extraWord = {};
    extraWord.data.extraWord.value = value;
    if (addressType != -1) {
        if (addressType == DIRECT_ADDRESSING) {
            if (isLabelExternal != -1) {
                extraWord.are = isLabelExternal ? external : relocatable;
            }
        } else if (addressType == IMMEDIATE_ADDRESSING || addressType == RELATIVE_ADDRESSING) {
            extraWord.are = absolute;
        }
    }
    machineCode[machineCodeSize - 1] = extraWord;
    machineCodeSize++;
    return extraWord;

}


