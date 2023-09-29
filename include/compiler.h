#ifndef cotlin_compiler_h
#define cotlin_compiler_h

#include "vm.h"

typedef struct
{
    Token current;
    Token previous;
    Scanner scanner;
    bool hadError;
    bool panicMode;
} Parser;

Parser parserNew();
bool compile(const char *source, Chunk *chunk);

#endif