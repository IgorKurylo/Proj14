#include <stdio.h>
#include <stdlib.h>
#include "FileMethods.h"
#include "FirstAsm.h"


int main(int argc, char *argv[]) {

    int numberOfFiles = argc, fileLines = 0, i = 0, index = 0;
    int IC = 100, DC = 0;

    AsmFileContent *asmContentFile = NULL;

    FILE *file = NULL;


    char *labelName = NULL;

    if (numberOfFiles == 1) {
        printf("\n You must enter asm file name \n");

        printf("Program exit!");

        exit(0);
    } else {
        for (i = 1; i < argc; i++) {
            file = readFile(argv[i]);
            if (file != NULL) {
                asmContentFile = fileContent(file, &fileLines);
                for (index = 0; index < fileLines; index++) {
                    firstRead(asmContentFile[index],  &IC,  &DC, index);
                }
            }
        }
    }
    return 0;
}
