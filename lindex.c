/*
 Program: lindex.c. Contains function implementations for unique allocation method
 Created on: 12 Mar 2020
 Edited on: 21 Mar 2020
 Created by: Edwin
*/

#include "lindex.h"

void blackOps_add(File_dir *file_dir, Vcb *vol_Blk, Block *block_Array,
               int numOfBlocksNeeded, int numberOfData, int *data,
               int identifier, int *entries)
{
    int accessCounter = 0;
    printf("\nAdding File: %d", identifier);

    printf("\nNumber of blocks needed: %i",numOfBlocksNeeded);

    int index = 0;
    bool hasFile = false;
    if(numberOfData == 0)
    {
        printf("\n ERROR - File is empty!");
    }
    else
    {
        while(file_dir->blackOps_block[index].identifier != 0)
        {
            if(file_dir->blackOps_block[index].identifier == identifier)
            {
                hasFile = true;
                break;
            }
            index++;
        }

        if(hasFile)
        {
            printf("\n ERROR - There is a file with the same name in the system!\n");
        }
        else
        {
            int numOfDirBlks = 0;
            
            if(numOfBlocksNeeded > vol_Blk->blockSize)
            {
                numOfDirBlks = ceil((float)numOfBlocksNeeded / (vol_Blk->blockSize - 1));
            }
            else
            {
                numOfDirBlks = 1;
            }

            if((numOfDirBlks + numOfBlocksNeeded) <= checkFreeSpace(vol_Blk, &accessCounter))
            {
                bool hasAllocated = false;

                int dirIndex = dirUpdator(file_dir, vol_Blk, 'b', identifier);

                int numBlksCount = numOfBlocksNeeded;
                int blockSize = vol_Blk->blockSize;

                int entryCounter = 0;


                int dirBlkIndex = nextFreeSpaceIndex(vol_Blk, &accessCounter);
                vol_Blk->freeBlock[dirBlkIndex] = 1;
                file_dir->blackOps_block[dirIndex].start = dirBlkIndex; 

                int dirBlkEntry = block_Array[dirBlkIndex - vol_Blk->numDirBlock].start;
                int i = 0;
                for(i = 0; i < blockSize; i++)
                {
                    if(i == blockSize - 1 && numBlksCount - 1 > 0)
                    {
                        // Get a new index block
                        dirBlkIndex = nextFreeSpaceIndex(vol_Blk, &accessCounter);
                        vol_Blk->freeBlock[dirBlkIndex] = 1;
                        entries[dirBlkEntry + i] = dirBlkIndex;

                        dirBlkEntry = block_Array[dirBlkIndex - vol_Blk->numDirBlock].start;
                        i = 0;
                    }

                    // Get index of dataBlk
                    int dataBlk = nextFreeSpaceIndex(vol_Blk, &accessCounter);
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
                printf("\nAccess Count: %d\n", accessCounter);
            }
            else 
            {
                printf("\nERROR - File size is too big. File not added.\n");
            }
        }
    }
}

void blackOps_read(const File_dir *file_dir, const Vcb *vol_Blk, const Block *block_Array,
                int data, int *entries)
{
    printf("\nReading file: %d", data);

    int accessCounter = 0;
    int blockPos = 0;
    int entryPos = 0;
    int blockSize = vol_Blk->blockSize;

    bool hasFound = false;

    int i = 0;
    while(file_dir->blackOps_block[i].identifier != 0)
    {
        int startBlk = file_dir->blackOps_block[i].start;
        int endBlk = file_dir->blackOps_block[i].end;

        if(startBlk != endBlk)
        {
            int blockPointer = startBlk;

            int indexEntry = block_Array[blockPointer - vol_Blk->numDirBlock].start;

            // Loops through index blocks
            int j = 0;
            for(j = 0; j < blockSize; j++)
            {
                
                if(entries[indexEntry + j] == -1)
                {
                    break;
                }
                if(j == blockSize - 1 && blockPointer != endBlk)
                {
                    blockPointer = entries[indexEntry + j];
                    
                    indexEntry = block_Array[blockPointer - vol_Blk->numDirBlock].start;
                    

                    j = 0;
                }
                int startEntry = block_Array[entries[indexEntry + j] - vol_Blk->numDirBlock].start;
                                
                //Loops through data blocks
                int k = 0;
                for(k = 0; k < blockSize; k++)
                {
                    accessCounter++;
                    if(entries[startEntry + k] == data)
                    {
                        entryPos = startEntry + k + (vol_Blk->blockSize * vol_Blk->numDirBlock);
                        blockPos = entries[indexEntry + j];
                        
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
                    accessCounter++;
                    
                    if(entries[startEntry + k] == data)
                    {
                        entryPos = (startEntry + k) + (vol_Blk->blockSize * vol_Blk->numDirBlock);
                        blockPos = entries[indexEntry + j];
                        
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
        printf("\n ERROR - File is not within the system!\n");
    }
    else
    {
        printf("\nFile Name: %d, Block Number: %d, Entry Number: %d", file_dir->blackOps_block[i].identifier, blockPos, entryPos);
        printf("\nAccess Count: %d\n", accessCounter);
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
        printf("\n ERROR - File is not within the system!");
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

                // Reset last index block
                vol_Blk->freeBlock[indexBlk] = 0;
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