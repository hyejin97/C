#include <stdio.h>
#include <math.h>
#include <limits.h>

int X[20];
int Y[20];
int isminus[20];
int len, xsum, ysum = 0;
double min;


void minLen(int n, int r){
	double result;
	xsum = 0;
        ysum = 0;
	
	if(n == r){
		for(int i = 0; i < len; i++){
			if(isminus[i] == 1){
				xsum -= X[i];
				ysum -= Y[i];
			}
			else{
				xsum += X[i];
				ysum += Y[i];
			}
		}
		result = sqrt(pow(xsum, 2) + pow(ysum, 2));
		if(result < min) min = result;
		return;
	}
	if(r == 0) return;

	minLen(n-1, r-1);
	isminus[n-1] = 1;
	minLen(n-1, r);		
	isminus[n-1] = 0;
}

int main(){
	int T, N;
	scanf("%d", &T);
	for(int i = 0; i < T; i++){	
		scanf("%d", &N);
		len = N;
			
		for(int j = 0; j < N; j++){
			scanf("%d %d", &X[j], &Y[j]);
		}
		
		//initialize min value
		min = LLONG_MAX;
		minLen(N, N/2);
		printf("%.6lf\n", min);
	}
}
