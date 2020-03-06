#include "index.h"

void index_add(File_dir *file_dir, Vcb *vol_Blk,
               int numberOfBlocksNeeded, int numberOfData, int *data,
               int identifier, int *entries)
{
    if (numberOfBlocksNeeded > vol_blk->blockSize ||
        /* + 1 to account for the index block*/
        numberOfBlocksNeeded + 1 > vol_blk->numFreeData)
    {
        printf("File is too big for storing.");
        return;
    }

    // Get one data block to store index of other blocks
    int indexOfindexBlock = nextFreeSpaceIndex(vol_Blk);

    // Updates directory to indicate block is used
    dirUpdator(file_dir, vol_Blk, 'i', indexOfindexBlock);
}

// Prints name, block number, entry number
void index_read(File_dir *file_dir, Vcb *vol_Blk,
                int data, int *entries)
{
}

// Deletes File, resets used entries to -1, and frees up blocks
void index_delete(File_dir *file_dir, Vcb *vol_Blk,
                  int identifier, int *entries)
{
}
