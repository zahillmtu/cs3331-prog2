// -----------------------------------------------------------
// NAME : Zachary Hill                   User ID: zahill
// DUE DATE : 10/14/2016
// PROGRAM ASSIGNMENT #2
// FILE NAME : main.c
// PROGRAM PURPOSE :
//     Prints information related to the problem as well as
//     calling the qsort and merge programs to complete
//     the problem
// -----------------------------------------------------------
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main (void)
{
    char input[1];
    int num;
    int status = 0;


    status = read(STDIN_FILENO, input, sizeof(char));

    num = atoi(input);

    printf("Number: %d\n", num);

    return 0;
}
