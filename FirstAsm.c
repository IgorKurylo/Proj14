//
// Created by Igork on 13/06/2020.
//

#include "HelpersMethods.h"
#include "SymbolTable.h"
#include <stdio.h>
#include "FileMethods.h"

void addSymbolInTable(char *label, int type, int address, int lineNumber, int *errorCounter) {
    SymbolTable row = {};
    int result = 0;
    row.name = label;
    row.address = address;
    row.type = type;
    row.is_extern = type == symbol_external;
    result = addSymbol(row, lineNumber);
    if (!result) {
        (*errorCounter)++;
    }
}

int firstRead(AsmFileContent asmContentFile, int *IC, int *DC, int lineNumber) {
    char *labelName, *command, *directiveData, *operands;
    int numberOfLine = lineNumber, directiveType = 0, errorsCounter = 0, rowType = 0, numOfOperands, result = -1;
    SymbolTable row = {};
    if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
        return 0;
    }
    //parse if is entry directive and skip this
    if (isEntryDirective(asmContentFile.line,&labelName)) {
        return 0;
    }
    //parse if is extern directive
    if (isExternDirective(asmContentFile.line, &labelName, &errorsCounter)) {
        row.name = labelName;
        row.address = 0;
        row.is_extern = 1;
        rowType = symbol_external;
        addSymbolInTable(labelName, rowType, 0, numberOfLine + 1, &errorsCounter);
        return errorsCounter;
    }
    // parse label
    if (parseLabel(asmContentFile.line, &labelName, numberOfLine + 1, &errorsCounter)) {
        asmContentFile.isLabel = 1;
    }
    // parse directive .data/.string
    if (parseDirective(asmContentFile.line, &directiveData, numberOfLine + 1, &directiveType, &errorsCounter)) {
        rowType = symbol_data;
        if (asmContentFile.isLabel) {
            addSymbolInTable(labelName, rowType, INIT_ADDRESS + *IC + *DC, numberOfLine + 1, &errorsCounter);
        }
        populateDataDirective(DC, directiveType, directiveData, &errorsCounter, numberOfLine + 1);
        return errorsCounter;
    } else if (parseCommand(asmContentFile.line, &command, numberOfLine + 1, &numOfOperands, &errorsCounter,
                            &operands)) { // parse command
        rowType = symbol_code;
        if (asmContentFile.isLabel) {
            addSymbolInTable(labelName, rowType, INIT_ADDRESS + *IC, numberOfLine + 1, &errorsCounter);
        }
        result = parseOperands(operands, command, numOfOperands, numberOfLine + 1, IC, &errorsCounter);
        if (!result) {
            printf("[ERROR] line %d: Error on parse operands from %s command\n", numberOfLine + 1, command);
            errorsCounter++;
        }
    }

    return errorsCounter;

}

