import random
import csv

numberOfInstructions = 50
methodChoices = ["add", "del", "read", "add"]

possiblefiles = [100, 200, 300, 400, 500, 600, 700, 800, 900]
currentfiles= []

with open("kai_test.csv", "w", newline="") as f:
    writer = csv.writer(f)
    for x in range(numberOfInstructions):
        row = []
        instruction = methodChoices[random.randint(0,2)]
        row.append(str(instruction))

        if instruction == "add":
            # get possible files by eliminating current files from possible
            choices = list(set(possiblefiles) - set(currentfiles))
            # choose one
            if len(choices) == 0:
                pass
            elif len(choices) == 1:
                file = choices[0]
            else:
                file = choices[random.randint(0, len(choices) - 1)]
            # add to current files
            currentfiles.append(file)
            # generate file length
            filelength = random.randint(10,65)
            # generate content
            for x in range(file, file+filelength):
                row.append(" " + str(x))

        elif instruction == "del":
            # rolling dice to see if deletes actual file or try to delete non existant file
            # 1 for actual file, 0 for non existant file
            if random.randint(0, 1):
                if len(currentfiles) == 0:
                    pass
                elif len(currentfiles) == 1:
                    file = currentfiles[0]
                else:
                    file = currentfiles[random.randint(0, len(currentfiles) - 1)]
                row.append(" " + str(file))
            else:
                choices = list(set(possiblefiles) - set(currentfiles))                 
                if len(choices) == 0:
                    pass
                elif len(choices) == 1:
                    file = choices[0]
                else:
                    file = choices[random.randint(0, len(choices) - 1)]
                row.append(" " + str(file))
        
        elif instruction == "read":
            # rolling dice to see if read actual file or try to delete non existant file
            # 1 for actual file, 0 for non existant file
            if (len(currentfiles) - 1) >= 0:
                if random.randint(0, 1):
                    if (len(currentfiles) - 1) == 0:
                        file = currentfiles[0] + random.randint(1, 10)
                    else:
                        file = currentfiles[random.randint(0, len(currentfiles) - 1)] + random.randint(1, 10)
                    row.append(" " + str(file))
                else:
                    choices = list(set(possiblefiles) - set(currentfiles))   
                    if len(choices) == 0:
                        pass
                    elif len(choices) == 1:
                        file = choices[0] + random.randint(1, 10)
                    else:    
                        file = choices[random.randint(0, len(choices) - 1)] + random.randint(1, 10)
                    row.append(" " + str(file))
            else:
                choices = list(set(possiblefiles) - set(currentfiles))   
                file = choices[random.randint(0, len(choices) - 1)] + random.randint(1, 10)
                row.append(" " + str(file))
            
        writer.writerow(row)

f.close()