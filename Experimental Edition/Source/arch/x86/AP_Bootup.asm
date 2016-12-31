section .bss

section .text


BITS 16

[GLOBAL AP_startup_Code]
[GLOBAL AP_startup_Code_end]
[EXTERN trial_code]
[EXTERN enable_pic]

AP_startup_Code:
  cli

  xor ax, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  xor esp, esp
  mov sp, 0x4284

  xor eax, eax
  mov edx, 0x10
  mov cr0, edx

  mov si, 0x3240
  lgdt [ds:si]

  mov si, 0x3250
  lidt [ds:si]


  mov	eax, cr0		; set bit 0 in cr0--enter pmode
  or	eax, 1
  mov	cr0, eax

  mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
  mov ds, ax        ; Load all data segment selectors
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
;  mov edx, 0x11
;  mov cr0, edx

;  mov eax, 0x6576


BITS 32
  mov eax, 1
  mov bx, 0x0500
  add dword [ds:bx], 0x1

  jmp 0x08:0x00001000
  hlt

AP_startup_Code_end:
  ret

[GLOBAL test_entry]
[GLOBAL gdt_setup2]

[GLOBAL pmode_AP_code]
[GLOBAL pmode_AP_code_end]
[GLOBAL INTbasedPmodeTrampoline]
[EXTERN test_handler]
[EXTERN AP_startup_Code_sz]
[EXTERN system_dir]

INTbasedPmodeTrampoline:
  ; Determines the CPU which is currently running the code and redirects it accordingly
  pop eax
  pop eax
  pop ebx
  pop eax

  mov dword eax, 0x00000500
  add dword [eax], 1

  mov dword eax, 0xfee00020                 ; LAPIC ID entry
  mov dword eax, [eax]

  shr eax, 11
  add dword eax, 0x1010
  add dword eax, [AP_startup_Code_sz]

  mov dword ebx, 0
  push ebx
  mov dword ebx, 0x202
  push ebx
  mov dword ebx, 0x08
  push ebx
  push eax
  iretd

pmode_AP_code:
  mov dword eax, [0x5599]
  cmp eax, 0

  je pmode_AP_code

  cmp eax, 0x4284               ; Enable Paging and other stuffs
  je AP_enable_Paging

  cmp eax, 0x3240
  jne pmode_AP_code

  mov dword eax, 0xfee000b0
  mov dword [eax], 0
  ; Add it to the SMP System
  int 52
  
  hlt

AP_enable_Paging:
  cli
;  xor eax, eax
  mov dword eax, 0xfee00370
  mov dword [eax], 0x1f

  mov dword eax, 0xfee00080
  mov dword [eax], 0

  mov dword eax, 0xfee000e0
  mov dword [eax], 0xffffffff

  mov dword eax, 0xfee000d0
  mov dword [eax], 0x01000000

  mov dword eax, 0xfee000f0
  mov dword [eax], 0x1ff

  mov dword eax, 0xfee003e0
  mov dword [eax], 0x3

  mov dword eax, 0xfee00380
  mov dword [eax], 0xFFFFFFFF

  mov dword eax, 0xfee00320
  mov dword [eax], 0x10000
  mov dword [eax], 51

  mov dword eax, 0xfee003e0
  mov dword [eax], 0x3

  mov dword eax, 0xfee00380
  mov dword [eax], 1

  mov dword eax, [system_dir]
  mov cr3, eax
  mov dword eax, 0x80000011
  mov cr0, eax

  jmp pmode_AP_code

pmode_AP_code_end:
  ret


[GLOBAL APIC_Error_vector]
[GLOBAL APIC_Error_vector_end]

APIC_Error_vector:
  add dword [0x6679], 1
  hlt
  call [0x42843240]
  ret

APIC_Error_vector_end:
  ret

[GLOBAL abcdt1]

abcdt1:
  hlt
