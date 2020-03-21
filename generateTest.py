import random
import csv

f = open("kai_test.csv", "w", newline="")
writer = csv.writer(f)

# choices
option = "random"
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

# Test number of files
if option == "add3":
    for startingfileLength in range(minimumFileSize, maximumFileSize):
        for fileLength in range(startingfileLength, maximumFileSize):
            # checking if there are still files
            if len(currentfiles) == 9:
                for filename in currentfiles:
                    writer.writerow(["delete", " " + str(filename)])
                currentfiles = []
            row = []
            # assigning instruction to add
            instruction = methodChoices[0]
            # adding instruction to row
            row.append(str(instruction))
            # deciding file name by random
            filename = list(set(possiblefiles) - set(currentfiles))[random.randint(0, len(possiblefiles) - len(currentfiles) - 1)]
            currentfiles.append(filename)
            # creating and adding file data to row
            for filecontent in range(filename, filename+fileLength):
                row.append(" " + str(filecontent))
            writer.writerow(row)
        # Deleting all files
        for filename in currentfiles:
            writer.writerow(["delete", " " + str(filename)])
            currentfiles = []

# Testing to read every single entry in a file
if option == "read1":
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
        # reading every entry in file
        for filecontent in range(filename + 1, filename+fileLength):
            writer.writerow(["read", " " + str(filecontent)])
        # deleting file afterwards
        writer.writerow(["delete", " " + str(filename)])

# Testing to read every single entry in a file
if option == "read2":
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
        writer.writerow(row)
        # reading every entry not in file
        for fakefilename in list(set(possiblefiles) - set(currentfiles)):
            for filecontent in range(fakefilename, fakefilename+maximumFileSize):
                writer.writerow(["read", " " + str(filecontent)])
        # deleting file afterwards
        writer.writerow(["delete", " " + str(filename)])
        currentfiles.remove(filename)

if option == "delete":
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
        writer.writerow(row)
        # reading every entry not in file
        for fakefilename in list(set(possiblefiles) - set(currentfiles)):
            for filecontent in range(fakefilename, fakefilename+maximumFileSize):
                writer.writerow(["delete", " " + str(filecontent)])
        # deleting file afterwards
        writer.writerow(["delete", " " + str(filename)])
        currentfiles.remove(filename)

if option == "random":
    for x in range(numberOfInstructions):
        row = []
        instruction = methodChoices[random.randint(0,2)]
        row.append(str(instruction))

        if instruction == "add":
            # get possible files by eliminating current files from possible
            choices = list(set(possiblefiles) - set(currentfiles))
            # if there are less than 2 possible files, create possible files by deleting current files
            while len(choices) < 2:
                chosenfile = currentfiles[random.randint(0, 9 - len(choices) -1)]
                writer.writerow(["delete", " " + str(chosenfile)])
                currentfiles.remove(chosenfile)
                choices = list(set(possiblefiles) - set(currentfiles))
            # choose one
            file = choices[random.randint(0, len(choices) - 1)]
            # add to current files
            currentfiles.append(file)
            # generate file length
            filelength = random.randint(10,130)
            # generate content
            for x in range(file, file+filelength):
                row.append(" " + str(x))

        elif instruction == "delete":
            # if there are no current files, choose 2 file to add
            while len(currentfiles) < 2:
                choices = list(set(possiblefiles) - set(currentfiles))
                chosenfile = choices[random.randint(0, len(choices) - 1)]
                writer.writerow(["add", " " + str(chosenfile)])
                currentfiles.append(chosenfile)
            
            # rolling dice to see if deletes actual file or try to delete non existant file
            # 1 for actual file, 0 for non existant file
            if random.randint(0, 1):
                # delete a file
                file = currentfiles[random.randint(0, len(currentfiles) - 1)]
                row.append(" " + str(file))
            else:
                # if there are no current files, choose 2 file to add
                choices = list(set(possiblefiles) - set(currentfiles))
                file = choices[random.randint(0, len(choices) - 1)]
                row.append(" " + str(file))
        
        elif instruction == "read":
            # if there are no current files, choose 2 file to add
            while len(currentfiles) < 2:
                choices = list(set(possiblefiles) - set(currentfiles))
                chosenfile = choices[random.randint(0, len(choices) - 1)]
                writer.writerow(["add", " " + str(chosenfile)])
                currentfiles.append(chosenfile)
            
            # rolling dice to see if read actual file or try to delete non existant file
            # 1 for actual file, 0 for non existant file
            if random.randint(0, 1):
                file = currentfiles[random.randint(0, len(currentfiles) - 1)] + random.randint(1, 10)
                row.append(" " + str(file))
            else:
                choices = list(set(possiblefiles) - set(currentfiles))   
                file = choices[random.randint(0, len(choices) - 1)] + random.randint(1, 10)
                row.append(" " + str(file))            
        writer.writerow(row)

f.close()