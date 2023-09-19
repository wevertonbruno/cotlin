#include <stdlib.h>
#include <stdio.h>

#include "value.h"
#include "memory.h"

ValueArray newValueArray() {
    ValueArray array;
    resetValueArray(&array);
    return array;
}

void resetValueArray(ValueArray* array) {
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

void addValueArray(ValueArray* array, Value byte) {
    if (array->capacity < array->count + 1) 
    {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
    }
    array->values[array->count] = byte;
    array->count++;
}

void destroyValueArray(ValueArray* array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    resetValueArray(array);
}

void printValue(Value value) {
    printf("%g", value);
}