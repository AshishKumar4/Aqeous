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

extern tick
extern system_dir
extern main_dir
extern _cur_directory
extern _prev_directory

section .text

[GLOBAL switcher]
[EXTERN Scheduler]
[GLOBAL context_switch2]

switcher:                             ; Main Scheduler + context swithcher
                                      ; EFLAGS, CS, EIP are already pushed onto the stack
    cli
    pusha                             ; PUSH ALL REGISTERS ON THE STACK

    mov ebx, [_cur_directory]         ; *Remove these lines once userspace has been implemented.
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
    mov [_cur_directory], eax


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
    mov [_cur_directory], eax

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

    mov ebx, [_cur_directory]         ; *Remove these lines once userspace has been implemented.
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
    mov [_cur_directory], ebx

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
    mov [_cur_directory], eax

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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;[EXTERN SAS_Catalouge_handler]

[GLOBAL Scheduler_Promoter_assistance]

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
