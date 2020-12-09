#include <stdio.h>
int C[30][30];

int combination(int m, int n){
	
	if(m == n || n == 0) return 1;
	if(C[m][n]) return C[m][n];

	C[m][n] = combination(m-1, n-1) + combination(m-1, n);
	return C[m][n];
}
void init(){
	for(int i = 0; i < 30; i++){
		for(int j = 0; j < 30; j++){
			C[i][j] = 0;
		}
	}
}
int main(){
	int T, N, M, result;
	scanf("%d", &T);

	for(int i = 0; i < T; i++){
		scanf("%d %d", &N, &M);

		//initialize arr
		init();
		result= combination(M, N);	
		printf("%d\n", result);

	}

}
