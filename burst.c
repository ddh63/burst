/*
  burst.c

  reads in txt file and separates them into 500 line segments
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BLOCK 500

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
	
	char buf[BLOCK];
	while (1) {
		int bytesread = read(infd, buf, BLOCK);
		fprintf(stderr, "\nBytes: %d\n", bytesread);
		if ((bytesread == -1) && (errno == EINTR))
			continue;

		if (bytesread == 0)
			break;

		write(STDOUT_FILENO, buf, bytesread);
	}

	close(infd);
	return 0;
}
