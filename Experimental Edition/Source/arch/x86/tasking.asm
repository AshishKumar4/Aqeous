section .bss

global current_task
current_task: RESD 1

global old_task
old_task: RESD 1

global new_process
new_process: RESD 1

global old_process
old_process: RESD 1

esp_backup: RESD 1

global kernel_stack
kernel_stack: RESD 1

old_eflags: RESD 1

global cur_eax
cur_eax: RESD 1

extern tick

section .text

[GLOBAL switcher]
[EXTERN Scheduler]
[EXTERN timer_stub]
[GLOBAL context_switch2]

switcher:                             ; EFLAGS, CS, EIP are already pushed onto the stack
    cli
    pusha                             ; PUSH ALL REGISTERS ON THE STACK

    mov eax, [current_task]           ; Save the old task i.e, current task
    mov [old_task], eax

    mov [eax], esp
    mov esp, 0xCD00000                ; Change to some temporary stack, I dont want to take risk

    call Scheduler                    ; call our scheduler


    mov eax, [old_task]
    mov eax, [eax+4]                  ; compare the processID of old task and new task.
  ;  mov eax, 1
    mov ebx, [current_task]
    mov ebx, [ebx+4]
    cmp eax, ebx
    je context_switch2                ; If both tasks are of same process, No need to do full context switch
    ; <-------- HERE IS THE PROBLEM -------->
    ; I DONT KNOW WHY, But the above code ALWAYS returns NOT EQUAL i.e it dosent jump to context_switch2
    ; BUT IT SHOULD jump IF the current_task and the Old task belongs to the SAME process. But it isnt happening :/

    jmp context_switch2               ; <---- Here is the problem. This line should be commented to enable the MM system completely. But doing so makes it fail. Fix this
  ;  jmp loop1
                                      ; Else, Complete context switch

    mov eax, [old_task]
    mov eax, [eax+8]                  ; Old Process
    mov [old_process], eax

    mov eax, [current_task]
    mov eax, [eax+8]                  ; New Process
    mov [new_process], eax
                                      ; replace old_process with old_task

    mov eax, [eax]
    mov cr3, eax                      ; SWITCH TO New Process's page directory

    mov eax, [current_task]
    mov esp, [eax]

    push eax
    popa
    mov eax, 0xFEE000B0               ; APIC End Of Interrupt
    mov dword [eax], 0
    pop eax

    iretd

context_switch2:
    cli

  ;  call timer_stub                  ; To record how many times did our scheduler worked Successfully

    mov eax, [current_task]
    mov esp, [eax]                    ; Load new esp

    popa                              ; Popa

  ;  mov eax, 1                       ; A functionality to DBUG scheduler at any given cycle 'x' where x = 1,2,3....
  ;  mov ebx, [tick]                  ; COMMENT THESE TO RUN MULTITASKING AT FULL
  ;  cmp eax,ebx
  ;  je secondout

    push eax
    mov eax, 0xFEE000B0                ; APIC Timer End Of Interrupt
    mov dword [eax], 0
    pop eax
  ;  mov dx, 0x20                      ; PIT Timer End Of Interrupt
  ;  mov ax, 0x20
  ;  out dx, ax

    iretd

secondout:
    iret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

loop1:
    jmp loop2

loop2:
    jmp loop1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[GLOBAL Scheduler_init]

Scheduler_init:
    mov eax, [current_task]
    mov esp, [eax]                 ; Load new esp

    popa                              ; Popa

    iretd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[GLOBAL get_eax]

get_eax:
    mov [cur_eax], eax
    ret

[GLOBAL reload_eax]

reload_eax:
    mov eax, [cur_eax]
    ret
