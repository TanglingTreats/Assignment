#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define TotalBlock 130

void allocate();
void deallocate();
void display();
int BlockFull();

struct indexed
{
    char name[30];
    int length;
    int indexblock;
    int*IndexBlock[TotalBlock];
}F[30];

int Table[TotalBlock+1], pos = 0, r, i, j, indexblock, k=0;
int choice, B = 0;
char fn[30];

int main()
{
    printf("\n Indexed File Allocation");
    do
    {
        printf("\n\n1. Allocate\n2. Deallocate\n3. Display\n4. Exit");
        printf("\n\nEnter your choice: ");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1: //File allocation
                pos++;
                allocate();
                break;
            case 2: //File deallocation
                k--;
                deallocate();
                break;
            case 3: //Display the selcted file indexblock and fileblocks
                display();
                break;
            case 4: //Exit the program
                exit(0);
            default:
                printf("\nInvalid Choice!");
        }
    } while (choice != 4);    

    return 0;
}

void allocate()
{   
    // To simulate the reading of the file name
    printf("\nEnter the file name: ");
    scanf("%s", &(F[pos].name));
    // To simulate the reading of the file length
    printf("\nEnter the file length: ");
    scanf("%d", &(F[pos].length));

    //Error message if the there is not enough blocks to allocate the file
    if(BlockFull())
    {
        pos--;
        printf("\n\nNot enough free space available \n");
        return;
    }

    //Allocation of the blocks
    F[pos].indexblock = rand() % TotalBlock+1;
    Table[F[pos].indexblock] = 1;

    for(i = 1; i <= F[pos].length; i++)
    {
        while(1)
        {
            r = rand()%TotalBlock+1;
            if (Table[r] == 0);
            {
                F[pos].IndexBlock[i] =& Table[r];
                Table[r] = r;
                break;
            }
        }
    }

    //Display the filename the respective Indexed Blocks
    printf("\n\tDirectory\n");
    printf("\nFileName\tIndexBlock\n");
    for(i = 1; i <= pos; i++)
    {
        printf("\n%s\t\t%d", F[i].name, F[i].indexblock);
        printf("\n");
    }
}

void deallocate()
{
   printf("\nEnter The File Name : ");
   scanf("%s",&fn);

   //Deallocation of the blocks
   for(i = 1; i <= pos; i++)
   {
            if(strcmp(F[i].name, fn) == 0)
            {
                for(j = 1; j <= F[pos].length;  j++)
                   *F[pos].IndexBlock[j] = 0;
                Table[F[pos].indexblock] = 0;
                strcpy(F[i].name,"NULL");
                F[i].indexblock = 0;
                F[i].length = 0;
                printf("\nFile (%s) Deleted Successfully \n", fn);
                break;
            }
            else
                printf("\nDeletion Unsuccessful\n");
    }

     //Display the filename the respective Indexed Blocks
    printf("\n\tDirectory\n");
    printf("\nFileName\tIndexBlock\n");
    for(i = 1;   i <= pos; i++)
    {
        printf("\n%s\t\t%d", F[i].name, F[pos].indexblock);
        printf("\n");
    }
}

void display()
{
    printf("\nEnter The File Name : ");
    scanf("%s", fn);
    printf("\nBlocks Allocated Are : \n");

    //Create a table that shows the selected file indexed block and the allocated blocks
    for( i = 1; i <= pos; i++)
    {
        if(strcmp(F[i].name, fn) == 0)
        {
           printf("\n\t IndexBlock\n");
            printf("\n\t     %d\n",F[pos].indexblock);
            printf("\t------------");
            for(j=1;j<=F[pos].length;j++)
            {
                printf("\n\t|    %d\t   |\n",*F[pos].IndexBlock[j]);
            }
            printf("\t------------");

            break;
        }
    }

    // Error message if there is no corrosponding file
    if(i == pos + 1)
    {
        printf("\n\nNo File Found\n");
    }
}

// Error message if there is no available blocks left to allocate
int BlockFull()
{
    for( i = 1;i <= pos; i++)
        B += F[i].length;
    if(B > TotalBlock - pos - k)
        return 1;
    else
        return 0;
}