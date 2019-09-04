;
; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram
	
	; fill the histogram with zeroes 
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop



PRINT_HIST

; Anna Miller (annam4)
; 9/5/19
; ECE 220
; BD2

; Partners: aairab2, daviday2

; This program will go through and print the histogram stored starting at x3F00.
; Each character is printed on a different line along with the number of times that
; character appears in a string. The character count is printed in hex.
; That hex value will be determined by reading the current value of the histogram
; starting at the MSB and copying the information over to R0 one bit at a time. To get 
; the next bit of the hex digit, the value R0 will undergo a left shift. The MSB of the
; original value will be added to the value in R0, and the original value will undergo 
; a left shift. This will continue until all 4 hex digits (all 16 bits) have been copied to R0.

; Register Table
; R0 - holds ASCII value to be printed from OUT
; R1 - pointer to start of histogram
; R2 - holds current character of histogram
; R3 - holds current value of histogram
; R4 - temporary register (additive inverse of Z, differences for counters, offsets, etc.)
; R5 - hex digit counter
; R6 - bit counter
			
			AND R0, R0, #0		; initialize R0 to 0
			LD R1, HIST_ADDR	; set R1 to starting address of histogram
			LD R2, CHAR				; set R2 to ASCII of first character counted
			AND R3, R3, #0    ; initialize R3 to 0
			AND R4, R4, #0		; initialize R4 to 0

CHECK_CHAR
			LD R4, NEG_Z			; set R4 to additive inverse of ASCII 'Z'
			ADD R0, R2, #0		; set R0 to current character of histogram (R2)
			ADD R4, R0, R4		; subtract 'Z' from R0 to check if all characters were printed
			BRp DONE					; if the difference is positive, end program
		
		  OUT								; print character in R0
			LD R0, SPACE			; set R0 to the space character
			OUT								; print ' '

			LDR R3, R1, #0    ; set R3 to current value in histogram

			AND R5, R5, #0    ; set R5 (digit counter) to 0
CHECK_DIG
			ADD R4, R5, #-4		; check if 4 hex digits printed
			BRzp NXT_CHAR     ; if all hex digits printed, go to next character
			AND R0, R0, #0    ; set R0 to 0
			
			AND R6, R6, #0		; set R6 (bit counter) to 0
CHECK_BIT
			ADD R4, R6, #-4   ; check if 4 bits of the hex digit have been read
			BRzp PRE_PRINT    ; if all bits read, prepare to print

			ADD R0, R0, R0    ; left-shift the value in R0
			ADD R3, R3, #0		; check if R3 holds a negative value
			BRzp ZERO_BIT     ; if not negative, move on to next bit
			ADD R0, R0, #1    ; if negative, MSB of R3 is #1, so add #1 to R0
	
ZERO_BIT
			ADD R3, R3, R3		; left-shift the value in R3
			ADD R6, R6, #1		; increment bit counter
			BR CHECK_BIT			; unconditional branch to check if all bits read

PRE_PRINT
			ADD R4, R0, #-9		; subtract #9 from R0 to determine if value is a numerical hex digit
			BRnz PRINT_NUM		; if difference in R4 is less than or equal to 0, hex digit is 0-9
			LD R0, CHAR				; if hex digit is A-F, load hex offset for ASCII value of letter into R0
			BR PRINT					; unconditional branch to PRINT

PRINT_NUM
			LD R4, NUM				; if hex digit is 0-9, load hex offset for ASCII value of number into R4

PRINT ADD R0, R0, R4    ; Add the hex offset to get the ASCII value for the appropriate hex character
			OUT
			ADD R5, R5, #1		; increment digit counter
			BR CHECK_DIG			; unconditional branch to check if all hex digits printed

NXT_CHAR
			LD R0, NEWLINE		; load the newline character to R0
			OUT								; print newline

			ADD R1, R1, #1		; increment histogram pointer address for next character
			ADD R2, R2, #1		; increment ASCII value of R2 to get next character
			BR CHECK_CHAR			; unconditional branch to beginning of program

DONE	HALT			; done


; the data needed by the program
NUM_BINS	.FILL #27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00 ; histogram starting address
STR_START	.FILL x4000	; string starting address

; the data for printing histogram
CHAR			.FILL x0040 ; the ASCII value of '@'
SPACE			.FILL x0020 ; the ASCII value of ' '
NEWLINE   .FILL x000A ; the ASCII value of newline character
NEG_Z     .FILL xFFA6 ; the additive inverse of ASCII value of 'Z'
NUM				.FILL x0030 ; the difference between a number and its ASCII hex value

; for testing, you can use the lines below to include the string in this
; program...
; STR_START	.FILL STRING	; string starting address
; STRING		.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

	.END
