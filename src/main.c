#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "tests.h"

int main(int argc, const char* argv[]) {
    runChunkTest();
    runVMTest();
    return 0;
}
