; Anna Miller (annam4)
; 9/19/19
; This program calculates expressions based on input in post-fix notation. If the input is a number, 
; it is pushed to a stack. If the input is an operator, 2 values are popped from the stack, the 
; corresponding operation is performed, and the result is pushed to the stack. If the input is an equal sign,
; the result is converted to hexadecimal and printed. The program ignores spaces, but if an invalid character 
; or expression exists, the program will print "Invalid Expression". (See specific subroutines for corresponding register tables).

; SUBROUTINES:
; EVALUATE - reads and exhoes input to screen hen evaluates input, calls other subroutines based on 
;						 the character that was input (INPUT: R0 = character input, OUTPUT: R5 = result of operations)
; PRINT_HEX - if valid expression, converts result from stack  to hex and prints it to screen,
;							if invalid expression, prints "Invalid Expression" 
;							(INPUT: R5 = result from stack, R6 = 0 (valid expression), 1 (invalid expression))
; PUSH - (provided) pushs value to stack (INPUT: R0 = value pushed, OUTPUT: R5 = 0 (successful push), 1 (overflow))
; POP - (provided) pops value from stack (OUTPUT: R0 = value popped, R5 = 0 (successful pop), 1 (underflow))

; OPERATION SUBROUTINES (INPUT: R3 = A, R4 = B, OUT: R0):
; PLUS - adds the A+B
; MIN - subtracts A-B
; MUL - multiplies A*B
; DIV - divides A/B
; EXP - performs A^B

.ORIG x3000
	
;your code goes here

; evaluate all input, call operations, print to screen
JSR EVALUATE

HALT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; if the expression was invald, print "Invalid Expression"
; if valid, convert the number in R5 to hexadecimal in ASCII and print to the screen
; REGISTER TABLE:
; R1 - hex digit counter
; R2 - temporary register for condition checks
; R3 - bit counter
; R5 - value to print in hexadecimal
PRINT_HEX
		ST R1, PRINT_SaveR1		; save R1
		ST R2, PRINT_SaveR2		; save R2
		ST R3, PRINT_SaveR3		; save R3
		ST R5, PRINT_SaveR5		; save R5
		ST R7, PRINT_SaveR7		; save R7
		
; check if expression was valid
		ADD R6, R6, #0				; R6 = 0 > valid, R6 =1 > invalid
		BRz CONVERT

		LEA R0, INVAL_EXP			; if invalid, load starting address of string
		PUTS									; print "Invalid Expression"
		BR PRINT_DONE					; done

; if expression valid, convert to hex
CONVERT
		AND R1, R1, #0				; initialize R1 (hex digit counter) to 0

CHECK_DIG
		ADD R2, R1, #-4				; check if 4 hex digits have been printed
		BRzp PRINT_DONE				; if 4 printed, return
		AND R0, R0, #0				; set R0 to 0

		AND R3, R3, #0				; initialize R3 (bit counter) to 0
CHECK_BIT
		ADD R2, R3, #-4				; check if all bits have been read
		BRzp PRE_PRINT				; if all are read, prepare to print
		
; left shift the value in R0
		ADD R0, R0, R0			
		ADD R5, R5, #0				; check if R5 is a negative value
		BRzp ZERO_BIT					; if not negative, move on to next bit
		ADD R0, R0, #1				; if negative, MSB of R3 is 1 so add 1 to R0

ZERO_BIT
		ADD R5, R5, R5				; left shift value in R5
		ADD R3, R3, #1				; increment bit counter
		BR CHECK_BIT					; branch to check if all bits are read

PRE_PRINT
		ADD R2, R0, #-9				; subtract 9 from R0 to determine if value is a numerical hex digit
		BRnz PRINT_NUM				; if difference in R2 is less than or equal to 0, hex digit is 0-9
		LD R0, ALPHA_OFFSET		; load R0 with alphabetic offset
		BR PRINT

PRINT_NUM
		LD R2, NUM_OFFSET			; load R2 with offset for '0'

PRINT
		ADD R0, R0, R2				; add the appropriate offset to get the hex character
		OUT										; print to screen
		ADD R1, R1, #1				; increment digit counter
		BR CHECK_DIG					; branch to check if 4 digits have been printed

PRINT_DONE
		LD R1, PRINT_SaveR1		; restore R1
		LD R2, PRINT_SaveR2		; restore R2
		LD R3, PRINT_SaveR3		; restore R3
		LD R5, PRINT_SaveR5		; restore R5
		LD R7, PRINT_SaveR7		; restore R7

		RET		

; data for printing
ALPHA_OFFSET .FILL x0040	; offset for alphabetic character
NUM_OFFSET .FILL x0030		; offset between number and it's ASCII value
; string to be printed on invalid expressions
INVAL_EXP .STRINGZ "Invalid Expression"

; store initical contents of  registers
PRINT_SaveR1 .BLKW #1
PRINT_SaveR2 .BLKW #1
PRINT_SaveR3 .BLKW #1
PRINT_SaveR5 .BLKW #1
PRINT_SaveR7 .BLKW #1 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; read and echo the input, then evaluate the input
; numbers are pushed to the stack, operators trigger an operation, and the equal sign will print the result
; spaces are ignored, but all other input is invalid
; REGISTER TABLE:
; R0 - character input from keyboard
; R5 - current numerical output
; R1 - temporary register for checking ASCII values
; R2 - hold various negative ASCII values
; R6 - expression validity status (1 if invalid expression, 0 if valid)

EVALUATE
;your code goes here
		ST R7, EVAL_SaveR7 ; save R7

GET_CHAR
		AND R6, R6, #0			; initialize R6 to 0
		GETC								; read and store input in R0
		OUT									; print input

; if input character is a space, ignore and move on
		LD R2, SPACE_NEG		; load R2 with negative of ' ' ASCII value
		ADD R1, R0, R2			; R1 = difference between character and ' '
		BRz GET_CHAR				; get next character if ' '

; if input character is a number, subtract ASCII offset and push to stack
		LD R2, ZERO_NEG			; load R2 with the negative of '0' ASCII value
		ADD R1, R0, R2			; R1 = difference between character and '0'
		BRn CONT_CHECK			; if character is less than x0030 ('0'), check if operand
		ADD R1, R1, #-9			; R1 = character - ASCII offset -9
		BRp CONT_CHECK			; if ASCII character is greater than x0039 ('9'), check if operand
; else, push the number to stack
		ADD R0, R1, #9			; add 9 back to R1 to get the character minus the ASCII offset and copy it to R0
		JSR PUSH						; push R0 to stack
		BR GET_CHAR					; get next character

; if input character is an operand, pop from stack
CONT_CHECK
; check for '+'
		LD R2, PLUS_NEG			; load R2 with negative of '+' ASCII value
		ADD R1, R0, R2			; R1 = difference between character and '+'
		BRnp NEXT_1					; continue checking if not equal

		JSR POP							; pop 1st operand
		ADD R4, R0, #0			; copy R0 to R4

		JSR POP							; pop 2nd operand
		ADD R3, R0, #0			; copy R0 to R3

		ADD R5, R5, #0			; check for underflow
		BRp INVALID			

		JSR PLUS						; perform addition
		JSR PUSH						; push result to stack
		
		BR GET_CHAR					; get next character

NEXT_1
; check for  '-'
		LD R2, MIN_NEG			; load R2 with negative of '-' ASCII value
		ADD R1, R0, R2			; R1 = difference between character and '-'
		BRnp NEXT_2

		JSR POP							; pop 1st operand
		ADD R4, R0, #0			; copy R0 to R4

		JSR POP							; pop 2nd operand
		ADD R3, R0, #0			; copy R0 to R3

		ADD R5, R5, #0			; check for underflow
		BRp INVALID

		JSR MIN							; perform subtraction
		JSR PUSH						; push result to stack

		BR GET_CHAR					; get next character

NEXT_2
; check for '*'
		LD R2, MUL_NEG			; load R2 with negative of '*' ASCII value
		ADD R1, R0, R2			; R1 = difference between character and '*'
		BRnp NEXT_3

		JSR POP							; pop 1st operand
		ADD R4, R0, #0			; copy R0 to R4

		JSR POP							; pop 2nd operand
		ADD R3, R0, #0			; copy R0 to R3
	
		ADD R5, R5, #0			; check for underflow
		BRp INVALID

		JSR MUL							; perform multiplication
		JSR PUSH						; push result to stack

		BR GET_CHAR					; get next character

NEXT_3
; check for '/'
		LD R2, DIV_NEG			; load R2 with negative of '/' ASCII value
		ADD R1, R0, R2			; R1 = difference between character and '/'
		BRnp NEXT_4

		JSR POP							; pop 1st operand
		ADD R4, R0, #0			; copy R0 to R4

		JSR POP							; pop 2nd operand
		ADD R3, R0, #0			; copy R0 to R3

		ADD R5, R5, #0			; check for underflow
		BRp INVALID

		JSR DIV 						; perform division
		JSR PUSH						; push result to stack

		BR GET_CHAR					; get next character

NEXT_4
; check for '^'
		LD R2, EXP_NEG			; load R2 with negative of '6' ASCII value
		ADD R1, R0, R2			; R1 = difference between character and '^'
		BRnp CHECK_EQUAL

		JSR POP							; pop 1st operand
		ADD R4, R0, #0			; copy R0 to R4

		JSR POP							; pop 2nd operand
		ADD R3, R0, #0			; copy R0 to R3

		ADD R5, R5, #0			; check for underflow
		BRp INVALID

		JSR EXP 						; perform power operation
		JSR PUSH						; push result to stack

		BR GET_CHAR					; get next character

CHECK_EQUAL
; if character is a '=', print result from stack
		LD R2, EQUAL_NEG		; load R2 with the negative of '=' ASCII value
		ADD R1, R0, R2			; R1 = differnce between character and '='
		BRnp INVALID				; if character not yet identified, it is invalid	

; else, chck if a valid expression exists in stack
		JSR POP							; pop final result from stack
		ADD R5, R5, #0			; check R5
		BRp INVALID					; if underflow occurred (R5 = 1), print "Invalid Expression"
		ST R0, FIN_RESULT		; store result
		JSR POP							; attempt a 2nd pop to make sure stack was empty
		ADD R5, R5, #0			; if successful pop (R5 = 0), there was an error
		BRz INVALID
		AND R6, R6, #0			; set R6 (validity status of result) to 0
		LD R5, FIN_RESULT		; load the result into R5
		BR GOTO_PRINT				; branch to print

INVALID
		ADD R6, R6, #1			; set R6 (validity status of result) to 1

GOTO_PRINT
		JSR PRINT_HEX				; print result

		LD R7, EVAL_SaveR7 ; restore R7

		RET

; negative of the ASCII values for each operator
PLUS_NEG .FILL xFFD5 ; '+'
MIN_NEG  .FILL xFFD3 ; '-'
MUL_NEG  .FILL xFFD6 ; '*''
DIV_NEG  .FILL xFFD1 ; '/'
EXP_NEG  .FILL xFFA2 ; '^'

; negative of ASCII space
SPACE_NEG .FILL xFFE0 ; ' '
; negative of ASCII '='
EQUAL_NEG .FILL xFFC3
; negative of ASCII offset for '0'
ZERO_NEG .FILL xFFD0

; result of operations
FIN_RESULT .BLKW #1

; saved registers
EVAL_SaveR7 .BLKW #1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
; adds R3 + R4 and outputs the result in R0
; REGISTER TABLE:
; R0 - result
; R3 - 1st operand
; R4 - 2nd operand
PLUS	
;your code goes here
		ST R7, PLUS_SaveR7	; save R7

		ADD R0, R3, R4			; R0 = R3 + R4 

		LD R7, PLUS_SaveR7	; restore R7
	
		RET

PLUS_SaveR7 .BLKW #1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
; subtracts R3 - R4 and outputs the result in R0
; REGISTER TABLE:
; R0 - result
; R3 - 1st operand
; R4 - 2nd operand
MIN	
;your code goes here
		ST R4, MIN_SaveR4 ; save R4
		ST R7, MIN_SaveR7	; save R7

		NOT R4, R4				; find negative of R4
		ADD R4, R4, #1

		ADD R0, R3, R4		; R0 = R3 - R4

		LD R4, MIN_SaveR4 ; restore R4
		LD R7, MIN_SaveR7 ; restore R7

		RET

MIN_SaveR4 .BLKW #1
MIN_SaveR7 .BLKW #1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
; multiplies R3 * R4 and outputs the result in R0
; REGISTER TABLE:
; R0 - result
; R3 - 1st operand
; R4 - 2nd operand
MUL	
;your code goes here
		ST R3, MUL_SaveR3 ; save R3
		ST R4, MUL_SaveR4 ; save R4
		ST R7, MUL_SaveR7 ; save R7
		 
		AND R0, R0, #0		; clear R0
; R3 * R4
; add value in R3 to itself as many times as the value in R4
MUL_LOOP
		ADD R4, R4, #-1		; decrement R4
		BRn DONE_MUL			; if R4 = 0, exit loop
		ADD R0, R0, R3		; add R0 + R3 to itself
		BR MUL_LOOP

DONE_MUL
		LD R3, MUL_SaveR3	; restore R3
		LD R4, MUL_SaveR4 ; restore R4
		LD R7, MUL_SaveR7	; restore R7

		RET
MUL_SaveR3 .BLKW #1
MUL_SaveR4 .BLKW #1
MUL_SaveR7 .BLKW #1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
; divides R3 / R4 and outputs the quotient without the remainder in R0
; REGISTER TABLE:
; R0 - result
; R3 - 1st operand
; R4 - 2nd operand
DIV	
;your code goes here
		ST R3, DIV_SaveR3 ; save R3
		ST R4, DIV_SaveR4 ; save R4
		ST R7, DIV_SaveR7 ; save R7

		AND R0, R0, #0		; clear R0

		NOT R4, R4				; find the negative of R4
		ADD R4, R4, #1

; R3 / R4:
; subtract R4 from R3 until R3 is less than or equal to zero
; return integer portion and discard remainder
DIV_LOOP
		ADD R0, R0, #1		; increment R0 (quotient)
		ADD R3, R3, R4		; subtract R3 = R3 - R4
		BRp DIV_LOOP			; if R3 positive, subtract again
		BRz DIV_DONE			; if R3 = 0, operation complete
		ADD R0, R0, #-1		; if R3 negative, decrement R0 for quotiet

DIV_DONE
		LD R3, DIV_SaveR3 ; restore R3
		LD R4, DIV_SaveR4 ; restore R4
		LD R7, DIV_SaveR7 ; restore R7
		
		RET

DIV_SaveR3 .BLKW #1
DIV_SaveR4 .BLKW #1
DIV_SaveR7 .BLKW #1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
; performs R3 ^ R4 and outputs the result in R0
; REGISTER TABLE:
; R0 - result
; R1 - counter for the number of times R3 is multiplied by itself
; R3 - 1st operand
; R4 - 2nd operand
EXP
;your code goes here
		ST R3, EXP_SaveR3 ; save R3
		ST R4, EXP_SaveR4 ; save R4
		ST R7, EXP_SaveR7 ; save R7
		
		ADD R1, R4, #0		; copy R4 to R1 to serve as a counter
		ADD R4, R3, #0		; copy R3 to R4 to hold original R3

; R3 ^ R4:
; multiply the value of R3 by itself as many times as the value of R4
EXP_LOOP
		ADD R1, R1, #-1		; decrement R1
		BRz DONE_EXP			; if R4 = 0, exit loop
		JSR	MUL						; multiply R3 by the original value in R3 (result in R0)
		ADD R3, R0, #0		; copy the value from R0 to R3
		BR EXP_LOOP				
		

DONE_EXP
		ADD R0, R3, #0		; put result in R0

		LD R3, EXP_SaveR3 ; restore R3
		LD R4, EXP_SaveR4 ; restore R4
		LD R7, EXP_SaveR7 ; restore R7

		RET

EXP_SaveR3 .BLKW #1
EXP_SaveR4 .BLKW #1
EXP_SaveR7 .BLKW #1

;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACk_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;


.END
