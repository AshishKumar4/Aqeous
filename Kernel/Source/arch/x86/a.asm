format ELF executable 
entry start 

start: 
org 100h

mov bx,65535 ;we store number we want to display in bx
 ;(because it's not used by DOS services we use)
mov cx,16 ;we are diplaying 16 digits (bits)

;display one digit from BX each loop

display_digit:
shl bx,1
jc display_one

;display '0'
mov ah,2
mov dl,'0'
int 21h
jmp continue

;display '1'
display_one:
mov ah,2
mov dl,'1'
int 21h

;check if we want to continue
continue:
dec cx
jnz display_digit

;end program
int 20h
