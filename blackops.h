/*
 Program: blackops.h. Contains function declarations for unique allocation method
 Created on: 12 Mar 2020
 Edited on: 12 Mar 2020
 Created by: Edwin
*/

#ifndef BLACKOPS_H
#define BLACKOPS_H

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "fileStructure.h"

void blackOps_add(File_dir *file_dir, Vcb *vol_Blk, Block *block_Array,
               int numOfBlocksNeeded, int numberOfData, int *data,
               int identifier, int *entries);

void blackOps_read(File_dir *file_dir, Vcb *vol_Blk,
                int data, int *entries);

void blackOps_delete(File_dir *file_dir, Vcb *vol_Blk,
                  int identifier, int *entries);

#endif