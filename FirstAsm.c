//
// Created by Igork on 13/06/2020.
//

#include "DataStructers.h"
#include "HelpersMethods.h"
#include "SymbolTable.h"
#include <stdio.h>
#include <stdlib.h>

void addSymbolInTable(char *label, int type, int address, int lineNumber, int *errorsCounter) {
    SymbolTable row = {};
    int result = 0;
    row.name = label;
    row.address = address;
    row.type = type;
    result = addSymbol(row, lineNumber);
    if (!result) {
        *errorsCounter++;
    }
}

int firstRead(AsmFileContent asmContentFile, int *IC, int *DC, int lineNumber) {
    char *labelName, *command, *directiveData;
    int numberOfLine = lineNumber, directiveType = 0, errorsCounter = 0, labelFlag = 0, rowType = 0;
    SymbolTable row = {};
    if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
        return 0;
    }
    //parse if is extern directive
    if (isExternDirective(asmContentFile.line, &errorsCounter)) {
        row.name = parseLabel(asmContentFile.line, &labelName, numberOfLine, &errorsCounter);
        row.address = 0;
        row.is_extern = 1;
        if (addSymbol(row, lineNumber)) {
            printf("[Line %d] .extern with operand %s added to table", numberOfLine + 1, labelName);
        } else {
            errorsCounter++;
        }
    }
    // parse label
    if (parseLabel(asmContentFile.line, &labelName, numberOfLine + 1, &errorsCounter)) {
        asmContentFile.isLabel=1;

    }
    // parse directive .data/.string
    if (parseDirective(asmContentFile.line, &directiveData, lineNumber, &directiveType, &errorsCounter)) {
        rowType = data;
        if(asmContentFile.isLabel){
            addSymbolInTable(labelName,rowType,INIT_ADDRESS+*IC+*DC,lineNumber,&errorsCounter);
        }
        populateDataDirective(DC, directiveType, directiveData, &errorsCounter,lineNumber);
    } else if (parseCommand(asmContentFile.line, &command, lineNumber, IC, &errorsCounter)) { // parse command
        rowType = code;
        if(asmContentFile.isLabel) {
            addSymbolInTable(labelName, rowType, INIT_ADDRESS + *IC, lineNumber, &errorsCounter);
        }
    }
    return errorsCounter;

}

