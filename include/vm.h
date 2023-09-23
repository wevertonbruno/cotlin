#ifndef cotlin_vm_h
#define cotlin_vm_h

#include "chunk.h"

#define VM_STACK_SIZE 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip; // Instruction pointer
    Value stack[VM_STACK_SIZE];
    Value* stackHead;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpreterResult;

VM vmNew();
void vmStop(VM*);
InterpreterResult vmInterpret(VM* vm, Chunk* chunk);
void vmStackPush(VM* vm, Value value);
Value vmStackPop(VM* vm);

#endif
