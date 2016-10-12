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

int main (int argc, char* argv[])
{
    int mainShmKey = 0;
    int mainStartInd = 0;
    int arrXsize = 0;
    int arrYsize = 0;

    key_t mergeShmKey;
    int mergeShmID = 0;
    int * mergeShmPtr;
    char * mergeDataPtr = NULL;
    int status;



    printf("MERGE WAS CALLED!\n");

    printf("argc: %d\n", argc);
    printf("arg1: %s\n", argv[1]);
    printf("arg2: %s\n", argv[2]);
    printf("arg3: %s\n", argv[3]);
    printf("arg4: %s\n", argv[4]);

    mainShmKey = atoi(argv[1]);
    mainStartInd = atoi(argv[2]);
    arrXsize = atoi(argv[3]);
    arrYsize = atoi(argv[4]);


    // Get a shared memory key for merge memory
    mergeShmKey = ftok("./", 'm');

    // Create shared memory
    mergeShmID = shmget(mergeShmKey, (arrXsize + arrYsize) * (sizeof(int)),
                   (IPC_CREAT | 0666));
    mergeShmPtr = (int *) shmat(mergeShmID, mergeDataPtr, 0);

    // Fork all the needed processes
    int i = 0;
    for (i = 0; i < arrXsize; i++) {
        // fork all x processes
    }
    for (i = 0; i < arrXsize; i++) {
        // fork all y processes
    }

    // Wait for all the processes
    for (i = 0; i < (arrXsize + arrYsize); i++) {
        //wait(&status);
    }




    return 0;
}
