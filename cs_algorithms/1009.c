#include <stdio.h>
#include <math.h>


int a, b;
int num;

int main(){
	int T;
	scanf("%d", &T);

	for(int i = 0; i < T; i++){
		scanf("%d %d", &a, &b);
		
		b = b % 4;
		if(b == 0) b = 4;
		
		num = (int)(pow(a, b)) % 10;
		if(num == 0) num = 10;
		
		printf("%d\n", num);
	}
}
