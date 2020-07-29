//
// Created by Igork on 13/06/2020.
//

#ifndef PROJECTMAMAN14_SYMBOLTABLE_H
#define PROJECTMAMAN14_SYMBOLTABLE_H

#include "Constanst.h"


enum TYPE_SYMBOL{
    symbol_code=1,symbol_data=2,symbol_external=3,symbol_entry=4
};
typedef struct
{
    char *name;
    enum TYPE_SYMBOL type;
    int is_entry;
    int is_extern;
    int address;
}SymbolTable;

extern SymbolTable *table;
extern int tableSize;

SymbolTable *allocateTable();
int addSymbol(SymbolTable row,int currentIndex);
SymbolTable getTableRow(int index);
int checkIfSymbolExists(char *symbolName,int lineNumber);
void updateIsEntrySymbol(int index);
void updateSymbolTable(int IC);
void freeTable();

#endif //PROJECTMAMAN14_SYMBOLTABLE_H
