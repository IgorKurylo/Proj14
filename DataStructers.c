//
// Created by Igork on 27/06/2020.
//
#include <stdlib.h>
#include "Constanst.h"
#include <string.h>
#include "DataStructers.h"
#include "HelpersMethods.h"

int dataSnapShotMemory[256];
int dataSize;

int addToDataSnapShot(char *number, int DC) {
    int num;
    char *end;
    num = (int) strtol(number, &end, 10);
    if (end != NULL) {
        dataSnapShotMemory[DC] = num;
        return 1;
    } else {
        return 0;
    }
}

int *
saveToSnapShotMemory(char *data, int directiveType, int *DC, int *deltaCounter, int *errorCounter, int lineNumber) {
    char *numberStr = NULL;
    int i = 0;
    int counterOfData = *DC;
    char *delim = ",";
    int value = 0;
    switch (directiveType) {
        case DATA_DIRECTIVE:
            numberStr = strtok(data, delim);
            while (numberStr) {
                if (numberValidation(numberStr, &value, lineNumber, errorCounter)) {
                    dataSnapShotMemory[counterOfData] = value;
                    counterOfData++;
                    numberStr = strtok(NULL, delim);

                }
            }
            *deltaCounter = counterOfData;
            break;
        case STRING_DIRECTIVE:
            if(stringValidation(&data,lineNumber,errorCounter)) {
                while (i <= strlen(data)) {
                    dataSnapShotMemory[counterOfData++] = (int) data[i];
                    i++;
                }
                *deltaCounter = counterOfData;
            }
            break;
        default:
            break;

    }
    *DC = counterOfData;
    dataSize += counterOfData;
    return dataSnapShotMemory;
}
