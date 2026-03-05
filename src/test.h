#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "column.h"
#include "file.h"
#include "row.h"

int runTests();
int testAndClose(FILE* in, FILE* expectedOut);
int compareFilesByLinesAndClose(FILE* expectedOut);