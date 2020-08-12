//
// Created by Igork on 30/05/2020.
//

#include <stdlib.h>
#include <stdio.h>
#include "Constanst.h"
#include "MemorySnapShot.h"
#include "SymbolTable.h"

#ifndef PROJECTMAMAN14_FILEMETHODS_H

typedef struct {
    char line[MAX_LINE_SIZE];
    int lineNumber;
    int lineSize;
    int isLabel;
} AsmFileContent;

#define ASM_FILE ".as"
#define OBJECT_FILE ".ob"
#define ENTRY_FILE ".ent"
#define EXTERNAL_FILE ".ext"


FILE *readFile(char *fileName,char *extension);

void fileNameToOpen(char *fileName, char *fileExtension,char **fileToOpen);

AsmFileContent *fileContent(FILE *file, int *fileLines);

void writeFile(FILE *file, char *buffer, char *format);

void writeMachineCodeFile(int IC, int DC, const int *machineCode,char *fileName);

void writeEntryFile(SymbolTable *table, int tableSize, char *fileName);

void writeExternFile(char **externalsLabelsArray, int size, char *fileName);

char *getFileName(char *file);
#endif //PROJECTMAMAN14_FILEMETHODS_H
