/* Created by Igork on 30/05/2020.*/

#ifndef PROJECTMAMAN14_HELPERSMETHODS_H

#include <stdio.h>
#include "DataStructers.h"

/* check is empty line*/
int isEmptyLine(char *line);

/* check is comment line*/
int isComment(char *line);

/* check is register*/
int isRegister(char *line);

/* parse command*/
int parseCommand(char *line, char **command, int lineNumber, int *numOfOperand, int *errorCounter, char **operands);

/* parse operands*/
int parseOperands(char *operands, char *command, int numOfOperand, int lineNumber, int *IC, int *errorCounter);

/* parse directive*/
int parseDirective(char *line, char **data, int lineNumber, int *directiveType, int *errorsCounter);

/* parse label*/
int parseLabel(char *line, char **labelName, int lineNumber, int *errorsCounter);

/* check is command exists*/
int isCommandExists(char *command, int *numOfOperands);

/* check is alpha numeric in string*/
int isAlphaNumeric(const char *str);

/* number validation check if the number is not out of max size*/
int numberValidation(char *number_value, int memorySize, int *value, int lineNumber, int *errorCounter);

/* string validation check if string begin and end with  "" */
int stringValidation(char **string, int lineNumber, int *errorCounter);

/* skip white spaces*/
char *skipWhitesSpaces(char *line);

/* skip label is needed*/
char *skipLabel(char *line);

/* check is extern directive*/
int isExternDirective(char *line, char **label, int *errorsCounter,int lineNumber);

/* check is entry directive*/
int isEntryDirective(char *line, char **labelEntry,int lineNumber,int *errorCounter);

/* save data from .data directive in memory block*/
int populateDataDirective(int *DC, int directiveType, char *directiveDefinedData, int *errorCounter, int lineNumber);

/*validate if operand is correct*/
int validateOperand(char *operand, int *addressType, int line, int *errorCounter, int *value, int *operandType);

/* validate command address type is a command have operand with valid address type*/
int validateCommandAddressType(char *command, int addressTypeSrc, int addressTypeDest);

/* check is directive entry or external*/
int checkIsDirective(char *line,  char *originalLine, int *counter, const char *type,int lineNumber,int* errorCounter);

/* extract operand from directive,label,etc*/
void extractOperand(char *line, char **label, char *originalLine, int counter);

/*calculate offset*/
int calculateOffsetAddress(int addressType);

/* return command by command name */
Command getCommandByCommandName(char *command);

/* parse one operand when command with one operand*/
void parseOneOperand(char *operands, char **oneOperand);

/* parse two operand when command with 2 operands*/
void parseTwoOperands(char *operands, char **firstOperand, char **secondOperand);

/* check if the command is jmp,jsr,bne*/
int isJmpCommand(char *command);

/* check if data directive in correct formatting*/
int isDataFormattingCorrect(char *directiveData, int *errorCounter, int lineNumber);

/* validate if immediate size*/
void validateImmediateSize(int lineNumber, int *errorCounter, char *operand, int addressType, int *value);

/* validate tmp jmp,jre,bne command types */
int validateJmpTypesCommandOperand(char *operand, int lineNumber, int *errorCount);

/* convert number to 2 complement*/
int convertTo2Complement(int value);

/* check if the value is a number*/
int isValueNumber(char *operand, int *value, int line, int *errorCounter);

/*adjust offsets by source and/or dest address types*/
void adjustOffsetOfSrcDest(int sourceAddressType, int destAddressType, int *srcOffset, int *destOffset);

int isLabel(char *line);

#endif
