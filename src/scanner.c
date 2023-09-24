#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

#define EOF_CHAR '\0'

static bool isEOF(const char current);
static Token newScanError(Scanner* scanner, const char* message);
static Token createToken(TokenType type, Scanner* scanner);
static char advance(Scanner* scanner);
static bool match(Scanner* scanner, char expected);

Scanner scannerNew(const char* source) {
    Scanner scanner;
    scanner.start = source;
    scanner.current = source;
    scanner.location = (Location) {1, 1};
    return scanner;
}

Token scanToken(Scanner* scanner) {
    scanner->start = scanner->current;
    if (isEOF(*(scanner->current))) return createToken(TOKEN_EOF, scanner);

    char current = advance(scanner);

    switch(current) {
        case '(': return createToken(TOKEN_LEFT_PAREN, scanner);
        case ')': return createToken(TOKEN_RIGHT_PAREN, scanner);
        case '{': return createToken(TOKEN_LEFT_BRACE, scanner);
        case '}': return createToken(TOKEN_RIGHT_BRACE, scanner);
        case ',': return createToken(TOKEN_COMMA, scanner);
        case '.': return createToken(TOKEN_DOT, scanner);
        case '-': return createToken(TOKEN_MINUS, scanner);
        case '+': return createToken(TOKEN_PLUS, scanner);
        case ';': return createToken(TOKEN_SEMICOLON, scanner);
        case '/': return createToken(TOKEN_SLASH, scanner);
        case '*': return createToken(TOKEN_STAR, scanner);
        default: return newScanError(scanner, "Unexpected character.");
    }

    // page 287

    //return newScanError(scanner, "Unexpected character.");
}

static bool isEOF(const char current) {
    return current == EOF_CHAR;
}

static Token newScanError(Scanner* scanner, const char* message){
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int) strlen(message);
    token.location = (*scanner).location;
    return token;
}

static Token createToken(TokenType type, Scanner* scanner) {
    Token token;
    token.type = type;
    token.start = (*scanner).start;
    token.length = (int) ((*scanner).current - (*scanner).start);
    token.location = (*scanner).location;
    return token;
}

static char advance(Scanner* scanner) {
    scanner->current++;
    return scanner->current[-1];
}

static bool match(Scanner* scanner, char expected) {
    if (isEOF(*scanner->current)) return false;
    if (*scanner->current != expected) return false;
    advance(scanner);
    return true;
}