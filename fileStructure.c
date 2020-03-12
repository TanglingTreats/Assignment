/*
 Program: fileStructure.c. Contains function definition for file structure.
 Created on: 24 Jan 2020
 Edited on: 4 Mar 2020 (by peikai)
 Created by: Edwin
*/

#include "fileStructure.h"

void calculateBlock(Vcb *vcb, char option)
{
    if (option == 's')
    {
        vcb->numTotal = HARD_DISK_SIZE / vcb->blockSize;
    }
    else if (option == 'n')
    {
        vcb->blockSize = HARD_DISK_SIZE / vcb->numTotal;
    }
    vcb->numDirBlock = ceil((pow(vcb->numTotal, 2) + vcb->numTotal) / (HARD_DISK_SIZE + vcb->numTotal));
    vcb->numData = vcb->numTotal - vcb->numDirBlock;
    vcb->numFreeData = vcb->numData;

    vcb->freeBlock = (int *)calloc(vcb->numTotal, sizeof(int));

    for (int i = 0; i < vcb->numDirBlock; i++)
    {
        vcb->freeBlock[i] = 2;
    }

    printf("\nTotal blocks: %d\n", vcb->numTotal);
    printf("Number of free blocks: %d\n", checkFreeSpace(vcb));
    printf("Number of directory blocks: %d\n", vcb->numDirBlock);
}

bool allocateMemory(int **memory, Vcb *vol_Blk)
{
    int sizeOfArray = vol_Blk->numData * vol_Blk->blockSize;
    *memory = (int *)calloc(sizeOfArray, sizeof(int));

    int i;
    for (i = 0; i < sizeOfArray; i++)
    {
        (*memory)[i] = -1;
    }

    return (memory != NULL);
}

bool allocateBlock(Block **data, Vcb *vol_Blk, File_dir *dir)
{
    *data = (Block *)calloc(vol_Blk->numData, sizeof(Block));

    if (data != NULL)
    {
        int i;
        for (i = 0; i < vol_Blk->numData; i++)
        {
            (*data)[i].index = i + vol_Blk->numDirBlock;
            (*data)[i].start = checkEntryPosition(i, vol_Blk);
            (*data)[i].end = (*data)[i].start + vol_Blk->blockSize - 1;
        }

        dir->ctg_block = (Ctg_file_dir *)calloc(vol_Blk->numDirBlock * vol_Blk->blockSize, sizeof(Ctg_file_dir));
        dir->linked_block = (Linked_file_dir *)calloc(vol_Blk->numDirBlock * vol_Blk->blockSize, sizeof(Linked_file_dir));
        dir->indexed_block = (Indexed_file_dir *)calloc(vol_Blk->numDirBlock * vol_Blk->blockSize, sizeof(Indexed_file_dir));
        dir->blackOps_block = (BlackOps_file_dir *)calloc(vol_Blk->numDirBlock * vol_Blk->blockSize, sizeof(BlackOps_file_dir));

        if (dir->ctg_block == NULL || dir->linked_block == NULL || dir->indexed_block == NULL || dir->blackOps_block == NULL)
        {
            printAllocateError("file directory");
            return false;
        }

        return true;
    }
    else
    {
        printAllocateError("blocks");
        return false;
    }
}

int checkEntryPosition(int blkNumber, Vcb *vol_Blk)
{
    return blkNumber * vol_Blk->blockSize;
}

void printAllocateError(char *input)
{
    printf("ERROR: Failed to allocate %s\n", input);
}

int dirUpdator(File_dir *file_dir, Vcb *vol_Blk, char option, int identifier)
{
    // contiguous
    if (option == 'c')
    {
        // identifier == -1 means deleting file,
        // != -1 means adding file
        if (identifier != -1)
            for (int i = 0; i < vol_Blk->numDirBlock * vol_Blk->blockSize; i++)
            {
                if (file_dir->ctg_block[i].identifier == 0)
                {
                    file_dir->ctg_block[i].identifier = identifier;
                    // return file
                    return i;
                }
            }
    }

    // indexed
    if (option == 'i')
    {
        // identifier == -1 means deleting file,
        // != -1 means adding file
        if (identifier != -1)
            for (int i = 0; i < vol_Blk->numDirBlock * vol_Blk->blockSize; i++)
            {
                if (file_dir->indexed_block[i].identifier == 0)
                {
                    file_dir->indexed_block[i].identifier = identifier;
                    // return file
                    return i;
                }
            }
    }

    // linked
    if (option == 'l')
    {
        // identifier == -1 means deleting file,
        // != -1 means adding file
        if (identifier != -1)
            for (int i = 0; i < vol_Blk->numDirBlock * vol_Blk->blockSize; i++)
            {
                if (file_dir->linked_block[i].identifier == 0)
                {
                    file_dir->linked_block[i].identifier = identifier;
                    // return file
                    return i;
                }
            }
    }
}

// Checks amount of free space in the file system.
// Restarts numbering to 0, and checks accordingly.
int checkFreeSpace(Vcb *vol_Blk)
{
    vol_Blk->numFreeData = 0;
    for (int i = 0; i < vol_Blk->numTotal; i++)
    {
        if (vol_Blk->freeBlock[i] == 0)
            vol_Blk->numFreeData += 1;
    }

    return vol_Blk->numFreeData;
}

int nextFreeSpaceIndex(Vcb *vol_Blk)
{
    for (int i = 0; i < vol_Blk->numTotal; i++)
    {
        if (vol_Blk->freeBlock[i] == 0)
            return i;
    }
}

int freeSpaceIndex_contiguous(Vcb *vol_Blk, int blocksNeeded)
{
    for (int i = 0; i < vol_Blk->numTotal; i++)
    {
        if (vol_Blk->freeBlock[i] == 0)
        {
            bool pass = true;
            for (int bN = 1; bN < blocksNeeded; bN++)
            {
                if (vol_Blk->freeBlock[bN] == 1)
                    pass = false;
            }
            if (pass == true)
            {
                return i;
            }
        }
    }

    return -1;
}

bool flushFileData(File_dir *file_dir, Vcb *vol_Blk, int *entries)
{
    int numOfDir = vol_Blk->numDirBlock * vol_Blk->blockSize;

    int i;

    for(i = 0; i < numOfDir; i++)
    {
        file_dir->ctg_block[i].identifier = 0;
        file_dir->ctg_block[i].start = 0;
        file_dir->ctg_block[i].length = 0;

        file_dir->linked_block[i].identifier = 0;
        file_dir->linked_block[i].start = 0;
        file_dir->linked_block[i].end = 0;

        file_dir->indexed_block[i].identifier = 0;
        file_dir->indexed_block[i].pos = 0;
    }

    for(i = 0; i < vol_Blk->numData * vol_Blk->blockSize; i++)
    {
        entries[i] = -1;
    }

    for(i = vol_Blk->numDirBlock; i < vol_Blk->numTotal; i++)
    {
        //printf("Value of freeblock before reset: %i\n", vol_Blk->freeBlock[i]);
        vol_Blk->freeBlock[i] = 0;
        //printf("Value of freeblock after reset: %i\n", vol_Blk->freeBlock[i]);
    }

    // for(i = 0; i < vol_Blk->numTotal; i++)
    // {
    //     printf("Vol space tracker %i\n", vol_Blk->freeBlock[i]);
    // }

    checkFreeSpace(vol_Blk);
    printf("\nFree space is now %i\n",vol_Blk->numFreeData );

}