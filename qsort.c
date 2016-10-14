// -----------------------------------------------------------
// NAME : Zachary Hill                   User ID: zahill
// DUE DATE : 10/14/2016
// PROGRAM ASSIGNMENT #2
// FILE NAME : qsort.c
// PROGRAM PURPOSE :
//     Partitions a set of data a[Left...Right] around an
//     element a[M] and recursively does this
// -----------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
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
// FUNCTION swap :
//    swaps the location of the items at given indecies
// PARAMETER USAGE :
//    shmPtr - pointer to attached memory segment
//    indexA - index of element to swap
//    indexB - index of element to swap
// FUNCTION CALLED :
//    NONE
// -----------------------------------------------------------
void swap(int * shmPtr, int indexA, int indexB)
{
    int temp = shmPtr[indexA];
    shmPtr[indexA] = shmPtr[indexB];
    shmPtr[indexB] = temp;
}

// -----------------------------------------------------------
// FUNCTION partition :
//    Places all values less than the pivot on the left of
//    the pivot and all values greater than the pivot on
//    the right
// PARAMETER USAGE :
//    shmPtr - pointer to attached memory segment
//    left   - index to start the partition
//    right  - index to end the partition
// FUNCTION CALLED :
//    swap
// -----------------------------------------------------------
int partition(int * shmPtr, int left, int right)
{
    char buf[100];
    int k = 0;
    int pivot = shmPtr[right];
    sprintf(buf, "   ### Q-PROC(%d): pivot element is "
            "a[%d] = %d\n", getpid(), right, shmPtr[right]);
    printWrap(buf);
    int i = left;
    for (k = left; k < right; k++) {
        if (shmPtr[k] <= pivot) {
            swap(shmPtr, i, k);
            i = i + 1;
        }
    }
    swap(shmPtr, i, right);
    return i;
}

// -----------------------------------------------------------
// FUNCTION quicksort :
//    recursive quicksort algorithm that spawns a new process
//    to handle the left and right partitions
// PARAMETER USAGE :
//    shmPtr - pointer to attached memory segment
//    left   - index to start the quicksort
//    right  - index to end the quicksort
// FUNCTION CALLED :
//    quicksort
//    partition
//    fork functions
// -----------------------------------------------------------
void quicksort(int * shmPtr, int left, int right)
{
    int m;
    int i;
    char buf[100];
    pid_t pid;
    if (left < right) {
        m = partition(shmPtr, left, right);
        // fork the process for left sort
        pid = fork();
        if(pid == -1) {
            perror("fork");
            return;
        }
        else if(pid == 0) {
            // The child process
            sprintf(buf, "   ### Q-PROC(%d): entering with "
                "a[%d..%d]\n", getpid(), left, m - 1);
            printWrap(buf);
            quicksort(shmPtr, left, m - 1);
            sprintf(buf, "   ### Q-PROC(%d): exits\n", getpid());
            printWrap(buf);
            exit(0);
        }
        // fork the process for right sort
        pid = fork();
        if(pid == -1) {
            perror("fork");
            return;
        }
        else if(pid == 0) {
            // The child process
            sprintf(buf, "   ### Q-PROC(%d): entering with "
                "a[%d..%d]\n", getpid(), right, m + 1);
            printWrap(buf);
            quicksort(shmPtr, m + 1, right);
            sprintf(buf, "   ### Q-PROC(%d): exits\n", getpid());
            printWrap(buf);
            exit(0);
        }
        // wait for child processs
        int status = 0;
        for (i = 0; i < 2; i++) {
            wait(&status);
        }
    }
}

// -----------------------------------------------------------
// FUNCTION main :
//    Sorts data stored in shared memory using a recursive
//    quicksort algorithm that spawns new processes
// PARAMETER USAGE :
//    argc    - Not used
//    argv[1] - Key to shared main memory segment
//    argv[2] - index to start the sort
//    argv[3] - index to end the sort
//    argv[4] - size of the main memory segment
// FUNCTION CALLED :
//    quicksort
//    shm functions
// -----------------------------------------------------------
int main (int argc, char* argv[])
{

    (void) argc;                // surpress warning
    key_t mainShmKey = atoi(argv[1]);
    int left = atoi(argv[2]);
    int right = atoi(argv[3]);
    int totalMemSize = atoi(argv[4]);
    int shmID = 0;
    int * shmPtr;
    int * dataPtr = NULL;
    char buf[100];



    shmID = shmget(mainShmKey, (totalMemSize) * (sizeof(int)), (0666));
    shmPtr = (int *) shmat(shmID, dataPtr, 0);
    sprintf(buf, "   ### Q-PROC(%d): main shared memory atached and "
            "is ready to use\n", getpid());
    printWrap(buf);

    quicksort(shmPtr, left, right);

    shmdt(shmPtr);

    return 0;
}
