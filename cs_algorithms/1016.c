#include <stdio.h>
#include <math.h>

long long min, max;
long long squares[1001000];
long long dividable[1000001] = {0, };
int len = 0;


int main(){
	scanf("%lld %lld", &min, &max);
	long long max_sqrt = (long long)sqrt(max);

	for(long long i = 2; i <= max_sqrt; i++){
		squares[i] = i * i;
		len++;
	}
	
	int count = 0;
	for(int i = 2; i < len + 2; i++){
		long long min_dividable = min;
		
		if(min_dividable % squares[i] != 0){
			min_dividable = ((min / squares[i]) + 1) * squares[i];
		}

		for(long long j = min_dividable; j <= max; j += squares[i]){
			if(dividable[j - min] != 1){
				dividable[j - min] = 1;
				count ++;
			}	
		}
	
	}
	long long result = max - min + 1 - count;
	printf("%lld\n", result);

	return 0;
}
