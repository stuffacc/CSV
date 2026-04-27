#pragma once

typedef enum CellType {
    NUMBER,
    STRING
} CellType;

typedef struct Cell {
    CellType type;
    char* string;
} Cell;

typedef enum RowType {
    HEADER,
    DATA
} RowType;

typedef struct Row {
    RowType type;
    Cell* cells;
} Row;

// создать строку (максимальный размер столбцов, количество столбцов, тип строки)
Row* initRow(int columnsMaxSize[], int columnCount, RowType rowType);
// установить тип строки
Row* setRowType(Row* row, RowType rowType);
// разрушить строку
void destroyRow(Row* row, int columnCount);

// создать ячейки (максимальный размер столбцов, количество столбцов)
Cell* initCells(int columnsMaxSize[], int columnCount);
// установить тип ячейки
Cell* setCellType(Cell* cell, CellType cellType);
// разрушить ячейки
void destroyCells(Cell* cells, int columnCount);

// заполнить ячейки в строку (Row) из сырой строки (char*)
void fillCellsInRow(Row* row, char* rowString);
// заполнить буффер для записи разделения между строками (Row)
void fillWriteRowBreakBuffer(char* writeRowBreakBuffer, RowType rowType, int columnsMaxSize[], int columnCount);
// заполнить буффер для записи данных из строки (Row) в файл
void fillWriteRowBuffer(char* writeRowBuffer, Row* row, int columnsMaxSize[], int columnCount);