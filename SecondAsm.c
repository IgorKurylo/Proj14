//
// Created by Igork on 19/07/2020.
//

#include "FileMethods.h"
#include "HelpersMethods.h"

int secondRead(AsmFileContent asmContentFile, int *IC, int *DC, int lineNumber) {

    char *string;
    int errorCounter = 0, directiveType = 0;
    if (isExternDirective(asmContentFile.line, &string, &errorCounter)) {
        return errorCounter;
    }
    if (parseDirective(asmContentFile.line, &string, lineNumber, &directiveType, &errorCounter)) {
        if (directiveType == DATA_DIRECTIVE || directiveType == STRING_DIRECTIVE)
            return errorCounter;
    }
    if (isEntryDirective(asmContentFile.line, &string)) {
        printf("%s", string);
    }
    return errorCounter;

}