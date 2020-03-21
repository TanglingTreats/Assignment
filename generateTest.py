import random
import csv

# cleaning file first
f = open("kai_test.csv", "w", newline="")
writer = csv.writer(f)
f.close()

# choices
option = "add3"
methodChoices = ["add", "delete", "read"]

# parameters
numberOfInstructions = 100
possiblefiles = [100, 200, 300, 400, 500, 600, 700, 800, 900]
currentfiles= []

minimumFileSize = 1
maximumFileSize = 65

def add1():
    f = open("kai_test.csv", "a", newline="")
    writer = csv.writer(f)
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
    f.close()

def add2():
    f = open("kai_test.csv", "a", newline="")
    writer = csv.writer(f)
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
    f.close()

def add3():
    f = open("kai_test.csv", "a", newline="")
    writer = csv.writer(f)
    global currentfiles
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
    f.close()

def read1():
    f = open("kai_test.csv", "a", newline="")
    writer = csv.writer(f)
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
    f.close()

def read2():
    f = open("kai_test.csv", "a", newline="")
    writer = csv.writer(f)
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
    f.close()

def delete1():
    f = open("kai_test.csv", "a", newline="")
    writer = csv.writer(f)    
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
        # deleting every entry not in file
        for fakefilename in list(set(possiblefiles) - set(currentfiles)):
            for filecontent in range(fakefilename, fakefilename+maximumFileSize):
                writer.writerow(["delete", " " + str(filecontent)])
        # deleting file afterwards
        writer.writerow(["delete", " " + str(filename)])
        currentfiles.remove(filename)
    f.close()

def randomly():
    f = open("kai_test.csv", "a", newline="")
    writer = csv.writer(f)
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

# Testing add function for every file size
if option == "add1":
    add1()

# For every file size, testing every other file size
if option == "add2":
    add2()    

# Test number of files
if option == "add3":
    add3()

# Testing to read every single entry in a file
if option == "read1":
    read1()

# Testing to read every single entry in a file
if option == "read2":
    read2()

# Testing to delete every entry not in file
if option == "delete1":
    delete1()

if option == "random":
    randomly()

if option == "all":
    choices = ["add1", "add2", "add3", "read1", "read2", "delete1", "random"]

    for testcase in choices:
        if testcase == "add1":
            add1()
        if testcase == "add2":
            add2()    
        if testcase == "add3":
            add3()
        if testcase == "read1":
            read1()
        if testcase == "read2":
            read2()
        if testcase == "delete1":
            delete1()
        if testcase == "random":
            randomly()

f.close()