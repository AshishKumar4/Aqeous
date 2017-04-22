; contains code for reference, easy understanding of concepts
section .bss

section .text

[GLOBAL Scheduler]
[GLOBAL Scheduler_end]

[GLOBAL Scheduler_t]
[GLOBAL Scheduler_end_t]

[EXTERN top_queue]
[EXTERN reached_bottom]
[EXTERN bottom_task]
[EXTERN current_task]
[EXTERN LAST_QUEUE]
[EXTERN time_slice]

;Symbol Table->
;      top_queue           ->        0x4284ABD1
;      reached_bottom      ->        0x4284ABD2
;      bottom_task         ->        0x4284ABD3
;      current_task        ->        0x4284ABD4
;      LAST_QUEUE          ->        0x4284ABD5
;      time_slice          ->        0x4284ABD6

;The Scheduler Rewritten in Assembly

Scheduler_t:
  mov dword edx, [top_queue]

  mov dword eax, [reached_bottom]
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
  mov dword [current_task], ebx                         ; current_task = *last_lower
  mov dword [ebx+24], ecx                               ; current_task->active = last_lower

  mov dword [time_slice], 10000


  ret


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

  mov dword [current_task], ebx                         ; current_task = *last_lower
  mov dword [ebx+24], ecx                               ; current_task->active = last_lower

  mov dword [top_queue], edx                            ; top_queue = _q

  mov dword ebx, edx
  mov dword eax, [LAST_QUEUE]
  cmp dword eax, ebx
  je bottomReached_exit_t

  mov dword eax, [top_queue]
  and dword eax, 0xfffff
  shr dword eax, 5
  mov dword [time_slice], eax

  ret

bottomReached_exit_t:
  mov dword [reached_bottom], 1

  mov dword eax, [top_queue]
  and dword eax, 0xfffff
  shr dword eax, 5
  mov dword [time_slice], eax

  ret

Bottom_Schedule_t:
  mov dword eax, [edx]
  mov dword ebx, [bottom_task]
  cmp dword ebx, eax
  jl alternate_flow_t

  mov dword [bottom_task], 1
  jmp back_t

back_t:
  shl dword ebx, 2
  add dword ebx, edx
  cmp dword [ebx], 0
  jne option1_t                                      ; if(*tmp)
                                                   ; else
  add dword [bottom_task], 1
  jmp back_t

option1_t:
  mov dword eax, [ebx]
  mov dword [current_task], eax
  mov dword [eax + 24], ebx

  mov dword eax, [top_queue]
  and dword eax, 0xfffff
  shr dword eax, 5
  mov dword [time_slice], eax

  ret

alternate_flow_t:
  add dword [bottom_task], 1
  jmp back_t

Scheduler_end_t:


Scheduler:
  mov dword edx, [top_queue]

  mov dword eax, [reached_bottom]
  cmp dword eax, 0
  jne Bottom_Schedule

  mov dword eax, [edx]
  cmp dword eax, 1
  je StepDown_Schedule
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
  mov dword [current_task], ebx                         ; current_task = *last_lower
  mov dword [ebx+24], ecx                               ; current_task->active = last_lower

  mov dword [time_slice], 10000


  ret


StepDown_Schedule:
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

  mov dword [current_task], ebx                         ; current_task = *last_lower
  mov dword [ebx+24], ecx                               ; current_task->active = last_lower

  mov dword [top_queue], edx                            ; top_queue = _q

  mov dword ebx, edx
  mov dword eax, [LAST_QUEUE]
  cmp dword eax, ebx
  je bottomReached_exit

  mov dword eax, [top_queue]
  and dword eax, 0xfffff
  shr dword eax, 5
  mov dword [time_slice], eax

  ret

bottomReached_exit:
  mov dword [reached_bottom], 1

  mov dword eax, [top_queue]
  and dword eax, 0xfffff
  shr dword eax, 5
  mov dword [time_slice], eax

  ret

Bottom_Schedule:
  mov dword eax, [edx]
  mov dword ebx, [bottom_task]
  cmp dword ebx, eax
  jl alternate_flow

  mov dword [bottom_task], 1
  jmp back

back:
  shl dword ebx, 2
  add dword ebx, edx
  cmp dword [ebx], 0
  jne option1                                      ; if(*tmp)
                                                   ; else
  add dword [bottom_task], 1
  jmp back

option1:
  mov dword eax, [ebx]
  mov dword [current_task], eax
  mov dword [eax + 24], ebx

  mov dword eax, [top_queue]
  and dword eax, 0xfffff
  shr dword eax, 5
  mov dword [time_slice], eax

  ret

alternate_flow:
  add dword [bottom_task], 1
  jmp back

Scheduler_end:
