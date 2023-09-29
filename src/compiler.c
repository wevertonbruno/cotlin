#include <stdio.h>

#include "compiler.h"
#include "common.h"
#include "scanner.h"

static Token nextToken(Parser *parser);
static void errorAtCurrent(Parser *);
static void errorAt(Parser *parser, Token token, const char *message);
static void error(Parser *, const char *message);

Parser parserNew(Scanner *scanner)
{
    Parser parser;
    parser.scanner = scanner;
    parser.current = parser.previous = NULL;
    parser.hadError = false;
    return parser;
}

bool compile(const char *source, Chunk *chunk)
{
    Scanner scanner = scannerNew(source);
    Parser parser = parserNew(&scanner);
    Token token = nextToken(&parser);

    // expression();
    // consume(TOKEN_EOF, "Expected end of expression");
    return !parser.hadError;
}

Token nextToken(Parser *parser)
{
    parser->previous = parser->current;

    for (;;)
    {
        parser->current = scanToken(&parser->scanner);
        if (parser->current.type != TOKEN_ERROR)
            break;

        errorAtCurrent(parser->current);
    }
}

static void errorAt(Parser *parser, Token token, const char *message)
{
    if (parser->panicMode)
        return;

    parser->panicMode = true;

    fprintf(stderr, "[line %d] Error", token.location.line);
    if (token.type == TOKEN_EOF)
        fprintf(stderr, " at end");
    else if (token.type == TOKEN_ERROR)
        fprintf(stderr, " at '%s'", token.start);
    else
        fprintf(stderr, " at '%.*s'", token.length, token.start);
    fprintf(stderr, ": %s\n", message);
    parser->hadError = true;
}

static void errorAtCurrent(Parser *parser)
{
    errorAt(parser, parser->current, parser->current.start);
}

static void error(Parser *parser, const char *message)
{
    errorAt(parser, parser->current, message);
}