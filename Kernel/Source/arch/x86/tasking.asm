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
[EXTERN kernel_main]

[global switcher_general_t]

;Symbol Map:
;    _cur_dir        ->        0x4284ACD1
;    system_dir      ->        0x4284ACD2
;    current_task    ->        0x4284ACD3
;    old_task        ->        0x4284ACD4
;    Scheduler       ->        0x4284ACD5
;    time_slice      ->        0x4284ACD6
;    Temporary Stack ->        0x4284CD00


switcher_ksp_t:                        ; NEW Dispatcher
switcher_general_t:
    cli
    
    push dword eax
    mov ax, ds
    cmp ax, 0x10
    je k_start                          ;   BRANCHING #1

        ; IF here, Means USER Mode
        
        ; This Procedure Saves the critical stack section in thread's stack
        mov dword ebp, esp
        mov dword esp, [esp+16]           ;   Switch to Thread Stack
        mov dword eax, esp

        push dword 0x23
        push dword eax                    ;   Push Stack
        push dword [ebp+12]               ;   Push Eglags
        push dword 0x1B
        push dword [ebp+4]                ;   Push Eip
        push eax

k_start:
    pop eax

    pusha
    
    mov ax, ds 
    push dword eax

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; CON1
;    mov dword ebx, [0x4284ACD1]
;    mov dword eax, [0x4284ACD3]         ;   Thread Structure
;    mov dword eax, [eax+12]
;    cmp ebx, [0x4284ACD2]
;    je con 
;    mov dword ecx, 0x1
;    hlt 
;con:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    mov dword ebx, [0x4284ACD1]
    mov dword eax, [0x4284ACD3]         ;   Thread Structure
    mov dword [eax+12], ebx             ;   Update Thread's Dir

    mov dword [eax], esp                ;   Save Esp

    mov dword esp, 0x4284CD00           ;   Change to some temporary stack, I dont want to take risk

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; CON3
;    mov dword eax, [0x4284ACD3]         ;       CON3
;    mov dword esi, eax
;    mov dword eax, [eax+28]
;    mov dword edx, eax
;    mov dword ebx, [eax+4]
;    cmp ebx, [0x4284ACD2]
;    je con3
;    mov dword eax, [eax]
;    mov dword ecx, 0x3
;    hlt 
;con3:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

k_schedule:
    call [0x4284ACD5]                   ;   call our scheduler
    
    mov dword ebx, [0x4284ACD3]         ;   Get New Task Entry
    mov dword ebx, [ebx+4]              ;   Get Page Directory of new Task

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; CON2
;    cmp ebx, [0x4284ACD2]               ;       CON2
;    je con2 
;    mov dword eax, [0x4284ACD3]
;    mov dword eax, [eax]
;    mov dword ecx, 0x2
;    hlt 
;con2:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    mov dword eax, cr3
    cmp eax, ebx                        ;   Compare current cr3 and new pgdir
    je k_nw 

    k_switch:                               ;   Switch Page Directory
        mov dword eax, ebx
        mov dword cr3, eax                  ;   Switch to New Page Directory
        mov dword [0x4284ACD1], eax         ;   Update Current Dir

k_nw:
    mov dword eax, [0x4284ACD3]         ;   Get New Thread Entry Struct
    mov dword esi, eax
    mov dword eax, [eax]                ;   Get New Task Struct
    mov dword [0x4284ACD3], eax         ;   Save The Task Structure in Current_Task instead of Task Structure Entry!

k_check:
    mov ebx, [eax+32]
    cmp ebx, 0x42841999                 ;   Check if magic not set, Schedule again!
    je kkk

        mov ecx, eax
        mov eax, [0x4284ACD2]               ;   Switch to kernel directory
        mov cr3, eax

        mov ebx, [ecx+24]
        mov eax, [0x42843333]
        mov edx, [eax+28]
        mov [ebx], edx                  ;   Replace the queue entry of zombie task with that of spurious task

        ;jmp k_check

kkk:
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
;    mov eax, 0xFEE000B0                ; APIC Timer End Of Interrupt
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

    mov ebx, esi
    mov eax, 0x1
    cmp ax, bx
    jne tel

    pusha 
    push ds

    mov dword eax, 0xFEE00380               ; Reset LVT Timer counter
    mov dword [eax], 0

    mov dword eax, [0x4284ACD2]
    mov dword cr3, eax

    mov dword ebx, [0x4284ACD1]         ; *Remove these lines once userspace has been implemented.
    mov dword eax, [0x4284ACD3]
    mov dword [0x4284ACD4], eax               ; Save the old task i.e, current task
    mov dword [eax+12], ebx                 ; Save the old page directory.

    mov dword [eax], esp
    mov dword esp, 0x4284CD00                ; Change to some temporary stack, I dont want to take risk

   ; call [0x4284ACD5]


    mov dword ebx, [0x4284ACD4]
    mov dword [0x4284ACD3], ebx 
    mov dword eax, [0x4284ACD3]

    mov dword esp, [eax]

    mov dword eax, [eax+12]
    mov dword [0x4284ACD1], eax

    mov dword cr3, eax


    pop eax 
    popa

    ;hlt
    pop eax 
    pop ebx 
    pop ecx 

   ; mov edx, esp 
   ; push 0x10
   ; push edx 
   ; push ecx 
   ; push ebx 
   ; push eax

    pop edx 
    pop esi 
    pop edi 
    hlt
    iretd

tel: