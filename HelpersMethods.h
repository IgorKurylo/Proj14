//
// Created by Igork on 30/05/2020.
//

#ifndef PROJECTMAMAN14_HELPERSMETHODS_H

int isEmptyLine(char *line);
int isComment(char *line);
int isSymbol(char *line,int index);
int isRegister(char *line,char *registers,int index);
int parseCommand(char *line,char **asm_command,int lineNumber,int *addressingType);
int isLegalLabel(char *label);
int isCommandExists(char *command);
int parseDirective(char *line,char **data,int lineNumber);
char *parseLabel(char *line,char **labelName,int lineNumber);
int isAlphaNumeric(const char *str);
int isNumber(const char *str);
int IsVirtualInstruction(char *line,unsigned char* type);
int operandValidation(char *line,int *num);
int IsEntryInstruction(char *line);
int IsExtendInstruction(char *line);
void trimWhiteSpaces(char *line,int size);
void skipWhitesSpaces(char *line);
char *skipLabel(char *line);
void convertToBase16(int number);
void convertToBase2(int number);
void collectLabels(char *labelName,int lineNumber);
int validateOperand(char *operand,int lineNumber);
int isValueNumber(char *operand);
#endif //PROJECTMAMAN14_HELPERSMETHODS_H
