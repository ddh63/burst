/*
  burst.c

  reads in txt file and separates them into 500 line segments
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

char* getLineInput(int infd) {
	int maxLen = 16;
  int currLen = 0;
	char* buf = malloc(maxLen);
  char t = 0;
	int bytesread;
  while ((bytesread = read(infd, &t, 1)) == 1 && t != '\n') {
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
  if (t == '\n')
		buf[currLen++] = t;
	buf[currLen] = '\0';
	return buf;
}

int writeLines(char* filename, int infd, int filenum) {
	// what will be returned if no error occurs
	// 1 means EOF hasn't been reached
	// 0 means EOF has been reached
	int keepGoing = 1;

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
  printf("%s\n", file);

  int outfd = open(file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
  if (outfd < 0) {
    perror("Output open error");
    return -1;
  }

	char* line;
  int i;
  for (i = 0; i < 500; ++i) {
    if (((line = getLineInput(infd)) != NULL) && strlen(line) != 0) {
			ssize_t byteswritten;
			while (((byteswritten = write(outfd, line, strlen(line)) == -1)) && (errno == EINTR));

			if (byteswritten == -1) {
				perror("Output write error");
				return -1;
			}

      free(line);
    }
    else {
			keepGoing = 0;
      break;
    }
	}

  close(outfd);
	return keepGoing;
}

int main(int argc, char* argv[]) {  
	if (argc < 2) {
		fprintf(stderr, "usage: burst <input file>\n");
		return 1;
	}

	int infd = open(argv[1], O_RDONLY);
  if (infd < 0) {
    perror("Input open error");
    return 1;
  }
	
	// used to get output filename
	int count = 1;
	while (1) {
		int result = writeLines(argv[1], infd, count);
		if (result == -1) {
			fprintf(stderr, "Error in writeLines function\n");
			return 1;
		}
		if (!result)
			break;
		count++;
	}

	close(infd);

	return 0;
}
