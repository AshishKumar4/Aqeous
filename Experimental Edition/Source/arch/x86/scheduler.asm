section .bss

section .text

[GLOBAL Scheduler_t]
[GLOBAL Scheduler_end_t]


;Symbol Table->
;      top_queue           ->        0x4284ABD1
;      reached_bottom      ->        0x4284ABD2
;      bottom_task         ->        0x4284ABD3
;      current_task        ->        0x4284ABD4
;      LAST_QUEUE          ->        0x4284ABD5
;      time_slice          ->        0x4284ABD6

;The Scheduler Rewritten in Assembly

Scheduler_t:
  mov dword edx, [0x4284ABD1]

  mov dword eax, [0x4284ABD2]
  cmp dword eax, 0
  jne Bottom_Schedule_t

  mov dword eax, [edx]
  cmp dword eax, 1
  je StepDown_Schedule_t
                                                  ; GENERAL
  mov eax, edx                                    ; first_curr  = _q + 1
  add dword eax, 4

  mov dword ebx, [edx]                                    ; last_curr
  shl dword ebx, 2
  add dword ebx, edx

  sub dword [edx], 1
  add dword edx, 4096
  add dword [edx], 1

  mov dword ecx, [edx]                                    ; last_lower
  shl dword ecx, 2
  add dword ecx, edx

  mov dword edx, [eax]                                  ; *last_lower = *first_curr
  mov dword [ecx], edx

  mov dword edx, [ebx]                                  ; *first_curr = *last_curr
  mov dword [eax], edx

  mov dword [ebx], 0                                    ; *last_curr = 0

  mov dword ebx, [ecx]
  mov dword [0x4284ABD4], ebx                         ; 0x4284ABD4 = *last_lower
  mov dword [ebx+24], ecx                               ; 0x4284ABD4->active = last_lower

  mov dword [0x4284ABD6], 10000


  ret

HALT:
  hlt


StepDown_Schedule_t:
  mov dword eax, edx                                    ; first_curr  = _q + 1
  add dword eax, 4

  sub dword [edx], 1
  add dword edx, 4096
  add dword [edx], 1

  mov dword ecx, [edx]                                    ; last_lower
  shl dword ecx, 2
  add dword ecx, edx

  mov dword ebx, [eax]                                  ; *last_lower = *first_curr
  mov dword [ecx], ebx

  mov dword [eax], 0                              ; *first_curr = 0

  mov dword [0x4284ABD4], ebx                         ; 0x4284ABD4 = *last_lower
  mov dword [ebx+24], ecx                               ; 0x4284ABD4->active = last_lower

  mov dword [0x4284ABD1], edx                            ; 0x4284ABD1 = _q

  mov dword ebx, edx
  mov dword eax, [0x4284ABD5]
  cmp dword eax, ebx
  je bottomReached_exit_t

  mov dword eax, [0x4284ABD1]
  and dword eax, 0xfffff
  shr dword eax, 5
  mov dword [0x4284ABD6], eax

  ret

bottomReached_exit_t:
  mov dword [0x4284ABD2], 1

  mov dword eax, [0x4284ABD1]
  and dword eax, 0xfffff
  shr dword eax, 5
  mov dword [0x4284ABD6], eax

  ret

Bottom_Schedule_t:
  mov dword eax, [edx]
  mov dword ebx, [0x4284ABD3]
  cmp dword ebx, eax
  jl alternate_flow_t

  mov dword [0x4284ABD3], 1
  jmp back_t

back_t:
  shl dword ebx, 2
  add dword ebx, edx
  cmp dword [ebx], 0
  jne option1_t                                      ; if(*tmp)
                                                   ; else
  add dword [0x4284ABD3], 1
  jmp back_t

option1_t:
  mov dword eax, [ebx]
  mov dword [0x4284ABD4], eax
  mov dword [eax + 24], ebx

  mov dword eax, [0x4284ABD1]
  and dword eax, 0xfffff
  shr dword eax, 5
  mov dword [0x4284ABD6], eax

  ret

alternate_flow_t:
  add dword [0x4284ABD3], 1
  jmp back_t

Scheduler_end_t:


;********************CODE TO STOP SPECIFIC CPU WHEN REQUIRED***********************

;  mov dword eax, 0xfee00020                 ; LAPIC ID entry
;  mov dword eax, [eax]
;
;  shr eax, 24
;  cmp eax, 0
;  jne HALT
