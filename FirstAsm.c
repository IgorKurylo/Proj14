//
// Created by Igork on 13/06/2020.
//

#include "DataStructers.h"
#include "HelpersMethods.h"
#include "SymbolTable.h"
#include <stdio.h>
#include <stdlib.h>



void firstRead(AsmFileContent asmContentFile,int *IC,int *lengthOfWords,int *DC,int lineNumber){
    char *labelName,*command,*directive;
    int addressType=-1;
    int numberOfLine=lineNumber+1;
    SymbolTable  row={};
    if(allocateTable()!=NULL) {
        if (isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
            return;
        }
        if(isExternEntryDirective(asmContentFile.line),&labelName){
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
            if (parseCommand(asmContentFile.line, &command, lineNumber, &addressType, IC, DC)) {
                printf("[Line %d ] Command - > %s\n", numberOfLine, command);
                row.type=code;
                row.address=*IC;
                //TODO: calculate length of words, watch video
            }
            printf("[Line %d]  Label : %s \n", numberOfLine, labelName);
            addSymbol(row,lineNumber);
        }

        if (parseDirective(asmContentFile.line, &directive, lineNumber)) {
            printf("[Line %d ] Directive - > %s\n", numberOfLine, directive);
        }


    }
    //TDB Check commands,data,string,extern,entry.

}