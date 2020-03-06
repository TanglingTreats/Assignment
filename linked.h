/*
 Program: linked.h. Contains function declarations for linked allocation method
 Created on: 6 Mar 2020
 Edited on: 6 Mar 2020
 Created by: Edwin
*/

#ifndef LINKED_H
#define LINKED_H

#include "fileStructure.h"

void linked_add(File_dir *file_dir, Vcb *vol_blk, Block *block_Array,
                    int numberOfBlocksNeeded, int numberOfData, int *data,
                    int identifier, int *entries);

int linked_read(File_dir *file_dir, Vcb *vol_Blk,
                    int identifier, int fileIndex, int *entries);

void linked_delete(File_dir *file_dir, Vcb *vol_Blk,
                       int identifier, int *entries);

#endif