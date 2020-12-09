#include <stdio.h>
int M, N;
int square[50][50];
int isvisited[50][50];

void init_arr(){
	for(int i = 0; i < 50; i++){
		for(int j = 0; j < 50; j++){
			square[i][j] = 0;
			isvisited[i][j] = 0;
		}
	}
}


void dfs(const int y, const int x){
	isvisited[y][x] = 1;

	int nx, ny;
		
	//up
	if(y > 0){
		nx = x;
		ny = y - 1;
		if(square[ny][nx] == 1 && isvisited[ny][nx] != 1){
			dfs(ny, nx);
		}
	}
	//down
	if(y < N-1){
		nx = x;
		ny = y + 1;
		if(square[ny][nx] == 1 && isvisited[ny][nx] != 1){
                        dfs(ny, nx);
                }
	}
	//left
	if(x > 0){
		nx = x - 1;
		ny = y;
		if(square[ny][nx] == 1 && isvisited[ny][nx] != 1){
                        dfs(ny, nx);
                }
	}
	//right	
	if(x < M-1){
		nx = x + 1;
		ny = y;
		if(square[ny][nx] == 1 && isvisited[ny][nx] != 1){
                        dfs(ny, nx);
                }
	}
	return;	
}

int minWorm(){
	int count = 0;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < M; j++){
			if(square[i][j] == 1 && isvisited[i][j] != 1){
				dfs(i, j);	
				count++;
				//printf("%d %d\n", j, i);	
			}
		
		}
	}

	return count;
}


int main(){
	int T, K;
	int x, y;
	int min;

	scanf("%d", &T);
	for(int i = 0; i < T; i++){
		scanf("%d %d %d", &M, &N, &K);
		
		//initialize square
		init_arr();
		
		for(int j = 0; j < K; j++){
			scanf("%d %d", &x, &y);
			square[y][x] = 1;
		}
		min = minWorm();
		printf("%d\n", min);
	}

}
