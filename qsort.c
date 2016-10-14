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

void swap(int * shmPtr, int indexA, int indexB)
{
    printf("swapping: %d and %d\n",shmPtr[indexA], shmPtr[indexB]);
    int temp = shmPtr[indexA];
    shmPtr[indexA] = shmPtr[indexB];
    shmPtr[indexB] = temp;
}

int partition(int * shmPtr, int left, int right)
{
    int k = 0;
    int pivot = shmPtr[right];
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

void quicksort(int * shmPtr, int left, int right)
{
    int m;
    int i;
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
            quicksort(shmPtr, left, m - 1);
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
            quicksort(shmPtr, m + 1, right);
            exit(0);
        }
        // wait for child processs
        int status = 0;
        for (i = 0; i < 2; i++) {
            wait(&status);
        }
    }
}

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
    sprintf(buf, "  ### PROC(%d): main shared memory atached and "
            "is ready to use\n", getpid());
    printWrap(buf);

    quicksort(shmPtr, left, right);

    shmdt(shmPtr);

    return 0;
}
