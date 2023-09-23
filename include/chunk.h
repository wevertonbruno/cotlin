#ifndef cotlin_chunk_h
#define cotlin_chunk_h

#include "common.h"
#include "value.h"
#include "lines.h"

typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN
} OpCode;

typedef struct {
    int count;
    int capacity;
    unsigned currentLine;
    uint8_t* code; // Array of instructions
    Lines lines; // Array of line, to user know where the instruction is
    ValueArray constants; // Array of constants to represent a literal
} Chunk;

Chunk newChunk();
void resetChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, unsigned int line);
void writeConstant(Chunk* chunk, Value value, unsigned line);
void destroyChunk(Chunk* chunk);
int addConstant(Chunk* chunk, Value value);
unsigned getLine(Chunk*, unsigned offset);

#endif
