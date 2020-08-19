//
// Created by Igork on 01/06/2020.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MemorySnapShot.h"
#include "Constanst.h"
#include "HelpersMethods.h"
#include "ctype.h"

const char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
const Command commands[] =
        {
                {"mov",  {0,  0, 2}},
                {"cmp",  {1,  0, 2}},
                {"add",  {2,  1, 2}},
                {"sub",  {2,  2, 2}},
                {"lea",  {4,  0, 2}},
                {"clr",  {5,  1, 1}},
                {"not",  {5,  2, 1}},
                {"inc",  {5,  3, 1}},
                {"dec",  {5,  4, 1}},
                {"jmp",  {9,  1, 1}},
                {"bne",  {9,  2, 1}},
                {"jsr",  {9,  3, 1}},
                {"red",  {12, 0, 0}},
                {"prn",  {13, 0, 1}},
                {"rst",  {14, 0, 0}},
                {"stop", {15, 0, 0}},
                {NULL}
        };


void adjustOffsetOfSrcDest(int sourceAddressType, int destAddressType, int *srcOffset, int *destOffset);

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
    if (line != NULL) {
        lineEmpty = skipWhitesSpaces(line);
        if (*lineEmpty == ' ' || *lineEmpty == '\t' || *lineEmpty == '\n') {
            return 1;
        }
    }
    return 0;
}

int isComment(char *line) {
    if (*line == ';')
        return 1;
    return 0;
}

int convertToBase2(int value) {
    int carry = 0, index = 15, counter = 0;
    char b[15];
    while (value > 0) {
        carry = value % 2;
        value /= 2;
        b[index--] = carry;
        counter++;
    }
    for (int i = 0; i < 15; i++) {
        printf("%d", b[i]);
    }

}

int convertTo2Complement(int value) {

    if (value < 0) {
        value *= -1;
        value = ~value;
        value += 1;
        return value;
    } else {
        return value;
    }
}


char *parseLabel(char *line, char **labelName, int lineNumber, int *errorCounter) {
    char *label = NULL;
    char *originalLine = line;
    int count = 0;
    if (strchr(originalLine, ':')) {
        while (*line != ':' && *line != ' ') {
            line++;
            count++;
        }
        if (count == 0) {
            printf("[ERROR] line %d: Label can't start with spaces \n", lineNumber);
            *errorCounter++;
            return NULL;
        }
        if (count > MAX_SYMBOL_SIZE) {
            printf("[ERROR] line %d: Label too large only %d characters \n", lineNumber, MAX_SYMBOL_SIZE);
            *errorCounter++;
            return NULL;
        }
        label = malloc(sizeof(char) * (count + 1));
        if (!label) {
            printf("[FATAL] Allocation  of memory fail\n");
            *errorCounter++;
            return NULL;
        }
        strncpy(label, originalLine, count);
        label[count] = 0;
        *labelName = label;
        if (!isAlphaNumeric(label)) {
            printf("[ERROR] line %d: Syntax error, label must be alpha numeric \n", lineNumber);
            *errorCounter++;
            return NULL;
        }
        return label;
    } else {
        return NULL;
    }
}

int isCommandExists(char *command, int *numOfOperands) {
    int i = 0;
    while (commands[i].key) {
        if (strcmp(command, commands[i].key) == 0) {
            *numOfOperands = commands[i].value.numParams;
            return i;
        }
        i++;
    }
    return -1;
}

Command commandOpCode_functCode(char *command) {

    Command command_obj = {.value=-1};
    int i = 0;
    while (commands[i].key) {
        if (strcmp(command, commands[i].key) == 0) {
            command_obj = commands[i];
        }
        i++;
    }
    return command_obj;
}

int isRegister(char *operand) {
    int i = 0;
    while (i < NUMBER_OF_REGISTERS) {
        if (strcmp(operand, registers[i]) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

int validateCommandAddressType(char *command, int addressTypeSrc, int addressTypeDest) {

    Command opCode = {0};
    int command_opCode = 0;
    opCode = commandOpCode_functCode(command);
    if (opCode.value.opCode != -1) {
        command_opCode = opCode.value.opCode;

        switch (command_opCode) {
            case 0: //mov
                return (addressTypeSrc == IMMEDIATE_ADDRESSING || addressTypeSrc == DIRECT_ADDRESSING ||
                        addressTypeSrc == REG_ADDRESSING) &&
                       (addressTypeDest == DIRECT_ADDRESSING || addressTypeDest == REG_ADDRESSING);

            case 1: //cmp
                return (addressTypeSrc == IMMEDIATE_ADDRESSING || addressTypeSrc == DIRECT_ADDRESSING ||
                        addressTypeSrc == REG_ADDRESSING) &&
                       (addressTypeDest == IMMEDIATE_ADDRESSING || addressTypeDest == DIRECT_ADDRESSING ||
                        addressTypeDest == REG_ADDRESSING);
                break;
            case 2: //add,sub
                return (addressTypeSrc == IMMEDIATE_ADDRESSING || addressTypeSrc == DIRECT_ADDRESSING ||
                        addressTypeSrc == REG_ADDRESSING) &&
                       (addressTypeDest == DIRECT_ADDRESSING || addressTypeDest == REG_ADDRESSING);

                break;
            case 4: //lea
                return (addressTypeSrc == DIRECT_ADDRESSING) &&
                       (addressTypeDest == DIRECT_ADDRESSING || addressTypeDest == REG_ADDRESSING);
                break;
            case 5: //clr,not,inc,dec:
                return (addressTypeDest == DIRECT_ADDRESSING || addressTypeDest == REG_ADDRESSING);

            case 9: // jmp,bne,jsr
                return (addressTypeDest == DIRECT_ADDRESSING || addressTypeDest == RELATIVE_ADDRESSING);

            case 12: //red
                return (addressTypeDest == DIRECT_ADDRESSING || addressTypeDest == REG_ADDRESSING);

            case 13: //prn
                return (addressTypeDest == IMMEDIATE_ADDRESSING || addressTypeDest == DIRECT_ADDRESSING ||
                        addressTypeDest == REG_ADDRESSING);

            case 14: //rts
            case 15: //stop
                return addressTypeDest == -1 || addressTypeSrc == -1;

            default:
                break;
        }
    }
}


int stringValidation(char **string, int lineNumber, int *errorCounter) {
    // if quotes detected, remove it from a string
    if (**string == '"' && (*string)[strlen(*string) - 1] == '"') {
        (*string)[strlen(*string) - 1] = '\0';
        ++*string;
    } else if (**string == '\0') {
        printf("[ERROR] line %d: No Data in string directive \n", lineNumber);
        (*errorCounter)++;
    } else if (isprint(**string)) {
        printf("[ERROR] line %d: string contains not printable characters, %c \n", lineNumber, **string);
        (*errorCounter)++;
    } else {
        printf("[ERROR] line %d: string must start and end with quotes \n", lineNumber);
        (*errorCounter)++;
    }
    return 1;
}

int numberValidation(char *number_value, int memorySize, int *value, int lineNumber, int *errorCounter) {

    int maxNum = (1 << (memorySize - 1));
    char *end;
    int valueLocal = 0;
    if (value != NULL) {
        *value = (int) strtol(number_value, &end, 10);
        if (*value >= maxNum || *value <= -maxNum) {
            (*errorCounter)++;
            printf("[ERROR] line %d: %s is %s, the number must be in the range [%d to %d] \n", lineNumber,
                   number_value,
                   *value > 0 ? "bigger" : "smaller", -maxNum,
                   maxNum);

            return 0;
        }
    } else {
        valueLocal = (int) strtol(number_value, &end, 10);
        if (valueLocal >= maxNum || valueLocal <= -maxNum) {
            (*errorCounter)++;
            printf("[ERROR] line  %d:%s is %s ,the number must be in the range %d to %d \n", lineNumber,
                   number_value,
                   *value > 0 ? "bigger" : "smaller", -maxNum,
                   maxNum);
            return 0;
        }
    }
    if (end && *end != '\0') {
        printf("[ERROR] line  %d: %s is not a valid number \n ", lineNumber, number_value);
        (*errorCounter)++;
        return 0;
    }
    return 1;
}

int isValueNumber(char *operand, int *value, int line, int *errorCounter) {
    char *sighNumber;
    sighNumber = strchr(operand, '#');
    if (sighNumber != NULL) {
        operand++;
        if (*operand == '+') {
            operand++;

            if (numberValidation(operand, MEMORY_WORD_SIZE, value, line, errorCounter)) {
                return 1;
            }
        } else if (*operand == '-') {

            if (numberValidation(operand, MEMORY_WORD_SIZE, value, line, errorCounter)) {
                return 1;
            }
        } else {

            if (numberValidation(operand, MEMORY_WORD_SIZE, value, line, errorCounter)) {
                return 1;
            }
        }
    } else {
        printf("[ERROR] line %d: %s invalid operand \n", line, operand);
        (*errorCounter)++;
        return 0;
    }
    return 0;
}


int validateOperand(char *operand, int *addressType, int line, int *errorCounter, int *value, int *operandType) {

    if (isRegister(operand) != -1) {
        *addressType = REG_ADDRESSING;
        *operandType = register_operand;
        return 1;

    } else if (isAlphaNumeric(operand)) {
        *addressType = DIRECT_ADDRESSING;
        *operandType = label_operand;
        return 1;
    } else if (isValueNumber(operand, value, line, errorCounter)) {
        *operandType = number_operand;
        *addressType = IMMEDIATE_ADDRESSING;
        return 1;
    }
    return -1;
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

void validateImmediateSize(int lineNumber, int *errorCounter, char *operand, int addressType, int *value) {
    if (addressType == IMMEDIATE_ADDRESSING) {
        if (strchr(operand, '#') != NULL) {
            operand++;
        }
        if (!numberValidation(operand, MEMORY_WORD_SIZE - 3, value, lineNumber, errorCounter)) {
            printf("[ERROR] line %d: Invalid %s number size,too %s to fit in %d bits\n", lineNumber, operand,
                   (*value > 0) ? "high" : "low", MEMORY_WORD_SIZE - 3); // 21 bits
        }
    }

}

int calculateOffsetAddress(int addressType) {
    int offSet = 0;
    switch (addressType) {

        case IMMEDIATE_ADDRESSING:
        case DIRECT_ADDRESSING:
        case REG_ADDRESSING:
        case RELATIVE_ADDRESSING:
            offSet = 1;
            break;
        case -1:
            offSet = -1;
            break;
    }
    return offSet;
}


int isJmpCommand(char *command) {
    return strcmp(command, "jmp") == 0 || strcmp(command, "bne") == 0 || strcmp(command, "jsr") == 0;
}

int validateJmpTypesCommandOperand(char *operand, int lineNumber, int *errorCounter) {

    if (*operand == '&' && !isAlphaNumeric(operand + 1)) {
        printf("[ERROR] line %d: Incomplete relative %s\n", lineNumber, operand);
        (*errorCounter)++;
        return 0;
    }
    if (*operand == '&' && *(operand + 1) == '&') {
        printf("[ERROR] line %d: Invalid characters %s\n", lineNumber, operand);
        (*errorCounter)++;
        return 0;
    }
    return 1;

}


int parseOperands(char *operands, char *command, int numOfOperand, int lineNumber, int *IC, int *errorCounter) {
    char *firstOp = NULL, *secondOp = NULL;
    int sourceAddressType = -1, destAddressType = -1, srcOffset = 0, destOffset = 0, valueSrc = 0, valueDest = 0;
    int operandTypeSrc = -1, operandTypeDest = -1; // 0 REGISTER, 1 NUMBER, 2 LABEL
    switch (numOfOperand) {
        case 0:
            if (operands == NULL) {
                ++(*IC);
            } else {
                printf("[ERROR] line %d: Invalid operand, no operand needed to %s command\n", lineNumber, command);
                (*errorCounter)++;
            }
            return 1;
        case 1:
            // if is from JMP command bne,jmp ,jsr, validate relative and calculate
            if (isJmpCommand(command)) {
                if (validateJmpTypesCommandOperand(operands, lineNumber, errorCounter)) {
                    destAddressType = RELATIVE_ADDRESSING;
                    destOffset = calculateOffsetAddress(destAddressType);
                    if (!validateCommandAddressType(command, sourceAddressType, destAddressType)) {
                        printf("[ERROR] line %d: Invalid addressing in %s command", lineNumber, command);
                        (*errorCounter)++;
                    }
                    *IC += destOffset + 1;
                }
                return 1;
            } else {
                parseOneOperand(operands, &firstOp);
                if (firstOp != NULL) {
                    if (validateOperand(firstOp, &destAddressType, lineNumber, errorCounter, &valueDest,
                                        &operandTypeDest) != -1) {
                        destOffset = calculateOffsetAddress(destAddressType);
                        if (destOffset == -1) {
                            printf("[ERROR] line %d: Addressing type id %d invalid \n", lineNumber, destAddressType);
                            (*errorCounter)++;
                        }
                        if (!validateCommandAddressType(command, sourceAddressType, destAddressType)) {
                            printf("[ERROR] line %d: Invalid addressing in %s command\n", lineNumber, command);
                            (*errorCounter)++;
                        }
                    } else {
                        printf("[ERROR] line %d: Operand %s invalid \n", lineNumber, firstOp);
                        (*errorCounter)++;
                    }
                } else {
                    printf("[ERROR] line %d: Command %s must have %d operands \n", lineNumber, command, numOfOperand);
                    (*errorCounter)++;
                }

            }
            if (destAddressType == REG_ADDRESSING) {
                destOffset = 0;
            }
            *IC += destOffset + 1;

            return 1;
        case 2:

            parseTwoOperands(operands, &firstOp, &secondOp);
            if (firstOp != NULL) {
                if (validateOperand(firstOp, &sourceAddressType, lineNumber, errorCounter, &valueSrc,
                                    &operandTypeSrc) != -1) {
                    srcOffset = calculateOffsetAddress(sourceAddressType);
                    if (srcOffset == -1) {
                        printf("[ERROR] line %d: Addressing type id %d invalid \n", lineNumber, sourceAddressType);
                        (*errorCounter)++;

                    }
                } else {
                    printf("[ERROR] line %d: Operand %s invalid \n", lineNumber, firstOp);
                    (*errorCounter)++;
                }
                validateImmediateSize(lineNumber, errorCounter, firstOp, sourceAddressType, &valueSrc);
            }
            if (secondOp != NULL) {
                if (validateOperand(secondOp, &destAddressType, lineNumber, errorCounter, &valueDest,
                                    &operandTypeDest) != -1) {
                    destOffset = calculateOffsetAddress(destAddressType);
                    if (destOffset == -1) {
                        printf("[ERROR] line %d: Addressing type id  %d invalid \n", lineNumber, destAddressType);
                        (*errorCounter)++;
                    }
                } else {
                    printf("[ERROR] line %d: Operand %s invalid \n", lineNumber, secondOp);
                    (*errorCounter)++;
                }
            } else {
                printf("[ERROR] line %d: Command %s must have %d operands \n", lineNumber, command, numOfOperand);
                (*errorCounter)++;

            }
            if (!validateCommandAddressType(command, sourceAddressType, destAddressType)) {
                printf("[ERROR] line %d: Invalid addressing in %s command \n", lineNumber, command);
                (*errorCounter)++;
            }
            validateImmediateSize(lineNumber, errorCounter, secondOp, destAddressType, &valueDest);

            adjustOffsetOfSrcDest(sourceAddressType, destAddressType, &srcOffset, &destOffset);

            *IC += srcOffset + destOffset + 1;
            return 1;
        default:
            break;
    }

    return 1;

}

void adjustOffsetOfSrcDest(int sourceAddressType, int destAddressType, int *srcOffset, int *destOffset) {
    if (sourceAddressType == REG_ADDRESSING && destAddressType == REG_ADDRESSING) {
        (*destOffset) = 0;
        (*srcOffset) = 0;

    } else if (sourceAddressType == REG_ADDRESSING && destAddressType != REG_ADDRESSING) {
        (*srcOffset) = 0;
    } else if (sourceAddressType != REG_ADDRESSING && destAddressType == REG_ADDRESSING) {
        (*destOffset) = 0;
    }
}


int parseCommand(char *line, char **command, int lineNumber, int *numOfOperand, int *errorCounter, char **operands) {
    int counter = 0;
    char *command_operands = NULL, *parserCommand = NULL, *originalCommandLine = NULL, *tmp_operands = NULL;

    /*Skip label if exists and skip white or tab spaces*/
    if (*line != ' ') {
        command_operands = skipLabel(line);
        command_operands = skipWhitesSpaces(command_operands);
    } else {
        command_operands = skipWhitesSpaces(line);
    }
    originalCommandLine = command_operands;

    while (*command_operands != ' ' && *command_operands != '\n') {
        command_operands++;
        counter++;
    }
    /* if no spaces between command and operands, return 0 for throw exception */
    if (counter == 0) {
        printf("[ERROR] line %d: No spaces between command and operands\n", lineNumber);
        (*errorCounter)++;
        return 0;
    }
    parserCommand = (char *) malloc(sizeof(char) * (counter + 1));
    strncpy(parserCommand, originalCommandLine, counter);
    parserCommand[counter] = 0;
    *command = parserCommand;

    if (isCommandExists(*command, numOfOperand) == -1) {
        printf("[ERROR] line %d: Not found %s command \n ", lineNumber, *command);
        *errorCounter++;
        free(parserCommand);
        return 0;
    } else {
        command_operands++;
        if (strcmp(command_operands, "") != 0) {
            counter = 0;
            tmp_operands = (char *) malloc(sizeof(char) * (strlen(command_operands) + 1));

            while (*command_operands != '\n') {
                tmp_operands[counter++] = *command_operands;
                command_operands++;
            }
            tmp_operands = skipWhitesSpaces(tmp_operands);
            tmp_operands[counter] = '\0';
            *operands = tmp_operands;
        }
        return 1;
    }
}

void extractOperand(char *line, char **label, char *originalLine, int counter) {
    line += (counter + 1);
    originalLine += (counter + 1);
    counter = 0;
    while (*line != '\n') {
        counter++;
        line++;
    }
    *label = (char *) malloc(sizeof(char) * (counter + 1));
    strncpy(*label, originalLine, counter);
}

int checkIsDirective(char *line, const char *originalLine, int *counter, const char *type) {
    char *finalDirective = NULL;
    while (*line != ' ') {
        (*counter)++;
        line++;
    }
    finalDirective = (char *) malloc(sizeof(char) * (*counter) + 1);
    strncpy(finalDirective, originalLine, *counter);
    finalDirective[*counter] = 0;
    if (strcmp(finalDirective, type) == 0) {
        free(finalDirective);
        return 1;
    } else {
        free(finalDirective);
        return 0;
    }
}

int isEntryDirective(char *line, char **labelEntry) {

    char *originalLine, *directiveStatement;
    int counter = 0;
    if (strchr(line, '.')) {
        originalLine = line;
        if (checkIsDirective(line, originalLine, &counter, ENTRY)) {
            extractOperand(line, labelEntry, originalLine, counter);
            return 1;
        }
    } else {
        directiveStatement = skipLabel(line);
        directiveStatement = skipWhitesSpaces(directiveStatement);
        originalLine = directiveStatement;
        if (strchr(directiveStatement, '.')) {
            if (checkIsDirective(directiveStatement, originalLine, &counter, ENTRY)) {
                extractOperand(line, labelEntry, originalLine, counter);
                return 1;
            }
        }
    }
    return 0;
}


int isExternDirective(char *line, char **label, int *errorCounter) {
    char *originalLine, *directiveStatement;
    int counter = 0;
    if (strchr(line, '.')) {
        originalLine = line;
        if (checkIsDirective(line, originalLine, &counter, EXTERN)) {
            extractOperand(line, label, originalLine, counter);
            return 1;
        }
    } else {
        directiveStatement = skipLabel(line);
        directiveStatement = skipWhitesSpaces(directiveStatement);
        originalLine = directiveStatement;
        if (strchr(directiveStatement, '.')) {

            if (checkIsDirective(directiveStatement, originalLine, &counter, EXTERN)) {
                extractOperand(line, label, originalLine, counter);
                return 1;
            }
        }
    }
    return 0;
}

int isDataFormattingCorrect(char *directiveData, int *errorCounter, int lineNumber) {
    int i = 0, j = 0, delim_counter = 0;

    if (*directiveData == ',') {
        printf("[ERROR] line %d: Can't write char %s in start of data directive \n", lineNumber, DELIM);
        (*errorCounter)++;
        return 0;
    } else {

        if (*(directiveData + strlen(directiveData) - 1) == ',') {
            printf("[ERROR] line %d: Can't write char %s in end of data directive \n", lineNumber, DELIM);
            (*errorCounter)++;
            return 0;
        }
    }
    for (i = 0; i < strlen(directiveData); i++) {

        while (directiveData[j] >= '0' && directiveData[j] <= '9') {
            j++;
        }
        if (directiveData[j] == DELIM_CHAR) {
            delim_counter++;
            i = j;
            j++;
        }
        if (directiveData[j] == ' ') {
            j++;
            i = j;
        }
    }
    if (delim_counter >= 2) {
        printf("[ERROR] line %d: Can't write char %s twice between number in data directive\n", lineNumber, DELIM);
        (*errorCounter)++;
        return 0;
    }
    return 1;

}


int populateDataDirective(int *DC, int directiveType, char *directiveDefinedData, int *errorCounter, int linerNumber) {
    int *snapShotMemory, deltaDataCounter = 0;
    if (isDataFormattingCorrect(directiveDefinedData, errorCounter, linerNumber)) {
        snapShotMemory = saveToSnapShotMemory(directiveDefinedData, directiveType, DC, &deltaDataCounter, errorCounter,
                                              linerNumber);
        if (snapShotMemory == NULL) {

            *errorCounter++;
            printf("[ERROR] line %d : Can't add Data to Data memory block\n ", linerNumber);
        }
    }

    return deltaDataCounter;
}

int parseDirective(char *line, char **data, int lineNumber, int *directiveType, int *errorsCounter) {
    char DATA[] = ".data", STRING[] = ".string", *directive, *dataFixer, *copiedData, *copiedNumberArray = NULL;

    int i = 0, directiveSeparatorIndex = 0, dataCounter = 0;
    char *directiveStatement = NULL;
    /*Skip label if exists and skip white or tab spaces*/
    if (*line != ' ') {
        directiveStatement = skipLabel(line);
        directiveStatement = skipWhitesSpaces(directiveStatement);
    }
    if (*line == ' ') {
        directiveStatement = skipWhitesSpaces(line);
    }
    if (strchr(directiveStatement, '.')) {
        for (i = 0; i < strlen(directiveStatement); i++) {
            if (directiveStatement[i] == ' ') {
                directiveSeparatorIndex = i;
                break;
            }
        }
        if (directiveSeparatorIndex == 0) {
            printf("[ERROR] line %d:  No spaces found between directive and Data \n ", lineNumber);
            *errorsCounter++;
            return 0;
        } else {
            directive = malloc(sizeof(char) * directiveSeparatorIndex);
            strncpy(directive, directiveStatement, directiveSeparatorIndex);
        }
    } else {
        return 0;
    }
    // check if a directive is .Data/.string/
    *directiveType = strcmp(directive, DATA) == 0 ? DATA_DIRECTIVE : STRING_DIRECTIVE;
    if (*directiveType == DATA_DIRECTIVE || *directiveType == STRING_DIRECTIVE) {
        directiveStatement += directiveSeparatorIndex;
        directive = directiveStatement;
        while (*directiveStatement != '\n') {
            directiveStatement++;
            dataCounter++;
        }
        copiedData = malloc(sizeof(char) * (dataCounter + 1));
        strncpy(copiedData, directive, dataCounter);
        copiedData[dataCounter] = 0;
        *data = skipWhitesSpaces(copiedData);

        return 1;
    } else {
        printf("[ERROR] line %d :  Not found %s directive \n", lineNumber, *directiveType == 1 ? "code" : "Data");
        *errorsCounter++;
        free(*data);
        return 0;
    }
}









