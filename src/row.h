#pragma once
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


typedef enum CellType {
    NUMBER, STRING
} CellType;

typedef struct Cell {
    CellType type;
    char* string;
} Cell;


typedef enum RowType {
    HEADER, DATA
} RowType;


typedef struct Row {
    RowType type;
    Cell* cells;
} Row;


Row* initRow(int columnsMaxSize[], int columnCount, RowType rowType);
Row* setRowType(Row* row, RowType rowType);
void destroyRow(Row* row, int columnCount);



Cell* initCells(int columnsMaxSize[], int columnCount);
Cell* setCellType(Cell* cell, CellType cellType);
void destroyCells(Cell* cells, int columnCount);



void fillCellsInRow(Row* row, char* rowString);
void fillWriteRowBreakBuffer(char* writeRowBreakBuffer, RowType rowType, int columnsMaxSize[], int columnCount);
void fillWriteRowBuffer(char* writeRowBuffer, Row* row, int columnsMaxSize[], int columnCount);