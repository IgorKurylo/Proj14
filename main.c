#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileMethods.h"
#include "FirstAsm.h"
#include "SecondAsm.h"

SymbolTable *table;
int tableSize;
int *memoryCodeArray;
int machineCodeSize;

void printSymbolTable();

void generateOutputFiles(char *const *argv, int i, int ICF, int DCF);

int main(int argc, char *argv[]) {

    int numberOfFiles = argc, fileLines = 0, i = 0, index = 0, IC = 0, DC = 0, ICF = 0, DCF = 0, errorsFirstRead = 0, errorSecondRead = 0;
    char *fileName;
    AsmFileContent *asmContentFile = NULL;
    FILE *file = NULL;
    if (numberOfFiles == 1) {
        printf("\n[ERROR] - You must enter asm file name \n");

        printf("Program exit! \n");

        exit(0);
    } else {
        for (i = 1; i < argc; i++) {
            fileName = getFileName(argv[i]);
            file = readFile(fileName, ASM_FILE);
            if (file != NULL) {
                asmContentFile = fileContent(file, &fileLines);
                if (allocateTable() != NULL) {
                    for (index = 0; index < fileLines; index++) {
                        errorsFirstRead += firstRead(asmContentFile[index], &IC, &DC, index);
                    }
                    /*print errors if we find it in first read, if not save the IC & DC final values.*/
                    if (errorsFirstRead > 0) {
                        printf("[INFO] Errors found %d\n", errorsFirstRead);
                        printf("[INFO] See all errors messages and warnings ,fix and run again\n");
                        exit(-1);
                    } else {
                        ICF = IC;
                        DCF = DC;
                        /* update symbol table with IC+INIT ADDRESS = (100) */
                        updateSymbolTable((INIT_ADDRESS + IC));
                    }
                    IC = 0;
                    if (initMachineMemoryCode(ICF) != NULL && initExternalLabels(ICF) != NULL) {
                        for (index = 0; index < fileLines; index++) {
                            errorSecondRead += secondRead(asmContentFile[index], &IC, index + 1);
                        }
                    }
                    /*print errors if we find in second read, if not we generate files */
                    if (errorSecondRead > 0) {
                        printf("[INFO] Errors found %d\n", errorSecondRead);
                        printf("[INFO] See all errors messages and warnings ,fix and run again\n");
                        exit(-1);
                    } else {
                        generateOutputFiles(argv, i, ICF, DCF);
                    }
                }
                freeTable();
                freeMemory();
            } else {
                printf("[ERROR] File %s not exists\n", fileName);
            }
        }
    }
    return 0;
}

/* generate output files*/
void generateOutputFiles(char *const *argv, int i, int ICF, int DCF) {
    char *file_name;

    file_name = getFileName(argv[i]);

    writeEntryFile(table, tableSize, file_name);

    file_name = getFileName(argv[i]);

    writeExternFile(externalLabels, ICF, file_name);

    file_name = getFileName(argv[i]);

    writeMachineCodeFile(ICF, DCF, memoryCodeArray, file_name);
}

/* print symbol table from first read just for debugging*/
void printSymbolTable() {
    int i = 0;
    if (table != NULL) {
        printf("%5s\t%3s\t\t%5s\n", "LABEL", "ADDRESS", "TYPE");
        printf("======================== \n");
        for (i = 0; i < tableSize; i++) {

            printf("%5s\t%3d\t\t%5s %s\n", table[i].name, table[i].address,
                   table[i].type == symbol_code ? "code" : table[i].type == symbol_data ? "data" : "",
                   table[i].is_extern ? "external" : table[i].is_entry ? "entry" : "");
        }
    }
}
