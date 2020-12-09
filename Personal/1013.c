#include <stdio.h>
#include <string.h>

int table[2][10] = {7, 2, 3, 3, 7, 6, 3, 9, 7, 9, 1, 9, 9, 4, 5, 5, 8, 8, 1, 9};
char radiosgnl[200];
int sgnlen;

int isinPattern(){
	int state = 0;

	for(int i = 0; i < sgnlen; i++){
		state = table[radiosgnl[i] - '0'][state];
	}
	
	return (state == 4 || state == 5 || state == 8);
}

int main(){
	int T;
	scanf("%d", &T);
	getchar();
	for(int i = 0; i < T; i++){
		scanf("%s", radiosgnl);
		sgnlen = strlen(radiosgnl);
		if(isinPattern()) printf("YES\n");
		else printf("NO\n");
	
	}
}
