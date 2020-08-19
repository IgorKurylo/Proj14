//
// Created by Igork on 30/05/2020.
//

#ifndef PROJECTMAMAN14_HELPERSMETHODS_H

#include <stdio.h>
#include "DataStructers.h"
/* check is empty line*/
int isEmptyLine(char *line);

/* check is empty line*/
int isComment(char *line);

/* check is empty line*/
int isRegister(char *line);

/* check is empty line*/
int parseCommand(char *line, char **command, int lineNumber, int *numOfOperand, int *errorCounter, char **operands);

/* check is empty line*/
int parseOperands(char *operands, char *command, int numOfOperand, int lineNumber, int *IC, int *errorCounter);

/* check is empty line*/
int isCommandExists(char *command, int *numOfOperands);

/* check is empty line*/
int parseDirective(char *line, char **data, int lineNumber, int *directiveType, int *errorsCounter);

/* check is empty line*/
char *parseLabel(char *line, char **labelName, int lineNumber, int *errorsCounter);

/* check is empty line*/
int isAlphaNumeric(const char *str);

/* check is empty line*/
int numberValidation(char *number_value, int memorySize, int *value, int lineNumber, int *errorCounter);

/* check is empty line*/
int stringValidation(char **string, int lineNumber, int *errorCounter);

/* check is empty line*/
char *skipWhitesSpaces(char *line);

/* check is empty line*/
char *skipLabel(char *line);

/* check is empty line*/
int isExternDirective(char *line, char **label, int *errorsCounter);

/* check is empty line*/
int isEntryDirective(char *line, char **labelEntry);

/* check is empty line*/
int populateDataDirective(int *DC, int directiveType, char *directiveDefinedData, int *errorCounter, int lineNumber);

/* check is empty line*/
int validateOperand(char *operand, int *addressType, int line, int *errorCounter, int *value, int *operandType);

/* check is empty line*/
int validateCommandAddressType(char *command, int addressTypeSrc, int addressTypeDest);

/* check is empty line*/
int checkIsDirective(char *line, const char *originalLine, int *counter, const char *type);

/* check is empty line*/
void extractOperand(char *line, char **label, char *originalLine, int counter);

/* check is empty line*/
int calculateOffsetAddress(int addressType);

/* check is empty line*/
Command commandOpCode_functCode(char *command);

/* check is empty line*/
void parseOneOperand(char *operands, char **oneOperand);

/* check is empty line*/
void parseTwoOperands(char *operands, char **firstOperand, char **secondOperand);

/* check is empty line*/
int isJmpCommand(char *command);

/* check is empty line*/
int isDataFormattingCorrect(char *directiveData, int *errorCounter, int lineNumber);

/* check is empty line*/
void validateImmediateSize(int lineNumber, int *errorCounter, char *operand, int addressType, int *value);

/* check is empty line*/
int validateJmpTypesCommandOperand(char *operand, int lineNumber, int *errorCount);

/* check is empty line*/
int convertTo2Complement(int value);


int convertToBase2(int number);


int isValueNumber(char *operand, int *value, int line, int *errorCounter);


#endif //PROJECTMAMAN14_HELPERSMETHODS_H
