#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define MAX_ROWS 100
#define MAX_COLS 100
#define MIN_ROWS 5
#define MIN_COLS 5

int rows, cols;
int start_row, start_col, end_row, end_col;

void create_maze(char **this,int rows,int cols){
    for (int i = 0; i <= cols; i++) {
        this[i] = (char *)malloc((rows + 1) * sizeof(char));//Allocate memory for map
    }
    srand(time(NULL));
    for (int i = 1; i <= cols; i++) {
        for (int j = 1; j <= rows; j++) {//Assign the path with a three out of four probability
            int temp = rand() % 4;
            if (temp != 1) {
                this[i][j] = ' ';
            }
            else{
                this[i][j] = '#';//One in four for the wall
            }
            this[i][j+1] = '\n';
        }
    }
    start_row = rand() % rows+1;//Get the end and start points randomly
    start_col = rand() % cols+1;
    end_row = rand() % rows+1;
    end_col = rand() % cols+1;
    while (start_row == end_row && start_col == end_col)
    {   
        end_row = rand() % rows+1;
        end_col = rand() % cols+1;  
    }
    this[start_col][start_row] = 'S';
    this[end_col][end_row] = 'E';
}
void findPath(char **this,int start_row,int start_col,int end_col,int end_row){
    int d1,d2;
    d1 = start_row - end_row;//The path is created from the vector difference between the starting point and the ending point
    if(d1>0){
        for(int i=1;i<=abs(d1);i++){
            this[start_col][start_row-i]=' ';
        }
    }
    if(d1<0){
        for(int i =1;i<abs(d1);i++){
            this[start_col][start_row+i] = ' ';
        }
    }
    d2 = start_col - end_col;
    if(d2>0){
        for(int i =1;i<abs(d2);i++){
            this[start_col-i][start_row] = ' ';
        }
    }
    if(d2<0){
        for(int i =1;i<abs(d2);i++){
            this[start_col+i][start_row] = ' ';
        }
    }
}

int main(int argc, char *argv[]) {
    if(argc != 4){
        fprintf(stderr, "Usage: ./mazegen <filename> <rows> <cols>\n");
        return 1;
    }
    FILE *file;
    char* filename = argv[1];
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);//Get args
    char **this = malloc(cols*sizeof(char*));
    file = fopen(filename,"w");
    create_maze(this,rows,cols);
    findPath(this,start_row,start_col,end_col,end_row);
    for(int i = 1;i<=cols;i++){//Enter each character into the file
        for(int j = 1;j<=rows;j++){
            fputc(this[i][j],file);
        }
        fputc('\n',file);
    }
    
    fclose(file);
}