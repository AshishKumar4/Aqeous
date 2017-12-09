section .bss

esp_backup: RESD 1

esp_backup2: RESD 1

curr_dir: RESD 1
eax_backup: RESD 1

syscall_stack_back1: RESD 1
syscall_stack_back2: RESD 1
syscall_stack_back3: RESD 1

tmp_stack: RESD 1

tmp_intStack: RESD 4096

tmp_cr3: RESD 1

global syscall_ia32_esp
syscall_ia32_esp: RESD 1

global t_c
t_c: RESD 1

section .text

tmp_esp:
  dd 4096
tmp_esp_end:

[EXTERN system_dir]
[EXTERN current_pdir]

[EXTERN PIT_Handle]
[GLOBAL PIT_handler]

[GLOBAL syscall_vector]
[EXTERN syscall_handler]

syscall_vector:
  cli
  ;pusha

;  mov eax, esp
;  mov [tmp_stack], eax

  mov eax, cr3
  mov [current_pdir], eax
  mov eax, [system_dir]
  mov cr3, eax


  mov eax, esp
  mov [tmp_stack], eax

  mov eax, [tmp_intStack]
  mov esp, eax

  call syscall_handler

  mov eax, [current_pdir]
  mov cr3, eax

  mov eax, [tmp_stack]
  mov esp, eax

;  popa
  iretd



Default_ContextSwitch:
  cli
  pusha
  mov ebx, cr3
  mov eax, [system_dir]
  mov cr3, eax
  mov [current_pdir], ebx

  mov eax, esp
  mov [esp_backup], eax

  mov eax, tmp_intStack
  add eax, 2048

  mov esp, eax

  ;call <FUNCTION>

  mov eax, [esp_backup]
  mov esp, eax
  popa
  push eax
  mov eax, [current_pdir]
  mov cr3, eax
  pop eax

  iretd


PIT_handler:
  cli
  mov ebx, esp
  push eax
  mov ax, ds 
  cmp ax, 0x10
  je tetn
  ;hlt
    mov eax, cr3 
    mov [tmp_cr3], eax
    mov eax, [system_dir]
    mov cr3, eax
   ;add esi, 0x1

tetn:
  pop eax 
  
  pusha
  
  mov eax, esp
  mov [esp_backup], eax

 ; mov eax, tmp_esp
 ; mov esp, eax
  
  call PIT_Handle

  mov eax, [esp_backup]
  mov esp, eax
  popa

  push eax
  mov eax, 0xFEE000B0                ; APIC Timer End Of Interrupt
  mov dword [eax], 0

  push edx
  mov dx, 0x20                      ; PIT Timer End Of Interrupt
  mov ax, 0x20
  out dx, ax
  mov dx, 0xA0
  mov ax, 0x20
  out dx, ax
  pop edx

  mov ax, ds 
  cmp ax, 0x10
  je jtetn

    mov eax, [tmp_cr3]
    mov cr3, eax
jtetn:
  pop eax 
  sti
  iret


[GLOBAL kb_handle]
[EXTERN keyboardInterrupt_handler]

kb_handle:
    cli
    push eax
    mov ax, ds 
    cmp ax, 0x10
    je tet
    
    mov eax, cr3 
    mov [tmp_cr3], eax
    mov eax, [system_dir]
    mov cr3, eax

tet:
    pop eax 
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

    push eax
    mov ax, ds 
    cmp ax, 0x10
    je jtet

    mov eax, [tmp_cr3]
    mov cr3, eax
jtet:
    pop eax 
    sti
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
    sti
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
