//
// Created by Igork on 30/05/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileMethods.h"
FILE *readFile(char *fileName){

    FILE *asmFile=NULL;
    asmFile=fopen(fileName,"r");
    if(asmFile!=NULL){
        return asmFile;
    }
    return NULL;
}
int writeFile(char *buffer,char *fileName,char *format){
    FILE *file=NULL;
    file=fopen(fileName,"w");
    if(file!=NULL){
        fprintf(file,format,buffer);
    }else{
        return 0;
    }
    fclose(file);
    return 1;

}
AsmFileContent *fileContent(FILE *file,int *fileLines)
{
    AsmFileContent *asmFileContent;
    asmFileContent=(AsmFileContent*)malloc(sizeof(AsmFileContent)*(MAX_LINES_FILE));
    if(asmFileContent!=NULL){
        while (fgets(asmFileContent[*fileLines].line, MAX_LINE_SIZE, file) != NULL){
            asmFileContent[*fileLines].lineSize=(int)strlen(asmFileContent->line);
            (*fileLines)++;
        }
    }else{
        printf("Allocation fail, program exit\n");
        exit(-1);
    }
    return asmFileContent;

}