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
    int numOfDirBlks = 0;
    printf("Number of blocks needed: %i\n", numOfBlocksNeeded);
    
    if(numOfBlocksNeeded > vol_Blk->blockSize)
    {
        numOfDirBlks = ceil((float)numOfBlocksNeeded / (vol_Blk->blockSize - 1));

    }
    else
    {
        numOfDirBlks = 1;
    }

    printf("Number of directory blocks needed is: %i\n", numOfDirBlks);

    if((numOfDirBlks + numOfBlocksNeeded) <= checkFreeSpace(vol_Blk))
    {
        int indexBlk = 0;

        int i;
        int count = 0;
        for(i = 0; i < numOfDirBlks; i++)
        {
            indexBlk = nextFreeSpaceIndex(vol_Blk);
            vol_Blk->freeBlock[indexBlk] = 1;

            printf("Index block: %i\n", indexBlk);

            int j;
            int startEntry = block_Array[indexBlk-vol_Blk->numDirBlock].start;
            int blockSize = vol_Blk->blockSize;
            for(j = 0; j < numOfBlocksNeeded; j++)
            {
                int assignBlock = nextFreeSpaceIndex(vol_Blk);
                entries[startEntry + j] = assignBlock;
                vol_Blk->freeBlock[assignBlock] = 1;

                printf("Data block: %i\n", assignBlock);

                int k;
                int dataStart = block_Array[assignBlock - vol_Blk->numDirBlock].start;
                for(k = 0; k < blockSize; k++)
                {
                    if(!data[count])
                    {
                        break;
                    }
                    printf("Entry index: %i\n", dataStart+k);
                    printf("Entry: %i\n", data[count]);
                    count++;
                }

            }
        }
    }
    else 
    {
        printf("ERROR - File size is too big. File not added.\n");
    }
    
}