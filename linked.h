/*
 Program: linked.h. Contains function declarations for linked allocation method
 Created on: 6 Mar 2020
 Edited on: 7 Mar 2020
 Created by: Edwin
*/

#ifndef LINKED_H
#define LINKED_H

#include <math.h>
#include "fileStructure.h"

void linked_add(File_dir *file_dir, Vcb *vol_Blk, Block *block_Array,
                    float numberOfData, int *data,
                    int identifier, int *entries);

int linked_read(const File_dir *file_dir, const Vcb *vol_Blk, const Block *block_Array,
                     int data, const int *entries);

void linked_delete(File_dir *file_dir, Vcb *vol_Blk,
                       int identifier, int *entries);

#endif