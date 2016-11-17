        name     load
        page     55,128
        title   'LOAD -- load .COM file for MS-DOS 2.0' 
;
;
; LOAD -- load .COM file bigger than 64K
; Requires MS-DOS 2.0
; Version 1.1   Dec 83 RGD
; Version 1     March 1983 RJW

; copyright (c) 1983
; Laboratory Microsystems
; 4147 Beethoven Street
; Los Angeles, CA  90066
;
cr      equ     0dh     ;ASCII carriage return
lf      equ     0ah     ;ASCII line feed

cseg    segment byte

        org     100h

        assume  cs:cseg,ds:cseg

load    proc    far             ; sets up far return ...

        push    es              ; save segment of PSP
        mov     dx,offset mes2  ; startup message
        mov     ah,9
        int     21h
        xor     dx,dx           ; zero DX
        mov     ah,25h          ; set terminate address ...
        mov     al,22h          ; ... for new program segment
        int     21h

        mov     dx,offset endofs ; offset to end of this loader
        mov     cl,4            ; no of bits to shift
        shr     dx,cl           ; convert byte addr to paragraph
        inc     dx              ; offset of 1st available segment
        mov     ax,cs           ; current segment to AX
        add     dx,ax           ; actual value of 1st available segment
        mov     useg,dx         ; save it for later ...
        mov     es,dx           ; ... and for subsequent move
        mov     ah,26h          ; call to DOS
        int     21h             ; create new program segment

        mov     si,6ch          ; 2nd param FCB in current segment
        mov     di,5ch          ; 1st param FCB in new segment
        mov     cx,0ch          ; byte count for move
        repz movsb              ; copy the filename

        mov     ax,cs           ; copy current code seg ...
        mov     ds,ax           ; ... to DS
        mov     dx,5ch          ; DS:DX points to FCB of .COM file
        mov     bx,dx           ; make FCB addressible
        mov     byte ptr  9 [bx],'C' ; force COM extension 
        mov     byte ptr 10 [bx],'O' 
        mov     byte ptr 11 [bx],'M' 
        mov     ah,0fh          ; open the .COM file
        int     21h
        or      al,al           ; test return code
        jnz     load8           ; exit if non-zero
        mov     word ptr 33 [bx],0000 ; zero the random 
        mov     word ptr 35 [bx],0000 ; record field in the FCB
        pop     es              ; get loader's PSP segment
        mov     bx,useg         ; let SS:SP = default buffer of
        mov     ss,bx           ; new PSP
        mov     sp,100h         
        push    es              ; save loader's PSP again
        add     bx,10h          ; BX = segment of current DTA
        mov     ds,bx           ; set up DS:DX to point to the DTA
        xor     dx,dx
        mov     ah,1ah          ; set up DOS call and do it
        int     21h

load5:  mov     cx,100h         ; number of records of length 80h
        mov     ax,cs           ; copy current CS to DS
        mov     ds,ax
        mov     dx,5ch          ; DS:DH points to FCB of .COM file
        mov     ah,27h          ; do random block read
        int     21h
        test    al,1            ; end of file?
        jnz     load9           ; yes, so exit
        add     bx,800h         ; increment location of DTA
        mov     ds,bx           ; copy to DS
        xor     dx,dx           ; DS:DX now points to next DTA
        mov     ah,1ah          ; set up DOS call to set DTA
        int     21h
        jmp     load5           ; do it again

load8:  mov     dx,offset mes1  ; "file not found"
        mov     ah,9            ; write to terminal
        int     21h
        int     20h             ; exit to DOS

load9:  mov     ax,useg         ; set up registers for new segment
        mov     ds,ax
        pop     es              ; pass loader's PSP segment to overlay
        push    ax              ; push new CS onto stack
        mov     ax,100h
        push    ax              ; push offset onto stack
        ret                     ; FAR return causes CS:IP to be set
load    endp

mes1    db      cr,lf,
        db      '.COM file not found'
        db      cr,lf,'$'
mes2    db      cr,lf
        db      'Multi-Segment Loader version 1.1 for MS-DOS 2.0'
        db      cr,lf
        db      'Copyright (c) 1983 Laboratory Microsystems Inc.'
        db      cr,lf,'$'

useg    dw      0

endofs  equ     $

cseg    ends                    ; end of code segment

        end     load 
