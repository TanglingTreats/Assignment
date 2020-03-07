/*
 Program: contiguous.h. Contains contiguous
 Created on: 4 Mar 2020
 Edited on: 6 Mar 2020
 Created by: KAI
*/

#ifndef CONTIGUOUS_H
#define CONTIGUOUS_H

#include "fileStructure.h"

void contiguous_add(File_dir *file_dir, Vcb *vol_Blk,
                    int numberOfBlocksNeeded, int numberOfData, int *data,
                    int identifier, int *entries);

// Prints name, block number, entry number
void contiguous_read(File_dir *file_dir, Vcb *vol_Blk,
                     int data, int *entries);

// Deletes File, resets used entries to -1, and frees up blocks
void contiguous_delete(File_dir *file_dir, Vcb *vol_Blk,
                       int identifier, int *entries);
#endif