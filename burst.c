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

#define BLOCK 500

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
    if ((line = getLineInput(infd)) != NULL) {
      ssize_t byteswritten;
			while (((byteswritten = write(outfd, line, strlen(line)) == -1)) && (errno == EINTR));

			if (byteswritten == -1) {
				perror("Output write error");
				return -1;
			}
      free(line);
    }
    else {
      break;
    }
  }

  close(outfd);
	return 0;
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
	int result = writeLines(argv[1], infd, count);
	if (result != 0) {
		fprintf(stderr, "Error in writeLines function\n");
	}

/*		char* point;
		char file[100];
		
		// gets point where . is to get filename and  full file extension 
		point = strchr(argv[1], '.');

		// adds filename to file
		strncpy(file, argv[1], point-argv[1]);
		file[point-argv[1]] = '\0';

		// puts file extension into a variable
		char ext[20];
		strcpy(ext, &argv[1][(int)(point-argv[1])]);

		// puts filename all together and increments count for next file
		sprintf(file, "%s-%d%s", file, count, ext);
		printf("%s\n", file);
		count++;

		int outfd = open(file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		if (outfd < 0) {
			perror("Output open error");
			return 1;
		}

		ssize_t byteswritten;
		while (((byteswritten = write(outfd, buf, bytesread)) == -1) && (errno == EINTR));
		
		if (byteswritten == -1) {
			perror("Output write error");
			return 1;
		}
		
		close(outfd);
*/
	close(infd);

	return 0;
}
