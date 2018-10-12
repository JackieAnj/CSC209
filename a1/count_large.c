#include <stdio.h>
#include <stdlib.h>


// TODO: Implement a helper named check_permissions that matches the prototype below.
int check_permissions(char *file, char *required){
    for (int i = 0; i < 9; i++) {
	if (required[i] != '-') {
	    if (file[i] != required[i]) {
		return 1;
	    }
	}
    }
    return 0;
}
int main(int argc, char** argv) {
    if (!(argc == 2 || argc == 3)) {
        fprintf(stderr, "USAGE: count_large size [permissions]\n");
        return 1;
    }

    // TODO: Process command line arguments
    int result = 0;
    char u1[5];
    int u2;
    scanf("%s %d", u1, &u2);

    char permissions[10];
    int u3;
    char u4[31];
    char u5[31];
    int size;
    char u6[31];
    int u7;
    char u8[31];
    char u9[31];
    int limit = strtol(argv[1], NULL, 10);
    while (scanf("%s %d %s %s %d %s %d %s %s", permissions, &u3, u4, u5, &size, u6, &u7, u8, u9) != EOF) {
	if (argc == 3) {
	    int is_valid = check_permissions(&permissions[1], argv[2]);
	    if ((is_valid == 0) && (size > limit)) {
		result ++;
	    }
	} else {
	    if (size > limit) {
		result ++;
	    }
	}
    }    	
    // TODO Call check permissions as part of your solution to counting the files to
    // compute and print the correct value.
    printf("%d\n", result);
    return 0;
}
