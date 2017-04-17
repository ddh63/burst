/*
  burst.c

  reads in txt file and separates them into 500 line segments
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

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

	fprintf(stderr, "filedes: %d\n", infd);

	close(infd);
  return 0;
}
