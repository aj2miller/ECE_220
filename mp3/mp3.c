/* Anna Miller (annam4)
 * BD2
 * 9/26/19
 *
 * A for loop is required for the given equation because the coefficient is calculated
 * by multiplying the current coefficient by (n + i)/i where n = row index and
 * i = position of the coefficient in the row (0 <== i <= n). Since dividing by 0
 * will cause a problem, an if statement is used. If i = 0, c is automatically set to 1.
 * In all other cases, the calculation from the given equation will occur.
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
  int row, i;				/* row index and for loop integer */ 
	unsigned long c;  /* coefficient to be printed*/

  printf("Enter a row index: ");
  scanf("%d",&row);

  // Write your code here//
	/* set up a for loop for i = 0 until i = row index
	 * i is incremented by 1 */
	for (i = 0; i < row + 1; i++){
		/* if i = 0, the coefficient is 1 */
		if(i==0){
			c = 1;
		}
		/* for all other cases, the current coefficient is multiplied by (n + i)/i */
		else{
			c = c*(row + 1 - i)/i;
		}
		/* print the coefficients seperated by a space */
		printf("%lu ", c);
	}

  return 0;
}
