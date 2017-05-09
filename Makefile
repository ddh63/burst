EXE=burst
OBJS=burst.o

CC=gcc
CCOPTS=-Wall

all : ${EXE}

burst : burst.o writeLines.o
	${CC} $^ -lpthread -larchive -o $@

burst.o : burst.c
	${CC} ${CCOPTS} -c $<

writeLines.o : writeLines.c writeLines.h
	${CC} ${CCOPTS} -c $<

clean : 
	rm -f ${EXE} ${OBJS} ${LIBS}
