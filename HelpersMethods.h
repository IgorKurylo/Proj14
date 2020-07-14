//
// Created by Igork on 30/05/2020.
//

#ifndef PROJECTMAMAN14_HELPERSMETHODS_H

int isEmptyLine(char *line);
int isComment(char *line);
int isRegister(char *line);
int parseCommand(char *line, char **command, int lineNumber, int *numOfOperand, int *errorCounter,char **operands);
int parseOperands(char *operands,char *command,int numOfOperand,int lineNumber,int *IC,int *errorCounter);
int isCommandExists(char *command,int *numOfOperands);
int parseDirective(char *line, char **data, int lineNumber, int *directiveType, int *errorsCounter);
char *parseLabel(char *line,char **labelName,int lineNumber,int *errorsCounter);
int isAlphaNumeric(const char *str);
int numberValidation(char *number, int *value, int lineNumber, int *errorCounter);
int stringValidation(char **string,int lineNumber,int *errorCounter);

int isJumpToLabelSymbol(const char *str);
void skipWhitesSpaces(char *line);
char *skipLabel(char *line);
int isExternDirective(char *line,int *errorsCounter);
int populateDataDirective(int *DC, int directiveType, char *directiveDefinedData,int *errorCounter,int lineNumber);

int isNumber(const char *str);

void convertToBase16(int number);
void convertToBase2(int number);
int validateOperand(char *operand, int *addressType);
int isValueNumber(char *operand);
void calculateOffsetAddress(int addressType,int *IC);

#endif //PROJECTMAMAN14_HELPERSMETHODS_H
