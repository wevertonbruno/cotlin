#include <stdlib.h>
#include <stdio.h>

#include "chunk.h"
#include "memory.h"

Chunk newChunk() {
    Chunk chunk;
    resetChunk(&chunk);
    return chunk;
}

void resetChunk(Chunk* chunk) {
    chunk->capacity = 0;
    chunk->count = 0;
    chunk->code = NULL;
    chunk->lines = newLines();
    chunk->constants = newValueArray();
}

void addChunk(Chunk* chunk, uint8_t byte, unsigned line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    addLines(&chunk->lines, line);
    chunk->code[chunk->count] = byte;
    chunk->count++;
}

void destroyChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    destroyValueArray(&chunk->constants);
    destroyLines(&chunk->lines);
    resetChunk(chunk);
}

int addConstant(Chunk* chunk, Value value) {
    addValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

static void printLines(Lines* lines) {
    printf("Printing lines...\n");
    printf("Line count: %d\n", lines->count);
    for (int i = 0; i < lines->count; i++) {
        printf("number: %4d \n", lines->values[i].line);
        printf("until: %4d \n", lines->values[i].until);
    }
    printf("\n");
}

unsigned getLine(Chunk* chunk, unsigned offset) {
    for (unsigned i = 0; i < chunk->lines.count; i++)
    {
        if (offset < chunk->lines.values[i].until) {
            return chunk->lines.values[i].line;
        }

    }
    return 0;
}