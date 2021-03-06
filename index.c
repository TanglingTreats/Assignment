/*
 Program: index.c. Contains index
 Created on: 6 Mar 2020
 Edited on: 6 Mar 2020
 Created by: KAI
*/

#include "index.h"

void index_add(File_dir *file_dir, Vcb *vol_Blk,
               int numberOfBlocksNeeded, int numberOfData, int *data,
               int identifier, int *entries, int*accessCounts)
{
    int accessCounter = 0;
    printf("\nAdding File: %d", identifier);

    // Size of blocks
    int blockSize = vol_Blk->blockSize;
    if (numberOfBlocksNeeded > vol_Blk->blockSize ||
        /* + 1 to account for the index block*/
        numberOfBlocksNeeded + 1 > checkFreeSpace(vol_Blk, &accessCounter))
    {
        printf("\nERROR - File is too big for storage!\n");
        return;
    }

    // Get one data block to store index of other blocks (indexBlock)
    int indexOfindexBlock = nextFreeSpaceIndex(vol_Blk, &accessCounter);

    // Updates directory to indicate block is used
    int directoryIndex = dirUpdator(file_dir, vol_Blk, 'i', identifier);
    file_dir->indexed_block[directoryIndex].pos = indexOfindexBlock;
    vol_Blk->freeBlock[indexOfindexBlock] = 1;

    int dataIndex = 0;
    for (int block = 0; block < numberOfBlocksNeeded; block++)
    {
        // Get as many data blocks as needed for file
        int indexCurrentBlock = nextFreeSpaceIndex(vol_Blk, &accessCounter);

        // Insert block position into indexBlock
        entries[blockSize * (indexOfindexBlock - vol_Blk->numDirBlock) + block] = indexCurrentBlock;
        
        // Updates directory to indicate block is used
        vol_Blk->freeBlock[indexCurrentBlock] = 1;

        // Insert file data into data blocks
        for (int i = 0; i < vol_Blk->blockSize; i++)
            {
            entries[blockSize * (indexCurrentBlock - vol_Blk->numDirBlock) + i] = data[dataIndex];
            dataIndex++;
            }
    }
    printf("\nAccess Count: %d\n", accessCounter);
    *accessCounts += accessCounter;
    return;
}

// Prints name, block number, entry number
void index_read(File_dir *file_dir, Vcb *vol_Blk,
                int data, int *entries, int*accessCounts)
{
    printf("\nReading file: %d", data);
    int accessCounter = 0;
    // Get blocksize, easier to work with afterwards
    int blockSize = vol_Blk->blockSize;

    // Access file dir, to find the index blocks of individual files
    for (int indexOffile_dir = 0; indexOffile_dir < vol_Blk->numDirBlock * blockSize; indexOffile_dir++)
    {
        Indexed_file_dir file = file_dir->indexed_block[indexOffile_dir];

        int fileIdentifier = file.identifier;

        if (fileIdentifier == 0)
        {
            printf("\nERROR - File is not within the system!\n");
            return;
        }

        // Get index of indexblock
        int indexOfindexBlock = file.pos;

        // Go through entries of indexBlock
        for (int indexBlock_iterator = 0; indexBlock_iterator < blockSize; indexBlock_iterator++)
        {
            int blockIndex = entries[blockSize * (indexOfindexBlock - vol_Blk->numDirBlock) + indexBlock_iterator];

            if (blockIndex == -1)
            {
                break;
            }

            // Go into the blocks that indexBlock is pointing to
            for (int entriesIndex = 0; entriesIndex < blockSize; entriesIndex++)
            {
                accessCounter++;
                int adjustedIndex = blockSize * (blockIndex - vol_Blk->numDirBlock) + entriesIndex;

                // printf("\nadjustedIndex: %d", adjustedIndex);
                // Find the data that matches
                if (entries[adjustedIndex] == data)
                {
                    // Prints name, block number, entry number if matches
                    printf("\nFile Name: %d, Block Number: %d, Entry Number: %d",
                           file.identifier, blockIndex, adjustedIndex + vol_Blk->numDirBlock * blockSize);
                    printf("\nAccess Count: %d\n", accessCounter);
                    *accessCounts += accessCounter;
                    return;
                }
            }
        }
    }
    // This is unreachable unless file does not exist
    printf("\nERROR - File is not within the system!\n");
    return;
}

// Deletes File, resets used entries to -1, and frees up blocks
void index_delete(File_dir *file_dir, Vcb *vol_Blk,
                  int identifier, int *entries)
{
    printf("\nDeleting file: %d", identifier);
    // Get blocksize, easier to work with afterwards
    int blockSize = vol_Blk->blockSize;

    // Access file dir, to find the index blocks of individual files
    for (int indexOfFile_dir = 0; indexOfFile_dir < vol_Blk->numDirBlock * blockSize; indexOfFile_dir++)
    {
        Indexed_file_dir file = file_dir->indexed_block[indexOfFile_dir];

        int fileIdentifier = file.identifier;

        if (fileIdentifier == identifier)
        {
            // Get index of indexblock
            int indexOfindexBlock = file.pos;

            // Go through entries of indexBlock
            for (int indexBlock_iterator = 0; indexBlock_iterator < blockSize; indexBlock_iterator++)
            {
                int blockIndex = entries[blockSize * (indexOfindexBlock - vol_Blk->numDirBlock) + indexBlock_iterator];
                if (blockIndex != -1)
                {
                    // Freeing up data block
                    vol_Blk->freeBlock[blockIndex] = 0;
                    // Go into the blocks that indexBlock is pointing to
                    for (int entriesIndex = 0; entriesIndex < blockSize; entriesIndex++)
                    {
                        int adjustedIndex = blockSize * (blockIndex - vol_Blk->numDirBlock) + entriesIndex;

                        // Writing -1 to disk
                        entries[adjustedIndex] = -1;
                    }
                }

                entries[blockSize * (indexOfindexBlock - vol_Blk->numDirBlock) + indexBlock_iterator] = -1;
            }
            // Removing from file directory
            file_dir->indexed_block[indexOfFile_dir].pos = 0;
            file_dir->indexed_block[indexOfFile_dir].identifier = 0;

            // Freeing up index block
            vol_Blk->freeBlock[indexOfindexBlock] = 0;
            printf("\n");
            return;
        }
    }
    // This is unreachable unless file does not exist
    printf("\nERROR - File is not within the system!\n");
    return;
}
