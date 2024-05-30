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
#include <stdbool.h>

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
    this->map = (char**)calloc(height,width);//Allocate memory for map
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
    for (int i = 0; i < this->height; i++) {//Release memory for  map
        free(this->map[i]);
    }
}

/**
 * @brief Validate and return the width of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid width (5-100)
 */
int get_width(FILE *file)
{   
    int height = 0;
    char ch[MAX_DIM];
    int width[MAX_DIM];
    int i = 0;
    while (fgets(ch, sizeof(ch), file) != NULL) {//Gets every line character of the file
        width[i] = strlen(ch) - 1;
        height++;//Number of rows
    }
    if (height < MIN_DIM || height > MAX_DIM) {//Boundary judgment
        return 0;
    }
    return width[0];
}

/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid height (5-100)
 */
int get_height(FILE *file)
{
    int height = 0;
    char ch[MAX_DIM];
    int width[MAX_DIM];
    int i = 0;
    while (fgets(ch, sizeof(ch), file) != NULL) {//Gets every line character of the file
        width[i] = strlen(ch) - 1;
        height++;//Number of rows
    }
    if (height < MIN_DIM || height > MAX_DIM) {//Boundary judgment
        return 0;
    }
    return height;
    
}
/**
 * @brief read in a maze file into a struct
 *
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int check_maze(FILE *file){
    int check_width;
    int check_num = 0;
    char line[MAX_DIM];
    int res = 1;
    while (fgets(line,sizeof(line),file)!=NULL)//Gets each column character
    {
        check_width = strlen(line);
        if(line[check_width-1] == '\n'){//Remove line breaks that are counted in the count
            check_width--;
        }
        if(check_num == 0){
            check_num = check_width;
        }else if(check_num != check_width){//Invalid file when there is a column of unequal length
            res = 0;
            rewind(file);
            return res;
        }
    }
    rewind(file);
    return res;
}
int read_maze(maze *this, FILE *file)
{
    this->width = get_width(file);//Get width
    rewind(file);
    this->height = get_height(file);//Get height
    if (this->height == 0 || this->width == 0) {
        return 1;
    }
    rewind(file);
    int height = this->height;
    int width = this->width;
    int count = 0;
    char str[MAX_DIM];
    int isValid = 1;
    const char *cellset = "# S E ";//Determines whether a character belongs to a character set that is valid or not
    this->map = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++) {
        this->map[i] = (char *)malloc((width + 1) * sizeof(char));//Allocate memory for map
    }
    if(height < MIN_DIM || width < MIN_DIM){////Boundary judgment
        return 1;
    }
    while (fgets(str,sizeof(str),file))//Copy each column of characters into the map and add \0 to the equal-width section to prevent overstepping
    {
        strncpy(this->map[count], str, width);
        this->map[count][width] = '\0';
        count++;
    }
    for(int i=0;i<height;i++){//Traverse the map looking for invalid characters and find the start and end points
        for(int j=0;j<width;j++){
            if(this->map[i][j] == 'S'){
                this->start.x = j;
                this->start.y = i;
            }
            if(this->map[i][j] == 'E'){
                this->end.x = j;
                this->end.y = i;
            }
            if(strchr(cellset,this->map[i][j]) == NULL){
                return 1;
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
    if(direction == 'W'||direction == 'w'){// Forward
        if(this->map[player->y-1][player->x] == '#'){
            player->y += 1;
            printf("inavlid move");
        }
        if(player->y - 1>this->height||player->y - 1 < 0){
            player->y += 1;
            printf("invlid move");
        }
        player->y -= 1;
    }
    else if(direction == 'A'||direction == 'a'){//Towards the left
        player->x -= 1;
        if(this->map[player->y][player->x-1] == '#'){
            player->x += 1;
            printf("inavlid move");
        }
        if(player->x - 1 > this->width||player->x - 1 <0){
            player->x += 1;
            printf("invlid move");
        }
        player->x -= 1;
    }
    else if(direction == 'S'||direction == 's'){//backward
        if(this->map[player->y+1][player->x] == '#'){
            player->y -= 1;
            printf("inavlid move");
        }
        if(player->y + 1 >this->height||player->y + 1 <0){
            player->y -= 1;
            printf("invlid move");
        }
        player->y += 1;
    }
    else if(direction == 'D'||direction == 'd'){//Towards the right
        if(this->map[player->y][player->x+1] == '#'){
            player->x -= 1;
            printf("inavlid move");
        }
        if(player->x + 1> this->width||player->x + 1< 0){
            player->x -= 1;
            printf("invlid move");
        }
        player->x += 1;
    }else{
        printf("Unknown operation,please retry other input");//Invalid operation
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
    if(player->x == this->end.x && player->y == this->end.y){//The player wins when he overlaps with the end
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
    if (argc != 2)
    {
       fprintf(stderr, "Usage: ./maze <mazefile path>\n");
       return EXIT_ARG_ERROR;
    }
    // set up some useful variables (you can rename or remove these if you want)
    coord *player = malloc(sizeof(coord));
    maze *this_maze = malloc(sizeof(maze));
    FILE *f;
    char input;
    //open and validate mazefile
    f = fopen(argv[1], "r");
    if(!check_maze(f)){
        fclose(f);
        fprintf(stderr, "Invalid maze file\n");
        return EXIT_FILE_ERROR;
    }
    if (!f)
    {
        fprintf(stderr, "Failed to read maze file\n");
        return EXIT_FILE_ERROR;
    }
    //read in mazefile to struct
    if(read_maze(this_maze,f) == 1){
        fclose(f);
        fprintf(stderr, "Failed to read maze file\n");
        return EXIT_FILE_ERROR;
    }
    printf("%d",check_maze(f));
    player->x = this_maze->start.x;
    player->y = this_maze->start.y;
    // maze game loop   q
    while (1)
    {
        scanf(" %c", &input);
        if (input == 'M' || input == 'm')
        {
            print_maze(this_maze,player);
        }else if (input == 'Q' || input == 'q')
        {
           break;
        }else{
            move(this_maze, player, input);
        }
        // win
        if (has_won(this_maze, player))
        {
            printf("you have pass this maze\n");
            break;
        }
    }
    // return, free, exit
    fclose(f);
    free_maze(this_maze);
    return EXIT_SUCCESS;
}