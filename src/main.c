#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "file.h"

static void repl(VM* vm) {
    char line[1024]; // Allow multiple lines

    while (true) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        vmInterpret(vm, line);
        break;
    }
}

static void runFile(VM* vm, const char* path) {
    char* source = readFile(path);
    InterpreterResult result = vmInterpret(vm, source);
    free(source);
    if(result == INTERPRET_COMPILE_ERROR) exit(65);
    if(result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv[]) {
    // Initializes VM
    VM vm = vmNew();

    if (argc == 1) {
        repl(&vm);
    } else if (argc == 2) {
        runFile(&vm, argv[1]);
    } else {
        fprintf(stderr, "Usage: cotlin [file]\n");
        exit(64);
    }

    // Free vm
    vmStop(&vm);
    return 0;
}
