#include <stdio.h>
#define CONST 5


int main(){
	int nums[] = {5,7,8,1,2,5,4,3,6,8,4,1,2,13,5,9,2,10,1,5,5,4,8,6,2,88,8,5,4};

	int amountOfNumbers = sizeof(nums)/sizeof(int);

	merge(nums,amountOfNumbers);

	for(int i=0; i<amountOfNumbers; i++){
		printf("%d ", nums[i]);
	}

	return 0;
}
