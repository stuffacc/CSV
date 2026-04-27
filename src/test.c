#include "file.h"
#include "row.h"
#include "test.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int getColumnCountFromFile(char* readBuffer, FILE* csv);
void countMaxSizeColumnsFromFile(FILE* csv, int* columnsMaxSize, char* readBuffer);
void prettyWriteInFile(FILE* csv, FILE* txt, char* readBuffer, Row* row, int* columnsMaxSize, int columnCount, char* writeRowBuffer, char* writeRowBreakBuffer);

int runTests()
{
    FILE* in = fopen("../data/test/test1.csv", "r");
    if (in == NULL) {
        printf("Файл с тестовыми даннами (test1.csv) не найден\n");
        return FAILED;
    }

    // TEST 1
    int exceptedCount = 2;
    if (testGetColumnCountFromFile(in, exceptedCount) == FAILED) {
        fclose(in);
        return FAILED;
    }

    // TEST 2
    fseek(in, 0, SEEK_SET);
    int exceptedSizes[2] = { 17, 12 };

    if (testCountMaxSizeColumnsFromFile(in, exceptedSizes, exceptedCount) == FAILED) {
        fclose(in);
        return FAILED;
    }

    FILE* out = fopen("../data/test/output.txt", "w");
    if (out == NULL) {
        printf("Не удалось открыть файл на запись\n");
        fclose(in);
        return FAILED;
    }

    // TEST 3
    fseek(in, 0, SEEK_SET);
    testPrettyWriteInFile(exceptedSizes, exceptedCount, in, out);

    fclose(in);
    fclose(out);

    FILE* actual = fopen("../data/test/output.txt", "r");
    FILE* excepted = fopen("../data/test/testout1.txt", "r");

    if (compareFiles(actual, excepted) == FAILED) {
        printf("Тест 3 (testPrettyWriteInFile) не прошёл\n");
        return FAILED;
    }

    else {
        printf("Тест 3 (testPrettyWriteInFile) прошёл\n");
    }

    return SUCCESS;
}

int testGetColumnCountFromFile(FILE* in, int excepted)
{
    char readBuffer[1024];
    int columnCount = getColumnCountFromFile(readBuffer, in);
    if (columnCount != excepted) {
        printf("Тест 1 (getColumnCountFromFile) не прошёл\n");
        fclose(in);

        return FAILED;
    }

    printf("Тест 1 (getColumnCountFromFile) прошёл\n");
    return columnCount;
}

int testCountMaxSizeColumnsFromFile(FILE* in, int exceptedSizes[], int columnCount)
{
    char readBuffer[1024];

    int* actualSizes = malloc(sizeof(int) * columnCount);

    countMaxSizeColumnsFromFile(in, actualSizes, readBuffer);

    for (int i = 0; i < columnCount; i++) {
        if (actualSizes[i] != exceptedSizes[i]) {
            printf("Тест 2 (CountMaxSizeColumnsFromFile) не прошёл\n");
            free(actualSizes);
            return FAILED;
        }
    }

    printf("Тест 2 (CountMaxSizeColumnsFromFile) прошёл\n");
    free(actualSizes);
    return SUCCESS;
}

int testPrettyWriteInFile(int* columnsMaxSize, int columnCount, FILE* in, FILE* out)
{
    Row* row = initRow(columnsMaxSize, columnCount, HEADER);
    if (row == NULL) {
        printf("Ошибка выделения памяти\n");
        return FAILED;
    }

    char writeRowBuffer[1024];
    char writeRowBreakBuffer[1024];
    char readBuffer[1024];

    prettyWriteInFile(in, out, readBuffer, row, columnsMaxSize, columnCount, writeRowBuffer, writeRowBreakBuffer);

    destroyRow(row, columnCount);
}

int compareFiles(FILE* actual, FILE* excepted)
{
    char readBufferActual[1024];
    char readBufferExcepted[1024];

    char* outBuffLine = fileReadLine(actual, readBufferActual);
    char* expectedOutBuffLine = fileReadLine(excepted, readBufferExcepted);

    while (true) {
        if (outBuffLine == NULL || expectedOutBuffLine == NULL) {
            fclose(actual);
            fclose(excepted);

            if (outBuffLine != expectedOutBuffLine) {
                return FAILED;
            }

            return SUCCESS;
        }

        if (strcmp(outBuffLine, expectedOutBuffLine) != 0) {
            fclose(actual);
            fclose(excepted);

            return FAILED;
        }

        outBuffLine = fileReadLine(actual, readBufferActual);
        expectedOutBuffLine = fileReadLine(excepted, expectedOutBuffLine);
    }
}