/* Created by Igork on 30/05/2020.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileMethods.h"


/* Read file function*/

FILE *readFile(char *fileName, char *extension) {

    FILE *asmFile = NULL;
    asmFile = fopen(strcat(fileName, extension), "r");
    if (asmFile != NULL) {
        return asmFile;
    } else {
        return NULL;

    }
}

/* Read file and save it on asm file content struct*/

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

/*write entry file*/

void writeEntryFile(SymbolTable *tableArray, int size, char *fileName) {
    int i = 0, counter = 0;
    FILE *filePtr;
    strcat(fileName, ENTRY_FILE);
    for (i = 0; i < size; i++) {
        if (tableArray[i].is_entry)
            counter++;
    }
    if (counter > 0) {
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

}

/*write extern file*/

void writeExternFile(char **externalsLabelsArray, int size, char *fileName) {
    int i = 0;
    FILE *filePtr;
    strcat(fileName, EXTERNAL_FILE);
    if (externalLabels != NULL) {
        filePtr = fopen(fileName, "w");
        if (filePtr != NULL) {
            for (i = 0; i < size; i++) {

                if (strcmp(externalLabels[i], "") > 0) {
                    fprintf(filePtr, "%s\t%07d\n", externalLabels[i], INIT_ADDRESS + i);
                }
            }
            fclose(filePtr);
            printf("[INFO] File %s successfully created\n", fileName);
        }

    }
}

/* get file name*/

char *getFileName(char *file) {
    char *fileName = NULL;
    int index = 0;
    if (strchr(file, '.') != NULL) { /* if the argument pass with extension*/
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

/* write machine code to file*/

void writeMachineCodeFile(int IC, int DC, const int *machine_code, char *fileName) {
    int index = 0, Value = 0;
    FILE *filePtr;
    char *file = fileName;
    strcat(fileName, OBJECT_FILE);
    filePtr = fopen(fileName, "w");
    fprintf(filePtr, "%7d\t%d\n", IC, DC);
    for (index = 0; index < machineCodeSize && index < (IC + DC); index++) {
        Value = machine_code[index];
        fprintf(filePtr, "%07d\t%06x\n", index + INIT_ADDRESS, Value);
    }
    printf("[INFO] File %s successfully created\n", fileName);
    fclose(filePtr);

}

void freeFileContent(AsmFileContent *asmFileContent) {
    if (asmFileContent != NULL) {
        free(asmFileContent);
    }
}