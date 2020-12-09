#include <stdio.h>
int N;
int count[10] = {0, };

int main(){

	scanf("%d", &N);

	int digit = 1;
	int temp;
	while(N){
		if(N / 10 == 0){
			for(int i = 1; i <= N; i++){
                        	count[i] += (N/10 + 1) * digit;
                	}
			break;
		}

		//make the one digit num 9
		while(N % 10 != 9 && N != 0){
			temp = N;
			//count the number while decreasing it
			while(temp){
				count[temp % 10] += 1 * digit;	
				temp = temp / 10;
			}
			N--;
		}
		for(int i = 0; i < 10; i++){
			count[i] += (N/10 + 1) * digit;
		}
		//except the cases when the highest digit number is zero
		count[0] -= 1 * digit;
		digit = digit * 10;
		N = N / 10;
	}

	for(int i = 0; i < 10; i++){
		printf("%d ", count[i]);
	}
	printf("\n");

}
