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
    char *intBuf;

    bool setComm = false;
    bool setData = false;
    
    int intOfData = 0;

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
                    
                    intOfData = 0;
                    char *input = &buf[i+2];

                    while(input[intOfData] >= '0' && input[intOfData] <= '9')
                    {
                    //    printf("This is an integer character %c\n", input[intOfData]);
                        intOfData++;
                    }

                    int sizeOfData = ceil(remainLen / (intOfData + 2));
                    
                    //printf("\nSize of data: %i\n", sizeOfData);

                    intBuf = (char *)calloc(intOfData, sizeof(char));
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
                if(buf[i] >= '0' && buf[i] <= '9')
                {
                    intBuf[j] = buf[i];
                    //printf("Buffer: %c\n", buf[i]);
                    //printf("j: %i\n", j);
                    j++;

                    if(j == intOfData)
                    {
                        (*data)[k] = atoi(intBuf);

                     //   printf("Converted data: %i\n", (*data)[k]);
                        j = 0;
                        k++; 
                    }
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
