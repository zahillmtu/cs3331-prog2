// -----------------------------------------------------------
// NAME : Zachary Hill                   User ID: zahill
// DUE DATE : 10/14/2016
// PROGRAM ASSIGNMENT #2
// FILE NAME : merge.c
// PROGRAM PURPOSE :
//     Creates child processes to carry out a binary search
//     in order to merge previously partitioned data together
// -----------------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// -----------------------------------------------------------
// FUNCTION printWrap :
//    A wrapper method for printing using write()
// PARAMETER USAGE :
//    buf - A character array of size 100 containing
//          the print statement
// FUNCTION CALLED :
//    write()
// -----------------------------------------------------------
void printWrap(char buf[100]) {
    write(1, buf, strlen(buf));
}

int main (int argc, char* argv[])
{
    printf("MERGE WAS CALLED!\n");

    printf("argc: %d\n", argc);
    printf("arg1: %s\n", argv[1]);
    printf("arg2: %s\n", argv[2]);
    printf("arg3: %s\n", argv[3]);
    printf("arg4: %s\n", argv[4]);

    return 0;
}
