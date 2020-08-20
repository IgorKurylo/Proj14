//
// Created by Igork on 21/06/2020.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SymbolTable.h"

SymbolTable *table;
int tableSize;

/* allocate symbol array table*/
SymbolTable *allocateTable() {
    SymbolTable *resizeTable;
    if (table == NULL) {
        table = (SymbolTable *) malloc(sizeof(SymbolTable) * DEFAULT_SYMBOL_TABLE_SIZE);
        if (table != NULL) {
            printf("[INFO] Symbol Table allocated successfully\n");
            return table;
        } else {
            printf("[FATAL] Fail on symbol Table allocation\n");
            return NULL;
        }
    } else {
        resizeTable = realloc(table, tableSize);
        if (resizeTable != NULL) {
            table = resizeTable;
            printf("[INFO] Symbol Table reallocated with size %d\n", tableSize);
            return table;
        } else {
            printf("Fail on Symbol Table reallocation\n");
            return NULL;
        }
    }

}

/* add symbol to table*/
int addSymbol(SymbolTable row, int lineNumber) {

    /* check if a symbol exists or not in the table, if not we update table size and add row to table*/
    if (isSymbolExists(row.name) == -1) {
        tableSize++;
        table = (SymbolTable *) realloc(table, sizeof(SymbolTable) * tableSize);
        if (table != NULL) {
            table[tableSize - 1] = row;
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }

}

/* check if the symbol exists in table*/
int isSymbolExists(char *symbolName) {
    int i = 0;
    for (i = 0; i < tableSize; i++) {
        if (strcmp(symbolName, table[i].name) == 0) {
            return i; // return the index in array of the symbol
        }
    }
    return -1;
}

/* update symbol is entry*/
void updateSymbolTable(int IC) {
    int i = 0;
    if (table != NULL) {
        for (i = 0; i < tableSize; i++) {
            if (table[i].type == symbol_data) {
                table[i].address += IC;
            }
        }
    }
}

/* update symbol is entry*/
void updateIsEntrySymbol(int index) {
    if (table != NULL) {
        table[index].is_entry = 1;
    }
}

/*free table array allocation*/
void freeTable() {
    if (table != NULL) {
        free(table);
        tableSize = 0;
    }
}


