#include <stdio.h>
#include <stdbool.h>

#define size 5

int sizes[7] = {1, 2, 5, 10, 13, 26, 65};

char buffer[56][78];

void blocknumber(int x, int y, int counter);
void indexnumber(int x, int y, int counter);

typedef struct block
{
    bool taken;
    int contents[size];
} block;

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

void main()
{
    block one = {true};
    one.contents[0] = 23;

    // Drawing Boxes
    for (int y = 0; y < 57; y++)
    {
        for (int x = 0; x < 78; x++)
        {
            buffer[y][x] = ' ';
            if (y % 8 == 0)
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

    // Titles
    int blockcounter = 0;
    int indexcounter = 0;
    char blockname[18] = "     block cc     ";
    char titles[18] = "  index  content  ";
    for (int y = 1; y < 50; y += 8)
    {
        for (int x = 1; x < 78 - 19; x += 19)
        {
            for (int z = 0; z < 18; z++)
            {
                buffer[y][x + z] = blockname[z];
                buffer[y + 1][x + z] = titles[z];
            }
            blocknumber(x, y, blockcounter);
            blockcounter += 1;
            for (int z = 2; z < 7; z++)
            {
                indexnumber(x, y + z, indexcounter);
                indexcounter += 1;
            }
        }
    }

    // Printing
    for (int y = 0; y < 57; y++)
    {
        for (int x = 0; x < 78; x++)
        {
            printf("%c", buffer[y][x]);
        }
    }
}