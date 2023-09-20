#ifndef cotlin_vm_h
#define cotlin_vm_h

#include "chunk.h"

typedef struct {
    Chunk* chunk;
    uint8_t* ip; // Instruction pointer
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpreterResult;

VM vmNew();
void vmStop(VM*);
InterpreterResult vmInterpret(VM* vm, Chunk* chunk);

#endif
