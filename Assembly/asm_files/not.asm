;TEST ASSEMBLY PROGRAM 
;not 
;========================

.ORIG x3000

;INSTRUCTIONS
;--------------------
  LEA R0, wrd_1
  PUTS			; TRAP x22; output to screen
  
  LD R1, num_1
  AND R2,R1,R1		; copy R1 --> R2

  NOT R2, R2		; when R2 = #n before, after = -(#n+1)
  ADD R2, R2, #1		; finish 2's complement  
  ADD R3, R1, R2		; R3 = (R1 + R2) = 0

  LEA R0, wrd_2
  PUTS

  HALT

;DATA
;--------------------
  num_1 .FILL #4
  wrd_1 .STRINGZ "START\n"
  wrd_2 .STRINGZ "END\n" 

.END