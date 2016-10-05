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


int getNumOfVals()
{
    int num;
    int status = 0;

    status = scanf("%d", &num);
    if (status < 0) {
        perror("scanf");
    }

    return num;
}

void getVals(int numOfVals, int vals[])
{
    int status = 0;
    int i = 0;

    for (i = 0; i < numOfVals; i++) {
        status = scanf("%d", &vals[i]);
        if (status < 0) {
            perror("scanf");
        }
    }
}

int main (void)
{
    int numA = 0; // Number of elements in array a[]
    //int numX = 0; // Number of elements in array x[]
    //int numY = 0; // Number of elements in array y[]
    int i = 0;

    // get value of first number
    numA = getNumOfVals();
    // Gather all of the values
    int arrayA[numA];
    getVals(numA, arrayA);

    printf("Number of values: %d\n", numA);
    for (i = 0; i < numA; i++) {
        printf("%d ", arrayA[i]);
    }
    printf("\n");

    return 0;
}
