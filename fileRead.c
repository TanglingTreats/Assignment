/*
 Program: fileRead.c
 Created on: 13 Feb 2020
 Edited on: 5 Mar 2020
 Created by: Edwin
*/

#include "fileRead.h"

bool readFile (FILE *file, char *comm, int **data, int *size)
{
    char buf[1024];
    char intBuf[3];

    bool setComm = false;
    bool setData = false;

    if(fgets(buf, 1024, file))
    {
        int i;
        int j = 0;
        int k = 0;

        for(i = 0; i < strlen(buf); i++)
        {
            if(buf[i] == ',')
            {
                if(!setComm)
                {
                    comm[j] = '\0';
                    setComm = true;
                    j = 0;

                    // Calculates length of remaining command
                    float remainLen = strlen(buf)-i-2; 
                    
                    int sizeOfData = ceil(remainLen / 5);

                    (*data) = (int *)calloc(sizeOfData, sizeof(int));
                    *size = sizeOfData;

                    setData = true;
                }

                i += 2;
            }

            if(!setComm)
            {
                comm[j] = buf[i];
                j++;
            }

            if(setData)
            {
                intBuf[j] = buf[i];
                j++;

                if(j == 3)
                {
                    (*data)[k] = atoi(intBuf);

                    j = 0;
                    k++; 
                }

            }
        }
        return true;
    }
    else
    {
        return false;
    }
}