//
// Created by Igork on 01/06/2020.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MemorySnapShot.h"
#include "Constanst.h"
#include "DataStructers.h"

const char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
const HashMap asm_commands[] =
        {
                {"mov",  {0,  0,  2}},
                {"cmp",  {1,  0,  2}},
                {"add",  {2,  1,  2}},
                {"sub",  {2,  2,  2}},
                {"lea",  {4,  0,  2}},
                {"clr",  {5,  1,  1}},
                {"not",  {5,  2,  1}},
                {"inc",  {5,  3,  1}},
                {"dec",  {5,  4,  1}},
                {"jmp",  {9,  1,  1}},
                {"bne",  {9,  2,  1}},
                {"jsr",  {9,  3,  1}},
                {"red",  {12, -1, 0}},
                {"prn",  {13, -1, 1}},
                {"rst",  {14, -1, 0}},
                {"stop", {15, -1, 0}},
                {NULL}
        };

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

int isJumpToLabelSymbol(const char *str) {
    return *str == '&';
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

int isComment(const char *line) {
    if (*line == ';')
        return 1;
    return 0;
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
    while (asm_commands[i].key) {
        if (strcmp(command, asm_commands[i].key) == 0) {
            *numOfOperands = asm_commands[i].value.numParams;
            return i;
        }
        i++;
    }
    return -1;
}

HashMap commandOpCode_functCode(char *command) {

    HashMap command_obj = {.value=-1};
    int i = 0;
    while (asm_commands[i].key) {
        if (strcmp(command, asm_commands[i].key) == 0) {
            command_obj = asm_commands[i];
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

    HashMap opCode = {0};
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
        printf("[ERROR] line %d: No data in string directive \n", lineNumber);
        (*errorCounter)++;
    } else {
        printf("[ERROR] line %d: string must start and end with quotes \n", lineNumber);
        (*errorCounter)++;
    }
    return 1;
}

int numberValidation(char *number, int *value, int lineNumber, int *errorCounter) {

    int maxNum = (1 << MEMORY_WORD_SIZE) - 1;
    char *end;
    int valueLocal = 0;
    if (value != NULL) {
        *value = (int) strtol(number, &end, 10);
        if (*value > maxNum || *value < -maxNum) {
            printf(" [ERROR] line  %d: %s is %s , the number must be in the range %d - %d \n", lineNumber, number,
                   value > 0 ? "bigger" : "smaller", -maxNum,
                   maxNum);
            return 0;
        }
    } else {
        valueLocal = (int) strtol(number, &end, 10);
        if (valueLocal > maxNum || valueLocal < -maxNum) {
            printf(" [ERROR] line  %d:%s is %s , the number must be in the range %d - %d \n", lineNumber, number,
                   value > 0 ? "bigger" : "smaller", -maxNum,
                   maxNum);
            return 0;
        }
    }
    if (end && *end != '\0') {
        printf("[ERROR] line  %d: %s is not a valid number \n ", lineNumber, number);
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
            if (numberValidation(operand, value, line, errorCounter)) {
                return 1;
            }
        } else if (*operand == '-') {
            if (numberValidation(operand, value, line, errorCounter)) {
                return 1;
            }
        } else {
            if (numberValidation(operand, value, line, errorCounter)) {
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
        *operandType = 0;
        return 1;
    } else if (isJumpToLabelSymbol(operand)) { // if the operand have a & symbol
        operand++;
        if (isAlphaNumeric(operand)) {
            *addressType = RELATIVE_ADDRESSING;
            *operandType = 2;
            return 1;
        }
    } else if (isAlphaNumeric(operand)) {
        *addressType = DIRECT_ADDRESSING;
        *operandType = 2;
        return 1;
    } else if (isValueNumber(operand, value, line, errorCounter)) {
        *operandType = 1;
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
    return strcmp(command, "jmp");
}


void createMachineCode(char *firstOp, HashMap *commandObj, int destAddressType, int valueDest, int operandTypeDest,
                       int destExtraWord, int destOperand, int IC, int wordLength) {
    if ((*commandObj).key != NULL) {

        switch (operandTypeDest) {
            case REGISTER_TYPE: // REGISTER TYPE
                destOperand = isRegister(firstOp);
                convertInstructionToMachineCode((*commandObj).value.opCode, (*commandObj).value.funct, 0, 0,
                                                destOperand,
                                                destAddressType, IC, wordLength);
                break;
            case NUMBER_TYPE: // NUMBER TYPE
                (destExtraWord) = valueDest; // convert to binary
                convertExtraValueToMachineCode(destExtraWord, operandTypeDest, -1);
                break;
            case LABEL_TYPE: // LABEL TYPE
                destExtraWord = 0;
                // add ascii code of symbol ?  in label type
                convertExtraValueToMachineCode((int) '?', operandTypeDest, -1);
                break;
            default:
                break;

        }

    }
}

void createMachineCode2(char *command, HashMap *commandObj, int sourceAddressType, int destAddressType, int valueSrc,
                        int valueDest, int operandTypeSrc, int operandTypeDest, int destOperand, int srcOperand,int IC,int wordLength) {
    (*commandObj) = commandOpCode_functCode(command);
    if ((*commandObj).key != NULL) {
        if (operandTypeDest == REGISTER_TYPE && operandTypeSrc == REGISTER_TYPE) {
            convertInstructionToMachineCode((*commandObj).value.opCode, (*commandObj).value.funct, srcOperand,
                                            sourceAddressType, destOperand,
                                            destAddressType,IC,wordLength);
        }

        if (operandTypeSrc == NUMBER_TYPE) {
            convertExtraValueToMachineCode(valueSrc, sourceAddressType, -1);
        }
        if (operandTypeDest == NUMBER_TYPE) {
            convertExtraValueToMachineCode(valueDest, destAddressType, -1);
        }
        if (operandTypeSrc == LABEL_TYPE) {
            convertExtraValueToMachineCode(0, operandTypeSrc, -1);
        }
        if (operandTypeDest == LABEL_TYPE) {
            convertExtraValueToMachineCode(0, operandTypeDest, -1);
        }

    }
}

int parseOperands(char *operands, char *command, int numOfOperand, int lineNumber, int *IC, int *errorCounter) {
    char *firstOp = NULL, *secondOp = NULL;
    HashMap commandObj;
    int sourceAddressType = -1, destAddressType = -1, srcOffset = 0, destOffset = 0, valueSrc = 0, valueDest = 0;
    int operandTypeSrc = -1, operandTypeDest = -1; // 0 REGISTER, 1 NUMBER, 2 LABEL
    int destExtraWord = 0, destOperand = 0, srcOperand = 0;
    int countOfWord = 0;

    switch (numOfOperand) {
        case 0:
            ++(*IC);
            if (!validateCommandAddressType(command, sourceAddressType, destAddressType)) {
                printf("[ERROR] line %d: No operand needed to %s command", lineNumber, command);
                (*errorCounter)++;
            }
            commandObj = commandOpCode_functCode(command);
            if (commandObj.key != NULL) {
                convertInstructionToMachineCode(commandObj.value.opCode, commandObj.value.funct, 0, 0, 0, 0,*IC,0);
            }
            return 1;
        case 1:
            // get operand after a command
            if (!isJmpCommand(command)) {
                destAddressType = RELATIVE_ADDRESSING;
                destOffset = calculateOffsetAddress(destAddressType);
                if (!validateCommandAddressType(command, sourceAddressType, destAddressType)) {
                    printf("[ERROR] line %d: Too many operands  to %s command", lineNumber, command);
                    (*errorCounter)++;
                }
            } else {
                parseOneOperand(operands, &firstOp);
                if (firstOp != NULL) {
                    if (validateOperand(firstOp, &destAddressType, lineNumber, errorCounter, &valueDest,
                                        &operandTypeDest) != -1) {
                        destOffset = calculateOffsetAddress(destAddressType);
                        if (destOffset == -1) {
                            printf("[ERROR] line %d: Addressing type id  %d invalid \n", lineNumber, destAddressType);
                            (*errorCounter)++;
                        }
                        if (!validateCommandAddressType(command, sourceAddressType, destAddressType)) {
                            printf("[ERROR] line %d: Too many operands  to %s command\n", lineNumber, command);
                            (*errorCounter)++;
                        }
                    } else {
                        printf(" [ERROR] line %d: Operand %s invalid \n", lineNumber, firstOp);
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
            commandObj = commandOpCode_functCode(command);
            createMachineCode(firstOp, &commandObj, destAddressType, valueDest, operandTypeDest, destExtraWord,
                              destOperand,*IC,destOffset);
            *IC += destOffset + 1;

            return 1;
        case 2:

            parseTwoOperands(operands, &firstOp, &secondOp);
            if (firstOp != NULL) {
                if (validateOperand(firstOp, &sourceAddressType, lineNumber, errorCounter, &valueSrc,
                                    &operandTypeSrc) != -1) {
                    srcOffset = calculateOffsetAddress(sourceAddressType);
                    if (srcOffset == -1) {
                        printf("[ERROR] line %d: Addressing type id  %d invalid \n", lineNumber, sourceAddressType);
                        (*errorCounter)++;

                    }
                } else {
                    printf("[ERROR] line %d: Operand %s invalid \n", lineNumber, firstOp);
                    (*errorCounter)++;
                }
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
                printf("[ERROR] line %d: Too many operands  to %s command \n", lineNumber, command);
                (*errorCounter)++;
            }
            if (sourceAddressType == REG_ADDRESSING && destAddressType == REG_ADDRESSING) {
                destOffset = 0;
                srcOffset = 0;

            } else if (sourceAddressType == REG_ADDRESSING && destAddressType != REG_ADDRESSING) {
                srcOffset = 0;
            } else if (sourceAddressType != REG_ADDRESSING && destAddressType == REG_ADDRESSING) {
                destOffset = 0;
            }


            if (operandTypeDest == REGISTER_TYPE && operandTypeSrc == REGISTER_TYPE) {
                destOperand = isRegister(firstOp);
                srcOperand = isRegister(secondOp);
            } else if (operandTypeSrc == REGISTER_TYPE && operandTypeDest != REGISTER_TYPE) {

                srcOperand = isRegister(firstOp);
                destAddressType = 0;
            } else if (operandTypeSrc != REGISTER_TYPE && operandTypeDest == REGISTER_TYPE) {
                destOperand = isRegister(secondOp);
                sourceAddressType = 0;
            }
            countOfWord=srcOffset+destOffset;
            createMachineCode2(command, &commandObj, sourceAddressType, destAddressType, valueSrc, valueDest,
                               operandTypeSrc, operandTypeDest,
                               destOperand, srcOperand,*IC,countOfWord);
            *IC += srcOffset + destOffset + 1;
            return 1;
        default:
            break;
    }

    return 1;

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
        *errorCounter++;
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
        if (*numOfOperand >= 1) {
            counter = 0;
            tmp_operands = (char *) malloc(sizeof(char) * (strlen(command_operands) + 1));
            command_operands++;
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

int checkIsDirective(char *line, const char *originalLine, char *finalDirective, int *counter, const char *type) {
    while (*line != ' ') {
        (*counter)++;
        line++;
    }
    finalDirective = (char *) malloc(sizeof(char) * (*counter));
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

    char *originalLine, *directiveStatement, *finalDirective = NULL;
    int counter = 0;
    if (strchr(line, '.')) {
        originalLine = line;
        if (checkIsDirective(line, originalLine, finalDirective, &counter, ENTRY)) {
            extractOperand(line, labelEntry, originalLine, counter);
            return 1;
        }
    } else {
        directiveStatement = skipLabel(line);
        directiveStatement = skipWhitesSpaces(directiveStatement);
        originalLine = directiveStatement;
        if (strchr(directiveStatement, '.')) {
            if (checkIsDirective(directiveStatement, originalLine, finalDirective, &counter, ENTRY)) {
                extractOperand(line, labelEntry, originalLine, counter);
                return 1;
            }
        }
    }
    return 0;
}


int isExternDirective(char *line, char **label, int *errorCounter) {
    char *originalLine, *directiveStatement, *finalDirective = NULL;
    int counter = 0;
    if (strchr(line, '.')) {
        originalLine = line;
        if (checkIsDirective(line, originalLine, finalDirective, &counter, EXTERN)) {
            extractOperand(line, label, originalLine, counter);
            return 1;
        }
    } else {
        directiveStatement = skipLabel(line);
        directiveStatement = skipWhitesSpaces(directiveStatement);
        originalLine = directiveStatement;
        if (strchr(directiveStatement, '.')) {

            if (checkIsDirective(directiveStatement, originalLine, finalDirective, &counter, EXTERN)) {
                extractOperand(line, label, originalLine, counter);
                return 1;
            }
        }
    }
    return 0;
}


int populateDataDirective(int *DC, int directiveType, char *directiveDefinedData, int *errorCounter, int linerNumber) {
    int *snapShotMemory, deltaDataCounter = 0;
    snapShotMemory = saveToSnapShotMemory(directiveDefinedData, directiveType, DC, &deltaDataCounter, errorCounter,
                                          linerNumber);
    if (snapShotMemory == NULL) {

        *errorCounter++;
        printf("[ERROR] line %d : Can't add data to data memory block\n ", linerNumber);
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
            printf("[ERROR] line %d:  No spaces found between directive and data \n ", lineNumber);
            *errorsCounter++;
            return 0;
        } else {
            directive = malloc(sizeof(char) * directiveSeparatorIndex);
            strncpy(directive, directiveStatement, directiveSeparatorIndex);
        }
    } else {
        return 0;
    }
    // check if a directive is .data/.string/
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
        printf("[ERROR] line %d :  Not found %s directive \n", lineNumber, *directiveType == 1 ? "code" : "data");
        *errorsCounter++;
        free(*data);
        return 0;
    }
}

int convertToBase2(int number, int size) {

}








