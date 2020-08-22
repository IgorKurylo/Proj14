/* Created by Igork on 30/05/2020.*/

#ifndef PROJECTMAMAN14_FILEMETHODS_H

#include <stdlib.h>
#include <stdio.h>
#include "Constanst.h"
#include "MemorySnapShot.h"
#include "SymbolTable.h"
#include "DataStructers.h"

#define ASM_FILE ".as"
#define OBJECT_FILE ".ob"
#define ENTRY_FILE ".ent"
#define EXTERNAL_FILE ".ext"

/* Read file function*/
FILE *readFile(char *fileName, char *extension);

/* Read file and save it on asm file content struct*/
AsmFileContent *fileContent(FILE *file, int *fileLines);

/* write machine code to file*/
void writeMachineCodeFile(int IC, int DC, const int *machineCode, char *fileName);

/*write entry file*/
void writeEntryFile(SymbolTable *table, int tableSize, char *fileName);

/*write extern file*/
void writeExternFile(char **externalLabels, int size, char *fileName);

/* get file name*/
char *getFileName(char *file);

#endif
