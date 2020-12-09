#include <stdio.h>
#include <math.h>




int main(){
	int x1, y1, x2, y2, T, P, count;
	int cx, cy, r;
	//the number of testcases
	scanf("%d", &T);
	

	for(int i = 0; i < T; i++){
		//source, destination
        	scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

        	//the number of planets
        	scanf("%d", &P);
		
		count = 0;
		double dist_src, dist_dest;
		for(int j = 0; j < P; j++){
			scanf("%d %d %d", &cx, &cy, &r);
			dist_src = sqrt(pow(cx - x1, 2) + pow(cy - y1, 2));
			dist_dest = sqrt(pow(cx - x2, 2) + pow(cy - y2, 2));	
			
			if(dist_src < r && dist_dest > r) count++;
			else if(dist_dest < r && dist_src > r) count++;

		}

		printf("%d\n", count);	
	}
	
}
