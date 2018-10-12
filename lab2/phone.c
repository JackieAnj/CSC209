#include <stdio.h>
#include <stdlib.h>

int phone(){
	char phone[11];
	int num;
	int result = 0;
	scanf("%s", phone);
	scanf("%d", &num);

	if (num == 0){
		printf("%s\n", phone);
	} else if (num > 0 && num < 10) {
		printf("%c\n", phone[num]);
	} else {
		printf("%s", "ERROR\n");
		result = 1;
	}
	return (result);
}

int main(){
	phone();
	return 0;
}
