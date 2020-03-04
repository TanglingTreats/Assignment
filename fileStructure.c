/*
 Program: fileStructure.c. Contains function definition for file structure.
 Created on: 24 Jan 2020
 Edited on: 4 Mar 2020
 Created by: Edwin
*/

#include "fileStructure.h"

void calculateBlock(Vcb* vcb, char option)
{
    if(option == 's')
    {
        vcb->numTotal = HARD_DISK_SIZE / vcb->blockSize;
    }
    else if(option == 'n')
    {
        vcb->blockSize = HARD_DISK_SIZE / vcb->numTotal;
    }
    vcb->numDirBlock = ceil((pow(vcb->numTotal, 2) + vcb->numTotal) / (HARD_DISK_SIZE + vcb->numTotal));
    vcb->numData = vcb->numTotal - vcb->numDirBlock;
    vcb->numFreeData = vcb->numData;

    printf("\nTotal blocks: %d\n", vcb->numTotal);
    printf("Number of free blocks: %d\n", vcb->numData);
    printf("Number of directory blocks: %d\n", vcb->numDirBlock);
}

bool allocateMemory(int** memory, Vcb vol_Blk)
{
    *memory = (int*)calloc(vol_Blk.numData * vol_Blk.blockSize, sizeof(int));

    return (memory != NULL);
}


bool allocateBlock(Block** data, Vcb vol_Blk, File_dir* dir)
{
    *data = (Block*)calloc(vol_Blk.numData, sizeof(Block));

    if(data != NULL)
    {
        int i;
        for (i = 0; i < vol_Blk.numData; i++)
        {
            (*data)[i].index = i + vol_Blk.numDirBlock;
            (*data)[i].start = checkEntryPosition (i, vol_Blk);
            (*data)[i].end = (*data)[i].start + vol_Blk.blockSize - 1;
        }

        dir->ctg_block = (Ctg_file_dir*)calloc(vol_Blk.numDirBlock, sizeof(Ctg_file_dir));
        dir->linked_block = (Linked_file_dir*)calloc(vol_Blk.numDirBlock, sizeof(Linked_file_dir));
        dir->indexed_block = (Indexed_file_dir*)calloc(vol_Blk.numDirBlock, sizeof(Indexed_file_dir));

        if(dir->ctg_block == NULL || dir->linked_block == NULL || dir->indexed_block == NULL)
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

int checkEntryPosition (int blkNumber, Vcb vol_Blk)
{
    return blkNumber * vol_Blk.blockSize;
}

void printAllocateError(char* input)
{
    printf("ERROR: Failed to allocate %s\n", input);
}


