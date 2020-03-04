////// ***DUE 22/3/2020 23:59*** ///////
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int main(){ 
    FILE *fp = fopen("CSC1007-SampleCSV.csv","r");
    char buf[1024];

    if (!fp) {
        printf("cant open file");
        return 1;
    }

    while (fgets(buf, 1024, fp)){
        printf("%s\n",buf); 
    }
    printf("\n%s",buf);
    
  
    fclose(fp);
}


























