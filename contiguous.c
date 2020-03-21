/*
 Program: contiguous.c. Contains contiguous
 Created on: 4 Mar 2020
 Edited on: 6 Mar 2020
 Created by: KAI
*/

#include "contiguous.h"
#include "output.h"

void contiguous_add(File_dir *file_dir, Vcb *vol_Blk,
                    int numberOfBlocksNeeded, int numberOfData, int *data,
                    int identifier, int *entries)
{
    int accessCounter = 0;
    printf("\nAdding File: %d", identifier);
    if (checkFreeSpace(vol_Blk, &accessCounter) >= numberOfBlocksNeeded)
    {   
        int index = freeSpaceIndex_contiguous(vol_Blk, numberOfBlocksNeeded, &accessCounter);
        if (index > -1)
        {
            // Placing data into the entries, adjusted for index position
            for (int d = 0; d < numberOfData; d++)
                entries[(index - vol_Blk->numDirBlock) * vol_Blk->blockSize + d] = data[d];

            int directoryIndex = dirUpdator(file_dir, vol_Blk, 'c', identifier);

            // Index + numDirBlock, to adjust for the directory blocks
            file_dir->ctg_block[directoryIndex].start = index;
            file_dir->ctg_block[directoryIndex].length = numberOfBlocksNeeded;

            // Changing free block states to occupied
            for (int i = 0; i < numberOfBlocksNeeded; i++)
                vol_Blk->freeBlock[index + i] = 1;
            
            // Printing access count
            printf("\nAccess Count: %d\n", accessCounter);
            return;
        }
        printf("\nERROR- Not enough contiguous blocks for data.\n");
        return;
    }
    printf("\nERROR - File is too big for storage\n");
    return;
}

void contiguous_read(File_dir *file_dir, Vcb *vol_Blk,
                     int data, int *entries)
{
    printf("\nReading file: %d", data);
    int accessCounter = 0;
    int block, entryNumber, name;
    // Print name, block number, entry number

    int fileNumber = 0;

    // Go through each file in directory
    while (file_dir->ctg_block[fileNumber].identifier != 0)
    {
        Ctg_file_dir file = file_dir->ctg_block[fileNumber];

        // For each file, scan through all entries, until entry matches data
        for (int block = 0; block < file.length; block++)
        {
            for (int index = 0; index < vol_Blk->blockSize; index++)
            {
                accessCounter++;
                int adjustedIndex = index + (block * vol_Blk->blockSize);
                if (entries[adjustedIndex] == data)
                {
                    printf("\nFile Name: %d, Block Number: %d, Entry Number: %d",
                           file.identifier, (block + file.start), adjustedIndex + vol_Blk->blockSize * file.start);
                    printf("\nAccess Count: %d\n", accessCounter);
                    return;
                }
            }
        }
        fileNumber += 1;
    }
    // This is unreachable unless file does not exist
    printf("\nERROR - File is not within the system!\n");
    return;
}

void contiguous_delete(File_dir *file_dir, Vcb *vol_Blk,
                       int identifier, int *entries)
{
    printf("\nDeleting file: %d", identifier);
    for (int index = 0; index < vol_Blk->numDirBlock * vol_Blk->blockSize; index++)
    {
        if (file_dir->ctg_block[index].identifier == identifier)
        {
            int start = file_dir->ctg_block[index].start - vol_Blk->numDirBlock;
            int length = file_dir->ctg_block[index].length;

            // Freeing up free block states
            for (int i = 0; i < length; i++)
                vol_Blk->freeBlock[(start + vol_Blk->numDirBlock) + i] = 0;

            // Writing -1 to disk
            for (int d = 0; d < length * vol_Blk->blockSize; d++)
                entries[start * vol_Blk->blockSize + d] = -1;

            // Removing from file directory
            file_dir->ctg_block[index].start = 0;
            file_dir->ctg_block[index].length = 0;
            file_dir->ctg_block[index].identifier = 0;

            // Display formatting
            printf("\n");
            return;
        }
    }
    // This is unreachable unless file does not exist
    printf("\nError: File does not exist.\n");
    return;
}