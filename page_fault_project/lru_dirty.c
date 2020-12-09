#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *cache;
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

	for(int i = 0; i < workload; i++){
		fgets(s, 7, fp);
		temp[0] = '\0';
		for(int j = 2; j < strlen(s); j++){
			temp[j-2] = s[j];			
		}
		rw[i] = s[0];
		works[i] = atoi(temp);
	}

	int dirtycount = 0;
	int idx_recent;
        int val_recent;
	int dirtybit_rct;
	//hit rate
	for(int i = 0; i < workload; i++){
		time++; //access cache
		
		if((idx_recent = search(works[i])) != -1){
			hit++;
			if(dirtycache[idx_recent] == 0){
                                if(rw[i] == 'W') dirtycache[idx_recent] = 1;
                        }

                        val_recent = cache[idx_recent];
                        memmove(cache + 1, cache, sizeof(int)*(idx_recent));
                        cache[0] = val_recent;
			
			dirtybit_rct = dirtycache[idx_recent];                        
			memmove(dirtycache + 1, dirtycache, sizeof(int)*(idx_recent));
			dirtycache[0] = dirtybit_rct;

			continue;

		}

		miss++;
		//access disk
		time = time + 100;
		
		if(cachesize < cNUM){
			memmove(cache+1, cache, sizeof(int)*(cachesize));
                        cache[0] = works[i];

			memmove(dirtycache+1, dirtycache, sizeof(int)*(cachesize));
			if(rw[i] == 'R') dirtycache[0] = 0;
			else if(rw[i] == 'W') dirtycache[0] = 1;
			cachesize++;
		}
		//replace
		else{
			//dirty page eviction
                        if(dirtycache[cachesize-1] == 1){
				time = time + 100;
				dirtycount++;
			}
			memmove(cache+1, cache, sizeof(int)*(cachesize - 1));
                        cache[0] = works[i];
			
			memmove(dirtycache+1, dirtycache, sizeof(int)*(cachesize - 1));
			if(rw[i] == 'R') dirtycache[0] = 0;
                        else if(rw[i] == 'W') dirtycache[0] = 1;	
		}
	
	}
	
	fclose(fp);
	printf("the number of miss : %d\n", miss);
	printf("elapsed time : %d\n", time);
	printf("dirtycount : %d\n", dirtycount);
	return 0;		
}
