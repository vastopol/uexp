;LOGICAL OPERATIONS
;Based on: NOT, AND
;==================

.ORIG x3000

;INSTRUCTIONS
;-----------------

LD R1, var_A	;VALUE
LD R2, var_B	;VALUE
LD R6, ptr	;ADDRESS

STR R1, R6, #0	; store: A
STR R2, R6, #1	; store: B

;AND
AND R3, R1, R2
STR R3, R6, #2	; store: A and B

;OR
NOT R4, R1	; not(A)
NOT R5, R2	; not(B)
AND R3, R4, R5	; not(A) and not(B)
NOT R3, R3	; not( not(A) and not(B) )
STR R3, R6, #3	; store: A or B



;r0 - r7

HALT

;DATA
;---------------------
var_A .FILL xC	;1100
var_B .FILL xA	;1010
ptr .FILL x4000	; store answers as remote data in x4000's

.END


