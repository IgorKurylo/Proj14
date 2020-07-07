//
// Created by Igork on 30/05/2020.
//

#ifndef PROJECTMAMAN14_HELPERSMETHODS_H

int isEmptyLine(char *line);
int isComment(char *line);
int isRegister(char *line);
int parseCommand(char *line,char **asm_command,int lineNumber,int *IC,int *errorsCounter);
int isCommandExists(char *command,int *numOfOperands);
int parseDirective(char *line, char **data, int lineNumber, int *directiveType, int *errorsCounter);
char *parseLabel(char *line,char **labelName,int lineNumber,int *errorsCounter);
int isAlphaNumeric(const char *str);
int isNumber(const char *str);

int operandValidation(char *line,int *num,HashMap command);
int addressingValidation(char *line,int addressingType,HashMap command);

int isJumpToLabelSymbol(const char *str);
void skipWhitesSpaces(char *line);
char *skipLabel(char *line);
void convertToBase16(int number);
void convertToBase2(int number);
int validateOperand(char *operand, int *addressType);
int isValueNumber(char *operand);
int isExternDirective(char *line,int *errorsCounter);
void calculateOffsetAddress(int addressType,int *IC);
void populateDataDirective(int *DC, int directiveType, char *directiveDefinedData,int *errorCounter);

#endif //PROJECTMAMAN14_HELPERSMETHODS_H
