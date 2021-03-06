/*
 Program: fileRead.h
 Created on: 13 Feb 2020
 Edited on: 4 Mar 2020
 Created by: Edwin
*/

#ifndef FILE_READ_H
#define FILE_READ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct op_store
{
    // Stores string input to be interpreted
    char* command;

    // Stores integer operands
    int* operand;
} Op_store;

typedef char String[1024];

bool readFile (FILE *file, char *comm, int **data, int *size);

#endif