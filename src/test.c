#include "test.h"

int runTests()
{
    int testNum = 1;

    char fileNameIn[128];
    char fileNameOutExpected[128];

    sprintf(fileNameIn, "%s%d%s", "../data/test/test", testNum, ".csv");

    FILE* in = fopen(fileNameIn, "r");

    while (in != NULL) {
        printf("Run test %d\n", testNum);
        sprintf(fileNameOutExpected, "%s%d%s", "../data/test/testout", testNum, ".txt");
        FILE* expectedOut = fopen(fileNameOutExpected, "r");

        if (expectedOut == NULL) {
            printf("Missed: data/test/testout%d.txt\n", testNum);
            fclose(in);
            return -1;
        }

        int testRes = testAndClose(in, expectedOut);
        if (testRes != 0) {
            printf("Error in test %d:\nSee 'data/test/test%d.csv', 'data/test/testout%d.txt' and 'data/test/output.txt'\n", testNum, testNum, testNum);
            return -1;
        }

        testNum++;

        sprintf(fileNameIn, "%s%d%s", "../data/test/test", testNum, ".csv");
        in = fopen(fileNameIn, "r");
    }

    if (in != NULL) {
        fclose(in);
    }

    return 0;
}

int testAndClose(FILE* in, FILE* expectedOut)
{
    char readBuffer[1024];

    char* header = fileReadLine(in, readBuffer);

    if (header == NULL) {
        printf("Файл с тестом пустой\n");
        fclose(in);
        return -1;
    }

    int columnCount = getColumnCount(header);
    int* columnsMaxSize = malloc(sizeof(int) * columnCount);

    updateMaxSizeColums(header, columnsMaxSize);

    char* nextLine = fileReadLine(in, readBuffer);
    while (nextLine != NULL) {
        updateMaxSizeColums(nextLine, columnsMaxSize);

        nextLine = fileReadLine(in, readBuffer);
    }

    // move pointer to start
    fseek(in, 0, SEEK_SET);

    header = fileReadLine(in, readBuffer);

    FILE* txt = fopen("../data/test/output.txt", "w");

    char writeRowBuffer[1024];
    char writeRowBreakBuffer[1024];

    Row* row = initRow(columnsMaxSize, columnCount, HEADER);

    fillCellsInRow(row, header);

    fillWriteRowBreakBuffer(writeRowBreakBuffer, row->type, columnsMaxSize, columnCount);

    fillWriteRowBuffer(writeRowBuffer, row, columnsMaxSize, columnCount);

    fprintf(txt, "%s", writeRowBreakBuffer);
    fprintf(txt, "%s", writeRowBuffer);
    fprintf(txt, "%s", writeRowBreakBuffer);

    nextLine = fileReadLine(in, readBuffer);

    setRowType(row, DATA);
    fillWriteRowBreakBuffer(writeRowBreakBuffer, row->type, columnsMaxSize, columnCount);

    while (nextLine != NULL) {
        fillCellsInRow(row, nextLine);
        fillWriteRowBuffer(writeRowBuffer, row, columnsMaxSize, columnCount);

        fprintf(txt, "%s", writeRowBuffer);
        fprintf(txt, "%s", writeRowBreakBuffer);

        nextLine = fileReadLine(in, readBuffer);
    }

    destroyRow(row, columnCount);
    free(columnsMaxSize);

    fclose(in);
    fclose(txt);

    return compareFilesByLinesAndClose(expectedOut);
}

int compareFilesByLinesAndClose(FILE* expectedOut)
{
    FILE* out = fopen("../data/test/output.txt", "r");

    char outBuff[4096];
    char expectedOutBuff[4096];

    char* outBuffLine = fileReadLine(out, outBuff);
    char* expectedOutBuffLine = fileReadLine(expectedOut, expectedOutBuff);

    while (1) {
        if (outBuffLine == NULL || expectedOutBuffLine == NULL) {
            // TODO:
            fclose(out);
            fclose(expectedOut);

            if (outBuffLine != expectedOutBuffLine) {

                return -1;
            }

            return 0;
        }

        if (strcmp(outBuffLine, expectedOutBuffLine) != 0) {

            fclose(out);
            fclose(expectedOut);
            return -1;
        }

        outBuffLine = fileReadLine(out, outBuff);
        expectedOutBuffLine = fileReadLine(expectedOut, expectedOutBuff);
    }
}
