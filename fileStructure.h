/*
 Program: fileStructure.h. Contains block and entry declarations
 Created on: 24 Jan 2020
 Edited on: 3 Feb 2020
 Created by: Edwin
*/

#ifndef FILE_STRUCTURE_H
#define FILE_STRUCTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "constant.h"

// Block-type struct that keeps track of start and end of each block
typedef struct block
{
    int index, start, end;

} Block;

// Contiguous file directory: Stores identifier, start position and length of block
typedef struct ctg_file_dir
{
    int identifier, start, length;
} Ctg_file_dir;

// Linked file directory: Stores identifier, start and end position of linked list
typedef struct linked_file_dir
{
    int identifier, start, end;
} Linked_file_dir;

// Indexed file directory: Stores identifier and position of index block
typedef struct indexed_file_dir
{
    int identifier, pos;
} Indexed_file_dir;

typedef struct blackOps_file_dir
{
    int identifier, pos;
} BlackOps_file_dir;

// Unique file structure here

//Contains struct depending on allocation method
typedef struct file_dir
{
    Ctg_file_dir *ctg_block;
    Linked_file_dir *linked_block;
    Indexed_file_dir *indexed_block;
    BlackOps_file_dir *blackOps_block;
} File_dir;

// Volume control block: Stores total number of blocks, number of free blocks, block size and a pointer to the linked list of free blocks.
typedef struct vcb
{
    // Total number of blocks
    int numTotal;

    // Number of directory blocks given
    int numDirBlock;

    // Number of data blocks allowed for storage
    int numData;

    // Current number of free data blocks
    int numFreeData;

    // Current block size
    int blockSize;

    // Pointer to structure keeping track of free block
    // 0 -> free
    // 1 -> currently storing data
    // 2 -> directory
    int *freeBlock;
} Vcb;

// Performs calculation of main memory blocks and file directory blocks
void calculateBlock(Vcb *vcb, char option);

// Allocate entries for memory according to the specified block size
bool allocateMemory(int **memory, Vcb *vol_Blk);

// Allocate blocks of memory and file directory according to the inputs
bool allocateBlock(Block **data, Vcb *vol_Blk, File_dir *dir);

// Calculate position of first entry of specified block
int checkEntryPosition(int blkNumber, Vcb *vol_Blk);

// Prints error message based on input;
// "ERROR: Failed to allocate -input-"
void printAllocateError(char *input);

// Updates File Directory and returns file's directory index
int dirUpdator(File_dir *file_dir, Vcb *vol_Blk, char option, int identifier);

// Updates and returns number of free blocks
int checkFreeSpace(Vcb *vol_Blk);

// Returns index of next free block
int nextFreeSpaceIndex(Vcb *vol_Blk);

// Returns index where x number of contiguous blocks are free for file
// returns -1 if doesn't exist
int freeSpaceIndex_contiguous(Vcb *vol_Blk, int blocksNeeded);

// Resets everything in the directory and entries to 0
bool flushFileData(File_dir *file_dir, Vcb *vol_Blk, int *entries);

#endif