EXE=burst
OBJS=burst.o

CC=gcc
CCOPTS=-Wall

all : ${EXE}

burst : burst.o
	${CC} $^ -o $@

burst.o : burst.c
	${CC} ${CCOPTS} -c $<

clean : 
	rm -f ${EXE} ${OBJS} ${LIBS}
