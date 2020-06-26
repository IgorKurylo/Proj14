//
// Created by Igork on 13/06/2020.
//

#ifndef PROJECTMAMAN14_SYMBOLTABLE_H
#define PROJECTMAMAN14_SYMBOLTABLE_H

#include "Constanst.h"

enum STATEMENT{
    SYMBOL = 1,COMMAND=2,VIRTUAL_INSTRUCTION=5,EXTERN=3,ENTRY=4
};
enum TYPE_SYMBOL{
    code=1,data=2
};
typedef struct
{
    char *name;
    enum TYPE_SYMBOL type;
    int is_extern;
    int address;
}SymbolTable;

extern SymbolTable *table;
extern int tableSize;

SymbolTable *allocateTable();
int addSymbol(SymbolTable row,int currentIndex);
int deleteSymbol(int index);
int checkIfSymbolExists(char *symbolName);
void freeTable();
#endif //PROJECTMAMAN14_SYMBOLTABLE_H
