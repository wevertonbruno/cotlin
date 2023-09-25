#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "debug.h"
#include "compiler.h"

static void printStack(VM *vm)
{
    printf("Stack -> ");
    for (Value *slot = vm->stack; slot < vm->stackHead; slot++)
    {
        printf("[ ");
        printValue(*slot);
        printf(" ]");
    }
    printf("\n");
}

static InterpreterResult execute(VM *vm)
{ // Decoding

#define READ_BYTE() (*vm->ip++)

#define READ_CONSTANT() (vm->chunk->constants.values[READ_BYTE()])

#define READ_CONSTANT_LONG() ((uint8_t)vm->chunk->constants.values[READ_BYTE()] | ((uint8_t)vm->chunk->constants.values[READ_BYTE()] << 8) | ((uint8_t)vm->chunk->constants.values[READ_BYTE()] << 16))

#define BINARY_OPERATION(operation)     \
    do                                  \
    {                                   \
        Value b = vmStackPop(vm);       \
        Value a = vmStackPop(vm);       \
        vmStackPush(vm, a operation b); \
    } while (false)

    while (true)
    {
#ifdef DEBUG_TRACE_EXECUTION
        printStack(vm);
        disassembleInstruction(vm->chunk, (int)(vm->ip - (vm->chunk)->code));
#endif

        uint8_t instruction = READ_BYTE();
        switch (instruction)
        {
        case OP_RETURN:
            printValue(vmStackPop(vm));
            printf("\n");
            return INTERPRET_OK;
        case OP_CONSTANT:
        {
            Value constant = READ_CONSTANT();
            vmStackPush(vm, constant);
            break;
        }
        case OP_CONSTANT_LONG: // Rever uso de CONSTANT_LONG
        {
            Value constant = READ_CONSTANT_LONG();
            vmStackPush(vm, constant);
            break;
        }
        case OP_NEGATE:
        {
            Value value = vmStackPop(vm);
            vmStackPush(vm, -value);
            break;
        }
        case OP_ADD:
            BINARY_OPERATION(+);
            break;
        case OP_SUBTRACT:
            BINARY_OPERATION(-);
            break;
        case OP_DIVIDE:
            BINARY_OPERATION(/);
            break;
        case OP_MULTIPLY:
            BINARY_OPERATION(*);
            break;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
#undef BINARY_OPERATION
}

InterpreterResult vmInterpret(VM *vm, const char *source)
{
    Chunk chunk = newChunk();
    if (!compile(source, &chunk))
    {
        destroyChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm->chunk = &chunk;
    vm->ip = vm->chunk->code;

    InterpreterResult result = execute(vm);
    destroyChunk(&chunk);

    return result;
}

VM vmNew()
{
    VM vm;
    vm.chunk = NULL;
    vm.ip = NULL;
    vm.stackHead = vm.stack; // Reset stack: stackHead points to stack first element
    return vm;
}

void vmStackPush(VM *vm, Value value)
{
    *vm->stackHead = value;
    vm->stackHead++;
}

Value vmStackPop(VM *vm)
{
    vm->stackHead--;
    return *vm->stackHead;
}

void vmStop(VM *vm)
{
    vm->ip = NULL;
}