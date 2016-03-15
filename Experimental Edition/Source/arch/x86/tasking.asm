section .bss

global old_esp
old_esp: RESD 1

global new_esp
new_esp: RESD 1

global old_eip
old_eip: RESD 1

global old_eflags
old_eflags: RESD 1

global old_cs
old_cs: RESD 1

global new_eip
new_eip: RESD 1

global new_eflags
new_eflags: RESD 1

global new_cs
new_cs: RESD 1

global new_seg
new_seg: RESD 1


section .text

[GLOBAL switcher]
[GLOBAL save_eip]
[EXTERN helper1]
[EXTERN scheduler]
[EXTERN save_esp]

save_eip:
    pop dword [old_eip]
    pop dword [old_cs]
    pop dword [old_eflags]

switcher:
    pusha;

    ;mov ax, ds               ; Lower 16-bits of eax = ds.
    ;push eax                 ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov [old_esp], esp

    call save_esp

    mov esp, [new_esp]

    ;pop eax
    mov ax, [new_seg]  ; load the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa

    push dword [new_eflags]
    push dword [new_cs]
    push dword [new_eip]
    call helper1
    iret;
