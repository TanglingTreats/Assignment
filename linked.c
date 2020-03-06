/*
 Program: linked.c. Contains function implementations for linked allocation method
 Created on: 6 Mar 2020
 Edited on: 6 Mar 2020
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
    int counter = 0;

    int ptrsNeeded = 0;

    printf("Number of data: %.0f\n", numberOfData);
    
    int blksNeeded = ceil((numberOfData + ptrsNeeded) / vol_Blk -> blockSize);           
    
    printf("Blocks needed without pointers: %i\n", blksNeeded);

    ptrsNeeded = blksNeeded;

    blksNeeded = ceil((numberOfData + ptrsNeeded) / vol_Blk -> blockSize);

    // Runs if there are not enough pointers given with number of blocks needed for the allocation
    while(blksNeeded != ptrsNeeded)
    {
        counter++;
        printf("Cycle %i in block calculation\n", counter);

        ptrsNeeded = blksNeeded;

        blksNeeded = ceil((numberOfData + ptrsNeeded) / vol_Blk -> blockSize);

    }

    printf("Pointers needed: %i\n", ptrsNeeded);
    printf("Blocks needed with pointers: %i\n", blksNeeded);

    if(blksNeeded <= checkFreeSpace(vol_Blk))
    {
        // Contains list of blocks to store at
        int *blockPointerArr = (int *) calloc(blksNeeded, sizeof(int));
        int filled = 0;                                                         // keeps track of whether blockPointerArr 
                                                                                //is filled
        // Generate indexes of blocks to store data at
        while (filled != blksNeeded)
        {
            int pointer = (rand() % vol_Blk->numFreeData) + vol_Blk->numDirBlock;

            if(vol_Blk->freeBlock[pointer] == 0)
            {
                printf("Block to store at is: %i\n", pointer);
                filled++;
            }
        }
    }
    else
    {
        printf("ERROR - File size is too big. File not added.");
    }
    
}

