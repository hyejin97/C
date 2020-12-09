#include <stdio.h>
#include <stdlib.h>

long long x, y;
long long  k, totalsum = 0;

long long minMove(){
	long long i = 1;

	if(y-x == 1) return 1;
	
	while(totalsum < y-x - 1){

		if((k + 1) * (1 + (k + 1)) / 2 <= y-x - totalsum){
			k = k + 1;
		}
		else if(k * (1 + k) / 2 > y-x - totalsum){
			k = k - 1;
		}
		totalsum += k;
		i++;
	}
	return i+1;		
}


int main(){

	int T;
	long long cnt;

	scanf("%d", &T);

	for(int i = 0; i < T; i++){
		scanf("%lld %lld", &x, &y);
		
		k = 1;
		totalsum = 1;

		cnt = minMove();
		printf("%lld\n", cnt);
		
	}
}
