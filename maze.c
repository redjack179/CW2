/**
 * @file maze.c
 * @author (YOUR NAME)
 * @brief Code for the maze game for COMP1921 Assignment 2
 * NOTE - You can remove or edit this file however you like - this is just a provided skeleton code
 * which may be useful to anyone who did not complete assignment 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

// defines for the required autograder exit codes
#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

typedef struct __Coord
{
    int x;
    int y;
} coord;

typedef struct __Maze
{
    char **map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

/**
 * @brief Initialise a maze object - allocate memory and set attributes
 *
 * @param this pointer to the maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return int 0 on success, 1 on fail
 */
int create_maze(maze *this, int height, int width)
{   
    this->height = height;
    this->width = width;
    this->map = (char**)calloc(height,width);
    if(this->map != NULL){
        return 0;
    }else{
        return 1;
    }
}

/**
 * @brief Free the memory allocated to the maze struct
 *
 * @param this the pointer to the struct to free
 */
void free_maze(maze *this)
{
    free(this->map);
}

/**
 * @brief Validate and return the width of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid width (5-100)
 */
int get_width(FILE *file)
{   
    int maze_width[MAX_DIM];
    int i = 0;
    int j = 0;
    int length;
    int ch;
    int count = 1;
    while (ch = fgetc(file) != EOF)
    {
        fseek(file,i,SEEK_SET);
        ch = fgetc(file);
        i++;
        if(ch == '\n'){
            maze_width[j] = i;
            j++;
        }
    }
    if(maze_width[0]>MAX_DIM || maze_width[0]<MIN_DIM){
        return 0;
    }
    for(int m=1;m<MAX_DIM;m++){
        if(maze_width[m]-maze_width[m-1] == maze_width[0]){
            count++;
        }
    }
    if(count != j){
        return 0;
    }else if (count == j)
    {
        return maze_width[0]-1;
    }
}

/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid height (5-100)
 */
int get_height(FILE *file)
{
    int maze_width[MAX_DIM];
    int i = 0;
    int j = 0;
    int length;
    int ch;
    int count = 1;
    while (ch = fgetc(file) != EOF)
    {
        fseek(file,i,SEEK_SET);
        ch = fgetc(file);
        i++;
        if(ch == '\n'){
            maze_width[j] = i;
            j++;
        }
    }
    if(j>MAX_DIM || j<MIN_DIM){
        return 0;
    }
    for(int m=1;m<MAX_DIM;m++){
        if(maze_width[m]-maze_width[m-1] == maze_width[0]){
            count++;
        }
    }
    if(count != j){
        return 0;
    }else if (count == j)
    {
        return j;
    }
    
}

/**
 * @brief read in a maze file into a struct
 *
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(maze *this, FILE *file)
{
    this->height = get_height(file);
    fseek(file,0,SEEK_SET);
    this->width = get_width(file);
    fseek(file,0,SEEK_SET);
    if(this->height <= MIN_DIM || this->width <= MIN_DIM){
        return 1;
    }
    for(int i=0;i<this->height;i++){
        fgets(this->map[i],this->width,file);
        this->map[i][this->width] = '\0';
        for(int j = 0;j<this->width;j++){
            if(this->map[i][j] == 'S'){
                this->start.x = j;
                this->start.y = i;
            }
            if(this->map[i][j] == 'E'){
                this->end.x = j;
                this->end.y = i;
            }
        }
    }
    return 0;
}

/**
 * @brief Prints the maze out - code provided to ensure correct formatting
 *
 * @param this pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze *this, coord *player)
{
    // make sure we have a leading newline..
    printf("\n");
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            // decide whether player is on this spot or not
            if (player->x == j && player->y == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", this->map[i][j]);
            }
        }
        // end each row with a newline.
        printf("\n");
    }
}

/**
 * @brief Validates and performs a movement in a given direction
 *
 * @param this Maze struct
 * @param player The player's current position
 * @param direction The desired direction to move in
 */
void move(maze *this, coord *player, char direction)
{
    player->x = this->start.x;
    player->y = this->start.y;  
    if(direction == 'W'||direction == 'w'){
        player->y -= 1;
        if(this->map[player->y][player->x] == '#'||player->x > this->width||player->y >this->height||player->x < 0||player->y <0){
            player->y += 1;
            printf("inavlid move");
        }
    }
    else if(direction == 'A'||direction == 'a'){
        player->x -= 1;
        if(this->map[player->y][player->x] == '#'||player->x > this->width||player->y >this->height||player->x < 0||player->y <0){
            player->x += 1;
            printf("inavlid move");
        }
    }
    else if(direction == 'S'||direction == 's'){
        player->y += 1;
        if(this->map[player->y][player->x] == '#'||player->x > this->width||player->y >this->height||player->x < 0||player->y <0){
            player->y -= 1;
            printf("inavlid move");
        }
    }
    else if(direction == 'D'||direction == 'd'){
        player->x += 1;
        if(this->map[player->y][player->x] == '#'||player->x > this->width||player->y >this->height||player->x < 0||player->y <0){
            player->x -= 1;
            printf("inavlid move");
        }
    }     
}

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param this current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze *this, coord *player)
{
    if(player->x == this->end.x && player->y == this->end.y){
        return 1;
    }else{
        return 0;
    }
}

/// @brief 
/// @param argc 
/// @param argv 
/// @return 
int main(int argc, char *argv[])
{
    // check args
    /*if (argc != 2)
    {
        fprintf(stderr, "Usage: ./maze <mazefile path>\n");
        return EXIT_ARG_ERROR;
    }*/
    // set up some useful variables (you can rename or remove these if you want)
    coord *player;
    maze *this_maze = malloc(sizeof(maze));
    FILE *f;
    char input;
    // open and validate mazefile
    f = fopen("15x9.txt", "r");
    if(read_maze(this_maze,f) == 1){
        printf("no");
    }else if(read_maze(this_maze,f) == 0){
        printf("yes");
    }else{
        printf("?");
    }
    /*if (!f)
    {
        fprintf(stderr, "Failed to read maze file\n");
        return EXIT_FILE_ERROR;
    }
    // read in mazefile to struct
    if(read_maze(this_maze,f) == 1){
        fclose(f);
        return EXIT_FILE_ERROR;
    }
    player->x = this_maze->start.x;
    player->y = this_maze->start.y;
    printf(this_maze);*/
    // maze game loop
    /*while (1)
    {
        scanf(" %c", &input);
        if (input == 'M' || input == 'm')
        {
            print_maze(this_maze,player);
        }
        move(this_maze, player, input);
        // win
        if (has_won(this_maze, player))
        {
            printf("Congratulations, you have reached the exit!\n");
            break;
        }
    }
    // return, free, exit
    fclose(f);
    free_maze(this_maze);
    return EXIT_SUCCESS;*/
}