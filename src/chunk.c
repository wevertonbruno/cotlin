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

void writeChunk(Chunk* chunk, uint8_t byte, unsigned line) {
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

void writeConstant(Chunk* chunk, Value value, unsigned line) {
    int index = addConstant(chunk, value);
    if (index < 256) {
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, (uint8_t)index, line);
    }else{
        writeChunk(chunk, OP_CONSTANT_LONG, line);
        writeChunk(chunk, (uint8_t) (index & 0xff), line);
        writeChunk(chunk, (uint8_t)((index >> 8) & 0xff), line);
        writeChunk(chunk, (uint8_t)((index >> 16) & 0xff), line);
    }
}

static void printLines(Lines* lines) {
    printf("Printing lines...\n");
    printf("Line count: %d\n", lines->count);
    for (int i = 0; i < lines->count; i++) {
        printf("number: %4d \n", lines->values[i].line);
        printf("offset: %4d \n", lines->values[i].offset);
    }
    printf("\n");
}


// Improve by dividing to conquer: mid = start - end / 2;
unsigned getLine(Chunk* chunk, unsigned offset) {
    for (int i = 0; i < chunk->lines.count; i++)
    {
        if (offset < chunk->lines.values[i].offset) {
            return chunk->lines.values[i].line;
        }
    }
    return 0;
}