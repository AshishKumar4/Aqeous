section .bss

eip_backup: RESD 1
cs_backup: RESD 1
eflags_backup: RESD 1
eax_backup: RESD 1
esp_backup: RESD 1

section .text

[GLOBAL keyboard_handler]
[EXTERN keyboardInterrupt_handler]

keyboard_handler:
    mov [eax_backup], eax

    pop eax
    mov [eip_backup], eax
    pop eax
    mov [cs_backup], eax
    pop eax
    mov [eflags_backup], eax

    mov eax, [eax_backup]

    pusha

    mov [esp_backup], esp

    call keyboardInterrupt_handler

    mov esp, [esp_backup]

    popa

    mov eax, [eflags_backup]
    push eax
    mov eax, [cs_backup]
    push eax
    mov eax, [eip_backup]
    push eax

    mov eax, [eax_backup]

    push eax
    mov dx, 0x20                      ; PIT Timer End Of Interrupt
    mov ax, 0x20
    out dx, ax
    pop eax

    iretd
