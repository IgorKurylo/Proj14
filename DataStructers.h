//
// Created by Igork on 30/05/2020.
//

#ifndef PROJECTMAMAN14_DATASTRUCTERS_H
typedef struct {
    int opCode;
    int funct;
    int numParams;
} commandMetaData;

typedef struct {
    char *key;
    commandMetaData value;

} HashMap;


//typedef struct {
//    char name[MAX_SYMBOL_SIZE];
//    int address;
//} EntriesTable;
//
//typedef struct {
//    char name[MAX_SYMBOL_SIZE];
//    int address;
//} ExternalTable;


#endif //PROJECTMAMAN14_DATASTRUCTERS_H
