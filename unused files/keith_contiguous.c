#include "contiguous.h"

int contiguous(Vcb vol_Blk, int *entries)
{
    // Print out user input:
    printf("User's input: add, 100, 101, 102, 103, 104, 105, 106 \n");

    // To get the number of element slots needed
    int elementSlots = 0;
    char str[1000] = "add, 100, 101, 102, 103, 104, 105, 106";
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == ',')
            elementSlots++;
    }

    // Show the number of elements slots required
    // elementSlots -1 as we will not include the first element ('100') after the 'add'
    printf("Element slots required: %d\n", elementSlots - 1);

    // Method to split user input String by ', '
    char *array[10];
    int j = 0;
    array[j] = strtok(str, ", ");
    while (array[j] != NULL)
    {
        array[++j] = strtok(NULL, ", ");
    }

    // Method to store the values of the user input into userInputArray
    int userInputArray[] = {};
    int tempCount = 0;
    while (array[tempCount] != NULL)
    {
        userInputArray[tempCount] = atoi(array[tempCount]);
        //printf("%i\n", userInputArray[tempCount]);
        tempCount++;
    }
    for (int i = 1; i < tempCount; i++)
        userInputArray[i - 1] = userInputArray[i];

    // Calculate the number of blocks required by the user's input
    int blocksNeeded = (elementSlots + vol_Blk.blockSize - 1) / vol_Blk.blockSize;
    //blocksNeeded = ceil(elementSlots / vol_Blk.blockSize);
    printf("Blocks needed: %d\n", blocksNeeded);

    // Array to track the position of the first element of each block
    int firstElementArray[vol_Blk.numFreeData][vol_Blk.blockSize];
    int blockPosition = vol_Blk.numDirBlock + 1;
    for (int firstPos = 0; firstPos < vol_Blk.numFreeData; firstPos++)
    {
        firstElementArray[firstPos][0] = firstPos;
        firstElementArray[firstPos][1] = blockPosition;
        printf("#%d: %d\n", firstElementArray[firstPos][0] + 1, firstElementArray[firstPos][1]);
        blockPosition += vol_Blk.blockSize;
    }

    int rowBreak = 0;

    // Method that uses the sumChecker to check if the block is empty
    // Sum of '0' means that the block is empty
    // Variable sumChecker is used to check if all the elements in the block is empty
    int emptySumChecker;
    int emptyArrayTracker[vol_Blk.numFreeData][2];
    for (int a = 0; a < vol_Blk.numFreeData; a++)
    {
        // printf("%d \n", entries[firstElementArray[a]]);

        for (int b = 0; b < vol_Blk.blockSize; b++)
        {
            //printf("%d", entries[firstElementArray[a] + b-1]);
            emptySumChecker += entries[firstElementArray[a][1] + b - 1];
        }
        //printf("Sum of block #%d: %d \n", a+1, sum);

        if (emptySumChecker == 0)
        {
            emptyArrayTracker[a][0] = a;
            emptyArrayTracker[a][1] = 999;
        }

        emptySumChecker = 0;
    }

    // Method to list the blocks that will be used
    int countBlocks = blocksNeeded;
    int arrayToBeUsed[vol_Blk.numFreeData][100];
    printf("\nAdding file%d and found free ", userInputArray[0]);
    for (int d = 0; d < vol_Blk.numFreeData; d++)
    {
        if (countBlocks != 0)
        {
            if (emptyArrayTracker[d][1] == 999)
            {
                // printf("\nd is %d\n", d);
                arrayToBeUsed[d][0] = emptyArrayTracker[d][0];
                arrayToBeUsed[d][1] = firstElementArray[d][1];
                printf("B%d (Pos %d), ", (int)arrayToBeUsed[d][0] + 1, (int)arrayToBeUsed[d][1]);
                countBlocks = countBlocks - 1;
            }
        }
    }

    printf("\b\b ");

    printf("\n\nAdded file%d at ", userInputArray[0]);

    // Method to add the element to arrayToBeUsed accordingly
    //
    int elementCounter = 1;
    int d1 = 0;
    int d2Counter = 0;
    printf("B%d(", d1 + 1);
    for (int d2 = 2; d2 < vol_Blk.blockSize + 2; d2++)
    {
        arrayToBeUsed[d1][d2] = userInputArray[elementCounter];
        printf("%d", arrayToBeUsed[d1][d2]);
        elementCounter++;
        d2Counter++;
        if (d2Counter != vol_Blk.blockSize)
        {
            printf(", ");
        }

        if (d2Counter == vol_Blk.blockSize)
        {
            printf("), ");
            d1 = d1 + 1;
            d2 = 1;
            printf("B%d(", d1 + 1);
        }

        if (elementCounter == elementSlots)
        {
            printf("\b\b)");
            break;
        }
    }

    printf("\n\n");

    //blocksNeeded = LEN(arrayToBeUsed);
    int posToInsert = arrayToBeUsed[0][1] - 1;

    for (int g = 1; g < elementSlots; g++)
    {
        entries[posToInsert] = userInputArray[g];
        posToInsert++;
    }

    // Print out the available elements in entries [] array in row of vol_Blk.blockSize
    rowBreak = 0;
    for (int rowCounter2 = vol_Blk.numDirBlock; rowCounter2 < 130; rowCounter2++)
    {
        printf("[%d] ", entries[rowCounter2]);
        rowBreak++;

        if (rowBreak == vol_Blk.blockSize)
        {
            printf("\n");
            rowBreak = 0;
        }
    }
    // End of program
}
