/*
 *
 * prog5.c - source file adapted from UIUC ECE198KL Spring 2013 Program 4
 *           student code -- GOLD VERSION by Steven S. Lumetta
 */


/*
 * The functions that you must write are defined in the header file.
 * Blank function prototypes with explanatory headers are provided
 * in this file to help you get started.
 */

 /* Anna Miller (annam4)
  * ECE 220 - BD2
  * 10/10/19
  *
  * The program prompts the user for a seed value in main.c. The set_seed function then uses sscanf to interpret
  * the input. If more than one integer is read, the string is invalid, and the program returns 0/ OTherwise,
  * srand(seed) is called, and the function returns 1. The next function called by main.c is start_game. This
  * function generates the 4 integers of the solution code and sets the current guess_number to 1. The solution
	* code integers are in the range [1, 8]. This is accomplished by calculating each digit as rand() % 8 + 1.
  * The integers are also set to their respective dereferenced pointers. After the game is set up, main.c wiil
  * will read strings entered by the user and call make_guess. This function will use sscanf to ensure the user i
	* nput follows the proper format of "%d %d %d %d". It will also check that the each guessed integer is within
	* the valid range (0 < guess < 9). If these conditions are satisfied, my function will set the guesses to the
	* appropriate pointers. My function saves them in an array (user_guess[]). It also uses a solution code
	* array (solution_code[]) and an array that holds information on whether each integer is paired (pair[]). The
	* function checks if the user had any perfect or misplaced matches. First it checks the corresponding indices
	* of user_guess[] and solution_code[] for a perfect match using a for loop. If user_guess[i] = solution_code[i],
	* pair[i] = 2 and the counter for perfect matches is incremented. Next, the first unpaired index is compared to
	* the other 3. If user_guess[i] = solution_code[i] and pair[i] < 1, the function records a misplaced match, and
	* the solution code integer is marked as pair = 1 (ex. if user_guess[i] = solution_code[i+1], pair[i+1 = 1).
  * The process is continued for any other unmatched digits. After comparing the guess and solution, a feedback
	* message is printed with number of perfect and misplaced matches, and the function returns 1. If there was an
	* invalid input string, the function returns 0.
  */

#include <stdio.h>
#include <stdlib.h>

#include "prog5.h"


/*
 * You will need to keep track of the solution code using file scope
 * variables as well as the guess number.
 */

static int guess_number;
static int solution1;
static int solution2;
static int solution3;
static int solution4;


/*
 * set_seed -- This function sets the seed value for pseudorandom
 * number generation based on the number the user types.
 * The input entered by the user is already stored in the string seed_str by the code in main.c.
 * This function should use the function sscanf to find the integer seed value from the
 * string seed_str, then initialize the random number generator by calling srand with the integer
 * seed value. To be valid, exactly one integer must be entered by the user, anything else is invalid.
 * INPUTS: seed_str -- a string (array of characters) entered by the user, containing the random seed
 * OUTPUTS: none
 * RETURN VALUE: 0 if the given string is invalid (string contains anything
 *               other than a single integer), or 1 if string is valid (contains one integer)
 * SIDE EFFECTS: initializes pseudo-random number generation using the function srand. Prints "set_seed: invalid seed\n"
 *               if string is invalid. Prints nothing if it is valid.
 */
int
set_seed (const char seed_str[])
{
//    If there is no integer, seed will not be set. If something else is read after the integer, it will go into "post".
//    The return value of sscanf indicates the number of items read succesfully.
//    When the user has typed in only an integer, only 1 item should be read sucessfully.
//    Check that the return value is 1 to ensure the user enters only an integer.

		int seed; 		// seed value from integer typed
		char post[2]; // hold anything that may have been tyoed after the integer

		// use sscanf to ensure proper format typed by user and to set seed to the integer entered
		// if 1 integer read, call srand(seed) and return 1
		if(sscanf(seed_str, "%d%1s", &seed, post) == 1){
				srand(seed);
				return 1;
		}
		// otherwise print error message and return 0
		else{
				printf("set_seed: invalid seed\n");
				return 0;
		}
}


/*
 * start_game -- This function is called by main.c after set_seed but before the user makes guesses.
 *               This function creates the four solution numbers using the approach
 *               described in the wiki specification (using rand())
 *               The four solution numbers should be stored in the static variables defined above.
 *               The values at the pointers should also be set to the solution numbers.
 *               The guess_number should be initialized to 1 (to indicate the first guess)
 * INPUTS: none
 * OUTPUTS: *one -- the first solution value (between 1 and 8)
 *          *two -- the second solution value (between 1 and 8)
 *          *three -- the third solution value (between 1 and 8)
 *          *four -- the fourth solution value (between 1 and 8)
 * RETURN VALUE: none
 * SIDE EFFECTS: records the solution in the static solution variables for use by make_guess, set guess_number
 */
void
start_game (int* one, int* two, int* three, int* four)
{
    //your code here
		// set the solution variables as well as the values corresponding to pointers one, two, three, and four
		// each value of the solution code should be between 1 and 8
		*one = solution1 = rand() % 8 + 1;
		*two = solution2 = rand() % 8 + 1;
		*three = solution3 = rand() % 8 + 1;
		*four = solution4 = rand() % 8 + 1;

		// set guess_number to 1 for the first guess
		guess_number = 1;
}

/*
 * make_guess -- This function is called by main.c after the user types in a guess.
 *               The guess is stored in the string guess_str.
 *               The function must calculate the number of perfect and misplaced matches
 *               for a guess, given the solution recorded earlier by start_game
 *               The guess must be valid (contain only 4 integers, within the range 1-8). If it is valid
 *               the number of correct and incorrect matches should be printed, using the following format
 *               "With guess %d, you got %d perfect matches and %d misplaced matches.\n"
 *               If valid, the guess_number should be incremented.
 *               If invalid, the error message "make_guess: invalid guess\n" should be printed and 0 returned.
 *               For an invalid guess, the guess_number is not incremented.
 * INPUTS: guess_str -- a string consisting of the guess typed by the user
 * OUTPUTS: the following are only valid if the function returns 1 (A valid guess)
 *          *one -- the first guess value (between 1 and 8)
 *          *two -- the second guess value (between 1 and 8)
 *          *three -- the third guess value (between 1 and 8)
 *          *four -- the fourth color value (between 1 and 8)
 * RETURN VALUE: 1 if the guess string is valid (the guess contains exactly four
 *               numbers between 1 and 8), or 0 if it is invalid
 * SIDE EFFECTS: prints (using printf) the number of matches found and increments guess_number(valid guess)
 *               or an error message (invalid guess)
 *               (NOTE: the output format MUST MATCH EXACTLY, check the wiki writeup)
 */
int
make_guess (const char guess_str[], int* one, int* two,
	    int* three, int* four)
{
//  One thing you will need to read four integers from the string guess_str, using a process
//  similar to set_seed
//  The statement, given char post[2]; and four integers w,x,y,z,
//  sscanf (guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post)
//  will read four integers from guess_str into the integers and read anything else present into post
//  The return value of sscanf indicates the number of items sucessfully read from the string.
//  You should check that exactly four integers were sucessfully read.
//  You should then check if the 4 integers are between 1-8. If so, it is a valid guess
//  Otherwise, it is invalid.

		int w, x, y, z; 	// hold the digits of the guess
		int result; 			// hold the return value of sscanf
		char post[2];			// hold anything that follows the guess and would invalidate the string
		// array to hold the solution code
		int solution_code[4] = {solution1, solution2, solution3, solution4};

		// use sscanf to get the digits of the guess
		result = sscanf(guess_str, "%d %d %d %d%1s", &w, &x, &y, &z, post);
		// set the values of pointers one through four to the respecitve variables/guess digits
		*one = w;
		*two = x;
		*three = y;
		*four = z;

		// check if the inout string is valid and that the integers entered are between 1 and 8
		if(result == 4 && w > 0 && w < 9 && x > 0 && x < 9 && y > 0 && y < 9 && z > 0 && z < 9){
				int perf_match = 0; 			// hold the number of perfect matches
				int mis_match = 0; 				// hold the number of misplaced matches

				int user_guess[] = {w, x, y, z}; 	// array that holds the guess variables in order

				// array with information on matched pairs for each digit
				// if pair[i] = 0, there is no match for solution digit i
				// if pair[i] = 1, there is a misplaced matched
				// if pair[i] = 2, there is a perfect match
 				int pair[] = {0, 0, 0, 0};

				// check each digit of the guess against the solution for a perfect match
				for(int i = 0; i < 4; i++){
						if(user_guess[i] == solution_code[i]){
								perf_match++;
								pair[i] = 2;
						}
				}

				// check for misplaced matches
				for(int j = 0; j < 4; j++){
						// only check if the current digit is not paired
						if(pair[j] < 2){
								// check each digit against the other unpaired digits
								if(j != 0 && pair[0] < 1 && user_guess[j] == solution_code[0]){
										mis_match++;
										pair[0] = 1;
								}
								else if(j != 1 && pair[1] < 1 && user_guess[j] == solution_code[1]){
										mis_match++;
										pair[1] = 1;
								}
								else if(j != 2 && pair[2] < 1 && user_guess[j] == solution_code[2]){
										mis_match++;
										pair[2] = 1;
								}
								else if(j != 3 && pair[3] < 1 && user_guess[j] == solution_code[3]){
										mis_match++;
										pair[3] = 1;
								}
						}
				}

				// print feedback
				printf("With guess %d, you got %d perfect matches and %d misplaced matches.\n", guess_number, perf_match, mis_match);
				// increment guess_number
				guess_number++;
				return 1;
		}
		// invalid string
		else{
				// print invalid message
				printf("make_guess: invalid guess\n");
				return 0;
		}
}
