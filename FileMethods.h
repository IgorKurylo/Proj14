//
// Created by Igork on 30/05/2020.
//

#include <stdlib.h>
#include <stdio.h>
#include "Constanst.h"

#ifndef PROJECTMAMAN14_FILEMETHODS_H

typedef struct {
    char line[MAX_LINE_SIZE];
    int lineNumber;
    int lineSize;
    int isLabel;
} AsmFileContent;



FILE * readFile(char *fileName);
//int writeFile(char *buffer);
AsmFileContent *fileContent(FILE *file,int *fileLines);


#endif //PROJECTMAMAN14_FILEMETHODS_H
