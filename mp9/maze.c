// Anna Miller (annam4)
// ECE 220 - BD2

// This program opens a file and reads data from it using fscanf() to create a maze. Memory
// is dynamically allocated for the maze struct and the cells of the maze, which are
// referenced by a double pointer and stored as a 2D array. In order to do this, memory is allocated
// an array of pointers, where each pointer points to a row. In the createMaze function, the
// starting position and ending position are also set. The maze is the printed using printMaze. this
// function simply iterates through the 2D cell array and prints the appropriate character on
// the corresponding row. The destroyMaze functions deallocates memory from the heap by first freeing
// the memory for the 2D cell array, then freeing the memory for the struct. The solveMazeDFS actually
// solves the maze using recursion. It marks the solution path with '*', and marks cells that were
// tried, but not part of the solution, with '~'. The base cases will return 0 when the cell is out of bounds
// or is not a viable path, and will return 1 when the end of the maze is reached. The recursive
// case sets the cell to '*' and then calls itself to the left, then to the right, then for the above cell,
// then for the below cell. If the recursive call returns true, the solution has been found. If no
// recursive call works, the function returns 0 and the maze is unsolvable.

#include <stdio.h>
#include <stdlib.h>
#include "maze.h"


/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
    // indices for loops
    int i, j;
    char curchar; // hold the current character read from file

    // allocate memory for each cell of the maze
    maze_t *maze = (maze_t*)malloc(sizeof(maze_t));

    // open the file with maze information
    FILE *file = fopen(fileName, "r");
    if(file == NULL){
      return NULL;
    }

    // scan the row and column dimensions from the file
    fscanf(file, "%d %d", &(maze->width), &(maze->height));

    // allocate memory for the cells of the maze_t as a 2D array
    maze->cells = (char **)malloc(maze->height*sizeof(char*));
    for (i = 0; i < maze->height; i++){
      maze->cells[i] = (char *)malloc(maze->width*sizeof(char));
    }

    // fill each cell with the contents from the file
    for(i = 0; i < maze->height; i++){
      for(j = 0; j < maze->width; j++){
        // read the next character from the file
        fscanf(file, "%c", &curchar);
        // if the character is a newline, decrement j to ensure cells reflect file
        if(curchar == '\n'){
          j--;
        }
        else{
          // set the character to the appropriate cell
          maze->cells[i][j] = curchar;
          // if the character is 'S', set the starting row and column
          if(maze->cells[i][j] == 'S'){
            maze->startRow = i;
            maze->startColumn = j;
          }
          // if the character is 'E', set the ending row and columnl
          if(maze->cells[i][j] == 'E'){
            maze->endRow = i;
            maze->endColumn = j;
          }
        }
      }
    }
    // close the file
    fclose(file);

    return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
    int i;

    // free the space allocated to the cells
    for (i = 0; i < maze->height; i++) {
      free(maze->cells[i]);
    }
    free(maze->cells);

    // free the space allocated to the struct
    free(maze);
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
    int i, j;
    // iterate through each cell and print the value
    for(i = 0; i < maze->height; i++){
      for(j = 0; j < maze->width; j++){
        printf("%c", (maze->cells[i][j]));
      }
      // print a new line at each new row
      printf("\n");
    }
}


/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // Your code here. Make sure to replace following line with your own code.
    // base case 1: cell is out of bounds
    if(col >= maze->width || row >= maze->height || col < 0 || row < 0){
      return 0;
    }
    // base case 2: cell is not empty
    if( maze->cells[row][col] == '%' || maze->cells[row][col] == '~' || maze->cells[row][col] == '*'){
      return 0;
    }
    // base case 3: cell is the end of the maze
    if(row == maze->endRow && col == maze->endColumn){
      return 1;
    }

    // recursive case
    // mark the current location as part of the solution
    maze->cells[row][col] = '*';

    // check to the left
    if(solveMazeDFS(maze, col - 1, row)){
      // set the starting location back to 'S'
      maze->cells[maze->startRow][maze->startColumn] = 'S';
      return 1;
    }
    // check to the right
    if(solveMazeDFS(maze, col + 1, row)){
      // set the starting location back to 'S'
      maze->cells[maze->startRow][maze->startColumn] = 'S';
      return 1;
    }
    // check above
    if(solveMazeDFS(maze, col, row - 1)){
      // set the starting location back to 'S'
      maze->cells[maze->startRow][maze->startColumn] = 'S';
      return 1;
    }
    // check below
    if(solveMazeDFS(maze, col, row + 1)){
      // set the starting location back to 'S'
      maze->cells[maze->startRow][maze->startColumn] = 'S';
      return 1;
    }
    // if no directions work, backtrack and unmake choice
    maze->cells[row][col] = '~';

    return 0;
}
