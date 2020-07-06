#include <stdio.h>
#include <stdlib.h>
#include "FileMethods.h"
#include "FirstAsm.h"
#include "SymbolTable.h"

SymbolTable *table;
int main(int argc, char *argv[]) {

    int numberOfFiles = argc, fileLines = 0, i = 0, index = 0,IC = 0, DC = 0,errors=0;
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
                        firstRead(asmContentFile[index], &IC, &DC, index,&errors);
                    }

                }
            }
        }
        if(table!=NULL) {
            printf("LABEL | ADDRESS | TYPE ");
            for (i = 0; i < MAX_DATA; i++) {
                printf("%2s %2d %2s ",table[i].name,table[i].address,table[i].type==code?"code":"data");
            }
        }
    }
    return 0;
}
