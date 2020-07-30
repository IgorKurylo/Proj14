//
// Created by Igork on 19/07/2020.
//

#include "FileMethods.h"
#include "HelpersMethods.h"
#include "SymbolTable.h"
#include "MemorySnapShot.h"

// second read of the asm file
int secondRead(AsmFileContent asmContentFile, int *IC, int lineNumber) {

    char *labelOperand, *command, *operands, *firstOperand, *secondOperand;
    int errorCounter = 0, directiveType = 0, symbolIndex = 0, numOfOperands = 0, isExternLabel = 0, destAddressType = -1, srcAddressType = -1, operandType = -1, destOffset = 0, srcOffset = 0;
    SymbolTable row;
    MachineCode instructionCounter;
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
        } else {
            printf("[FATAL] line %d: %s symbol not exists in symbol table\n", lineNumber, labelOperand);
            return (++errorCounter);
        }
    }
    if ((parseCommand(asmContentFile.line, &command, lineNumber, &numOfOperands, &errorCounter,
                      &operands))) {
        switch (numOfOperands) {
            case 1:
                parseOneOperand(operands, &firstOperand);
                if (firstOperand != NULL) {
                    if (validateOperand(firstOperand, &destAddressType, lineNumber, &errorCounter, 0, &operandType)) {
                        symbolIndex = checkIfSymbolExists(firstOperand, lineNumber);
                        if (symbolIndex != -1) {
                            destOffset = calculateOffsetAddress(destAddressType);
                            row = getTableRow(symbolIndex);
                            if (row.is_extern) {
                                instructionCounter = getInstructionCounter(lineNumber, destOffset);
                                row.address = INIT_ADDRESS + instructionCounter.data.instructionCounter.IC +
                                              instructionCounter.data.instructionCounter.wordLength;

                            }
                        } else {
                            printf("[ERROR] line %d: %s symbol not exists in table\n", lineNumber, firstOperand);
                        }
                    }
                }
                break;

            case 2:
                parseTwoOperands(operands, &firstOperand, &secondOperand);
                if (firstOperand != NULL) {
                    if (validateOperand(firstOperand, &srcAddressType, lineNumber, &errorCounter, 0, &operandType)) {
                        symbolIndex = checkIfSymbolExists(firstOperand, lineNumber);
                        if (symbolIndex != -1) {
                            destOffset = calculateOffsetAddress(srcAddressType);
                            row = getTableRow(symbolIndex);
                            if (row.is_extern) {
                                instructionCounter = getInstructionCounter(lineNumber, destOffset);
                                row.address = INIT_ADDRESS + instructionCounter.data.instructionCounter.IC +
                                              instructionCounter.data.instructionCounter.wordLength;
                            }
                        } else {
                            printf("[ERROR] line %d: %s symbol not exists in table\n", lineNumber, firstOperand);
                        }
                    }
                }
                if (secondOperand != NULL) {
                    if (validateOperand(secondOperand, &destAddressType, lineNumber, &errorCounter, 0, &operandType)) {
                        symbolIndex = checkIfSymbolExists(firstOperand, lineNumber);
                        if (symbolIndex != -1) {
                            destOffset = calculateOffsetAddress(destAddressType);
                            row = getTableRow(symbolIndex);
                            if (row.is_extern) {
                                instructionCounter = getInstructionCounter(lineNumber, destOffset);
                                row.address = INIT_ADDRESS + instructionCounter.data.instructionCounter.IC +
                                              instructionCounter.data.instructionCounter.wordLength;
                            }
                        } else {
                            printf("[ERROR] line %d: %s symbol not exists in table\n", lineNumber, firstOperand);
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