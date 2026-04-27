#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "column.h"
#include "file.h"
#include "row.h"
#include "test.h"

#define CANNOT_OPEN_FILE -1
#define FILE_IS_EMPTY -2
#define ALLOCATE_ERROR -3

int getColumnCountFromFile(char* readBuffer, FILE* csv)
{
    char* header = fileReadLine(csv, readBuffer);

    if (header == NULL) {
        return FILE_IS_EMPTY;
    }

    return getColumnCount(header);
}

void countMaxSizeColumnsFromFile(FILE* csv, int* columnsMaxSize, char* readBuffer)
{
    updateMaxSizeColums(readBuffer, columnsMaxSize);

    char* nextLine = fileReadLine(csv, readBuffer);
    while (nextLine != NULL) {
        updateMaxSizeColums(nextLine, columnsMaxSize);

        nextLine = fileReadLine(csv, readBuffer);
    }
}

void prettyWriteInFile(FILE* csv, FILE* txt, char* readBuffer, Row* row, int* columnsMaxSize, int columnCount, char* writeRowBuffer, char* writeRowBreakBuffer)
{
    char* header = fileReadLine(csv, readBuffer);
    fillCellsInRow(row, header);

    fillWriteRowBreakBuffer(writeRowBreakBuffer, row->type, columnsMaxSize, columnCount);

    fillWriteRowBuffer(writeRowBuffer, row, columnsMaxSize, columnCount);

    fprintf(txt, "%s", writeRowBreakBuffer);
    fprintf(txt, "%s", writeRowBuffer);
    fprintf(txt, "%s", writeRowBreakBuffer);

    char* nextLine = fileReadLine(csv, readBuffer);

    setRowType(row, DATA);
    fillWriteRowBreakBuffer(writeRowBreakBuffer, row->type, columnsMaxSize, columnCount);

    while (nextLine != NULL) {
        fillCellsInRow(row, nextLine);
        fillWriteRowBuffer(writeRowBuffer, row, columnsMaxSize, columnCount);

        fprintf(txt, "%s", writeRowBuffer);
        fprintf(txt, "%s", writeRowBreakBuffer);

        nextLine = fileReadLine(csv, readBuffer);
    }
}

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--test") == 0) {
            int res = runTests();
            if (res == 0) {
                printf("Все тесты прошли\n");
            }
            return res;
        }
    }

    FILE* csv = fopen("../data/input.csv", "r");

    if (csv == NULL) {
        printf("Не удалось открыть файл на чтение\n");
        return CANNOT_OPEN_FILE;
    }

    char readBuffer[4096];
    int columnCount = getColumnCountFromFile(readBuffer, csv);

    if (columnCount == FILE_IS_EMPTY) {
        printf("Файл пустой\n");

        fclose(csv);

        return columnCount;
    }

    int* columnsMaxSize = malloc(sizeof(int) * columnCount);
    if (columnsMaxSize == NULL) {
        printf("Ошибка выделения памяти\n");

        fclose(csv);

        return ALLOCATE_ERROR;
    }

    countMaxSizeColumnsFromFile(csv, columnsMaxSize, readBuffer);

    // move pointer to start
    fseek(csv, 0, SEEK_SET);

    FILE* txt = fopen("../data/output.txt", "w");
    if (txt == NULL) {
        printf("Не удалось открыть файл на запись\n");

        free(columnsMaxSize);
        fclose(csv);
        return CANNOT_OPEN_FILE;
    }

    Row* row = initRow(columnsMaxSize, columnCount, HEADER);
    if (row == NULL) {
        printf("Ошибка выделения памяти\n");

        free(columnsMaxSize);
        fclose(csv);
        fclose(txt);
        return ALLOCATE_ERROR;
    }

    char writeRowBuffer[4096];
    char writeRowBreakBuffer[4096];

    prettyWriteInFile(csv, txt, readBuffer, row, columnsMaxSize, columnCount, writeRowBuffer, writeRowBreakBuffer);

    destroyRow(row, columnCount);
    free(columnsMaxSize);

    fclose(csv);
    fclose(txt);

    return 0;
}