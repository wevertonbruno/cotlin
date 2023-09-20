#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "chunk.h"
#include "vm.h"

void runVMTest(void) {
    printf("==== VM Test ===\n");

    VM vm = vmNew();
    Chunk chunk = newChunk();

    for (int i = 0; i < 270; i++) {
        writeConstant(&chunk, (Value) i, i / 10);
    }
    writeChunk(&chunk, OP_RETURN, 0);

    vmInterpret(&vm, &chunk);
    destroyChunk(&chunk);
    printf("==== Done! ===\n");
}