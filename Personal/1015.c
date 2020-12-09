#include <stdio.h>
#include <stdlib.h>

int N;
int* A;
int* B;
int* P;

void sort(int* arr){
	for(int i = 0; i < N-1; i++){
		int min = i;
		for(int j = i+1; j < N; j++){
			if(arr[min] > arr[j]) min = j;
		}
		int tmp = arr[min];
		arr[min] = arr[i];
		arr[i] = tmp;
	} 
}

void findarr(){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if(B[j] != -1 && A[i] == B[j]){
				P[i] = j;
				B[j] = -1;				
				break;
			}	
		}
	}
}

int main(){

	scanf("%d", &N);
	A = (int*)malloc(sizeof(int) * N);
	B = (int*)malloc(sizeof(int) * N);

	for(int i = 0; i < N; i++){
		scanf(" %d", &A[i]);
	}
	
	P = (int*)malloc(sizeof(int) * N);
	for(int i = 0; i < N; i++) B[i] = A[i];
	sort(B);

	findarr();

	for(int i = 0; i < N; i++) printf("%d ", P[i]);
	printf("\n");
	
	free(A);
	free(B);
	free(P);
}
