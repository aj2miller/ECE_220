/* Anna Miller
 * ECE 220 - BD2
 *
 * Using the typedef struct game, an empty board is created as a 1D array. The value -1 indicates an empty space. The first step
 * was to initialize the board and set its members to the appropriate value in game *make_game(rows, cols). This function
 * returns a pointer to the board. The remake_game function operates similarly but with new dimensions for the board at the
 * memory location as the current board. The get_cell(cur_game, row, col) function returns a pointer to the specified cell
 * on the board, or returns a NULL pointer if the row and column are out of range. The movement functions all operate similarly.
 * move_w and move_s both iterate through each row in each column to find an empty cell. The difference is that move_w starts at
 * the second from the top row, and move_s at second from the bottom. When an empty cell is found, move_w looks for the topmost empty cell as the target, and
 * move_s lookks for the bottom empty cell as a target. The current filled cell is copied to the target, then set to -1. Next, the
 * functions check to see if any merges are possible. If so, the merge is implemented, and the score is changed to reflect the merge.
 * move_a and move_d are almost identical, except they iterate through each column in each row. move_a starts from the second leftmost column,
 * and move_d starts from the second rightmost column. When looking for a target cell, move_a searches for the leftmost empty cell
 * and move_d searches for the rightmost. The movement functions return 1 if any changes were made, and 0 if the move did nothing.
 * The function legal_move_check checks if the game has reached a "game over" state. It returns 1 if the game can continue and 0 if
 * no more moves are possible. This function creates a copy of the current game board, then calls all 4 movements for the copy.
 * if the movements all return 0, you know that no more legal moves exist, and the game ends.
 */

#include "game.h"


game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure)
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    int i, j; // row and column indices of game

    // fill each cell with -1
    for(i = 0; i < rows; i++){
      for(j =0 ; j < cols; j++){
        mygame->cells[i*cols + j] = -1;
      }
    }
    // set the row and column dimensions
    mygame->rows = rows;
    mygame->cols = cols;

    // set the score to 0
    mygame->score = 0;

    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any
	memory previously allocated is not lost in this function.
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	 //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
   int i, j; // row and column indices of game

   // fill each cell with -1
   for(i = 0; i < new_rows; i++){
     for(j =0 ; j < new_cols; j++){
     (*_cur_game_ptr)->cells[i*new_cols + j] = -1;
     }
   }
   // set the row and column dimensions
   (*_cur_game_ptr)->rows = new_rows;
   (*_cur_game_ptr)->cols = new_cols;

   // set the score to 0
   (*_cur_game_ptr)->score = 0;

	return;
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
    if(row >= 0 && row < cur_game->rows && col >= 0 && col < cur_game->cols){
      // return the address of the current cell in the game
      int *cur_cell;
      cur_cell = &cur_game->cells[row*(cur_game->cols) + col];
      return cur_cell;
    }
    // return NULL if the position is out of bounds
    return NULL;
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can
   not merge twice in one turn. If sliding the tiles up does not cause any
   cell to change value, w is an invalid move and return 0. Otherwise, return 1.
*/
{
    //YOUR CODE STARTS HERE
    // array corresponding to game board, element is 1 if current cell has been combined
    // this turn, and 0 if it has not
    int combined[(cur_game->rows) * (cur_game->cols)];
    // initialize array to 0
    int i, j;   //row and  column indices
    for(i = 0; i < cur_game->rows; i++){
      for(j = 0; j < cur_game->cols; j++){
        combined[i*(cur_game->cols) + j] = 0;
      }
    }

    int m, n; // row and column indices of the game board
    int x;  // extra index for finding the target row
    int target; // the target row (highest empty row)

    int changes = 0; // keep track of the number of changes from the move
    int move = 0; // flag to indicate a move can occur

    // go though board for tile moves
    //iterate through columns
    for(n = 0; n < cur_game->cols; n++){
      // iterate through each row in the column
      for(m = 1; m < cur_game->rows; m++){
        // check for a filled tile
        if(cur_game->cells[m*(cur_game->cols) + n] != -1){
          for(x = 0; x < m; x++){
            // look for the topmost empty cell
            if(cur_game->cells[x*(cur_game->cols) + n] == -1){
              target = x;
              move = 1;
              break;
            }
          }

          // if the target cell is empty, swap the filled cell with the target
          if(move == 1){
            // increment change counter
            changes++;
            // move the tile up
            cur_game->cells[target*(cur_game->cols) + n] = cur_game->cells[m*(cur_game->cols) + n];
            cur_game->cells[m*(cur_game->cols) + n] = -1;

            // reset the flag
            move = 0;

            // check if a merge is possible
            if(target > 0 && combined[(target -1)*(cur_game->cols) + n] == 0 && cur_game->cells[(target - 1)*(cur_game->cols) + n] == cur_game->cells[target*(cur_game->cols) + n]){
              // combine the rows
              cur_game->cells[(target - 1)*(cur_game->cols) + n] += cur_game->cells[target*(cur_game->cols) + n];
              cur_game->cells[target*(cur_game->cols) + n] = -1;

              // add the combo to the score
              cur_game->score += cur_game->cells[(target - 1)*(cur_game->cols) + n];

              // mark the cell as combined
              combined[(target - 1)*(cur_game->cols) + n] = 1;
            }
          }
          // if there was no tile movement, check for a merge
          else{
            if(combined[(m - 1)*(cur_game->cols) + n] == 0 && cur_game->cells[(m - 1)*(cur_game->cols) + n] == cur_game->cells[m*(cur_game->cols) + n]){
              // increment the change counter
              changes++;
              // merge the tiles
              cur_game->cells[(m - 1)*(cur_game->cols) + n] += cur_game->cells[m*(cur_game->cols) + n];
              cur_game->cells[m*(cur_game->cols) + n] = -1;

              // add the combo to the score
              cur_game->score += cur_game->cells[(m - 1)*(cur_game->cols) + n];

              // mark the cell as combined
              combined[(m - 1)*(cur_game->cols) + n] = 1;
            }
          }
        }
      }
    }

    // if any movements, return 1
    if(changes > 0){
      return 1;
    }
    // if no legalmoves, return 0
    else{
      return 0;
    }
};

int move_s(game * cur_game) //slide down
{
    //YOUR CODE STARTS HERE
    // array corresponding to game board, element is 1 if current cell has been combined
    // this turn, and 0 if it has not
    int combined[(cur_game->rows) * (cur_game->cols)];
    // initialize array to 0
    int i, j;   //row and  column indices
    for(i = 0; i < cur_game->rows; i++){
      for(j = 0; j < cur_game->cols; j++){
        combined[i*(cur_game->cols) + j] = 0;
      }
    }

    int m, n; // row and column indices of the game board
    int x;  // extra index for finding the target row
    int target; // the target row (highest empty row)

    int changes = 0; // keep track of the number of changes from the move
    int move = 0; // flag to indicate a move can occur

    // go though board for tile moves
    //iterate through columns
    for(n = cur_game->cols - 1; n >= 0; n--){
      // iterate through each row from the secon to last in the column
      for(m = cur_game->rows - 2; m >= 0; m--){
        // check for a filled tile
        if(cur_game->cells[m*(cur_game->cols) + n] != -1){
          for(x = cur_game->rows - 1; x > m; x--){
            // look for the lowest empty cell
            if(cur_game->cells[x*(cur_game->cols) + n] == -1){
              target = x;
              move = 1;
              break;
            }
          }

          // if the target cell is empty, swap the filled cell with the target
          if(move == 1){
            // increment change counter
            changes++;
            // move the tile up
            cur_game->cells[target*(cur_game->cols) + n] = cur_game->cells[m*(cur_game->cols) + n];
            cur_game->cells[m*(cur_game->cols) + n] = -1;

            // reset the flag
            move = 0;

            // check if a merge is possible
            if(target < cur_game->rows && combined[(target + 1)*(cur_game->cols) + n] == 0 && cur_game->cells[(target + 1)*(cur_game->cols) + n] == cur_game->cells[target*(cur_game->cols) + n]){
              // combine the rows
              cur_game->cells[(target + 1)*(cur_game->cols) + n] += cur_game->cells[target*(cur_game->cols) + n];
              cur_game->cells[target*(cur_game->cols) + n] = -1;

              // add the combo to the score
              cur_game->score += cur_game->cells[(target + 1)*(cur_game->cols) + n];

              // mark the cell as combined
              combined[(target + 1)*(cur_game->cols) + n] = 1;
            }
          }
          // if there was no tile movement, check for a merge
          else{
            if(combined[(m + 1)*(cur_game->cols) + n] == 0 && cur_game->cells[(m + 1)*(cur_game->cols) + n] == cur_game->cells[m*(cur_game->cols) + n]){
              // increment the change counter
              changes++;
              // merge the tiles
              cur_game->cells[(m + 1)*(cur_game->cols) + n] += cur_game->cells[m*(cur_game->cols) + n];
              cur_game->cells[m*(cur_game->cols) + n] = -1;

              // add the combo to the score
              cur_game->score += cur_game->cells[(m + 1)*(cur_game->cols) + n];

              // mark the cell as combined
              combined[(m + 1)*(cur_game->cols) + n] = 1;
            }
          }
        }
      }
    }

    // if any movements, return 1
    if(changes > 0){
      return 1;
    }
    // if no legal moves, return 0
    else{
      return 0;
    }
};

int move_a(game * cur_game) //slide left
{
    //YOUR CODE STARTS HERE
    // array corresponding to game board, element is 1 if current cell has been combined
    // this turn, and 0 if it has not
    int combined[(cur_game->rows) * (cur_game->cols)];
    // initialize array to 0
    int i, j;   //row and  column indices
    for(i = 0; i < cur_game->rows; i++){
      for(j = 0; j < cur_game->cols; j++){
        combined[i*(cur_game->cols) + j] = 0;
      }
    }

    int m, n; // row and column indices of the game board
    int x;  // extra index for finding the target row
    int target; // the target row (highest empty row)

    int changes = 0; // keep track of the number of changes from the move
    int move = 0; // flag to indicate a move can occur

    // go though board for tile moves
    //iterate through row
    for(m = 0; m < cur_game->rows; m++){
      // iterate through each column in the row
      for(n = 1; n < cur_game->cols; n++){
        // check for a filled tile
        if(cur_game->cells[m*(cur_game->cols) + n] != -1){
          for(x = 0; x < n; x++){
            // look for the leftmost empty cell
            if(cur_game->cells[m*(cur_game->cols) + x] == -1){
              target = x;
              move = 1;
              break;
            }
          }

          // if the target cell is empty, swap the filled cell with the target
          if(move == 1){
            // increment change counter
            changes++;
            // move the tile left
            cur_game->cells[m*(cur_game->cols) + target] = cur_game->cells[m*(cur_game->cols) + n];
            cur_game->cells[m*(cur_game->cols) + n] = -1;

            // reset the flag
            move = 0;

            // check if a merge is possible
            if(target > 0 && combined[m*(cur_game->cols) + (target - 1)] == 0 && cur_game->cells[m*(cur_game->cols) + (target - 1)] == cur_game->cells[m*(cur_game->cols) + target]){
              // combine the rows
              cur_game->cells[m*(cur_game->cols) + (target - 1)] += cur_game->cells[m*(cur_game->cols) + target];
              cur_game->cells[m*(cur_game->cols) + target] = -1;

              // add the combo to the score
              cur_game->score += cur_game->cells[m*(cur_game->cols) + (target - 1)];

              // mark the cell as combined
              combined[m*(cur_game->cols) + (target - 1)] = 1;
            }
          }
          // if there was no tile movement, still check for a merge
          else{
            if(combined[m*(cur_game->cols) + (n - 1)] == 0 && cur_game->cells[m*(cur_game->cols) + (n - 1)] == cur_game->cells[m*(cur_game->cols) + n]){
              // increment the change counter
              changes++;
              // merge the tiles
              cur_game->cells[m*(cur_game->cols) + (n - 1)] += cur_game->cells[m*(cur_game->cols) + n];
              cur_game->cells[m*(cur_game->cols) + n] = -1;

              // add the combo to the score
              cur_game->score += cur_game->cells[m*(cur_game->cols) + (n - 1)];

              // mark the cell as combined
              combined[m*(cur_game->cols) + (n - 1)] = 1;
            }
          }
        }
      }
    }

    // if any movements, return 1
    if(changes > 0){
      return 1;
    }
    // if no legal moves, return 0
    else{
      return 0;
    }
};

int move_d(game * cur_game){ //slide to the right
    //YOUR CODE STARTS HERE
    // array corresponding to game board, element is 1 if current cell has been combined
    // this turn, and 0 if it has not
    int combined[(cur_game->rows) * (cur_game->cols)];
    // initialize array to 0
    int i, j;   //row and  column indices
    for(i = 0; i < cur_game->rows; i++){
      for(j = 0; j < cur_game->cols; j++){
        combined[i*(cur_game->cols) + j] = 0;
      }
    }

    int m, n; // row and column indices of the game board
    int x;  // extra index for finding the target row
    int target; // the target row (highest empty row)

    int changes = 0; // keep track of the number of changes from the move
    int move = 0; // flag to indicate a move can occur

    // go though board for tile moves
    //iterate through row
    for(m = cur_game->rows - 1; m >= 0; m--){
      // iterate through each column from the right in the row
      for(n = cur_game->cols - 2; n >= 0; n--){
        // check for a filled tile
        if(cur_game->cells[m*(cur_game->cols) + n] != -1){
          for(x = cur_game->cols - 1; x > n; x--){
            // look for the rightmost empty cell
            if(cur_game->cells[m*(cur_game->cols) + x] == -1){
              target = x;
              move = 1;
              break;
            }
          }

          // if the target cell is empty, swap the filled cell with the target
          if(move == 1){
            // increment change counter
            changes++;
            // move the tile left
            cur_game->cells[m*(cur_game->cols) + target] = cur_game->cells[m*(cur_game->cols) + n];
            cur_game->cells[m*(cur_game->cols) + n] = -1;

            // reset the flag
            move = 0;

            // check if a merge is possible
            if(target < (cur_game->cols - 1) && combined[m*(cur_game->cols) + (target + 1)] == 0 && cur_game->cells[m*(cur_game->cols) + (target + 1)] == cur_game->cells[m*(cur_game->cols) + target]){
              // combine the rows
              cur_game->cells[m*(cur_game->cols) + (target + 1)] += cur_game->cells[m*(cur_game->cols) + target];
              cur_game->cells[m*(cur_game->cols) + target] = -1;

              // add the combo to the score
              cur_game->score += cur_game->cells[m*(cur_game->cols) + (target + 1)];

              // mark the cell as combined
              combined[m*(cur_game->cols) + (target - 1)] = 1;
            }
          }
          // if there was no tile movement, still check for a merge
          else{
            if(combined[m*(cur_game->cols) + (n + 1)] == 0 && cur_game->cells[m*(cur_game->cols) + (n + 1)] == cur_game->cells[m*(cur_game->cols) + n]){
              // increment the change counter
              changes++;
              // merge the tiles
              cur_game->cells[m*(cur_game->cols) + (n + 1)] += cur_game->cells[m*(cur_game->cols) + n];
              cur_game->cells[m*(cur_game->cols) + n] = -1;

              // add the combo to the score
              cur_game->score += cur_game->cells[m*(cur_game->cols) + (n + 1)];

              // mark the cell as combined
              combined[m*(cur_game->cols) + (n + 1)] = 1;
            }
          }
        }
      }
    }

    // if any movement, return 1
    if(changes > 0){
      return 1;
    }
    // if there were no legal moves, return 0
    else{
      return 0;
    }
};

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE
    int i, j;   // row and column indices

    // create a copy of the current game
    game * game_copy = malloc(sizeof(game));
    game_copy->cells = malloc(cur_game->rows*cur_game->cols*sizeof(cell));

    game_copy->rows = cur_game->rows;
    game_copy->cols = cur_game->cols;

    for(i = 0; i < cur_game->rows; i++){
      for(j = 0; j < cur_game->cols; j++){
        game_copy->cells[i*(cur_game->cols) + j] = cur_game->cells[i*(cur_game->cols) + j];
      }
    }

    // try each movement on the copy
    if(move_w(game_copy) == 0 && move_d(game_copy) == 0 && move_a(game_copy) == 0 && move_s(game_copy) == 0){
      // if all movements return 0, there are no legal movements left
      // free memory spcae alloocated to the copy
			free(game_copy->cells);
			free(game_copy);
			return 0;
    }
		// free memory space allocated to the copy
		free(game_copy->cells);
		free(game_copy);
    return 1;
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{

	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    if (cell_ptr == NULL){
        printf("Bad Cell Pointer.\n");
        exit(0);
    }


	//check for an empty cell
	int emptycheck = 0;
	int i;

	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}

    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game)
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;

	printf("\n\n\nscore:%d\n",cur_game->score);


	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char


    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  ");
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }

	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char

    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;

    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}

		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		}

		remake_game(&cur_game,rows,cols);

		move_success = 1;

    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }




    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game);
		 print_game(cur_game);
    }

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
