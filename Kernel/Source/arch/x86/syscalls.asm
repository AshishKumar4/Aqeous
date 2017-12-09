section .bss

section .text

[EXTERN system_dir]
[EXTERN current_pdir]

[GLOBAL syscall_vector_t]
[GLOBAL syscall_vector_end_t]

[GLOBAL syscall_ia32_vector_t]
[GLOBAL syscall_ia32_vector_end_t]
;[EXTERN syscall_ia32_handler]

;Symbol Map:
;    current_pdir    ->        0x4284ACD1
;    system_dir      ->        0x4284ACD2
;    current_task    ->        0x4284ACD3
;    old_task        ->        0x4284ACD4
;    syscall_handler ->        0x4284ACD5
;    Old Stack       ->        0x4284ACD6
;    temp_pdir       ->        0x4284ACD7
;    Temporary Stack ->        0x4284CD00

[GLOBAL switch_ToUserspace]
[GLOBAL switch_ToKernelspace]
[EXTERN kernel_main]
[GLOBAL ttll]
[EXTERN kernel_test]
[EXTERN user_test]

switch_ToKernelspace:

 ; cli 
 ; iretd
  pop eax 
  pop ebx 
  pop ebx 
  pop ecx 

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov eax, ecx
  push 0x10
  push eax
  pushf
  push 0x08
  push kernel_test
  iretd
  hlt

switch_ToUserspace:
 ; cli
 ; int 50
 ; iretd
  pop eax 
  pop ebx 
  pop ecx 
 ;; pop edx
 ; pop esi 
 ; pop edi
 ; hlt
 ; iret

  mov ax, 0x23
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
;  pop eax
;  pop eax
;  pop eax
;  pop eax
;  pop eax
;  pop eax

  mov eax, esp
  push 0x23
  push eax
  pushf
  pop eax 
  or eax, 0x200
  push eax
  push 0x1B
  push kernel_main

;  hlt

  iret

ttll:
  ;int 50
  nop
  nop
  nop 
  nop
  jmp user_test
  hlt
  nop
  nop
  nop
  nop
  nop
  nop
  ;hlt 
  jmp ttll;

  cli

  pop eax
  pop ebx
  mov ebx, 0x18   ; Code segment for user space
  push ebx
  push eax

  mov ax, 0x20
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  iretd


syscall_vector_t:
  cli

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  
  mov eax, cr3
  mov ebx, eax
  mov eax, [0x4284ACD2]
  mov cr3, eax
  mov [0x4284ACD7], ebx
 ; mov [0x4284ACD7], eax

  mov eax, esp
  mov [0x4284ACD6], eax

  mov eax, [0x4284CD00]
  mov esp, eax

  call [0x4284ACD5]

  mov eax, [0x4284ACD6]
  mov esp, eax

  mov eax, [0x4284ACD7]
 ; cmp eax, 0x1d07000
;  je tl
;  hlt
;tl:
 ; mov [0x4284ACD7], eax
  mov cr3, eax

  mov ax, 0x23
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
;  popa
  sti
  iretd

syscall_vector_end_t:
  ret

;Symbol Map:
;    _cur_dir        ->        0x4284ACD1
;    system_dir      ->        0x4284ACD2
;    current_task    ->        0x4284ACD3
;    old_task        ->        0x4284ACD4
;    syscall_handler ->        0x4284ACD5
;    Old Stack       ->        0x4284ACD6
;    Temporary Stack ->        0x4284CD00

syscall_ia32_vector_t:
  cli

  mov eax, cr3
  mov [0x4284ACD1], eax
  mov eax, [0x4284ACD2]
  mov cr3, eax

  mov eax, esp
  mov [0x4284ACD6], eax

  mov eax, [0x4284CD00]
  mov esp, eax

  call [0x4284ACD5]

  mov eax, [0x4284ACD1]
  mov cr3, eax

  mov eax, [0x4284ACD6]
  mov esp, eax

  sysexit

syscall_ia32_vector_end_t:
  ret
