#pragma once

#include <stdio.h>

#define FAILED -1 // OnFailure
#define SUCCESS 0 // OnSuccess

// тест на количество столбцов в файле
int testGetColumnCountFromFile(FILE* in, int excepted);

// тест на максимальную длину столбцов
int testCountMaxSizeColumnsFromFile(FILE* in, int exceptedSizes[], int columnCount);

// запись в файл, не полный тест (сравнивается отдельно)
int testPrettyWriteInFile(int* columnsMaxSize, int columnCount, FILE* in, FILE* out);

// сравнение двух файлов
int compareFiles(FILE* actual, FILE* excepted);

// запуск всех тестов
int runTests();