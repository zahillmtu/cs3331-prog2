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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


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

void loadData(int * shmPrt, int startInd, int endInd, int array[])
{
    int i = 0;
    int k = 0;
    for (i = startInd; i < endInd; i++) {
        shmPrt[i] = array[k];
        k++;
    }
}

int main (void)
{
    int numA = 0; // Number of elements in array a[]
    int numX = 0; // Number of elements in array x[]
    int numY = 0; // Number of elements in array y[]
    int shmID = 0;
    int * shmPtr;
    char * dataPrt = NULL;

    int i = 0;

    // get shared memory key
    key_t shmKey = ftok("./", 'z');

    // get value of first number
    numA = getNumOfVals();
    // Gather all of the values
    int arrayA[numA];
    getVals(numA, arrayA);

    // get value of second number
    numX = getNumOfVals();
    // Gather all of the values
    int arrayX[numX];
    getVals(numX, arrayX);

    // get value of third number
    numY = getNumOfVals();
    // Gather all of the values
    int arrayY[numY];
    getVals(numY, arrayY);

    // Load all of the values into shared memory
    shmID = shmget(shmKey, (numA + numX + numY) * (sizeof(int)), (IPC_CREAT | 0666));
    shmPtr = (int *) shmat(shmID, dataPrt, 0);
    loadData(shmPtr, 0, numA, arrayA);
    loadData(shmPtr, numA, numX, arrayX);
    loadData(shmPtr, numX, numY, arrayY);

    // detach from the shared memory
    shmdt(shmPtr);

    printf("Number of values for a: %d\n", numA);
    for (i = 0; i < numA; i++) {
        printf("%d ", arrayA[i]);
    }
    printf("\n");
    printf("Number of values for x: %d\n", numX);
    for (i = 0; i < numX; i++) {
        printf("%d ", arrayX[i]);
    }
    printf("\n");
    printf("Number of values for y: %d\n", numY);
    for (i = 0; i < numY; i++) {
        printf("%d ", arrayY[i]);
    }
    printf("\n");

    // Close out the shared memory
    shmctl(shmID, IPC_RMID, NULL);

    return 0;
}
