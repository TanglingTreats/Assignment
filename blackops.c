/*
 Program: blackops.c. Contains function implementations for unique allocation method
 Created on: 12 Mar 2020
 Edited on: 13 Mar 2020
 Created by: Edwin
*/

#include "blackops.h"

void blackOps_add(File_dir *file_dir, Vcb *vol_Blk, Block *block_Array,
               int numOfBlocksNeeded, int numberOfData, int *data,
               int identifier, int *entries)
{
    printf("\nAdding File: %d\n", identifier);
    
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
                //printf("Data in entries: %i\n", data[entryCounter]);
                entryCounter++;
                if(entryCounter == numberOfData)
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

void blackOps_read(const File_dir *file_dir, const Vcb *vol_Blk, const Block *block_Array,
                int data, int *entries)
{
    printf("\nReading file: %d\n", data);

    int blockPos = 0;
    int entryPos = 0;
    int blockSize = vol_Blk->blockSize;

    bool hasFound = false;

    int i = 0;
    while(file_dir->blackOps_block[i].identifier != 0)
    {
        int startBlk = file_dir->blackOps_block[i].start;
        int endBlk = file_dir->blackOps_block[i].end;

        //printf("Start block: %i\t End block: %i\n", startBlk, endBlk);
        if(startBlk != endBlk)
        {
            int blockPointer = startBlk;

            int indexEntry = block_Array[blockPointer - vol_Blk->numDirBlock].start;
            //printf("Index entry: %i\n", indexEntry);
            // Loops through index blocks
            int j = 0;
            for(j = 0; j < blockSize; j++)
            {
                //printf("entries: %i\n", entries[indexEntry + j]);
                if(entries[indexEntry + j] == -1)
                {
                    break;
                }
                if(j == blockSize - 1 && blockPointer != endBlk)
                {
                    blockPointer = entries[indexEntry + j];
                    //printf("block pointer: %i\n", blockPointer);
                    indexEntry = block_Array[blockPointer - vol_Blk->numDirBlock].start;
                    //printf("Index entry: %i\n", indexEntry);

                    j = 0;
                }
                int startEntry = block_Array[entries[indexEntry + j] - vol_Blk->numDirBlock].start;
                //printf("start entry: %i\n", startEntry);
                
                //Loops through data blocks
                int k = 0;
                for(k = 0; k < blockSize; k++)
                {
                    if(entries[startEntry + k] == data)
                    {
                        entryPos = startEntry + k + (vol_Blk->blockSize * vol_Blk->numDirBlock);
                        blockPos = entries[indexEntry + j];
                        //printf("Entry position where data is found: %i\n", entryPos);
                        hasFound = true;
                        break;
                    }
                    else if(entries[startEntry + k] == -1)
                    {
                        break;
                    }
                }
                if(hasFound)
                {
                    break;
                }
                
            }
        }
        else
        {
            blockPos = startBlk;

            int j = 0;
            int indexEntry = block_Array[blockPos - vol_Blk->numDirBlock].start;
            for(j = 0; j < blockSize; j++)
            {
                int startEntry = block_Array[entries[indexEntry + j] - vol_Blk->numDirBlock].start;

                int k = 0;
                for(k = 0; k < blockSize; k++)
                {
                    //printf("Start entry %i\n", startEntry);
                    //printf("Start entry data: %i\n", entries[startEntry + k]);
                    if(entries[startEntry + k] == data)
                    {
                        entryPos = (startEntry + k) + (vol_Blk->blockSize * vol_Blk->numDirBlock);
                        blockPos = entries[indexEntry + j];
                        //printf("Entry position where data is found: %i\n", entryPos);
                        hasFound = true;
                        break;
                    }
                    else if(entries[startEntry + k] == -1)
                    {
                        break;
                    }
                }
                if(hasFound)
                {
                    break;
                }

                if(entries[indexEntry + j] == -1)
                {
                    break;
                }
            }

        }

        if(hasFound)
        {
            break;
        }
        

        i++;
    }

    if (blockPos == 0 || entryPos == 0)
    {
        printf("File not found!\n");
    }
    else
    {
        printf("File Name: %d, Block Number: %d, Entry Number: %d\n", file_dir->blackOps_block[i].identifier, blockPos, entryPos);
    }
}

void blackOps_delete(File_dir *file_dir, Vcb *vol_Blk, Block *block_Array,
                  int identifier, int *entries)
{
    printf("\nDeleting File: %d\n", identifier);

    int fileIndex = 0;
    while(file_dir->blackOps_block[fileIndex].identifier != 0)
    {
        if(file_dir->blackOps_block[fileIndex].identifier == identifier)
            break;
        fileIndex++;
    }

    if(file_dir->blackOps_block[fileIndex].identifier == 0)
    {
        printf("ERROR - File is not within the system!\n");
    }
    else
    {
        int blockSize = vol_Blk->blockSize;

        int startBlk = file_dir->blackOps_block[fileIndex].start;
        int endBlk = file_dir->blackOps_block[fileIndex].end;

        int indexBlk = 0;

        if(startBlk != endBlk)
        {
            indexBlk = startBlk;
            int indexEntry = block_Array[indexBlk - vol_Blk->numDirBlock].start;

            // Loops through index block
            int j = 0;
            for(j = 0; j < blockSize; j++)
            {
                if(entries[indexEntry + j] == -1)
                {
                    break;
                }
                if(j == blockSize - 1 && indexBlk != endBlk)
                {
                    // Reset free block array in volume block
                    vol_Blk->freeBlock[indexBlk] = 0;

                    indexBlk = entries[indexEntry + j];
                    // Reset pointer in last entry of index block
                    entries[indexEntry + j] = -1;
                    
                    indexEntry = block_Array[indexBlk - vol_Blk->numDirBlock].start;
                    //printf("Index entry: %i\n", indexEntry);
                    j = 0;
                }

                // Sets the start entry of data block within the index block
                int startEntry = block_Array[entries[indexEntry + j] - vol_Blk->numDirBlock].start;

                int k = 0;
                for(k = 0; k < blockSize; k++)
                {
                    entries[startEntry + k] = -1;
                    if(entries[startEntry + k + 1] == -1)
                    {
                        break;
                    }
                }
                // Reset free block array in volume block
                vol_Blk->freeBlock[entries[indexEntry + j]] = 0;

                //Reset index block entries
                entries[indexEntry + j] = -1;
            }

        }
        else
        {
            indexBlk = startBlk;
            int indexEntry = block_Array[indexBlk - vol_Blk->numDirBlock].start;

            // Loops index blocks
            int j = 0;
            for(j = 0; j < blockSize; j++)
            {
                if(entries[indexEntry + j] == -1)
                {
                    break;
                }
                int startEntry = block_Array[entries[indexEntry + j] - vol_Blk->numDirBlock].start;

                // Loops data blocks
                int k = 0;
                for(k = 0; k < blockSize; k++)
                {
                    // Reset data block entries
                    entries[startEntry + k] = -1;
                    if(entries[startEntry + k + 1] == -1)
                    {
                        break;
                    }
                }

                // Reset free block array in volume blocks
                vol_Blk->freeBlock[entries[indexEntry + j]] = 0;

                //Reset index block entries
                entries[indexEntry + j] = -1;

            }
            vol_Blk->freeBlock[indexBlk] = 0;
        }

        if(file_dir->blackOps_block[fileIndex + 1].identifier != 0)
        {
            // Shift file up directory
            int j = fileIndex;
            do
            {
                file_dir->blackOps_block[j].identifier = file_dir->blackOps_block[j + 1].identifier;
                file_dir->blackOps_block[j].start = file_dir->blackOps_block[j + 1].start;
                file_dir->blackOps_block[j].end = file_dir->blackOps_block[j + 1].end;

                j++;
            } while (file_dir->blackOps_block[j].identifier != 0);
            
        }
        else
        {
            file_dir->blackOps_block[fileIndex].identifier = 0;
            file_dir->blackOps_block[fileIndex].start = 0;
            file_dir->blackOps_block[fileIndex].end = 0;
        }
    }
    
}