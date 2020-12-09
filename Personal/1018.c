#include <stdio.h>
#define min(a, b) a < b? a : b

int N, M;
char board[50][50];
int mincount = 2500;

int main(){
    scanf("%d %d", &N, &M);
    for(int i = 0; i < N; i++){
            scanf("%s", &board[i]);
    }
    for(int i = 0; i < N - 7; i++){
	for(int j = 0; j < M - 7; j++){
		int cntb, cntw = 0;
		for(int p = i; p < i + 8; p++){
			for(int q = j; q < j + 8; q++){
				if((p + q) % 2 == 0){
					if(board[p][q] == 'B') cntw++; 
					else cntb++;
				}
				else{
					if(board[p][q] == 'B') cntb++;
					else cntw++;
				}	
			}
		}
		mincount = min(mincount, cntb);
		mincount = min(mincount, cntw);
	}
    }

    printf("%d\n", mincount) ;   
}
