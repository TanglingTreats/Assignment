import math

numberOfEntries = 130
blockSize, numberofBlocks, dirBlocks, dataBlocks = 0, 0, 0, 0

sizeornumber = input("Block size or number of blocks? s/n: ")

if sizeornumber.lower() == 's':
    blockSize = int(input("Enter block size: "))
    numberofBlocks = int(130/blockSize)
elif sizeornumber.lower() == 'n':
    numberofBlocks = int(input("Enter number of blocks: "))
    blockSize = int(130/numberofBlocks)

# Calculations
dirBlocks = math.ceil(
    (pow(numberofBlocks, 2) + numberofBlocks) / (130 + numberofBlocks))
dataBlocks = numberofBlocks - dirBlocks

# Checking Values
print(blockSize, numberofBlocks, dirBlocks, dataBlocks)

dataEntries = []
for x in range(dataBlocks * blockSize):
    dataEntries.append(0)

# Checking Length of data entries
print(len(dataEntries))
# Printing data blocks
for x in range(dataBlocks):
    if x % 5 == 0:
        print('\n')
    else:
        print(' ', end='')
    for y in range(blockSize):
        print(dataEntries[x*y], end='')


def checkPosition(blockNumber):
    return blockNumber * blockSize


print('\n')
while True:
    test = int(input('Which block? : '))
    print(checkPosition(test))
