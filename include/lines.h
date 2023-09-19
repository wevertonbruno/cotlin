#ifndef cotlin_lines_h
#define cotlin_lines_h

#include "common.h"

typedef struct
{
    unsigned line;
    unsigned until;
} Line;


typedef struct {
    int capacity;
    int count;
    Line *values;
} Lines;

Lines newLines();
void destroyLines(Lines* array);
void addLines(Lines* array, unsigned);
void resetLines(Lines* array);
Line* getLastLine(Lines* array);

#endif
