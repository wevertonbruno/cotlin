#include <stdlib.h>
#include <stdio.h>

#include "file.h"
#include "common.h"

char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", path);
        exit(OPEN_FILE_ERROR); // Define exit codes
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(fileSize + 1);
    if(!buffer) {
        fprintf(stderr, "Not enough memory to read file %s\n", path);
        exit(OPEN_FILE_ERROR);    
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}   