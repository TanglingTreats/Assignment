#include "index.h"

void index_add(File_dir *file_dir, Vcb *vol_Blk,
               int numberOfBlocksNeeded, int numberOfData, int *data,
               int identifier, int *entries)
{
    // Size of blocks
    int blockSize = vol_Blk->blockSize;
    if (numberOfBlocksNeeded > vol_Blk->blockSize ||
        /* + 1 to account for the index block*/
        numberOfBlocksNeeded + 1 > checkFreeSpace(vol_Blk))
    {
        printf("\nERROR - File is too big for storage!\n");
        return;
    }

    // Get one data block to store index of other blocks (indexBlock)
    int indexOfindexBlock = nextFreeSpaceIndex(vol_Blk);

    // Updates directory to indicate block is used
    int directoryIndex = dirUpdator(file_dir, vol_Blk, 'i', identifier);
    file_dir->indexed_block[directoryIndex].pos = indexOfindexBlock;
    vol_Blk->freeBlock[indexOfindexBlock] = 1;

    int dataIndex = 0;
    for (int block = 0; block < numberOfBlocksNeeded; block++)
    {
        // Get as many data blocks as needed for file
        int indexCurrentBlock = nextFreeSpaceIndex(vol_Blk);
        // Insert block position into indexBlock
        entries[blockSize * indexOfindexBlock + block] = indexCurrentBlock;
        // Updates directory to indicate block is used
        vol_Blk->freeBlock[indexCurrentBlock] = 1;
        // Insert file data into data blocks

        int i = 0;
        while (dataIndex < numberOfData)
        {
            entries[blockSize * indexCurrentBlock + i] = data[dataIndex];
            dataIndex++;
            i++;
        }
    }
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
