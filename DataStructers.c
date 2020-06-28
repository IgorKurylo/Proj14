//
// Created by Igork on 27/06/2020.
//
#include <stdlib.h>
#include "Constanst.h"
#include <string.h>
int *dataSnapShotMemory;
int dataSize;
const char separator[2]=",";
int *allocationDataSnapShotMemory(){
    dataSnapShotMemory=(int*)malloc(sizeof(int)*1);
    if(dataSnapShotMemory!=NULL){
        return dataSnapShotMemory;
    }
    return NULL;
}
int *addDataToSnapShotMemory(char *data,int directiveType,int DC){

    char *dataToken;
    int i=1;
    switch (directiveType) {
        case DATA_DIRECTIVE:
            dataToken=strtok(data, separator);
            while (dataToken!=NULL){
                dataSnapShotMemory[DC++]=atoi(dataToken);
                dataToken=strtok(data, separator);
                dataSnapShotMemory=realloc(dataSnapShotMemory,DC);
            }
            break;
        case STRING_DIRECTIVE:
            while(i<strlen(data)-1){
                dataSnapShotMemory[DC++]=(int)data[i];
                dataSnapShotMemory=realloc(dataSnapShotMemory,DC);
            }
            break;
        default:
            break;

    }
    return dataSnapShotMemory;
}