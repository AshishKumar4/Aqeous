section .bss

global faulting_address
faulting_address: RESD 1

global current_pdir
current_pdir: RESD 1

global CancerCure_addr
CancerCure_addr: RESD 1

esp_backup: RESD 1
eax_backup: RESD 1

tmp_esp: RESD 4096

section .text
[EXTERN system_dir]
[EXTERN pageFault_caller]
[EXTERN CancerCure]

[GLOBAL pageFault_handler]
[GLOBAL coprocessor_handler]
[GLOBAL invalidInstr_handler]
[GLOBAL doubleFault_handler]
[GLOBAL generalProtectionFault_handler]
[GLOBAL breakpoint_handler]

temporary_stack:
  dd tmp_esp

generalProtectionFault_handler:
;  cli
;  pop eax
;  pop eax
;  mov ebx, [CancerCure_addr]
;  push ebx
;  push eax
;  mov eax, 0x01
;  push eax
;  iretd

  cli
  pop eax
  pop ebx
  pop ecx
  pop edx

  hlt

doubleFault_handler:
;  cli
;  pop eax
;  pop eax
;  mov ebx, [CancerCure_addr]
;  push ebx
;  push eax
;  mov eax, 0x01
;  push eax
;  iretd

  cli
  pop eax
  pop ebx
  pop ecx
  pop edx
  hlt
  iretd

coprocessor_handler:
  cli
;  pop eax
;  mov ebx, [CancerCure_addr]
;  push ebx
;  push eax
;  mov eax, 0x01
;  push eax
;  iretd

  cli
  pop eax
  pop ebx
  pop ecx
  pop edx
  hlt

breakpoint_handler:
  iretd
  pop eax
  pop ebx
  pop ecx
  pop edx
  hlt
  cli
  pop eax
  mov ebx, [CancerCure_addr]
  push ebx
  push eax
  mov eax, 0x01
  push eax
  iretd

pageFault_handler:
  cli

;  pop eax
;  pop ebx
;  pop ecx
;  pop edx
;  hlt

  pusha
  mov ebx, cr3

  mov eax, [system_dir]
  mov cr3, eax

  mov [current_pdir], ebx

  mov eax, esp
  mov [esp_backup], eax

  mov eax, tmp_esp

  mov esp, eax

;  hlt
  mov eax, cr2
  mov [faulting_address], eax

  call pageFault_caller

  mov eax, [esp_backup]
  mov esp, eax

  popa

  mov [eax_backup], eax
  pop eax

  push eax

  mov eax, 0xFEE000B0
  mov dword [eax], 0

  mov eax, [current_pdir]
  mov cr3, eax
  pop eax

  iretd

invalidInstr_handler:
  cli
;  pop eax
;  mov ebx, [CancerCure_addr]
;  push ebx
;  push eax
;  mov eax, 0x01
;  push eax
;  iretd
  pop eax
  pop ebx
  pop ecx
  pop edx
  hlt
