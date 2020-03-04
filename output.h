#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define size 5

int bplusNameHeight = 3 + size;

int verticalHeight;

int sizes[7] = {1, 2, 5, 10, 13, 26, 65};

char buffer[134][78];

void blocknumber(int x, int y, int counter);
void indexnumber(int x, int y, int counter);

#endif