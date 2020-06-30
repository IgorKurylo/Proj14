//
// Created by Igork on 01/06/2020.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "DataStructers.h"
#include "SymbolTable.h"
#include "Constanst.h"

const HashMap asm_commands[] =
        {
                {"mov",  {0,  0, 2}},
                {"cmp",  {1,  0, 2}},
                {"add",  {2,  1, 2}},
                {"sub",  {3,  2, 2}},
                {"not",  {4,  1, 1}},
                {"clr",  {5,  1, 1}},
                {"lea",  {6,  0, 2}},
                {"inc",  {7,  3, 1}},
                {"dec",  {8,  4, 1}},
                {"jmp",  {9,  1, 1}},
                {"bne",  {10, 2, 1}},
                {"red",  {11, 1, 0}},
                {"prn",  {12, 1, 1}},
                {"jsr",  {13, 3, 1}},
                {"rst",  {14, 0, 0}},
                {"stop", {15, 0, 0}}
        };
const char *directives[] = {".string", ".data", ".extern", ".entry"};
const char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

void trimWhiteSpaces(char *line, int size) {
    int i = 0, whiteSpaceCnt = 0;
    if (size > 0) {
        for (i = 0; i < size; i++) {
            if (line[i] != ' ') {
                line[whiteSpaceCnt++] = line[i];
            }
        }
    }
    line[whiteSpaceCnt] = '\0';
}

char *skipWhitesSpaces(char *line) {
    while ((*line == ' ') || (*line == '\t')) {
        line++;
    }
    return line;
}

char *skipLabel(char *line) {
    while (*line != ':') {
        line++;
    }
    line++;
    return line;
}

int isAlphaNumeric(const char *str) {
    return (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z');
}

int isNumber(const char *str) {
    return *str >= '0' && *str <= '9';
}

int isEmptyLine(char *line) {
    char *lineEmpty;
    lineEmpty = skipWhitesSpaces(line);
    if (*lineEmpty == ' ' || *lineEmpty == '\t') {
        return 1;
    }
    return 0;
}

int isComment(const char *line) {
    if (*line == ';')
        return 1;
    return 0;
}

char *parseLabel(char *line, char **labelName, int lineNumber) {
    char *label = NULL;
    char *originalLine = line;
    int count = 0;
    if (strchr(originalLine, ':')) {
        while (*line != ':' && *line != ' ') {
            line++;
            count++;
        }
        if (count == 0) {
            printf("[Error] - Label can't start with spaces, line %d", lineNumber);
        }
        if (count > MAX_SYMBOL_SIZE) {
            printf("[Error] - Label too large only %d characters, line %d", MAX_SYMBOL_SIZE, lineNumber);
            return NULL;
        }
        label = malloc(sizeof(char) * (count+1));
        if (!label) {
            printf("Allocation fail\n");
            return NULL;
        }
        strncpy(label, originalLine, count);
        label[count]=0;
        *labelName = label;
        if (!isAlphaNumeric(label)) {
            printf("[Error] - Syntax error, label must be Alpha Numeric, line %d", lineNumber);
            return NULL;
        }
        return label;
    } else {
        printf("[Error] - Syntax error, label must be defined with ':' in the end, line %d", lineNumber);
    }

    return NULL;
}

int isCommandExists(char *command, int *numOfOperands) {
    int i = 0;
    while (asm_commands[i].key) {
        if (strcmp(command, asm_commands[i].key) == 0) {
            *numOfOperands = asm_commands[i].value.numParams;
            return i;
        }
        i++;
    }
    return -1;
}

int isRegister(char *operand) {
    int i = 0;
    while (registers[i]) {
        if (strcmp(operand, registers[i]) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

int isValueNumber(char *operand) {
    char *sighNumber;
    sighNumber = strchr(operand, '#');

    if (sighNumber != NULL) {
        sighNumber++;
        if (strchr(sighNumber, '+') != NULL || strchr(sighNumber, '-') != NULL) {
            sighNumber++;
            if (isNumber(sighNumber)) {
                return 1;
            }
        } else {
            if (isNumber(sighNumber)) {
                return 1;
            }
        }
    }
    return 0;
}

int validateOperand(char *operand, int *addressType) {

    if (isRegister(operand) != -1) {
        *addressType = REG_ADDRESSING;
        return 1;
    }
    if (isAlphaNumeric(operand)) {
        *addressType = DIRECT_ADDRESSING;
        return 1;
    }
    if (isValueNumber(operand)) {
        *addressType = IMMEDIATE_ADDRESSING;
        return 1;
    }
    return 0;
}


void parseTwoOperands(char *operands, char **firstOperand, char **secondOperand) {
    char *separator, *firstOp = NULL, *originalStr = operands;
    int counter = 0;

    skipWhitesSpaces(operands);
    separator = strchr(operands, ',');

    if (separator != NULL) {

        while (*operands != ',') {
            operands++;
            counter++;
        }
        *firstOperand = (char *) malloc(counter * sizeof(char));
        if (*firstOperand != NULL) {
            strncpy(*firstOperand, originalStr, counter);
            *secondOperand = (separator + 1);
        }
    }

}

void parseOneOperand(char *operands, char **oneOperand) {
    int counter = 0;
    char *orgOperand = operands;
    while (*operands != '\0') {
        operands++;
        counter++;
    }
    *oneOperand = (char *) malloc(sizeof(char) * counter);
    if (orgOperand != NULL) {
        strncpy(*oneOperand, orgOperand, counter);
    }

}

void calculateICAddress(int addressType, int *IC) {
    switch (addressType) {

        case IMMEDIATE_ADDRESSING:
        case DIRECT_ADDRESSING:
        case RELATIVE_ADDRESSING:
            *IC = *IC + 2;
            break;
        case REG_ADDRESSING:
            *IC = *IC + 1;
            break;
        default:
            break;

    }
}


int isJmpCommand(char *command) {
    return strcmp(command, "jmp");
}

int parseCommand(char *line, char **command, int lineNumber, int *IC) {
    int i = 0, counter = 0, len = 0, numOfOperand = 0, addressingType;
    char *command_operands = NULL, *parserCommand = NULL, *originalCommandLine = NULL, *operands = NULL, *firstOp = NULL, *secondOp = NULL;

    /*Skip label if exists and skip white or tab spaces*/
    if (*line != ' ') {
        command_operands = skipLabel(line);
        command_operands = skipWhitesSpaces(command_operands);
    } else {
        command_operands = skipWhitesSpaces(line);
    }
    originalCommandLine = command_operands;

    while (*command_operands != ' ' && *command_operands != '\r') {
        command_operands++;
        counter++;
    }
    /* if no spaces between command and operands, return 0 for throw exception */
    if (counter == 0) {
        printf("[ERROR] - No spaces between command and operands, line %d", lineNumber);
        return 0;
    }

    parserCommand = (char *) malloc(sizeof(char) * (counter + 1));
    strncpy(parserCommand, originalCommandLine, counter);
    parserCommand[counter] = 0;
    *command = parserCommand;

    if (strchr(*command, '.') != NULL) return 0;

    if (!isCommandExists(*command, &numOfOperand)) {
        printf("[ERROR] - Not found %s command,line %d  ", *command, lineNumber);
        free(parserCommand);
        return 0;
    } else {
        if (numOfOperand >= 1) {
            counter = 0;
            operands = (char *) malloc(sizeof(char) * (strlen(command_operands) - counter));
            while (*command_operands != '\r') {
                command_operands++;
                operands[counter++] = *command_operands;
            }
            operands[counter - 1] = '\0';

        }
        switch (numOfOperand) {
            case 0:

                break;
            case 1:
                // get operand after a command
                if (!isJmpCommand(*command)) {
                    addressingType = RELATIVE_ADDRESSING;
                    calculateICAddress(addressingType, IC);
                } else {
                    parseOneOperand(operands, &firstOp);
                    if (firstOp != NULL) {
                        if (validateOperand(firstOp, &addressingType)) {
                            printf("Command %s  Operand %s Address Type %d", *command, firstOp, addressingType);
                        }
                    } else {
                        printf("Command %s must have %d operands \n", *command, numOfOperand);
                    }
                    free(operands);
                }

                break;
            case 2:
                parseTwoOperands(operands, &firstOp, &secondOp);
                if (firstOp != NULL && secondOp != NULL) {
                    if (validateOperand(firstOp, &addressingType)) {
                        printf("Operand %s, Address Type %d", firstOp, addressingType);
                        calculateICAddress(addressingType, IC);
                    }
                    if (validateOperand(secondOp, &addressingType)) {
                        printf("Operand %s, Address Type %d", secondOp, addressingType);
                        calculateICAddress(addressingType, IC);
                    }
                } else {
                    printf("Command %s must have %d operands \n", *command, numOfOperand);
                }
                free(operands);
                break;
            default:
                break;
        }
    }
    return 1;
}

int validateCommandAddressType(char *command, char *operand, int operandDirection) {

}

int isExternDirective(char *line) {
    char EXTERN[] = ".extern", *originalLine,*directiveStatement,*finalDirective;
    int counter = 0, startOperandIndex = 0, i = 0;
    if (*line != ' ') {
        directiveStatement = skipLabel(line);
        directiveStatement = skipWhitesSpaces(directiveStatement);
        originalLine=directiveStatement;
        if (strchr(directiveStatement, '.')) {
            while (*directiveStatement != ' ') {
                    counter++;
                    directiveStatement++;
            }

            finalDirective=(char*)malloc(sizeof(char)*counter);
            strncpy(finalDirective,originalLine,counter);
            finalDirective[counter]=0;

            return !strcmp(finalDirective,EXTERN);
        }
    }
    return 0;
}

void populateDataDirective(int *DC,int *IC, int directiveType, char *directiveDefinedData) {
    int *snapShotMemory;
    if (allocationDataSnapShotMemory() != NULL) {
        snapShotMemory = addDataToSnapShotMemory(directiveDefinedData, directiveType, *DC);
        *IC=*IC+*DC;
        if (snapShotMemory != NULL) {
            printf("Data Added Succesfully -  DC = %d", *DC);
        } else {
            printf("[ERROR] - Can't allocate data snap shot memory ");
        }
    } else {
        printf("[ERROR] - Can't allocate data snap shot memory ");
    }

}

int parseDirective(char *line, char **data, int lineNumber, int *directiveType) {
    char DATA[] = ".data", STRING[] = ".string", *directive,*dataFixer,*copiedData;

    int i = 0, directiveSeparatorIndex = 0, dataCounter = 0;
    char *directiveStatement = NULL;
    /*Skip label if exists and skip white or tab spaces*/
    if (*line != ' ') {
        directiveStatement = skipLabel(line);
        directiveStatement = skipWhitesSpaces(directiveStatement);

        if (strchr(directiveStatement, '.')) {
            for (i = 0; i < strlen(directiveStatement); i++) {
                if (directiveStatement[i] == ' ') {
                    directiveSeparatorIndex = i;
                    break;
                }
            }
            if (directiveSeparatorIndex == 0) {
                printf("[ERROR] - No spaces found between directive and data ,line %d  ", lineNumber);
                return 0;
            } else {
                directive = malloc(sizeof(char) * directiveSeparatorIndex);
                strncpy(directive, directiveStatement, directiveSeparatorIndex);
            }
        } else {
            //printf("[ERROR] - Not found %s directive ,line %d  ", *data, lineNumber);
            return 0;
        }
        // check if a directive is .data/.string/
        *directiveType = strcmp(directive, DATA) == 0 ? DATA_DIRECTIVE : STRING_DIRECTIVE;
        if (*directiveType == DATA_DIRECTIVE || *directiveType == STRING_DIRECTIVE) {
            directiveStatement += directiveSeparatorIndex;
            directive=directiveStatement;
            while (*directiveStatement != '\r') {
                directiveStatement++;
                dataCounter++;
            }
            copiedData = malloc(sizeof(char) * (dataCounter+1));
            strncpy(copiedData, directive, dataCounter);
            copiedData[dataCounter]=0;
            *data=skipWhitesSpaces(copiedData);
            return 1;
        } else {
            printf("[ERROR] - Not found %s directive ,line %d  ", *data, lineNumber);
            free(data);
            return 0;
        }
    } else {
        return 0;
    }

}





