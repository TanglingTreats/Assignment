#include "output.h"

void printdisk(Vcb *vol_Blk, File_dir *file_dir, int *entries, int choice)
{
    int size = vol_Blk->blockSize;
    int blockCounter = 0;
    printf("\nDirectory Blocks: ");
    for (int i = 0; i < size * vol_Blk->numDirBlock; i++)
    {
        if (i % size == 0)
        {
            printf("\nBlock %d: \t", blockCounter);
            blockCounter++;
        }

        // 0 -> Contiguous
        // 1 -> Linked
        // 2 -> Indexed
        // 3 -> Black Ops (Same as Indexed)
        if (choice == 0)
        {
            printf("\n\tFile ID: %d", file_dir->ctg_block[i].identifier);
            printf("\tStart:\t%d", file_dir->ctg_block[i].start);
            printf("\tLength:\t%d", file_dir->ctg_block[i].length);
        }
        else if (choice == 1)
        {
            printf("\n\tFile ID: %d", file_dir->linked_block[i].identifier);
            printf("\tStart:\t%d", file_dir->linked_block[i].start);
            printf("\tEnd:\t%d", file_dir->linked_block[i].end);
        }
        else if (choice == 2)
        {
            printf("\n\tFile ID: %d", file_dir->indexed_block[i].identifier);
            printf("\tIndex Block:\t%d", file_dir->indexed_block[i].pos);
        }
        else if (choice == 3)
        {
            printf("\n\tFile ID: %d", file_dir->lindex_block[i].identifier);
            printf("\nStart:\t%d", file_dir->lindex_block[i].start);
            printf("\nEnd:\t%d", file_dir->lindex_block[i].end);
        }
    }
    printf("\n");

    size = vol_Blk->blockSize;
    bool first = true;
    printf("\nData Blocks: ");
    for (int i = 0; i < size * vol_Blk->numData; i++)
    {
        if (i % size == 0)
        {
            printf("\nBlock %d: \t", blockCounter);

            first = true;
            blockCounter++;
        }
        if (entries[i] == -1 && first)
        {
            if (i % size == 0)
            {
                printf("0, ");
            }
            else
            {
                printf("%d, ", entries[i]);
            }

            first = false;
        }
        else if (entries[i] == -1)
            printf("0, ");
        else
            printf("%d, ", entries[i]);
    }
    printf("\n\n");
}

// print -1 for last entry of block, if it contains data
// else replace -1 with 0