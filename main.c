#include <stdio.h>
#include <stdlib.h>
#include "FileMethods.h"
#include "FirstAsm.h"
#include "SymbolTable.h"

SymbolTable *table;
int tableSize;
int main(int argc, char *argv[]) {

    int numberOfFiles = argc, fileLines = 0, i = 0, index = 0,IC = 0, DC = 0,errorsFirstRead=0;
    char *labelName = NULL;
    AsmFileContent *asmContentFile = NULL;
    FILE *file = NULL;

    if (numberOfFiles == 1) {
        printf("\n You must enter asm file name \n");

        printf("Program exit! \n");

        exit(0);
    } else {
        for (i = 1; i < argc; i++) {
            file = readFile(argv[i]);
            if (file != NULL) {
                asmContentFile = fileContent(file, &fileLines);
                if(allocateTable()!=NULL) {
                    for (index = 0; index < fileLines; index++) {
                        errorsFirstRead+=firstRead(asmContentFile[index], &IC, &DC, index);
                    }
                }
            }
        }
        if(table!=NULL) {
            printf("LABEL | ADDRESS | TYPE \n ");
            printf("====================== \n");
            for (i = 0; i < tableSize; i++) {
                printf("%2s %2d %2s \n",table[i].name,table[i].address,table[i].type==code?"code":"data");
            }
            printf("Errors found %d\n",errorsFirstRead);
        }
    }
    return 0;
}
