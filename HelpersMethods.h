//
// Created by Igork on 30/05/2020.
//

#ifndef PROJECTMAMAN14_HELPERSMETHODS_H

#include "DataStructers.h"
#include <stdio.h>

int isEmptyLine(char *line);

int isComment(char *line);

int isRegister(char *line);

int parseCommand(char *line, char **command, int lineNumber, int *numOfOperand, int *errorCounter, char **operands);

int parseOperands(char *operands, char *command, int numOfOperand, int lineNumber, int *IC, int *errorCounter);

int isCommandExists(char *command, int *numOfOperands);

int parseDirective(char *line, char **data, int lineNumber, int *directiveType, int *errorsCounter);

char *parseLabel(char *line, char **labelName, int lineNumber, int *errorsCounter);

int isAlphaNumeric(const char *str);

int numberValidation(char *number, int *value, int lineNumber, int *errorCounter);

int stringValidation(char **string, int lineNumber, int *errorCounter);

int isJumpToLabelSymbol(const char *str);

void skipWhitesSpaces(char *line);

char *skipLabel(char *line);

int isExternDirective(char *line, char **label, int *errorsCounter);

int populateDataDirective(int *DC, int directiveType, char *directiveDefinedData, int *errorCounter, int lineNumber);

int validateOperand(char *operand, int *addressType, int line, int *errorCounter, int *value, int *operandType);

int validateCommandAddressType(char *command, int addressTypeSrc, int addressTypeDest);

void createMachineCode(char *firstOp, HashMap *commandObj, int destAddressType, int valueDest, int operandTypeDest,
                       int *destExtraWord, int destOperand);

void createMachineCode2(char *command, HashMap *commandObj, int sourceAddressType, int destAddressType, int *valueSrc,
                        int *valueDest, int operandTypeSrc, int operandTypeDest, int destOperand, int srcOperand);

int isEntryDirective(char *line, char **labelEntry);

int checkIsDirective(char *line, const char *originalLine, char *finalDirective, int *counter, const char *type);

void extractOperand(char *line, char **label, char *originalLine, int counter);

int isNumber(const char *str);

void convertToBase16(int number);

int convertToBase2(int number, int size);

int isValueNumber(char *operand);

int calculateOffsetAddress(int addressType);

HashMap commandOpCode_functCode(char *command);

void parseOneOperand(char *operands, char **oneOperand);

void parseTwoOperands(char *operands, char **firstOperand, char **secondOperand);


#endif //PROJECTMAMAN14_HELPERSMETHODS_H
