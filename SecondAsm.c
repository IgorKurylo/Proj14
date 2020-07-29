//
// Created by Igork on 19/07/2020.
//

#include "FileMethods.h"
#include "HelpersMethods.h"
#include "SymbolTable.h"

// second read of the asm file
int secondRead(AsmFileContent asmContentFile, int *IC, int lineNumber) {

    char *labelOperand, *command, *operands, *firstOperand, *secondOperand;
    int errorCounter = 0, directiveType = 0, symbolIndex = 0, numOfOperands = 0,isExternLabel=0;
    SymbolTable row;
    if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
        return 0;
    }
    if (isExternDirective(asmContentFile.line, &labelOperand, &errorCounter)) {
        isExternLabel=1;
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
                    symbolIndex = checkIfSymbolExists(firstOperand, lineNumber);
                    if (symbolIndex != -1) {
                        row=getTableRow(symbolIndex);
                        if(row.is_extern){
                            //TODO need to update the address
                        }

                    } else {
                        printf("[ERROR] line %d: %s symbol not exists in table\n", lineNumber, firstOperand);
                    }
                }
                break;

            case 2:
                parseTwoOperands(operands,&firstOperand,&secondOperand);
                if (isAlphaNumeric(firstOperand)){
                    symbolIndex = checkIfSymbolExists(firstOperand, lineNumber);
                    if(symbolIndex!=-1){
                        row=getTableRow(symbolIndex);
                        if(row.is_extern){
                            //TODO need to update the address
                        }
                    }else{
                        printf("[ERROR] line %d: %s symbol not exists in table\n", lineNumber, firstOperand);
                    }
                }
                else if (isAlphaNumeric(secondOperand)){
                    symbolIndex = checkIfSymbolExists(firstOperand, lineNumber);
                    if(symbolIndex!=-1){
                        row=getTableRow(symbolIndex);
                        if(row.is_extern){
                            //TODO need to update the address
                        }
                    }else{
                        printf("[ERROR] line %d: %s symbol not exists in table\n", lineNumber, firstOperand);
                    }
                }
                break;
            default:
                break;
        }
    }
    //TODO: run and search for label and update the machine code by labels addresses

    return errorCounter;

}