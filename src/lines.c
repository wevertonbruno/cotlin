#include "lines.h"
#include "memory.h"

#include <stdio.h>

Lines newLines() {
    Lines lines;
    resetLines(&lines);
    return lines;
}

static Line newLine(unsigned lineNumber, unsigned offset) {
    Line lineValue;
    lineValue.line = lineNumber;
    lineValue.offset = offset;
    return lineValue;
}

void resetLines(Lines* array) {
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

void addLines(Lines* array, unsigned line) {
    if (array->count > 0 && (array->values[array->count - 1].line == line)) {
        array->values[array->count - 1].offset++;
        return;
    }

    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Line, array->values, oldCapacity, array->capacity);
    }

    array->values[array->count] = newLine(line, getLastLine(array)->offset + 1);
    array->count++;
}

void destroyLines(Lines* array) {
    FREE_ARRAY(Line, array->values, array->capacity);
    resetLines(array);
}

Line* getLastLine(Lines* array) {
    return &(array->values[array->count - 1]);
}