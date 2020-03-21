import random
import csv

f = open("kai_test.csv", "w", newline="")
writer = csv.writer(f)

# choices
option = "add2"
methodChoices = ["add", "delete", "read"]

# parameters
numberOfInstructions = 50
possiblefiles = [100, 200, 300, 400, 500, 600, 700, 800, 900]
currentfiles= []

minimumFileSize = 1
maximumFileSize = 65

# Testing add function for every file size
if option == "add1":
    for fileLength in range(minimumFileSize, maximumFileSize):
        row = []
        # assigning instruction to add
        instruction = methodChoices[0]
        # adding instruction to row
        row.append(str(instruction))
        # deciding file name by random
        filename = possiblefiles[random.randint(0, len(possiblefiles) - 1)]
        # creating and adding file data to row
        for filecontent in range(filename, filename+fileLength):
            row.append(" " + str(filecontent))
        writer.writerow(row)
        # deleting file afterwards
        writer.writerow(["delete", " " + str(filename)])

# For every file size, testing every other file size
if option == "add2":
    for fileLength in range(minimumFileSize, maximumFileSize):
        row = []
        # assigning instruction to add
        instruction = methodChoices[0]
        # adding instruction to row
        row.append(str(instruction))
        # deciding file name by random
        filename = possiblefiles[random.randint(0, len(possiblefiles) - 1)]
        currentfiles.append(filename)
        # creating and adding file data to row
        for filecontent in range(filename, filename+fileLength):
            row.append(" " + str(filecontent))

        for fileLength2 in range(minimumFileSize, maximumFileSize):
            row2 = []
            # assigning instruction to add
            instruction = methodChoices[0]
            # adding instruction to row
            row2.append(str(instruction))
            # deciding file name by random
            filename2 = list(set(possiblefiles) - set(currentfiles))[random.randint(0, len(possiblefiles) - len(currentfiles) - 1)]
            # creating and adding file data to row
            for filecontent in range(filename2, filename2+fileLength2):
                row2.append(" " + str(filecontent))
            writer.writerow(row2)
            # deleting file afterwards
            writer.writerow(["delete", " " + str(filename2)])
        
        writer.writerow(row)
        # deleting file afterwards
        currentfiles.remove(filename)
        writer.writerow(["delete", " " + str(filename)])





    # for x in range(numberOfInstructions):
    #     row = []
    #     instruction = methodChoices[random.randint(0,2)]
    #     row.append(str(instruction))

    #     if instruction == "add":
    #         # get possible files by eliminating current files from possible
    #         choices = list(set(possiblefiles) - set(currentfiles))
    #         # choose one
    #         if len(choices) == 0:
    #             pass
    #         elif len(choices) == 1:
    #             file = choices[0]
    #         else:
    #             file = choices[random.randint(0, len(choices) - 1)]
    #         # add to current files
    #         currentfiles.append(file)
    #         # generate file length
    #         filelength = random.randint(10,130)
    #         # generate content
    #         for x in range(file, file+filelength):
    #             row.append(" " + str(x))

    #     elif instruction == "del":
    #         # rolling dice to see if deletes actual file or try to delete non existant file
    #         # 1 for actual file, 0 for non existant file
    #         if random.randint(0, 1):
    #             if len(currentfiles) == 0:
    #                 pass
    #             elif len(currentfiles) == 1:
    #                 file = currentfiles[0]
    #             else:
    #                 file = currentfiles[random.randint(0, len(currentfiles) - 1)]
    #             row.append(" " + str(file))
    #         else:
    #             choices = list(set(possiblefiles) - set(currentfiles))                 
    #             if len(choices) == 0:
    #                 pass
    #             elif len(choices) == 1:
    #                 file = choices[0]
    #             else:
    #                 file = choices[random.randint(0, len(choices) - 1)]
    #             row.append(" " + str(file))
        
    #     elif instruction == "read":
    #         # rolling dice to see if read actual file or try to delete non existant file
    #         # 1 for actual file, 0 for non existant file
    #         if (len(currentfiles) - 1) >= 0:
    #             if random.randint(0, 1):
    #                 if (len(currentfiles) - 1) == 0:
    #                     file = currentfiles[0] + random.randint(1, 10)
    #                 else:
    #                     file = currentfiles[random.randint(0, len(currentfiles) - 1)] + random.randint(1, 10)
    #                 row.append(" " + str(file))
    #             else:
    #                 choices = list(set(possiblefiles) - set(currentfiles))   
    #                 if len(choices) == 0:
    #                     pass
    #                 elif len(choices) == 1:
    #                     file = choices[0] + random.randint(1, 10)
    #                 else:    
    #                     file = choices[random.randint(0, len(choices) - 1)] + random.randint(1, 10)
    #                 row.append(" " + str(file))
    #         else:
    #             choices = list(set(possiblefiles) - set(currentfiles))   
    #             file = choices[random.randint(0, len(choices) - 1)] + random.randint(1, 10)
    #             row.append(" " + str(file))
            
    #     writer.writerow(row)

f.close()