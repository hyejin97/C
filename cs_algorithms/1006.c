#include <stdio.h>

int e[10000][2];
int a[10001], b[10001], c[10001];
int N, W;


void minGroup(int start){
	int min;
	for(int i = start; i < N; i++){
		//update a
		if(b[i] + 1 > c[i] + 1) a[i+1] = c[i] + 1;
		else a[i+1] = b[i] + 1;
	       	
		if(e[i][0] + e[i][1] <= W){
			if(a[i+1] > a[i] + 1) a[i+1] = a[i] + 1;
		}
		if(i > 0 && e[i-1][0] + e[i][0] <= W && e[i-1][1] + e[i][1] <= W){
			if(a[i+1] > a[i-1] + 2) a[i+1] = a[i-1] + 2;
		}

		//update b
		if(i >= N-1) continue;
		b[i+1] = a[i+1] + 1;
		if(e[i][0] + e[i+1][0] <= W){
			if(b[i+1] > c[i] + 1) b[i+1] = c[i] + 1;
		}

		//update c
		c[i+1] = a[i+1] + 1;
                if(e[i][1] + e[i+1][1] <= W){
                        if(c[i+1] > b[i] + 1) c[i+1] = b[i] + 1;
                } 	
	}
	
}


int main(){
	int T;
	
	scanf("%d", &T);
       	
	for(int i = 0; i < T; i++){
	
		scanf("%d %d", &N, &W); 
	
		for(int j = 0; j < 2; j++){
			for(int k = 0; k < N; k++)
				scanf("%d", &e[k][j]);
		}
		
		//initial conditions & round array
		
		//case1 : continuous neither 1&N nor N+1&2N
		a[0] = 0;
		b[0] = 1;
		c[0] = 1;
		minGroup(0);
		int minresult = a[N];
		//case2 : continuous 1&N
		if(e[0][0] + e[N-1][0] <= W){
              		a[1] = 1;
            		b[1] = 2;
            		c[1] = e[0][1] + e[1][1] <= W? 1 : 2;  
			minGroup(1);
			if(c[N-1] + 1 < minresult) minresult = c[N-1] + 1;
		}
		//case3 : continuous N+1&2N
		if(e[0][1] + e[N-1][1] <= W){
			a[1] = 1;
            		b[1] = e[0][0] + e[1][0] <= W? 1 : 2;
            		c[1] = 2;
			minGroup(1);
			if(b[N-1] + 1 < minresult) minresult = b[N-1] + 1;
		}
		//case4 : continuous 1&N, N+1&2N 
		if(e[0][0] + e[N-1][0] <= W && e[0][1] + e[N-1][1] <= W){
			a[1] = 0;
            		b[1] = 1;
			c[1] = 1;
			minGroup(1);
			if(a[N-1] + 2 < minresult) minresult = a[N-1] + 2;
		}
		printf("%d\n", minresult);
	}
	return 0;
		
}
