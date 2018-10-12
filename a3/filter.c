#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int filter(int m, int read_fd, int write_fd) {
    // helper function for pfact
    int curr;
    // loop through all the numbers in read_fd
    while (read(read_fd, &curr, sizeof(int)) > 0) {
	// filter out all multiples of m
	if (curr%m != 0) {
	    // write the remaining numbers to write_fd
	    if (write(write_fd, &curr, sizeof(int)) != sizeof(int)) {
		perror("write");
		return 1;
	    }
	}
    }
    // error check for read
    if (read(read_fd, &curr, sizeof(int)) < 0) {
	perror("read");
	return 1;
    }
    return 0;
}
