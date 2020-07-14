//
// Created by Igork on 30/05/2020.
//
#include "Constanst.h"

#ifndef PROJECTMAMAN14_DATASTRUCTERS_H
typedef struct{
    int opCode;
    int funct;
    int numParams;
}commandMetaData;

typedef struct
{
    char *key;
    commandMetaData value;

}HashMap;


//typedef struct {
//    unsigned int era:3;
//    union {
//        struct {
//            unsigned int opCode:5;
//            unsigned int srcRegister:3;
//            unsigned int destRegister:3;
//            unsigned int srcAddress:2;
//            unsigned int destAddress:2;
//            unsigned int funct:5;
//        } instructionWord;
//        struct {
//            unsigned int value:21;
//        }extraWord;
//    }data;
//}memoryWord;

typedef struct
{
    char name[2];
}REGISTERS_STRUCT;
typedef struct{
    char label[MAX_SYMBOL_SIZE];
    int lineNumber;
}Labels;

typedef struct
{
    char name[MAX_SYMBOL_SIZE];
    int address;
}EntriesTable;

typedef struct
{
    char name[MAX_SYMBOL_SIZE];
    int address;
}ExternalTable;

typedef struct {
    char line[MAX_LINE_SIZE];
    int lineNumber;
    int lineSize;
    int isLabel;
}AsmFileContent;



#endif //PROJECTMAMAN14_DATASTRUCTERS_H
