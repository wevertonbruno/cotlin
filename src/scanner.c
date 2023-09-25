#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

#define EOF_CHAR '\0'
#define NEW_LINE '\n'

static bool isEOF(Scanner *);
static Token newScanError(Scanner *scanner, const char *message);
static Token createToken(TokenType type, Scanner *scanner);
static char advance(Scanner *scanner);
static bool match(Scanner *scanner, char expected);
static bool isDigit(char c);
static bool isAlpha(char c);
static bool isAlphaNumeric(char c);
static char peek(Scanner *scanner);
static char peekNext(Scanner *scanner);
static void skipWhiteSpace(Scanner *scanner);
static void incrementLocation(Scanner *scanner);
static Token stringLiteral(Scanner *scanner);
static Token numberLiteral(Scanner *scanner);
static Token identifier(Scanner *scanner);
static Token newLine(Scanner *scanner);
static TokenType identifierType(Scanner *scanner); // Return the type of the identifier. It can be a reserved word or a variable

Scanner scannerNew(const char *source)
{
    Scanner scanner;
    scanner.start = source;
    scanner.current = source;
    scanner.location = (Location){1, 1};
    scanner.lastToken = -1;
    return scanner;
}

Token scanToken(Scanner *scanner)
{
    skipWhiteSpace(scanner);
    scanner->start = scanner->current;
    if (isEOF(scanner))
        return createToken(TOKEN_EOF, scanner);

    char current = advance(scanner);

    if (isDigit(current))
        return numberLiteral(scanner);

    if (isAlpha(current))
        return identifier(scanner);

    switch (current)
    {
    case '(':
        return createToken(TOKEN_LEFT_PAREN, scanner);
    case ')':
        return createToken(TOKEN_RIGHT_PAREN, scanner);
    case '{':
        return createToken(TOKEN_LEFT_BRACE, scanner);
    case '}':
        return createToken(TOKEN_RIGHT_BRACE, scanner);
    case ',':
        return createToken(TOKEN_COMMA, scanner);
    case '.':
        return createToken(TOKEN_DOT, scanner);
    case '-':
        return createToken(TOKEN_MINUS, scanner);
    case '+':
        return createToken(TOKEN_PLUS, scanner);
    case ';':
        return createToken(TOKEN_SEMICOLON, scanner);
    case '/':
        return createToken(TOKEN_SLASH, scanner);
    case '*':
        return createToken(TOKEN_STAR, scanner);
    case '!':
        return createToken(match(scanner, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG, scanner);
    case '=':
        return createToken(match(scanner, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL, scanner);
    case '<':
        return createToken(match(scanner, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS, scanner);
    case '>':
        return createToken(match(scanner, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER, scanner);
    case '"': // TODO - Support string interpolation
        return stringLiteral(scanner);
    case NEW_LINE:
        return newLine(scanner);
    }
    return newScanError(scanner, "Unexpected character.");
}

static bool isEOF(Scanner *scanner)
{
    return *scanner->current == EOF_CHAR;
}

static Token newScanError(Scanner *scanner, const char *message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.location = (*scanner).location;
    return token;
}

static Token createToken(TokenType type, Scanner *scanner)
{
    Token token;
    Location loc = (*scanner).location;
    int length = (int)((*scanner).current - (*scanner).start);
    token.type = type;
    token.start = (*scanner).start;
    token.length = length;
    token.location = (Location){loc.line, loc.column - length};
    scanner->lastToken = type;
    return token;
}

static char advance(Scanner *scanner)
{
    scanner->current++;
    scanner->location.column++;
    return scanner->current[-1];
}

static bool match(Scanner *scanner, char expected)
{
    if (isEOF(scanner))
        return false;
    if (*scanner->current != expected)
        return false;
    advance(scanner);
    return true;
}

static bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

static char peek(Scanner *scanner)
{
    return *scanner->current;
}

static char peekNext(Scanner *scanner)
{
    if (isEOF(scanner))
        return EOF_CHAR;
    return scanner->current[1];
}

static void skipWhiteSpace(Scanner *scanner)
{
    for (;;)
    {
        char c = peek(scanner);
        switch (c)
        {
        case ' ':
        case '\r':
        case '\t':
            advance(scanner);
            break;
        case '/':
        {
            char next = peekNext(scanner);
            if (next == '/')
            {
                while (peek(scanner) != NEW_LINE && !isEOF(scanner))
                    advance(scanner);
            }
            else if (next == '*')
            {
                // Checks for a multiline comment end
                while (!(peek(scanner) == '*' && peekNext(scanner) == '/'))
                {
                    if (isEOF(scanner))
                    {
                        return;
                    }

                    if (peek(scanner) == NEW_LINE)
                        incrementLocation(scanner);

                    advance(scanner);
                }
                advance(scanner); // consumes *
                advance(scanner); // consumes /
            }
            else
            {
                return;
            }
            break;
        }
        default:
            return;
        }
    }
}

static Token stringLiteral(Scanner *scanner)
{
    while (peek(scanner) != '"' && !isEOF(scanner))
    {
        if (peek(scanner) == '\n')
            incrementLocation(scanner);

        advance(scanner);
    }

    if (isEOF(scanner))
        return newScanError(scanner, "Unterminated string.");

    advance(scanner); // Consume the closing "
    return createToken(TOKEN_STRING, scanner);
}

static Token numberLiteral(Scanner *scanner)
{
    while (isDigit(peek(scanner)))
        advance(scanner);

    if (peek(scanner) == '.' && isDigit(peekNext(scanner)))
    {
        advance(scanner);
        while (isDigit(peek(scanner)))
            advance(scanner);
    }
    return createToken(TOKEN_NUMBER, scanner);
}

static Token identifier(Scanner *scanner)
{
    while (isAlphaNumeric(peek(scanner)))
        advance(scanner);

    TokenType type = identifierType(scanner);
    return createToken(type, scanner);
}

static bool checkKeyword(Scanner *scanner, int length, const char *word)
{
    int keywordLength = scanner->current - scanner->start;
    return keywordLength == length &&
           memcmp(scanner->start, word, length) == 0;
}

static TokenType identifierType(Scanner *scanner)
{
    switch (scanner->start[0])
    {
    case 'a':
        if (checkKeyword(scanner, 3, "and"))
            return TOKEN_AND;
        break;
    case 'c':
        if (checkKeyword(scanner, 5, "class"))
            return TOKEN_CLASS;
        break;
    case 'i':
        if (checkKeyword(scanner, 2, "if"))
            return TOKEN_IF;
        break;
    case 'e':
        if (checkKeyword(scanner, 4, "else"))
            return TOKEN_ELSE;
        break;
    case 'n':
        if (checkKeyword(scanner, 3, "nil"))
            return TOKEN_NIL;
        break;
    case 'o':
        if (checkKeyword(scanner, 2, "or"))
            return TOKEN_OR;
        break;
    case 'p':
        if (checkKeyword(scanner, 5, "print"))
            return TOKEN_PRINT;
        break;
    case 'r':
        if (checkKeyword(scanner, 6, "return"))
            return TOKEN_RETURN;
        break;
    case 's':
        if (checkKeyword(scanner, 5, "super"))
            return TOKEN_SUPER;
        break;
    case 'v':
        if (checkKeyword(scanner, 3, "var"))
            return TOKEN_VAR;
        break;
    case 'w':
        if (checkKeyword(scanner, 5, "while"))
            return TOKEN_WHILE;
        if (checkKeyword(scanner, 4, "when"))
            return TOKEN_WHEN;
        break;
    case 'f':
        if (checkKeyword(scanner, 5, "false"))
            return TOKEN_FALSE;
        if (checkKeyword(scanner, 3, "fun"))
            return TOKEN_FUN;
        if (checkKeyword(scanner, 3, "for"))
            return TOKEN_FOR;
        break;
    case 't':
        if (checkKeyword(scanner, 4, "this"))
            return TOKEN_THIS;
        if (checkKeyword(scanner, 4, "true"))
            return TOKEN_TRUE;
        break;
    }
    return TOKEN_IDENTIFIER;
}

static Token newLine(Scanner *scanner)
{
    Token tk;
    if (scanner->lastToken != TOKEN_NL)
    {
        tk = createToken(TOKEN_NL, scanner);
        incrementLocation(scanner);
    }
    else
    {
        incrementLocation(scanner);
        tk = scanToken(scanner);
    }
    return tk;
}

static void incrementLocation(Scanner *scanner)
{
    scanner->location.line++;
    scanner->location.column = 1;
}