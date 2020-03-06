#include "contiguous.h"

void contiguous_add(File_dir *file_dir, Vcb *vol_Blk, Block *block_Array,
                    int numberOfBlocksNeeded, int numberOfData, int *data,
                    int identifier, int *entries)
{
    if (checkFreeSpace(vol_Blk) >= numberOfBlocksNeeded)
    {
        int index = freeSpaceIndex_contiguous(vol_Blk, numberOfBlocksNeeded);
        if (index > -1)
        {
            // Placing data into the entries, adjusted for index position
            for (int d = 0; d < numberOfData; d++)
                entries[index * vol_Blk->blockSize + d] = data[d];

            int filePosition = dirUpdator(file_dir, vol_Blk, 'c', identifier);

            // Index + numDirBlock, to adjust for the directory blocks
            file_dir->ctg_block[filePosition].start = index + vol_Blk->numDirBlock;
            file_dir->ctg_block[filePosition].length = numberOfBlocksNeeded;

            // Changing free block states to occupied
            for (int i = 0; i < numberOfBlocksNeeded; i++)
                vol_Blk->freeBlock[i] = 1;

            return;
        }
        printf("Not enough contiguous blocks for data.\n");
        return;
    }
    printf("Not enough space for data.\n");
    return;
}

int contiguous_read(File_dir *file_dir, Vcb *vol_Blk,
                    int identifier, int fileIndex, int *entries)
{
    for (int index = 0; index < vol_Blk->numDirBlock * vol_Blk->blockSize; index++)
    {
        if (file_dir->ctg_block[index].identifier == identifier)
        {
            // - vol_Blk->numDirBlock to adjust for directory blocks
            int startingBlock = file_dir->ctg_block[index].start - vol_Blk->numDirBlock;
            // calculating entries index, adjusting for blocks fileIndex
            int entriesIndex = fileIndex + startingBlock * vol_Blk->blockSize;

            return entries[entriesIndex];
        }
    }
}

void contiguous_delete(File_dir *file_dir, Vcb *vol_Blk,
                       int identifier, int fileIndex, int *entries)
{
    for (int index = 0; index < vol_Blk->numDirBlock * vol_Blk->blockSize; index++)
    {
        if (file_dir->ctg_block[index].identifier == identifier)
        {
            int start = file_dir->ctg_block[index].start;
            int length = file_dir->ctg_block[index].length;

            // Freeing up free block states
            for (int i = 0; i < (int)ceil(length / blockSize); i++)
                vol_Blk->freeBlock[i] = 0;

            // Writing -1 to disk
            for (int d = 0; d < length * vol_Blk->blockSize; d++)
                entries[index * vol_Blk->blockSize + d] = -1;

            // Removing from file directory
            file_dir->ctg_block[index].start = 0;
            file_dir->ctg_block[index].length = 0;
            file_dir->ctg_block[index].identifier = 0;
        }
    }
}