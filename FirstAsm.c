//
// Created by Igork on 13/06/2020.
//

#include "DataStructers.h"
#include "HelpersMethods.h"
#include "SymbolTable.h"
#include <stdio.h>
#include <stdlib.h>


int firstRead(AsmFileContent asmContentFile, int *IC,  int *DC, int lineNumber) {
    char *labelName, *command, *directiveData;
    int numberOfLine = lineNumber,directiveType = 0,errorsCounter=0,labelDataFlag=0;
    SymbolTable row = {};
    if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
        return 0;
    }
    //parse if is extern directive
    if (isExternDirective(asmContentFile.line,&errorsCounter)) {
        row.name = parseLabel(asmContentFile.line,&labelName,numberOfLine,&errorsCounter);
        row.address = 0;
        row.is_extern = 1;
        if (addSymbol(row, lineNumber)) {
            printf("[Line %d] .extern with operand %s added to table", numberOfLine+1, labelName);
        }else{
            errorsCounter++;
        }
    }
    // parse label
    if (parseLabel(asmContentFile.line, &labelName, numberOfLine+1,&errorsCounter)) {
        //add a label to symbol table
        row.name = labelName;
        row.address = INIT_ADDRESS + *IC;
        // ic come already calculation;
        labelDataFlag=1;
    }
    if (parseDirective(asmContentFile.line, &directiveData, lineNumber, &directiveType,&errorsCounter)) {
        populateDataDirective(DC,directiveType, directiveData,&errorsCounter);
        if(labelDataFlag) {
            row.address = INIT_ADDRESS +*IC+*DC;
            row.type = data;
        }
    }else {
        if (parseCommand(asmContentFile.line, &command, lineNumber, IC,&errorsCounter)) {
            if(labelDataFlag) {
                row.type = code;
                if(!addSymbol(row, lineNumber)){
                    errorsCounter++;
                    printf("[Line %d] label %s exists in symbol table \n",numberOfLine+1,labelName);
                }
            }
        }
    }
    return errorsCounter;

}