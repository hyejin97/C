#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cNUM;
int *cache;
int *dirtycache;
int *visited;
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
        dirtycache = (int *)malloc(sizeof(int) * cNUM);
	visited = (int *)malloc(sizeof(int) * cNUM);

	for(int i = 0; i < workload; i++){
		fgets(s, 7, fp);
		temp[0] = '\0';
		for(int j = 2; j < strlen(s); j++){
			temp[j-2] = s[j];			
		}
		rw[i] = s[0];
		works[i] = atoi(temp);
	}

	int clockhand = 0;
	int idx_evict;
	int idx_visited;
	//hit rate
	for(int i = 0; i < workload; i++){
		time++; //access cache
		if((idx_visited = search(works[i])) != -1){
			hit++;
			if(dirtycache[idx_visited] == 0){
                                if(rw[i] == 'W') dirtycache[idx_visited] = 1;
                        }

                        visited[idx_visited] = 1;
                        continue;
		}

		miss++;
		//access disk
		time = time + 100;
		
		if(cachesize < cNUM){
			cachesize++;
                        cache[cachesize - 1] = works[i];
                        visited[cachesize - 1] = 1;
			if(rw[i] == 'R') dirtycache[cachesize - 1] = 0;
			else if(rw[i] == 'W') dirtycache[cachesize - 1] = 1;
		}
		//replace
		else{
			idx_evict = -1;
			while(idx_evict == -1){
                                if(visited[clockhand] == 1) visited[clockhand] = 0;
                                else if(visited[clockhand] == 0){
					idx_evict = clockhand;
					//dirty page eviction
                                        if(dirtycache[clockhand] == 1) time = time + 100;
                                        cache[clockhand] = works[i];
					visited[clockhand] = 1;
					if(rw[i] == 'R') dirtycache[clockhand] = 0;
                        		else if(rw[i] == 'W') dirtycache[clockhand] = 1;
                                }

				clockhand = (clockhand + 1) % cNUM;
                        }	
		}
		
	}
	
	fclose(fp);
	printf("the number of miss : %d\n", miss);
	printf("elapsed time : %d\n", time);
	return 0;		
}
