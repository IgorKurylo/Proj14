//
// Created by Igork on 19/07/2020.
//

#include "FileMethods.h"
#include "HelpersMethods.h"
#include "SymbolTable.h"
#include "MemorySnapShot.h"
#include <string.h>

void
updateAddressesOfSymbol(int lineNumber, const char *firstOperand, int symbolIndex, int srcAddressType, int *destOffset,
                        SymbolTable *row, MachineCode *instructionCounter, int addressInstructionCounter);

void buildMachineCodeOneOperand(const int *IC, int lineNumber, char *firstOperand,
                                int labelAddress, int isDistanceLabel, int symbolIndex, int *destAddressType,
                                int *operandType, int *errorCounter, int *destOffset, HashMap command);

void updateExternSymbol(const int *IC, int labelAddress, int symbolIndex);

// second read of the asm file
int secondRead(AsmFileContent asmContentFile, int *IC, int lineNumber) {

    char *labelOperand, *command, *operands, *firstOperand, *secondOperand;
    int errorCounter = 0, labelAddress = 0, distanceOfJmpCommands = 0, isDistanceLabel = 0, regDest = -1,
            directiveType = 0, symbolIndex = 0, numOfOperands = 0, destAddressType = -1, srcAddressType = -1, operandType = -1, destOffset = 0, srcOffset = 0, valueSrc, valueDest;
    SymbolTable row;
    MachineCode instructionCounter;
    HashMap commandObj;
    if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
        return 0;
    }
    if (isExternDirective(asmContentFile.line, &labelOperand, &errorCounter)) {
        return errorCounter;
    }
    if (parseDirective(asmContentFile.line, &labelOperand, lineNumber, &directiveType, &errorCounter)) {
        if (directiveType == DATA_DIRECTIVE || directiveType == STRING_DIRECTIVE)
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
            case 1:
                parseOneOperand(operands, &firstOperand);
                if (firstOperand != NULL) {

                    buildMachineCodeOneOperand(IC, lineNumber, firstOperand, labelAddress,
                                               isDistanceLabel, symbolIndex,
                                               &destAddressType, &operandType, &errorCounter, &destOffset,
                                               commandObj);

                    if ((destAddressType) == REGISTER_TYPE) {
                        (destOffset) = 0;
                    }
                    *IC += destOffset + 1;

                }
                break;
            case 2:
                parseTwoOperands(operands, &firstOperand, &secondOperand);
                if (firstOperand != NULL) {
                    if (validateOperand(firstOperand, &srcAddressType, lineNumber, &errorCounter, 0, &operandType)) {
                        if (operandType == 2) { // if operand is label
                            symbolIndex = checkIfSymbolExists(firstOperand, lineNumber);
                            if (symbolIndex == -1) {
                                printf("[ERROR] line %d: %s not found on symbol table", lineNumber, firstOperand);
                            }
                        }
                    }
                }
                if (secondOperand != NULL) {
                    if (validateOperand(secondOperand, &destAddressType, lineNumber, &errorCounter, 0, &operandType)) {
                        if (operandType == 2) { // if operand is label
                            symbolIndex = checkIfSymbolExists(secondOperand, lineNumber);
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
    return errorCounter;

}

void buildMachineCodeOneOperand(const int *IC, int lineNumber, char *firstOperand,
                                int labelAddress, int isDistanceLabel, int symbolIndex, int *destAddressType,
                                int *operandType, int *errorCounter, int *destOffset, HashMap command) {
    int regDest = -1, distanceOfJmpCommands = -1;
    MachineCode *code;
    if (validateOperand(firstOperand, destAddressType, lineNumber, errorCounter, 0, operandType)) {
        if ((*operandType) == 2) {
            if (strchr(firstOperand, '&')) {
                firstOperand++;
                isDistanceLabel = 1;
            }
            symbolIndex = checkIfSymbolExists(firstOperand, lineNumber);
            if (symbolIndex == -1) {
                printf("[ERROR] line %d: %s not found on symbol table", lineNumber, firstOperand);
            } else {
                labelAddress = table[symbolIndex].address;
                updateExternSymbol(IC, labelAddress, symbolIndex);
                (*destOffset) = calculateOffsetAddress((*destAddressType));
                code = saveInstruction(command.value.opCode, command.value.funct, 0, 0, 0, *destAddressType);
                if (isDistanceLabel) {
                    distanceOfJmpCommands = labelAddress - *IC;
                    //build machine code with this distance value
                    saveWord(code, machineCodeSize - 1, distanceOfJmpCommands, *destAddressType,
                             table[symbolIndex].is_extern, 1, 0);
                } else {
                    saveWord(code, machineCodeSize - 1, labelAddress, *destAddressType,
                             table[symbolIndex].is_extern, 1, 0);
                }
                // build machine code with extra value
            }
        } else if ((*operandType) == 0) {
            // is register , save instruction with register
            regDest = isRegister(firstOperand);
            saveInstruction(command.value.opCode, command.value.funct, 0, 0, regDest, *destAddressType);
        }

    }
}

void updateExternSymbol(const int *IC, int labelAddress, int symbolIndex) {
    if (labelAddress == 0 && table[symbolIndex].is_extern) {
        table[symbolIndex].address = *IC;
    }
}

