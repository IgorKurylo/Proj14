//
// Created by Igork on 13/06/2020.
//

#ifndef PROJECTMAMAN14_FIRSTASM_H
#define PROJECTMAMAN14_FIRSTASM_H

/* first read function, calculate IC , DC, create symbol table*/
int firstRead(AsmFileContent asmContentFile, int *IC, int *DC, int lineNumber);

/*add symbol to symbols table*/
void addSymbolInTable(char *label, int type, int address, int lineNumber, int *errorCounter);

#endif //PROJECTMAMAN14_FIRSTASM_H
