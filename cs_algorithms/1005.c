#include <stdio.h>

int time[1001]; // time cost to build the building i
int D[1001]; //shortest time to build the building i

struct Node{
	int parents[1000];
	int len;

};

struct Node links[100001]; //the array of rules

void node_init(int n){
	for(int i = 1; i < n+1; i++){
		for(int j = 0; j < n; j++){

			links[i].parents[j] = 0;
		}
		links[i].len = 0;
	}
}

int shortestTime(int n){

	if(links[n].len == 0) return time[n];

	int max = 0;
	int pval;
	for(int i = 0; i < links[n].len; i++){
		pval = D[links[n].parents[i]];
		if(pval == -1) pval = shortestTime(links[n].parents[i]);
		if(pval > max) max = pval;
				
	}
	D[n] = max + time[n];
	return D[n];
	
}

int main(){
	
	int T, N, K, X, Y, W;
	scanf("%d", &T);
	
	for(int i = 0; i < T; i++){
		scanf("%d %d", &N, &K);
	
		node_init(N);	
		for(int j = 1; j < N+1; j++){
			scanf("%d", &time[j]);
			D[j] = -1;
		}
		
		for(int j = 0; j < K; j++){
			scanf("%d %d", &X, &Y);
					
			links[Y].parents[links[Y].len] = X;
			links[Y].len++;
	
		}
		scanf("%d", &W);
		printf("%d\n", shortestTime(W));
	}

}
