#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "common.h"
#include "chunk.h"

static void chunkIsInitialized(Chunk* chunk) {
    assert(chunk != NULL);
    assert(chunk->count == 0);
    assert(chunk->capacity == 0);
    assert(chunk->code == NULL);
    assert(chunk->lines.count == 0);
    assert(chunk->lines.values == NULL);
    assert(chunk->lines.capacity == 0);
}

void runChunkTest(void) {
    printf("==== Chunk Test ===\n");
    Chunk chunk = newChunk();
    chunkIsInitialized(&chunk);

    // Add code
    writeChunk(&chunk, OP_RETURN, 0);
    assert(chunk.count == 1);
    assert(chunk.capacity == 8);
    assert(chunk.code[0] == OP_RETURN);
    assert(chunk.lines.count == 1);

    Line currentLine = chunk.lines.values[0];
    assert(currentLine.line == 0);
    assert(currentLine.offset == 1);

    writeChunk(&chunk, OP_RETURN, 0);
    writeChunk(&chunk, OP_RETURN, 0);
    writeChunk(&chunk, OP_RETURN, 1);
    writeChunk(&chunk, OP_RETURN, 2);


    currentLine = chunk.lines.values[chunk.lines.count - 1];
    assert(currentLine.line == 2);
    assert(currentLine.offset == 5);

    destroyChunk(&chunk);
    chunkIsInitialized(&chunk);
    printf("==== Done! ===\n");
}