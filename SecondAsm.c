//
// Created by Igork on 19/07/2020.
//

#include "FileMethods.h"
#include "HelpersMethods.h"
#include "SymbolTable.h"
#include "MemorySnapShot.h"
#include <string.h>


void buildMachineCodeOneOperand(const int *IC, int lineNumber, char *firstOperand,
                                int labelAddress, int isDistanceLabel, int symbolIndex, const int *destAddressType,
                                const int *operandType, int *destOffset, HashMap command);

void buildMachineCodeDirective(int lineNumber, char *data, int directiveType, int *errorCounter);

void adaptOffsetsByAddressType(int destAddressType, int srcAddressType, int *srcOffset, int *destOffset);

void updateExternalLabelAddress(const int *IC, int srcOffset, int destOffset, int isSrcExternalLabel,
                                int isDestExternalLabel, char *firstOperand, char *secondOperand);

// second read of the asm file
int secondRead(AsmFileContent asmContentFile, int *IC, int lineNumber) {

    char *labelOperand, *command, *operands, *firstOperand, *secondOperand, *directiveData;
    int errorCounter = 0, labelDestAddress = 0, labelSrcAddress = 0, distanceOfJmpCommands = 0, isDistanceLabel = 0, regDest = -1, regSrc = -1, isSrcExternalLabel = 0, isDestExternalLabel = 0,
            directiveType = 0, symbolIndex = 0, numOfOperands = 0, destAddressType = -1, srcAddressType = -1, operandDestType = -1, operandSrcType = -1, destOffset = 0, srcOffset = 0, valueSrc, valueDest;
    HashMap commandObj;
    if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
        return 0;
    }
    if (isExternDirective(asmContentFile.line, &labelOperand, &errorCounter)) {
        return errorCounter;
    }
    if (parseDirective(asmContentFile.line, &directiveData, lineNumber, &directiveType, &errorCounter)) {
        if (directiveType == DATA_DIRECTIVE || directiveType == STRING_DIRECTIVE)
            buildMachineCodeDirective(lineNumber, directiveData, directiveType, &errorCounter);
        return errorCounter;
    }
    if (isEntryDirective(asmContentFile.line, &labelOperand)) {
        symbolIndex = checkIfSymbolExists(labelOperand, lineNumber);
        if (symbolIndex != -1) {
            updateIsEntrySymbol(symbolIndex);
            return 0;
        } else {
            printf("[ERROR] line %d: %s symbol not exists in symbol table\n", lineNumber, labelOperand);
            return (++errorCounter);
        }
    }
    if ((parseCommand(asmContentFile.line, &command, lineNumber, &numOfOperands, &errorCounter,
                      &operands))) {


        commandObj = commandOpCode_functCode(command);
        switch (numOfOperands) {
            case 0:
                ++(*IC);
                saveInstruction(commandObj.value.opCode, commandObj.value.funct, 0, 0, 0, 0);
                break;
            case 1:
                if (isJmpCommand(command)) {
                    destAddressType = RELATIVE_ADDRESSING;
                    destOffset = calculateOffsetAddress(destAddressType);
                }
                parseOneOperand(operands, &firstOperand);
                if (firstOperand != NULL) {
                    if (validateOperand(firstOperand, &destAddressType, lineNumber, &errorCounter, &valueDest,
                                        &operandDestType)) {
                        buildMachineCodeOneOperand(IC, lineNumber, firstOperand, labelDestAddress,
                                                   isDistanceLabel, symbolIndex,
                                                   &destAddressType, &operandDestType, &destOffset,
                                                   commandObj);


                    }
                }
                if ((destAddressType) == REGISTER_TYPE) {
                    (destOffset) = 0;
                }
                *IC += destOffset + 1;

                break;
            case 2:
                parseTwoOperands(operands, &firstOperand, &secondOperand);
                if (firstOperand != NULL) {
                    if (validateOperand(firstOperand, &srcAddressType, lineNumber, &errorCounter, &valueSrc,
                                        &operandSrcType)) {
                        if (operandSrcType == 2) { // if operand is label
                            symbolIndex = checkIfSymbolExists(firstOperand, lineNumber);
                            if (symbolIndex != -1) {
                                labelSrcAddress = table[symbolIndex].address;
                                if (table[symbolIndex].is_extern && labelSrcAddress == 0) {
                                    isSrcExternalLabel = table[symbolIndex].is_extern;
                                }

                            } else {
                                printf("[ERROR] line %d: %s not found on symbol table", lineNumber, firstOperand);
                            }
                        }
                        srcOffset = calculateOffsetAddress(srcAddressType);
                    }
                }
                if (secondOperand != NULL) {
                    if (validateOperand(secondOperand, &destAddressType, lineNumber, &errorCounter, &valueDest,
                                        &operandDestType)) {
                        if (operandDestType == 2) { // if operand is label
                            symbolIndex = checkIfSymbolExists(secondOperand, lineNumber);
                            if (symbolIndex != -1) {
                                labelDestAddress = table[symbolIndex].address;
                                if (table[symbolIndex].is_extern && labelDestAddress == 0) {
                                    isDestExternalLabel = table[symbolIndex].is_extern;
                                }
                            } else {
                                printf("[ERROR] line %d: %s not found on symbol table", lineNumber, firstOperand);
                            }
                        }
                        destOffset = calculateOffsetAddress(destAddressType);
                    }
                }

                if (operandSrcType == register_operand && operandDestType == label_operand) {
                    regSrc = isRegister(firstOperand);
                    saveInstruction(commandObj.value.opCode, commandObj.value.funct, regSrc,
                                    srcAddressType, 0, 0);
                    saveWord(labelDestAddress, destAddressType, isSrcExternalLabel);
                }
                if (operandSrcType == label_operand && operandDestType == register_operand) {
                    regDest = isRegister(secondOperand);
                    saveInstruction(commandObj.value.opCode, commandObj.value.funct, 0,
                                    0, regDest, destAddressType);
                    saveWord(labelSrcAddress, srcAddressType, isSrcExternalLabel);
                }
                if (operandSrcType == label_operand && operandDestType == number_operand) {
                    saveInstruction(commandObj.value.opCode, commandObj.value.funct, 0,
                                    0, 0, 0);
                    saveWord(labelSrcAddress, destAddressType, isSrcExternalLabel);
                    saveWord(convertTo2Complement(valueDest), destAddressType, isSrcExternalLabel);
                }
                if (operandSrcType == register_operand && operandDestType == register_operand) {
                    regSrc = isRegister(firstOperand);
                    regDest = isRegister(secondOperand);
                    saveInstruction(commandObj.value.opCode, commandObj.value.funct, regSrc,
                                    srcAddressType, regDest, destAddressType);
                }
                if (operandSrcType == label_operand && operandDestType == label_operand) {
                    saveInstruction(commandObj.value.opCode, commandObj.value.funct, 0,
                                    0, 0, 0);
                    saveWord(labelSrcAddress, srcAddressType, isSrcExternalLabel);
                    saveWord(labelDestAddress, destAddressType, isDestExternalLabel);
                }
                if (operandSrcType == number_operand && operandDestType == register_operand) {
                    regDest = isRegister(secondOperand);
                    saveInstruction(commandObj.value.opCode, commandObj.value.funct, 0,
                                    0, regDest, destAddressType);
                    saveWord(convertTo2Complement(valueSrc), srcAddressType, -1);
                }
                adaptOffsetsByAddressType(destAddressType, srcAddressType, &srcOffset, &destOffset);
                updateExternalLabelAddress(IC, srcOffset, destOffset, isSrcExternalLabel, isDestExternalLabel,
                                           firstOperand, secondOperand);
                *IC += srcOffset + destOffset + 1;
                break;
            default:
                break;
        }
        return errorCounter;
    }

}

void updateExternalLabelAddress(const int *IC, int srcOffset, int destOffset, int isSrcExternalLabel,
                                int isDestExternalLabel, char *firstOperand, char *secondOperand) {
    if (isSrcExternalLabel && isDestExternalLabel) {
        addExternalLabel(*IC + srcOffset, firstOperand);
        addExternalLabel(*IC + srcOffset + destOffset, firstOperand);
    }
    if (isSrcExternalLabel) {
        addExternalLabel(*IC + srcOffset, firstOperand);
    }
    if (isDestExternalLabel) {
        addExternalLabel(*IC + destOffset, secondOperand);
    }
}

void adaptOffsetsByAddressType(int destAddressType, int srcAddressType, int *srcOffset, int *destOffset) {

    if (srcAddressType == REG_ADDRESSING && destAddressType == REG_ADDRESSING) {
        *destOffset = 0;
        *srcOffset = 0;

    } else if (srcAddressType == REG_ADDRESSING && destAddressType != REG_ADDRESSING) {
        *srcOffset = 0;
    } else if (srcAddressType != REG_ADDRESSING && destAddressType == REG_ADDRESSING) {
        *destOffset = 0;
    }
}


void buildMachineCodeOneOperand(const int *IC, int lineNumber, char *firstOperand,
                                int labelAddress, int isDistanceLabel, int symbolIndex, const int *destAddressType,
                                const int *operandType, int *destOffset, HashMap command) {
    int regDest = -1, distanceOfJmpCommands = -1, value = 0;
    if ((*operandType) == label_operand) {
        if (strchr(firstOperand, '&')) {
            firstOperand++;
            isDistanceLabel = 1;
        }
        symbolIndex = checkIfSymbolExists(firstOperand, lineNumber);
        if (symbolIndex == -1) {
            printf("[ERROR] line %d: %s not found on symbol table", lineNumber, firstOperand);
        } else {
            labelAddress = table[symbolIndex].address;
            (*destOffset) = calculateOffsetAddress((*destAddressType));
            saveInstruction(command.value.opCode, command.value.funct, 0, 0, 0, *destAddressType);
            if (isDistanceLabel) {
                distanceOfJmpCommands = labelAddress - *IC;
                //build machine code with this distance value
                saveWord(distanceOfJmpCommands, *destAddressType,
                         table[symbolIndex].is_extern);
            } else {
                saveWord(labelAddress, *destAddressType,
                         table[symbolIndex].is_extern);
            }
            // build machine code with extra value
        }
    } else if ((*operandType) == register_operand) {
        // is register , save instruction with register
        regDest = isRegister(firstOperand);
        saveInstruction(command.value.opCode, command.value.funct, 0, 0, regDest, *destAddressType);
    } else if ((*operandType) == number_operand) {
        saveInstruction(command.value.opCode, command.value.funct, 0, 0, 0, 0);
        saveWord(convertTo2Complement(value), *destAddressType, table[symbolIndex].is_extern);
    }

}

void buildMachineCodeDirective(int lineNumber, char *data, int directiveType, int *errorCounter) {
    char *numberStr;
    int value = 0, i = 0;
    switch (directiveType) {

        case DATA_DIRECTIVE:
            numberStr = strtok(data, DELIM);
            while (numberStr) {
                if (numberValidation(numberStr, &value, lineNumber, errorCounter)) {
                    saveData(convertTo2Complement(value));
                    numberStr = strtok(NULL, DELIM);
                }
            }
            break;
        case STRING_DIRECTIVE:
            if (stringValidation(&data, lineNumber, errorCounter)) {
                while (i <= strlen(data)) {
                    value = (int) data[i];
                    saveData(convertTo2Complement(value));
                    i++;
                }

            }
            break;
        default:
            break;
    }
}



