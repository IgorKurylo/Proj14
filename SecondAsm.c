//
// Created by Igork on 19/07/2020.
//

#include "FileMethods.h"
#include "HelpersMethods.h"
#include "SymbolTable.h"

// second read of the asm file
int secondRead(AsmFileContent asmContentFile, int *IC, int lineNumber) {

    char *labelOperand;
    int errorCounter = 0, directiveType = 0, symbolIndex = 0;
    if(isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)){
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
    //TODO: run and search for label and update the machine code by labels addresses

    return errorCounter;

}