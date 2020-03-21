import random
import csv

numberOfInstructions = 100
methodChoices = ["add", "del", "read"]

with open("kai_test.csv", "w", newline="") as f:
    writer = csv.writer(f)
    for x in range(numberOfInstructions):
        row = []
        instruction = methodChoices[random.randint(0,2)]
        row.append(str(instruction))
        file = random.randint(1,9) * 100
        if instruction != "add":
            row.append(" " + str(file))
        else:
            filelength = random.randint(1,9)
            for x in range(file, file+filelength):
                row.append(" " + str(x))
        writer.writerow(row)

f.close()