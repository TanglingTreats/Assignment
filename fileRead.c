/*
 Program: fileRead.c
 Created on: 13 Feb 2020
 Edited on: 13 Feb 2020
 Created by: Edwin
*/

#include "fileRead.h"

bool readFile (FILE* file, char* comm, int** data)
{
    //counter for getting number of lines
    int counter = 0;

    char buf[1024];
    char intBuf[3];

    bool setComm = false;
    bool setData = false;

    if(fgets(buf, 1024, file))
    {
        int i;
        int j = 0;
        printf("This is one command: %s\n", buf);
        for(i = 0; i < strlen(buf); i++)
        {
            if(buf[i] == ',')
            {
                if(!setComm)
                {
                    comm[j] = '\0';
                    //printf("The command is: %s\n", comm);
                    setComm = true;
                    j = 0;

                    *data = (int *)calloc(strlen(buf)-i / 5, sizeof(int));
                    break;
                }
                i += 2;
            }
            if(!setComm)
            {
                comm[j] = buf[i];
                j++;
            }
        }
        return true;
    }
    else
    {
        return false;
    }

    
}