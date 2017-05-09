/*
  burst.c

  reads in txt file and separates them into 500 line segments
*/

#include "writeLines.h"

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

void getHelp() {
	printf("To use this program add a filename to the command line\nOptional -l # to set lines per file (500 lines when not set)\n");
}

int main(int argc, char* argv[]) {  
	if (argc < 2) {
		fprintf(stderr, "usage: burst <input file>\n");
		return 1;
	}

	int argvFileLoc = 0;
	int lines = 500;

	// get options from command line
	int argcCounter;
	for (argcCounter = 1; argcCounter < argc; ++argcCounter) {
		
		char letter = argv[argcCounter][0];
		// user option
		if (letter == '-') {	
			letter = argv[argcCounter][1];
			
			// helper
			if (letter == 'h') {
				getHelp();
				return 0;
			}
			// number of lines per file
			else if (letter == 'l') {
				++argcCounter;
				lines = atoi(argv[argcCounter]);
			}
			// error because no option selected after '-'
			else {
				fprintf(stderr, "Must have an option after '-'\n");
				return 1;
			}
		}
		// filename
		// assumes an option without a '-' at the beginning is a filename
		else {
			argvFileLoc = argcCounter;
		}

	}

	if (argvFileLoc == 0) {
		fprintf(stderr, "No file given in command line\n");
		return 1;
	}

	// setup archive
	struct archive* a = archive_read_new();
	archive_read_support_filter_all(a);
	archive_read_support_format_raw(a);

	// open archive file
	archive_read_open_filename(a, argv[argvFileLoc], 10240);

	// go to first header file
	struct archive_entry* entry;
	archive_read_next_header(a, &entry);

/*
	int infd = open(argv[1], O_RDONLY);
  if (infd < 0) {
    perror("Input open error");
    return 1;
  }
*/
	
	// used to get output filename
	int count = 1;
	while (1) {
		int result = writeLines(argv[argvFileLoc], a, count, lines);
		if (result == -1) {
			fprintf(stderr, "Error in writeLines function\n");
			return 1;
		}
		if (!result)
			break;
		count++;
	}

	archive_read_close(a);

	return 0;
}
