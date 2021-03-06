/*
 Program: index.c. Contains index
 Created on: 6 Mar 2020
 Edited on: 6 Mar 2020
 Created by: KAI
*/

#ifndef INDEX_H
#define INDEX_H

#include "fileStructure.h"

void index_add(File_dir *file_dir, Vcb *vol_Blk,
               int numberOfBlocksNeeded, int numberOfData, int *data,
               int identifier, int *entries, int*accessCounts);

// Prints name, block number, entry number
void index_read(File_dir *file_dir, Vcb *vol_Blk,
                int data, int *entries, int*accessCounts);

// Deletes File, resets used entries to -1, and frees up blocks
void index_delete(File_dir *file_dir, Vcb *vol_Blk,
                  int identifier, int *entries);
#endif