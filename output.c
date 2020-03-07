#include "output.h"

void blocknumber(int x, int y, int counter)
{
    int tens = counter / 10;
    int ones = counter % 10;
    buffer[y][x + 11] = (char)48 + tens;
    buffer[y][x + 12] = (char)48 + ones;
}

void indexnumber(int x, int y, int counter)
{
    int hundreds = counter / 100;
    int tens = (counter / 10) % 10;
    int ones = counter % 10;
    buffer[y][x + 3] = (char)48 + hundreds;
    buffer[y][x + 4] = (char)48 + tens;
    buffer[y][x + 5] = (char)48 + ones;
}

void calculateVerticalHeight(int size, int bplusNameHeight)
{
    verticalHeight = (int)(ceil(ceil(130 / size) / 4) * bplusNameHeight);
}

void printdisk(Vcb *vol_Blk, int *entries)
{
    int size = vol_Blk->blockSize;
    int bplusNameHeight = 3 + size;

    calculateVerticalHeight(size, bplusNameHeight);

    // Drawing Boxes
    for (int y = 0; y < verticalHeight + 1; y++)
    {
        for (int x = 0; x < 78; x++)
        {
            buffer[y][x] = ' ';
            if (y % bplusNameHeight == 0)
            {
                buffer[y][x] = '_';
            }
            else if (x % 19 == 0 || x == 0)
            {
                buffer[y][x] = '|';
            }
            if (x == 77)
            {
                buffer[y][x] = '\n';
            }
        }
    }

    // Titles, Index, Data
    int blockCounter = 0;
    int indexCounter = 0;
    int entriesCounter = 0;
    char blockname[18] = "     block cc     ";
    char titles[18] = "  index  content  ";
    for (int y = 1; y < verticalHeight - size; y += bplusNameHeight)
    {
        for (int x = 1; x < 78 - 19; x += 19)
        {
            // Putting in Titles
            for (int z = 0; z < 18; z++)
            {
                buffer[y][x + z] = blockname[z];
                buffer[y + 1][x + z] = titles[z];
            }

            blocknumber(x, y, blockCounter);
            blockCounter += 1;

            // Putting in Index
            for (int z = 2; z < bplusNameHeight - 1; z++)
            {
                indexnumber(x, y + z, indexCounter);
                indexCounter += 1;
            }

            // Putting in Data
            for (int z = 2; z < bplusNameHeight - 1; z++)
            {
                buffer[y + z][x + 11] = 'h';
                buffer[y + z][x + 12] = 'e';
                buffer[y + z][x + 13] = 'y';
                entriesCounter += 1;
            }
        }
    }

    // Printing
    // for (int y = 0; y < verticalHeight + 1; y++)
    // {
    //     for (int x = 0; x < 78; x++)
    //     {
    //         printf("%c", buffer[y][x]);
    //     }
    // }

    for (int i = 0; i < size * vol_Blk->numData; i++)
    {
        if (i % size == 0)
        {
            printf("\n");
        }
        printf("%d, ", entries[i]);
    }
    printf("\n");
}