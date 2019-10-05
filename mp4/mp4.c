/* Anna Miller (annam4)
 * ECE 220 - BD2
 * 10/5/19
 * 
 * The program prompts the user for two positive integers (a and  b) where a < b, and determines
 * if any semi-primes exist in the range [a, b]. The main function calls the print_semiprimes(a, b) function.
 * This function loops through all integers (n) in [a, b]. For each n value, it loops through integers (k) in the
 * range [2, n) and uses the is_prime(number) function to check if k is a prime number, if k is a factor of n,
 * and if n/k is prime. If these 3 conditions are satisfied, n is a semi-prime. If n is a semi-prime, the
 * semi-prime counter is incremented, n is printed, and the program moves on to the next value n. Otherwise, 
 * the the loop will continue. If any semi-primes exist in [a, b], the function returns 1. Otherwise, it will return 0.
 * The is_prime(number) function iterates through integers from 2 to number/2. IT checks to see if number is evenly
 * divisble by any of these values by using the modulous operator. If number is prime, it returns 1. Otherwise, it wil return 0.
 */


#include <stdlib.h>
#include <stdio.h>


int is_prime(int number);
int print_semiprimes(int a, int b);


int main(){   
   int a, b;	/* input variables */

	 /* Prompt user to enter two positive integers and save them as a and b (a must be less than b) */
   printf("Input two numbers: ");
   scanf("%d %d", &a, &b);

	 /* if a or b is negative, alert user, return 1, and stop program */
   if( a <= 0 || b <= 0 ){
     printf("Inputs should be positive integers\n");
     return 1;
   }

	 /* if a > b, alert user, return 1, and stop program */
   if( a > b ){
     printf("The first number should be smaller than or equal to the second number\n");
     return 1;
   }

   // TODO: call the print_semiprimes function to print semiprimes in [a,b] (including a and b) //
	print_semiprimes(a, b);

	return 0;
}


/*
 * TODO: implement this function to check the number is prime or not.
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1
 */
int is_prime(int number)
{
	int i;		/* for loop index */

	/* loop i through all values from 2 to the input number/2 */
	for(i = 2; i <= number/2; i++){
		/* if the input number is evenly divisible by i, it is not prime */
		if(number % i == 0){		
			return 0;				/* if number is not prime, return 0 */
		}
	}
	/* if there is no value between 2 and number/2 that number can be divided by, number is prime */
	return 1;						/* if number is prime, return 1 */
}


/*
 * TODO: implement this function to print all semiprimes in [a,b] (including a, b).
 * Input   : a, b (a should be smaller than or equal to b)
 * Return  : 0 if there is no semiprime in [a,b], else 1
 */
int print_semiprimes(int a, int b)
{
	int n, k;		/* for loop indices */
	int semi_count = 0;		/* counter for semi_primes in range [a, b] initialized to 0 */

	/* loop n through all numbers from [a, b] */
	for(n = a; n < b+1; n++){
		/* loop k through values from [2, n - 1] */
		for(k = 2; k < n; k++){
			/* in order to be a semi-prime, k must be prime, n % k must equal 0, and n/k must be prime */
			if(is_prime(k) && n % k == 0 && is_prime(n/k)){
				semi_count++;			/* increment the semi-prime counter */
				printf("%d ", n); /* print the semi-prime n */
				break;				/* break out of loop and go to the next value of n to avoid duplicates */
			}		
		}
	}		
	/* if any semi-primes existed in the range [a, b], return 1 */
	if(semi_count > 0){
		return 1;
	}
	/* if no semi-primes existed in the range [a, b], return 0 */
	else{
		return 0;
	}
}



