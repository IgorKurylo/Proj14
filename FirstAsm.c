//
// Created by Igork on 13/06/2020.
//

#include "DataStructers.h"
#include "HelpersMethods.h"
#include <stdio.h>
#include <stdlib.h>



void firstRead(AsmFileContent asmContentFile,int *IC,int *DC,int lineNumber){
    char *labelName,*command,*directive;
    int addressType=-1;
    int numberOfLine=lineNumber+1;
    if(isComment(asmContentFile.line) || isEmptyLine(asmContentFile.line)) {
        return;
    }
    if(parseLabel(asmContentFile.line,&labelName,numberOfLine)){
        printf("[Line %d]  Label : %s \n",numberOfLine,labelName);
    }
    if(parseCommand(asmContentFile.line, &command,lineNumber,&addressType)){
        printf("[Line %d ] Command - > %s\n",numberOfLine,command);
    }
    if(parseDirective(asmContentFile.line,&directive,lineNumber)){
        printf("[Line %d ] Directive - > %s\n",numberOfLine,directive);
    }

    //TDB Check commands,data,string,extern,entry.

}