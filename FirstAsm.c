//
// Created by Igork on 13/06/2020.
//
#include <stdio.h>
#include "SymbolTable.h"
#include "HelpersMethods.h"

/*add symbol to symbols table*/
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

/* first read function, calculate IC , DC, create symbol table*/
int firstRead(AsmFileContent asmContentFile, int *IC, int *DC, int lineNumber) {
    char *labelName = NULL, *command = NULL, *directiveData = NULL, *operands = NULL;
    int numberOfLine = lineNumber, directiveType = 0, errorsCounter = 0, rowType = 0, numOfOperands, result = -1;
    SymbolTable row = {};
    if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
        return 0;
    }
    /*parse if is entry directive and skip this*/
    if (isEntryDirective(asmContentFile.line, &labelName, lineNumber, &errorsCounter)) {
        return 0;
    }
    /*parse if is extern directive*/
    if (isExternDirective(asmContentFile.line, &labelName, &errorsCounter, lineNumber)) {
        if (isSymbolExists(labelName) == -1) {
            row.name = labelName;
            row.address = 0;
            row.is_extern = 1;
            rowType = symbol_external;
            addSymbolInTable(labelName, rowType, 0, numberOfLine + 1, &errorsCounter);

        } else {
            printf("[ERROR] line %d: %s can't be local label\n", lineNumber, labelName);
            errorsCounter++;
        }
        return errorsCounter;
    }
    /* parse label*/
    if (parseLabel(asmContentFile.line, &labelName, numberOfLine + 1, &errorsCounter)) {
        if (labelName == NULL) {
            return errorsCounter;
        }
        asmContentFile.isLabel = 1;
    }
    /* parse directive .data/.string */
    if (parseDirective(asmContentFile.line, &directiveData, numberOfLine + 1, &directiveType, &errorsCounter)) {
        /* populate data directive */

        /*add label to symbol table*/
        if (labelName != NULL) {
            rowType = symbol_data;
            if (asmContentFile.isLabel) {
                addSymbolInTable(labelName, rowType, *DC, numberOfLine + 1, &errorsCounter);
            }
        }
        if (directiveData != NULL) {
            populateDataDirective(DC, directiveType, directiveData, &errorsCounter, numberOfLine + 1);
        }
        return errorsCounter;
        /*parse command */
    } else if (parseCommand(asmContentFile.line, &command, numberOfLine + 1, &numOfOperands, &errorsCounter,
                            &operands)) { // parse command
        rowType = symbol_code;
        if (asmContentFile.isLabel) {
            addSymbolInTable(labelName, rowType, INIT_ADDRESS + *IC, numberOfLine + 1, &errorsCounter);
        }
        /*parse operands*/
        result = parseOperands(operands, command, numOfOperands, numberOfLine + 1, IC, &errorsCounter);
        if (!result) {
            printf("[ERROR] line %d: Error on parse operands from %s command\n", numberOfLine + 1, command);
            errorsCounter++;
        }
    }
    return errorsCounter;

}

