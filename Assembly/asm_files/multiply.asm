;TEST ASSEMBLY
;(a*b)
;========================

.ORIG x3000

;INSTRUCTIONS
;--------------------
  LEA R0, wrd_1
  PUTS			; TRAP x22; output to screen

  ;TRAP x20		; R0 <-- user input character 
  ;TRAP x21		; echo char in R0

  LD R1, num_1
  LD R2, num_2
  ADD R4, R2, #0	; index

  LOOP_START
    ADD R3, R3, R1
    ADD R4, R4, #-1
  BRp LOOP_START
  
  LEA R0, wrd_2
  PUTS

  HALT

;DATA
;--------------------
  num_1 .FILL #4		; a
  num_2 .FILL #5		; b

  wrd_1 .STRINGZ "START\n"
  wrd_2 .STRINGZ "END\n" 

.END