/*
 Program: main.c. Starting point of the program.
 Created on: 24 Jan 2020
 Edited on: 7 Mar 2020
 Created by: Edwin

 Algorithm:
 - Prompt desired block size

 Error codes:
 -1 - Memory allocation failed
 -2 - Block allocation failed
 -3 - File open failed
*/

#include <stdio.h>
#include <ctype.h>

#include "fileStructure.h"
#include "constant.h"
#include "fileRead.h"
#include "contiguous.h"
#include "index.h"
#include "linked.h"
#include "output.h"

#define DEBUG 0

// ---- Function Prototype ----
void printInputError();
void freePointers(int *entries, Block *block_Array, File_dir *file_dir, FILE *file);

int main(int argc, char **argv)
{
    // block size for options
    int blkSize = -1;

    // number of blocks for options
    int numOfBlk = -1;

    // Character input option: For selecting block size or number of blocks
    char option = ' ';

    // Integer input choice: For selecting allocation methods or quitting program
    // 0 - Contiguous Allocation
    // 1 - Linked Allocation
    // 2 - Indexed Allocation
    // 3 - Unique Allocation
    // 4 - Exit
    // Default - Error
    int choice = -1;

    // File data and block pointer integer array
    int *entries;

    // Blocks to track entries
    Block *block_Array;

    // File directory struct that contains other file structures
    File_dir file_dir;

    // Volume control block
    Vcb vol_Blk;
    vol_Blk.blockSize = 0;
    vol_Blk.numTotal = 0;

    // File pointer
    FILE *fp;

    printf("\n---------- Shrodinger's OS ------------\n");
    printf("\nWelcome User\n");

    printf("Current block size is: %d\n", vol_Blk.blockSize);
    printf("Current number of blocks is: %d\n", vol_Blk.numTotal);

    while (option != 's' && option != 'n')
    {
        printf("\nWould you like to input block size or number of blocks? s/n: ");
        scanf(" %c", &option);
        if (option == 's')
        {
            while (blkSize < 2)
            {
                printf("\nPlease input your desired block size: ");
                scanf("%d", &blkSize);

                if (blkSize < 2)
                {
                    printf("\nChosen block size cannot be less than 2\n");
                }
                else
                {
                    vol_Blk.blockSize = blkSize;
                }
            }
        }
        else if (option == 'n')
        {
            while (numOfBlk < 2)
            {
                printf("\nPlease input your desired number of blocks: ");
                scanf("%d", &numOfBlk);

                if (numOfBlk < 2)
                {
                    printf("\nChosen number of blocks cannot be less than 2\n");
                }
                else
                {
                    vol_Blk.numTotal = numOfBlk;
                }
            }
        }
        else
        {
            printInputError();
        }
    }

    // Send volume block for calculation.
    calculateBlock(&vol_Blk, option);

    printf("\nYour desired block size is: %d\n", vol_Blk.blockSize);
    printf("Your desired number of blocks is: %d\n", vol_Blk.numTotal);

    // Allocate memory after block calculation
    if (allocateMemory(&entries, &vol_Blk))
    {
        printf("\nMemory allocation successful!\n");
    }
    else
    {
        printf("\nERROR - Failed to allocate memory!");
        return 1;
    }

    if (allocateBlock(&block_Array, &vol_Blk, &file_dir))
    {
        printf("\nBlock allocation successful!\n");

// Debug block allocation
#if DEBUG
        int i;
        for (i = 0; i < vol_Blk.numData; i++)
        {
            printf("Index: %d. Start: %d. End: %d.\n", block_Array[i].index, block_Array[i].start, block_Array[i].end);
        }
        for (i = 0; i < vol_Blk.numDirBlock; i++)
        {
            printf("contiguous %d\n", file_dir.ctg_block[i].identifier);
            printf("linked %d\n", file_dir.linked_block[i].identifier);
            printf("index %d\n", file_dir.indexed_block[i].identifier);
        }

        printf("Number of free blocks: %i\n", vol_Blk.numFreeData);
#endif
    }
    else
    {
        printf("\nERROR - Failed to allocate blocks!");

        return 2;
    }

    fp = fopen("CSC1007-SampleCSV.csv", "r");
    if (fp != NULL)
    {
        printf("\nFile open successful!\n");
    }
    else
    {
        printf("\nERROR - File open failed!");
        return -3;
    }

    // Main program loop
    while (true)
    {
        printf("\nWhat would you like to do now? Key in a number:\n");
        printf("0 - Contiguous Allocation\n");
        printf("1 - Linked Allocation\n");
        printf("2 - Indexed Allocation\n");
        printf("3 - Unique Allocation\n");
        printf("4 - Exit Program\n: ");

        // Read in an integer
        scanf("%d", &choice);
        while (getchar() != '\n')
            ; //Error-checking for character and string inputs

        if (choice >= 0 && choice < 4)
        {
            // Execute allocation methods etc.
            char comm[7];

            // Contains all information about the file that is to be added, read or deleted
            int *fileInfo;

            // Contains only the data about the file
            int *fileData;

            // The integer identifier of the file
            int fileIdentifier = 0;

            //  The total size of the file including the identifier
            int fileDataSize = 0;

            // Number of blocks needed to store a particular file
            int numOfBlocksNeeded = 0;
            while (readFile(fp, comm, &fileInfo, &fileDataSize))
            {
                printf("\nThe command is: %s\n", comm);
                fileIdentifier = fileInfo[0];

                //Loop through as long as there is a line to read
                if (!strcmp(comm, "add"))
                {
                    int numOfBlocksNeeded = ceil(((float)fileDataSize - 1) / vol_Blk.blockSize);

                    // Initialise data of file
                    fileData = &fileInfo[1];

                    if (choice == 0)
                    {
                        // contiguous
                        contiguous_add(&file_dir, &vol_Blk, numOfBlocksNeeded, fileDataSize - 1,
                                       fileData, fileIdentifier, entries);
                    }
                    else if (choice == 1)
                    {
                        linked_add(&file_dir, &vol_Blk, block_Array, fileDataSize - 1, fileData, fileIdentifier, entries);
                    }
                    else if (choice == 2)
                    {
                        // indexed
                        index_add(&file_dir, &vol_Blk, numOfBlocksNeeded, fileDataSize - 1,
                                  fileData, fileIdentifier, entries);
                    }
                    else if (choice == 3)
                    {
                    }
                }
                // If reading, fileInfo has only 1 element inside
                else if (!strcmp(comm, "read"))
                {
                    if (choice == 0)
                    {
                        // contiguous
                        contiguous_read(&file_dir, &vol_Blk, fileIdentifier, entries);
                    }
                    else if (choice == 1)
                    {
                        linked_read(&file_dir, &vol_Blk, block_Array, fileIdentifier, entries);
                    }
                    else if (choice == 2)
                    {
                        printf("Reading block - index\n");
                    }
                    else if (choice == 3)
                    {
                    }
                }
                // If deleting, fileInfo has only 1 element inside
                else if (!strcmp(comm, "delete"))
                {
                    if (choice == 0)
                    {
                        // contiguous
                        contiguous_delete(&file_dir, &vol_Blk, fileIdentifier, entries);
                    }
                    else if (choice == 1)
                    {
                        printf("Deleting block - linked\n");
                    }
                    else if (choice == 2)
                    {
                        printf("Deleting block - index\n");
                    }
                    else if (choice == 3)
                    {
                    }
                }
            }
            // Print Output
            printdisk(&vol_Blk, entries);

            // Reset choice
            choice = -1;
            fseek(fp, 0, SEEK_SET);
        }
        else if (choice == 4)
        {
            break;
        }
        else
        {
            printInputError();
        }
    }

    freePointers(entries, block_Array, &file_dir, fp);

    printf("\nThank you for using Shrodinger's OS\n");
    printf("\n----------- End of Program ------------\n");
    return 0;
}

// -------------------------------------------------------------

void printInputError()
{
    printf("\nInvalid input detected. Please try again.\n");
}

void freePointers(int *entries, Block *block_Array, File_dir *file_dir, FILE *file)
{
    printf("\nFree contiguous\n");
    free(file_dir->ctg_block);

    printf("Free linked\n");
    free(file_dir->linked_block);

    printf("Free indexed\n");
    free(file_dir->indexed_block);

    //unique file needs to be freed

    printf("Free blocks\n");
    free(block_Array);

    printf("Free entries\n");
    free(entries);

    printf("Free file pointer\n");
    free(file);
}