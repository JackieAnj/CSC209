#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ptree.h"


int main(int argc, char **argv) {
    // TODO: Update error checking and add support for the optional -d flag
    // printf("Usage:\n\tptree [-d N] PID\n");
 
    // NOTE: This only works if no -d option is provided and does not
    // error check the provided argument or generate_ptree. Fix this!
    struct TreeNode *root = NULL;
    if (argc == 2) {
	//if there is no optional flag run the program as is
        if (generate_ptree(&root, strtol(argv[1], NULL, 10)) == 1) {
	    return 2;
	}
        print_ptree(root, 0);
    } else if ((argc == 4) && (strcmp(argv[1], "[-d")==0)){
	//if there is an optional flagi only display trees with depth less than the argument
	if (generate_ptree(&root, strtol(argv[3], NULL, 10)) == 1) {
	    return 2;
	}
	int depth = strtol(strtok(argv[2], "]"), NULL, 10);
	print_ptree(root, depth);
    } else {
	//if the number of argument is incorrect or if the
	//optional flag is incorrect print the proper input format
	fprintf(stderr, "Usage: \n\tptree [-d N] PID\n");
	exit(1);
    }
    return 0;
}

