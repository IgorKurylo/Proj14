
/* Created by Igork on 19/07/2020.*/

#include "HelperMethods.h"
#include "SymbolTable.h"
#include "MemorySnapShot.h"
#include <string.h>
#include "SecondAsm.h"


/* Second Read function which, parse labels,calculate distance of jmp,jre,bne commands , calculate IC and  build binary machine code */
int secondRead(AsmFileContent asmContentFile, int *IC, int lineNumber) {

    char *labelOperand = NULL, *command = NULL, *operands = NULL, *firstOperand = NULL, *secondOperand = NULL, *directiveData = NULL;
    int errorCounter = 0, labelDestAddress = 0, labelSrcAddress = 0, isDistanceLabel = 0, regDest = 0, regSrc = 0, isSrcExternalLabel = 0, isDestExternalLabel = 0,
            directiveType = 0, symbolIndex = 0, numOfOperands = 0, operandDestType = -1, operandSrcType = -1, destOffset = 0, srcOffset = 0, valueSrc = 0, valueDest = 0;
    int destAddressType = 0, srcAddressType = 0;
    Command commandObj;
    if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
        return 0;
    }
    if (isEntryDirective(asmContentFile.line, &labelOperand, lineNumber, &errorCounter)) {
        symbolIndex = isSymbolExists(labelOperand);
        if (symbolIndex != -1) {
            updateIsEntrySymbol(symbolIndex);
            return 0;
        } else {
            printf("[ERROR] line %d: %s symbol not exists in symbol table\n", lineNumber, labelOperand);
            return (++errorCounter);
        }
    }
    if (isExternDirective(asmContentFile.line, &labelOperand, &errorCounter, lineNumber)) {
        if (isSymbolExists(labelOperand) == -1) {
            printf("[ERROR] line %d: %s symbol not exists in symbol table\n", lineNumber, labelOperand);
            errorCounter++;
        }
        return errorCounter;
    }
    if (parseDirective(asmContentFile.line, &directiveData, lineNumber, &directiveType, &errorCounter)) {
        if (directiveType == DATA_DIRECTIVE || directiveType == STRING_DIRECTIVE)
            buildMachineCodeDirective(lineNumber, directiveData, directiveType, &errorCounter);
        return errorCounter;
    }

    if ((parseCommand(asmContentFile.line, &command, lineNumber, &numOfOperands, &errorCounter,
                      &operands))) {


        commandObj = getCommandByCommandName(command);
        switch (numOfOperands) {
            case 0:
                ++(*IC);
                saveInstruction(commandObj.value.opCode, commandObj.value.funct, 0, 0, 0, 0);
                break;
            case 1:
                parseOneOperand(operands, &firstOperand);
                if (isJmpCommand(command)) {
                    destAddressType = RELATIVE_ADDRESSING;
                    destOffset = calculateOffsetAddress(destAddressType);
                    if (strchr(firstOperand, '&')) {
                        firstOperand++;
                        isDistanceLabel = 1;
                    }
                }
                if (firstOperand != NULL) {
                    if (validateOperand(firstOperand, &destAddressType, lineNumber, &errorCounter, &valueDest,
                                        &operandDestType)) {
                        if (operandDestType == 2) { /* if operand is label */
                            symbolIndex = isSymbolExists(firstOperand);
                            if (symbolIndex != -1) {
                                labelDestAddress = table[symbolIndex].address;
                                if (table[symbolIndex].is_extern && labelDestAddress == 0) {
                                    isDestExternalLabel = table[symbolIndex].is_extern;
                                }

                            } else {
                                printf("[ERROR] line %d: %s not found on symbol table\n", lineNumber, firstOperand);
                            }
                        }
                        destOffset = calculateOffsetAddress(destAddressType);
                        if (isDestExternalLabel) {
                            updateExternalLabelAddress(*IC + destOffset, firstOperand);
                        }
                        if ((destAddressType) == REGISTER_TYPE) {
                            (destOffset) = 0;
                        }

                        *IC += destOffset + 1;


                        buildMachineCodeOneOperand(IC, lineNumber, firstOperand, labelDestAddress,
                                                   isDistanceLabel, symbolIndex,
                                                   &destAddressType, &operandDestType, &destOffset,
                                                   commandObj, valueDest);
                    }

                }
                break;
            case 2:
                parseTwoOperands(operands, &firstOperand, &secondOperand);
                if (firstOperand != NULL) {
                    if (validateOperand(firstOperand, &srcAddressType, lineNumber, &errorCounter, &valueSrc,
                                        &operandSrcType)) {
                        if (operandSrcType == 2) { /* if operand is label */
                            symbolIndex = isSymbolExists(firstOperand);
                            if (symbolIndex != -1) {
                                labelSrcAddress = table[symbolIndex].address;
                                if (table[symbolIndex].is_extern && labelSrcAddress == 0) {
                                    isSrcExternalLabel = table[symbolIndex].is_extern;
                                }

                            } else {
                                printf("[ERROR] line %d: %s not found on symbol table\n", lineNumber, firstOperand);
                            }
                        }

                        srcOffset = calculateOffsetAddress(srcAddressType);


                    }
                }
                if (secondOperand != NULL) {
                    if (validateOperand(secondOperand, &destAddressType, lineNumber, &errorCounter, &valueDest,
                                        &operandDestType)) {
                        if (operandDestType == 2) { /* if operand is label*/
                            symbolIndex = isSymbolExists(secondOperand);
                            if (symbolIndex != -1) {
                                labelDestAddress = table[symbolIndex].address;
                                if (table[symbolIndex].is_extern && labelDestAddress == 0) {
                                    isDestExternalLabel = table[symbolIndex].is_extern;
                                }
                            } else {
                                printf("[ERROR] line %d: %s not found on symbol table\n", lineNumber, secondOperand);
                            }
                        }
                        destOffset = calculateOffsetAddress(destAddressType);

                    }
                }
                adaptOffsetsByAddressType(destAddressType, srcAddressType, &srcOffset, &destOffset);
                calculateExternalLabelsAddresses(IC, firstOperand, secondOperand, isSrcExternalLabel,
                                                 isDestExternalLabel, destAddressType, srcAddressType,
                                                 destOffset, srcOffset);
                *IC += srcOffset + destOffset + 1;
                buildMachineCode_2Operands(firstOperand, secondOperand, labelDestAddress, labelSrcAddress, regDest,
                                           regSrc, isSrcExternalLabel,
                                           isDestExternalLabel, destAddressType, srcAddressType, operandDestType,
                                           operandSrcType, valueSrc, valueDest,
                                           &commandObj);
                break;
            default:
                break;
        }
        return errorCounter;
    }

}

void calculateExternalLabelsAddresses(const int *IC, char *firstOperand, char *secondOperand,
                                      int isSrcExternalLabel, int isDestExternalLabel, int destAddressType,
                                      int srcAddressType, int destOffset, int srcOffset) {
    if (srcAddressType == DIRECT_ADDRESSING && destAddressType == DIRECT_ADDRESSING) {
        if (isSrcExternalLabel && isDestExternalLabel) {
            updateExternalLabelAddress(*IC + srcOffset, firstOperand);
            updateExternalLabelAddress(*IC + destOffset + 1, secondOperand);
        }
    } else if (srcAddressType == DIRECT_ADDRESSING || destAddressType == DIRECT_ADDRESSING) {
        if (isSrcExternalLabel) {
            updateExternalLabelAddress(*IC + srcOffset + 1, firstOperand);
        } else if (isDestExternalLabel) {
            updateExternalLabelAddress(*IC + destOffset + 1, secondOperand);
        }
    }
}

void buildMachineCode_2Operands(char *firstOperand, char *secondOperand, int labelDestAddress, int labelSrcAddress,
                                int regDest, int regSrc, int isSrcExternalLabel, int isDestExternalLabel,
                                int destAddressType, int srcAddressType, int operandDestType, int operandSrcType,
                                int valueSrc, int valueDest, Command *commandObj) {
    if (operandSrcType == register_operand && operandDestType == label_operand) {
        regSrc = isRegister(firstOperand);
        saveInstruction((*commandObj).value.opCode, (*commandObj).value.funct, regSrc,
                        srcAddressType, regDest, destAddressType);
        saveWord(labelDestAddress, destAddressType, isSrcExternalLabel);
    }
    if (operandSrcType == label_operand && operandDestType == register_operand) {
        regDest = isRegister(secondOperand);
        saveInstruction((*commandObj).value.opCode, (*commandObj).value.funct, regSrc,
                        srcAddressType, regDest, destAddressType);
        saveWord(labelSrcAddress, srcAddressType, isSrcExternalLabel);
    }
    if (operandSrcType == label_operand && operandDestType == number_operand) {
        saveInstruction((*commandObj).value.opCode, (*commandObj).value.funct, regSrc,
                        srcAddressType, regDest, destAddressType);
        saveWord(labelSrcAddress, destAddressType, isSrcExternalLabel);
        saveWord(convertTo2Complement(valueDest), destAddressType, isSrcExternalLabel);
    }
    if (operandSrcType == register_operand && operandDestType == register_operand) {
        regSrc = isRegister(firstOperand);
        regDest = isRegister(secondOperand);
        saveInstruction((*commandObj).value.opCode, (*commandObj).value.funct, regSrc,
                        srcAddressType, regDest, destAddressType);
    }
    if (operandSrcType == label_operand && operandDestType == label_operand) {
        saveInstruction((*commandObj).value.opCode, (*commandObj).value.funct, regSrc,
                        srcAddressType, regDest, destAddressType);
        saveWord(labelSrcAddress, srcAddressType, isSrcExternalLabel);
        saveWord(labelDestAddress, destAddressType, isDestExternalLabel);
    }
    if (operandSrcType == number_operand && operandDestType == register_operand) {
        regDest = isRegister(secondOperand);
        saveInstruction((*commandObj).value.opCode, (*commandObj).value.funct, regSrc,
                        srcAddressType, regDest, destAddressType);
        saveWord(convertTo2Complement(valueSrc), srcAddressType, 0);
    }
}

/* update the symbol which is external by correct address*/
void updateExternalLabelAddress(int IC, char *operand) {
    addExternalLabel(IC, operand);
}

/* adapt offsets by addressing types*/
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

/* build machine code one operand*/
void buildMachineCodeOneOperand(const int *IC, int lineNumber, char *firstOperand,
                                int labelAddress, int isDistanceLabel, int symbolIndex, const int *destAddressType,
                                const int *operandType, int *destOffset, Command command, int valueDest) {
    int regDest = -1, distanceOfJmpCommands = -1, value = 0;
    if ((*operandType) == label_operand) {
        symbolIndex = isSymbolExists(firstOperand);
        if (symbolIndex == -1) {
            printf("[ERROR] line %d: %s not found on symbol table\n", lineNumber, firstOperand);
        } else {
            labelAddress = table[symbolIndex].address;
            (*destOffset) = calculateOffsetAddress((*destAddressType));
            saveInstruction(command.value.opCode, command.value.funct, 0, 0, 0, *destAddressType);
            if (isDistanceLabel) {
                distanceOfJmpCommands = labelAddress - (*IC + *destOffset + INIT_ADDRESS);
                /*build machine code with this distance value */
                value = convertTo2Complement(
                        distanceOfJmpCommands); /* convert to 2 complement the distance can be negative */
                saveWord(value, *destAddressType,
                         table[symbolIndex].is_extern);
            } else {
                saveWord(labelAddress, *destAddressType,
                         table[symbolIndex].is_extern);
            }

        }
    } else if ((*operandType) == register_operand) {
        /*is register , save instruction with register*/
        regDest = isRegister(firstOperand);
        saveInstruction(command.value.opCode, command.value.funct, 0, 0, regDest, *destAddressType);
    } else if ((*operandType) == number_operand) {
        saveInstruction(command.value.opCode, command.value.funct, 0, 0, 0, 0);
        saveWord(convertTo2Complement(valueDest), *destAddressType, table[symbolIndex].is_extern);
    }

}

/* build machine code on directive*/
void buildMachineCodeDirective(int lineNumber, char *data, int directiveType, int *errorCounter) {
    char *numberStr;
    int value = 0, i = 0;
    switch (directiveType) {

        case DATA_DIRECTIVE:
            numberStr = strtok(data, DELIM);
            while (numberStr) {
                if (numberValidation(numberStr, MEMORY_WORD_SIZE, &value, lineNumber, errorCounter)) {
                    saveData(convertTo2Complement(value));
                    numberStr = strtok(NULL, DELIM);
                } else {
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



