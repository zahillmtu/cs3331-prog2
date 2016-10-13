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
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

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

int recurseBinary (int value, int startInd, int endInd, int * mainShmPtr)
{
    int length = endInd - startInd + 1;
    // When you get to size one, if it is smaller than it goes
    // at this index, larger go one to the right
    if (length == 1 && mainShmPtr[startInd] < value) {
        return startInd + 1;
    }
    else if (length == 1 && mainShmPtr[startInd] > value) {
        return startInd;
    }
    // continue dividing size until length = 1
    else {
        if (value > mainShmPtr[(length / 2) + startInd]) {
            return recurseBinary(value, (length / 2) + startInd,
                                 endInd, mainShmPtr);
        }
        else {
            return recurseBinary(value, startInd, (length / 2) - 1 + startInd, mainShmPtr);
        }
    }
}

int main (int argc, char* argv[])
{
    int mainShmKey = 0;
    int mainShmID = 0;
    int mainStartInd = 0;
    int arrXsize = 0;
    int arrYsize = 0;
    int * mainShmPtr;
    char * dataPtr = NULL;
    (void) argc;    // surpress warning

    key_t mergeShmKey;
    int mergeShmID = 0;
    int * mergeShmPtr;
    char * mergeDataPtr = NULL;
    int status;
    int mergeInd;               // index of where to store value in merge shm

    pid_t pid;

    mainShmKey = atoi(argv[1]);
    mainStartInd = atoi(argv[2]);
    arrXsize = atoi(argv[3]);
    arrYsize = atoi(argv[4]);

    // Get a shared memory key for merge memory
    mergeShmKey = ftok("./", 'm');

    // Create shared memory for merged section
    mergeShmID = shmget(mergeShmKey, (arrXsize + arrYsize) * (sizeof(int)),
                   (IPC_CREAT | 0666));

    // find shared memory from main program
    mainShmID = shmget(mainShmKey,
                       (mainStartInd + arrXsize + arrYsize) * sizeof(int),
                       (066));


    // Fork all the needed processes
    int i = 0;
    int index = 0;
    for (i = 0; i < arrXsize; i++) {
        // fork all x processes
        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // attach to the main memory
            mainShmPtr = (int *) shmat(mainShmID, dataPtr, 0);

            // attach to the merge memory
            mergeShmPtr = (int *) shmat(mergeShmID, mergeDataPtr, 0);

            // use binary search to find index, subtract off extra from
            // arrayA in front in the main shared memory
            mergeInd = recurseBinary(mainShmPtr[i + mainStartInd],
                                     mainStartInd + arrXsize,
                                     mainStartInd + arrXsize + arrYsize - 1,
                                     mainShmPtr) - mainStartInd;

           index = mergeInd - (arrXsize) + i;
           mergeShmPtr[index] = mainShmPtr[i + mainStartInd];

           // detach from memory
           shmdt(mainShmPtr);
           shmdt(mergeShmPtr);

           // exit child process
           exit(0);
        }
    }
    for (i = 0; i < arrYsize; i++) {
        // fork all y processes
        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // attach to the main memory
            mainShmPtr = (int *) shmat(mainShmID, dataPtr, 0);

            // attach to the merge memory
            mergeShmPtr = (int *) shmat(mergeShmID, mergeDataPtr, 0);

            // use binary search to find index, subtract off extra from
            // arrayA in front in the main shared memory
            mergeInd = recurseBinary(mainShmPtr[i + mainStartInd + arrXsize],
                                     mainStartInd,
                                     mainStartInd + arrXsize,
                                     mainShmPtr) - mainStartInd;

            index = mergeInd + i;
            if (index > arrXsize + arrYsize - 2) {
                index = index - 1;
            }
            mergeShmPtr[index] = mainShmPtr[i + mainStartInd + arrXsize];

            // detach from memory
            shmdt(mainShmPtr);
            shmdt(mergeShmPtr);

            // exit child process
            exit(0);
        }
    }

    // Wait for all the processes
    for (i = 0; i < (arrXsize + arrYsize); i++) {
        wait(&status);
    }

    // Put values back into main shared memory location
    mergeShmPtr = (int *) shmat(mergeShmID, mergeDataPtr, 0);
    mainShmPtr = (int *) shmat(mainShmID, dataPtr, 0);
    int k = mainStartInd;
    for (i = 0; i < arrXsize + arrYsize; i++) {
        mainShmPtr[k] = mergeShmPtr[i];
        k++;
    }

    shmdt(mainShmPtr);
    shmdt(mergeShmPtr);
    shmctl(mergeShmID, IPC_RMID, NULL);

    return 0;
}
