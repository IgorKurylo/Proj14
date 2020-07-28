#include <stdio.h>
#include <stdlib.h>
#include "FileMethods.h"
#include "FirstAsm.h"
#include "SecondAsm.h"
#include "SymbolTable.h"
#include "MemorySnapShot.h"

SymbolTable *table;
int tableSize;
MachineCode *machineCode;
int machineCodeSize;

int main(int argc, char *argv[]) {

    int numberOfFiles = argc, fileLines = 0, i = 0, index = 0, IC = 0, DC = 0, ICF = 0, DCF = 0, errorsFirstRead = 0, errorSecondRead = 0;
    char *labelName = NULL;
    AsmFileContent *asmContentFile = NULL;
    FILE *file = NULL;

    if (numberOfFiles == 1) {
        printf("\n[ERROR] - You must enter asm file name \n");

        printf("Program exit! \n");

        exit(0);
    } else {
        for (i = 1; i < argc; i++) {
            file = readFile(argv[i]);
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
                    }
                    for (index = 0; index < fileLines; index++) {
                        errorSecondRead += secondRead(asmContentFile[index], &IC, index + 1);
                    }

                }
            }
        }
        if (table != NULL) {
            printf("%5s\t%3s\t\t%5s\n", "LABEL", "ADDRESS", "TYPE");
            printf("======================== \n");
            for (i = 0; i < tableSize; i++) {
                printf("%5s\t%3d\t\t%5s\n", table[i].name, table[i].address,
                       table[i].type == symbol_code ? "code" : table[i].is_extern ? "external" : "data");
            }

            printf("Machine Code Size %d", machineCodeSize);
            printf("Instructor Counter %d\n", IC);
            printf("Data Counter %d\n", DC);

            printf("Final Instructor Counter %d\n", ICF);
            printf("Final Data Counter %d\n", DCF);


            printf("Errors found %d\n", errorsFirstRead);
        }
    }
    return 0;
}
