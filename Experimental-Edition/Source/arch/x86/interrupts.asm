section .bss

global esp_backup
esp_backup: RESD 1

global esp_backup2
esp_backup2: RESD 1

section .text

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
;    push edx
;    mov dx, 0x20                      ; PIT Timer End Of Interrupt
;    mov ax, 0x20
;    out dx, ax
;    pop edx
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
