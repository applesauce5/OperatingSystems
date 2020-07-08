#include <stdio.h>
void printBits(unsigned int num){
	unsigned int size = sizeof(unsigned int);
	unsigned int maxPow = 1<<(size*8-1);
	printf("MAX POW:  %u\n",maxPow);
	int i = 0;
	while(i < size*8){
		printf("%u ",num&maxPow ? 1 : 0);
		num = num<<1;
		i++;
	}
	printf("\n");
}

int main(){
	char str1[10];
	char str2[10];
	
	sprintf(str1,"%d",1);
	sprintf(str2,"%d", 2);

	strcat(str1,str2);

	unsigned int one = ((1<<10) | 2)<<12 ;
	unsigned int two = 2;
	
	printf("%d\n",atoi(str1));
	printBits(one);
	printBits(two);
	printf("Value of the number1..... %d\n",one);
	printf("Value of the number2..... %d\n", two);
	return 1;
}
