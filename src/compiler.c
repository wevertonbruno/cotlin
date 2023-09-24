#include <stdio.h>

#include "compiler.h"
#include "common.h"
#include "scanner.h"

void compile(const char* source) {
    Scanner scanner = scannerNew(source);
    int line = -1;
    while (true) {
        Token token = scanToken(&scanner);
        if(token.location.line != line) {
            printf("%4d ", token.location.line);
            line = token.location.line;
        } else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);
        if (token.type == TOKEN_EOF) break;
    }
}