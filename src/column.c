#include "column.h"

// получить максимум
static int max(int value1, int value2)
{
    return value1 > value2 ? value1 : value2;
}

int getColumnCount(const char* line)
{
    int columnCount = 1;

    char currentChar = *line;
    while (currentChar != '\0') {
        if (currentChar == ',') {
            columnCount++;
        }
        line++;
        currentChar = *line;
    }

    return columnCount;
}

void updateMaxSizeColums(const char* line, int columnsMaxSize[])
{
    char currentChar = *line;

    int columnIndex = 0;
    int columnSize = 0;
    while (currentChar != '\0') {
        if (currentChar != ',') {
            columnSize++;
        }

        else {
            columnsMaxSize[columnIndex] = max(columnsMaxSize[columnIndex], columnSize);

            columnIndex++;
            columnSize = 0;
        }

        line++;
        currentChar = *line;
    }

    columnsMaxSize[columnIndex] = max(columnsMaxSize[columnIndex], columnSize);
}