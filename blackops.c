/*
 Program: blackops.c. Contains function implementations for unique allocation method
 Created on: 12 Mar 2020
 Edited on: 12 Mar 2020
 Created by: Edwin
*/

#include "blackops.h"

void blackOps_add(File_dir *file_dir, Vcb *vol_Blk, Block *block_Array,
               int numOfBlocksNeeded, int numberOfData, int *data,
               int identifier, int *entries)
{
    printf("Adding File: %d\n", identifier);
    
    int numOfDirBlks = 0;
    //printf("Number of blocks needed: %i\n", numOfBlocksNeeded);
    
    if(numOfBlocksNeeded > vol_Blk->blockSize)
    {
        numOfDirBlks = ceil((float)numOfBlocksNeeded / (vol_Blk->blockSize - 1));
    }
    else
    {
        numOfDirBlks = 1;
    }

    //printf("Number of directory blocks needed is: %i\n", numOfDirBlks);

    if((numOfDirBlks + numOfBlocksNeeded) <= checkFreeSpace(vol_Blk))
    {
        bool hasAllocated = false;

        int dirIndex = dirUpdator(file_dir, vol_Blk, 'b', identifier);

        int numBlksCount = numOfBlocksNeeded;
        int blockSize = vol_Blk->blockSize;

        int entryCounter = 0;


        int dirBlkIndex = nextFreeSpaceIndex(vol_Blk);
        vol_Blk->freeBlock[dirBlkIndex] = 1;
        file_dir->blackOps_block[dirIndex].start = dirBlkIndex; 

        // printf("Index of first indexed block: %i\n", dirBlkIndex);
        // printf("numBlksCount: %i\n", numBlksCount);

        int dirBlkEntry = block_Array[dirBlkIndex - vol_Blk->numDirBlock].start;
        int i = 0;
        for(i = 0; i < blockSize; i++)
        {
            if(i == blockSize - 1 && numBlksCount - 1 > 0)
            {
                // printf("i: %i \t numBlksCountr: %i\n", i, numBlksCount);
                // printf("If require more index blocks\n");
                // Get a new index block
                dirBlkIndex = nextFreeSpaceIndex(vol_Blk);
                vol_Blk->freeBlock[dirBlkIndex] = 1;
                entries[dirBlkEntry + i] = dirBlkIndex;

                dirBlkEntry = block_Array[dirBlkIndex - vol_Blk->numDirBlock].start;
                i = 0;
            }

            // Get index of dataBlk
            int dataBlk = nextFreeSpaceIndex(vol_Blk);
            vol_Blk->freeBlock[dataBlk] = 1;

            // Add index of data block to index block
            entries[dirBlkEntry + i] = dataBlk;

            int startEntryIndex = block_Array[dataBlk - vol_Blk->numDirBlock].start;

            int j = 0;
            for(j = 0; j < blockSize; j++)
            {
                //Fill up entry
                entries[startEntryIndex + j] = data[entryCounter];
                entryCounter++;
                if(!data[entryCounter + 1])
                {
                    hasAllocated = true;
                    break;
                }
            }
            if(hasAllocated)
            {
                break;
            }
            numBlksCount--;

        }

        file_dir->blackOps_block[dirIndex].end = dirBlkIndex; 
        
        //printf("Start blk: %i\t End blk: %i\n", file_dir->blackOps_block[dirIndex].start, file_dir->blackOps_block[dirIndex].end);
    }
    else 
    {
        printf("ERROR - File size is too big. File not added.\n");
    }
    
}