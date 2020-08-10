//
// Created by Igork on 30/05/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileMethods.h"

FILE *readFile(char *fileName, char *extension) {

    FILE *asmFile = NULL;
    asmFile = fopen(strcat(fileName, extension), "r");
    if (asmFile != NULL) {
        return asmFile;
    } else {
        return NULL;

    }
}


AsmFileContent *fileContent(FILE *file, int *fileLines) {
    AsmFileContent *asmFileContent;
    asmFileContent = (AsmFileContent *) malloc(sizeof(AsmFileContent) * (MAX_LINES_FILE));
    if (asmFileContent != NULL) {
        while (fgets(asmFileContent[*fileLines].line, MAX_LINE_SIZE, file) != NULL) {
            asmFileContent[*fileLines].lineSize = (int) strlen(asmFileContent[*fileLines].line);
            (*fileLines)++;
        }
    } else {
        printf("Allocation fail, program exit\n");
        exit(-1);
    }
    fclose(file);
    return asmFileContent;

}

void writeEntryFile(SymbolTable *tableArray, int size, char *fileName) {
    int i = 0;
    FILE *filePtr;
    strcat(fileName, ENTRY_FILE);
    filePtr = fopen(fileName, "w");
    if (filePtr != NULL) {
        for (i = 0; i < size; i++) {
            if (tableArray[i].is_entry) {
                fprintf(filePtr, "%s\t%07d\n", tableArray[i].name, tableArray[i].address);
            }
        }
        fclose(filePtr);
        printf("[INFO] File %s successfully created\n", fileName);
    }

}

void writeExternFile(char **externalsLabelsArray, int size, char *fileName) {
    int i = 0;
    FILE *filePtr;
    strcat(fileName, EXTERNAL_FILE);
    filePtr = fopen(fileName, "w");
    if (filePtr != NULL) {
        for (i = 0; i < size; i++) {

            if (externalsLabelsArray[i] != NULL) {
                fprintf(filePtr, "%s\t%07d\n", externalsLabelsArray[i], INIT_ADDRESS + i);
            }

        }
        fclose(filePtr);
        printf("[INFO] File %s successfully created\n", fileName);
    }
}

char *getFileName(char *file) {
    char *fileName = NULL;
    int index = 0;
    if (strchr(file, '.') != NULL) { // if the argument pass with extension
        fileName = (char *) malloc(sizeof(char) * strlen(file));
        if (fileName != NULL) {
            while (*file != '.') {
                fileName[index++] = *file;
                file++;
            }
        }
        fileName[index] = 0;
        return fileName;
    } else {
        return file;
    }
}

void writeMachineCodeFile(int IC, int DC, MachineCode *machine_code, char *fileName) {
    FILE *filePtr;
    strcat(fileName, OBJECT_FILE);
    filePtr = fopen(fileName, "w");
    fprintf(filePtr, "%5d\t%d\n", IC, DC);
}