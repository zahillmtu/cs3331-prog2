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
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


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

/**
 * Function to fork a process and run the execvp command. exec should be set
 * to the name of the executable and args should contain an array where arg[0]
 * is the name of the exec and arg[1 - 31] are the arguments passed
 */
void run(char *exec, char *args[]) {

    // fork the process
    pid_t pid = fork();
    if(pid == -1) {
        perror("fork");
        return;
    }
    else if(pid == 0) {
        // The child process
        // Call execvp
        if (execvp(exec, args) < 0) {
            printf("An error occured running the program %s - Try again\n", exec);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else {
        // wait for the child
        int status = 0;
        wait(&status);
        return;
    }
    return;
}

void qsortChild(key_t shmID, int arraySize)
{
    char *exec = "./qsort";
    char *args[5] = { NULL };
    char buf[100];

    sprintf(buf, "*** MAIN: about to spawn the process for qsort\n");
    printWrap(buf);

    args[0] = exec;
    sprintf(buf, "%d", shmID);
    args[1] = strdup(buf);
    sprintf(buf, "%d", 0);
    args[2] = strdup(buf);
    sprintf(buf, "%d", arraySize);
    args[3] = strdup(buf);

    run(exec, args);
}

void mergeChild(key_t shmID, int startInd, int arrXsize, int arrYsize)
{
    char *exec = "./merge";
    char *args[7] = { NULL };
    char buf[100];

    sprintf(buf, "*** MAIN: about to spawn the process for merge\n");
    printWrap(buf);

    args[0] = exec;
    sprintf(buf, "%d", shmID);
    args[1] = strdup(buf);
    sprintf(buf, "%d", startInd);
    args[2] = strdup(buf);
    sprintf(buf, "%d", arrXsize);
    args[3] = strdup(buf);
    sprintf(buf, "%d", arrYsize);
    args[4] = strdup(buf);

    run(exec, args);
}

int main (void)
{
    char buf[100];
    int numA = 0; // Number of elements in array a[]
    int numX = 0; // Number of elements in array x[]
    int numY = 0; // Number of elements in array y[]
    int total = 0; // total number of elements in sharedmem
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

    total = numA + numX + numY;

    sprintf(buf, "*** MAIN: shared memory key = %d\n", shmKey);
    printWrap(buf);
    // create the shared memory
    shmID = shmget(shmKey, (numA + numX + numY) * (sizeof(int)), (IPC_CREAT | 0666));
    sprintf(buf, "*** MAIN: shared memory created\n");
    printWrap(buf);
    shmPtr = (int *) shmat(shmID, dataPrt, 0);
    sprintf(buf, "*** MAIN: shared memory attached and is ready to use\n");
    printWrap(buf);

    sprintf(buf, "\n");
    printWrap(buf);
    sprintf(buf, "Input array for qsort has %d elements\n", numA);
    printWrap(buf);
    for (i = 0; i < numA; i++) {
        sprintf(buf, "%4d", arrayA[i]);
        printWrap(buf);
    }
    sprintf(buf, "\n");
    printWrap(buf);
    sprintf(buf, "Input array for qsort has %d elements\n", numX);
    printWrap(buf);
    for (i = 0; i < numX; i++) {
        sprintf(buf, "%4d", arrayX[i]);
        printWrap(buf);
    }
    sprintf(buf, "\n");
    printWrap(buf);
    sprintf(buf, "Input array for qsort has %d elements\n", numY);
    printWrap(buf);
    for (i = 0; i < numY; i++) {
        sprintf(buf, "%4d", arrayY[i]);
        printWrap(buf);
    }
    sprintf(buf, "\n");
    printWrap(buf);
    sprintf(buf, "\n");
    printWrap(buf);

    // Load all of the values into shared memory
    loadData(shmPtr, 0, numA, arrayA);
    loadData(shmPtr, numA, numX + numA, arrayX);
    loadData(shmPtr, numX + numA, numY + numA + numX, arrayY);

    // detach from the shared memory
    shmdt(shmPtr);
    sprintf(buf, "*** MAIN: shared memory successfully detached\n");
    printWrap(buf);

    // Print out information about data


    // Create qsort child and run it
    qsortChild(shmKey, numA);


    // Create merge child and runt it
    mergeChild(shmKey, numA, numX, numY);

    // wait for both children to run
    int status;
    for(i = 0; i < 2; i++) {
        wait(&status);
    }

    shmPtr = (int *) shmat(shmID, dataPrt, 0);
    sprintf(buf, "*** MAIN: shared memory attached and is ready to use\n");
    printWrap(buf);

    sprintf(buf, "*** MAIN: merged array:\n");
    printWrap(buf);
    sprintf(buf, "  ");
    printWrap(buf);
    for (i = numA; i < total; i++) {
        sprintf(buf, "%4d", shmPtr[i]);
        printWrap(buf);
    }
    printf("\n");

    shmdt(shmPtr);
    sprintf(buf, "*** MAIN: shared memory successfully detached\n");
    printWrap(buf);
    // Close out the shared memory
    shmctl(shmID, IPC_RMID, NULL);
    sprintf(buf, "*** MAIN: shared memory successfully removed\n");
    printWrap(buf);

    return 0;
}
