#ifndef CONTIGUOUS_H
#define CONTIGUOUS_H

#include "fileStructure.h"

void contiguous_add(File_dir *file_dir, Vcb *vol_blk,
                    int numberOfBlocksNeeded, int numberOfData, int *data,
                    int identifier, int *entries);

int contiguous_read(File_dir *file_dir, Vcb *vol_Blk,
                    int identifier, int fileIndex, int *entries);

void contiguous_delete(File_dir *file_dir, Vcb *vol_Blk,
                       int identifier, int *entries);
#endif