#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "fileStructure.h"

int verticalHeight;

char buffer[134][78];

void blocknumber(int x, int y, int counter);
void indexnumber(int x, int y, int counter);
void calculateVerticalHeight(int size, int bplusNameHeight);
void printdisk(Vcb *vol_Blk, int *entries);

#endif