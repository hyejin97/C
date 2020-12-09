#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define cNUM 100

int *cache;
int *fifoq;
int *dirtycache;
int *works;
char *rw;
int hit, miss, total, qsize, cachesize, workload = 0;

int search(int page){
        //search
        for(int i = 0; i < cachesize; i++){
                if(page == cache[i]){
                        return i;
                }
        }
        return -1;

}

int main(int argc, char** argv){
	int idx_evict;
	int time = 0;
	char *s = (char *)malloc(sizeof(char) * 6);
	char temp[3];
	FILE *fp = fopen(argv[1], "r");	
	int cNUM = atoi(argv[2]);
	
        workload = 100000;

	printf("---------------------------------\n");
	printf("cache size : %d\n", cNUM);	

	//input workload
	works = (int *)malloc(sizeof(int) * workload);	
	rw = (char *)malloc(sizeof(char) * workload);
	cache = (int *)malloc(sizeof(int) * cNUM);
	fifoq = (int *)malloc(sizeof(int) * cNUM);
	dirtycache = (int *)malloc(sizeof(int) * cNUM);

	for(int i = 0; i < workload; i++){
		fgets(s, 7, fp);
		temp[0] = '\0';
		for(int j = 2; j < strlen(s); j++){
			temp[j-2] = s[j];			
		}
		rw[i] = s[0];
		works[i] = atoi(temp);
	}

	int idx_visited;
	int dirtycount = 0;
	//hit rate
	for(int i = 0; i < 100000; i++){
		time++; //access cache
		if((idx_visited = search(works[i])) != -1){
			hit++;
			if(dirtycache[idx_visited] == 0){
				if(rw[i] == 'W') dirtycache[idx_visited] = 1;
			}

			continue;
		}

		miss++;
		//access disk
		time = time + 100;
		
		if(cachesize < cNUM){
			cachesize++;
			cache[cachesize - 1] = works[i];

			if(rw[i] == 'R') dirtycache[cachesize - 1] = 0;
			else if(rw[i] == 'W') dirtycache[cachesize - 1] = 1;

			qsize++;
			fifoq[qsize - 1] = cachesize - 1;
		}
		//replace
		else{
			
			idx_evict = fifoq[0];
			for(int k = 0; k < qsize-1; k++){
                                fifoq[k] = fifoq[k+1];
                        }
                        fifoq[qsize - 1] = idx_evict;

			//dirty page eviction
                        if(dirtycache[idx_evict] == 1){
				time = time + 100;
				dirtycount++;
			}
			cache[idx_evict] = works[i];
			if(rw[i] == 'R') dirtycache[idx_evict] = 0;
                        else if(rw[i] == 'W') dirtycache[idx_evict] = 1;	
		}
		
	}
	
	fclose(fp);
	printf("the number of miss : %d\n", miss);
	printf("elapsed time : %d\n", time);
	printf("dirtycount : %d\n", dirtycount);
	return 0;		
}
