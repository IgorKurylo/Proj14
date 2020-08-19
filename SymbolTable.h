//
// Created by Igork on 13/06/2020.
//

#ifndef PROJECTMAMAN14_SYMBOLTABLE_H
#define PROJECTMAMAN14_SYMBOLTABLE_H

#include "Constanst.h"

/* define enum of symbols types*/
enum TYPE_SYMBOL {
    symbol_code = 1, symbol_data = 2, symbol_external = 3, symbol_entry = 4
};
/* struct which represent symbol row in symbol table*/
typedef struct {
    char *name;
    enum TYPE_SYMBOL type;
    int is_entry;
    int is_extern;
    int address;
} SymbolTable;

/* array of symbol table*/
extern SymbolTable *table;
/* current table size*/
extern int tableSize;

/* allocate symbol array table*/
SymbolTable *allocateTable();

/* add symbol to table*/
int addSymbol(SymbolTable row, int currentIndex);

/* check if the symbol exists in table*/
int checkIfSymbolExists(char *symbolName, int lineNumber);

/* update symbol is entry*/
void updateIsEntrySymbol(int index);

/* update data symbol addressing*/
void updateSymbolTable(int IC);

/* free table*/
void freeTable();

#endif //PROJECTMAMAN14_SYMBOLTABLE_H
