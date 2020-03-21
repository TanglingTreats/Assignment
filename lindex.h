/*
 Program: lindex.h. Contains function declarations for unique allocation method
 Created on: 12 Mar 2020
 Edited on: 21 Mar 2020
 Created by: Edwin
*/

#ifndef LINDEX_H
#define LINDEX_H

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "fileStructure.h"

void lindex_add(File_dir *file_dir, Vcb *vol_Blk, Block *block_Array,
               int numOfBlocksNeeded, int numberOfData, int *data,
               int identifier, int *entries, int *accessCounts);

void lindex_read(const File_dir *file_dir, const Vcb *vol_Blk, const Block *block_Array,
                int data, int *entries, int *accessCounts);

void lindex_delete(File_dir *file_dir, Vcb *vol_Blk, Block *block_Array,
                  int identifier, int *entries);

#endif