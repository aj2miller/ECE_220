/* Anna Miller (annam4)
 * ECE 220 - BD2
 * 10/17/19
 *
 * The function countLiveNeighbor will iterate through (col - 1) to (col + 1) in rows from
 * (row - 1) to (row + 1) where the current cell is in row = row and column = col and check
 * for live neighbors (cell = 1). It skips any row or column that is less than 0 or greater
 * than or equal to the board dimensions. This prevents the program from trying to access
 * the array at a location out of bounds. It also skips the current cell. It returns an
 * integer value for the number of live neighbors counted.
 * The function updateBoard stores the original board in a temporary array. It then
 * iterates through the board and calls countLiveNeighbor for each cell. Conditions are
 * checked to apply changes and the game board is updated accordingly. Saving the
 * original board ensures that changes are based off the original board and not the changes
 * being updated.
 * The aliveStable function returns 0 if the board is unstable and 1 if the board is stable.
 * The game board is stored in a temporary array, and the temporary array is updated using
 * updateBoard. The updated temporary board is then compared to the original board. If any
 * changes were made to the board, the function returns 0. If no changes were detected, the
 * board is stable, and the function returns 1.
 *
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
	int live_nbrs = 0;		/* set the number of live neighbors to 0 */
	int i;		/* current row index */
	int j;			/* current col index */

	/* iterate through all neighboring rows of the active cell */
	for(i = row - 1; i < row + 2; i++){
		if(i >= 0 && i < boardRowSize){ 	/* skip if i is not on the board */
			/* iterate through all neighboring columns of the active cell */
			for(j = col - 1; j < col + 2; j++){
				if(j >= 0 && j < boardColSize){ 	/* skip if j is not on the board */
					if(i == row && j == col){ 	/* skip if on the active cell */
						live_nbrs = live_nbrs;
					}
					else if(board[i * boardColSize + j]){ 	/* check if neighbor = 1 */
						live_nbrs++; 	/* increment live_nbrs */
					}
				}
			}
		}
	}

	/* return the number of live neighbors */
	return live_nbrs;
}
/*
 * Update the game board to the next step.
 * Input:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
	int curRow, curCol; 	/* index of the current row and column */
	int live_nbrs; 		/* number of live neighbors to the active cell */
  int oldBoard[boardRowSize * boardColSize]; 	/* hold the original board */

	/* iterate through the columns in each row of the board */
	for(curRow = 0; curRow < boardRowSize; curRow++){
		for(curCol = 0; curCol < boardColSize; curCol++){
			/* store the original board in oldBoard[] */
			oldBoard[curRow * boardColSize + curCol] = board[curRow * boardColSize + curCol];
		}
	}

	/* iterate through the rows and columns of the game board */
	for(curRow = 0; curRow < boardRowSize; curRow++){
		for(curCol = 0; curCol < boardColSize; curCol++){
			/* get the number of live neighbors for the active cell */
			live_nbrs = countLiveNeighbor(oldBoard, boardRowSize, boardColSize, curRow, curCol);

			/* if the cell is dead and has exactly 3 live neighbors, bring it back to life */
			if(live_nbrs == 3 && oldBoard[curRow * boardColSize + curCol] == 0){
				board[curRow * boardColSize + curCol] = 1;
			}
			/* if the cell has less than 2 or greater than 3 live neighbors and is currently alive, kill it */
			else if((live_nbrs > 3 || live_nbrs < 2) && oldBoard[curRow * boardColSize + curCol]){
				board[curRow * boardColSize + curCol] = 0;
			}
		}
	}
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */
int aliveStable(int* board, int boardRowSize, int boardColSize){
	int i, j; 	/* i is the current row, j is the current column*/
	int tempBoard[boardRowSize * boardColSize]; 	/* temporary board to check for changes */

	/* iterate through each column in each row of the board */
	for(i = 0; i < boardRowSize; i++){
		for(j = 0; j < boardColSize; j++){
			/* set the temporary board to the actual game board */
			tempBoard[i * boardColSize + j] = board[i * boardColSize + j];
		}
	}
	/* update the temporary board to check for changes */
	updateBoard(tempBoard, boardRowSize, boardColSize);

	/* iterate through the rows and columns*/
	for(i = 0; i < boardRowSize; i++){
		for(j = 0; j < boardColSize; j++){
			/* compare the updated temmporary board to the original board, and if a change is detected, return 0 */
			if(tempBoard[i * boardColSize + j] != board[i * boardColSize + j]){
				return 0;
			}
		}
	}
	/* if no changes detected, return 1 */
	return 1;
}
