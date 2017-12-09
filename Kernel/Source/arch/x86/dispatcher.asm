section .bss

section .text

[EXTERN Scheduler_t]

;****Switcher Template for Manual (software interrupts) switching**** When switching within kernel space

[global switcher_general_t]
[global k_start]

;Symbol Map:
;    _cur_dir        ->        0x4284ACD1
;    system_dir      ->        0x4284ACD2
;    current_task    ->        0x4284ACD3
;    old_task        ->        0x4284ACD4
;    Scheduler       ->        0x4284ACD5
;    time_slice      ->        0x4284ACD6
;    Temporary Stack ->        0x4284CD00

switcher_general_t:
    cli
    
    push dword eax
    mov ax, ds
    cmp ax, 0x10
    je k_start                          ;   BRANCHING #1

    mov dword ebp, esp
    mov dword esp, [esp+16]           ;   Switch to Thread Stack
    mov dword eax, esp
    push dword 0x23
    push dword eax
    pushf
    push dword 0x1B
    push dword [ebp+4]

    push eax

k_start:
    pop eax

    pusha
    
    mov ax, ds 
    push dword eax
    mov dword ebx, [0x4284ACD1]
    mov dword eax, [0x4284ACD3]         ;   Thread Structure
    mov dword [eax+12], ebx             ;   Update Thread's Dir

    mov dword [eax], esp                ;   Save Esp

    mov dword esp, 0x4284CD00           ;   Change to some temporary stack, I dont want to take risk
    call [0x4284ACD5]                   ;   call our scheduler
    
    mov dword ebx, [0x4284ACD3]         ;   Get New Task Entry
    mov dword ebx, [ebx+4]              ;   Get Page Directory of new Task
    mov dword eax, cr3
    cmp eax, ebx                        ;   Compare current cr3 and new pgdir
    je k_nw                             ;    BRANCHING #2

    mov dword eax, ebx
    mov dword cr3, eax                  ;   Switch to New Page Directory
    mov dword [0x4284ACD1], eax         ;   Update Current Dir

k_nw:
    mov dword eax, [0x4284ACD3]         ;   Get New Thread Entry Struct
    mov dword esi, eax
    mov dword eax, [eax]                ;   Get New Task Struct
    mov dword [0x4284ACD3], eax         ;   Save The Task Structure in Current_Task instead of Task Structure Entry!

    mov dword esp, [eax]                ;   Load New Stack

    pop dword eax
    mov ds, ax 
    mov es, ax 
    mov fs, ax 
    mov gs, ax

    popa                                ;   Unload all Registers

    push dword eax                            ;   Safety Purpose
    push dword edx
    mov dx, 0x20                      ; PIC Timer End Of Interrupt
    mov ax, 0x20
    out dx, ax
    mov dword eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword edx, [0x4284ACD6]
    mov dword [eax], edx
    pop dword edx
    pop dword eax

    iretd


switcher_end_t: