
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

    call Scheduler_t                    ; call our scheduler


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


    call Scheduler_t                    ; call our scheduler


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

    call Scheduler_t                    ; call our scheduler


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


    call Scheduler_t                    ; call our scheduler


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
