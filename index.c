#include "index.h"

void index_add(File_dir *file_dir, Vcb *vol_Blk,
               int numberOfBlocksNeeded, int numberOfData, int *data,
               int identifier, int *entries)
{
    // Size of blocks
    int blockSize = vol_Blk->blockSize;
    if (numberOfBlocksNeeded > vol_Blk->blockSize ||
        /* + 1 to account for the index block*/
        numberOfBlocksNeeded + 1 > checkFreeSpace(vol_Blk))
    {
        printf("\nERROR - File is too big for storage!\n");
        return;
    }

    // Get one data block to store index of other blocks (indexBlock)
    int indexOfindexBlock = nextFreeSpaceIndex(vol_Blk);

    // Updates directory to indicate block is used
    int directoryIndex = dirUpdator(file_dir, vol_Blk, 'i', identifier);
    file_dir->indexed_block[directoryIndex].pos = indexOfindexBlock;
    vol_Blk->freeBlock[indexOfindexBlock] = 1;

    int dataIndex = 0;
    for (int block = 0; block < numberOfBlocksNeeded; block++)
    {
        // Get as many data blocks as needed for file
        int indexCurrentBlock = nextFreeSpaceIndex(vol_Blk);
        // Insert block position into indexBlock
        entries[blockSize * indexOfindexBlock + block] = indexCurrentBlock;
        // Updates directory to indicate block is used
        vol_Blk->freeBlock[indexCurrentBlock] = 1;
        // Insert file data into data blocks

        int i = 0;
        while (dataIndex < numberOfData)
        {
            entries[blockSize * indexCurrentBlock + i] = data[dataIndex];
            dataIndex++;
            i++;
        }
    }
}

// Prints name, block number, entry number
void index_read(File_dir *file_dir, Vcb *vol_Blk,
                int data, int *entries)
{
    printf("Reading file: %d\n", data);
    // Get blocksize, easier to work with afterwards
    int blockSize = vol_Blk->blockSize;

    // Initialise index of file dir to 0
    int indexOffile_dir = 0;
    // Access file dir, to find the index blocks on individual files
    while (file_dir->indexed_block[indexOffile_dir].identifier != 0)
    {

        // Go into each individual file
        Indexed_file_dir file = file_dir->indexed_block[indexOffile_dir];

        // Get index of indexblock
        int indexOfindexBlock = file.pos;

        // initialise indexBlock iterator to 0
        int indexBlock_iterator = 0;

        // Go through entries of indexBlock
        while (entries[blockSize * indexOfindexBlock + indexBlock_iterator] != -1)
        {
            int blockIndex = entries[blockSize * indexOfindexBlock + indexBlock_iterator];
            // Go into the blocks that indexBlock is pointing to
            for (int entriesIndex = 0; entriesIndex < blockSize; entriesIndex++)
            {
                int adjustedIndex = blockSize * blockIndex + entriesIndex;

                // Find the data that matches
                if (entries[adjustedIndex] == data)
                {
                    // Prints name, block number, entry number if matches
                    printf("File Name: %d, Block Number: %d, Entry Number: %d\n",
                           file.identifier, blockIndex, adjustedIndex);
                    return;
                }
            }
            // Go to next entry of indexBlock
            indexBlock_iterator++;
        }
        // increment index
        indexOffile_dir++;
    }
    // This is unreachable unless file does not exist
    printf("File does not exist.");
    return;
}

// Deletes File, resets used entries to -1, and frees up blocks
void index_delete(File_dir *file_dir, Vcb *vol_Blk,
                  int identifier, int *entries)
{
}
