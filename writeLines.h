#ifndef WRITELINES_H
#define WRITELINES_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <archive.h>
#include <archive_entry.h>

extern int keepGoing;

struct threaddata_t {
  int id;
  int outfd;
  char* dataToWrite;
  int status;
  pthread_t tid;
};

void* process_thread(void* args);

char* getLineInput(struct archive* infd, int lines);

int writeLines(char* filename, struct archive* infd, int filenum, int lines);

#endif
