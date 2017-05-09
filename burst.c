/*
  burst.c

  reads in txt file and separates them into 500 line segments
*/

#include "writeLines.c"

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

int main(int argc, char* argv[]) {  
	if (argc < 2) {
		fprintf(stderr, "usage: burst <input file>\n");
		return 1;
	}

	// setup archive
	struct archive* a = archive_read_new();
	archive_read_support_filter_all(a);
	archive_read_support_format_raw(a);

	// open archive file
	archive_read_open_filename(a, argv[1], 10240);

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
		int result = writeLines(argv[1], a, count);
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
