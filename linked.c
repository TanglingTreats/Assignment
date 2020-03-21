/*
 Program: linked.c. Contains function implementations for linked allocation method
 Created on: 6 Mar 2020
 Edited on: 21 Mar 2020
 Created by: Edwin
*/

#include "linked.h"

void linked_add(File_dir *file_dir, Vcb *vol_Blk, Block *block_Array,
                float numberOfData, int *data,
                int identifier, int *entries)
{
    // Calculate number of blocks needed for allocation
    // Create an array of size of number of blocks needed
    // Generate a random number from first and last index in block_array
    // If index is free, add to array
    // Else, index + 1. Repeat check.
    // If array is full, start allocation

    // Keeps track of cycle of calculation

    int accessCounter = 0;
    printf("\nAdding File: %d", identifier);

    int index = 0;
    bool hasFile = false;

    if(numberOfData == 0)
    {
        printf("\n ERROR - File is empty!");
    }
    else
    {
        while(file_dir->linked_block[index].identifier != 0)
        {
            if(file_dir->linked_block[index].identifier == identifier)
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
            int counter = 0;

            int ptrsNeeded = 0;
            printf("\n Number of data: %f", numberOfData);
            
            int blksNeeded = ceil(numberOfData / (vol_Blk->blockSize - 1));

            ptrsNeeded = blksNeeded - 1;

            if (blksNeeded <= checkFreeSpace(vol_Blk, &accessCounter))
            {
                // Contains list of blocks to store at
                int *blockPointerArr = (int *)calloc(blksNeeded, sizeof(int));
                int filled = 0; // keeps track of whether blockPointerArr
                                //is filled
                                
                // Generate indexes of blocks to store data at
                printf("\nBlock needed: %i\n", blksNeeded);
                while (filled != blksNeeded)
                {
                    int pointer = (rand() % vol_Blk->numFreeData) + vol_Blk->numDirBlock;

                    if (vol_Blk->freeBlock[pointer] == 0)
                    {
                        printf("Block to store at is: %i\n", pointer);
                        vol_Blk->freeBlock[pointer] = 1;
                        blockPointerArr[filled] = pointer;
                        filled++;
                    }
                }

                int i = 0;
                int dataCount = 0;

                // Allocating data into blocks
                for (i = 0; i < blksNeeded; i++)
                {
                    int start = block_Array[(blockPointerArr[i] - vol_Blk->numDirBlock)].start;
                    int end = block_Array[(blockPointerArr[i] - vol_Blk->numDirBlock)].end;

                    for (; start < end; start++)
                    {
                        if (dataCount < numberOfData)
                        {
                            entries[start] = data[dataCount];
                        }
                        dataCount++;
                    }
                    if (i < blksNeeded - 1)
                    {
                        entries[end] = blockPointerArr[i + 1];
                    }
                }

                int directoryIndex = dirUpdator(file_dir, vol_Blk, 'l', identifier);

                file_dir->linked_block[directoryIndex].start = blockPointerArr[0];
                file_dir->linked_block[directoryIndex].end = blockPointerArr[blksNeeded - 1];
            }
            else
            {
                printf(" ERROR - File size is too big. File not added.");
            }
        }
        
        printf("\nAccess Count: %d\n", accessCounter);
    }
    
    return;
}

int linked_read(const File_dir *file_dir, const Vcb *vol_Blk, const Block *block_Array,
                int data, const int *entries)
{
    // Required information: File name, block number and entry number
    // Loop through file directory one by by
    // For each file, loop through until a file is found and return block and entry index

    printf("\nReading file: %d", data);
    int accessCounter = 0;

    // Keep track of block count
    int blockPos = 0;
    int entryPos = 0;
    int i = 0;

    // Loop through file directory while there is still a file
    while (file_dir->linked_block[i].identifier != 0)
    {
        //printf("File identifier: %i\n", file_dir->linked_block[i].identifier);

        blockPos = file_dir->linked_block[i].start;

        //printf("Starting block: %i\n", blockPos);

        int startEntry = block_Array[blockPos - vol_Blk->numDirBlock].start;

        //printf("Starting entry: %i\n", startEntry);

        int blockSize = vol_Blk->blockSize;

        int j = 0;

        for (j = 0; j < blockSize; j++)
        {
            accessCounter++;
            //printf("Data in entry[%i]: %i\n", startEntry + j, entries[startEntry + j]);
            if (entries[startEntry + j] == data)
            {
                entryPos = (startEntry + j) + (vol_Blk->numDirBlock * vol_Blk->blockSize);
                break;
            }
            else if (entries[startEntry + j] == -1)
            {
                entryPos = 0;
                blockPos = 0;
                break;
            }

            if (j == blockSize - 1)
            {
                int prevEntry = startEntry;
                startEntry = block_Array[entries[startEntry + j] - vol_Blk->numDirBlock].start;

                blockPos = blockPos + ceil(((float)startEntry - (float)prevEntry) / vol_Blk->blockSize);

                // Reset count of j to -1 to compensate for increment after each round
                j = -1;
            }
        }
        if (entries[startEntry + j] == data)
        {
            break;
        }

        // Increment counteer
        i++;
    }

    if (blockPos == 0 || entryPos == 0)
    {
        printf("\n ERROR - File is not within the system!\n");
    }
    else
    {
        printf("\nFile Name: %d, Block Number: %d, Entry Number: %d", file_dir->linked_block[i].identifier, blockPos, entryPos);
        printf("\nAccess Count: %d\n", accessCounter);
    }
}

void linked_delete(File_dir *file_dir, Vcb *vol_Blk, const Block *block_Array,
                   int identifier, int *entries)
{
    printf("\nDeleting File: %d\n", identifier);
    // Search file_dir for identifier and take note of index within file_dir array
    // Go through each block and make them -1 until next entry within block is -1
    // If there are files after current file index, shift upwards until current + 1 is 0

    int fileIndex = 0;

    while (file_dir->linked_block[fileIndex].identifier != 0)
    {
        if (file_dir->linked_block[fileIndex].identifier == identifier)
            break;
        fileIndex++;
    }
    if (file_dir->linked_block[fileIndex].identifier == 0)
    {
        printf(" ERROR - File is not within the system!\n");
    }
    else
    {
        //printf("Deleting file %i\n", identifier);

        int startBlock = file_dir->linked_block[fileIndex].start;

        int startEntry = block_Array[startBlock - vol_Blk->numDirBlock].start;

        int i = 0;
        int blockSize = vol_Blk->blockSize;

        while (i < blockSize)
        {
            if (entries[startEntry + i] == -1)
            {
                // Reset state of last block in file system
                vol_Blk->freeBlock[startBlock] = 0;
                break;
            }

            if (i == blockSize - 1)
            {

                vol_Blk->freeBlock[startBlock] = 0;

                startBlock = entries[startEntry + i];

                entries[startEntry + i] = -1;

                startEntry = block_Array[startBlock - vol_Blk->numDirBlock].start;
                i = -1;
            }
            else
            {
                // Resetting position
                entries[startEntry + i] = -1;
            }

            i++;
        }

        if (file_dir->linked_block[fileIndex + 1].identifier != 0)
        {
            // Shift file up the directory
            int j = fileIndex;
            do
            {

                file_dir->linked_block[j].identifier = file_dir->linked_block[j + 1].identifier;
                file_dir->linked_block[j].start = file_dir->linked_block[j + 1].start;
                file_dir->linked_block[j].end = file_dir->linked_block[j + 1].end;

                j++;
            } while (file_dir->linked_block[j].identifier != 0);
        }
        else
        {
            file_dir->linked_block[fileIndex].identifier = 0;
            file_dir->linked_block[fileIndex].start = 0;
            file_dir->linked_block[fileIndex].end = 0;
        }
    }

}