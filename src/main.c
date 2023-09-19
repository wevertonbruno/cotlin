#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
    Chunk chunk = newChunk();
    int constant = addConstant(&chunk, 1.2);
    addChunk(&chunk, OP_CONSTANT, 0);
    addChunk(&chunk, constant, 1);
    addChunk(&chunk, OP_RETURN, 1);
    addChunk(&chunk, OP_RETURN, 1);
    addChunk(&chunk, OP_RETURN, 2);
    addChunk(&chunk, OP_RETURN, 2);
    addChunk(&chunk, OP_RETURN, 3);
    addChunk(&chunk, OP_RETURN, 4);
    addChunk(&chunk, OP_RETURN, 4);
    addChunk(&chunk, OP_RETURN, 4);
    addChunk(&chunk, OP_RETURN, 5);
    addChunk(&chunk, OP_RETURN, 5);
    disassembleChunk(&chunk, "Test Chunk");
    destroyChunk(&chunk);
    return 0;
}
