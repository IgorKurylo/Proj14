//
// Created by Igork on 13/06/2020.
//

#include "DataStructers.h"
#include "HelpersMethods.h"
#include "SymbolTable.h"
#include <stdio.h>
#include <stdlib.h>



void firstRead(AsmFileContent asmContentFile,int *IC,int *DC,int lineNumber){
    char *labelName,*command,*directiveData;
    int addressType=-1,directiveType=0;
    int numberOfLine=lineNumber+1;
    SymbolTable  row={};
    if(allocateTable()!=NULL) {
        if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
            return;
        }
        if(isExternEntryDirective(asmContentFile.line,&labelName)){
            row.name=labelName;
            row.address=0;
            row.is_extern=1;
            if(addSymbol(row,lineNumber)){
                printf("[Line %d] .extern with operand %s added to table",lineNumber,labelName);
            }

        }
        if (parseLabel(asmContentFile.line, &labelName, numberOfLine)) {
            if(!checkIfSymbolExists(labelName)){
                row.name=labelName;
            }
            if (parseCommand(asmContentFile.line, &command, lineNumber, IC)) {
                printf("[Line %d ] Command - > %s\n", numberOfLine, command);
                row.type=code;
                row.address=*IC; // ic come already calculation. ;
            }
            printf("[Line %d]  Label : %s \n", numberOfLine, labelName);
            addSymbol(row,lineNumber);
            if (parseDirective(asmContentFile.line, &directiveData, lineNumber,&directiveType)) {
                populateDataDirective(DC,directiveType,directiveData);
            }
        }
        else if (parseCommand(asmContentFile.line, &command, lineNumber, IC)) {
        }

    }
    //TDB Check commands,data,string,extern,entry.

}