#include <stdio.h>
#include <string.h>
#include <math.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

int N, sum = 0;
int inputval;

int lines[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 5};
int dp[15][15*10]; //dp[i][j] : the minimum number with j number of lines until ith decimal point

int cntCycle(int num){
	if(num > inputval) return num - inputval;
	else return pow(10, N) - inputval + num;	
}

int calMinCycle(){
	int ans = pow(10, N);
	int tmpsum, tmpval, tmpans, cand;

	for(int i = 1; i <= N; i++){
		tmpsum = sum;
		tmpval = (int)(inputval / pow(10, i)) * pow(10, i);
		int subs = inputval - tmpval;
		for(int j = 1; j <= i; j++){
			tmpsum = tmpsum - lines[subs / (int)pow(10, i-1)];
			subs = subs / 10;
		}	
		printf("%d %d\n", tmpval, tmpsum);

		for(int k = 0; k <= 9; k++){
			cand = dp[i][tmpsum - lines[k]];
			if(cand != -1){
				printf("candidate : %d\n", cand);
				tmpans = tmpval + cand;
				if(tmpans != inputval) ans = tmpans; 
			} 
		}
	
	}
	return ans;
}

void digitalCnt(){
	
	for(int k = 0; k <= 9; k++){
		if(dp[1][lines[k]] == -1) dp[1][lines[k]] = k;
		else{
			if(lines[k] < lines[dp[1][lines[k]]]) dp[1][lines[k]] = k;
		}
	}
	for(int i = 2; i <= N; i++){
		for(int k = 0; k <= 9; k++){
			for(int j = 0; j < sum; j++){ 	
				if(dp[i-1][j] == -1) continue;
				
				if(j + lines[k] <= sum){
					if(dp[i][j + lines[k]] != -1) 
						dp[i][j + lines[k]] = MIN((dp[i-1][j] + k * pow(10, i-1)), (dp[i][j + lines[k]]));
					
					else dp[i][j + lines[k]] = dp[i-1][j] + k * (pow(10, i-1));
				}
			}
		}
	}
}


int main(){

	char inputstring[15] = "";
	scanf("%s", inputstring);
	
	inputval = atoi(inputstring);

	//calculate sum
	for(int i = 0; i < strlen(inputstring); i++){
		sum = sum + lines[inputstring[i] - '0'];
		N++;	
	}
	
	//initialize dp array
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 15 * 10; j++) dp[i][j] = -1;
	}

	digitalCnt();

	//debug code//
	for(int i = 1; i <= N; i++){
		for(int j = 0; j <= sum; j++){
			printf("%d ", dp[i][j]);
		}
		printf("\n");
	}
	//////////////
	printf("%d\n", calMinCycle());	
}
