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

// -----------------------------------------------------------
// FUNCTION getNumOfVals :
//    Returns the integer passed into stdin giving th
//    size of the next array of numbers
// PARAMETER USAGE :
//    N/A
// FUNCTION CALLED :
//    scanf()
// -----------------------------------------------------------
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

// -----------------------------------------------------------
// FUNCTION getVals :
//    Returns from stdin an array of integers
// PARAMETER USAGE :
//    numOfVals - size of the array
//    vals[]    - the array to store the vales
// FUNCTION CALLED :
//    scanf()
// -----------------------------------------------------------
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

// -----------------------------------------------------------
// FUNCTION loadData :
//    Used to load an array into shared memory
// PARAMETER USAGE :
//    shmPtr - pointer to a shared memory location
//              MUST already be attached!
//    startInd - starting index for loading the array
//    endInd   - ending index for loading the array
//    array[]  - array to copy data from into shared memory
// FUNCTION CALLED :
//    None
// -----------------------------------------------------------
void loadData(int * shmPtr, int startInd, int endInd, int array[])
{
    int i = 0;
    int k = 0;
    for (i = startInd; i < endInd; i++) {
        shmPtr[i] = array[k];
        k++;
    }
}

/**
 * Function to fork a process and run the execvp command. exec should be set
 * to the name of the executable and args should contain an array where arg[0]
 * is the name of the exec and arg[1 - 31] are the arguments passed
 */
// -----------------------------------------------------------
// FUNCTION loadData :
//    Function to fork a process and run the execvp command. exec should be set
//    to the name of the executable and args should contain an array where arg[0]
//    is the name of the exec and arg[1 - 31] are the arguments passed
// PARAMETER USAGE :
//    exec - name of program to run
//    args[] - list of arguments passed to the program
//             in typical unix standard
// FUNCTION CALLED :
//    execvp
// -----------------------------------------------------------
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
    return;
}

// -----------------------------------------------------------
// FUNCTION qsortChild :
//    Calls the qsort program with passed in arguments
// PARAMETER USAGE :
//    shmID - ID of the main shared memory segment
//    arraySize - size of elements in array a[]
//    totalSize - total size of main shared memory
// FUNCTION CALLED :
//    run()
// -----------------------------------------------------------
void qsortChild(key_t shmID, int arraySize, int totalSize)
{
    char *exec = "./qsort";
    char *args[6] = { NULL };
    char buf[100];

    sprintf(buf, "*** MAIN: about to spawn the process for qsort\n");
    printWrap(buf);

    args[0] = exec;
    sprintf(buf, "%d", shmID);
    args[1] = strdup(buf);
    sprintf(buf, "%d", 0);
    args[2] = strdup(buf);
    sprintf(buf, "%d", arraySize - 1);
    args[3] = strdup(buf);
    sprintf(buf, "%d", totalSize);
    args[4] = strdup(buf);

    run(exec, args);
}

// -----------------------------------------------------------
// FUNCTION mergeChild :
//    Calls the merge program with specified arguments
// PARAMETER USAGE :
//    shmID - ID of the main shared memory segment
//    startInd - beginning of arrays in shared memory
//    arrXsize - the size of array x[]
//    arrYsize - the size of array y[]
// FUNCTION CALLED :
//    run()
// -----------------------------------------------------------
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

// -----------------------------------------------------------
// FUNCTION main :
//    Prints information about running the program and shared
//    memory. Pulls the data from stdin and loads the arrays
//    into shared memory. Then calls qsort and merge to handle
//    the data.
// PARAMETER USAGE :
//    NONE
// FUNCTION CALLED :
//    shm functions
//    getVals
//    loadData
//    qsortChild
//    mergeChild
// -----------------------------------------------------------
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

    // Create qsort child and run it
    qsortChild(shmKey, numA, total);

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

    sprintf(buf, "*** MAIN: qsort array:\n");
    printWrap(buf);
    sprintf(buf, "  ");
    printWrap(buf);
    for (i = 0; i < numA; i++) {
        sprintf(buf, "%4d", shmPtr[i]);
        printWrap(buf);
    }
    printf("\n");

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
