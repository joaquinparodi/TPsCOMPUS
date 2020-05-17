#include <stdio.h>
#define SIZE 10


int main(){
	int nums[SIZE] = {10,9,8,7,6,20,22,15,63,1};

	for (int i=0; i<SIZE; i++){
		printf("Original:%d\n", nums[i]);
	}

	printf("-----------------------------------\n");
	int* ptr = merge(nums, SIZE);

	for (int i=0; i<SIZE; i++){
		printf("Modificado:%d\n", nums[i]);
	}


	printf("---------------------------------\n");

	for (int i=0; i<SIZE; i++){
		printf("Temporal:%d\n", ptr[i]);
	}

	return 0;
}
