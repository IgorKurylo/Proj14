//
// Created by Igork on 30/05/2020.
//

#ifndef PROJECTMAMAN14_HELPERSMETHODS_H

int isEmptyLine(char *line);
int isComment(char *line);
int isRegister(char *line);
int parseCommand(char *line,char **asm_command,int lineNumber,int *IC);
int isCommandExists(char *command,int *numOfOperands);
int parseDirective(char *line,char **data,int lineNumber,int *directiveType);
char *parseLabel(char *line,char **labelName,int lineNumber);
int isAlphaNumeric(const char *str);
int isNumber(const char *str);
int IsVirtualInstruction(char *line,unsigned char* type);
int operandValidation(char *line,int *num);
void trimWhiteSpaces(char *line,int size);
void skipWhitesSpaces(char *line);
char *skipLabel(char *line);
void convertToBase16(int number);
void convertToBase2(int number);
int validateOperand(char *operand, int *addressType);
int isValueNumber(char *operand);
int isExternEntryDirective(char *line,char **labelOperand);
void calculateICAddress(int addressType,int *IC);
void populateDataDirective(int *DC, int directiveType, char *directiveDefinedData);
#endif //PROJECTMAMAN14_HELPERSMETHODS_H
