/*
 Program: main.c. Starting point of the program.
 Created on: 24 Jan 2020
 Edited on: 21 Mar 2020
 Created by: Edwin

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
#include "lindex.h"
#include "output.h"

#define DEBUG 0

// ---- Function Prototype ----
void printInputError();
void freePointers(int *entries,Vcb *vcb, Block *block_Array, File_dir *file_dir, FILE *file);

// accesscounters
int accessCounts[4] = {0,0,0,0};
int *accessCounts_ptr = accessCounts;

int main(int argc, char **argv)
{
    char *inputName;

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

    if(argc > 1)
    {
        inputName = argv[1];
        printf("file name: %s\n", inputName);
    }
    else
    {
        printf("No input files specified. Resorting to default\n");
    }

    printf("\n---------- Shrodinger's OS ------------\n");
    printf("\nWelcome User\n");

    printf("Current block size is: %d\n", vol_Blk.blockSize);
    printf("Current number of blocks is: %d\n", vol_Blk.numTotal);

    while (option != 's' && option != 'S' && option != 'n' && option != 'N')
    {
        printf("\nWould you like to input block size or number of blocks? s/n: ");
        scanf(" %c", &option);
        while (getchar() != '\n')
            ; //Error-checking for character and string inputs
        if (option == 's' || option == 'S')
        {
            while (blkSize < 2 || blkSize > 65)
            {
                printf("\nPlease input your desired block size: ");
                scanf("%d", &blkSize);

                if (blkSize < 2)
                {
                    printf("\nChosen block size cannot be less than 2\n");
                }
                else if (blkSize > 65)
                {
                    printf("\nChosen block size cannot be more than 65\n");
                }
                else
                {
                    vol_Blk.blockSize = blkSize;
                }
            }
        }
        else if (option == 'n' || option == 'N')
        {
            while (numOfBlk < 2 || numOfBlk > 65)
            {
                printf("\nPlease input your desired number of blocks: ");
                scanf("%d", &numOfBlk);

                if (numOfBlk < 2)
                {
                    printf("\nChosen number of blocks cannot be less than 2\n");
                }
                else if (numOfBlk > 65)
                {
                    printf("\nChosen number of blocks cannot be more than 65\n");
                }
                else {
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
            printf("Black Ops %d\n", file_dir.blackOps_block[i].identifier);
        }

        printf("Number of free blocks: %i\n", vol_Blk.numFreeData);
        #endif
    }
    else
    {
        printf("\nERROR - Failed to allocate blocks!");

        return 2;
    }

    // --------------- FILE READ SECTION --------------
    if(argc > 1)
    {
        fp = fopen(inputName, "r");
    }
    else
    {
        fp = fopen("test_file.csv", "r");
    }
    // Check file pointer validity
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
        // Eliminates invalid input loop
        while (getchar() != '\n')
            ; //Error-checking for character and string inputs

        if (choice >= 0 && choice < 4)
        {
            // Resets all data within structures
            flushFileData(&file_dir, &vol_Blk, entries);

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

            int instructionCount = 0;
            int dataUtilisation = 0;
            while (readFile(fp, comm, &fileInfo, &fileDataSize))
            {
                fileIdentifier = fileInfo[0];
                //printf("fileIdentifier: %i\n", fileIdentifier);

                //Loop through as long as there is a line to read
                if (!strcmp(comm, "add"))
                {
                    int numOfBlocksNeeded = ceil(((float)fileDataSize - 1) / vol_Blk.blockSize);

                    // Initialise data of file
                    fileData = &fileInfo[1];
                    if(fileDataSize == 1)
                    {
                        fileData = (int *)calloc(1, sizeof(int));
                    }
                    else
                    {
                        printf("\nData: %i", fileInfo[1]);
                    }

                    if (choice == 0)
                    {
                        // contiguous
                        contiguous_add(&file_dir, &vol_Blk, numOfBlocksNeeded, fileDataSize - 1,
                                    fileData, fileIdentifier, entries, &accessCounts_ptr[0]);
                    }
                    else if (choice == 1)
                    {
                        linked_add(&file_dir, &vol_Blk, block_Array, fileDataSize - 1, 
                                    fileData, fileIdentifier, entries, &accessCounts_ptr[1]);
                    }
                    else if (choice == 2)
                    {
                        // indexed
                        index_add(&file_dir, &vol_Blk, numOfBlocksNeeded, fileDataSize - 1,
                                    fileData, fileIdentifier, entries, &accessCounts_ptr[2]);
                    }
                    else if (choice == 3)
                    {
                        lindex_add(&file_dir, &vol_Blk, block_Array, numOfBlocksNeeded, fileDataSize - 1, 
                                    fileData, fileIdentifier, entries, &accessCounts_ptr[3]);
                    }
                }
                // If reading, fileInfo has only 1 element inside
                else if (!strcmp(comm, "read"))
                {
                    if (choice == 0)
                    {
                        // contiguous
                        contiguous_read(&file_dir, &vol_Blk, fileIdentifier, entries, &accessCounts_ptr[0]);
                    }
                    else if (choice == 1)
                    {
                        // linked
                        linked_read(&file_dir, &vol_Blk, block_Array, fileIdentifier, entries, &accessCounts_ptr[1]);
                    }
                    else if (choice == 2)
                    {
                        // index
                        index_read(&file_dir, &vol_Blk, fileIdentifier, entries, &accessCounts_ptr[2]);
                    }
                    else if (choice == 3)
                    {
                        // black ops
                        lindex_read(&file_dir, &vol_Blk, block_Array, fileIdentifier, entries, &accessCounts_ptr[3]);
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
                        linked_delete(&file_dir, &vol_Blk, block_Array, fileIdentifier, entries);
                    }
                    else if (choice == 2)
                    {
                        index_delete(&file_dir, &vol_Blk, fileIdentifier, entries);
                    }
                    else if (choice == 3)
                    {
                        lindex_delete(&file_dir, &vol_Blk, block_Array, fileIdentifier, entries);
                    }
                }

                instructionCount++;
                for (int i = 0; i < vol_Blk.numData; i++)
                    if (entries[i] == -1)
                        dataUtilisation++;
            }
            // Print Output
            printdisk(&vol_Blk, &file_dir, entries, choice);

            if(choice == 0)
            {
                printf("Contiguous Allocation\n");
                printf("Total Access counts: %d", accessCounts_ptr[0]);
                accessCounts_ptr[0] = 0;
            }
            else if(choice == 1)
            {
                printf("Linked Allocation\n");
                printf("Total Access counts: %d", accessCounts_ptr[1]);
                accessCounts_ptr[1] = 0;
            }
            else if(choice == 2)
            {
                printf("Indexed Allocation\n");
                printf("Total Access counts: %d", accessCounts_ptr[2]);
                accessCounts_ptr[2] = 0;
            }
            else if(choice == 3)
            {
                printf("Unique Allocation\n");
                printf("Total Access counts: %d", accessCounts_ptr[3]);
                accessCounts_ptr[3] = 0;
            }            
            printf("\nSpace Utilisation: %0.2f%%\n", (instructionCount * vol_Blk.numData - dataUtilisation * 1.0) / (instructionCount * vol_Blk.numData) * 100);
            dataUtilisation = 0;
            instructionCount = 0;

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

    freePointers(entries, &vol_Blk, block_Array, &file_dir, fp);
    printf("\nThank you for using Shrodinger's OS\n");
    printf("\n----------- End of Program ------------\n");
    return 0;
}

// -------------------------------------------------------------

void printInputError()
{
    printf("\nInvalid input detected. Please try again.\n");
}

void freePointers(int *entries, Vcb *vcb, Block *block_Array, File_dir *file_dir, FILE *file)
{
    free(file_dir->ctg_block);
    printf("\nFree contiguous\n");

    free(file_dir->linked_block);
    printf("Free linked\n");

    free(file_dir->indexed_block);
    printf("Free indexed\n");

    free(file_dir->lindex_block);
    printf("Free blackops\n");

    free(vcb->freeBlock);
    printf("Free vcb\n");

    free(block_Array);
    printf("Free blocks\n");

    free(entries);
    printf("Free entries\n");

    //unique file needs to be freed

    fclose(file);
    printf("Free file pointer\n");
}
