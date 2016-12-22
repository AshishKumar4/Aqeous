section .bss

global current_task
current_task: RESD 1

global old_task
old_task: RESD 1

global new_process
new_process: RESD 1

global old_process
old_process: RESD 1

pdir_backup: RESD 1

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

[GLOBAL switcher]
[EXTERN Scheduler]
[GLOBAL context_switch2]

switcher:                             ; Main Scheduler + context swithcher
                                      ; EFLAGS, CS, EIP are already pushed onto the stack
    cli
    pusha                             ; PUSH ALL REGISTERS ON THE STACK

    mov ebx, [_cur_dir]         ; *Remove these lines once userspace has been implemented.
    mov eax, [current_task]           ; Save the old page directory.
    mov [eax+12], ebx

    mov [old_task], eax               ; Save the old task i.e, current task

    mov [eax], esp
    mov esp, 0xCD00000                ; Change to some temporary stack, I dont want to take risk

    mov eax, [system_dir]
    mov cr3, eax


    call Scheduler                    ; call our scheduler


    mov eax, [old_task]
    mov eax, [eax+4]                  ; compare the processID of old task and new task.

    mov ebx, [current_task]
    mov ebx, [ebx+4]

    cmp eax, ebx
    je context_switch2                ; If both tasks are of same process, No need to do full context switch

                                      ; Else, Complete context switch

    mov eax, [new_process]
    mov [old_process], eax

    mov eax, [current_task]
    mov eax, [eax+8]                  ; New Process
    mov [new_process], eax

    mov eax, [current_task]
    mov esp, [eax]

    mov eax, [eax+12]
    mov cr3, eax
    mov [_cur_dir], eax


    popa

    push eax
    mov eax, 0xFEE000B0               ; APIC End Of Interrupt
    mov dword [eax], 0
;    pop eax

;    push eax
    push edx
       mov dx, 0x20                      ; PIC Timer End Of Interrupt
       mov ax, 0x20
       out dx, ax
    mov eax, 0xFEE00380               ; Reset LVT Timer counter
    mov edx, [time_slice]
    mov [eax], edx
    pop edx
    pop eax

    iretd

context_switch2:

    mov eax, [current_task]
    mov esp, [eax]                    ; Load new esp

    mov eax, [eax+12]
    mov cr3, eax
    mov [_cur_dir], eax

    popa                              ; Popa

    push eax
    mov eax, 0xFEE000B0               ; APIC Timer End Of Interrupt
    mov dword [eax], 0
;    pop eax

;    push eax
    push edx
       mov dx, 0x20                      ; PIC Timer End Of Interrupt
       mov ax, 0x20
       out dx, ax
    mov eax, 0xFEE00380               ; Reset LVT Timer counter
    mov edx, [time_slice]
    mov [eax], edx
    pop edx
    pop eax

    iretd

;****Switcher for Manual (software interrupts) switching****

[GLOBAL switcher_manual]

switcher_manual:                      ; Alternate Scheduler + context swithcher, for Manual Scheduling.
                                      ; EFLAGS, CS, EIP are already pushed onto the stack
    cli
    pusha                             ; PUSH ALL REGISTERS ON THE STACK

    mov eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword [eax], 0

    mov ebx, [_cur_dir]         ; *Remove these lines once userspace has been implemented.
    mov eax, [current_task]           ; Save the old page directory.
    mov [eax+12], ebx

    mov [old_task], eax               ; Save the old task i.e, current task

    mov [eax], esp
    mov esp, 0xCD00000                ; Change to some temporary stack, I dont want to take risk

    mov eax, [system_dir]
    mov cr3, eax

    call Scheduler                    ; call our scheduler


    mov eax, [old_task]
    mov eax, [eax+4]                  ; compare the processID of old task and new task.

    mov ebx, [current_task]
    mov ebx, [ebx+4]

    cmp eax, ebx
    je context_switch2_manual         ; If both tasks are of same process, No need to do full context switch

                                      ; Else, Complete context switch

    mov eax, [new_process]
    mov [old_process], eax

    mov eax, [current_task]
    mov eax, [eax+8]                  ; New Process
    mov [new_process], eax

    mov eax, [current_task]
    mov ebx, [eax+12]
    mov cr3, ebx
    mov [_cur_dir], ebx

    mov esp, [eax]

    popa

    push eax
    mov eax, 0xFEE000B0               ; APIC End Of Interrupt
    mov dword [eax], 0
;    pop eax

;    push eax
    push edx
       mov dx, 0x20                      ; PIC Timer End Of Interrupt
       mov ax, 0x20
       out dx, ax
    mov eax, 0xFEE00380               ; Reset LVT Timer counter
    mov edx, [time_slice]
    mov [eax], edx
    pop edx
    pop eax

    iretd

context_switch2_manual:

    mov eax, [current_task]
    mov esp, [eax]                    ; Load new esp

    mov eax, [eax+12]
    mov cr3, eax
    mov [_cur_dir], eax

    popa                              ; Popa

    push eax
    mov eax, 0xFEE000B0               ; APIC Timer End Of Interrupt
    mov dword [eax], 0
;    pop eax

;    push eax
    push edx
       mov dx, 0x20                      ; PIC Timer End Of Interrupt
       mov ax, 0x20
       out dx, ax
    mov eax, 0xFEE00380               ; Reset LVT Timer counter
    mov edx, [time_slice]
    mov [eax], edx
    pop edx
    pop eax

    iretd


;****Switcher 2 for Manual (software interrupts) switching**** When switching within kernel space

[GLOBAL switcher_ksp]

switcher_ksp:                         ; Alternate Scheduler + context swithcher, for kerenl space Scheduling.
                                      ; EFLAGS, CS, EIP are already pushed onto the stack
    cli
    pusha                             ; PUSH ALL REGISTERS ON THE STACK

    mov dword eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword [eax], 0

    mov dword ebx, [_cur_dir]
    mov dword eax, [system_dir]
    cmp eax, ebx
    je cw_ksp_ss

    mov dword eax, [system_dir]
    mov dword cr3, eax

    mov dword ebx, [_cur_dir]         ; *Remove these lines once userspace has been implemented.
    mov dword eax, [current_task]
    mov dword [old_task], eax               ; Save the old task i.e, current task
    mov dword [eax+12], ebx                 ; Save the old page directory.

    mov dword [eax], esp
    mov dword esp, 0xCD00000                ; Change to some temporary stack, I dont want to take risk

    call Scheduler                    ; call our scheduler


    mov dword ebx, [_cur_dir]
    mov dword eax, [current_task]
    mov dword eax, [eax+12]
    cmp eax, ebx
    je cw_p2_ndc_c


    mov dword eax, [current_task]

    mov dword eax, [current_task]
    mov dword esp, [eax]

    mov dword eax, [eax+12]
    mov dword [_cur_dir], eax

    mov dword cr3, eax

    popa

    push eax
    mov dword eax, 0xFEE000B0               ; APIC End Of Interrupt
    mov dword [eax], 0

    push edx
       mov dx, 0x20                      ; PIC Timer End Of Interrupt
       mov ax, 0x20
       out dx, ax
    mov dword eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword edx, [time_slice]
    mov dword [eax], edx
    pop edx
    pop eax

    iretd


cw_ksp_ss:

    mov dword eax, [current_task]           ; Save the old page directory.
    mov dword [old_task], eax               ; Save the old task i.e, current task
    mov dword [eax], esp
    mov dword esp, 0xCD00000                ; Change to some temporary stack, I dont want to take risk


    call Scheduler                    ; call our scheduler


    mov dword ebx, [system_dir]
    mov dword eax, [current_task]
    mov dword eax, [eax+12]
    cmp eax, ebx
    je cw_p2_ndc_c


    mov dword eax, [current_task]

    mov dword eax, [current_task]
    mov dword esp, [eax]

    mov dword eax, [eax+12]
    mov dword [_cur_dir], eax

    mov dword cr3, eax

    popa

    push eax
    mov dword eax, 0xFEE000B0               ; APIC End Of Interrupt
    mov dword [eax], 0

    push edx
       mov dx, 0x20                      ; PIC Timer End Of Interrupt
       mov ax, 0x20
       out dx, ax
    mov dword eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword edx, [time_slice]
    mov dword [eax], edx
    pop edx
    pop eax

    iretd

cw_p2_ndc_c:  ;Context switch part 2, no page directory change

    mov dword eax, [current_task]
    mov dword esp, [eax]                    ; Load new esp

    popa                              ; Popa

    push eax
    mov dword eax, 0xFEE000B0               ; APIC Timer End Of Interrupt
    mov dword [eax], 0

    push edx
       mov dx, 0x20                      ; PIC Timer End Of Interrupt
       mov ax, 0x20
       out dx, ax
    mov dword eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword edx, [time_slice]
    mov dword [eax], edx
    pop edx
    pop eax

    iretd


[GLOBAL switcher_ksp2]

switcher_ksp2:                         ; Alternate Scheduler + context swithcher, for kerenl space Scheduling.
                                      ; EFLAGS, CS, EIP are already pushed onto the stack
    cli
    pusha                             ; PUSH ALL REGISTERS ON THE STACK

    mov eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword [eax], 0

    mov dword ebx, [_cur_dir]
    mov dword eax, [system_dir]
    cmp eax, ebx
    je cw_ksp_ss2


    mov dword ebx, [_cur_dir]         ; *Remove these lines once userspace has been implemented.
    mov dword eax, [current_task]
    mov dword [old_task], eax               ; Save the old task i.e, current task
    mov dword [eax+12], ebx                 ; Save the old page directory.

    mov dword [eax], esp
    mov esp, 0xCD00000                ; Change to some temporary stack, I dont want to take risk

    mov dword eax, [system_dir]
    mov cr3, eax

    call Scheduler                    ; call our scheduler


    mov dword ebx, [_cur_dir]
    mov dword eax, [current_task]
    mov dword eax, [eax+12]
    cmp eax, ebx
    je cw_p2_ndc_c2


    mov dword eax, [current_task]

    mov dword eax, [current_task]
    mov esp, [eax]

    mov dword eax, [eax+12]
    mov [_cur_dir], eax

    mov cr3, eax

    popa

    push eax
    mov dword eax, 0xFEE000B0               ; APIC End Of Interrupt
    mov dword [eax], 0

    push edx
       mov dx, 0x20                      ; PIC Timer End Of Interrupt
       mov ax, 0x20
       out dx, ax
    mov eax, 0xFEE00380               ; Reset LVT Timer counter
    mov edx, [time_slice]
    mov [eax], edx
    pop edx
    pop eax

    iretd


cw_ksp_ss2:

    mov dword eax, [current_task]           ; Save the old page directory.
    mov dword [old_task], eax               ; Save the old task i.e, current task
    mov dword [eax], esp
    mov esp, 0xCD00000                ; Change to some temporary stack, I dont want to take risk


    call Scheduler                    ; call our scheduler


    mov dword ebx, [system_dir]
    mov dword eax, [current_task]
    mov dword eax, [eax+12]
    cmp eax, ebx
    je cw_p2_ndc_c2


    mov dword eax, [current_task]

    mov dword eax, [current_task]
    mov esp, [eax]

    mov dword eax, [eax+12]
    mov [_cur_dir], eax

    mov cr3, eax

    popa

    push eax
    mov dword eax, 0xFEE000B0               ; APIC End Of Interrupt
    mov dword [eax], 0

    push edx
       mov dx, 0x20                      ; PIC Timer End Of Interrupt
       mov ax, 0x20
       out dx, ax
    mov eax, 0xFEE00380               ; Reset LVT Timer counter
    mov edx, [time_slice]
    mov [eax], edx
    pop edx
    pop eax

    iretd

cw_p2_ndc_c2:  ;Context switch part 2, no page directory change

    mov dword eax, [current_task]
    mov esp, [eax]                    ; Load new esp

    popa                              ; Popa

    push eax
    mov dword eax, 0xFEE000B0               ; APIC Timer End Of Interrupt
    mov dword [eax], 0

    push edx
       mov dx, 0x20                      ; PIC Timer End Of Interrupt
       mov ax, 0x20
       out dx, ax
    mov eax, 0xFEE00380               ; Reset LVT Timer counter
    mov edx, [time_slice]
    mov [eax], edx
    pop edx
    pop eax

    iretd


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

[GLOBAL Scheduler_init]

Scheduler_init:
    mov eax, [system_dir]
    mov cr3, eax

    mov eax, [current_task]
    mov esp, [eax]                 ; Load new esp

    popa                              ; Popa

    iretd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
