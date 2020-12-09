#include <stdio.h>
#include <string.h>

int N; // list size
int lg[25], rg[25]; //to save paired el (left, right)
int visited[25]; 
int input[50];
int result[25], left[25], right[25];
int resultidx, leftidx, rightidx = 0;

int isPrime(int num){
    if(num == 1) return 0;
    else if(num != 2 && num % 2 == 0) return 0;
    
    for (int i = 3; i * i <= num; i += 2) {
        if(num % i == 0) return 0;
    }
 
    return 1;

}

int dfs(int lidx){

	if(visited[lidx]) return 0;
	visited[lidx] = 1;

	for(int i = 0; i < N/2; i++){
		//if rg is empty(not paired yet) or paired left element has another candidate
		if(isPrime(left[lidx] + right[i]) && (rg[i] == -1 || dfs(rg[i]))){
			lg[lidx] = i;
			rg[i] = lidx;
			return 1;
		}
	}
	return 0;
}

void selectsort(int* arr, int len){
	for(int i = 0; i < len - 1; i++){
                        int minresult = i;
                        for(int j = i+1; j < len; j++){
                                if(arr[minresult] > arr[j]) minresult = j;
                        }
                        if(minresult != i){
                                int temp = arr[i];
                                arr[i] = arr[minresult];
                                arr[minresult] = temp;
                        }
                }

}

int main(){
	scanf("%d", &N);

	for(int i = 0; i < N; i++){
		scanf(" %d", &input[i]);
			
		if(input[0] % 2 == input[i] % 2) left[leftidx++]  = input[i];
                else right[rightidx++] = input[i];
	}
	
	//exception
	if(leftidx != rightidx){
		printf("-1\n");
		return 0;
	}

	for(int i = 0; i < N/2; i++){
		if(isPrime(left[0] + right[i])){
			int count = 1;
			for(int k = 0; k < 25; k++){
                        	lg[k] = -1;
                        	rg[k] = -1;
                	}
			lg[0] = i;
			rg[i] = 0;
			
			for(int j = 1; j < N/2; j++){
				for(int k = 0; k < 25; k++) visited[k] = 0;
				visited[0] = 1;
				if(dfs(j)) count++;
			}
			if(count == N/2) result[resultidx++] = right[i];
		}  
	}
	//sort result
	if(resultidx == 0) printf("-1\n");
	else{
		selectsort(result, resultidx);	
		for(int i = 0; i < resultidx; i++) printf("%d ", result[i]);
		printf("\n");
	}
	return 0;
}
