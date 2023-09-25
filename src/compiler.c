#include <stdio.h>

#include "compiler.h"
#include "common.h"
#include "scanner.h"

bool compile(const char *source, Chunk *chunk)
{
    Scanner scanner = scannerNew(source);
    // advance();
    // expression();
    // consume(TOKEN_EOF, "Expected end of expression");
    return true;
}

/**
 * Debug Tokens
 * int line = -1;
    while (true)
    {
        Token token = scanToken(&scanner);
        if (token.location.line != line)
        {
            printf("(%2d,%3d) ", token.location.line, token.location.column);
            line = token.location.line;
        }
        else
        {
            printf("      |  ");
        }

        if (token.type == TOKEN_NL)
        {
            printf("%2d 'NL'\n", token.type);
        }
        else
        {
            printf("%2d '%.*s'\n", token.type, token.length, token.start);
        }

        if (token.type == TOKEN_EOF)
            break;
    }
*/