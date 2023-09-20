#include <stdio.h>
#include "vm.h"
#include "debug.h"

/**
 * @param vm
 * Decoding every single instruction
*/
static InterpreterResult execute(VM* vm) {
    #define READ_BYTE() (*vm->ip++)
    #define READ_CONSTANT() (vm->chunk->constants.values[READ_BYTE()])
    #define READ_CONSTANT_LONG() ((uint8_t) vm->chunk->constants.values[READ_BYTE()] \
        | ((uint8_t) vm->chunk->constants.values[READ_BYTE()] << 8) \
        | ((uint8_t) vm->chunk->constants.values[READ_BYTE()] << 16))

    while (true){
        #ifdef DEBUG_TRACE_EXECUTION
        disassembleInstruction(vm->chunk, (int)(vm->ip - (vm->chunk)->code));
        #endif

        uint8_t instruction = READ_BYTE();
        switch (instruction)
        {
            case OP_RETURN:{
                return INTERPRET_OK;
            }
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                // printValue(constant);
                // printf("\n");
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant = READ_CONSTANT_LONG();
                // printValue(constant);
                // printf("\n");
                break;
            }
        }
    }
    #undef READ_BYTE
}

InterpreterResult vmInterpret(VM* vm, Chunk* chunk) {
    vm->chunk = chunk;
    vm->ip = chunk->code;
    return execute(vm);
}

VM vmNew() {
    VM vm;
    vm.chunk = NULL;
    vm.ip = NULL;
    return vm;
}