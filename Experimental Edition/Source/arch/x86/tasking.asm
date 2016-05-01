section .bss

global current_task
current_task: RESD 1

global old_task
old_task: RESD 1

global new_process
new_process: RESD 1

global old_process
old_process: RESD 1

eax_backup: RESD 1
ebx_backup: RESD 1
ecx_backup: RESD 1
edx_backup: RESD 1

global kernel_stack
kernel_stack: RESD 1

old_eflags: RESD 1

global cur_eax
cur_eax: RESD 1

section .text

[GLOBAL switcher]
[EXTERN Scheduler]

switcher:                              ; EFLAGS, CS, EIP are already pushed onto the stack

    pusha                              ; PUSH ALL REGISTERS ON THE STACK

    mov eax, [current_task]            ; Save the old task i.e, current task
    mov [old_task], eax

    call Scheduler                    ; call our scheduler


    mov eax, [old_task]
    mov ebx, [current_task]

    mov ecx, [eax+8]                  ; compare the processID of old task and new task.
    mov edx, [ebx+8]
    cmp ecx, edx
    je context_switch2                ; If both tasks are of same process, No need to do full context switch

                                      ; Else, Complete context switch

    mov ecx, [eax+20]                 ; Old Process
    mov [old_process], ecx

    mov edx, [ebx+20]                 ; New Process
    mov [new_process], edx


    mov [eax+12], esp                 ; TODO: Find out shall each thread should have a seperate stack? if so,
                                      ; replace old_process with old_task

    mov edx, [edx]
    mov cr3, edx                      ; TODO: SWITCH TO New Process's page directory

    mov esp, [ebx+12]


    popa

    mov dx, 0x20                      ; Send EOI
    mov ax, 0x20
    out dx, ax

    iret

context_switch2:
    mov [eax+12], esp                 ; Save old esp

    mov esp, [ebx+12]                 ; Load new esp

    popa                              ; Popa

    mov dx, 0x20                      ; Send EOI
    mov ax, 0x20
    out dx, ax

    iret

[GLOBAL get_eax]

get_eax:
    mov [cur_eax], eax
    ret

[GLOBAL reload_eax]

reload_eax:
    mov eax, [cur_eax]
    ret


        ;mov eax, [current_task]            ; Save the old task i.e, current task

        ;pop dword [eax]                    ; POP EIP, CS, EFLAGS so we can access the eflags
        ;pop dword [eax+4]
        ;pop dword [eax+8]

        ;mov ebx, [eax+8]
        ;mov [old_eflags], ebx              ; Mission successfull

        ;push dword [eax+8]                 ; Revert the change
        ;push dword [eax+4]
        ;push dword [eax]
