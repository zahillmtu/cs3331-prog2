CC = gcc
CCFLAGS = -g -Wall -Wextra -lreadline

all: main.c qsort.c merge.c
	${CC} ${CCFLAGS} main.c -o main
	${CC} ${CCFLAGS} qsort.c -o qsort
	${CC} ${CCFLAGS} merge.c -o merge

noflags: main.c qsort.c merge.c
	${CC} main.c -o main
	${CC} qsort.c -o qsort
	${CC} merge.c -o merge

clean:
	rm -f main qsort merge
