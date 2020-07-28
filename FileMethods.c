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
    return NULL;
}

FILE *openFile(char *fileName, char *fileExtension) {
    FILE *file = NULL;
    char *fileToOpen = NULL;
    fileToOpen = (char *) malloc(sizeof(char) * strlen(fileName) * strlen(fileExtension));
    if (fileToOpen != NULL) {
        strcpy(fileToOpen, fileName);
        fileToOpen = strcat(fileToOpen, fileExtension);
    }
    file = fopen(fileToOpen, "w");
    if (file != NULL) {
        free(fileToOpen);
        return file;
    }
    return NULL;
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
    FILE *file = NULL;
    file = openFile(fileName, ENTRY_FILE);
    if (file != NULL) {
        for (i = 0; i < size; i++) {
            if (table[i].is_entry) {

                fprintf(file, "%s\t%04d\n", table[i].name, table[i].address);
            }
        }
        fclose(file);
    }
}

void writeExternFile(SymbolTable *tableArray, int size, char *fileName) {
    int i = 0;
    FILE *file = NULL;
    file = openFile(fileName, EXTERNAL_FILE);
    if (file != NULL) {
        for (i = 0; i < size; i++) {
            if (tableArray[i].is_extern) {
                fprintf(file, "%s\t%04d\n", tableArray[i].name, tableArray[i].address);
            }
        }
        fclose(file);
    }
}