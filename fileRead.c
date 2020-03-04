/*
 Program: fileRead.c
 Created on: 13 Feb 2020
 Edited on: 13 Feb 2020
 Created by: Edwin
*/

#include "fileRead.h"

bool readFile (char* file)
{
    //counter for getting number of lines
    int counter = 0;

    FILE* fp = fopen(file, "r");

    char oldBuf[1024];
    char buf[1024];

    Op_store comm;

    comm.command = (char*)calloc(7, sizeof(char));

    if(fp == NULL)
    {
        printf("ERROR - Failed to open file!\n");

        return false;
    }
    else
    {
        while (fgets(buf, 1024, fp))
        {
            printf("%i\n", counter);
            //strcpy(comm.command[counter], buf);
            //memcpy((char*)comm.command[counter], buf, strlen(buf)+1);
            comm.command = buf;
            counter++;
            printf("%s\n",comm.command); 
            char* oldBuffer = realloc(comm.command, ((counter+1) * sizeof(char)));

            if(!oldBuffer)
            {
                printf("Failed to reallocate!\n");
                return false;
            }
            else
            {
                comm.command = oldBuffer;
            }
            
        }
        // printf("%i\n", counter);
        // fgets(buf, 1024, fp);
        // while(strcmp(buf, oldBuf) != 0)
        // {
        //     counter++;
        //     strcpy(oldBuf, buf);
        //     printf("%s\n",buf);



        //     fgets(buf, 1024, fp);
        // }
    
  
    fclose(fp);
    }

    return true;
}