// Anna Miller (annam4)
// ECE 220 - BD2
// 10/24/19

// The function is_val_in_row returns true if the value (val) being tried at the cell (i, j)
// has already been used in the row. It iterates though each cloumn of the
// row given by the index i and comparing the values in each cell to val.

// The function is_val_in_col returns true if the value (val) being tried at the cell (i, j)
// has already been used in the column. It iterates through each row of the column given by the
// index j and comparing the values in each cell to val.

// The function is_val_in_3x3_zone returns true if the value (val) being tried at the cell (i, j)
// exists in the 3x3 zone of the board. It calculates the index of the 3x3 zone as if each zone
// was a single cell by diving i and j by  3 and taking the integer result. Nested for loops are then
// used to check each square in the grid By adding the index of 3x3 zone multiplied by 3 to the
// current index of the for loop, the function is able to check each cell in the specific 3x3 grid.
// The function is_val_valid simply uses the above 3 functions to determine if the value can be placed
// in the cell. If it can, this function returns true.

// The function is_board_complete is used to check if there are any empty spaces remaining on the board.
// It iterates through each cell on the board and checks if any of the cells hold 0. If there are no
// empty spaces, the function returns true.

// The function solve_sudoku uses recursion to solve the sudoku board. The base case is satisfied when the
// board is filled. This condition is evaluated using the is_board_complete function. In the recursive case,
// the indices of the first empty cell is found, then the function tries each value 1-9 until it finds a is_val_valid
// value by using the function is_val_valid. If the value is valid, the cell is set to that value and the
// backtracking begins. The function calls itself, and checks if it returns true. If it does not return true,
// the board could not be properly completed, so the current cell is set back to 0, and the next value is checked.
// This continues until the board is completed. If the board cannot be completed, the function returns 0.

#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  int col;  // index of column
  // iterate through each column in the current row and check for the value
  for(col = 0; col < 9; col++){
    // if the value exists in the row, return true
    if(sudoku[i][col] == val){
      return true;
    }
  }

  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
  int row; // index of row
  // iterate through each row in the curent column and check for the value
  for(row = 0; row < 9; row++){
    // if the value exists in the column, return true
    if(sudoku[row][j] == val){
      return true;
    }
  }

  return false;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  int row, col; // row and column index
  int rowzone;  // the row if each 3x3 zone was treated as one cell
  int colzone;  // the column if each 3x3 zone was treated as one cell

  // calculate the "row" and "column" integers
  // these values will be used to ensure the proper 3x3 square is being checked
  rowzone = i/3;
  colzone = j/3;

  // iterate through each cell ofthe 3x3 grid
  for(row = 0; row < 3; row++){
    for(col = 0; col < 3; col++){
      // if the value exists in the 3x3 grid, return true
      if(sudoku[row + (rowzone * 3)][col + (colzone * 3)] == val){
        return true;
      }
    }
  }


  return false;
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  // use the helper functions to check if the value has been used in the curent row, column, or grid
  if(is_val_in_row(val, i, sudoku) || is_val_in_col(val, j, sudoku) || is_val_in_3x3_zone(val, i, j, sudoku)){
    // return false if the value cannot be placed
    return false;
  }

  // return true if the value is valid
  return true;
  // END TODO
}

// Function: is_board_complete
// return true if the board is completely filled
int is_board_complete(const int sudoku[9][9]){
  int i, j;   // row and column index of the board

  for(i = 0; i < 9; i ++){
    for(j = 0; j < 9; j++){
      if(sudoku[i][j] == 0){
        return false;
      }
    }
  }
  // if no empty cells found, return true
  return true;
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
  int i, j; // column index of the board
  int row, col; // row and column index of empty cell
  int val;  // current value being placed
  int found = 0; // holds 1 if an empty sudoku cell is found

  // base case: complete board
  if(is_board_complete(sudoku)){
    return true;
  }

  // recursive case; find an empty box on the board at (i, j)
  else{
    // iterate through each cell of the board
    for(i = 0; i < 9; i++){
      for(j = 0; j < 9; j++){
        // if an empty cell is found break the inner loop
        if(sudoku[i][j] == 0){
          row = i;  // save index of the empty cell row
          col = j;  // save index of the empty cell column
          found = 1;  // flag indicates an empty space has been found
          break;
        }
      }
      // break from the outer loop
      if(found == 1){
        break;
      }
    }
  }

  // check each value 1-9 until finding one that or can be placed in the empty cell, or 9 is reached
  for(val = 1; val <= 9; val++){
    // if there are no conflicts, set the cell to the value
    if(is_val_valid(val, row, col, sudoku)){
      sudoku[row][col] = val;

      // backtracking: function calls itself to try to fill the next empty cell
      // if the call returns true, the board has been completed
      if(solve_sudoku(sudoku)){
        return true;
      }
      // otherwise, set the cell back to 0 and try the next value
      sudoku[row][col] = 0;
    }
  }

  // return 0 if solving the board was impossible
  return 0;
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}
