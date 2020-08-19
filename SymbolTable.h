//
// Created by Igork on 13/06/2020.
//

#ifndef PROJECTMAMAN14_SYMBOLTABLE_H
#define PROJECTMAMAN14_SYMBOLTABLE_H

#include "Constanst.h"

/* check is empty line*/

enum TYPE_SYMBOL {
    symbol_code = 1, symbol_data = 2, symbol_external = 3, symbol_entry = 4
};
/* check is empty line*/

typedef struct {
    char *name;
    enum TYPE_SYMBOL type;
    int is_entry;
    int is_extern;
    int address;
} SymbolTable;
/* check is empty line*/
extern SymbolTable *table;
/* check is empty line*/

extern int tableSize;

/* check is empty line*/

SymbolTable *allocateTable();

/* check is empty line*/

int addSymbol(SymbolTable row, int currentIndex);

/* check is empty line*/

int checkIfSymbolExists(char *symbolName, int lineNumber);

/* check is empty line*/

void updateIsEntrySymbol(int index);

/* check is empty line*/

void updateSymbolTable(int IC);

/* check is empty line*/
void freeTable();

#endif //PROJECTMAMAN14_SYMBOLTABLE_H
