//
// Created by Igork on 13/06/2020.
//

#include "DataStructers.h"
#include "HelpersMethods.h"
#include "SymbolTable.h"
#include <stdio.h>
#include <stdlib.h>


void firstRead(AsmFileContent asmContentFile, int *IC,  int *DC, int lineNumber,int *errorsCounter) {
    char *labelName, *command, *directiveData;
    int addressType = -1, directiveType = 0;
    int numberOfLine = lineNumber + 1;
    int flagData=0;
    int labelData=0;
    SymbolTable row = {};
    if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
        return;
    }
    //parse if is extern directive
    if (isExternDirective(asmContentFile.line)) {
        row.name = parseLabel(asmContentFile.line,&labelName,numberOfLine);
        row.address = 0;
        row.is_extern = 1;
        if (addSymbol(row, lineNumber)) {
            printf("[Line %d] .extern with operand %s added to table", lineNumber, labelName);
        }
    }
    // parse label
    if (parseLabel(asmContentFile.line, &labelName, numberOfLine)) {
        //add a label to symbol table
        row.name = labelName;
        row.address = INIT_ADDRESS + *IC;
        // ic come already calculation;
        labelData=1;
        if(!addSymbol(row, lineNumber)){
            *errorsCounter++;
            printf("[Line %d] label %s exists in symbol table \n",numberOfLine,labelName);
        }
    }
    if (parseDirective(asmContentFile.line, &directiveData, lineNumber, &directiveType)) {
        populateDataDirective(DC,directiveType, directiveData);
        if(labelData) {
            row.address = INIT_ADDRESS + *DC;
            row.type = data;
        }
    }else {
        if (parseCommand(asmContentFile.line, &command, lineNumber, IC)) {
            if(labelData) {
                row.type = code;
            }
        }
    }

    //TDB Check commands,data,string,extern,entry.

}