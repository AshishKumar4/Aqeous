section .bss

esp_backup: RESD 1

esp_backup2: RESD 1

curr_dir: RESD 1
eax_backup: RESD 1

global t_c
t_c: RESD 1

section .text

[EXTERN system_dir]

[EXTERN PIT_Handle]
[GLOBAL PIT_handler]

PIT_handler:
  cli
  pusha

  mov eax, esp
  mov [esp_backup], eax

  call PIT_Handle

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

  iret



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
iretd;
    cli
    pusha

    mov eax, esp
    mov [esp_backup2], eax

    call mouse_handler

    mov eax, [esp_backup2]
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
    mov dx, 0xA0
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
;0010a14f\
