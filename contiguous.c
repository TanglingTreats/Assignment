#include "contiguous.h"

void contiguous_add(File_dir *file_dir, Vcb *vol_blk, Block *block_Array,
                    int numberOfBlocksNeeded, int numberOfData, int *data,
                    int identifier, int *entries)
{
    if (checkFreeSpace(vol_blk) >= numberOfBlocksNeeded)
    {
        int index = freeSpaceIndex_contiguous(vol_blk, numberOfBlocksNeeded);
        if (index > -1)
        {
            // Placing data into the entries, adjusted for index position
            for (int d = 0; d < numberOfData; d++)
                entries[index * vol_blk->blockSize + d] = data[d];

            int filePosition = dirUpdator(file_dir, vol_blk, 'c', identifier);

            file_dir->ctg_block[filePosition].start = index * vol_blk->blockSize;
            file_dir->ctg_block[filePosition].length = numberOfData;
        }
    }
}

void contiguous_read(Ctg_file_dir file_dir, Vcb *vol_blk, Block *block_Array,
                     int fileNumber, int *entries)
{
}

void contiguous_delete(Ctg_file_dir file_dir, Vcb *vol_blk, Block *block_Array,
                       int fileNumber, int *entries)
{
}