//
// Created by Igork on 19/07/2020.
//


#ifndef PROJ14_SECONDASM_H
#define PROJ14_SECONDASM_H


/* check is empty line*/
int secondRead(AsmFileContent asmContentFile, int *IC, int lineNumber);
/* check is empty line*/
void buildMachineCodeOneOperand(const int *IC, int lineNumber, char *firstOperand,
                                int labelAddress, int isDistanceLabel, int symbolIndex, const int *destAddressType,
                                const int *operandType, int *destOffset, Command command, int valueDest);
/* check is empty line*/
void buildMachineCodeDirective(int lineNumber, char *data, int directiveType, int *errorCounter);
/* check is empty line*/
void adaptOffsetsByAddressType(int destAddressType, int srcAddressType, int *srcOffset, int *destOffset);
/* check is empty line*/
void updateExternalLabelAddress(int IC, int isSrcExternalLabel,
                                int isDestExternalLabel, char *firstOperand, char *secondOperand);

#endif //PROJ14_SECONDASM_H