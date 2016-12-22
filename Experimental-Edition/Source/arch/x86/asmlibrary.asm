section .bss

section .text

[EXTERN vga_mem]
[EXTERN buff]
[EXTERN mouse_buff]

[GLOBAL Enable_SSE]

Enable_SSE:
                                            ;now enable SSE and the like
  mov eax, cr0
  and ax, 0xFFFB		                        ;clear coprocessor emulation CR0.EM
  or ax, 0x2		                           	;set coprocessor monitoring  CR0.MP
  mov cr0, eax
  mov eax, cr4
  or ax, 3 << 9	                           	;set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
  mov cr4, eax
  ret

[EXTERN SSE_available]
[GLOBAL check_SSE]

check_SSE:
  mov eax, 0x1
  cpuid
  test edx, 1<<25
  jz No_SSE
  ret

No_SSE:
  mov eax, 0x0
  mov [SSE_available], eax


[GLOBAL memcpy_sse]

memcpy_sse:
  mov edi, [esp+4]
  mov esi, [esp+8]
  mov ebx, [esp+12]

  loop_copy:
    prefetchnta [128+ESI]; //SSE2 prefetch
    prefetchnta [160+ESI];
    prefetchnta [192+ESI];
    prefetchnta [224+ESI];

    movdqa xmm0, [0+ESI]; //move data from src to registers
    movdqa xmm1, [16+ESI];
    movdqa xmm2, [32+ESI];
    movdqa xmm3, [48+ESI];
    movdqa xmm4, [64+ESI];
    movdqa xmm5, [80+ESI];
    movdqa xmm6, [96+ESI];
    movdqa xmm7, [112+ESI];

    movntdq [0+EDI], xmm0 ; //move data from registers to dest
    movntdq [16+EDI], xmm1
    movntdq [32+EDI], xmm2;
    movntdq [48+EDI], xmm3;
    movntdq [64+EDI], xmm4;
    movntdq [80+EDI], xmm5;
    movntdq [96+EDI], xmm6;
    movntdq [112+EDI], xmm7;

    add esi, 128;
    add edi, 128;
    dec ebx;

    jnz loop_copy; //loop please
  ret


[GLOBAL memset_sse]

memset_sse:
  mov edi, [esp+4]
  mov eax, [esp+8]
  mov ebx, [esp+12]

  loop_copy2:

;    movdqa xmm0, eax; //move data from src to registers
;    movdqa xmm1, eax;
;    movdqa xmm2, eax;
;    movdqa xmm3, eax;
;    movdqa xmm4, eax;
;    movdqa xmm5, eax;
;    movdqa xmm6, eax;
;    movdqa xmm7, eax;

    movntdq [0+EDI], xmm0 ; //move data from registers to dest
    movntdq [16+EDI], xmm1
    movntdq [32+EDI], xmm2;
    movntdq [48+EDI], xmm3;
    movntdq [64+EDI], xmm4;
    movntdq [80+EDI], xmm5;
    movntdq [96+EDI], xmm6;
    movntdq [112+EDI], xmm7;

    add esi, 128;
    add edi, 128;
    dec ebx;

    jnz loop_copy2; //loop please
  ret

[GLOBAL memcpy_rep]

memcpy_rep:
  mov edi, [esp+4]
  mov esi, [esp+8]
  mov ecx, [esp+12]

  rep movsd

  ret
