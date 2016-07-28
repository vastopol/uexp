;TEST ASSEMBLY PROGRAM #2
;Fibonacci Sequence
;========================

.ORIG x3000

;INSTRUCTIONS
;--------------------
  LEA R0, wrd_1
  PUTS			; TRAP x22; print R0 to screen
  
  LD R1, num_1
  LD R2, num_2
  LD R3, #0
  LD R4, index

  LOOP_START
    ADD R3, R1, R2
    
    AND R1, R1, #0	
    ADD R1, R2, #0	; set R1 = R2
    
    AND R2, R2, #0	
    ADD R2, R3, #0	; set R2 = R3

    ADD R4, R4, #-1	; index--
  BRp LOOP_START

  LEA R0, wrd_2
  PUTS

  HALT

;DATA
;--------------------
  num_1 	.FILL #0	;R1
  num_2 	.FILL #1	;R2
  ;answer			 R3
  index 	.FILL #7	;R4  nth fibonacci #

  wrd_1 .STRINGZ "START\n"
  wrd_2 .STRINGZ "END\n" 

  ;fib#: 0,1_,1,2,3,5,8,13,21,34,55,89,144,...
  ;seq#:    0 1 2 3 4 5  6  7  8  9 10  11...

.END