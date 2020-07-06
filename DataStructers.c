//
// Created by Igork on 27/06/2020.
//
#include <stdlib.h>
#include "Constanst.h"
#include <string.h>
#include "DataStructers.h"

int dataSnapShotMemory[256];
int dataSize;


int *addDataToSnapShotMemory(char *data, int directiveType, int *DC) {

    int i = 0;
    int sigh = 1;
    int counterOfData = 0;
    int ascii_code = 48;
    switch (directiveType) {

        case DATA_DIRECTIVE:
            while (i < strlen(data) - 1) {

                if (data[i] != ',') {
                    if (data[i] == '-') {
                        sigh = -1;
                        i++;
                        dataSnapShotMemory[counterOfData++] = ((int) data[i] - ascii_code) * sigh;
                    } else {
                        dataSnapShotMemory[counterOfData++] = ((int) data[i] - ascii_code) * sigh;
                    }
                    i++;
                } else {
                    i++;
                }
            }
            break;
        case STRING_DIRECTIVE:
            while (i < strlen(data)) {
                if (data[i] != '"') {
                    dataSnapShotMemory[counterOfData++] = (int) data[i];
                    i++;
                } else {
                    i++;
                }
            }
            break;
        default:
            break;

    }
    *DC=counterOfData;
    return dataSnapShotMemory;
}