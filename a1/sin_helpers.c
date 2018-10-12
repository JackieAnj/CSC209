// TODO: Implement populate_array
/*
 * Convert a 9 digit int to a 9 element int array.
 */
int populate_array(int sin, int *sin_array) {
    int count = 0;
    while(sin > 0) {
        int digit = sin % 10;
	sin_array[8-count] = digit;
	sin /= 10;
	count ++;
    }

    if (count != 9) {
	return 1;
    }
    return 0;
} 

// TODO: Implement check_sin
/* 
 * Return 0 (true) iff the given sin_array is a valid SIN.
 */
int check_sin(int *sin_array) {
    int n[9] = {1, 2, 1, 2, 1, 2, 1, 2, 1};

    if (sin_array[0] == 0) {
	return 1;
    } else {
	int total_sum = 0;
	int temp;
	for (int i=0; i <9; i++) {
	    temp = sin_array[i] * n[i];
	    int sum = 0;
	    while (temp > 0) {
		int digit = temp % 10;
		sum = sum + digit;
		temp /= 10;
	    }
	    sin_array[i] = sum;
	}
	for (int i=0; i<9; i++) {
	    total_sum = total_sum + sin_array[i];
	}
	if (total_sum % 10 == 0) {
	    return 0;
	}
    }
    return 1;
}

