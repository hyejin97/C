#include <stdio.h>
#include <math.h>

#ifndef max
#define max(a,b) ((a > b) ? a : b)
#endif

int seat[10][10];
int brocken[10];
int dp[10][1025];
int N, M;
int ans = 0;

void maxArrange(int i, unsigned int state){

	if(i == M) return;

	unsigned int cunning;

	//check eligibility
	cunning = (state | (state >> 1));

	int count, result = 0;
	//for every possible arrangement
	for(unsigned int bit = 0; bit < (1 << N); bit++){
				
		//count # of students of eligible arrangement
		count = 0;
		if(cunning & bit || brocken[i] & bit) continue;

		for(int k = 0; k < N; k++){
			if(bit & (1 << k)) count++;
		}
		if(result < dp[i][state]) result = dp[i][state];
		dp[i+1][bit] = count + result;
		ans = max(ans, dp[i+1][bit]);
		
		//printf("i : %d\n", i);
                //printf("state : %u\n", bit);
                //printf("count : %d\n", count);
		//printf("dp : %d\n", dp[i+1][bit]);
		maxArrange(i+1, bit);		
	}
}

int main(){
	int T;
	char tok;
	scanf("%d", &T);
	getchar();
	for(int i = 0; i < T; i++){
		scanf("%d %d", &N, &M);
		for(int j = 0; j < M; j++){
			for(int k = 0; k < N; k++){
				scanf(" %c", &tok);
				if(tok == '.') seat[j][k] = 1;
				else if(tok == 'x'){
					seat[j][k] = 0;
					brocken[j] |= 1 << k;
				}
			}
		}
		for(int j = 0; j < 10; j++){
			for(int k = 0; k < 1025; k++){
				dp[j][k] = 0;
			}
		}
		maxArrange(0,0);
		printf("%d\n", ans);
	}

}
