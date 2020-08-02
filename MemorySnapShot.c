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
                    //addDataToMachineCode(dataSnapShotMemory[counterOfData]);
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
    machineCodeSize++;
    machineCodeTemp = (MachineCode *) realloc(machineCode, sizeof(MachineCode) * (machineCodeSize));
    if (machineCodeTemp != NULL) {
        machineCode = machineCodeTemp;
    }
    return machineCode;
}

MachineCode *
convertInstructionToMachineCode(unsigned int opcode, unsigned int funct, unsigned int sourceOperand,
                                unsigned int sourceAddressType, unsigned int destOperand,
                                unsigned int destAddressType, int IC, int wordLength) {
    int index=machineCodeSize-1;
    if (machineCode == NULL) {
        initMachineMemoryCode();
    } else {
        machineCode = resizeMachineMemoryCode();
    }

    machineCode[index].data.instructions.opCode = opcode;
    machineCode[index].data.instructions.funct = funct;
    machineCode[index].data.instructions.srcRegister = sourceOperand;
    machineCode[index].data.instructions.srcAddress = sourceAddressType;
    machineCode[index].data.instructions.destRegister = destOperand;
    machineCode[index].data.instructions.destAddress = destAddressType;
    machineCode[index].are = absolute;
    machineCode[index].IC = IC;
    machineCode[index].wordLength = wordLength;
    //machineCode[machineCodeSize - 1].data.extraWord.value = 0; // init extra world to 0

    return &machineCode[machineCodeSize];
}

MachineCode *
convertExtraValueToMachineCode(MachineCode *code, int index, unsigned int value, unsigned int addressType,
                               int isLabelExternal) {

    if (machineCode != NULL && code != NULL) {
        (*code).extraWordValue = value;
        if (addressType != -1) {
            if (addressType == DIRECT_ADDRESSING) {
                if (isLabelExternal != -1) {
                    (*code).are = isLabelExternal ? external : relocatable;
                }
            } else if (addressType == IMMEDIATE_ADDRESSING || addressType == RELATIVE_ADDRESSING) {
                (*code).are = absolute;
            }
            machineCode[index] = *code;
        }
    }
    return &machineCode[index];

}

void addDataToMachineCode(unsigned int value) {
    if (machineCode == NULL) {
        initMachineMemoryCode();
    } else {
        resizeMachineMemoryCode();
    }
    if (machineCode != NULL) {
        MachineCode codeBlockData = {};
        codeBlockData.extraWordValue = value;
        machineCode[machineCodeSize-1] = codeBlockData;
    }

}

MachineCode getInstructionCounter(int address) {
    MachineCode instructionCounter = machineCode[address];
    return instructionCounter;
}

void updateMachineCode(int address, int offset, int IsExternalSymbol) {
    machineCode[offset].extraWordValue = address;
    if (IsExternalSymbol) {
        machineCode[offset].are = external;
    }
}
