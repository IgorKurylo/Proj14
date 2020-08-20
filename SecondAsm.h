//
// Created by Igork on 19/07/2020.
//


#ifndef PROJ14_SECONDASM_H
#define PROJ14_SECONDASM_H


/* Second Read function which, parse labels,calculate distance of jmps,jre,bne commands , calculate IC and  build binary machine code */
int secondRead(AsmFileContent asmContentFile, int *IC, int lineNumber);

/* build machine code one operand*/
void buildMachineCodeOneOperand(const int *IC, int lineNumber, char *firstOperand,
                                int labelAddress, int isDistanceLabel, int symbolIndex, const int *destAddressType,
                                const int *operandType, int *destOffset, Command command, int valueDest);

/* build machine code on directive*/
void buildMachineCodeDirective(int lineNumber, char *data, int directiveType, int *errorCounter);

/* adapt offsets by addressing types*/
void adaptOffsetsByAddressType(int destAddressType, int srcAddressType, int *srcOffset, int *destOffset);

/* update the symbol which is external by correct address*/
//void updateExternalLabelAddress(int IC, int isSrcExternalLabel,
//                                int isDestExternalLabel, char *firstOperand, char *secondOperand);

void updateExternalLabelAddress(int IC, char *operand);

/* build machine code two operands*/
void buildMachineCode2Operands(char *firstOperand, char *secondOperand, int labelDestAddress, int labelSrcAddress,
                               int regDest, int regSrc, int isSrcExternalLabel, int isDestExternalLabel,
                               int destAddressType, int srcAddressType, int operandDestType, int operandSrcType,
                               int valueSrc, int valueDest, Command *commandObj);
void calculateExternalLabelsAddresses(const int *IC, char *firstOperand, char *secondOperand,
                                      int isSrcExternalLabel, int isDestExternalLabel, int destAddressType,
                                      int srcAddressType, int destOffset, int srcOffset);
#endif //PROJ14_SECONDASM_H