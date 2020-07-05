//
// Created by Igork on 27/06/2020.
//
#include <stdlib.h>
#include "Constanst.h"
#include <string.h>

int *dataSnapShotMemory;
int dataSize;
const char separator[2] = ",";

int *allocationDataSnapShotMemory() {
    dataSnapShotMemory = (int *) malloc(sizeof(int) * 1);
    if (dataSnapShotMemory != NULL) {
        dataSize++;
        return dataSnapShotMemory;
    }
    return NULL;
}

int *addDataToSnapShotMemory(char *data, int directiveType, int *DC) {

    int i = 0;
    int sigh = 1;
    int counterOfData = 0;
    int ascii_code = 48;
    switch (directiveType) {

        case DATA_DIRECTIVE:

            while (i < strlen(data)-1) {

                if (data[i] != ',') {
                    if (data[i] == '-') {
                        sigh = -1;
                        i++;
                        dataSnapShotMemory[*(DC)++] = ((int) data[i] - ascii_code) * sigh;
                    } else {
                        dataSnapShotMemory[*(DC)++] = ((int) data[i] - ascii_code) * sigh;
                    }
                    i++;
                } else {
                    i++;
                }
            }
            *DC = +counterOfData;
            break;
        case STRING_DIRECTIVE:
            while (i < strlen(data)) {
                if (data[i] != '"') {
                    dataSnapShotMemory[*(DC)++] = (int) data[i];
                    i++;
                } else {
                    i++;
                }
            }
            break;
        default:
            break;

    }
    return dataSnapShotMemory;
}