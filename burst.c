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
	// used to get output filename
	int count = 1;

	while (1) {
		int bytesread = read(infd, buf, BLOCK);
		fprintf(stderr, "\nBytes: %d\n", bytesread);
		if ((bytesread == -1) && (errno == EINTR))
			continue;

		if (bytesread == 0)
			break;
		
		char* point;
		char file[100];
		
		/* gets point where . is to get filename and  full file extension */
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

		write(outfd, buf, bytesread);

		close(outfd);
	}

	close(infd);

	return 0;
}
