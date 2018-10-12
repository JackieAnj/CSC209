#include <stdio.h>
#include <stdlib.h>

int populate_array(int, int *);
int check_sin(int *);


int main(int argc, char **argv) {
    // TODO: Verify that command line arguments are valid.
    if (!(argc == 2)) {
	return 1;
    }
    // TODO: Parse arguments and then call the two helpers in sin_helpers.c
    // to verify the SIN given as a command line argument.
    int num = strtol(argv[1], NULL, 10);
    int num_array[9];
    populate_array(num, num_array);
    int check = check_sin(num_array);
    if (check==1) {
	printf("%s\n", "Invaid SIN");
    } else {
	printf("%s\n", "Valid SIN");
    }
    return 0;
}
