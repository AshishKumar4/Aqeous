section .bss

global kernel_stack
kernel_stack: RESD 1

esp_backup: RESD 1

global time_slice
time_slice: RESD 1

global test_pit_timer
test_pit_timer: RESD 1

extern tick
extern system_dir
extern main_dir
extern _cur_dir
extern _prev_dir

section .text

[EXTERN Scheduler_t]

;****Switcher Template for Manual (software interrupts) switching**** When switching within kernel space

[GLOBAL switcher_ksp_t]
[GLOBAL switcher_end_t]
[GLOBAL cw_ksp_ss_t]
[GLOBAL cw_p2_ndc_c_t]

;Symbol Map:
;    _cur_dir        ->        0x4284ACD1
;    system_dir      ->        0x4284ACD2
;    current_task    ->        0x4284ACD3
;    old_task        ->        0x4284ACD4
;    Scheduler       ->        0x4284ACD5
;    time_slice      ->        0x4284ACD6
;    Temporary Stack ->        0x4284CD00

switcher_ksp_t:                         ; Alternate Scheduler + context swithcher, for kerenl space Scheduling.
                                      ; EFLAGS, CS, EIP are already pushed onto the stack
    cli
    pusha                             ; PUSH ALL REGISTERS ON THE STACK

;;;    xor dword eax, eax

;;;    mov dword eax, 0x1
;;;    mov dword [0x4284BCD1], eax           ; Core Locking Mechanism

    mov dword eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword [eax], 0

    mov dword ebx, [0x4284ACD1]
    mov dword eax, [0x4284ACD2]
    cmp eax, ebx
    je cw_ksp_ss_t

    mov dword eax, [0x4284ACD2]
    mov dword cr3, eax

    mov dword ebx, [0x4284ACD1]         ; *Remove these lines once userspace has been implemented.
    mov dword eax, [0x4284ACD3]
    mov dword [0x4284ACD4], eax               ; Save the old task i.e, current task
    mov dword [eax+12], ebx                 ; Save the old page directory.

    mov dword [eax], esp
    mov dword esp, 0x4284CD00                ; Change to some temporary stack, I dont want to take risk

    call [0x4284ACD5]                    ; call our scheduler


    mov dword ebx, [0x4284ACD1]
    mov dword eax, [0x4284ACD3]
    mov dword eax, [eax+12]
    cmp eax, ebx
    je cw_p2_ndc_c_t


    mov dword eax, [0x4284ACD3]

    mov dword eax, [0x4284ACD3]
    mov dword esp, [eax]

    mov dword eax, [eax+12]
    mov dword [0x4284ACD1], eax

    mov dword cr3, eax

    popa

    push eax
    mov dword eax, 0xFEE000B0               ; APIC End Of Interrupt
    mov dword [eax], 0

;;;    xor dword eax, eax
;;;    mov dword [0x4284BCD1], eax           ; Core Locking Mechanism

    push edx
    mov dx, 0x20                      ; PIC Timer End Of Interrupt
    mov ax, 0x20
    out dx, ax
    mov dword eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword edx, [0x4284ACD6]
    mov dword [eax], 0xf0000000
    pop edx
    pop eax

    iretd


cw_ksp_ss_t:

    mov dword eax, [0x4284ACD3]           ; Save the old task.
    mov dword [0x4284ACD4], eax               ; Save the old task i.e, current task
    mov dword [eax], esp
    mov dword esp, 0x4284CD00                ; Change to some temporary stack, I dont want to take risk


    call [0x4284ACD5]                     ; call our scheduler


    mov dword ebx, [0x4284ACD2]
    mov dword eax, [0x4284ACD3]
    mov dword eax, [eax+12]
    cmp eax, ebx
    je cw_p2_ndc_c_t


;    mov dword eax, [0x4284ACD3]

    mov dword eax, [0x4284ACD3]
    mov dword esp, [eax]

    mov dword eax, [eax+12]
    mov dword [0x4284ACD1], eax

    mov dword cr3, eax

    popa

    push eax
    mov dword eax, 0xFEE000B0               ; APIC End Of Interrupt
    mov dword [eax], 0

;;;    xor dword eax, eax
;;;    mov dword [0x4284BCD1], eax           ; Core Locking Mechanism

    push edx
    mov dx, 0x20                      ; PIC Timer End Of Interrupt
    mov ax, 0x20
    out dx, ax
    mov dword eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword edx, [0x4284ACD6]
    mov dword [eax], 0xf0000000
    pop edx
    pop eax

    iretd

cw_p2_ndc_c_t:  ;Context switch part 2, no page directory change

    mov dword eax, [0x4284ACD3]
    mov dword esp, [eax]                    ; Load new esp

    popa                              ; Popa

    push eax
    mov dword eax, 0xFEE000B0               ; APIC Timer End Of Interrupt
    mov dword [eax], 0

;;;    xor dword eax, eax
;;;    mov dword [0x4284BCD1], eax           ; Core Locking Mechanism

    push edx
    mov dx, 0x20                      ; PIC Timer End Of Interrupt
    mov ax, 0x20
    out dx, ax
    mov dword eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword edx, [0x4284ACD6]
    mov dword [eax], 0xf0000000
    pop edx
    pop eax

    iretd

switcher_end_t:


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;[EXTERN SAS_Catalouge_handler]

;[GLOBAL Scheduler_Promoter_assistance]

Scheduler_Promoter_assistance:
    cli
    pusha

    mov eax, esp
    mov [esp_backup], eax

    ;call SAS_Catalouge_handler

    mov eax, [esp_backup]
    mov esp, eax

    popa

    ;TODO: EOI

    push eax
    mov eax, 0xFEE000B0                ; APIC Timer End Of Interrupt
    mov dword [eax], 0
;    pop eax

;    push eax
    push edx
    mov dx, 0x20                      ; PIC Timer End Of Interrupt
    mov ax, 0x20
    out dx, ax
    pop edx
    pop eax

    iretd


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[GLOBAL Test_Timer]

Test_Timer:
;  cli
;
;  push eax
;
;  mov eax, [test_pit_timer]
;  add eax, 0x1
;  mov [test_pit_timer], eax

;  pop eax

  iretd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

loop1:
    jmp loop2

loop2:
    jmp loop1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[GLOBAL Scheduler_init_t]
[GLOBAL Scheduler_init_end_t]

Scheduler_init_t:
    mov eax, [0x4284ACD2]
    mov cr3, eax

    mov eax, [0x4284ACD3]
    mov esp, [eax]                 ; Load new esp

    popa                              ; Popa

    iretd

Scheduler_init_end_t:
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
