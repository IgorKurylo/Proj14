#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileMethods.h"
#include "FirstAsm.h"
#include "SecondAsm.h"
#include "SymbolTable.h"
#include "MemorySnapShot.h"

SymbolTable *table;
int tableSize;
char **externalLabels;
int *memoryCodeArray;
int machineCodeSize;
void printSymbolTable();

int main(int argc, char *argv[]) {

    int numberOfFiles = argc, fileLines = 0, i = 0, index = 0, IC = 0, DC = 0, ICF = 0, DCF = 0, errorsFirstRead = 0, errorSecondRead = 0;
    char *fileName;
    AsmFileContent *asmContentFile = NULL;
    FILE *file = NULL;
    SymbolTable *table_symbol;
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
                    if (errorsFirstRead > 0) {
                        printf("Errors found %d\n", errorsFirstRead);
                        printf("See all errors messages and warnings ,fix and run again\n");
                        exit(-1);
                    } else {
                        ICF = IC;
                        DCF = DC;
                        // update symbol table with IC+INIT ADDRESS = (100)
                        updateSymbolTable((INIT_ADDRESS + IC));
                        printf("\n");
                        printSymbolTable();
                        printf("Final Instructor Counter %d\n", ICF);
                        printf("Final Data Counter %d\n", DCF);
                    }
                    IC = 0;
                    if (initMachineMemoryCode(ICF) != NULL && initExternalLabels(ICF) != NULL) {
                        for (index = 0; index < fileLines; index++) {
                            errorSecondRead += secondRead(asmContentFile[index], &IC, index + 1);
                        }
                    }
                    if (errorSecondRead > 0) {
                        printf("Errors found %d\n", errorSecondRead);
                        printf("See all errors messages and warnings ,fix and run again\n");
                    } else {
                        fileName = getFileName(argv[i]);
                        writeEntryFile(table, tableSize, fileName);
                        fileName = getFileName(argv[i]);
                        writeExternFile(externalLabels, ICF, fileName);
                        fileName = getFileName(argv[i]);
                        writeMachineCodeFile(ICF, DCF, memoryCodeArray, fileName);

                    }

                }
            } else {
                printf("[ERROR] File %s not exists\n", fileName);
            }
        }


    }
    return 0;
}


void printSymbolTable() {
    int i = 0;
    if (table != NULL) {
        printf("%5s\t%3s\t\t%5s\n", "LABEL", "ADDRESS", "TYPE");
        printf("======================== \n");
        for (i = 0; i < tableSize; i++) {

            printf("%5s\t%3d\t\t%5s %s\n", table[i].name, table[i].address,
                   table[i].type == symbol_code ? "code" : table[i].type == symbol_data ? "Data" : "",
                   table[i].is_extern ? "external" : table[i].is_entry ? "entry" : "");
        }
    }
}
