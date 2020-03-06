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

    printf("Number of data: %.0f\n", numberOfData);
    
    int blksNeeded = ceil(numberOfData / vol_Blk->blockSize);               // number of blocks pre-pointer
    
    printf("Blocks needed without pointers: %i\n", blksNeeded);

    int ptrsNeeded = blksNeeded - 1;
    blksNeeded = ceil((numberOfData + ptrsNeeded) / vol_Blk -> blockSize);  // number of blocks post-pointer

    printf("Pointers needed: %i\n", ptrsNeeded);
    printf("Blocks needed with pointers: %i\n", blksNeeded);

}

