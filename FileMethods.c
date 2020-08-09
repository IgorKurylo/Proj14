//
// Created by Igork on 30/05/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileMethods.h"

FILE *readFile(char *fileName) {

    FILE *asmFile = NULL;
    asmFile = fopen(fileName, "r");
    if (asmFile != NULL) {
        return asmFile;
    }
    fclose(asmFile);
    return NULL;
}

void fileNameToOpen(char *fileName, char *fileExtension, char **fileToOpen) {
    *fileToOpen = (char *) malloc(sizeof(char*) * strlen(fileName) * strlen(fileExtension));
    if (fileToOpen != NULL) {
        strcpy(*fileToOpen, fileName);
        strcat(*fileToOpen, fileExtension);
    }
}


AsmFileContent *fileContent(FILE *file, int *fileLines) {
    AsmFileContent *asmFileContent;
    asmFileContent = (AsmFileContent *) malloc(sizeof(AsmFileContent) * (MAX_LINES_FILE));
    if (asmFileContent != NULL) {
        while (fgets(asmFileContent[*fileLines].line, MAX_LINE_SIZE, file) != NULL) {
            asmFileContent[*fileLines].lineSize = (int) strlen(asmFileContent->line);
            (*fileLines)++;
        }
    } else {
        printf("Allocation fail, program exit\n");
        exit(-1);
    }
    return asmFileContent;

}

void writeEntryFile(SymbolTable *tableArray, int size, char *fileName) {
    int i = 0;
    char *fileToOpen=NULL;
    FILE *filePtr;
//    fileNameToOpen(fileName,ENTRY_FILE,&fileToOpen);
//
    strcat(fileName,ENTRY_FILE);
    filePtr=fopen(fileName,"w");
    if (filePtr != NULL) {
        for (i = 0; i < size; i++) {
            if (tableArray[i].is_entry) {
                fprintf(filePtr, "%s\t%07d\n", tableArray[i].name, tableArray[i].address);
            }
        }
        //fclose(filePtr);
    }

}

void writeExternFile(char **externalsLabelsArray, int size, char *fileName) {
    int i = 0;
    char *fileToOpen=NULL;
    FILE *filePtr;
    fileNameToOpen(fileName,EXTERNAL_FILE,&fileToOpen);
    filePtr=fopen(fileToOpen,"w");
    if (filePtr != NULL) {
        for (i = 0; i < size; i++) {
            if (strcmp(externalsLabelsArray[i], "") != 0) {
                fprintf(filePtr, "%s\t%07d\n", externalsLabelsArray[i], i);
            }
        }
        fclose(filePtr);
    }
}

char *getFileName(char *file) {
    char *fileName;
    int index = 0;
    fileName = (char *) malloc(sizeof(char) * strlen(file));
    if (fileName != NULL) {
        while (*file != '.') {
            fileName[index++] = *file;
            file++;
        }
    }
    fileName[index] = 0;
    return fileName;

}