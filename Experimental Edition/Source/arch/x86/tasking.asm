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

global temp_dir
temp_dir: RESD 1

extern tick
extern system_dir
extern main_dir
extern _cur_directory
extern _prev_directory

section .text

[GLOBAL switcher]
[EXTERN Scheduler]
[EXTERN timer_stub]
[GLOBAL context_switch2]

switcher:                             ; Main Scheduler + context swithcher
                                      ; EFLAGS, CS, EIP are already pushed onto the stack
    cli
    pusha                             ; PUSH ALL REGISTERS ON THE STACK

    mov eax, cr3                      ; *Remove these lines once userspace has been implemented.
    mov [temp_dir], eax

    mov eax, [system_dir]
    mov cr3, eax

    mov eax, [current_task]           ; Save the old task i.e, current task
    mov [old_task], eax

    mov [eax], esp
    mov esp, 0xCD00000                ; Change to some temporary stack, I dont want to take risk


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
                                      ; replace old_process with old_task

;    mov eax, [eax]
;    mov [_cur_directory], eax
;    mov eax, cr3
;    mov [_prev_directory], eax
;    mov eax, [_cur_directory]
;    mov cr3, eax                     ; SWITCH TO New Process's page directory

    mov eax, [eax]
    mov cr3, eax                      ; SWITCH TO New Process's page directory
    mov [_cur_directory], eax


    mov eax, [current_task]
    mov esp, [eax]

    popa

;    push eax
;    mov eax, 0xFEE000B0               ; APIC End Of Interrupt
;    mov dword [eax], 0
;    pop eax

    push eax
    push edx
    mov dx, 0x20                      ; PIC Timer End Of Interrupt
    mov ax, 0x20
    out dx, ax
    pop edx
    pop eax

    iretd

context_switch2:
  ;  call timer_stub                  ; To record how many times did our scheduler worked Successfully

    mov eax, [current_task]
    mov esp, [eax]                    ; Load new esp

    mov eax, [temp_dir]
    mov cr3, eax

    popa                              ; Popa

  ;  mov eax, 1                       ; A functionality to DBUG scheduler at any given cycle 'x' where x = 1,2,3....
  ;  mov ebx, [tick]                  ; COMMENT THESE TO RUN MULTITASKING AT FULL
  ;  cmp eax,ebx
  ;  je secondout

;    push eax
;    mov eax, 0xFEE000B0                ; APIC Timer End Of Interrupt
;    mov dword [eax], 0
;    pop eax

    push eax
    push edx
    mov dx, 0x20                      ; PIC Timer End Of Interrupt
    mov ax, 0x20
    out dx, ax
    pop edx
    pop eax

    iretd

secondout:
    iretd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[EXTERN Task_Promoter]

[GLOBAL Scheduler_Promoter_assistance]

Scheduler_Promoter_assistance:
    pusha

    mov eax, cr3
    mov [pdir_backup], eax

    mov eax, [system_dir]
    mov cr3, eax

    call Task_Promoter

    mov eax, [pdir_backup]
    mov cr3, eax

    popa

    ;TODO: EOI

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
