#pragma once
#include <stdio.h>

int getColumnCount(char* line);
void updateMaxSizeColums(char* line, int columnsMaxSize[]);
void printMaxSizes(int columnsMaxSize[], int columnCount);
int max(int value1, int value2);