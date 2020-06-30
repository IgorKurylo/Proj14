//
// Created by Igork on 13/06/2020.
//

#include "DataStructers.h"
#include "HelpersMethods.h"
#include "SymbolTable.h"
#include <stdio.h>
#include <stdlib.h>


void firstRead(AsmFileContent asmContentFile, int *IC, int *DC, int lineNumber) {
    char *labelName, *command, *directiveData;
    int addressType = -1, directiveType = 0;
    int numberOfLine = lineNumber + 1;
    int flagData=0;
    SymbolTable row = {};

    if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
        return;
    }
    if (isExternDirective(asmContentFile.line)) {
        row.name = parseLabel(asmContentFile.line,&labelName,numberOfLine);
        row.address = 0;
        row.is_extern = 1;
        if (addSymbol(row, lineNumber)) {
            printf("[Line %d] .extern with operand %s added to table", lineNumber, labelName);
        }
    }
    if (parseLabel(asmContentFile.line, &labelName, numberOfLine)) {
        // add a label
        row.name = labelName;
        row.address = *IC;
        if (parseCommand(asmContentFile.line, &command, lineNumber, IC)) {
            printf("[Line %d] Command: %s \n", numberOfLine, command);
            row.type = code;
            // ic come already calculation;
        }
        printf("[Line %d] Label: %s \n", numberOfLine, labelName);
        if(!addSymbol(row, lineNumber)){
            printf("[Line %d] label %s exists in symbol table \n",numberOfLine,labelName);
        }
        if (parseDirective(asmContentFile.line, &directiveData, lineNumber, &directiveType)) {
            populateDataDirective(DC,IC,directiveType, directiveData);
            flagData=1;
        }
    } else if (parseCommand(asmContentFile.line, &command, lineNumber, IC)) {
        //DEBUG IC calculation
        printf("[Line %d] Command: %s \n", numberOfLine, command);

    }

    //TDB Check commands,data,string,extern,entry.

}