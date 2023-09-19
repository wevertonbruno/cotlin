#ifndef cotlin_value_h
#define cotlin_value_h

#include "common.h"

typedef double Value;

typedef struct {
    int capacity;
    int count;
    Value *values;
} ValueArray;

ValueArray newValueArray();
void destroyValueArray(ValueArray* array);
void addValueArray(ValueArray* array, Value value);
void printValue(Value value);
void resetValueArray(ValueArray* array);

#endif