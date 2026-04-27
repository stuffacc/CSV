#include "row.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Row* initRow(int columnsMaxSize[], int columnCount, RowType rowType)
{
    Row* row = malloc(sizeof(Row));

    if (row == NULL) {
        return NULL;
    }

    row->type = rowType;

    Cell* cells = initCells(columnsMaxSize, columnCount);
    if (cells == NULL) {
        free(row);
        return NULL;
    }

    row->cells = cells;

    return row;
}

Row* setRowType(Row* row, RowType rowType)
{
    row->type = rowType;

    return row;
}

void destroyRow(Row* row, int columnCount)
{
    Cell* cells = row->cells;
    destroyCells(cells, columnCount);

    free(cells);
    free(row);
}

Cell* initCells(const int columnsMaxSize[], int columnCount)
{
    Cell* cells = malloc(sizeof(Cell) * columnCount);
    if (cells == NULL) {
        return NULL;
    }

    for (int i = 0; i < columnCount; i++) {
        Cell* currentCell = cells + i;
        int currentMaxSize = columnsMaxSize[i];

        char* rowString = malloc(sizeof(char) * (currentMaxSize + 1));
        currentCell->string = rowString;
    }

    return cells;
}

Cell* setCellType(Cell* cell, CellType cellType)
{
    cell->type = cellType;

    return cell;
}

void destroyCells(Cell* cells, int columnCount)
{
    for (int i = 0; i < columnCount; i++) {
        Cell* currentCell = cells + i;

        free(currentCell->string);
    }
}

void fillCellsInRow(Row* row, char* rowString)
{
    Cell* cells = row->cells;

    char currentChar = *rowString;
    char* startRowStr = rowString;

    int columnIndex = 0;

    int cellIsString = 0;
    int countPoint = 0;
    while (currentChar != '\0') {
        if (currentChar == ',') {
            *rowString = '\0';

            Cell* currentCell = cells + columnIndex;

            if (cellIsString || (countPoint >= 2)) {
                setCellType(currentCell, STRING);
            }

            else {
                setCellType(currentCell, NUMBER);
            }

            currentCell->string = strcpy(currentCell->string, startRowStr);

            *rowString = ',';

            startRowStr = rowString + 1;

            cellIsString = 0;
            countPoint = 0;

            columnIndex++;
        }

        else if (currentChar == '.') {
            countPoint++;
        }

        else if (!isdigit(currentChar)) {
            cellIsString = 1;
        }

        rowString++;
        currentChar = *rowString;
    }

    Cell* currentCell = cells + columnIndex;

    if (cellIsString || (countPoint >= 2)) {
        setCellType(currentCell, STRING);
    }

    else {
        setCellType(currentCell, NUMBER);
    }

    currentCell->string = strcpy(currentCell->string, startRowStr);
}

void fillWriteRowBuffer(char* writeRowBuffer, Row* row, const int columnsMaxSize[], int columnCount)
{
    Cell* cells = row->cells;

    for (int i = 0; i < columnCount; i++) {
        int currentMaxSize = columnsMaxSize[i];
        Cell* currentCell = cells + i;

        *writeRowBuffer = '|';
        writeRowBuffer++;
        *writeRowBuffer = ' ';
        writeRowBuffer++;

        int shift = 0;

        if ((row->type == DATA) && (currentCell->type == NUMBER)) {
            shift = sprintf(writeRowBuffer, "%*s", currentMaxSize, currentCell->string);
        }

        else {
            shift = sprintf(writeRowBuffer, "%-*s", currentMaxSize, currentCell->string);
        }

        writeRowBuffer += shift;
        *writeRowBuffer = ' ';
        writeRowBuffer++;
    }

    *writeRowBuffer = '|';
    writeRowBuffer++;
    *writeRowBuffer = '\n';
    writeRowBuffer++;
    *writeRowBuffer = '\0';
}

void fillWriteRowBreakBuffer(char* writeRowBreakBuffer, RowType rowType, const int columnsMaxSize[], int columnCount)
{
    *writeRowBreakBuffer = '+';
    writeRowBreakBuffer++;

    char fillChar = (rowType == HEADER) ? '=' : '-';

    for (int i = 0; i < columnCount; i++) {
        int charCounter = 0;
        int currentSize = columnsMaxSize[i] + 2;
        while (charCounter < currentSize) {
            *writeRowBreakBuffer = fillChar;
            writeRowBreakBuffer++;

            charCounter++;
        }
        *writeRowBreakBuffer = '+';
        writeRowBreakBuffer++;
    }

    *writeRowBreakBuffer = '\n';
    writeRowBreakBuffer++;
    *writeRowBreakBuffer = '\0';
}