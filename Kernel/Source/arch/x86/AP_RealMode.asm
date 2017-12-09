section .bss

section .text

[GLOBAL rmode_AP_code]
[GLOBAL rmode_AP_code_end]
[EXTERN trial_code]
[EXTERN enable_pic]
[GLOBAL gdt16_base]

BITS 16

rmode_AP_code: use16
  cli

  xor ax, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  xor esp, esp
  mov sp, 0x4284

#  xor esi, esi
  mov si, 0x3240
  lgdt [ds:si]

  mov si, 0x3250
  lidt [ds:si]

  xor ax, ax
  mov ax, 0x1
  mov si, 0x1000
  mov [ds:si], ax

back_code:
  mov si, 0x1004
  mov ax, [ds:si]
  cmp ax, 0

  je back_code

  xor ax, ax
  mov ax, 0x1500
  mov sp, ax
  popa
  pop  gs                                ; load gs from 16bit stack
  pop  fs                                ; load fs from 16bit stack
  pop  es                                ; load es from 16bit stack
  pop  ds
  sti
#  hlt

  int 0x10

  push ds                                ; save ds to 16bit stack
  push es                                ; save es to 16bit stack
  push fs                                ; save fs to 16bit stack
  push gs                                ; save gs to 16bit stack
  pusha
  xor ax, ax
  mov si, 0x1004
  mov [ds:si], ax

  jmp back_code

  hlt

idt16_ptr:                                 ; IDT table pointer for 16bit access
  dw 0x03FF                              ; table limit (size)
  dd 0x00000000                          ; table base address

gdt16_base:                                ; GDT descriptor table
  .null:                                 ; 0x00 - null segment descriptor
    dd 0x00000000                      ; must be left zero'd
    dd 0x00000000                      ; must be left zero'd

  .code32:                               ; 0x01 - 32bit code segment descriptor 0xFFFFFFFF
    dw 0xFFFF                          ; limit  0:15
    dw 0x0000                          ; base   0:15
    db 0x00                            ; base  16:23
    db 0x9A                            ; present, iopl/0, code, execute/read
    db 0xCF                            ; 4Kbyte granularity, 32bit selector; limit 16:19
    db 0x00                            ; base  24:31

  .data32:                               ; 0x02 - 32bit data segment descriptor 0xFFFFFFFF
    dw 0xFFFF                          ; limit  0:15
    dw 0x0000                          ; base   0:15
    db 0x00                            ; base  16:23
    db 0x92                            ; present, iopl/0, data, read/write
    db 0xCF                            ; 4Kbyte granularity, 32bit selector; limit 16:19
    db 0x00                            ; base  24:31

  .code16:                               ; 0x03 - 16bit code segment descriptor 0x000FFFFF
    dw 0xFFFF                          ; limit  0:15
    dw 0x0000                          ; base   0:15
    db 0x00                            ; base  16:23
    db 0x9A                            ; present, iopl/0, code, execute/read
    db 0x0F                            ; 1Byte granularity, 16bit selector; limit 16:19
    db 0x00                            ; base  24:31

  .data16:                               ; 0x04 - 16bit data segment descriptor 0x000FFFFF
    dw 0xFFFF                          ; limit  0:15
    dw 0x0000                          ; base   0:15
    db 0x00                            ; base  16:23
    db 0x92                            ; present, iopl/0, data, read/write
    db 0x0F                            ; 1Byte granularity, 16bit selector; limit 16:19
    db 0x00                            ; base  24:31

gdt16_ptr:                                 ; GDT table pointer for 16bit access
  dw gdt16_ptr - gdt16_base - 1          ; table limit (size)
  dd gdt16_base                          ; table base address

resetpic:                                  ; reset's 8259 master and slave pic vectors
  push ax                                ; expects bh = master vector, bl = slave vector
  mov  al, 0x11                          ; 0x11 = ICW1_INIT | ICW1_ICW4
  out  0x20, al                          ; send ICW1 to master pic
  out  0xA0, al                          ; send ICW1 to slave pic
  mov  al, bh                            ; get master pic vector param
  out  0x21, al                          ; send ICW2 aka vector to master pic
  mov  al, bl                            ; get slave pic vector param
  out  0xA1, al                          ; send ICW2 aka vector to slave pic
  mov  al, 0x04                          ; 0x04 = set slave to IRQ2
  out  0x21, al                          ; send ICW3 to master pic
  shr  al, 1                             ; 0x02 = tell slave its on IRQ2 of master
  out  0xA1, al                          ; send ICW3 to slave pic
  shr  al, 1                             ; 0x01 = ICW4_8086
  out  0x21, al                          ; send ICW4 to master pic
  out  0xA1, al                          ; send ICW4 to slave pic
  pop  ax                                ; restore ax from stack
  ret                                    ; return to caller

rmode_AP_code_end:
  ret
