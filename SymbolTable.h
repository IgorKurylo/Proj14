//
// Created by Igork on 13/06/2020.
//

#ifndef PROJECTMAMAN14_SYMBOLTABLE_H
#define PROJECTMAMAN14_SYMBOLTABLE_H

#include "Constanst.h"

enum STATEMENT{
    SYMBOL = 1,COMMAND=2,VIRTUAL_INSTRUCTION=5,EXTERN=3,ENTRY=4
};
typedef struct
{
    char name[MAX_SYMBOL_SIZE];
    enum STATEMENT statement;
    int is_extern;
    int address;
}SymbolTable;

void allocateTable(SymbolTable *table[]);
int addSymbol(SymbolTable row);
int deleteSymbol(int index);
int checkIfSymbolExists(char symbolName);
void freeTable();
#endif //PROJECTMAMAN14_SYMBOLTABLE_H
