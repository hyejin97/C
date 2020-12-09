#include <stdio.h>
#define ROW 9
#define COL 9 
#define CELL 9

int sudoku[ROW][COL];
int cellinfo[CELL][10];
int rowinfo[ROW][10];
int colinfo[COL][10];

void display(){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(j == 3 | j == 6) printf(" ");
            printf("%d", sudoku[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void solve(){
    //check if solved
/**    int count = 0;
    for(int i = 0; i < 9; i++)
	for(int j = 0; j < 9; j++)
		if(sudoku[i][j] == 0) count++;

    if(count == 0){
	display();
	return 1;
    }
**/
    //find empty cell
    int count = 0;
    int row, col, empty; 
    for(int i = 0; i < 9; i++){
	empty = 1;
        for(int j = 0; j < 9; j++){
            if(sudoku[i][j] == 0){
                row = i;
                col = j;
		empty = 0;
		count++;
                break;
            }
        }
        if(empty == 0) break;
    }

    if(count == 0)
        display();

    //try 1~9
    for(int val = 1; val < 10; val++){
        if(rowinfo[row][val] == 0 && colinfo[col][val] == 0 && cellinfo[3*(row/3)+col/3][val] == 0){
	    //change conditions and assign number
	    sudoku[row][col] = val;

            rowinfo[row][val] = 1;
	    colinfo[col][val] = 1;
	    cellinfo[3*(row/3)+col/3][val] = 1;
            //dfs
       	    solve();

	    sudoku[row][col] = 0;
	    rowinfo[row][val] = 0;
	    colinfo[col][val] = 0;
	    cellinfo[3*(row/3)+col/3][val] = 0;
        }
    }
}
       
int main(){

    int testnum;
    scanf("%d", &testnum);
    char input_row[11];

    while(testnum){
	//initialize arrays
        for (int i = 0; i < 9; i++){
                for (int j = 0; j < 10; j++){
                        rowinfo[i][j] = 0;
                        colinfo[i][j] = 0;
                        cellinfo[i][j] = 0;
                }
        }

        for(int i = 0; i < 9; i++){
            getchar();
            scanf("%[^\n]", input_row);

            int spacecnt = 0;
            for(int j = 0; j < 11 ;j++){
                if(input_row[j] == ' ') spacecnt++;
                else sudoku[i][j - spacecnt] = input_row[j] - '0';
            }
        }

	//save sudoku board info
	int val;
	for (int i = 0; i < 9; i++){
                for (int j = 0; j < 9; j++){
			val = sudoku[i][j];
                        rowinfo[i][val] = 1;
                        colinfo[j][val] = 1;
                        cellinfo[3*(i/3)+j/3][val] = 1;
                }
        }

        solve();
        testnum --;
        getchar();
    }

    return 0;
}

