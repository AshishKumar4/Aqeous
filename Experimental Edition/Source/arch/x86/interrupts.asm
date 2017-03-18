section .bss

global esp_backup
esp_backup: RESD 1

global esp_backup2
esp_backup2: RESD 1

global faulting_address
faulting_address: RESD 1

curr_dir: RESD 1
eax_backup: RESD 1

global t_c
t_c: RESD 1

section .text

[GLOBAL pageFault_handler]
[EXTERN pageFault_caller]
[EXTERN system_dir]

pageFault_handler:
    cli

    pusha

    mov eax, cr3
    mov [curr_dir], eax

    mov eax, [system_dir]
    mov cr3, eax

    mov eax, esp
    mov [esp_backup], eax

    mov eax, cr2
    mov [faulting_address], eax

    call pageFault_caller

    mov eax, [esp_backup]
    mov esp, eax

    popa

    push eax
    mov eax, 0xFEE000B0
    mov dword [eax], 0

    pop eax

    mov [eax_backup], eax
    mov eax, [curr_dir]
    mov cr3, eax
    mov eax, [eax_backup]

    iretd


[GLOBAL kb_handle]
[EXTERN keyboardInterrupt_handler]

kb_handle:
    cli
    pusha

    mov eax, esp
    mov [esp_backup], eax

    call keyboardInterrupt_handler

    mov eax, [esp_backup]
    mov esp, eax

    popa

    push eax
    mov eax, 0xFEE000B0                ; APIC Timer End Of Interrupt
    mov dword [eax], 0
;    pop eax

;    push eax
    push edx
    mov dx, 0x20                      ; PIT Timer End Of Interrupt
    mov ax, 0x20
    out dx, ax
    pop edx
    pop eax

    iretd


[GLOBAL mouse_handle]
[EXTERN mouse_handler]

mouse_handle:
;    cli
;    pusha

;    mov eax, esp
;    mov [esp_backup2], eax

;    call mouse_handler

;    mov eax, [esp_backup2]
;    mov esp, eax

;    popa

    push eax
    mov eax, 0xFEE000B0                ; APIC Timer End Of Interrupt
    mov dword [eax], 0
    ;    pop eax

    ;    push eax
    push edx
    mov dx, 0x20                      ; PIT Timer End Of Interrupt
    mov ax, 0x20
    out dx, ax
    pop edx
    pop eax

    iretd

[GLOBAL test_ttt]
[EXTERN ttt_t]

test_ttt:
    cli
    pusha

    mov eax, esp
    mov [esp_backup], eax

    call ttt_t

    mov eax, [esp_backup]
    mov esp, eax

    popa

    push eax
    mov eax, 0xFEE000B0                ; APIC Timer End Of Interrupt
    mov dword [eax], 0
    ;    pop eax

    ;    push eax
    ;    push edx
    ;    mov dx, 0x20                      ; PIT Timer End Of Interrupt
    ;    mov ax, 0x20
    ;    out dx, ax
    ;    pop edx
    pop eax

    iretd
