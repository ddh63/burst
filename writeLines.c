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

int keepGoing = 1;

struct threaddata_t {
  int id;
  int outfd;
  char* dataToWrite;
  int status;
  pthread_t tid;
};

void* process_thread(void* args) {
  struct threaddata_t* data = args;

  ssize_t byteswritten;
  while (((byteswritten = write(data->outfd, data->dataToWrite, strlen(data->dataToWrite)) == -1)) && (errno == EINTR));

  if (byteswritten == -1) {
    perror("Output write error");
    exit(1);
  }

  return &(data->status);
}

char* getLineInput(struct archive* infd) {
  int maxLen = 16;
  int currLen = 0;
  char* buf = malloc(maxLen);
  char t = 0;
  int bytesread;
  int i;
  for (i = 0; i < 500; ++i) {
    while ((bytesread = archive_read_data(infd, &t, 1)) == 1 && t != '\n') {
      if (currLen + 2 >= maxLen) {
        int newLen = maxLen * 2;
        char* newBuf = realloc(buf, newLen);
	if (newBuf == 0) {
          free(buf);
          return NULL;
        }
        buf = newBuf;
        maxLen = newLen;
      }
      buf[currLen++] = t;
    }
    if (t == '\n' && bytesread != 0)
      buf[currLen++] = t;
    // EOF
    if (bytesread == 0) {
      keepGoing = 0;
      break;
    }
  }
  buf[currLen] = '\0';
  return buf;
}

int writeLines(char* filename, struct archive* infd, int filenum) {
  char* point;
  char file[100];

  // gets point where . is to get filename and  full file extension
  point = strchr(filename, '.');

  // adds filename to file
  strncpy(file, filename, point-filename);
  file[point-filename] = '\0';

  // puts file extension into a variable
  char ext[20];
  strcpy(ext, &filename[(int)(point-filename)]);

  // puts filename all together and increments count for next file
  sprintf(file, "%s-%d%s", file, filenum, ext);

  int outfd = open(file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  if (outfd < 0) {
    perror("Output open error");
    return -1;
  }

  char* line;
  if (((line = getLineInput(infd)) != NULL) && strlen(line) != 0) {

    struct threaddata_t threadinfo;

    // create thread
    threadinfo.id = 0;
    threadinfo.dataToWrite = line;
    threadinfo.outfd = outfd;
    pthread_create(&threadinfo.tid, NULL, process_thread, &threadinfo);

    // wait for thread to finish
    pthread_join(threadinfo.tid, NULL);

    free(line);
  }
  else {
    keepGoing = 0;
  }

  close(outfd);
  return keepGoing;
}