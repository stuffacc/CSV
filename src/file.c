#include "file.h"

char* fileReadLine(FILE* file, char* buffer)
{
    int result = fscanf(file, "%[^\n]\n", buffer);
    if (result == 1) {
        return buffer;
    }

    return NULL;
}