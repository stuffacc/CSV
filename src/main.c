#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "column.h"
#include "file.h"
#include "row.h"
#include "test.h"

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--test") == 0) {
            int res = runTests();
            if (res == 0) {
                printf("All tests passed\n");
            }
            return res;
        }
    }

    char readBuffer[4096];

    FILE* csv = fopen("../data/input.csv", "r");

    if (csv == NULL) {
        printf("Не удалось открыть файл\n");
        return -1;
    }

    char* header = fileReadLine(csv, readBuffer);

    if (header == NULL) {
        printf("Файл пустой\n");
        return -2;
    }

    int columnCount = getColumnCount(header);
    int* columnsMaxSize = malloc(sizeof(int) * columnCount);

    updateMaxSizeColums(header, columnsMaxSize);

    char* nextLine = fileReadLine(csv, readBuffer);
    while (nextLine != NULL) {
        updateMaxSizeColums(nextLine, columnsMaxSize);

        nextLine = fileReadLine(csv, readBuffer);
    }

    // move pointer to start
    fseek(csv, 0, SEEK_SET);

    header = fileReadLine(csv, readBuffer);

    FILE* txt = fopen("../data/output.txt", "w");

    char writeRowBuffer[4096];
    char writeRowBreakBuffer[4096];

    Row* row = initRow(columnsMaxSize, columnCount, HEADER);

    fillCellsInRow(row, header);

    fillWriteRowBreakBuffer(writeRowBreakBuffer, row->type, columnsMaxSize, columnCount);

    fillWriteRowBuffer(writeRowBuffer, row, columnsMaxSize, columnCount);

    fprintf(txt, "%s", writeRowBreakBuffer);
    fprintf(txt, "%s", writeRowBuffer);
    fprintf(txt, "%s", writeRowBreakBuffer);

    nextLine = fileReadLine(csv, readBuffer);

    setRowType(row, DATA);
    fillWriteRowBreakBuffer(writeRowBreakBuffer, row->type, columnsMaxSize, columnCount);

    while (nextLine != NULL) {
        fillCellsInRow(row, nextLine);
        fillWriteRowBuffer(writeRowBuffer, row, columnsMaxSize, columnCount);

        fprintf(txt, "%s", writeRowBuffer);
        fprintf(txt, "%s", writeRowBreakBuffer);

        nextLine = fileReadLine(csv, readBuffer);
    }

    destroyRow(row, columnCount);
    free(columnsMaxSize);

    fclose(csv);
    fclose(txt);

    return 0;
}