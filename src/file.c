#include "file.h"

char* fileReadLine(FILE* file, char* buffer)
{
    int res = fscanf(file, "%[^\n]\n", buffer);
    if (res == 1) {
        return buffer;
    }

    return NULL;
}