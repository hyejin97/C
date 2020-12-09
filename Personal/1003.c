#include <stdio.h>

int fibo[40];
int zero[40];
int one[40];

int fibonacci(int n){

	for(int i = 2; i < n + 1; i++){
		fibo[i] = fibo[i - 1] + fibo[i - 2];
		zero[i] = zero[i - 1] + zero[i - 2];
		one[i] = one[i - 1] + one[i - 2];

	}

	return fibo[n];
}

int main(){
	int num, n;
	scanf("%d", &num);

	for(int i = 0; i < num; i++){

		scanf("%d", &n);
		//initialization
		for(int i = 0; i < 40; i++) fibo[i] = -1;
		fibo[0] = 0;
		fibo[1] = 1;

		zero[0] = 1;
		zero[1] = 0;

		one[0] = 0;
		one[1] = 1;

		fibonacci(n);
		printf("%d %d\n", zero[n], one[n]);

	}
}
