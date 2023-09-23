#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "chunk.h"
#include "vm.h"

void runVMTest(void) {
    printf("==== VM Test ===\n");
    VM vm = vmNew();
    Chunk chunk = newChunk();

    // Testing 1 * 2 + 3
    writeConstant(&chunk, (Value) 1, 0);
    writeConstant(&chunk, (Value) 2, 0);
    writeChunk(&chunk, OP_MULTIPLY, 0);
    writeConstant(&chunk, (Value) 3, 0);
    writeChunk(&chunk, OP_ADD, 0);
    writeChunk(&chunk, OP_RETURN, 0);
    assert(vmInterpret(&vm, &chunk) == INTERPRET_OK);
    printf("==== Done! ===\n");
}