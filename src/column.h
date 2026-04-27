#pragma once

// Получить количество столбцов
int getColumnCount(const char* line);

// Обновить максимальный размер каждого столбца
void updateMaxSizeColums(const char* line, int columnsMaxSize[]);