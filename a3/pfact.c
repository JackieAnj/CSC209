#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <errno.h>
#include "filter.h"

int print_message(int m, int n, int factor1, int factor2) {
    // helper for pfact
    // takes in variables and print out the type of number n is
    // returns 0 if conditions are met returns 1 otherwise
    if (factor2 != 0) {
	// two factors were found
	if (factor2 < sqrt(n)) {
	    printf("%d is not the product of two primes\n", n);
	} else {
	    // factor 2 is greater or equal to the square root of n
	    printf("%d %d %d\n", n, factor1, factor2);
	}
	return 0;
    }
    // if less than 2 factors were found
    if (sqrt(n) <= m) {
	if (factor1 == 0) {
	    // if no factors was found
	    if (n/m == m) {
		printf("%d %d %d\n", n, m, m);
	    } else {
		printf("%d is a prime\n", n);
	    }
	} else if (factor2 == 0) {
	    // if 1 factor was found
	    printf("%d is not the product of two primes\n", n);
	}
	return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    if (argc == 2) {
	// if the number of arguments is correct
	// find the value of n
	char *endptr;
	int n;
	n = strtol(argv[1], &endptr, 10);
	// if n is a positive integer proceed with function
	if (*endptr == 0 && n > 1) {
	    // set up all useful variables
	    int m = 2, factor1 = 0, factor2 = 0, result = 0, status;

	    // create a pipe
	    // pipe all the numbers from 2 to n into the pipe
	    int fd[2];
	    pipe(fd);
	    for (int i = 2; i <= n; i ++) {
		if (write(fd[1], &i, sizeof(int)) != sizeof(int)) {
		    perror("wrtie");
		    exit(1);
		}
	    }

	    // fork the first child process
	    int masterpid = fork();
	    if (masterpid < 0) {
		perror("fork");
		exit(1);
	    } else if (masterpid == 0) { // child processes
		// set original fd as parent fd
		int *parent_fd = fd;
		// keep forking new children as long as factor2 is not found
		// and square root of n is greater than m
		while(sqrt(n) > m && factor2 == 0) {
		    int child_fd[2];
		    pipe(child_fd);
		    int childpid = fork();
		    if (childpid < 0) {
			perror("fork");
			exit(1);
		    } else if (childpid == 0) { // second child
			// set m as the first number in the updated list
			if (read(child_fd[0], &m, sizeof(int)) < 0) {
			    perror("read");
			    exit(1);
			}
			close(parent_fd[1]);
			close(child_fd[0]);
			// set the factors of n if there are any
			if (n%m == 0) {
			    if (factor1 == 0) {
				factor1 = m;
			    } else {
				factor2 = m;
			    }
			}
			// if the while loop is no longer satisfied
			// print the corresponding message and exit the loop
			if (print_message(m, n, factor1, factor2) == 0) {
			    exit(0);
			}
		    } else { // first child
			close(parent_fd[1]);
			close(child_fd[0]);
			// filter out multiples of m and writes to child_fd
			filter(m, parent_fd[0], child_fd[1]); 
			// close pipes no longer used and set the new parent_fd to child_fd
			close(parent_fd[0]);
			parent_fd = child_fd;
			close(child_fd[1]);
			// collect exit status of child and exit with 1 plus the result
			wait(&status);
			if (WIFEXITED(status)) {
			    result = WEXITSTATUS(status);
			}
			exit(result + 1);
		    }
		}

	    } else { // master process
		//print the total number of filters used
		wait(&status);
		if (WIFEXITED(status)) {
		    result += WEXITSTATUS(status);
		}
		printf("Number of filters = %d\n", result);
		exit(0);
	    }
	}
    }
    // if the input is not valid print error message and exit
    fprintf(stderr, "Usage: \n\tpfact n\n");
    exit(1);
}
