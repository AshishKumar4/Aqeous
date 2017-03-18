	.file	"kernel.c"
	.comm	printlockLocked,4,4
	.text
	.align 16
	.globl	posix_time
	.type	posix_time, @function
posix_time:
.LFB4:
	.cfi_startproc
	movl	$65535, %eax
	ret
	.cfi_endproc
.LFE4:
	.size	posix_time, .-posix_time
	.globl	mousex
	.data
	.align 4
	.type	mousex, @object
	.size	mousex, 4
mousex:
	.long	512
	.globl	mousey
	.align 4
	.type	mousey, @object
	.size	mousey, 4
mousey:
	.long	384
	.globl	mousedeltax
	.section	.bss
	.align 4
	.type	mousedeltax, @object
	.size	mousedeltax, 4
mousedeltax:
	.zero	4
	.globl	mousedeltay
	.align 4
	.type	mousedeltay, @object
	.size	mousedeltay, 4
mousedeltay:
	.zero	4
	.local	mouse_bytes
	.comm	mouse_bytes,3,1
	.local	cycle
	.comm	cycle,4,4
	.globl	mouse_buff
	.align 4
	.type	mouse_buff, @object
	.size	mouse_buff, 4
mouse_buff:
	.zero	4
	.globl	background
	.data
	.align 4
	.type	background, @object
	.size	background, 4
background:
	.long	90
	.comm	tempbuff,4,4
	.comm	offset,4,4
	.globl	width
	.align 4
	.type	width, @object
	.size	width, 4
width:
	.long	1022
	.globl	height
	.align 4
	.type	height, @object
	.size	height, 4
height:
	.long	766
	.comm	general_buff,4,4
	.globl	cx0
	.align 4
	.type	cx0, @object
	.size	cx0, 4
cx0:
	.long	512
	.globl	cx1
	.align 4
	.type	cx1, @object
	.size	cx1, 4
cx1:
	.long	512
	.globl	cy0
	.align 4
	.type	cy0, @object
	.size	cy0, 4
cy0:
	.long	384
	.globl	cy1
	.align 4
	.type	cy1, @object
	.size	cy1, 4
cy1:
	.long	384
	.comm	SMI_CMD,4,4
	.comm	ACPI_ENABLE,1,1
	.comm	ACPI_DISABLE,1,1
	.comm	PM1a_CNT,4,4
	.comm	PM1b_CNT,4,4
	.comm	SLP_TYPa,2,2
	.comm	SLP_TYPb,2,2
	.comm	SLP_EN,2,2
	.comm	SCI_EN,2,2
	.comm	PM1_CNT_LEN,1,1
	.text
	.align 16
	.globl	faster_memcpy
	.type	faster_memcpy, @function
faster_memcpy:
.LFB582:
	.cfi_startproc
	jmp	memcpy_faster
	.cfi_endproc
.LFE582:
	.size	faster_memcpy, .-faster_memcpy
	.align 16
	.globl	faster_memset
	.type	faster_memset, @function
faster_memset:
.LFB6:
	.cfi_startproc
	movl	12(%esp), %eax
	movl	8(%esp), %ecx
	movl	4(%esp), %edx
	testl	%eax, %eax
	je	.L3
	.align 16
.L7:
	addl	$4, %edx
	subl	$1, %eax
	movl	%ecx, -4(%edx)
	jne	.L7
.L3:
	rep ret
	.cfi_endproc
.LFE6:
	.size	faster_memset, .-faster_memset
	.align 16
	.globl	memcmp
	.type	memcmp, @function
memcmp:
.LFB7:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	24(%esp), %ebx
	movl	16(%esp), %esi
	movl	20(%esp), %edi
	testl	%ebx, %ebx
	je	.L16
	movzbl	(%esi), %ecx
	movzbl	(%edi), %edx
	cmpb	%cl, %dl
	jne	.L12
	movl	$1, %eax
	jmp	.L14
	.align 16
.L15:
	movzbl	(%esi,%eax), %ecx
	addl	$1, %eax
	movzbl	-1(%edi,%eax), %edx
	cmpb	%dl, %cl
	jne	.L12
.L14:
	cmpl	%ebx, %eax
	jne	.L15
.L16:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	xorl	%eax, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L12:
	.cfi_restore_state
	cmpb	%dl, %cl
	sbbl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	orl	$1, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE7:
	.size	memcmp, .-memcmp
	.align 16
	.globl	memcpy
	.type	memcpy, @function
memcpy:
.LFB8:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	24(%esp), %esi
	movl	20(%esp), %edx
	movl	16(%esp), %edi
	testl	%esi, %esi
	leal	(%edx,%esi), %ebx
	je	.L26
	movl	%edi, %ecx
	.align 16
.L24:
	addl	$1, %edx
	movzbl	-1(%edx), %eax
	addl	$1, %ecx
	cmpl	%edx, %ebx
	movb	%al, -1(%ecx)
	jne	.L24
.L26:
	movl	%edi, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE8:
	.size	memcpy, .-memcpy
	.align 16
	.globl	memmove
	.type	memmove, @function
memmove:
.LFB9:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	12(%esp), %eax
	movl	16(%esp), %ebx
	movl	20(%esp), %esi
	cmpl	%ebx, %eax
	jb	.L29
	testl	%esi, %esi
	je	.L35
	.align 16
.L37:
	movzbl	-1(%ebx,%esi), %ecx
	movb	%cl, -1(%eax,%esi)
	subl	$1, %esi
	jne	.L37
.L35:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L29:
	.cfi_restore_state
	testl	%esi, %esi
	je	.L35
	xorl	%ecx, %ecx
	.align 16
.L32:
	movzbl	(%ebx,%ecx), %edx
	movb	%dl, (%eax,%ecx)
	addl	$1, %ecx
	cmpl	%ecx, %esi
	jne	.L32
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE9:
	.size	memmove, .-memmove
	.align 16
	.globl	memset
	.type	memset, @function
memset:
.LFB10:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	16(%esp), %ebx
	movl	8(%esp), %eax
	movzbl	12(%esp), %ecx
	testl	%ebx, %ebx
	leal	(%eax,%ebx), %edx
	je	.L42
	.align 16
.L46:
	addl	$1, %eax
	movb	%cl, -1(%eax)
	cmpl	%edx, %eax
	jne	.L46
.L42:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE10:
	.size	memset, .-memset
	.align 16
	.globl	memcmp_fast
	.type	memcmp_fast, @function
memcmp_fast:
.LFB11:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	24(%esp), %ebx
	movl	16(%esp), %esi
	movl	20(%esp), %edi
	shrl	$2, %ebx
	testl	%ebx, %ebx
	je	.L55
	movl	(%esi), %ecx
	movl	(%edi), %edx
	sall	$2, %ebx
	movl	$4, %eax
	cmpl	%ecx, %edx
	je	.L53
	jmp	.L51
	.align 16
.L54:
	movl	(%esi,%eax), %ecx
	addl	$4, %eax
	movl	-4(%edi,%eax), %edx
	cmpl	%edx, %ecx
	jne	.L51
.L53:
	cmpl	%ebx, %eax
	jne	.L54
.L55:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	xorl	%eax, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L51:
	.cfi_restore_state
	cmpl	%edx, %ecx
	sbbl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	orl	$1, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE11:
	.size	memcmp_fast, .-memcmp_fast
	.align 16
	.globl	memcpy_fast
	.type	memcpy_fast, @function
memcpy_fast:
.LFB12:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	20(%esp), %ecx
	movl	12(%esp), %eax
	movl	16(%esp), %esi
	shrl	$2, %ecx
	testl	%ecx, %ecx
	je	.L66
	leal	0(,%ecx,4), %ebx
	xorl	%edx, %edx
	.align 16
.L64:
	movl	(%esi,%edx), %ecx
	movl	%ecx, (%eax,%edx)
	addl	$4, %edx
	cmpl	%ebx, %edx
	jne	.L64
.L66:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE12:
	.size	memcpy_fast, .-memcpy_fast
	.align 16
	.globl	memmove_fast
	.type	memmove_fast, @function
memmove_fast:
.LFB13:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	12(%esp), %eax
	movl	16(%esp), %ebx
	movl	20(%esp), %edx
	shrl	$2, %edx
	cmpl	%ebx, %eax
	jb	.L69
	testl	%edx, %edx
	je	.L75
	.align 16
.L77:
	movl	-4(%ebx,%edx,4), %ecx
	movl	%ecx, -4(%eax,%edx,4)
	subl	$1, %edx
	jne	.L77
.L75:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L69:
	.cfi_restore_state
	testl	%edx, %edx
	je	.L75
	xorl	%ecx, %ecx
	.align 16
.L72:
	movl	(%ebx,%ecx,4), %esi
	movl	%esi, (%eax,%ecx,4)
	addl	$1, %ecx
	cmpl	%ecx, %edx
	jne	.L72
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE13:
	.size	memmove_fast, .-memmove_fast
	.align 16
	.globl	memset_fast
	.type	memset_fast, @function
memset_fast:
.LFB14:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	16(%esp), %ebx
	movl	8(%esp), %eax
	movl	12(%esp), %ecx
	shrl	$2, %ebx
	testl	%ebx, %ebx
	leal	(%eax,%ebx,4), %edx
	je	.L82
	.align 16
.L86:
	addl	$4, %eax
	movl	%ecx, -4(%eax)
	cmpl	%edx, %eax
	jne	.L86
.L82:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE14:
	.size	memset_fast, .-memset_fast
	.align 16
	.globl	strlen
	.type	strlen, @function
strlen:
.LFB15:
	.cfi_startproc
	movl	4(%esp), %edx
	xorl	%eax, %eax
	cmpb	$0, (%edx)
	je	.L92
	.align 16
.L91:
	addl	$1, %eax
	cmpb	$0, (%edx,%eax)
	jne	.L91
	rep ret
.L92:
	rep ret
	.cfi_endproc
.LFE15:
	.size	strlen, .-strlen
	.align 16
	.globl	strcmp
	.type	strcmp, @function
strcmp:
.LFB16:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %edx
	movl	12(%esp), %ecx
	movzbl	(%edx), %eax
	movzbl	(%ecx), %ebx
	cmpb	%al, %bl
	jne	.L95
	testb	%bl, %bl
	jne	.L98
	jmp	.L101
	.align 16
.L99:
	testb	%al, %al
	je	.L101
.L98:
	addl	$1, %edx
	addl	$1, %ecx
	movzbl	(%edx), %eax
	movzbl	(%ecx), %ebx
	cmpb	%bl, %al
	je	.L99
.L95:
	cmpb	%bl, %al
	sbbl	%eax, %eax
	orl	$1, %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L101:
	.cfi_restore_state
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE16:
	.size	strcmp, .-strcmp
	.align 16
	.globl	strcmpx
	.type	strcmpx, @function
strcmpx:
.LFB17:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	20(%esp), %ebx
	movl	24(%esp), %ecx
	jmp	.L105
	.align 16
.L112:
	addl	$1, %ebx
	addl	$1, %ecx
.L105:
	movzbl	(%ecx), %esi
	movzbl	(%ebx), %eax
	movl	%esi, %edx
	cmpb	%dl, %al
	je	.L107
	movsbl	%al, %edi
	movsbl	%dl, %edx
	leal	-32(%edi), %ebp
	cmpl	%edx, %ebp
	je	.L107
	subl	$32, %edx
	cmpl	%edx, %edi
	jne	.L111
.L107:
	testb	%al, %al
	jne	.L112
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L111:
	.cfi_restore_state
	movl	%esi, %ecx
	cmpb	%cl, %al
	sbbl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	orl	$1, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE17:
	.size	strcmpx, .-strcmpx
	.align 16
	.globl	strncmp
	.type	strncmp, @function
strncmp:
.LFB18:
	.cfi_startproc
	movl	12(%esp), %ecx
	movl	4(%esp), %eax
	movl	8(%esp), %edx
	testl	%ecx, %ecx
	je	.L114
	jmp	strncmp.part.0
	.align 16
.L114:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE18:
	.size	strncmp, .-strncmp
	.align 16
	.globl	strcpy
	.type	strcpy, @function
strcpy:
.LFB19:
	.cfi_startproc
	movl	8(%esp), %edx
	movl	4(%esp), %eax
	movzbl	(%edx), %ecx
	.align 16
.L116:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L116
	movb	$0, (%eax)
	ret
	.cfi_endproc
.LFE19:
	.size	strcpy, .-strcpy
	.align 16
	.globl	strcat
	.type	strcat, @function
strcat:
.LFB20:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %eax
	movl	12(%esp), %ebx
	cmpb	$0, (%eax)
	je	.L122
	leal	1(%eax), %ecx
	.align 16
.L120:
	movl	%ecx, %edx
	addl	$1, %ecx
	cmpb	$0, -1(%ecx)
	jne	.L120
.L119:
	movzbl	(%ebx), %ecx
	.align 16
.L121:
	addl	$1, %edx
	addl	$1, %ebx
	movb	%cl, -1(%edx)
	movzbl	(%ebx), %ecx
	testb	%cl, %cl
	jne	.L121
	movb	$0, (%edx)
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L122:
	.cfi_restore_state
	movl	%eax, %edx
	jmp	.L119
	.cfi_endproc
.LFE20:
	.size	strcat, .-strcat
	.align 16
	.globl	strcmpy
	.type	strcmpy, @function
strcmpy:
.LFB21:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$8, %esp
	.cfi_def_cfa_offset 28
	movl	28(%esp), %edi
	movl	32(%esp), %edx
	movl	$0, (%esp)
	movzbl	(%edi), %eax
	movb	%al, 7(%esp)
.L127:
	xorl	%eax, %eax
	cmpb	$0, 7(%esp)
	je	.L134
	.align 16
.L135:
	addl	$1, %eax
	cmpb	$0, (%edi,%eax)
	jne	.L135
.L134:
	movl	(%esp), %ebp
	cmpl	%eax, %ebp
	ja	.L140
	movzbl	(%edx), %esi
	xorl	%ecx, %ecx
	xorl	%ebx, %ebx
	leal	(%edi,%ebp), %ebp
	movl	%esi, %eax
	testb	%al, %al
	movl	$0, %eax
	je	.L129
	.align 16
.L130:
	addl	$1, %eax
	cmpb	$0, (%edx,%eax)
	jne	.L130
	cmpl	%eax, %ecx
	jnb	.L145
	movzbl	(%edx,%ecx), %eax
	cmpb	%al, 0(%ebp,%ecx)
	sete	%al
	addl	$1, %ecx
	movzbl	%al, %eax
	addl	%eax, %ebx
	movl	%esi, %eax
	testb	%al, %al
	movl	$0, %eax
	jne	.L130
.L129:
	cmpl	%eax, %ebx
	je	.L138
	addl	$1, (%esp)
	jmp	.L127
.L140:
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L138:
	.cfi_restore_state
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L145:
	.cfi_restore_state
	xorl	%eax, %eax
	.align 16
.L132:
	addl	$1, %eax
	cmpb	$0, (%edx,%eax)
	jne	.L132
	jmp	.L129
	.cfi_endproc
.LFE21:
	.size	strcmpy, .-strcmpy
	.align 16
	.globl	strchr
	.type	strchr, @function
strchr:
.LFB22:
	.cfi_startproc
	movl	4(%esp), %eax
	movzbl	8(%esp), %ecx
	jmp	.L147
	.align 16
.L149:
	addl	$1, %eax
	testb	%dl, %dl
	je	.L150
.L147:
	movzbl	(%eax), %edx
	cmpb	%cl, %dl
	jne	.L149
	rep ret
	.align 16
.L150:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE22:
	.size	strchr, .-strchr
	.align 16
	.globl	strspn
	.type	strspn, @function
strspn:
.LFB23:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	xorl	%eax, %eax
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %esi
	movl	20(%esp), %edi
	movzbl	(%esi), %ebx
	testb	%bl, %bl
	je	.L151
	.align 16
.L156:
	movl	%edi, %edx
	jmp	.L153
	.align 16
.L154:
	addl	$1, %edx
	testb	%cl, %cl
	je	.L151
.L153:
	movzbl	(%edx), %ecx
	cmpb	%bl, %cl
	jne	.L154
	addl	$1, %eax
	movzbl	(%esi,%eax), %ebx
	testb	%bl, %bl
	jne	.L156
.L151:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE23:
	.size	strspn, .-strspn
	.align 16
	.globl	strcspn
	.type	strcspn, @function
strcspn:
.LFB24:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	xorl	%eax, %eax
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %esi
	movl	20(%esp), %edi
	movzbl	(%esi), %ebx
	testb	%bl, %bl
	je	.L164
	.align 16
.L171:
	movl	%edi, %edx
	jmp	.L168
	.align 16
.L169:
	addl	$1, %edx
	testb	%cl, %cl
	je	.L175
.L168:
	movzbl	(%edx), %ecx
	cmpb	%bl, %cl
	jne	.L169
.L164:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L175:
	.cfi_restore_state
	addl	$1, %eax
	movzbl	(%esi,%eax), %ebx
	testb	%bl, %bl
	jne	.L171
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE24:
	.size	strcspn, .-strcspn
	.align 16
	.globl	strtok
	.type	strtok, @function
strtok:
.LFB25:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %edi
	movl	20(%esp), %esi
	testl	%edi, %edi
	je	.L177
	movl	%edi, p.1398
.L178:
	movzbl	(%edi), %ecx
	testb	%cl, %cl
	je	.L180
	xorl	%ebx, %ebx
	.align 16
.L190:
	movl	%esi, %eax
	jmp	.L181
	.align 16
.L183:
	addl	$1, %eax
	testb	%dl, %dl
	je	.L182
.L181:
	movzbl	(%eax), %edx
	cmpb	%cl, %dl
	jne	.L183
	addl	$1, %ebx
	movzbl	(%edi,%ebx), %ecx
	testb	%cl, %cl
	jne	.L190
	.align 16
.L182:
	leal	(%edi,%ebx), %eax
	movzbl	(%eax), %ebx
	testb	%bl, %bl
	je	.L180
	xorl	%edi, %edi
	.align 16
.L191:
	movl	%esi, %edx
	jmp	.L187
	.align 16
.L188:
	addl	$1, %edx
	testb	%cl, %cl
	je	.L207
.L187:
	movzbl	(%edx), %ecx
	cmpb	%bl, %cl
	jne	.L188
.L192:
	addl	%eax, %edi
	cmpl	%edi, %eax
	movl	%edi, p.1398
	je	.L180
	xorl	%edx, %edx
	cmpb	$0, (%edi)
	je	.L189
	leal	1(%edi), %edx
	movb	$0, (%edi)
.L189:
	movl	%edx, p.1398
.L176:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L207:
	.cfi_restore_state
	addl	$1, %edi
	movzbl	(%eax,%edi), %ebx
	testb	%bl, %bl
	jne	.L191
	jmp	.L192
.L177:
	movl	p.1398, %edi
	testl	%edi, %edi
	jne	.L178
	xorl	%eax, %eax
	jmp	.L176
	.align 16
.L180:
	movl	$0, p.1398
	xorl	%eax, %eax
	jmp	.L176
	.cfi_endproc
.LFE25:
	.size	strtok, .-strtok
	.align 16
	.globl	strstr
	.type	strstr, @function
strstr:
.LFB26:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	movl	36(%esp), %ebp
	movl	32(%esp), %ebx
	movzbl	0(%ebp), %esi
	movl	%ebx, %eax
	movl	%esi, %ecx
	testb	%cl, %cl
	je	.L208
	xorl	%edi, %edi
	.align 16
.L210:
	addl	$1, %edi
	cmpb	$0, 0(%ebp,%edi)
	jne	.L210
	movzbl	(%ebx), %eax
	movl	%esi, %edx
	cmpb	%al, %dl
	je	.L226
	.align 16
.L213:
	addl	$1, %ebx
	testb	%al, %al
	je	.L217
.L212:
	movzbl	(%ebx), %eax
	movl	%esi, %edx
	cmpb	%al, %dl
	jne	.L213
.L226:
	movl	%edi, %ecx
	movl	%ebp, %edx
	movl	%ebx, %eax
	call	strncmp.part.0
	testl	%eax, %eax
	je	.L227
	addl	$1, %ebx
	jmp	.L212
	.align 16
.L217:
	xorl	%eax, %eax
.L208:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L227:
	.cfi_restore_state
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE26:
	.size	strstr, .-strstr
	.align 16
	.globl	stroccr
	.type	stroccr, @function
stroccr:
.LFB27:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %ecx
	xorl	%eax, %eax
	movzbl	12(%esp), %ebx
	movzbl	(%ecx), %edx
	testb	%dl, %dl
	je	.L228
	.align 16
.L231:
	cmpb	%dl, %bl
	sete	%dl
	addl	$1, %ecx
	movzbl	%dl, %edx
	addl	%edx, %eax
	movzbl	(%ecx), %edx
	testb	%dl, %dl
	jne	.L231
.L228:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE27:
	.size	stroccr, .-stroccr
	.align 16
	.globl	delay
	.type	delay, @function
delay:
.LFB30:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE30:
	.size	delay, .-delay
	.align 16
	.globl	mouse_wait
	.type	mouse_wait, @function
mouse_wait:
.LFB43:
	.cfi_startproc
	cmpb	$0, 4(%esp)
	jne	.L237
	jmp	mouse_wait.part.1
	.align 16
.L237:
	movl	$100000, %edx
	jmp	.L239
	.align 16
.L244:
	subl	$1, %edx
	je	.L236
.L239:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L244
.L236:
	rep ret
	.cfi_endproc
.LFE43:
	.size	mouse_wait, .-mouse_wait
	.align 16
	.globl	mouse_write
	.type	mouse_write, @function
mouse_write:
.LFB44:
	.cfi_startproc
	movl	4(%esp), %ecx
	movl	$100000, %edx
	jmp	.L247
	.align 16
.L258:
	subl	$1, %edx
	je	.L246
.L247:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L258
.L246:
	movl	$-44, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $100
/  0 "" 2
/NO_APP
	movl	$100000, %edx
	jmp	.L249
	.align 16
.L259:
	subl	$1, %edx
	je	.L248
.L249:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L259
.L248:
	movl	%ecx, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE44:
	.size	mouse_write, .-mouse_write
	.align 16
	.globl	mouse_read
	.type	mouse_read, @function
mouse_read:
.LFB45:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	mouse_wait.part.1
/APP
/  45 "Library/sys.h" 1
	inb $96, %al
/  0 "" 2
/NO_APP
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE45:
	.size	mouse_read, .-mouse_read
	.align 16
	.globl	mouseinit
	.type	mouseinit, @function
mouseinit:
.LFB46:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	movl	$100000, %edx
	jmp	.L264
	.align 16
.L312:
	subl	$1, %edx
	je	.L263
.L264:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L312
.L263:
	movl	$-88, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $100
/  0 "" 2
/NO_APP
	movl	$100000, %edx
	jmp	.L266
	.align 16
.L313:
	subl	$1, %edx
	je	.L265
.L266:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L313
.L265:
	movl	$32, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $100
/  0 "" 2
/NO_APP
	call	mouse_wait.part.1
/APP
/  45 "Library/sys.h" 1
	inb $96, %al
/  0 "" 2
/NO_APP
	orl	$2, %eax
	movl	$100000, %edx
	movl	%eax, %ecx
	jmp	.L268
	.align 16
.L314:
	subl	$1, %edx
	je	.L267
.L268:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L314
.L267:
	movl	$96, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $100
/  0 "" 2
/NO_APP
	movl	$100000, %edx
	jmp	.L270
	.align 16
.L315:
	subl	$1, %edx
	je	.L269
.L270:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L315
.L269:
	movl	%ecx, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
/NO_APP
	movl	$100000, %edx
	jmp	.L272
	.align 16
.L316:
	subl	$1, %edx
	je	.L271
.L272:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L316
.L271:
	movl	$-44, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $100
/  0 "" 2
/NO_APP
	movl	$100000, %edx
	jmp	.L274
	.align 16
.L317:
	subl	$1, %edx
	je	.L273
.L274:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L317
.L273:
	movl	$-10, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
/NO_APP
	call	mouse_wait.part.1
/APP
/  45 "Library/sys.h" 1
	inb $96, %al
/  0 "" 2
/NO_APP
	movl	$100000, %edx
	jmp	.L276
	.align 16
.L318:
	subl	$1, %edx
	je	.L275
.L276:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L318
.L275:
	movl	$-44, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $100
/  0 "" 2
/NO_APP
	movl	$100000, %edx
	jmp	.L278
	.align 16
.L319:
	subl	$1, %edx
	je	.L277
.L278:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L319
.L277:
	movl	$-12, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
/NO_APP
	call	mouse_wait.part.1
/APP
/  45 "Library/sys.h" 1
	inb $96, %al
/  0 "" 2
/NO_APP
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE46:
	.size	mouseinit, .-mouseinit
	.align 16
	.globl	mouse_updater
	.type	mouse_updater, @function
mouse_updater:
.LFB47:
	.cfi_startproc
	movl	cycle, %edx
	cmpl	$2, %edx
	ja	.L325
	.align 16
.L321:
	addl	$mouse_bytes, %edx
	.align 16
.L324:
/APP
/  45 "Library/sys.h" 1
	inb $96, %al
/  0 "" 2
/NO_APP
	movb	%al, (%edx)
	addl	$1, %edx
	cmpl	$mouse_bytes+3, %edx
	jne	.L324
.L325:
	movzbl	mouse_bytes, %eax
	testb	%al, %al
	js	.L322
	testb	$64, %al
	jne	.L322
	testb	$32, %al
	jne	.L328
	movl	mousedeltay, %edx
	orl	$-256, %edx
	movl	%edx, mousedeltay
.L328:
	testb	$16, %al
	jne	.L329
	movl	mousedeltax, %eax
	orl	$-256, %eax
	movl	%eax, mousedeltax
.L329:
	movsbl	mouse_bytes+1, %eax
	movl	%eax, mousedeltax
	movsbl	mouse_bytes+2, %eax
	movl	%eax, mousedeltay
/APP
/  103 "Source/Drivers/mouse/mouse.c" 1
	int $50
/  0 "" 2
/NO_APP
	xorl	%edx, %edx
	jmp	.L321
.L322:
	movl	$0, cycle
	ret
	.cfi_endproc
.LFE47:
	.size	mouse_updater, .-mouse_updater
	.comm	consolerow,4,4
	.comm	consolecolumn,4,4
	.comm	console_color,1,1
	.comm	console_dbuffer,4,4
	.comm	console_dbuffer_original,4,4
	.comm	console_dbuffer_limit,4,4
	.globl	console_rows_skipped
	.section	.bss
	.align 4
	.type	console_rows_skipped, @object
	.size	console_rows_skipped, 4
console_rows_skipped:
	.zero	4
	.globl	console_skip
	.align 4
	.type	console_skip, @object
	.size	console_skip, 4
console_skip:
	.zero	4
	.comm	console_buffer,4,4
	.comm	default_console_color,4,4
	.comm	mdbug,4,4
	.comm	vesa,4,4
	.comm	kmalloc,4,4
	.text
	.align 16
	.type	print, @function
print:
.LFB48:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	leal	(%eax,%edx), %esi
	subl	$4, %esp
	.cfi_def_cfa_offset 16
	testl	%edx, %edx
	je	.L332
	movl	%eax, %ebx
	.align 16
.L336:
	movsbl	(%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	addl	$1, %ebx
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	console_putchar
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	cmpl	%ebx, %esi
	jne	.L336
.L332:
	addl	$4, %esp
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE48:
	.size	print, .-print
	.globl	plock
	.section	.bss
	.align 4
	.type	plock, @object
	.size	plock, 4
plock:
	.zero	4
	.text
	.align 16
	.globl	scanf
	.type	scanf, @function
scanf:
.LFB50:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$92, %esp
	.cfi_def_cfa_offset 112
	movl	plock, %eax
	movl	112(%esp), %ebx
	movl	%eax, 4(%esp)
	.align 16
.L340:
	testl	%eax, %eax
	jne	.L340
	movzbl	(%ebx), %edx
	leal	116(%esp), %eax
	movl	$1, plock
	movb	$0, 11(%esp)
	movl	%eax, 12(%esp)
.L341:
	testb	%dl, %dl
	je	.L379
	.align 16
.L354:
	cmpb	$37, %dl
	je	.L342
	movzbl	1(%ebx), %eax
	leal	1(%ebx), %esi
.L343:
.L344:
	testb	%al, %al
	je	.L345
.L384:
	cmpb	$37, %al
	je	.L345
	leal	2(%ebx), %eax
	movl	$1, %edi
	jmp	.L346
	.align 16
.L380:
	cmpb	$37, %dl
	je	.L358
.L346:
	movzbl	(%eax), %edx
	addl	$1, %edi
	movl	%eax, %esi
	addl	$1, %eax
	testb	%dl, %dl
	jne	.L380
.L358:
	testl	%edi, %edi
	je	.L348
	xorl	%ebp, %ebp
	.align 16
.L349:
	movsbl	(%ebx,%ebp), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 124
	addl	$1, %ebp
	pushl	%eax
	.cfi_def_cfa_offset 128
	call	_putchar
	addl	$16, %esp
	.cfi_def_cfa_offset 112
	cmpl	%ebp, %edi
	ja	.L349
.L355:
	movzbl	(%esi), %edx
.L348:
	addl	%edi, 4(%esp)
	testb	%dl, %dl
	movl	%esi, %ebx
	jne	.L354
.L379:
	movl	4(%esp), %eax
	movl	$0, plock
	addl	$92, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L342:
	.cfi_restore_state
	movzbl	1(%ebx), %eax
	leal	1(%ebx), %edx
	cmpb	$37, %al
	je	.L381
	cmpb	$0, 11(%esp)
	jne	.L356
	cmpb	$100, %al
	je	.L382
	cmpb	$115, %al
	je	.L383
	testb	%al, %al
	movl	%edx, %esi
	movb	$1, 11(%esp)
	jne	.L384
.L345:
	movsbl	(%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 124
	movl	$1, %edi
	pushl	%eax
	.cfi_def_cfa_offset 128
	call	_putchar
	addl	$16, %esp
	.cfi_def_cfa_offset 112
	jmp	.L355
	.align 16
.L356:
	movl	%edx, %esi
	jmp	.L344
.L381:
	leal	2(%ebx), %esi
	movzbl	2(%ebx), %eax
	movl	%edx, %ebx
	jmp	.L344
.L382:
	movl	12(%esp), %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 120
	leal	2(%ebx), %esi
	movl	%ecx, %eax
	addl	$4, %ecx
	movl	%ecx, 20(%esp)
	movl	(%eax), %ebp
	pushl	$15
	.cfi_def_cfa_offset 124
	leal	42(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 128
	call	kb_getline
	popl	%eax
	.cfi_def_cfa_offset 124
	leal	42(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 128
	call	StrToInt
	addl	$16, %esp
	.cfi_def_cfa_offset 112
	movl	%eax, 0(%ebp)
	movzbl	2(%ebx), %edx
	movl	%esi, %ebx
	jmp	.L341
.L383:
	movl	12(%esp), %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 120
	leal	2(%ebx), %esi
	movl	%ecx, %eax
	addl	$4, %ecx
	movl	%ecx, 20(%esp)
	movl	(%eax), %ebp
	pushl	$50
	.cfi_def_cfa_offset 124
	leal	42(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 128
	call	kb_getline
	movzbl	46(%esp), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 112
	movl	%ebp, %eax
	leal	30(%esp), %ecx
	subl	%ebp, %ecx
	.align 16
.L353:
	addl	$1, %eax
	movb	%dl, -1(%eax)
	movzbl	(%eax,%ecx), %edx
	testb	%dl, %dl
	jne	.L353
	movb	$0, (%eax)
	movzbl	2(%ebx), %edx
	movl	%esi, %ebx
	jmp	.L341
	.cfi_endproc
.LFE50:
	.size	scanf, .-scanf
	.align 16
	.globl	printf
	.type	printf, @function
printf:
.LFB51:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	xorl	%ecx, %ecx
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	$1, %edx
	subl	$76, %esp
	.cfi_def_cfa_offset 96
	movl	96(%esp), %esi
	.align 16
.L386:
	movl	%ecx, %eax
	lock cmpxchgl	%edx, printlockLocked
	sete	%bl
	jne	.L386
	lock orl	$0, (%esp)
	movl	multitasking_ON, %eax
	testl	%eax, %eax
	je	.L387
	movzbl	(%esi), %eax
	leal	100(%esp), %edx
	movb	%bl, 31(%esp)
	movb	$0, 8(%esp)
	movl	$0, 20(%esp)
	movl	%edx, %ebx
.L388:
	testb	%al, %al
	je	.L479
.L410:
	cmpb	$37, %al
	je	.L389
	movzbl	1(%esi), %eax
	leal	1(%esi), %edi
.L390:
.L391:
	cmpb	$37, %al
	je	.L392
.L493:
	testb	%al, %al
	je	.L392
	leal	2(%esi), %ecx
	movl	$1, %ebp
	jmp	.L393
.L480:
	cmpb	$37, %al
	je	.L440
.L393:
	movzbl	(%ecx), %eax
	addl	$1, %ebp
	movl	%ecx, %edi
	addl	$1, %ecx
	testb	%al, %al
	jne	.L480
.L440:
	testl	%ebp, %ebp
	je	.L395
	xorl	%ecx, %ecx
	movl	%ebx, 24(%esp)
	movl	%ecx, %ebx
.L396:
	movsbl	(%esi,%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	addl	$1, %ebx
	pushl	%eax
	.cfi_def_cfa_offset 112
	call	_putchar
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	cmpl	%ebx, %ebp
	ja	.L396
	movl	24(%esp), %ebx
.L433:
	movzbl	(%edi), %eax
.L395:
	addl	%ebp, 20(%esp)
	testb	%al, %al
	movl	%edi, %esi
	jne	.L410
.L479:
	movl	$0, plock
	lock orl	$0, (%esp)
	movl	$0, printlockLocked
	movl	20(%esp), %eax
	addl	$76, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L387:
	.cfi_restore_state
	leal	100(%esp), %edi
	movb	$0, 8(%esp)
	movl	$0, 20(%esp)
.L412:
	movzbl	(%esi), %eax
	testb	%al, %al
	je	.L479
	cmpb	$37, %al
	je	.L413
	movzbl	1(%esi), %eax
	leal	1(%esi), %ebp
.L414:
.L415:
	testb	%al, %al
	je	.L436
.L486:
	cmpb	$37, %al
	je	.L436
	leal	2(%esi), %eax
	movl	$1, %ecx
	jmp	.L417
.L481:
	cmpb	$37, %dl
	je	.L416
.L417:
	movzbl	(%eax), %edx
	addl	$1, %ecx
	movl	%eax, %ebp
	addl	$1, %eax
	testb	%dl, %dl
	jne	.L481
.L416:
	movl	%ecx, %edx
	movl	%esi, %eax
	movl	%ecx, 24(%esp)
	movl	%ebp, %esi
	call	print
	movl	24(%esp), %ecx
	addl	%ecx, 20(%esp)
	jmp	.L412
.L413:
	leal	1(%esi), %eax
	movl	%eax, 24(%esp)
	movzbl	1(%esi), %eax
	cmpb	$37, %al
	je	.L482
	cmpb	$0, 8(%esp)
	jne	.L437
	cmpb	$99, %al
	je	.L483
	cmpb	$115, %al
	je	.L484
	cmpb	$105, %al
	sete	%dl
	cmpb	$100, %al
	sete	%cl
	orb	%cl, %dl
	jne	.L478
	cmpb	$108, %al
	sete	%dl
	cmpb	$120, %al
	sete	%cl
	orb	%cl, %dl
	jne	.L478
	cmpb	$97, %al
	sete	%dl
	cmpb	$76, %al
	sete	%cl
	orb	%cl, %dl
	movb	%dl, 8(%esp)
	jne	.L485
	cmpb	$112, %al
	je	.L441
	cmpb	$103, %al
	je	.L441
	testb	%al, %al
	leal	1(%esi), %ebp
	movb	%bl, 8(%esp)
	jne	.L486
.L436:
	movl	$1, %ecx
	jmp	.L416
	.align 16
.L389:
	leal	1(%esi), %eax
	movl	%eax, 24(%esp)
	movzbl	1(%esi), %eax
	cmpb	$37, %al
	je	.L487
	cmpb	$0, 8(%esp)
	jne	.L434
	cmpb	$99, %al
	je	.L488
	cmpb	$115, %al
	je	.L489
	cmpb	$105, %al
	sete	%dl
	cmpb	$100, %al
	movl	%edx, %ebp
	sete	%dl
	movl	%ebp, %ecx
	orb	%dl, %cl
	jne	.L490
	cmpb	$108, %al
	sete	%dl
	cmpb	$120, %al
	movl	%edx, %ebp
	sete	%dl
	movl	%ebp, %ecx
	orb	%dl, %cl
	movb	%cl, 8(%esp)
	jne	.L491
	cmpb	$112, %al
	sete	%dl
	cmpb	$103, %al
	movl	%edx, %ebp
	sete	%dl
	movl	%ebp, %ecx
	orb	%dl, %cl
	jne	.L492
	movzbl	31(%esp), %edx
	cmpb	$102, %al
	leal	1(%esi), %edi
	movb	%dl, 8(%esp)
	jne	.L391
	fldl	(%ebx)
	leal	2(%esi), %edi
	leal	8(%ebx), %ebp
	fldz
	fucomip	%st(1), %st
	jbe	.L432
	fstpl	8(%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$45
	.cfi_def_cfa_offset 112
	call	_putchar
	fldl	24(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	fchs
.L432:
	fnstcw	38(%esp)
	movzwl	38(%esp), %eax
	movl	$0, 12(%esp)
	movb	$12, %ah
	movw	%ax, 36(%esp)
	fldcw	36(%esp)
	fistl	32(%esp)
	fldcw	38(%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	movl	44(%esp), %eax
	movl	%eax, 20(%esp)
	fildq	20(%esp)
	fsubrp	%st, %st(1)
	fstpl	20(%esp)
	pushl	%eax
	.cfi_def_cfa_offset 112
	call	_printint
	movl	$46, (%esp)
	call	_putchar
	fnstcw	54(%esp)
	fldl	24(%esp)
	movzwl	54(%esp), %eax
	fmuls	.LC1
	movb	$12, %ah
	movw	%ax, 52(%esp)
	fldcw	52(%esp)
	fistpl	48(%esp)
	fldcw	54(%esp)
	movl	48(%esp), %eax
	movl	%eax, (%esp)
.L477:
	call	_printint
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	movzbl	2(%esi), %eax
	movl	%ebp, %ebx
	movb	$0, 8(%esp)
	movl	%edi, %esi
	jmp	.L388
	.align 16
.L437:
	leal	1(%esi), %ebp
	jmp	.L415
.L434:
	cmpb	$37, %al
	leal	1(%esi), %edi
	jne	.L493
.L392:
	movsbl	(%esi), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	movl	$1, %ebp
	pushl	%eax
	.cfi_def_cfa_offset 112
	call	_putchar
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	jmp	.L433
.L487:
	movzbl	2(%esi), %eax
	leal	2(%esi), %edi
	movl	24(%esp), %esi
	jmp	.L391
.L489:
	movl	%ebx, %eax
	xorl	%edi, %edi
	addl	$4, %ebx
	movl	(%eax), %ecx
	leal	2(%esi), %ebp
	cmpb	$0, (%ecx)
	je	.L476
.L457:
	addl	$1, %edi
	cmpb	$0, (%ecx,%edi)
	jne	.L457
	addl	%ecx, %edi
.L403:
	movsbl	(%ecx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	movl	%ecx, 36(%esp)
	pushl	%eax
	.cfi_def_cfa_offset 112
	call	_putchar
	movl	40(%esp), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	addl	$1, %ecx
	cmpl	%ecx, %edi
	jne	.L403
.L476:
	movzbl	2(%esi), %eax
	movl	%ebp, %esi
	jmp	.L388
.L488:
	movsbl	(%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	leal	2(%esi), %edi
	leal	4(%ebx), %ebp
	movl	%ebp, %ebx
	pushl	%eax
	.cfi_def_cfa_offset 112
	call	_putchar
	movzbl	2(%esi), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	movl	%edi, %esi
	jmp	.L388
.L492:
	movl	%ebx, %eax
	addl	$4, %ebx
	leal	2(%esi), %ecx
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	.L408
	movl	default_console_color, %eax
	movb	%al, console_color
	movzbl	2(%esi), %eax
	movl	%ecx, %esi
	jmp	.L388
.L491:
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 108
	leal	2(%esi), %edi
	leal	4(%ebx), %ebp
	pushl	(%ebx)
	.cfi_def_cfa_offset 112
	jmp	.L477
.L490:
	.cfi_restore_state
	leal	4(%ebx), %ebp
	movl	(%ebx), %ebx
	leal	2(%esi), %edi
	testl	%ebx, %ebx
	jns	.L431
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	negl	%ebx
	pushl	$45
	.cfi_def_cfa_offset 112
	call	_putchar
	addl	$16, %esp
	.cfi_def_cfa_offset 96
.L431:
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	movl	%ebp, %ebx
	call	_printint
	movzbl	2(%esi), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	movl	%edi, %esi
	jmp	.L388
.L408:
	movb	%al, console_color
	movzbl	2(%esi), %eax
	movl	%ecx, %esi
	jmp	.L388
.L482:
	leal	2(%esi), %ebp
	movzbl	2(%esi), %eax
	movl	24(%esp), %esi
	jmp	.L415
.L484:
	movl	(%edi), %eax
	addl	$2, %esi
	xorl	%edx, %edx
	leal	4(%edi), %ebp
	cmpb	$0, (%eax)
	je	.L423
.L424:
	addl	$1, %edx
	cmpb	$0, (%eax,%edx)
	jne	.L424
.L423:
	call	print
	movl	%ebp, %edi
	jmp	.L412
.L483:
	movl	(%edi), %eax
	leal	4(%edi), %ebp
	movl	$1, %edx
	addl	$2, %esi
	movl	%ebp, %edi
	movb	%al, 63(%esp)
	leal	63(%esp), %eax
	call	print
	jmp	.L412
.L441:
	movl	%edi, %eax
	addl	$2, %esi
	addl	$4, %edi
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	.L429
	movl	default_console_color, %eax
	movb	%al, console_color
	jmp	.L412
.L485:
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	(%edi)
	.cfi_def_cfa_offset 112
	leal	8(%edi), %ebp
	addl	$2, %esi
	call	console_write_dec
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	movl	%ebp, %edi
	movb	$0, 8(%esp)
	jmp	.L412
.L478:
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	leal	4(%edi), %ebp
	pushl	(%edi)
	.cfi_def_cfa_offset 112
	addl	$2, %esi
	call	printint
	movl	%ebp, %edi
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	jmp	.L412
.L429:
	movb	%al, console_color
	jmp	.L412
	.cfi_endproc
.LFE51:
	.size	printf, .-printf
	.comm	tbuf,32,32
	.globl	bchars
	.data
	.align 4
	.type	bchars, @object
	.size	bchars, 16
bchars:
	.byte	48
	.byte	49
	.byte	50
	.byte	51
	.byte	52
	.byte	53
	.byte	54
	.byte	55
	.byte	56
	.byte	57
	.byte	65
	.byte	66
	.byte	67
	.byte	68
	.byte	69
	.byte	70
	.text
	.align 16
	.globl	itoa
	.type	itoa, @function
itoa:
.LFB52:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	8(%esp), %edx
	movl	12(%esp), %ecx
	testl	%eax, %eax
	je	.L495
	cmpl	$16, %ecx
	ja	.L495
	jmp	itoa.part.3
	.align 16
.L495:
	movb	$48, (%edx)
	movb	$0, 1(%edx)
	ret
	.cfi_endproc
.LFE52:
	.size	itoa, .-itoa
	.section	.rodata
.LC3:
	.string	"%s\n"
	.text
	.align 16
	.globl	puts
	.type	puts, @function
puts:
.LFB53:
	.cfi_startproc
	subl	$20, %esp
	.cfi_def_cfa_offset 24
	pushl	24(%esp)
	.cfi_def_cfa_offset 28
	pushl	$.LC3
	.cfi_def_cfa_offset 32
	call	printf
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE53:
	.size	puts, .-puts
	.comm	widthVESA,4,4
	.comm	heightVESA,4,4
	.comm	depthVESA,4,4
	.comm	vga_mem,4,4
	.comm	buff,4,4
	.comm	vesa_buff,4,4
	.align 16
	.globl	console_init
	.type	console_init, @function
console_init:
.LFB56:
	.cfi_startproc
	movl	$0, consolerow
	movl	$0, consolecolumn
	movl	$753664, %eax
	movb	$15, console_color
	movl	$15, default_console_color
	movl	$753664, console_buffer
	.align 16
.L499:
	leal	160(%eax), %edx
	.align 16
.L500:
	movl	$3872, %ecx
	addl	$2, %eax
	movw	%cx, -2(%eax)
	cmpl	%edx, %eax
	jne	.L500
	cmpl	$757664, %eax
	jne	.L499
	rep ret
	.cfi_endproc
.LFE56:
	.size	console_init, .-console_init
	.align 16
	.globl	console_putentryat
	.type	console_putentryat, @function
console_putentryat:
.LFB58:
	.cfi_startproc
	movl	16(%esp), %eax
	movl	console_buffer, %edx
	leal	(%eax,%eax,4), %eax
	sall	$4, %eax
	addl	12(%esp), %eax
	leal	(%edx,%eax,2), %ecx
	movzbl	8(%esp), %eax
	sall	$8, %eax
	movl	%eax, %edx
	movsbw	4(%esp), %ax
	orl	%edx, %eax
	movw	%ax, (%ecx)
	ret
	.cfi_endproc
.LFE58:
	.size	console_putentryat, .-console_putentryat
	.align 16
	.globl	console_putchar
	.type	console_putchar, @function
console_putchar:
.LFB59:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	$32, %edx
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$28, %esp
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	movl	8(%ebp), %esi
	movl	consolerow, %edi
	movl	consolecolumn, %ebx
	leal	-9(%esi), %eax
	movl	%edi, -28(%ebp)
	leal	(%edi,%edi,4), %edi
	cmpb	$1, %al
	movl	console_buffer, %eax
	cmova	%esi, %edx
	sall	$4, %edi
	addl	%ebx, %edi
	movsbw	%dl, %dx
	leal	(%eax,%edi,2), %edi
	movl	%edi, %ecx
	movzbl	console_color, %edi
	sall	$8, %edi
	orl	%edi, %edx
	movw	%dx, (%ecx)
	leal	1(%ebx), %edx
	cmpl	$80, %edx
	je	.L521
	movl	%esi, %ecx
	cmpb	$10, %cl
	je	.L521
	addl	$6, %ebx
	cmpb	$9, %cl
	cmovne	%edx, %ebx
	movl	%ebx, consolecolumn
.L504:
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
.L521:
	.cfi_restore_state
	movl	-28(%ebp), %ecx
	movl	$0, consolecolumn
	addl	$1, %ecx
	cmpl	$25, %ecx
	movl	%ecx, consolerow
	jne	.L504
	movl	%esp, -28(%ebp)
	subl	$4336, %esp
	movl	$24, consolerow
	leal	4322(%esp), %ecx
	movl	%esp, -32(%ebp)
	movl	%esp, %edi
	movl	%esp, %edx
	.align 16
.L510:
	xorl	%ebx, %ebx
	addl	$2, %edx
	movw	%bx, -2(%edx)
	cmpl	%ecx, %edx
	jne	.L510
	leal	4000(%eax), %esi
	movl	%eax, %edx
	movl	%eax, -36(%ebp)
	.align 16
.L513:
	leal	160(%edx), %ebx
	movl	%edi, %eax
	.align 16
.L512:
	movzwl	(%edx), %ecx
	addl	$2, %edx
	addl	$2, %eax
	movw	%cx, -2(%eax)
	cmpl	%ebx, %edx
	jne	.L512
	addl	$160, %edi
	cmpl	%edx, %esi
	jne	.L513
	movl	-32(%ebp), %esi
	movl	-36(%ebp), %eax
	leal	160(%esi), %edx
	addl	$4160, %esi
	.align 16
.L516:
	leal	160(%edx), %edi
	movl	%eax, %ecx
	.align 16
.L515:
	movzwl	(%edx), %ebx
	addl	$2, %edx
	addl	$2, %ecx
	movw	%bx, -2(%ecx)
	cmpl	%edx, %edi
	jne	.L515
	addl	$160, %eax
	cmpl	%edi, %esi
	jne	.L516
	movl	-28(%ebp), %esp
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE59:
	.size	console_putchar, .-console_putchar
	.align 16
	.globl	console_write
	.type	console_write, @function
console_write:
.LFB60:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$4, %esp
	.cfi_def_cfa_offset 16
	movl	20(%esp), %eax
	movl	16(%esp), %ebx
	testl	%eax, %eax
	leal	(%ebx,%eax), %esi
	je	.L535
	.align 16
.L539:
	movsbl	(%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	addl	$1, %ebx
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	console_putchar
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	cmpl	%esi, %ebx
	jne	.L539
.L535:
	addl	$4, %esp
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE60:
	.size	console_write, .-console_write
	.align 16
	.globl	console_writestring
	.type	console_writestring, @function
console_writestring:
.LFB61:
	.cfi_startproc
	jmp	printf
	.cfi_endproc
.LFE61:
	.size	console_writestring, .-console_writestring
	.align 16
	.globl	putchar
	.type	putchar, @function
putchar:
.LFB62:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	28(%esp), %ebx
	movsbl	%bl, %eax
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	console_putchar
	addl	$24, %esp
	.cfi_def_cfa_offset 8
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE62:
	.size	putchar, .-putchar
	.align 16
	.globl	console_write_dec
	.type	console_write_dec, @function
console_write_dec:
.LFB63:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	movl	32(%esp), %edi
	testl	%edi, %edi
	je	.L552
	movl	%edi, %edx
	xorl	%esi, %esi
	movl	$-858993459, %ecx
	.align 16
.L547:
	movl	%edx, %eax
	addl	$1, %esi
	mull	%ecx
	shrl	$3, %edx
	testl	%edx, %edx
	jne	.L547
	testl	%esi, %esi
	movl	$1, %ebx
	je	.L545
	.align 16
.L553:
	leal	(%ebx,%ebx,4), %ebx
	addl	$1, %edx
	addl	%ebx, %ebx
	cmpl	%edx, %esi
	ja	.L553
.L546:
	movl	%ebx, %eax
	movl	$-858993459, %edx
	xorl	%ebp, %ebp
	mull	%edx
	movl	%edx, %ebx
	shrl	$3, %ebx
	.align 16
.L550:
	movl	%edi, %eax
	xorl	%edx, %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	divl	%ebx
	addl	$1, %ebp
	addl	$48, %eax
	movl	%edx, %edi
	movsbl	%al, %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	console_putchar
	movl	$-858993459, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	mull	%ebx
	movl	%edx, %ebx
	shrl	$3, %ebx
	cmpl	%ebp, %esi
	ja	.L550
.L545:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L552:
	.cfi_restore_state
	movl	$1, %esi
	movl	$10, %ebx
	jmp	.L546
	.cfi_endproc
.LFE63:
	.size	console_write_dec, .-console_write_dec
	.align 16
	.globl	printint
	.type	printint, @function
printint:
.LFB64:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$4, %esp
	.cfi_def_cfa_offset 16
	movl	16(%esp), %esi
	cmpl	$999999999, %esi
	jbe	.L569
	movl	%esi, %edx
	xorl	%ebx, %ebx
	movl	$-858993459, %ecx
	jmp	.L566
	.align 16
.L563:
	movl	%eax, %ebx
.L566:
	movl	%edx, %eax
	mull	%ecx
	leal	1(%ebx), %eax
	shrl	$3, %edx
	testl	%edx, %edx
	jne	.L563
	subl	$8, %ebx
	movl	$1, %ecx
	je	.L561
	.align 16
.L562:
	leal	(%ecx,%ecx,4), %ecx
	addl	$1, %edx
	addl	%ecx, %ecx
	cmpl	%edx, %ebx
	jne	.L562
.L561:
	movl	%esi, %eax
	xorl	%edx, %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	divl	%ecx
	pushl	%eax
	.cfi_def_cfa_offset 32
	movl	%edx, %esi
	call	console_write_dec
	movl	%esi, 32(%esp)
	addl	$20, %esp
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	jmp	console_write_dec
	.align 16
.L569:
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -12
	.cfi_offset 6, -8
	addl	$4, %esp
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	jmp	console_write_dec
	.cfi_endproc
.LFE64:
	.size	printint, .-printint
	.align 16
	.globl	print64int
	.type	print64int, @function
print64int:
.LFB65:
	.cfi_startproc
	jmp	console_write_dec
	.cfi_endproc
.LFE65:
	.size	print64int, .-print64int
	.align 16
	.globl	_console_putentryat
	.type	_console_putentryat, @function
_console_putentryat:
.LFB66:
	.cfi_startproc
	movl	16(%esp), %eax
	movl	console_dbuffer, %edx
	leal	(%eax,%eax,4), %eax
	sall	$4, %eax
	addl	12(%esp), %eax
	leal	(%edx,%eax,2), %ecx
	movzbl	8(%esp), %eax
	sall	$8, %eax
	movl	%eax, %edx
	movsbw	4(%esp), %ax
	orl	%edx, %eax
	movw	%ax, (%ecx)
	ret
	.cfi_endproc
.LFE66:
	.size	_console_putentryat, .-_console_putentryat
	.align 16
	.globl	_putchar
	.type	_putchar, @function
_putchar:
.LFB67:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	12(%esp), %eax
	cmpb	$10, %al
	je	.L581
	movl	consolecolumn, %esi
	cmpb	$9, %al
	leal	5(%esi), %edx
	je	.L576
	movl	consolerow, %ecx
	movl	consolecolumn, %edx
	movl	console_dbuffer, %ebx
	leal	(%ecx,%ecx,4), %ecx
	sall	$4, %ecx
	addl	%edx, %ecx
	leal	(%ebx,%ecx,2), %esi
	movzbl	console_color, %ecx
	movl	%ecx, %ebx
	movsbw	%al, %cx
	sall	$8, %ebx
	orl	%ebx, %ecx
	movw	%cx, (%esi)
.L576:
	addl	$1, %edx
	cmpl	$79, %edx
	ja	.L577
	movl	%edx, consolecolumn
.L578:
	movsbl	%al, %eax
.L572:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L577:
	.cfi_restore_state
	movl	consolerow, %esi
	movl	$0, consolecolumn
	leal	1(%esi), %edx
	cmpl	$24, %edx
	movl	%edx, consolerow
	jbe	.L578
	movl	console_rows_skipped, %edx
	addl	$1, %edx
	movl	%edx, console_rows_skipped
	movl	console_skip, %edx
	addl	$1, %edx
	movl	%edx, console_skip
	jmp	.L578
	.align 16
.L581:
	movl	consolerow, %eax
	movl	$0, consolecolumn
	leal	1(%eax), %edx
	movl	$10, %eax
	cmpl	$24, %edx
	movl	%edx, consolerow
	jbe	.L572
	movl	console_rows_skipped, %edx
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	addl	$1, %edx
	movl	%edx, console_rows_skipped
	movl	console_skip, %edx
	addl	$1, %edx
	movl	%edx, console_skip
	ret
	.cfi_endproc
.LFE67:
	.size	_putchar, .-_putchar
	.align 16
	.globl	_putchar_old
	.type	_putchar_old, @function
_putchar_old:
.LFB68:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$28, %esp
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	movzbl	8(%ebp), %eax
	movzbl	8(%ebp), %esi
	movl	consolerow, %edx
	movl	consolecolumn, %ecx
	subl	$9, %eax
	leal	(%edx,%edx,4), %ebx
	cmpb	$1, %al
	movl	$32, %eax
	cmova	%esi, %eax
	movl	console_dbuffer, %esi
	sall	$4, %ebx
	addl	%ecx, %ebx
	cbtw
	leal	(%esi,%ebx,2), %edi
	movzbl	console_color, %ebx
	sall	$8, %ebx
	orl	%ebx, %eax
	movw	%ax, (%edi)
	leal	1(%ecx), %eax
	cmpl	$80, %eax
	je	.L599
	cmpb	$10, 8(%ebp)
	je	.L599
	addl	$6, %ecx
	cmpb	$9, 8(%ebp)
	cmovne	%eax, %ecx
	movl	%ecx, consolecolumn
.L587:
	movl	8(%ebp), %eax
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
.L599:
	.cfi_restore_state
	addl	$1, %edx
	movl	$0, consolecolumn
	cmpl	$25, %edx
	movl	%edx, consolerow
	jne	.L587
	movl	%esp, -28(%ebp)
	subl	$4336, %esp
	movl	$24, consolerow
	leal	4322(%esp), %edx
	movl	%esp, %eax
	movl	%esp, -32(%ebp)
	movl	%esp, %edi
	.align 16
.L588:
	xorl	%ecx, %ecx
	addl	$2, %eax
	movw	%cx, -2(%eax)
	cmpl	%edx, %eax
	jne	.L588
	leal	4000(%esi), %eax
	movl	%esi, -36(%ebp)
	movl	%eax, %ebx
	movl	%esi, %eax
	movl	%ebx, %esi
	.align 16
.L591:
	leal	160(%eax), %ebx
	movl	%edi, %edx
	.align 16
.L590:
	movzwl	(%eax), %ecx
	addl	$2, %eax
	addl	$2, %edx
	movw	%cx, -2(%edx)
	cmpl	%ebx, %eax
	jne	.L590
	addl	$160, %edi
	cmpl	%eax, %esi
	jne	.L591
	movl	-32(%ebp), %ebx
	movl	-36(%ebp), %esi
	movl	%ebx, %eax
	addl	$4160, %ebx
	addl	$160, %eax
	.align 16
.L594:
	leal	160(%eax), %edi
	movl	%esi, %edx
	.align 16
.L593:
	movzwl	(%eax), %ecx
	addl	$2, %eax
	addl	$2, %edx
	movw	%cx, -2(%edx)
	cmpl	%eax, %edi
	jne	.L593
	addl	$160, %esi
	cmpl	%edi, %ebx
	jne	.L594
	movl	8(%ebp), %eax
	movl	-28(%ebp), %esp
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE68:
	.size	_putchar_old, .-_putchar_old
	.align 16
	.globl	_console_write_dec
	.type	_console_write_dec, @function
_console_write_dec:
.LFB69:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	20(%esp), %edi
	testl	%edi, %edi
	je	.L620
	movl	%edi, %edx
	xorl	%esi, %esi
	movl	$-858993459, %ecx
	.align 16
.L615:
	movl	%edx, %eax
	addl	$1, %esi
	mull	%ecx
	shrl	$3, %edx
	testl	%edx, %edx
	jne	.L615
	testl	%esi, %esi
	movl	$1, %ebx
	je	.L613
	.align 16
.L621:
	leal	(%ebx,%ebx,4), %ebx
	addl	$1, %edx
	addl	%ebx, %ebx
	cmpl	%edx, %esi
	ja	.L621
.L614:
	movl	%ebx, %eax
	movl	$-858993459, %edx
	xorl	%ebp, %ebp
	mull	%edx
	movl	%edx, %ebx
	shrl	$3, %ebx
	.align 16
.L618:
	movl	%edi, %eax
	xorl	%edx, %edx
	addl	$1, %ebp
	divl	%ebx
	addl	$48, %eax
	movl	%edx, %edi
	movsbl	%al, %eax
	pushl	%eax
	.cfi_def_cfa_offset 24
	call	_putchar
	movl	$-858993459, %eax
	mull	%ebx
	popl	%eax
	.cfi_def_cfa_offset 20
	movl	%edx, %ebx
	shrl	$3, %ebx
	cmpl	%ebp, %esi
	ja	.L618
.L613:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L620:
	.cfi_restore_state
	movl	$10, %ebx
	movl	$1, %esi
	jmp	.L614
	.cfi_endproc
.LFE69:
	.size	_console_write_dec, .-_console_write_dec
	.align 16
	.globl	backspace
	.type	backspace, @function
backspace:
.LFB70:
	.cfi_startproc
	movl	consolecolumn, %eax
	movl	consolerow, %edx
	subl	$1, %eax
	testl	%eax, %eax
	movl	%eax, consolecolumn
	jne	.L628
	movl	consolerow, %eax
	movl	$80, consolecolumn
	leal	-1(%eax), %edx
	movl	$80, %eax
	movl	%edx, consolerow
.L628:
	leal	(%edx,%edx,4), %edx
	sall	$4, %edx
	addl	%edx, %eax
	movl	console_dbuffer, %edx
	leal	(%edx,%eax,2), %edx
	movzbl	console_color, %eax
	sall	$8, %eax
	orl	$32, %eax
	movw	%ax, (%edx)
	ret
	.cfi_endproc
.LFE70:
	.size	backspace, .-backspace
	.align 16
	.globl	clrline
	.type	clrline, @function
clrline:
.LFB580:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE580:
	.size	clrline, .-clrline
	.align 16
	.globl	_printint
	.type	_printint, @function
_printint:
.LFB72:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	12(%esp), %esi
	cmpl	$999999999, %esi
	jbe	.L641
	movl	%esi, %edx
	xorl	%ebx, %ebx
	movl	$-858993459, %ecx
	jmp	.L638
	.align 16
.L635:
	movl	%eax, %ebx
.L638:
	movl	%edx, %eax
	mull	%ecx
	leal	1(%ebx), %eax
	shrl	$3, %edx
	testl	%edx, %edx
	jne	.L635
	subl	$8, %ebx
	movl	$1, %ecx
	je	.L633
	.align 16
.L634:
	leal	(%ecx,%ecx,4), %ecx
	addl	$1, %edx
	addl	%ecx, %ecx
	cmpl	%edx, %ebx
	jne	.L634
.L633:
	movl	%esi, %eax
	xorl	%edx, %edx
	divl	%ecx
	pushl	%eax
	.cfi_def_cfa_offset 16
	movl	%edx, %esi
	call	_console_write_dec
	popl	%eax
	.cfi_def_cfa_offset 12
	movl	%esi, 12(%esp)
.L641:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	jmp	_console_write_dec
	.cfi_endproc
.LFE72:
	.size	_printint, .-_printint
	.align 16
	.globl	_printf
	.type	_printf, @function
_printf:
.LFB73:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	movl	32(%esp), %ebx
	leal	36(%esp), %eax
	movb	$0, 7(%esp)
	movl	$0, (%esp)
	movl	%eax, 8(%esp)
	movzbl	(%ebx), %edx
.L643:
	testb	%dl, %dl
	je	.L691
	.align 16
.L663:
	cmpb	$37, %dl
	je	.L644
	movzbl	1(%ebx), %eax
	leal	1(%ebx), %esi
.L645:
.L646:
	testb	%al, %al
	je	.L647
.L698:
	cmpb	$37, %al
	je	.L647
	leal	2(%ebx), %eax
	movl	$1, %edi
	jmp	.L648
	.align 16
.L692:
	cmpb	$37, %dl
	je	.L667
.L648:
	movzbl	(%eax), %edx
	addl	$1, %edi
	movl	%eax, %esi
	addl	$1, %eax
	testb	%dl, %dl
	jne	.L692
.L667:
	testl	%edi, %edi
	je	.L650
	xorl	%ebp, %ebp
	.align 16
.L651:
	movsbl	(%ebx,%ebp), %eax
	addl	$1, %ebp
	pushl	%eax
	.cfi_def_cfa_offset 36
	call	_putchar
	cmpl	%ebp, %edi
	popl	%eax
	.cfi_def_cfa_offset 32
	ja	.L651
.L664:
	movzbl	(%esi), %edx
.L650:
	addl	%edi, (%esp)
	testb	%dl, %dl
	movl	%esi, %ebx
	jne	.L663
.L691:
	movl	(%esp), %eax
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L644:
	.cfi_restore_state
	movzbl	1(%ebx), %eax
	leal	1(%ebx), %edi
	cmpb	$37, %al
	je	.L693
	cmpb	$0, 7(%esp)
	jne	.L665
	cmpb	$99, %al
	je	.L694
	cmpb	$115, %al
	je	.L695
	cmpb	$105, %al
	sete	%cl
	cmpb	$100, %al
	movl	%ecx, %esi
	sete	%cl
	movl	%esi, %edx
	orb	%cl, %dl
	jne	.L696
	cmpb	$108, %al
	sete	%cl
	cmpb	$120, %al
	movl	%ecx, %esi
	sete	%cl
	movl	%esi, %edx
	orb	%cl, %dl
	movb	%dl, 7(%esp)
	jne	.L697
	cmpb	$112, %al
	je	.L668
	cmpb	$103, %al
	je	.L668
	testb	%al, %al
	movl	%edi, %esi
	movb	$1, 7(%esp)
	jne	.L698
.L647:
	movsbl	(%ebx), %eax
	movl	$1, %edi
	pushl	%eax
	.cfi_def_cfa_offset 36
	call	_putchar
	popl	%eax
	.cfi_def_cfa_offset 32
	jmp	.L664
	.align 16
.L665:
	movl	%edi, %esi
	jmp	.L646
.L693:
	leal	2(%ebx), %esi
	movzbl	2(%ebx), %eax
	movl	%edi, %ebx
	jmp	.L646
.L696:
	movl	8(%esp), %ecx
	leal	2(%ebx), %esi
	movl	%ecx, %eax
	addl	$4, %ecx
	movl	%ecx, 8(%esp)
	pushl	(%eax)
	.cfi_def_cfa_offset 36
	call	_printint
	movzbl	2(%ebx), %edx
	popl	%ebx
	.cfi_def_cfa_offset 32
	movl	%esi, %ebx
	jmp	.L643
.L697:
	movl	8(%esp), %ecx
	leal	2(%ebx), %esi
	movl	%ecx, %eax
	addl	$4, %ecx
	movl	%ecx, 8(%esp)
	pushl	(%eax)
	.cfi_def_cfa_offset 36
	call	_printint
	popl	%ecx
	.cfi_def_cfa_offset 32
	movzbl	2(%ebx), %edx
	movl	%esi, %ebx
	movb	$0, 7(%esp)
	jmp	.L643
.L694:
	movl	8(%esp), %ecx
	leal	2(%ebx), %esi
	movl	%ecx, %eax
	addl	$4, %ecx
	movsbl	(%eax), %eax
	movl	%ecx, 8(%esp)
	pushl	%eax
	.cfi_def_cfa_offset 36
	call	_putchar
	movzbl	2(%ebx), %edx
	movl	%esi, %ebx
	popl	%eax
	.cfi_def_cfa_offset 32
	jmp	.L643
.L695:
	movl	8(%esp), %ecx
	xorl	%ebp, %ebp
	leal	2(%ebx), %esi
	movl	%ecx, %eax
	addl	$4, %ecx
	movl	(%eax), %edx
	movl	%ecx, 8(%esp)
	cmpb	$0, (%edx)
	je	.L689
	.align 16
.L677:
	addl	$1, %ebp
	cmpb	$0, (%edx,%ebp)
	jne	.L677
	movl	%edx, %edi
	addl	%edx, %ebp
	.align 16
.L658:
	movsbl	(%edi), %eax
	addl	$1, %edi
	pushl	%eax
	.cfi_def_cfa_offset 36
	call	_putchar
	cmpl	%edi, %ebp
	popl	%eax
	.cfi_def_cfa_offset 32
	jne	.L658
.L689:
	movzbl	2(%ebx), %edx
	movl	%esi, %ebx
	jmp	.L643
.L668:
	movl	8(%esp), %esi
	leal	2(%ebx), %eax
	movl	(%esi), %edx
	movl	%esi, %ecx
	addl	$4, %ecx
	testl	%edx, %edx
	jne	.L662
	movl	default_console_color, %edx
	movl	%ecx, 8(%esp)
	movb	%dl, console_color
	movzbl	2(%ebx), %edx
	movl	%eax, %ebx
	jmp	.L643
.L662:
	movb	%dl, console_color
	movl	%ecx, 8(%esp)
	movzbl	2(%ebx), %edx
	movl	%eax, %ebx
	jmp	.L643
	.cfi_endproc
.LFE73:
	.size	_printf, .-_printf
	.globl	CPUID_FLAG_MSR
	.section	.rodata
	.align 4
	.type	CPUID_FLAG_MSR, @object
	.size	CPUID_FLAG_MSR, 4
CPUID_FLAG_MSR:
	.long	32
	.globl	SSE_available
	.data
	.align 4
	.type	SSE_available, @object
	.size	SSE_available, 4
SSE_available:
	.long	1
	.comm	BSP_id,4,4
	.comm	AP_startup_Code_sz,4,4
	.comm	pmode_code_size,4,4
	.comm	hpet_sdt,4,4
	.comm	hpet,4,4
	.globl	hpet_base
	.section	.bss
	.align 4
	.type	hpet_base, @object
	.size	hpet_base, 4
hpet_base:
	.zero	4
	.globl	hpet_virt_address
	.align 4
	.type	hpet_virt_address, @object
	.size	hpet_virt_address, 4
hpet_virt_address:
	.zero	4
	.comm	HPET_main_counter,4,4
	.text
	.align 16
	.globl	init_descriptor_tables
	.type	init_descriptor_tables, @function
init_descriptor_tables:
.LFB76:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	init_gdt
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	jmp	init_idt
	.cfi_endproc
.LFE76:
	.size	init_descriptor_tables, .-init_descriptor_tables
	.align 16
	.type	init_gdt, @function
init_gdt:
.LFB77:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$16, %esp
	.cfi_def_cfa_offset 24
	movl	AP_startup_Code_sz, %eax
	leal	8208(%eax), %ebx
	addl	$8216, %eax
	pushl	%ebx
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	AP_gdt_Setup
	movl	%ebx, (%esp)
	call	lgdt
	addl	$24, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE77:
	.size	init_gdt, .-init_gdt
	.align 16
	.globl	AP_gdt_Setup
	.type	AP_gdt_Setup, @function
AP_gdt_Setup:
.LFB78:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$112, %esp
	.cfi_def_cfa_offset 120
	movl	124(%esp), %ebx
	movl	120(%esp), %ecx
	leal	8(%ebx), %edx
	movl	%ebx, %eax
	.align 16
.L704:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %edx
	jne	.L704
	leal	40(%ecx), %edx
	movl	%ecx, %eax
	.align 16
.L705:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L705
	movl	$39, %eax
	movl	%ecx, 2(%ebx)
	movw	%ax, (%ebx)
	leal	8(%esp), %eax
	movl	$0, (%ecx)
	movl	$0, 4(%ecx)
	movl	$65535, 8(%ecx)
	movl	%eax, %edx
	movl	$13605376, 12(%ecx)
	movl	$65535, 16(%ecx)
	sall	$16, %edx
	movl	$13603328, 20(%ecx)
	movl	$65535, 24(%ecx)
	orl	$104, %edx
	movl	$13629952, 28(%ecx)
	movl	$65535, 32(%ecx)
	movl	%edx, 40(%ecx)
	movl	%eax, %edx
	andl	$-16777216, %eax
	shrl	$16, %edx
	orl	$4229376, %eax
	movl	$13627904, 36(%ecx)
	movzbl	%dl, %edx
	orl	%edx, %eax
	movl	$16, %edx
	movl	%eax, 44(%ecx)
	movl	initial_esp, %eax
	movl	$104, %ecx
	movw	%dx, 16(%esp)
	movl	%eax, 12(%esp)
	movw	%cx, 110(%esp)
	addl	$112, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE78:
	.size	AP_gdt_Setup, .-AP_gdt_Setup
	.align 16
	.globl	gdt_set_gate
	.type	gdt_set_gate, @function
gdt_set_gate:
.LFB79:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	20(%esp), %edi
	movl	24(%esp), %ebx
	movl	36(%esp), %eax
	movl	28(%esp), %ecx
	movl	32(%esp), %esi
	leal	0(,%edi,8), %edx
	addl	40(%esp), %edx
	movl	%ebx, %ebp
	movzwl	%cx, %edi
	sall	$16, %ebp
	sall	$16, %eax
	orl	%ebp, %edi
	andl	$15728640, %eax
	andl	$983040, %ecx
	movl	%edi, (%edx)
	movl	%eax, %edi
	movl	%esi, %eax
	movzbl	%al, %esi
	movl	%edi, %eax
	sall	$8, %esi
	orl	%esi, %eax
	movl	%ebx, %esi
	shrl	$16, %ebx
	andl	$-16777216, %esi
	movzbl	%bl, %ebx
	orl	%esi, %eax
	orl	%ebx, %eax
	orl	%ecx, %eax
	movl	%eax, 4(%edx)
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE79:
	.size	gdt_set_gate, .-gdt_set_gate
	.align 16
	.globl	test_handler
	.type	test_handler, @function
test_handler:
.LFB80:
	.cfi_startproc
/APP
/  73 "Source/GDT_IDT/descriptors.c" 1
	hlt
/  0 "" 2
/NO_APP
.L712:
	jmp	.L712
	.cfi_endproc
.LFE80:
	.size	test_handler, .-test_handler
	.globl	ddd
	.section	.bss
	.align 4
	.type	ddd, @object
	.size	ddd, 4
ddd:
	.zero	4
	.text
	.align 16
	.globl	ttt_t
	.type	ttt_t, @function
ttt_t:
.LFB81:
	.cfi_startproc
	movl	t_c, %eax
	addl	$1, %eax
	cmpl	$6, %eax
	movl	%eax, t_c
	je	.L715
	rep ret
	.align 16
.L715:
	movl	$1, dbuff_run
	movl	$0, t_c
	ret
	.cfi_endproc
.LFE81:
	.size	ttt_t, .-ttt_t
	.align 16
	.globl	AP_idt_Setup
	.type	AP_idt_Setup, @function
AP_idt_Setup:
.LFB82:
	.cfi_startproc
	movl	4(%esp), %edx
	movl	8(%esp), %eax
	movl	$2047, %ecx
	movw	%cx, (%eax)
	leal	2048(%edx), %ecx
	movl	%edx, 2(%eax)
	movl	%edx, %eax
	.align 16
.L717:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%ecx, %eax
	jne	.L717
	movl	$divByZero_handler, %eax
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 4(%edx)
	movl	$debug_handler, %eax
	movl	%ecx, (%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 12(%edx)
	movl	$NMI_handler, %eax
	movl	%ecx, 8(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 20(%edx)
	movl	$breakpoint_handler, %eax
	movl	%ecx, 16(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 28(%edx)
	movl	$overflow_handler, %eax
	movl	%ecx, 24(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 36(%edx)
	movl	$outOfBounds_handler, %eax
	movl	%ecx, 32(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 44(%edx)
	movl	$invalidInstr_handler, %eax
	movl	%ecx, 40(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 52(%edx)
	movl	$noCoprocessor_handler, %eax
	movl	%ecx, 48(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 60(%edx)
	movl	$doubleFault_handler, %eax
	movl	%ecx, 56(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 68(%edx)
	movl	$coprocessor_handler, %eax
	movl	%ecx, 64(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 76(%edx)
	movl	$badTSS_handler, %eax
	movl	%ecx, 72(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orl	$524288, %ecx
	orb	$142, %ah
	movl	%eax, 84(%edx)
	movl	$segmentNotPresent_handler, %eax
	movl	%ecx, 80(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 92(%edx)
	movl	$stackFault_handler, %eax
	movl	%ecx, 88(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 100(%edx)
	movl	$INTbasedPmodeTrampoline, %eax
	movl	%ecx, 96(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 108(%edx)
	movl	$pageFault_handler, %eax
	movl	%ecx, 104(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 116(%edx)
	movl	$unknownInterrupt_handler, %eax
	movl	%ecx, 112(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 124(%edx)
	movl	$coprocessorFault_handler, %eax
	movl	%ecx, 120(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orl	$524288, %ecx
	orb	$142, %ah
	movl	%ecx, 128(%edx)
	movl	%eax, 132(%edx)
	movl	$alignmentCheck_handler, %eax
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 140(%edx)
	movl	$machineCheck_handler, %eax
	movl	%ecx, 136(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 148(%edx)
	movl	$reserved_handler, %eax
	movl	%ecx, 144(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 156(%edx)
	movl	%eax, 164(%edx)
	movl	%eax, 172(%edx)
	movl	%eax, 180(%edx)
	movl	%eax, 188(%edx)
	movl	%eax, 196(%edx)
	movl	%eax, 204(%edx)
	movl	%eax, 212(%edx)
	movl	%eax, 220(%edx)
	movl	%eax, 228(%edx)
	movl	%eax, 236(%edx)
	movl	%eax, 244(%edx)
	movl	%eax, 252(%edx)
	movl	$PIT_handler, %eax
	movl	%ecx, 152(%edx)
	movl	%ecx, 160(%edx)
	movl	%ecx, 168(%edx)
	movl	%ecx, 176(%edx)
	movl	%ecx, 184(%edx)
	movl	%ecx, 192(%edx)
	movl	%ecx, 200(%edx)
	movl	%ecx, 208(%edx)
	movl	%ecx, 216(%edx)
	movl	%ecx, 224(%edx)
	movl	%ecx, 232(%edx)
	movl	%ecx, 240(%edx)
	movl	%ecx, 248(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orl	$524288, %ecx
	orb	$142, %ah
	movl	%ecx, 256(%edx)
	movl	%eax, 260(%edx)
	movl	$kb_handle, %eax
	movzwl	%ax, %ecx
	orl	$524288, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	movl	%ecx, 264(%edx)
	movl	%eax, 268(%edx)
	movl	$cascade_handler, %eax
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 276(%edx)
	movl	$COM2_handler, %eax
	movl	%ecx, 272(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 284(%edx)
	movl	$COM1_handler, %eax
	movl	%ecx, 280(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 292(%edx)
	movl	$LPT2_handler, %eax
	movl	%ecx, 288(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 300(%edx)
	movl	$floppyDisk_handler, %eax
	movl	%ecx, 296(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 308(%edx)
	movl	$LPT1_handler, %eax
	movl	%ecx, 304(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 316(%edx)
	movl	$RTC_handler, %eax
	movl	%ecx, 312(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 324(%edx)
	movl	$periph1_handler, %eax
	movl	%ecx, 320(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 332(%edx)
	movl	$periph2_handler, %eax
	movl	%ecx, 328(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 340(%edx)
	movl	$periph3_handler, %eax
	movl	%ecx, 336(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 348(%edx)
	movl	$mouse_handle, %eax
	movl	%ecx, 344(%edx)
	movzwl	%ax, %ecx
	orl	$524288, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	movl	%ecx, 352(%edx)
	movl	%eax, 356(%edx)
	movl	$FPU_handler, %eax
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 364(%edx)
	movl	$primaryHDD_handler, %eax
	movl	%ecx, 360(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %ecx
	movl	%eax, 372(%edx)
	movl	$secondaryHDD_handler, %eax
	movl	%ecx, 368(%edx)
	movzwl	%ax, %ecx
	xorw	%ax, %ax
	orl	$524288, %ecx
	orb	$142, %ah
	movl	%ecx, 376(%edx)
	movl	%eax, 380(%edx)
	ret
	.cfi_endproc
.LFE82:
	.size	AP_idt_Setup, .-AP_idt_Setup
	.comm	idt_entries,2048,32
	.comm	idt_ptr,6,4
	.align 16
	.type	init_idt, @function
init_idt:
.LFB83:
	.cfi_startproc
	movl	$2047, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	movl	$idt_entries, idt_ptr+2
	movw	%ax, idt_ptr
	movl	$idt_entries, %eax
	.align 16
.L720:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	$idt_entries+2048, %eax
	jne	.L720
	movl	$divByZero_handler, %eax
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+4
	movl	$debug_handler, %eax
	movl	%edx, idt_entries
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+12
	movl	$NMI_handler, %eax
	movl	%edx, idt_entries+8
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+20
	movl	$breakpoint_handler, %eax
	movl	%edx, idt_entries+16
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+28
	movl	$overflow_handler, %eax
	movl	%edx, idt_entries+24
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+36
	movl	$outOfBounds_handler, %eax
	movl	%edx, idt_entries+32
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+44
	movl	$invalidInstr_handler, %eax
	movl	%edx, idt_entries+40
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+52
	movl	$noCoprocessor_handler, %eax
	movl	%edx, idt_entries+48
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+60
	movl	$doubleFault_handler, %eax
	movl	%edx, idt_entries+56
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+68
	movl	$coprocessor_handler, %eax
	movl	%edx, idt_entries+64
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+76
	movl	$badTSS_handler, %eax
	movl	%edx, idt_entries+72
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orl	$524288, %edx
	orb	$142, %ah
	movl	%eax, idt_entries+84
	movl	$segmentNotPresent_handler, %eax
	movl	%edx, idt_entries+80
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+92
	movl	$stackFault_handler, %eax
	movl	%edx, idt_entries+88
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+100
	movl	$generalProtectionFault_handler, %eax
	movl	%edx, idt_entries+96
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+108
	movl	$pageFault_handler, %eax
	movl	%edx, idt_entries+104
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+116
	movl	$unknownInterrupt_handler, %eax
	movl	%edx, idt_entries+112
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+124
	movl	$coprocessorFault_handler, %eax
	movl	%edx, idt_entries+120
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orl	$524288, %edx
	orb	$142, %ah
	movl	%edx, idt_entries+128
	movl	%eax, idt_entries+132
	movl	$alignmentCheck_handler, %eax
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+140
	movl	$machineCheck_handler, %eax
	movl	%edx, idt_entries+136
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+148
	movl	$reserved_handler, %eax
	movl	%edx, idt_entries+144
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+156
	movl	%eax, idt_entries+164
	movl	%eax, idt_entries+172
	movl	%eax, idt_entries+180
	movl	%eax, idt_entries+188
	movl	%eax, idt_entries+196
	movl	%eax, idt_entries+204
	movl	%eax, idt_entries+212
	movl	%eax, idt_entries+220
	movl	%eax, idt_entries+228
	movl	%eax, idt_entries+236
	movl	%eax, idt_entries+244
	movl	%eax, idt_entries+252
	movl	$test_ttt, %eax
	movl	%edx, idt_entries+152
	movl	%edx, idt_entries+160
	movl	%edx, idt_entries+168
	movl	%edx, idt_entries+176
	movl	%edx, idt_entries+184
	movl	%edx, idt_entries+192
	movl	%edx, idt_entries+200
	movl	%edx, idt_entries+208
	movl	%edx, idt_entries+216
	movl	%edx, idt_entries+224
	movl	%edx, idt_entries+232
	movl	%edx, idt_entries+240
	movl	%edx, idt_entries+248
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orl	$524288, %edx
	orb	$142, %ah
	movl	%edx, idt_entries+256
	movl	%eax, idt_entries+260
	movl	$kb_handle, %eax
	movzwl	%ax, %edx
	orl	$524288, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	movl	%edx, idt_entries+264
	movl	%eax, idt_entries+268
	movl	$cascade_handler, %eax
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+276
	movl	$COM2_handler, %eax
	movl	%edx, idt_entries+272
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+284
	movl	$COM1_handler, %eax
	movl	%edx, idt_entries+280
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+292
	movl	$LPT2_handler, %eax
	movl	%edx, idt_entries+288
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+300
	movl	$floppyDisk_handler, %eax
	movl	%edx, idt_entries+296
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+308
	movl	$LPT1_handler, %eax
	movl	%edx, idt_entries+304
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+316
	movl	$RTC_handler, %eax
	movl	%edx, idt_entries+312
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+324
	movl	$periph1_handler, %eax
	movl	%edx, idt_entries+320
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+332
	movl	$periph2_handler, %eax
	movl	%edx, idt_entries+328
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+340
	movl	$periph3_handler, %eax
	movl	%edx, idt_entries+336
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+348
	movl	$mouse_handle, %eax
	movl	%edx, idt_entries+344
	movzwl	%ax, %edx
	orl	$524288, %edx
	xorw	%ax, %ax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	orb	$142, %ah
	movl	%edx, idt_entries+352
	movl	%eax, idt_entries+356
	movl	$FPU_handler, %eax
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+364
	movl	$primaryHDD_handler, %eax
	movl	%edx, idt_entries+360
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+372
	movl	$secondaryHDD_handler, %eax
	movl	%edx, idt_entries+368
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orb	$142, %ah
	orl	$524288, %edx
	movl	%eax, idt_entries+380
	movl	$test_handler, %eax
	movl	%edx, idt_entries+376
	movzwl	%ax, %edx
	xorw	%ax, %ax
	orl	$524288, %edx
	orb	$142, %ah
	movl	%edx, idt_entries+408
	movl	%eax, idt_entries+412
	movl	%edx, idt_entries+400
	pushl	$idt_ptr
	.cfi_def_cfa_offset 32
	movl	%eax, idt_entries+404
	call	lidt
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE83:
	.size	init_idt, .-init_idt
	.align 16
	.type	makeFlagByte, @function
makeFlagByte:
.LFB84:
	.cfi_startproc
	sall	$5, %edx
	sall	$7, %eax
	andl	$96, %edx
	orl	%edx, %eax
	orl	$14, %eax
	ret
	.cfi_endproc
.LFE84:
	.size	makeFlagByte, .-makeFlagByte
	.align 16
	.type	idtSetEntry, @function
idtSetEntry:
.LFB85:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	12(%esp), %ebx
	sall	$16, %ecx
	leal	(%ebx,%eax,8), %eax
	movzwl	%dx, %ebx
	xorw	%dx, %dx
	orl	%ebx, %ecx
	movl	%ecx, (%eax)
	movl	8(%esp), %ecx
	sall	$8, %ecx
	orl	%ecx, %edx
	movl	%edx, 4(%eax)
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE85:
	.size	idtSetEntry, .-idtSetEntry
	.globl	APIC_LOCAL_BASE
	.data
	.align 4
	.type	APIC_LOCAL_BASE, @object
	.size	APIC_LOCAL_BASE, 4
APIC_LOCAL_BASE:
	.long	-18874368
	.globl	APIC_IO_BASE
	.align 4
	.type	APIC_IO_BASE, @object
	.size	APIC_IO_BASE, 4
APIC_IO_BASE:
	.long	-20971520
	.globl	counter
	.section	.bss
	.align 4
	.type	counter, @object
	.size	counter, 4
counter:
	.zero	4
	.comm	IOAPIC_entry,4,4
	.comm	LAPIC_entry,4,4
	.comm	ISD_entry,4,4
	.comm	madt,4,4
	.globl	tick
	.align 4
	.type	tick, @object
	.size	tick, 4
tick:
	.zero	4
	.globl	rtc_timer
	.align 4
	.type	rtc_timer, @object
	.size	rtc_timer, 4
rtc_timer:
	.zero	4
	.local	scancodes
	.comm	scancodes,4,4
	.data
	.align 32
	.type	Main_key_codes, @object
	.size	Main_key_codes, 332
Main_key_codes:
	.long	97
	.long	4097
	.long	49
	.long	50
	.long	51
	.long	52
	.long	53
	.long	54
	.long	55
	.long	56
	.long	57
	.long	48
	.long	45
	.long	61
	.long	8
	.long	16384
	.long	113
	.long	119
	.long	101
	.long	114
	.long	116
	.long	121
	.long	117
	.long	105
	.long	111
	.long	112
	.long	91
	.long	93
	.long	13
	.long	0
	.long	97
	.long	115
	.long	100
	.long	102
	.long	103
	.long	104
	.long	106
	.long	107
	.long	108
	.long	59
	.long	39
	.long	96
	.long	0
	.long	92
	.long	122
	.long	120
	.long	99
	.long	118
	.long	98
	.long	110
	.long	109
	.long	44
	.long	46
	.long	47
	.long	0
	.long	42
	.long	0
	.long	32
	.long	0
	.long	4609
	.long	4610
	.long	4611
	.long	4612
	.long	4613
	.long	4614
	.long	4615
	.long	4616
	.long	4617
	.long	4618
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	50
	.long	51
	.long	48
	.long	46
	.long	97
	.long	97
	.long	97
	.long	0
	.long	0
	.section	.rodata
	.align 32
	.type	Special_key_codes, @object
	.size	Special_key_codes, 332
Special_key_codes:
	.long	97
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	16387
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	16386
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	16390
	.long	0
	.long	16392
	.long	0
	.long	16385
	.long	4609
	.long	4610
	.long	4611
	.long	4612
	.long	4613
	.long	4614
	.long	4615
	.long	4616
	.long	4617
	.long	4618
	.long	12303
	.long	16400
	.long	16396
	.long	4352
	.long	16398
	.long	50
	.long	51
	.long	48
	.long	46
	.long	97
	.long	97
	.long	97
	.long	4619
	.long	4619
	.local	call
	.comm	call,4,4
	.local	_ctrl
	.comm	_ctrl,4,4
	.local	_shift
	.comm	_shift,4,4
	.local	_alt
	.comm	_alt,4,4
	.local	_numlock
	.comm	_numlock,4,4
	.local	_capslock
	.comm	_capslock,4,4
	.local	_scrolllock
	.comm	_scrolllock,4,4
	.local	_arrow_up
	.comm	_arrow_up,4,4
	.local	_arrow_down
	.comm	_arrow_down,4,4
.LC4:
	.string	"Kernel Panic: abort()\n"
	.text
	.align 16
	.globl	abort
	.type	abort, @function
abort:
.LFB89:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC4
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L727:
	jmp	.L727
	.cfi_endproc
.LFE89:
	.size	abort, .-abort
	.align 16
	.globl	StrToInt
	.type	StrToInt, @function
StrToInt:
.LFB90:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	20(%esp), %esi
	testl	%esi, %esi
	je	.L737
	cmpb	$0, (%esi)
	je	.L736
	xorl	%eax, %eax
	.align 16
.L732:
	addl	$1, %eax
	cmpb	$0, (%esi,%eax)
	jne	.L732
	testl	%eax, %eax
	movl	%eax, %ebp
	js	.L737
	xorl	%edx, %edx
	movl	$1, %ebx
	.align 16
.L733:
	leal	(%ebx,%ebx,4), %ebx
	addl	$1, %edx
	addl	%ebx, %ebx
	cmpl	%edx, %eax
	jge	.L733
.L731:
	movl	%ebx, %eax
	movl	$1374389535, %edx
	sarl	$31, %ebx
	imull	%edx
	xorl	%edi, %edi
	sarl	$5, %edx
	movl	%edx, %ecx
	subl	%ebx, %ecx
	xorl	%ebx, %ebx
	.align 16
.L734:
	movsbl	(%esi,%ebx), %eax
	addl	$1, %ebx
	subl	$48, %eax
	imull	%ecx, %eax
	addl	%eax, %edi
	movl	$1717986919, %eax
	imull	%ecx
	sarl	$31, %ecx
	sarl	$2, %edx
	subl	%ecx, %edx
	cmpl	%ebp, %ebx
	movl	%edx, %ecx
	jle	.L734
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	movl	%edi, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L737:
	.cfi_restore_state
	xorl	%edi, %edi
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	movl	%edi, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L736:
	.cfi_restore_state
	movl	$10, %ebx
	xorl	%ebp, %ebp
	jmp	.L731
	.cfi_endproc
.LFE90:
	.size	StrToInt, .-StrToInt
	.comm	malloc,4,4
	.comm	free,4,4
	.comm	kfree,4,4
	.section	.rodata
	.align 32
	.type	powf_log_table, @object
	.size	powf_log_table, 2048
powf_log_table:
	.long	0
	.long	1072693248
	.long	0
	.long	-2147483648
	.long	0
	.long	1072676864
	.long	-1576045785
	.long	1065823355
	.long	66588640
	.long	1072660734
	.long	746536380
	.long	1066866022
	.long	792917039
	.long	1072644852
	.long	728057311
	.long	1067532010
	.long	-754078228
	.long	1072629212
	.long	-1114173219
	.long	1067902960
	.long	1822107338
	.long	1072613810
	.long	1578293841
	.long	1068271090
	.long	-1550063385
	.long	1072598639
	.long	-792247276
	.long	1068567692
	.long	1217975800
	.long	1072583695
	.long	333745664
	.long	1068749000
	.long	1399841193
	.long	1072568972
	.long	1365893160
	.long	1068928959
	.long	-505290270
	.long	1072554465
	.long	1862992251
	.long	1069107590
	.long	909153661
	.long	1072540171
	.long	-494394944
	.long	1069284912
	.long	-1244918057
	.long	1072526083
	.long	-1259365857
	.long	1069460945
	.long	-401687589
	.long	1072512198
	.long	-1025455604
	.long	1069591613
	.long	1963413621
	.long	1072498512
	.long	1190883606
	.long	1069678368
	.long	609215219
	.long	1072485020
	.long	1366696622
	.long	1069764505
	.long	-604924971
	.long	1072471717
	.long	-1667652275
	.long	1069850033
	.long	-1621875762
	.long	1072458601
	.long	-1281036672
	.long	1069934961
	.long	-1908874354
	.long	1072445667
	.long	-199438918
	.long	1070019297
	.long	-473927426
	.long	1072432911
	.long	-1894778446
	.long	1070103050
	.long	-176505505
	.long	1072420330
	.long	-1974469915
	.long	1070186227
	.long	847306473
	.long	1072407921
	.long	-1046517743
	.long	1070268836
	.long	580400986
	.long	1072395679
	.long	-405412922
	.long	1070350885
	.long	1700691748
	.long	1072383601
	.long	-2013641439
	.long	1070432382
	.long	-1317123304
	.long	1072371684
	.long	106123426
	.long	1070513334
	.long	-739530793
	.long	1072359925
	.long	-1586536065
	.long	1070593747
	.long	-1356305462
	.long	1072348321
	.long	965848990
	.long	1070634863
	.long	982508859
	.long	1072336869
	.long	-1195332701
	.long	1070674542
	.long	-2119594250
	.long	1072325565
	.long	-1209825521
	.long	1070713963
	.long	-1551730120
	.long	1072314407
	.long	-1885180918
	.long	1070753129
	.long	-770891566
	.long	1072303392
	.long	-2011266349
	.long	1070792043
	.long	1367823980
	.long	1072292518
	.long	-648145335
	.long	1070830708
	.long	2011566961
	.long	1072281781
	.long	-1414182522
	.long	1070869128
	.long	-1404643392
	.long	1072271179
	.long	405503971
	.long	1070907306
	.long	1717986918
	.long	1072260710
	.long	685610788
	.long	1070945244
	.long	773627650
	.long	1072250371
	.long	-648211891
	.long	1070982945
	.long	-1696777203
	.long	1072240159
	.long	386683425
	.long	1071020414
	.long	1396523109
	.long	1072230073
	.long	-1049102528
	.long	1071057651
	.long	209510600
	.long	1072220110
	.long	-1431467912
	.long	1071094661
	.long	-1978287967
	.long	1072210267
	.long	-1752205611
	.long	1071131446
	.long	-1655891006
	.long	1072200543
	.long	1076271085
	.long	1071168009
	.long	617240809
	.long	1072190936
	.long	1341205951
	.long	1071204352
	.long	204522252
	.long	1072181443
	.long	1714393762
	.long	1071240478
	.long	1270700383
	.long	1072172062
	.long	372154056
	.long	1071276390
	.long	-404232216
	.long	1072162791
	.long	-410002454
	.long	1071312089
	.long	-251167678
	.long	1072153629
	.long	1452251874
	.long	1071347580
	.long	-2097542168
	.long	1072144574
	.long	-733413830
	.long	1071382863
	.long	-993056022
	.long	1072135623
	.long	-956822582
	.long	1071417942
	.long	-394939521
	.long	1072126775
	.long	-1975920912
	.long	1071452819
	.long	711737438
	.long	1072118029
	.long	1867174934
	.long	1071487496
	.long	-780903145
	.long	1072109381
	.long	-1119656536
	.long	1071521975
	.long	776491263
	.long	1072100832
	.long	-1319980570
	.long	1071556259
	.long	-1689032083
	.long	1072092378
	.long	2130169963
	.long	1071590350
	.long	2087498071
	.long	1072084019
	.long	1345936826
	.long	1071624250
	.long	1049880895
	.long	1072075753
	.long	-1561476812
	.long	1071651316
	.long	1471204267
	.long	1072067578
	.long	-1144373862
	.long	1071668078
	.long	1179936070
	.long	1072059493
	.long	2048907084
	.long	1071684748
	.long	-1854111565
	.long	1072051496
	.long	-519590390
	.long	1071701326
	.long	-933688543
	.long	1072043586
	.long	-279021453
	.long	1071717814
	.long	-2112659589
	.long	1072035762
	.long	-1612970313
	.long	1071734213
	.long	1570203097
	.long	1072028022
	.long	-383114480
	.long	1071750523
	.long	22967740
	.long	1072020365
	.long	-1107634889
	.long	1071766746
	.long	456911414
	.long	1072012789
	.long	220073435
	.long	1071782883
	.long	1613453323
	.long	1072005293
	.long	-1047085325
	.long	1071798933
	.long	-1944037829
	.long	1071997876
	.long	-1028692587
	.long	1071814899
	.long	1641532003
	.long	1071990537
	.long	-200406587
	.long	1071830781
	.long	-1431655765
	.long	1071983274
	.long	902281374
	.long	1071846581
	.long	912402379
	.long	1071976087
	.long	1685290227
	.long	1071862298
	.long	-619892187
	.long	1071968973
	.long	1497160307
	.long	1071877934
	.long	1960267125
	.long	1071961933
	.long	-369766609
	.long	1071893489
	.long	-438261969
	.long	1071954964
	.long	-383230157
	.long	1071908965
	.long	370631696
	.long	1071948067
	.long	640134627
	.long	1071924363
	.long	-216917540
	.long	1071941238
	.long	1830856822
	.long	1071939682
	.long	1877699270
	.long	1071934479
	.long	-2027272955
	.long	1071954924
	.long	-2061584302
	.long	1071927787
	.long	978671960
	.long	1071970090
	.long	811983867
	.long	1071921162
	.long	1237060753
	.long	1071985180
	.long	1956123719
	.long	1071914602
	.long	1972430533
	.long	1072000195
	.long	1502180680
	.long	1071908107
	.long	2066579802
	.long	1072015136
	.long	-336860180
	.long	1071901675
	.long	354474752
	.long	1072030004
	.long	1026601939
	.long	1071895307
	.long	-79867391
	.long	1072044798
	.long	1667948464
	.long	1071889000
	.long	-492419205
	.long	1072059521
	.long	2033366159
	.long	1071882754
	.long	2111663637
	.long	1072074173
	.long	-1651910498
	.long	1071876568
	.long	2093991576
	.long	1072088754
	.long	-205500828
	.long	1071870441
	.long	-1931326249
	.long	1072103265
	.long	-1554369117
	.long	1071864373
	.long	1493063446
	.long	1072117707
	.long	-671724743
	.long	1071858362
	.long	-1986155959
	.long	1072132080
	.long	-1053482544
	.long	1071852408
	.long	-992590560
	.long	1072146385
	.long	-1834939080
	.long	1071846510
	.long	-1369054172
	.long	1072160623
	.long	-2087273826
	.long	1071840667
	.long	-407027535
	.long	1072174794
	.long	-819040275
	.long	1071834878
	.long	269159650
	.long	1072188900
	.long	-1272582903
	.long	1071829143
	.long	-1002000833
	.long	1072202939
	.long	1959455126
	.long	1071823461
	.long	-1623541517
	.long	1072216914
	.long	1457924678
	.long	1071817831
	.long	965684568
	.long	1072230825
	.long	-1549326102
	.long	1071812252
	.long	701725066
	.long	1072244672
	.long	-1483715975
	.long	1071806724
	.long	76038298
	.long	1072258456
	.long	-1302094158
	.long	1071801246
	.long	1546184955
	.long	1072272177
	.long	386933991
	.long	1071795818
	.long	-1053494333
	.long	1072285836
	.long	731877835
	.long	1071790438
	.long	-1036444132
	.long	1072299434
	.long	1227133513
	.long	1071785106
	.long	-338145664
	.long	1072312971
	.long	-878082203
	.long	1071779821
	.long	-925610246
	.long	1072326448
	.long	304068481
	.long	1071774584
	.long	-501852693
	.long	1072339865
	.long	2119102807
	.long	1071769392
	.long	-1095278956
	.long	1072353223
	.long	1959107889
	.long	1071764246
	.long	-469216608
	.long	1072366522
	.long	1556691203
	.long	1071759145
	.long	-711326209
	.long	1072379763
	.long	-1605944293
	.long	1071754088
	.long	356845112
	.long	1072392947
	.long	-1413062833
	.long	1071749075
	.long	590429784
	.long	1072406073
	.long	-296204641
	.long	1071744105
	.long	2111664295
	.long	1072419142
	.long	-645166761
	.long	1071739178
	.long	-1574533327
	.long	1072432155
	.long	-513927711
	.long	1071734293
	.long	189709370
	.long	1072445113
	.long	2083516050
	.long	1071729450
	.long	855940000
	.long	1072458015
	.long	582368447
	.long	1071724648
	.long	-1855332830
	.long	1072470862
	.long	1341044641
	.long	1071719886
	.long	-1659232364
	.long	1072483655
	.long	-2129437567
	.long	1071715164
	.long	-885952285
	.long	1072496394
	.long	898528723
	.long	1071710482
	.long	-1890400557
	.long	1072509080
	.long	-286331153
	.long	1071705838
	.long	1538147939
	.long	1072521713
	.long	819786289
	.long	1071701234
	.long	-1593348190
	.long	1072534293
	.long	-2129735849
	.long	1071696667
	.long	-826595364
	.long	1072546821
	.long	1732126728
	.long	1071692138
	.long	1388684257
	.long	1072559298
	.long	1830641798
	.long	1071687646
	.long	-1714932935
	.long	1072571723
	.long	508383884
	.long	1071683191
	.long	252636409
	.long	1072584098
	.long	139673733
	.long	1071678772
	.long	479608589
	.long	1072596422
	.long	-1165031615
	.long	1071674388
	.long	722493595
	.long	1072608696
	.long	-969831325
	.long	1071670040
	.long	-1578457025
	.long	1072620920
	.long	-1103927337
	.long	1071665727
	.long	-414005560
	.long	1072633095
	.long	927712936
	.long	1071661449
	.long	1614538525
	.long	1072645222
	.long	-941128292
	.long	1071657204
	.long	1885582333
	.long	1072657300
	.long	136348168
	.long	1071652994
	.long	2052539426
	.long	1072669330
	.long	-1850400930
	.long	1071648816
	.long	-545788550
	.long	1072681312
	.long	0
	.long	1071644672
	.long	0
	.long	1072693248
	.comm	system_dir,4,4
	.comm	_cur_dir,4,4
	.comm	_prev_dir,4,4
	.comm	system_pdirCap,4,4
	.comm	_cur_pdirCap,4,4
	.comm	_prev_pdirCap,4,4
	.comm	vga,4,4
	.globl	maxmem
	.section	.bss
	.align 4
	.type	maxmem, @object
	.size	maxmem, 4
maxmem:
	.zero	4
	.globl	memAvailable
	.align 4
	.type	memAvailable, @object
	.size	memAvailable, 4
memAvailable:
	.zero	4
	.globl	blockID
	.data
	.align 2
	.type	blockID, @object
	.size	blockID, 2
blockID:
	.value	10
	.globl	max_mem
	.section	.bss
	.align 4
	.type	max_mem, @object
	.size	max_mem, 4
max_mem:
	.zero	4
	.comm	mmap_info,4,4
	.comm	Fmemmap,4,4
	.globl	strMemoryTypes
	.section	.rodata
.LC5:
	.string	"Available"
.LC6:
	.string	"Reserved"
.LC7:
	.string	"ACPI Reclaim"
	.align 4
.LC8:
	.string	"ACPI NVS MemoryKernel ReservedKernel Page Directory"
	.data
	.align 4
	.type	strMemoryTypes, @object
	.size	strMemoryTypes, 16
strMemoryTypes:
	.long	.LC5
	.long	.LC6
	.long	.LC7
	.long	.LC8
	.globl	frame_stack_end
	.section	.bss
	.align 4
	.type	frame_stack_end, @object
	.size	frame_stack_end, 4
frame_stack_end:
	.zero	4
	.globl	kSupportedDevices
	.section	.rodata
.LC9:
	.string	"ATI SB600"
.LC10:
	.string	"ATI SB700/800"
.LC11:
	.string	"ATI IXP700"
.LC12:
	.string	"SiS 966"
.LC13:
	.string	"SiS 968"
.LC14:
	.string	"Acer Labs M5288"
.LC15:
	.string	"NVIDIA MCP65"
.LC16:
	.string	"NVIDIA MCP67"
.LC17:
	.string	"NVIDIA MCP73"
.LC18:
	.string	"NVIDIA MCP77"
.LC19:
	.string	"NVIDIA MCP79"
.LC20:
	.string	"NVIDIA MCP89"
.LC21:
	.string	"VIA VT8251"
.LC22:
	.string	"Marvell 6121"
.LC23:
	.string	"Marvell 6145"
.LC24:
	.string	"JMicron JMB360"
.LC25:
	.string	"JMicron JMB361"
.LC26:
	.string	"JMicron JMB362"
.LC27:
	.string	"JMicron JMB363"
.LC28:
	.string	"JMicron JMB366"
.LC29:
	.string	"Intel ICH6R"
.LC30:
	.string	"Intel ICH6-M"
.LC31:
	.string	"Intel 63xxESB"
.LC32:
	.string	"Intel ESB2"
.LC33:
	.string	"Intel ICH7R (AHCI mode)"
.LC34:
	.string	"Intel ICH7R (RAID mode)"
.LC35:
	.string	"Intel ICH7-M (AHCI mode)"
.LC36:
	.string	"Intel ICH7-M DH (RAID mode)"
.LC37:
	.string	"Intel ICH8 (AHCI mode)"
	.align 4
.LC38:
	.string	"Intel ICH8R / ICH9 (RAID mode)"
.LC39:
	.string	"Intel ICH8M (AHCI mode)"
.LC40:
	.string	"Intel ICH8M (RAID mode)"
.LC41:
	.string	"Intel ICH9 (AHCI mode)"
.LC42:
	.string	"Intel ICH9"
.LC43:
	.string	"Intel ICH9M"
.LC44:
	.string	"Intel ICH10"
.LC45:
	.string	"UnKnown Device"
	.align 32
	.type	kSupportedDevices, @object
	.size	kSupportedDevices, 944
kSupportedDevices:
	.value	4098
	.value	17280
	.long	.LC9
	.value	4098
	.value	17296
	.long	.LC10
	.value	4098
	.value	17297
	.long	.LC11
	.value	4098
	.value	17298
	.long	.LC10
	.value	4098
	.value	17299
	.long	.LC10
	.value	4098
	.value	17300
	.long	.LC10
	.value	4098
	.value	17301
	.long	.LC10
	.value	4153
	.value	4484
	.long	.LC12
	.value	4153
	.value	4485
	.long	.LC12
	.value	4153
	.value	390
	.long	.LC13
	.value	4281
	.value	21128
	.long	.LC14
	.value	4318
	.value	1100
	.long	.LC15
	.value	4318
	.value	1101
	.long	.LC15
	.value	4318
	.value	1102
	.long	.LC15
	.value	4318
	.value	1103
	.long	.LC15
	.value	4318
	.value	1116
	.long	.LC15
	.value	4318
	.value	1117
	.long	.LC15
	.value	4318
	.value	1118
	.long	.LC15
	.value	4318
	.value	1119
	.long	.LC15
	.value	4318
	.value	1360
	.long	.LC16
	.value	4318
	.value	1361
	.long	.LC16
	.value	4318
	.value	1362
	.long	.LC16
	.value	4318
	.value	1363
	.long	.LC16
	.value	4318
	.value	1364
	.long	.LC16
	.value	4318
	.value	1365
	.long	.LC16
	.value	4318
	.value	1366
	.long	.LC16
	.value	4318
	.value	1367
	.long	.LC16
	.value	4318
	.value	1368
	.long	.LC16
	.value	4318
	.value	1369
	.long	.LC16
	.value	4318
	.value	1370
	.long	.LC16
	.value	4318
	.value	1371
	.long	.LC16
	.value	4318
	.value	2032
	.long	.LC17
	.value	4318
	.value	2033
	.long	.LC17
	.value	4318
	.value	2034
	.long	.LC17
	.value	4318
	.value	2035
	.long	.LC17
	.value	4318
	.value	2036
	.long	.LC17
	.value	4318
	.value	2037
	.long	.LC17
	.value	4318
	.value	2038
	.long	.LC17
	.value	4318
	.value	2039
	.long	.LC17
	.value	4318
	.value	2040
	.long	.LC17
	.value	4318
	.value	2041
	.long	.LC17
	.value	4318
	.value	2042
	.long	.LC17
	.value	4318
	.value	2043
	.long	.LC17
	.value	4318
	.value	2768
	.long	.LC18
	.value	4318
	.value	2769
	.long	.LC18
	.value	4318
	.value	2770
	.long	.LC18
	.value	4318
	.value	2771
	.long	.LC18
	.value	4318
	.value	2772
	.long	.LC18
	.value	4318
	.value	2773
	.long	.LC18
	.value	4318
	.value	2774
	.long	.LC18
	.value	4318
	.value	2775
	.long	.LC18
	.value	4318
	.value	2776
	.long	.LC18
	.value	4318
	.value	2777
	.long	.LC18
	.value	4318
	.value	2778
	.long	.LC18
	.value	4318
	.value	2779
	.long	.LC18
	.value	4318
	.value	2740
	.long	.LC19
	.value	4318
	.value	2741
	.long	.LC19
	.value	4318
	.value	2742
	.long	.LC19
	.value	4318
	.value	2743
	.long	.LC19
	.value	4318
	.value	2744
	.long	.LC19
	.value	4318
	.value	2745
	.long	.LC19
	.value	4318
	.value	2746
	.long	.LC19
	.value	4318
	.value	2747
	.long	.LC19
	.value	4318
	.value	2748
	.long	.LC19
	.value	4318
	.value	2749
	.long	.LC19
	.value	4318
	.value	2750
	.long	.LC19
	.value	4318
	.value	2751
	.long	.LC19
	.value	4318
	.value	3460
	.long	.LC20
	.value	4318
	.value	3461
	.long	.LC20
	.value	4318
	.value	3462
	.long	.LC20
	.value	4318
	.value	3463
	.long	.LC20
	.value	4318
	.value	3464
	.long	.LC20
	.value	4318
	.value	3465
	.long	.LC20
	.value	4318
	.value	3466
	.long	.LC20
	.value	4318
	.value	3467
	.long	.LC20
	.value	4318
	.value	3468
	.long	.LC20
	.value	4318
	.value	3469
	.long	.LC20
	.value	4318
	.value	3470
	.long	.LC20
	.value	4318
	.value	3471
	.long	.LC20
	.value	4358
	.value	13129
	.long	.LC21
	.value	4358
	.value	25223
	.long	.LC21
	.value	4523
	.value	24865
	.long	.LC22
	.value	4523
	.value	24901
	.long	.LC23
	.value	6523
	.value	9056
	.long	.LC24
	.value	6523
	.value	9057
	.long	.LC25
	.value	6523
	.value	9058
	.long	.LC26
	.value	6523
	.value	9059
	.long	.LC27
	.value	6523
	.value	9062
	.long	.LC28
	.value	-32634
	.value	9810
	.long	.LC29
	.value	-32634
	.value	9811
	.long	.LC30
	.value	-32634
	.value	9857
	.long	.LC31
	.value	-32634
	.value	9858
	.long	.LC32
	.value	-32634
	.value	9859
	.long	.LC32
	.value	-32634
	.value	10177
	.long	.LC33
	.value	-32634
	.value	10179
	.long	.LC34
	.value	-32634
	.value	10181
	.long	.LC35
	.value	-32634
	.value	10182
	.long	.LC36
	.value	-32634
	.value	10273
	.long	.LC37
	.value	-32634
	.value	10274
	.long	.LC38
	.value	-32634
	.value	10276
	.long	.LC37
	.value	-32634
	.value	10281
	.long	.LC39
	.value	-32634
	.value	10282
	.long	.LC40
	.value	-32634
	.value	10530
	.long	.LC41
	.value	-32634
	.value	10531
	.long	.LC41
	.value	-32634
	.value	10532
	.long	.LC42
	.value	-32634
	.value	10533
	.long	.LC42
	.value	-32634
	.value	10535
	.long	.LC42
	.value	-32634
	.value	10537
	.long	.LC43
	.value	-32634
	.value	10538
	.long	.LC43
	.value	-32634
	.value	10539
	.long	.LC43
	.value	-32634
	.value	10540
	.long	.LC43
	.value	-32634
	.value	10543
	.long	.LC43
	.value	-32634
	.value	10573
	.long	.LC42
	.value	-32634
	.value	10574
	.long	.LC43
	.value	-32634
	.value	14853
	.long	.LC44
	.value	-32634
	.value	14882
	.long	.LC44
	.value	-32634
	.value	14885
	.long	.LC44
	.value	0
	.value	0
	.long	.LC45
	.comm	ident,512,32
	.comm	HDD,1,1
	.comm	drive,2,2
	.globl	PciVenTable
	.section	.bss
	.align 4
	.type	PciVenTable, @object
	.size	PciVenTable, 0
PciVenTable:
	.zero	1
	.globl	PciDevTable
	.align 4
	.type	PciDevTable, @object
	.size	PciDevTable, 0
PciDevTable:
	.zero	1
	.globl	PciClassCodeTable
	.section	.rodata
	.align 4
.LC46:
	.string	"Pre-2.0 PCI Specification Device"
.LC47:
	.string	"Non-VGA"
.LC48:
	.string	""
.LC49:
	.string	"VGA Compatible"
.LC50:
	.string	"Mass Storage Controller"
.LC51:
	.string	"SCSI"
.LC52:
	.string	"IDE"
.LC53:
	.string	"Floppy"
.LC54:
	.string	"IPI"
.LC55:
	.string	"RAID"
.LC56:
	.string	"Other"
.LC57:
	.string	"Network Controller"
.LC58:
	.string	"Ethernet"
.LC59:
	.string	"Token Ring"
.LC60:
	.string	"FDDI"
.LC61:
	.string	"ATM"
.LC62:
	.string	"Display Controller"
.LC63:
	.string	"PC Compatible"
.LC64:
	.string	"VGA"
.LC65:
	.string	"8514"
.LC66:
	.string	"XGA"
.LC67:
	.string	"Multimedia Device"
.LC68:
	.string	"Video"
.LC69:
	.string	"Audio"
.LC70:
	.string	"Memory Controller"
.LC71:
	.string	"RAM"
.LC72:
	.string	"Flash"
.LC73:
	.string	"Bridge Device"
.LC74:
	.string	"Host/PCI"
.LC75:
	.string	"PCI/ISA"
.LC76:
	.string	"PCI/EISA"
.LC77:
	.string	"PCI/Micro Channel"
.LC78:
	.string	"PCI/PCI"
.LC79:
	.string	"PCI/PCMCIA"
.LC80:
	.string	"PCI/NuBus"
.LC81:
	.string	"PCI/CardBus"
	.align 4
.LC82:
	.string	"Simple Communications Controller"
.LC83:
	.string	"Serial"
.LC84:
	.string	"Generic XT Compatible"
.LC85:
	.string	"16450 Compatible"
.LC86:
	.string	"16550 Compatible"
.LC87:
	.string	"Parallel"
.LC88:
	.string	"Standard"
.LC89:
	.string	"Bidirectional"
.LC90:
	.string	"ECP 1.X Compliant"
.LC91:
	.string	"Base Systems Peripheral"
	.align 4
.LC92:
	.string	"PIC (Programmable Interrupt Controller)"
.LC93:
	.string	"Generic 8259"
.LC94:
	.string	"ISA"
.LC95:
	.string	"PCI"
.LC96:
	.string	"DMA (Direct Memory Access)"
.LC97:
	.string	"EISA"
.LC98:
	.string	"System Timer"
.LC99:
	.string	"RTC (Real Time Clock)"
.LC100:
	.string	"Generic"
.LC101:
	.string	"Input Device"
.LC102:
	.string	"Keyboard"
.LC103:
	.string	"Digitizer (Pen)"
.LC104:
	.string	"Mouse"
.LC105:
	.string	"Docking Station"
.LC106:
	.string	"Processor"
.LC107:
	.string	"i386"
.LC108:
	.string	"i486"
.LC109:
	.string	"Pentium"
.LC110:
	.string	"Alpha"
.LC111:
	.string	"Power PC"
.LC112:
	.string	"Co-processor"
.LC113:
	.string	"Serial Bus Controller"
.LC114:
	.string	"Firewire (IEEE 1394)"
.LC115:
	.string	"ACCESS.bus"
	.align 4
.LC116:
	.string	"SSA (Serial Storage Archetecture)"
.LC117:
	.string	"USB (Universal Serial Bus)"
.LC118:
	.string	"Fibre Channel"
.LC119:
	.string	"Unknown"
	.align 4
.LC120:
	.string	"Device Does Not Fit In Class Codes"
.LC121:
	.string	"UDF"
	.data
	.align 32
	.type	PciClassCodeTable, @object
	.size	PciClassCodeTable, 1104
PciClassCodeTable:
	.byte	0
	.byte	0
	.byte	0
	.zero	1
	.long	.LC46
	.long	.LC47
	.long	.LC48
	.byte	0
	.byte	1
	.byte	0
	.zero	1
	.long	.LC46
	.long	.LC49
	.long	.LC48
	.byte	1
	.byte	0
	.byte	0
	.zero	1
	.long	.LC50
	.long	.LC51
	.long	.LC48
	.byte	1
	.byte	1
	.byte	0
	.zero	1
	.long	.LC50
	.long	.LC52
	.long	.LC48
	.byte	1
	.byte	2
	.byte	0
	.zero	1
	.long	.LC50
	.long	.LC53
	.long	.LC48
	.byte	1
	.byte	3
	.byte	0
	.zero	1
	.long	.LC50
	.long	.LC54
	.long	.LC48
	.byte	1
	.byte	4
	.byte	0
	.zero	1
	.long	.LC50
	.long	.LC55
	.long	.LC48
	.byte	1
	.byte	-128
	.byte	0
	.zero	1
	.long	.LC50
	.long	.LC56
	.long	.LC48
	.byte	2
	.byte	0
	.byte	0
	.zero	1
	.long	.LC57
	.long	.LC58
	.long	.LC48
	.byte	2
	.byte	1
	.byte	0
	.zero	1
	.long	.LC57
	.long	.LC59
	.long	.LC48
	.byte	2
	.byte	2
	.byte	0
	.zero	1
	.long	.LC57
	.long	.LC60
	.long	.LC48
	.byte	2
	.byte	3
	.byte	0
	.zero	1
	.long	.LC57
	.long	.LC61
	.long	.LC48
	.byte	2
	.byte	-128
	.byte	0
	.zero	1
	.long	.LC57
	.long	.LC56
	.long	.LC48
	.byte	3
	.byte	0
	.byte	0
	.zero	1
	.long	.LC62
	.long	.LC63
	.long	.LC64
	.byte	3
	.byte	0
	.byte	1
	.zero	1
	.long	.LC62
	.long	.LC63
	.long	.LC65
	.byte	3
	.byte	1
	.byte	0
	.zero	1
	.long	.LC62
	.long	.LC66
	.long	.LC48
	.byte	3
	.byte	-128
	.byte	0
	.zero	1
	.long	.LC62
	.long	.LC56
	.long	.LC48
	.byte	4
	.byte	0
	.byte	0
	.zero	1
	.long	.LC67
	.long	.LC68
	.long	.LC48
	.byte	4
	.byte	1
	.byte	0
	.zero	1
	.long	.LC67
	.long	.LC69
	.long	.LC48
	.byte	4
	.byte	-128
	.byte	0
	.zero	1
	.long	.LC67
	.long	.LC56
	.long	.LC48
	.byte	5
	.byte	0
	.byte	0
	.zero	1
	.long	.LC70
	.long	.LC71
	.long	.LC48
	.byte	5
	.byte	1
	.byte	0
	.zero	1
	.long	.LC70
	.long	.LC72
	.long	.LC48
	.byte	5
	.byte	-128
	.byte	0
	.zero	1
	.long	.LC70
	.long	.LC56
	.long	.LC48
	.byte	6
	.byte	0
	.byte	0
	.zero	1
	.long	.LC73
	.long	.LC74
	.long	.LC48
	.byte	6
	.byte	1
	.byte	0
	.zero	1
	.long	.LC73
	.long	.LC75
	.long	.LC48
	.byte	6
	.byte	2
	.byte	0
	.zero	1
	.long	.LC73
	.long	.LC76
	.long	.LC48
	.byte	6
	.byte	3
	.byte	0
	.zero	1
	.long	.LC73
	.long	.LC77
	.long	.LC48
	.byte	6
	.byte	4
	.byte	0
	.zero	1
	.long	.LC73
	.long	.LC78
	.long	.LC48
	.byte	6
	.byte	5
	.byte	0
	.zero	1
	.long	.LC73
	.long	.LC79
	.long	.LC48
	.byte	6
	.byte	6
	.byte	0
	.zero	1
	.long	.LC73
	.long	.LC80
	.long	.LC48
	.byte	6
	.byte	7
	.byte	0
	.zero	1
	.long	.LC73
	.long	.LC81
	.long	.LC48
	.byte	6
	.byte	-128
	.byte	0
	.zero	1
	.long	.LC73
	.long	.LC56
	.long	.LC48
	.byte	7
	.byte	0
	.byte	0
	.zero	1
	.long	.LC82
	.long	.LC83
	.long	.LC84
	.byte	7
	.byte	0
	.byte	1
	.zero	1
	.long	.LC82
	.long	.LC83
	.long	.LC85
	.byte	7
	.byte	0
	.byte	2
	.zero	1
	.long	.LC82
	.long	.LC83
	.long	.LC86
	.byte	7
	.byte	1
	.byte	0
	.zero	1
	.long	.LC82
	.long	.LC87
	.long	.LC88
	.byte	7
	.byte	1
	.byte	0
	.zero	1
	.long	.LC82
	.long	.LC87
	.long	.LC89
	.byte	7
	.byte	1
	.byte	1
	.zero	1
	.long	.LC82
	.long	.LC87
	.long	.LC90
	.byte	7
	.byte	-128
	.byte	2
	.zero	1
	.long	.LC82
	.long	.LC56
	.long	.LC48
	.byte	8
	.byte	0
	.byte	0
	.zero	1
	.long	.LC91
	.long	.LC92
	.long	.LC93
	.byte	8
	.byte	0
	.byte	1
	.zero	1
	.long	.LC91
	.long	.LC92
	.long	.LC94
	.byte	8
	.byte	0
	.byte	2
	.zero	1
	.long	.LC91
	.long	.LC92
	.long	.LC95
	.byte	8
	.byte	1
	.byte	0
	.zero	1
	.long	.LC91
	.long	.LC96
	.long	.LC93
	.byte	8
	.byte	1
	.byte	1
	.zero	1
	.long	.LC91
	.long	.LC96
	.long	.LC94
	.byte	8
	.byte	1
	.byte	2
	.zero	1
	.long	.LC91
	.long	.LC96
	.long	.LC97
	.byte	8
	.byte	2
	.byte	0
	.zero	1
	.long	.LC91
	.long	.LC98
	.long	.LC93
	.byte	8
	.byte	2
	.byte	1
	.zero	1
	.long	.LC91
	.long	.LC98
	.long	.LC94
	.byte	8
	.byte	2
	.byte	2
	.zero	1
	.long	.LC91
	.long	.LC98
	.long	.LC97
	.byte	8
	.byte	3
	.byte	0
	.zero	1
	.long	.LC91
	.long	.LC99
	.long	.LC100
	.byte	8
	.byte	3
	.byte	1
	.zero	1
	.long	.LC91
	.long	.LC99
	.long	.LC94
	.byte	8
	.byte	-128
	.byte	0
	.zero	1
	.long	.LC91
	.long	.LC56
	.long	.LC48
	.byte	9
	.byte	0
	.byte	0
	.zero	1
	.long	.LC101
	.long	.LC102
	.long	.LC48
	.byte	9
	.byte	1
	.byte	0
	.zero	1
	.long	.LC101
	.long	.LC103
	.long	.LC48
	.byte	9
	.byte	2
	.byte	0
	.zero	1
	.long	.LC101
	.long	.LC104
	.long	.LC48
	.byte	9
	.byte	-128
	.byte	0
	.zero	1
	.long	.LC101
	.long	.LC56
	.long	.LC48
	.byte	10
	.byte	0
	.byte	0
	.zero	1
	.long	.LC105
	.long	.LC100
	.long	.LC48
	.byte	10
	.byte	-128
	.byte	0
	.zero	1
	.long	.LC105
	.long	.LC56
	.long	.LC48
	.byte	11
	.byte	0
	.byte	0
	.zero	1
	.long	.LC106
	.long	.LC107
	.long	.LC48
	.byte	11
	.byte	1
	.byte	0
	.zero	1
	.long	.LC106
	.long	.LC108
	.long	.LC48
	.byte	11
	.byte	2
	.byte	0
	.zero	1
	.long	.LC106
	.long	.LC109
	.long	.LC48
	.byte	11
	.byte	16
	.byte	0
	.zero	1
	.long	.LC106
	.long	.LC110
	.long	.LC48
	.byte	11
	.byte	32
	.byte	0
	.zero	1
	.long	.LC106
	.long	.LC111
	.long	.LC48
	.byte	11
	.byte	-128
	.byte	0
	.zero	1
	.long	.LC106
	.long	.LC112
	.long	.LC48
	.byte	12
	.byte	0
	.byte	0
	.zero	1
	.long	.LC113
	.long	.LC114
	.long	.LC48
	.byte	12
	.byte	1
	.byte	0
	.zero	1
	.long	.LC113
	.long	.LC115
	.long	.LC48
	.byte	12
	.byte	2
	.byte	0
	.zero	1
	.long	.LC113
	.long	.LC116
	.long	.LC48
	.byte	12
	.byte	3
	.byte	0
	.zero	1
	.long	.LC113
	.long	.LC117
	.long	.LC48
	.byte	12
	.byte	4
	.byte	0
	.zero	1
	.long	.LC113
	.long	.LC118
	.long	.LC48
	.byte	-1
	.byte	0
	.byte	0
	.zero	1
	.long	.LC119
	.long	.LC120
	.long	.LC121
	.globl	PciCommandFlags
	.section	.rodata
.LC122:
	.string	"I/O Access"
.LC123:
	.string	"Memory Access"
.LC124:
	.string	"Bus Mastering"
.LC125:
	.string	"Special Cycles"
.LC126:
	.string	"Memory Write & Invalidate"
.LC127:
	.string	"Palette Snoop"
.LC128:
	.string	"Parity Errors"
.LC129:
	.string	"Wait Cycles"
.LC130:
	.string	"System Errors"
.LC131:
	.string	"Fast Back-To-Back"
.LC132:
	.string	"Reserved 10"
.LC133:
	.string	"Reserved 11"
.LC134:
	.string	"Reserved 12"
.LC135:
	.string	"Reserved 13"
.LC136:
	.string	"Reserved 14"
.LC137:
	.string	"Reserved 15"
	.data
	.align 32
	.type	PciCommandFlags, @object
	.size	PciCommandFlags, 64
PciCommandFlags:
	.long	.LC122
	.long	.LC123
	.long	.LC124
	.long	.LC125
	.long	.LC126
	.long	.LC127
	.long	.LC128
	.long	.LC129
	.long	.LC130
	.long	.LC131
	.long	.LC132
	.long	.LC133
	.long	.LC134
	.long	.LC135
	.long	.LC136
	.long	.LC137
	.globl	PciStatusFlags
	.section	.rodata
.LC138:
	.string	"Reserved 0"
.LC139:
	.string	"Reserved 1"
.LC140:
	.string	"Reserved 2"
.LC141:
	.string	"Reserved 3"
.LC142:
	.string	"Reserved 4"
.LC143:
	.string	"66 MHz Capable"
.LC144:
	.string	"User-Defined Features"
.LC145:
	.string	"Data Parity Reported"
.LC146:
	.string	"Signalled Target Abort"
.LC147:
	.string	"Received Target Abort"
.LC148:
	.string	"Received Master Abort"
.LC149:
	.string	"Signalled System Error"
.LC150:
	.string	"Detected Parity Error"
	.data
	.align 32
	.type	PciStatusFlags, @object
	.size	PciStatusFlags, 64
PciStatusFlags:
	.long	.LC138
	.long	.LC139
	.long	.LC140
	.long	.LC141
	.long	.LC142
	.long	.LC143
	.long	.LC144
	.long	.LC131
	.long	.LC145
	.long	.LC48
	.long	.LC48
	.long	.LC146
	.long	.LC147
	.long	.LC148
	.long	.LC149
	.long	.LC150
	.globl	PciDevSelFlags
	.section	.rodata
.LC151:
	.string	"Fast Devsel Speed"
.LC152:
	.string	"Medium Devsel Speed"
.LC153:
	.string	"Slow Devsel Speed"
.LC154:
	.string	"Reserved 9&10"
	.data
	.align 4
	.type	PciDevSelFlags, @object
	.size	PciDevSelFlags, 16
PciDevSelFlags:
	.long	.LC151
	.long	.LC152
	.long	.LC153
	.long	.LC154
	.local	devices
	.comm	devices,229376,32
	.local	AHCI_Devices
	.comm	AHCI_Devices,896,32
	.local	TotalAHCIDevices
	.comm	TotalAHCIDevices,4,4
	.globl	test
	.align 4
	.type	test, @object
	.size	test, 20
test:
	.string	"6317065029"
	.zero	9
	.globl	AHCI_BASE
	.section	.bss
	.align 4
	.type	AHCI_BASE, @object
	.size	AHCI_BASE, 4
AHCI_BASE:
	.zero	4
	.comm	Disk_dev,4,4
	.comm	Disk_dev_start,4,4
	.comm	ahci,4,4
	.comm	ahci_start,4,4
	.globl	sata
	.align 4
	.type	sata, @object
	.size	sata, 4
sata:
	.zero	4
	.comm	fs_buf,1024,32
	.comm	clear_buf512,4,4
	.comm	start_handle,4,4
	.comm	end,4,4
	.comm	current,4,4
	.comm	curr_dir,108,32
	.comm	temp_curr_dir,108,32
	.comm	root_dir,108,32
	.comm	current_,146,32
	.comm	curr_port,4,4
	.comm	curr_disk,4,4
	.comm	curr_ahci,4,4
	.comm	sectors,4,4
	.comm	bytemap_off,4,4
	.comm	bytemap_end,4,4
	.comm	curr_file,162,32
	.comm	fsbuf,4,4
	.globl	header_data
	.data
	.align 4
	.type	header_data, @object
	.size	header_data, 4
header_data:
	.long	976
	.comm	MotherSpace,4,4
	.comm	KitList,4,4
	.comm	Kernel_task,4,4
	.comm	Idle_task,4,4
	.comm	Shell_task,4,4
	.comm	Shell_Istream_task,4,4
	.comm	Shell_Ostream_task,4,4
	.comm	TASK_LOCK_KILLLocked,4,4
	.comm	TASK_LOCK_ATDLocked,4,4
	.comm	kernel_proc,4,4
	.comm	Shell_proc,4,4
	.comm	SAS_proc,4,4
	.globl	processes
	.section	.bss
	.align 4
	.type	processes, @object
	.size	processes, 4
processes:
	.zero	4
	.globl	TIME_MASK
	.section	.rodata
	.align 4
	.type	TIME_MASK, @object
	.size	TIME_MASK, 4
TIME_MASK:
	.long	1044480
	.globl	total_CPU_Cores
	.data
	.align 4
	.type	total_CPU_Cores, @object
	.size	total_CPU_Cores, 4
total_CPU_Cores:
	.long	1
	.globl	aad
	.align 4
	.type	aad, @object
	.size	aad, 4
aad:
	.long	10
	.globl	adeg
	.section	.bss
	.align 4
	.type	adeg, @object
	.size	adeg, 4
adeg:
	.zero	4
	.globl	cli_already
	.align 4
	.type	cli_already, @object
	.size	cli_already, 4
cli_already:
	.zero	4
	.globl	multitasking_ON
	.align 4
	.type	multitasking_ON, @object
	.size	multitasking_ON, 4
multitasking_ON:
	.zero	4
	.globl	sas_bottom_task
	.data
	.align 4
	.type	sas_bottom_task, @object
	.size	sas_bottom_task, 4
sas_bottom_task:
	.long	1
	.comm	Shell_Commands_list,4,4
	.globl	up_input
	.section	.bss
	.align 4
	.type	up_input, @object
	.size	up_input, 4
up_input:
	.zero	4
	.globl	Main_CSI_struct
	.data
	.align 4
	.type	Main_CSI_struct, @object
	.size	Main_CSI_struct, 4
Main_CSI_struct:
	.long	52428800
	.comm	CSI_entries_ptr,4,4
	.comm	tot_entries,4,4
	.globl	CSI_mem_start
	.align 4
	.type	CSI_mem_start, @object
	.size	CSI_mem_start, 4
CSI_mem_start:
	.long	52428800
	.globl	key_pressed
	.section	.bss
	.type	key_pressed, @object
	.size	key_pressed, 1
key_pressed:
	.zero	1
	.globl	enter_pressed
	.align 4
	.type	enter_pressed, @object
	.size	enter_pressed, 4
enter_pressed:
	.zero	4
	.comm	Istream_ptr,4,4
	.comm	Current_buf,4,4
	.globl	kb_buff
	.align 4
	.type	kb_buff, @object
	.size	kb_buff, 4
kb_buff:
	.zero	4
	.comm	shell_buf,4,4
	.globl	shell_in
	.align 4
	.type	shell_in, @object
	.size	shell_in, 4
shell_in:
	.zero	4
	.globl	Current_strlen
	.align 4
	.type	Current_strlen, @object
	.size	Current_strlen, 4
Current_strlen:
	.zero	4
	.globl	shell_awake
	.data
	.align 4
	.type	shell_awake, @object
	.size	shell_awake, 4
shell_awake:
	.long	1
	.globl	shell_sleeping
	.section	.bss
	.align 4
	.type	shell_sleeping, @object
	.size	shell_sleeping, 4
shell_sleeping:
	.zero	4
	.comm	Shell_Istream,4,4
	.comm	Input_stream,4,4
	.comm	Istream_end,4,4
	.comm	shell_cmdTask,4,4
	.comm	tmpIstream,4,4
	.comm	shell_cmdFunc,4,4
	.globl	shlock
	.align 4
	.type	shlock, @object
	.size	shlock, 4
shlock:
	.zero	4
	.comm	SHELL_CMDTASKLocked,4,4
	.globl	LAPIC_EOI_send
	.data
	.align 4
	.type	LAPIC_EOI_send, @object
	.size	LAPIC_EOI_send, 4
LAPIC_EOI_send:
	.long	-18874192
	.section	.rodata
.LC155:
	.string	"\nFault1"
	.text
	.align 16
	.globl	divByZero_handler
	.type	divByZero_handler, @function
divByZero_handler:
.LFB115:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC155
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  26 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L743:
	jmp	.L743
	.cfi_endproc
.LFE115:
	.size	divByZero_handler, .-divByZero_handler
	.section	.rodata
.LC156:
	.string	"\nFault2 %x"
	.text
	.align 16
	.globl	debug_handler
	.type	debug_handler, @function
debug_handler:
.LFB116:
	.cfi_startproc
	subl	$20, %esp
	.cfi_def_cfa_offset 24
	movzbl	-18874333, %eax
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC156
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  39 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/  42 "Source/Interrupts/int_handlers.c" 1
	iret
/  0 "" 2
/NO_APP
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE116:
	.size	debug_handler, .-debug_handler
	.section	.rodata
.LC157:
	.string	"\nFault3"
	.text
	.align 16
	.globl	NMI_handler
	.type	NMI_handler, @function
NMI_handler:
.LFB117:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC157
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  49 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L748:
	jmp	.L748
	.cfi_endproc
.LFE117:
	.size	NMI_handler, .-NMI_handler
	.section	.rodata
.LC158:
	.string	"\nFault4"
	.text
	.align 16
	.globl	breakpoint_handler
	.type	breakpoint_handler, @function
breakpoint_handler:
.LFB118:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC158
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  59 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L751:
	jmp	.L751
	.cfi_endproc
.LFE118:
	.size	breakpoint_handler, .-breakpoint_handler
	.section	.rodata
.LC159:
	.string	"\nFault5"
	.text
	.align 16
	.globl	overflow_handler
	.type	overflow_handler, @function
overflow_handler:
.LFB119:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC159
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  69 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L754:
	jmp	.L754
	.cfi_endproc
.LFE119:
	.size	overflow_handler, .-overflow_handler
	.section	.rodata
.LC160:
	.string	"\nFault6 %x"
	.text
	.align 16
	.globl	outOfBounds_handler
	.type	outOfBounds_handler, @function
outOfBounds_handler:
.LFB120:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	Get_Scheduler
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	(%eax)
	.cfi_def_cfa_offset 28
	pushl	$.LC160
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  79 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L757:
	jmp	.L757
	.cfi_endproc
.LFE120:
	.size	outOfBounds_handler, .-outOfBounds_handler
	.section	.rodata
.LC161:
	.string	"\nFault7 %x"
	.text
	.align 16
	.globl	invalidInstr_handler
	.type	invalidInstr_handler, @function
invalidInstr_handler:
.LFB121:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	Get_Scheduler
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	(%eax)
	.cfi_def_cfa_offset 28
	pushl	$.LC161
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  89 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L760:
	jmp	.L760
	.cfi_endproc
.LFE121:
	.size	invalidInstr_handler, .-invalidInstr_handler
	.section	.rodata
.LC162:
	.string	"\nFault8"
	.text
	.align 16
	.globl	noCoprocessor_handler
	.type	noCoprocessor_handler, @function
noCoprocessor_handler:
.LFB122:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC162
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  99 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L763:
	jmp	.L763
	.cfi_endproc
.LFE122:
	.size	noCoprocessor_handler, .-noCoprocessor_handler
	.section	.rodata
.LC163:
	.string	"\nFault9"
	.text
	.align 16
	.globl	doubleFault_handler
	.type	doubleFault_handler, @function
doubleFault_handler:
.LFB123:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC163
	.cfi_def_cfa_offset 32
	call	printf
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L766:
	jmp	.L766
	.cfi_endproc
.LFE123:
	.size	doubleFault_handler, .-doubleFault_handler
	.section	.rodata
.LC164:
	.string	"\nFault10"
	.text
	.align 16
	.globl	coprocessor_handler
	.type	coprocessor_handler, @function
coprocessor_handler:
.LFB124:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC164
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  119 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L769:
	jmp	.L769
	.cfi_endproc
.LFE124:
	.size	coprocessor_handler, .-coprocessor_handler
	.section	.rodata
.LC165:
	.string	"\nFault11"
	.text
	.align 16
	.globl	badTSS_handler
	.type	badTSS_handler, @function
badTSS_handler:
.LFB125:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC165
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  129 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L772:
	jmp	.L772
	.cfi_endproc
.LFE125:
	.size	badTSS_handler, .-badTSS_handler
	.section	.rodata
.LC166:
	.string	"\nFault12"
	.text
	.align 16
	.globl	segmentNotPresent_handler
	.type	segmentNotPresent_handler, @function
segmentNotPresent_handler:
.LFB126:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC166
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  139 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L775:
	jmp	.L775
	.cfi_endproc
.LFE126:
	.size	segmentNotPresent_handler, .-segmentNotPresent_handler
	.section	.rodata
.LC167:
	.string	"\nFault13"
	.text
	.align 16
	.globl	stackFault_handler
	.type	stackFault_handler, @function
stackFault_handler:
.LFB127:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC167
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  149 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L778:
	jmp	.L778
	.cfi_endproc
.LFE127:
	.size	stackFault_handler, .-stackFault_handler
	.globl	avc
	.section	.bss
	.align 4
	.type	avc, @object
	.size	avc, 4
avc:
	.zero	4
	.text
	.align 16
	.globl	generalProtectionFault_handler
	.type	generalProtectionFault_handler, @function
generalProtectionFault_handler:
.LFB128:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
/APP
/  168 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
.L781:
	jmp	.L781
	.cfi_endproc
.LFE128:
	.size	generalProtectionFault_handler, .-generalProtectionFault_handler
	.section	.rodata
.LC168:
	.string	"\nPage Fault"
	.text
	.align 16
	.globl	pageFault_caller
	.type	pageFault_caller, @function
pageFault_caller:
.LFB129:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC168
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  196 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE129:
	.size	pageFault_caller, .-pageFault_caller
	.section	.rodata
.LC169:
	.string	"\nFaul15t"
	.text
	.align 16
	.globl	unknownInterrupt_handler
	.type	unknownInterrupt_handler, @function
unknownInterrupt_handler:
.LFB130:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC169
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  236 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L785:
	jmp	.L785
	.cfi_endproc
.LFE130:
	.size	unknownInterrupt_handler, .-unknownInterrupt_handler
	.section	.rodata
.LC170:
	.string	"\nFault16"
	.text
	.align 16
	.globl	coprocessorFault_handler
	.type	coprocessorFault_handler, @function
coprocessorFault_handler:
.LFB131:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC170
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  246 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L788:
	jmp	.L788
	.cfi_endproc
.LFE131:
	.size	coprocessorFault_handler, .-coprocessorFault_handler
	.section	.rodata
.LC171:
	.string	"\nFault17"
	.text
	.align 16
	.globl	alignmentCheck_handler
	.type	alignmentCheck_handler, @function
alignmentCheck_handler:
.LFB132:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC171
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  256 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L791:
	jmp	.L791
	.cfi_endproc
.LFE132:
	.size	alignmentCheck_handler, .-alignmentCheck_handler
	.section	.rodata
.LC172:
	.string	"\nFault18"
	.text
	.align 16
	.globl	machineCheck_handler
	.type	machineCheck_handler, @function
machineCheck_handler:
.LFB133:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC172
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  266 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L794:
	jmp	.L794
	.cfi_endproc
.LFE133:
	.size	machineCheck_handler, .-machineCheck_handler
	.section	.rodata
.LC173:
	.string	"\nFault19"
	.text
	.align 16
	.globl	reserved_handler
	.type	reserved_handler, @function
reserved_handler:
.LFB134:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC173
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  276 "Source/Interrupts/int_handlers.c" 1
	hlt
/  0 "" 2
/NO_APP
	call	Shell_Double_buffer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L797:
	jmp	.L797
	.cfi_endproc
.LFE134:
	.size	reserved_handler, .-reserved_handler
	.align 16
	.globl	PIT_handler
	.type	PIT_handler, @function
PIT_handler:
.LFB135:
	.cfi_startproc
/APP
/  287 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/  292 "Source/Interrupts/int_handlers.c" 1
	iret
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE135:
	.size	PIT_handler, .-PIT_handler
	.globl	ag
	.data
	.align 4
	.type	ag, @object
	.size	ag, 4
ag:
	.long	1
	.globl	ab
	.section	.bss
	.align 4
	.type	ab, @object
	.size	ab, 4
ab:
	.zero	4
	.text
	.align 16
	.globl	keyboardInterrupt_handler
	.type	keyboardInterrupt_handler, @function
keyboardInterrupt_handler:
.LFB136:
	.cfi_startproc
/APP
/  45 "Library/sys.h" 1
	inb $96, %al
/  0 "" 2
/NO_APP
	testb	%al, %al
	movzbl	%al, %edx
	js	.L849
	movl	scancodes, %eax
	movl	(%eax,%edx,4), %eax
	testl	%eax, %eax
	je	.L850
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	keyboard_scancodes
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	cmpl	$13, %eax
	je	.L851
	cmpl	$8, %eax
	je	.L852
	movl	Istream_ptr, %edx
	movb	%al, (%edx)
	movl	Istream_ptr, %ecx
	leal	1(%ecx), %edx
	movl	kb_buff, %ecx
	movl	%edx, Istream_ptr
	addl	$1, %ecx
	movl	%ecx, kb_buff
	movl	Istream_end, %ecx
	cmpl	%ecx, %edx
	je	.L853
.L825:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movsbl	%al, %eax
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	_putchar
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L800:
	addl	$12, %esp
	.cfi_def_cfa_offset 4
.L845:
	rep ret
	.align 16
.L850:
	movl	Special_key_codes(,%edx,4), %eax
	cmpl	$16385, %eax
	je	.L805
	jle	.L854
	cmpl	$16390, %eax
	je	.L809
	cmpl	$16400, %eax
	je	.L810
	cmpl	$16386, %eax
	jne	.L845
	movl	$1, _shift
	ret
	.align 16
.L849:
	movl	Special_key_codes-512(,%edx,4), %eax
	cmpl	$16386, %eax
	je	.L803
	cmpl	$16390, %eax
	jne	.L845
.L803:
	movl	$0, _shift
	ret
	.align 16
.L854:
	cmpl	$4352, %eax
	je	.L807
	cmpl	$12303, %eax
	jne	.L845
	movl	_numlock, %eax
	testl	%eax, %eax
	sete	%al
	movzbl	%al, %edx
	orl	$4, %eax
	movl	%edx, _numlock
	movl	_capslock, %edx
	testl	%edx, %edx
	movl	$0, %edx
	cmovne	%eax, %edx
	.align 16
.L817:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L817
	movl	$-19, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
	.align 16
/NO_APP
.L818:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L818
.L847:
	movl	%edx, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
/NO_APP
	ret
	.align 16
.L853:
	.cfi_def_cfa_offset 16
	movl	Input_stream, %edx
	movl	%edx, Istream_ptr
	jmp	.L825
	.align 16
.L852:
	movl	kb_buff, %eax
	testl	%eax, %eax
	je	.L800
	movl	Istream_ptr, %eax
	leal	-1(%eax), %edx
	movl	%edx, Istream_ptr
	movb	$0, -1(%eax)
	movl	kb_buff, %eax
	subl	$1, %eax
	movl	%eax, kb_buff
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	jmp	backspace
	.align 16
.L851:
	.cfi_restore_state
	movl	$1, enter_pressed
	jmp	.L800
	.align 16
.L807:
	.cfi_def_cfa_offset 4
	movl	$1, up_input
	ret
	.align 16
.L810:
	movl	_scrolllock, %edx
	xorl	%eax, %eax
	movl	_capslock, %ecx
	testl	%edx, %edx
	movl	_numlock, %edx
	sete	%al
	movl	%eax, _scrolllock
	movl	%edx, %eax
	andl	$4, %edx
	orl	$4, %eax
	testl	%ecx, %ecx
	cmovne	%eax, %edx
	.align 16
.L821:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L821
	movl	$-19, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
	.align 16
/NO_APP
.L822:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L822
	jmp	.L847
	.align 16
.L809:
	movl	$2, _shift
	ret
	.align 16
.L805:
	movl	_capslock, %eax
	xorl	%edx, %edx
	testl	%eax, %eax
	sete	%dl
	movl	%edx, _capslock
	movl	_numlock, %edx
	movl	%edx, %ecx
	andl	$4, %edx
	orl	$4, %ecx
	testl	%eax, %eax
	cmove	%ecx, %edx
	.align 16
.L814:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L814
	movl	$-19, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
	.align 16
/NO_APP
.L815:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L815
	jmp	.L847
	.cfi_endproc
.LFE136:
	.size	keyboardInterrupt_handler, .-keyboardInterrupt_handler
	.section	.rodata
.LC174:
	.string	"\nInterrupt 2"
	.text
	.align 16
	.globl	cascade_handler
	.type	cascade_handler, @function
cascade_handler:
.LFB137:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  426 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC174
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L856:
	jmp	.L856
	.cfi_endproc
.LFE137:
	.size	cascade_handler, .-cascade_handler
	.section	.rodata
.LC175:
	.string	"\nInterrupt 3"
	.text
	.align 16
	.globl	COM2_handler
	.type	COM2_handler, @function
COM2_handler:
.LFB138:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  435 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC175
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L859:
	jmp	.L859
	.cfi_endproc
.LFE138:
	.size	COM2_handler, .-COM2_handler
	.section	.rodata
.LC176:
	.string	"\nInterrupt 4"
	.text
	.align 16
	.globl	COM1_handler
	.type	COM1_handler, @function
COM1_handler:
.LFB139:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  444 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC176
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L862:
	jmp	.L862
	.cfi_endproc
.LFE139:
	.size	COM1_handler, .-COM1_handler
	.section	.rodata
.LC177:
	.string	"\nInterrupt 5"
	.text
	.align 16
	.globl	LPT2_handler
	.type	LPT2_handler, @function
LPT2_handler:
.LFB140:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  453 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC177
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L865:
	jmp	.L865
	.cfi_endproc
.LFE140:
	.size	LPT2_handler, .-LPT2_handler
	.section	.rodata
.LC178:
	.string	"\nInterrupt 6"
	.text
	.align 16
	.globl	floppyDisk_handler
	.type	floppyDisk_handler, @function
floppyDisk_handler:
.LFB141:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  462 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC178
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L868:
	jmp	.L868
	.cfi_endproc
.LFE141:
	.size	floppyDisk_handler, .-floppyDisk_handler
	.section	.rodata
.LC179:
	.string	"\nInterrupt 7"
	.text
	.align 16
	.globl	LPT1_handler
	.type	LPT1_handler, @function
LPT1_handler:
.LFB142:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  471 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC179
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L871:
	jmp	.L871
	.cfi_endproc
.LFE142:
	.size	LPT1_handler, .-LPT1_handler
	.section	.rodata
.LC180:
	.string	"\nInterrupt 8"
	.text
	.align 16
	.globl	RTC_handler
	.type	RTC_handler, @function
RTC_handler:
.LFB143:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  480 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC180
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L874:
	jmp	.L874
	.cfi_endproc
.LFE143:
	.size	RTC_handler, .-RTC_handler
	.section	.rodata
.LC181:
	.string	"\nInterrupt 9"
	.text
	.align 16
	.globl	periph1_handler
	.type	periph1_handler, @function
periph1_handler:
.LFB144:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  489 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC181
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L877:
	jmp	.L877
	.cfi_endproc
.LFE144:
	.size	periph1_handler, .-periph1_handler
	.section	.rodata
.LC182:
	.string	"\nInterrupt 10"
	.text
	.align 16
	.globl	periph2_handler
	.type	periph2_handler, @function
periph2_handler:
.LFB145:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  498 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC182
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L880:
	jmp	.L880
	.cfi_endproc
.LFE145:
	.size	periph2_handler, .-periph2_handler
	.section	.rodata
.LC183:
	.string	"\nInterrupt 11"
	.text
	.align 16
	.globl	periph3_handler
	.type	periph3_handler, @function
periph3_handler:
.LFB146:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  507 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC183
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L883:
	jmp	.L883
	.cfi_endproc
.LFE146:
	.size	periph3_handler, .-periph3_handler
	.align 16
	.globl	mouse_handler
	.type	mouse_handler, @function
mouse_handler:
.LFB147:
	.cfi_startproc
	movl	cycle, %eax
	cmpl	$2, %eax
	leal	mouse_bytes(%eax), %edx
	ja	.L890
	.align 16
.L893:
/APP
/  45 "Library/sys.h" 1
	inb $96, %al
/  0 "" 2
/NO_APP
	movb	%al, (%edx)
	addl	$1, %edx
	cmpl	$mouse_bytes+3, %edx
	jne	.L893
.L890:
	movzbl	mouse_bytes, %eax
	movl	$0, cycle
	testb	%al, %al
	js	.L896
	testb	$64, %al
	jne	.L885
	testb	$32, %al
	jne	.L891
	movl	mousedeltay, %edx
	orl	$-256, %edx
	movl	%edx, mousedeltay
.L891:
	testb	$16, %al
	jne	.L892
	movl	mousedeltax, %eax
	orl	$-256, %eax
	movl	%eax, mousedeltax
.L892:
	movsbl	mouse_bytes+1, %eax
	movl	%eax, mousedeltax
	movsbl	mouse_bytes+2, %eax
	movl	%eax, mousedeltay
.L885:
	rep ret
	.align 16
.L896:
	rep ret
	.cfi_endproc
.LFE147:
	.size	mouse_handler, .-mouse_handler
	.section	.rodata
.LC184:
	.string	"\nInterrupt 13"
	.text
	.align 16
	.globl	FPU_handler
	.type	FPU_handler, @function
FPU_handler:
.LFB148:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  558 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC184
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L898:
	jmp	.L898
	.cfi_endproc
.LFE148:
	.size	FPU_handler, .-FPU_handler
	.section	.rodata
.LC185:
	.string	"\nInterrupt 14"
	.text
	.align 16
	.globl	primaryHDD_handler
	.type	primaryHDD_handler, @function
primaryHDD_handler:
.LFB149:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  567 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC185
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L901:
	jmp	.L901
	.cfi_endproc
.LFE149:
	.size	primaryHDD_handler, .-primaryHDD_handler
	.section	.rodata
.LC186:
	.string	"\nInterrupt 15"
	.text
	.align 16
	.globl	secondaryHDD_handler
	.type	secondaryHDD_handler, @function
secondaryHDD_handler:
.LFB150:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  576 "Source/Interrupts/int_handlers.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC186
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L904:
	jmp	.L904
	.cfi_endproc
.LFE150:
	.size	secondaryHDD_handler, .-secondaryHDD_handler
	.comm	Lapic,4,4
	.comm	mpfp,4,4
	.comm	mpcth,4,4
	.comm	Processor_Entries,4,4
	.align 16
	.globl	cpuHasMSR
	.type	cpuHasMSR, @function
cpuHasMSR:
.LFB151:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	$1, %eax
/APP
/  19 "Source/arch/x86/cpu/cpu.c" 1
	cpuid
/  0 "" 2
/NO_APP
	movl	%edx, %eax
	shrl	$5, %eax
	andl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE151:
	.size	cpuHasMSR, .-cpuHasMSR
	.align 16
	.globl	cpuGetMSR
	.type	cpuGetMSR, @function
cpuGetMSR:
.LFB152:
	.cfi_startproc
	movl	4(%esp), %ecx
/APP
/  25 "Source/arch/x86/cpu/cpu.c" 1
	rdmsr
/  0 "" 2
/NO_APP
	movl	8(%esp), %ecx
	movl	%eax, (%ecx)
	movl	12(%esp), %eax
	movl	%edx, (%eax)
	ret
	.cfi_endproc
.LFE152:
	.size	cpuGetMSR, .-cpuGetMSR
	.align 16
	.globl	cpuSetMSR
	.type	cpuSetMSR, @function
cpuSetMSR:
.LFB153:
	.cfi_startproc
	movl	12(%esp), %edx
	movl	8(%esp), %eax
	movl	4(%esp), %ecx
/APP
/  30 "Source/arch/x86/cpu/cpu.c" 1
	wrmsr
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE153:
	.size	cpuSetMSR, .-cpuSetMSR
	.globl	AP_stacks
	.data
	.align 4
	.type	AP_stacks, @object
	.size	AP_stacks, 4
AP_stacks:
	.long	8192
	.text
	.align 16
	.globl	BasicCPU_Init
	.type	BasicCPU_Init, @function
BasicCPU_Init:
.LFB156:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	movl	$17, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $32
/  0 "" 2
/  37 "Library/sys.h" 1
	outb %al, $160
/  0 "" 2
/NO_APP
	movl	$32, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/NO_APP
	movl	$40, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $161
/  0 "" 2
/NO_APP
	movl	$4, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/NO_APP
	movl	$2, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $161
/  0 "" 2
/NO_APP
	movl	$1, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/  37 "Library/sys.h" 1
	outb %al, $161
/  0 "" 2
/NO_APP
	xorl	%eax, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/  37 "Library/sys.h" 1
	outb %al, $161
/  0 "" 2
/NO_APP
	movl	$-1, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/  37 "Library/sys.h" 1
	outb %al, $161
/  0 "" 2
/NO_APP
	movl	$27, %ecx
/APP
/  25 "Source/arch/x86/cpu/cpu.c" 1
	rdmsr
/  0 "" 2
/NO_APP
	andl	$-4096, %eax
	pushl	%eax
	.cfi_def_cfa_offset 32
	movl	%eax, APIC_LOCAL_BASE
	call	BSP_init_LAPIC
	call	MPtables_parse
	call	MADTapic_parse
	movl	Lapic, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	32(%eax), %eax
	testl	%eax, %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, BSP_id
/APP
/  57 "Source/arch/x86/cpu/cpu.c" 1
	sti
/  0 "" 2
/NO_APP
	movl	total_CPU_Cores, %edx
	testl	%edx, %edx
	jne	.L911
	movl	$1, total_CPU_Cores
.L911:
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	jmp	BootAPs
	.cfi_endproc
.LFE156:
	.size	BasicCPU_Init, .-BasicCPU_Init
	.section	.rodata
.LC187:
	.string	"\nReatempting INIT SIPI"
	.align 4
.LC188:
	.string	"\n\t\t%gThe AP #%x has been Booted Successfully %x Error %x Vector %x %x %g"
	.text
	.align 16
	.globl	BootAPs
	.type	BootAPs, @function
BootAPs:
.LFB157:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$56, %esp
	.cfi_def_cfa_offset 64
	pushl	$4096
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$126976
	.cfi_def_cfa_offset 76
	call	memset
	addl	$12, %esp
	.cfi_def_cfa_offset 64
	movl	$APIC_Error_vector_end, %edx
	movl	$APIC_Error_vector, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	pushl	%eax
	.cfi_def_cfa_offset 68
	pushl	$APIC_Error_vector
	.cfi_def_cfa_offset 72
	pushl	$126976
	.cfi_def_cfa_offset 76
	call	memcpy
	addl	$12, %esp
	.cfi_def_cfa_offset 64
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	$APIC_Error_vector_end
	.cfi_def_cfa_offset 76
	pushl	$APIC_Error_vector
	.cfi_def_cfa_offset 80
	pushl	$2
	.cfi_def_cfa_offset 84
	pushl	$1280
	.cfi_def_cfa_offset 88
	pushl	$2
	.cfi_def_cfa_offset 92
	pushl	$26233
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	$15, %eax
	movb	$10, (%eax)
	movl	$0, 36(%esp)
	movl	$AP_startup_Code_end, %edx
	movl	$AP_startup_Code, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, AP_startup_Code_sz
	movl	AP_startup_Code_sz, %eax
	andl	$-4, %eax
	addl	$4, %eax
	movl	%eax, AP_startup_Code_sz
	movl	$pmode_AP_code_end, %edx
	movl	$pmode_AP_code, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, pmode_code_size
	movl	pmode_code_size, %eax
	andl	$-4, %eax
	addl	$4, %eax
	movl	%eax, pmode_code_size
	movl	$1280, 32(%esp)
	movl	32(%esp), %eax
	movl	$0, (%eax)
	movl	$1284, 28(%esp)
	movl	28(%esp), %eax
	movl	$0, (%eax)
	movl	$1, 36(%esp)
	movl	$4096, 24(%esp)
	movl	AP_stacks, %eax
	movl	%eax, 44(%esp)
	movl	$0, 40(%esp)
	jmp	.L914
.L918:
	movl	BSP_id, %eax
	cmpl	40(%esp), %eax
	je	.L919
	movl	44(%esp), %eax
	subl	$4096, %eax
	movl	%eax, 24(%esp)
	movl	24(%esp), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	$4096
	.cfi_def_cfa_offset 72
	pushl	$0
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	memset
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	AP_startup_Code_sz, %edx
	movl	24(%esp), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	%edx
	.cfi_def_cfa_offset 72
	pushl	$AP_startup_Code
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	memcpy
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	AP_startup_Code_sz, %edx
	movl	24(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	24(%esp), %eax
	movl	44(%esp), %edx
	addl	$4092, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%ecx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$2
	.cfi_def_cfa_offset 84
	pushl	%edx
	.cfi_def_cfa_offset 88
	pushl	$2
	.cfi_def_cfa_offset 92
	pushl	$17028
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	AP_startup_Code_sz, %edx
	movl	24(%esp), %eax
	addl	%eax, %edx
	movl	pmode_code_size, %eax
	addl	%edx, %eax
	addl	$16, %eax
	movl	%eax, 20(%esp)
	movl	AP_startup_Code_sz, %edx
	movl	24(%esp), %eax
	addl	%eax, %edx
	movl	pmode_code_size, %eax
	addl	%edx, %eax
	addl	$24, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	28(%esp)
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	AP_gdt_Setup
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	AP_startup_Code_sz, %edx
	movl	24(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	24(%esp), %edx
	movl	20(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%ecx
	.cfi_def_cfa_offset 76
	pushl	%edx
	.cfi_def_cfa_offset 80
	pushl	$2
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$2
	.cfi_def_cfa_offset 92
	pushl	$12864
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	AP_startup_Code_sz, %edx
	movl	24(%esp), %eax
	addl	%eax, %edx
	movl	pmode_code_size, %eax
	addl	%edx, %eax
	addl	$64, %eax
	movl	%eax, 16(%esp)
	movl	AP_startup_Code_sz, %edx
	movl	24(%esp), %eax
	addl	%eax, %edx
	movl	pmode_code_size, %eax
	addl	%edx, %eax
	addl	$72, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	24(%esp)
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	AP_idt_Setup
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	AP_startup_Code_sz, %edx
	movl	24(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	24(%esp), %edx
	movl	16(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%ecx
	.cfi_def_cfa_offset 76
	pushl	%edx
	.cfi_def_cfa_offset 80
	pushl	$2
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$2
	.cfi_def_cfa_offset 92
	pushl	$12880
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	AP_startup_Code_sz, %edx
	movl	24(%esp), %eax
	addl	%edx, %eax
	addl	$16, %eax
	movl	%eax, 12(%esp)
	movl	pmode_code_size, %edx
	movl	12(%esp), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	%edx
	.cfi_def_cfa_offset 72
	pushl	$pmode_AP_code
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	memcpy
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	AP_startup_Code_sz, %edx
	movl	24(%esp), %eax
	addl	%edx, %eax
	addl	$8, %eax
	movl	$0, (%eax)
	movl	pmode_code_size, %edx
	movl	12(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ebx
	movl	12(%esp), %eax
	movl	AP_startup_Code_sz, %ecx
	movl	24(%esp), %edx
	addl	%ecx, %edx
	addl	$8, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%ebx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$2
	.cfi_def_cfa_offset 84
	pushl	%edx
	.cfi_def_cfa_offset 88
	pushl	$2
	.cfi_def_cfa_offset 92
	pushl	$21913
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	pmode_code_size, %edx
	movl	12(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	movl	12(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$2
	.cfi_def_cfa_offset 84
	pushl	32(%esp)
	.cfi_def_cfa_offset 88
	pushl	$2
	.cfi_def_cfa_offset 92
	pushl	$18777
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	24(%esp), %eax
	shrl	$12, %eax
	movl	%eax, 36(%esp)
	movl	40(%esp), %eax
	sall	$24, %eax
	movl	%eax, %edx
	movl	APIC_LOCAL_BASE, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	%edx
	.cfi_def_cfa_offset 72
	pushl	$784
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	APIC_LOCAL_BASE, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	$17664
	.cfi_def_cfa_offset 72
	pushl	$768
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	call	delay1
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	40(%esp), %eax
	sall	$24, %eax
	movl	%eax, %edx
	movl	APIC_LOCAL_BASE, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	%edx
	.cfi_def_cfa_offset 72
	pushl	$784
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	36(%esp), %eax
	orb	$70, %ah
	movl	%eax, %edx
	movl	APIC_LOCAL_BASE, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	%edx
	.cfi_def_cfa_offset 72
	pushl	$768
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$4
	.cfi_def_cfa_offset 80
	call	delay1
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	32(%esp), %eax
	movl	(%eax), %eax
	addl	%eax, %eax
	cmpl	44(%esp), %eax
	jne	.L917
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC187
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	36(%esp), %eax
	orb	$70, %ah
	movl	%eax, %edx
	movl	APIC_LOCAL_BASE, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	%edx
	.cfi_def_cfa_offset 72
	pushl	$768
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L917:
	movl	AP_startup_Code_sz, %edx
	movl	24(%esp), %eax
	addl	%edx, %eax
	leal	8(%eax), %ebx
	movl	$1342181376, %eax
	movl	(%eax), %edx
	movl	Lapic, %eax
	movl	768(%eax), %eax
	andl	$4096, %eax
	movl	%eax, %ecx
	movl	32(%esp), %eax
	movl	(%eax), %eax
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%ebx
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%ecx
	.cfi_def_cfa_offset 80
	pushl	%eax
	.cfi_def_cfa_offset 84
	pushl	60(%esp)
	.cfi_def_cfa_offset 88
	pushl	$10
	.cfi_def_cfa_offset 92
	pushl	$.LC188
	.cfi_def_cfa_offset 96
	call	printf
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	jmp	.L916
.L919:
	nop
.L916:
	addl	$8192, 44(%esp)
	addl	$1, 40(%esp)
.L914:
	movl	total_CPU_Cores, %eax
	subl	$1, %eax
	cmpl	40(%esp), %eax
	ja	.L918
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$4
	.cfi_def_cfa_offset 80
	call	delay1
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	nop
	addl	$56, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE157:
	.size	BootAPs, .-BootAPs
	.section	.rodata
.LC189:
	.string	"Sun"
	.text
	.align 16
	.globl	__cmos_get_short_day
	.type	__cmos_get_short_day, @function
__cmos_get_short_day:
.LFB158:
	.cfi_startproc
	movzbl	4(%esp), %eax
	leal	-2(%eax), %edx
	movl	$.LC189, %eax
	cmpb	$5, %dl
	ja	.L920
	movzbl	%dl, %edx
	movl	CSWTCH.1398(,%edx,4), %eax
.L920:
	rep ret
	.cfi_endproc
.LFE158:
	.size	__cmos_get_short_day, .-__cmos_get_short_day
	.section	.rodata
.LC190:
	.string	"Sunday"
	.text
	.align 16
	.globl	__cmos_get_day
	.type	__cmos_get_day, @function
__cmos_get_day:
.LFB159:
	.cfi_startproc
	movzbl	4(%esp), %eax
	leal	-2(%eax), %edx
	movl	$.LC190, %eax
	cmpb	$5, %dl
	ja	.L923
	movzbl	%dl, %edx
	movl	CSWTCH.1400(,%edx,4), %eax
.L923:
	rep ret
	.cfi_endproc
.LFE159:
	.size	__cmos_get_day, .-__cmos_get_day
	.section	.rodata
.LC191:
	.string	"Jan"
	.text
	.align 16
	.globl	__cmos_get_short_month
	.type	__cmos_get_short_month, @function
__cmos_get_short_month:
.LFB160:
	.cfi_startproc
	movzbl	4(%esp), %eax
	leal	-2(%eax), %edx
	movl	$.LC191, %eax
	cmpb	$10, %dl
	ja	.L926
	movzbl	%dl, %edx
	movl	CSWTCH.1402(,%edx,4), %eax
.L926:
	rep ret
	.cfi_endproc
.LFE160:
	.size	__cmos_get_short_month, .-__cmos_get_short_month
	.section	.rodata
.LC192:
	.string	"January"
	.text
	.align 16
	.globl	__cmos_get_month
	.type	__cmos_get_month, @function
__cmos_get_month:
.LFB161:
	.cfi_startproc
	movzbl	4(%esp), %eax
	leal	-2(%eax), %edx
	movl	$.LC192, %eax
	cmpb	$10, %dl
	ja	.L929
	movzbl	%dl, %edx
	movl	CSWTCH.1404(,%edx,4), %eax
.L929:
	rep ret
	.cfi_endproc
.LFE161:
	.size	__cmos_get_month, .-__cmos_get_month
	.align 16
	.globl	__cmos_do_doomsday
	.type	__cmos_do_doomsday, @function
__cmos_do_doomsday:
.LFB162:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	16(%esp), %ebx
	movl	12(%esp), %eax
	movl	20(%esp), %esi
	cmpb	$2, %bl
	ja	.L934
	leal	13(%ebx), %edx
	leal	-1(%esi), %ecx
.L933:
	movl	%ecx, %ebx
	movzbl	%al, %eax
	movzbl	%dl, %edx
	shrb	$2, %bl
	movzbl	%bl, %esi
	movzbl	%cl, %ebx
	leal	0(,%ebx,4), %ecx
	addl	%ebx, %eax
	addl	%esi, %eax
	movl	%ebx, %esi
	addl	%ebx, %ecx
	leal	(%esi,%ecx,8), %ecx
	shrw	$12, %cx
	subl	%ecx, %eax
	movl	%eax, %ecx
	leal	(%edx,%edx,2), %eax
	leal	-27(%edx,%eax,4), %ebx
	movl	$1717986919, %edx
	movl	%ebx, %eax
	sarl	$31, %ebx
	imull	%edx
	sarl	%edx
	subl	%ebx, %edx
	leal	(%ecx,%edx), %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	movzwl	%ax, %edx
	imull	$9363, %edx, %edx
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	movl	%edx, %ecx
	movl	%eax, %edx
	shrl	$16, %ecx
	subl	%ecx, %edx
	shrw	%dx
	addl	%ecx, %edx
	shrw	$2, %dx
	leal	0(,%edx,8), %ecx
	subl	%edx, %ecx
	movl	%ecx, %edx
	subl	%edx, %eax
	addl	$1, %eax
	ret
	.align 16
.L934:
	.cfi_restore_state
	movl	%ebx, %edx
	movl	%esi, %ecx
	jmp	.L933
	.cfi_endproc
.LFE162:
	.size	__cmos_do_doomsday, .-__cmos_do_doomsday
	.align 16
	.globl	decode_bcd
	.type	decode_bcd, @function
decode_bcd:
.LFB163:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %eax
	movzbl	%al, %ebx
	andl	$15, %eax
	movl	%ebx, %edx
	sarl	$3, %ebx
	sarl	%edx
	andl	$30, %ebx
	movl	%edx, %ecx
	andl	$120, %ecx
	leal	(%ecx,%ebx), %edx
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	addl	%edx, %eax
	ret
	.cfi_endproc
.LFE163:
	.size	decode_bcd, .-decode_bcd
	.comm	cmos,2,1
	.align 16
	.globl	init_cmos
	.type	init_cmos, @function
init_cmos:
.LFB164:
	.cfi_startproc
/APP
/  151 "Source/Drivers/timer/cmos.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	$11, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $112
/  0 "" 2
/  45 "Library/sys.h" 1
	inb $113, %al
/  0 "" 2
/  154 "Source/Drivers/timer/cmos.c" 1
	sti
/  0 "" 2
/NO_APP
	movl	%eax, %edx
	shrb	$2, %al
	shrb	%dl
	xorl	$1, %eax
	andl	$1, %edx
	andl	$1, %eax
	movb	%dl, cmos
	movb	%al, cmos+1
	ret
	.cfi_endproc
.LFE164:
	.size	init_cmos, .-init_cmos
	.align 16
	.globl	cmos_24hour
	.type	cmos_24hour, @function
cmos_24hour:
.LFB165:
	.cfi_startproc
	movzbl	cmos, %eax
	ret
	.cfi_endproc
.LFE165:
	.size	cmos_24hour, .-cmos_24hour
	.align 16
	.globl	cmos_bcd_encoded
	.type	cmos_bcd_encoded, @function
cmos_bcd_encoded:
.LFB166:
	.cfi_startproc
	movzbl	cmos+1, %eax
	ret
	.cfi_endproc
.LFE166:
	.size	cmos_bcd_encoded, .-cmos_bcd_encoded
	.align 16
	.globl	cmos_updating
	.type	cmos_updating, @function
cmos_updating:
.LFB167:
	.cfi_startproc
/APP
/  151 "Source/Drivers/timer/cmos.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	$10, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $112
/  0 "" 2
/  45 "Library/sys.h" 1
	inb $113, %al
/  0 "" 2
/  154 "Source/Drivers/timer/cmos.c" 1
	sti
/  0 "" 2
/NO_APP
	shrb	$7, %al
	ret
	.cfi_endproc
.LFE167:
	.size	cmos_updating, .-cmos_updating
	.align 16
	.globl	cmos_rtc_get_value
	.type	cmos_rtc_get_value, @function
cmos_rtc_get_value:
.LFB169:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %ecx
	movl	12(%esp), %ebx
/APP
/  151 "Source/Drivers/timer/cmos.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	%ecx, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $112
/  0 "" 2
/  45 "Library/sys.h" 1
	inb $113, %al
/  0 "" 2
/NO_APP
	movl	%eax, %edx
/APP
/  154 "Source/Drivers/timer/cmos.c" 1
	sti
/  0 "" 2
/NO_APP
	cmpb	$0, cmos+1
	je	.L942
	testb	%bl, %bl
	je	.L942
	cmpl	$50, %ecx
	ja	.L942
	jmp	*.L945(,%ecx,4)
	.section	.rodata
	.align 4
	.align 4
.L945:
	.long	.L944
	.long	.L942
	.long	.L944
	.long	.L942
	.long	.L944
	.long	.L942
	.long	.L944
	.long	.L944
	.long	.L944
	.long	.L944
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L942
	.long	.L944
	.text
	.align 16
.L944:
	movzbl	%al, %edx
	andl	$15, %eax
	movl	%edx, %ecx
	sarl	$3, %edx
	sarl	%ecx
	andl	$30, %edx
	movl	%ecx, %ebx
	andl	$120, %ebx
	leal	(%ebx,%edx), %ecx
	leal	(%ecx,%eax), %edx
.L942:
	movl	%edx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE169:
	.size	cmos_rtc_get_value, .-cmos_rtc_get_value
	.align 16
	.globl	pci_vendor_lookup
	.type	pci_vendor_lookup, @function
pci_vendor_lookup:
.LFB170:
	.cfi_startproc
	movl	$.LC48, %eax
	ret
	.cfi_endproc
.LFE170:
	.size	pci_vendor_lookup, .-pci_vendor_lookup
	.align 16
	.globl	pci_device_lookup
	.type	pci_device_lookup, @function
pci_device_lookup:
.LFB171:
	.cfi_startproc
	movl	$.LC48, %eax
	ret
	.cfi_endproc
.LFE171:
	.size	pci_device_lookup, .-pci_device_lookup
	.align 16
	.globl	pciConfigReadWord
	.type	pciConfigReadWord, @function
pciConfigReadWord:
.LFB172:
	.cfi_startproc
	movl	16(%esp), %ecx
	movzwl	12(%esp), %edx
	movl	%ecx, %eax
	andl	$252, %eax
	sall	$8, %edx
	orl	$-2147483648, %eax
	orl	%edx, %eax
	movzwl	8(%esp), %edx
	sall	$11, %edx
	orl	%edx, %eax
	movzwl	4(%esp), %edx
	sall	$16, %edx
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	$3324, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	andl	$2, %ecx
	movzwl	%cx, %ecx
	sall	$3, %ecx
	shrl	%cl, %eax
	ret
	.cfi_endproc
.LFE172:
	.size	pciConfigReadWord, .-pciConfigReadWord
	.align 16
	.globl	PciRead32
	.type	PciRead32, @function
PciRead32:
.LFB173:
	.cfi_startproc
	movl	16(%esp), %eax
	movl	12(%esp), %edx
	andl	$252, %eax
	sall	$8, %edx
	orl	$-2147483648, %eax
	orl	%edx, %eax
	movl	8(%esp), %edx
	sall	$11, %edx
	orl	%edx, %eax
	movl	4(%esp), %edx
	sall	$16, %edx
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	$3324, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE173:
	.size	PciRead32, .-PciRead32
	.align 16
	.globl	PciRead16
	.type	PciRead16, @function
PciRead16:
.LFB174:
	.cfi_startproc
	movl	16(%esp), %ecx
	movl	12(%esp), %edx
	movl	%ecx, %eax
	sall	$8, %edx
	andl	$252, %eax
	orl	$-2147483648, %eax
	orl	%edx, %eax
	movl	8(%esp), %edx
	sall	$11, %edx
	orl	%edx, %eax
	movl	4(%esp), %edx
	sall	$16, %edx
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	andl	$2, %ecx
	leal	3324(%ecx), %edx
/APP
/  61 "Library/sys.h" 1
	inw %dx, %ax
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE174:
	.size	PciRead16, .-PciRead16
	.align 16
	.globl	PciRead8
	.type	PciRead8, @function
PciRead8:
.LFB175:
	.cfi_startproc
	movl	16(%esp), %ecx
	movl	12(%esp), %edx
	movl	%ecx, %eax
	sall	$8, %edx
	andl	$252, %eax
	orl	$-2147483648, %eax
	orl	%edx, %eax
	movl	8(%esp), %edx
	sall	$11, %edx
	orl	%edx, %eax
	movl	4(%esp), %edx
	sall	$16, %edx
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	andl	$3, %ecx
	leal	3324(%ecx), %edx
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE175:
	.size	PciRead8, .-PciRead8
	.align 16
	.globl	PciWrite32
	.type	PciWrite32, @function
PciWrite32:
.LFB176:
	.cfi_startproc
	movl	16(%esp), %eax
	movl	12(%esp), %edx
	andl	$252, %eax
	sall	$8, %edx
	orl	$-2147483648, %eax
	orl	%edx, %eax
	movl	8(%esp), %edx
	sall	$11, %edx
	orl	%edx, %eax
	movl	4(%esp), %edx
	sall	$16, %edx
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	$3324, %edx
	movl	20(%esp), %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE176:
	.size	PciWrite32, .-PciWrite32
	.align 16
	.globl	PciWrite16
	.type	PciWrite16, @function
PciWrite16:
.LFB177:
	.cfi_startproc
	movl	16(%esp), %ecx
	movl	12(%esp), %edx
	movl	%ecx, %eax
	sall	$8, %edx
	andl	$252, %eax
	orl	$-2147483648, %eax
	orl	%edx, %eax
	movl	8(%esp), %edx
	sall	$11, %edx
	orl	%edx, %eax
	movl	4(%esp), %edx
	sall	$16, %edx
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	andl	$2, %ecx
	movl	20(%esp), %eax
	leal	3324(%ecx), %edx
/APP
/  53 "Library/sys.h" 1
	outw %ax, %dx
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE177:
	.size	PciWrite16, .-PciWrite16
	.align 16
	.globl	PciWrite8
	.type	PciWrite8, @function
PciWrite8:
.LFB178:
	.cfi_startproc
	movl	16(%esp), %ecx
	movl	12(%esp), %edx
	movl	%ecx, %eax
	sall	$8, %edx
	andl	$252, %eax
	orl	$-2147483648, %eax
	orl	%edx, %eax
	movl	8(%esp), %edx
	sall	$11, %edx
	orl	%edx, %eax
	movl	4(%esp), %edx
	sall	$16, %edx
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	andl	$3, %ecx
	movl	20(%esp), %eax
	leal	3324(%ecx), %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE178:
	.size	PciWrite8, .-PciWrite8
	.align 16
	.globl	PciDeviceRead
	.type	PciDeviceRead, @function
PciDeviceRead:
.LFB179:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	16(%esp), %eax
	movl	8(%esp), %edx
	movl	12(%esp), %ecx
	cmpl	$1, %eax
	je	.L961
	cmpl	$2, %eax
	movl	8(%edx), %eax
	je	.L962
	andl	$252, %ecx
	sall	$11, %eax
	orl	$-2147483648, %ecx
	orl	%ecx, %eax
	movl	12(%edx), %ecx
	sall	$8, %ecx
	orl	%ecx, %eax
	movl	4(%edx), %ecx
	movl	$3320, %edx
	sall	$16, %ecx
	orl	%ecx, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	$3324, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L961:
	.cfi_restore_state
	movl	8(%edx), %eax
	movl	%ecx, %ebx
	andl	$252, %ebx
	orl	$-2147483648, %ebx
	sall	$11, %eax
	orl	%ebx, %eax
	movl	12(%edx), %ebx
	movl	4(%edx), %edx
	sall	$8, %ebx
	sall	$16, %edx
	orl	%ebx, %eax
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	andl	$3, %ecx
	leal	3324(%ecx), %edx
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	movzbl	%al, %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L962:
	.cfi_restore_state
	movl	%ecx, %ebx
	sall	$11, %eax
	andl	$252, %ebx
	orl	$-2147483648, %ebx
	orl	%ebx, %eax
	movl	12(%edx), %ebx
	movl	4(%edx), %edx
	sall	$8, %ebx
	sall	$16, %edx
	orl	%ebx, %eax
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	andl	$2, %ecx
	leal	3324(%ecx), %edx
/APP
/  61 "Library/sys.h" 1
	inw %dx, %ax
/  0 "" 2
/NO_APP
	movzwl	%ax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE179:
	.size	PciDeviceRead, .-PciDeviceRead
	.align 16
	.globl	PciDeviceWrite
	.type	PciDeviceWrite, @function
PciDeviceWrite:
.LFB180:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	20(%esp), %eax
	movl	8(%esp), %edx
	movl	12(%esp), %ecx
	cmpl	$1, %eax
	je	.L968
	cmpl	$2, %eax
	movl	8(%edx), %eax
	je	.L969
	andl	$252, %ecx
	sall	$11, %eax
	orl	$-2147483648, %ecx
	orl	%ecx, %eax
	movl	12(%edx), %ecx
	movl	4(%edx), %edx
	sall	$8, %ecx
	sall	$16, %edx
	orl	%ecx, %eax
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	$3324, %edx
	movl	16(%esp), %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L968:
	.cfi_restore_state
	movl	8(%edx), %eax
	movl	%ecx, %ebx
	andl	$252, %ebx
	orl	$-2147483648, %ebx
	sall	$11, %eax
	orl	%ebx, %eax
	movl	12(%edx), %ebx
	movl	4(%edx), %edx
	sall	$8, %ebx
	sall	$16, %edx
	orl	%ebx, %eax
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	andl	$3, %ecx
	movzbl	16(%esp), %eax
	leal	3324(%ecx), %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L969:
	.cfi_restore_state
	movl	%ecx, %ebx
	sall	$11, %eax
	andl	$252, %ebx
	orl	$-2147483648, %ebx
	orl	%ebx, %eax
	movl	12(%edx), %ebx
	movl	4(%edx), %edx
	sall	$8, %ebx
	sall	$16, %edx
	orl	%ebx, %eax
	orl	%edx, %eax
	movl	$3320, %edx
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	andl	$2, %ecx
	movzwl	16(%esp), %eax
	leal	3324(%ecx), %edx
/APP
/  53 "Library/sys.h" 1
	outw %ax, %dx
/  0 "" 2
/NO_APP
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE180:
	.size	PciDeviceWrite, .-PciDeviceWrite
	.comm	tempPCS,4,4
	.align 16
	.globl	checkDevice
	.type	checkDevice, @function
checkDevice:
.LFB181:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	tempPCS, %eax
	movl	52(%esp), %ecx
	movl	56(%esp), %ebx
	leal	64(%eax), %edx
	.align 16
.L971:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %edx
	jne	.L971
	movl	tempPCS, %eax
	movzbl	%cl, %edi
	movl	$3320, %ebp
	movl	%edi, 12(%esp)
	xorl	%ecx, %ecx
	movl	%eax, %esi
	movl	%eax, 4(%esp)
	movzbl	%bl, %eax
	movl	%edi, %ebx
	movl	%eax, 8(%esp)
	sall	$11, %eax
	sall	$16, %ebx
	movl	$3324, %edi
	orl	$-2147483648, %ebx
	orl	%eax, %ebx
.L972:
	movl	%ecx, %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%ebx, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, (%esi)
	leal	4(%ecx), %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%ebx, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, 4(%esi)
	leal	8(%ecx), %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%ebx, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, 8(%esi)
	leal	12(%ecx), %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%ebx, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	addl	$16, %ecx
	movl	%eax, 12(%esi)
	addl	$16, %esi
	cmpl	$64, %ecx
	jne	.L972
	movl	4(%esp), %eax
	cmpw	$-1, (%eax)
	je	.L980
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	$3320, %ebp
	movl	$3324, %edi
	pushl	$64
	.cfi_def_cfa_offset 64
	call	*kmalloc
	xorl	%ecx, %ecx
	movl	%eax, 20(%esp)
	movl	%eax, %esi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L975:
	movl	%ecx, %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%ebx, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, (%esi)
	leal	4(%ecx), %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%ebx, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, 4(%esi)
	leal	8(%ecx), %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%ebx, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, 8(%esi)
	leal	12(%ecx), %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%ebx, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	addl	$16, %ecx
	movl	%eax, 12(%esi)
	addl	$16, %esi
	cmpl	$64, %ecx
	jne	.L975
	movl	48(%esp), %eax
	movl	12(%esp), %edi
	movl	%edi, 4(%eax)
	movl	8(%esp), %edi
	movl	$0, (%eax)
	movl	$0, 12(%eax)
	movl	$0, 20(%eax)
	movl	$0, 24(%eax)
	movl	%edi, 8(%eax)
	movl	4(%esp), %edi
	movl	%edi, 16(%eax)
	movl	48(%esp), %eax
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret	$4
.L980:
	.cfi_restore_state
	movl	48(%esp), %eax
	movl	$-1, (%eax)
	movl	$-1, 4(%eax)
	movl	$-1, 8(%eax)
	movl	$-1, 12(%eax)
	movl	$0, 16(%eax)
	movl	$0, 20(%eax)
	movl	$0, 24(%eax)
	movl	48(%esp), %eax
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret	$4
	.cfi_endproc
.LFE181:
	.size	checkDevice, .-checkDevice
	.section	.rodata
	.align 4
.LC193:
	.string	"\n\tSecondary DEVICE: %s CLASS: %x SUBCLASS: %x"
	.text
	.align 16
	.globl	checkSecBus
	.type	checkSecBus, @function
checkSecBus:
.LFB182:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	movl	$3320, %ebp
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	$3324, %edi
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	pushl	$60
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	68(%esp), %esi
	movl	%eax, 20(%esp)
	movl	%eax, %ebx
	movl	64(%esp), %eax
	xorl	%ecx, %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	sall	$11, %esi
	orl	$-2147483648, %esi
	sall	$16, %eax
	orl	%eax, %esi
.L982:
	movl	%ecx, %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%esi, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, (%ebx)
	leal	4(%ecx), %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%esi, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, 4(%ebx)
	leal	8(%ecx), %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%esi, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, 8(%ebx)
	leal	12(%ecx), %eax
	movl	%ebp, %edx
	andl	$252, %eax
	orl	%esi, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%edi, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	addl	$16, %ecx
	movl	%eax, 12(%ebx)
	addl	$16, %ebx
	cmpl	$64, %ecx
	jne	.L982
	movl	4(%esp), %eax
	movl	$3320, %edi
	movl	$0, 4(%esp)
	movl	$3324, %ebp
	movzbl	25(%eax), %eax
	sall	$16, %eax
	movl	%eax, 12(%esp)
	.align 16
.L986:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$64
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	20(%esp), %ecx
	movl	%eax, 24(%esp)
	movl	%eax, %ebx
	movl	%ecx, %esi
	sall	$3, %esi
	orl	28(%esp), %esi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	orl	$-2147483648, %esi
	orl	%ecx, %esi
	xorl	%ecx, %ecx
.L983:
	movl	%ecx, %eax
	movl	%edi, %edx
	andl	$252, %eax
	orl	%esi, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%ebp, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, (%ebx)
	leal	4(%ecx), %eax
	movl	%edi, %edx
	andl	$252, %eax
	orl	%esi, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%ebp, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, 4(%ebx)
	leal	8(%ecx), %eax
	movl	%edi, %edx
	andl	$252, %eax
	orl	%esi, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%ebp, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	movl	%eax, 8(%ebx)
	leal	12(%ecx), %eax
	movl	%edi, %edx
	andl	$252, %eax
	orl	%esi, %eax
/APP
/  86 "Library/sys.h" 1
	outl %eax, %dx
/  0 "" 2
/NO_APP
	movl	%ebp, %edx
/APP
/  94 "Library/sys.h" 1
	inl %dx, %eax
/  0 "" 2
/NO_APP
	addl	$16, %ecx
	movl	%eax, 12(%ebx)
	addl	$16, %ebx
	cmpl	$64, %ecx
	jne	.L983
	movl	8(%esp), %eax
	cmpw	$-1, (%eax)
	je	.L991
	movl	8(%esp), %ebx
	movzbl	10(%ebx), %eax
	pushl	%eax
	.cfi_def_cfa_offset 52
	movzbl	11(%ebx), %eax
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$.LC48
	.cfi_def_cfa_offset 60
	pushl	$.LC193
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L985:
	addl	$256, 4(%esp)
	addl	$65536, 12(%esp)
	movl	4(%esp), %eax
	cmpl	$2048, %eax
	jne	.L986
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L991:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	20(%esp)
	.cfi_def_cfa_offset 64
	call	*kfree
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L985
	.cfi_endproc
.LFE182:
	.size	checkSecBus, .-checkSecBus
	.section	.rodata
.LC194:
	.string	"\n\n  Secondary Bus number "
.LC195:
	.string	"\n\n  BUS number "
.LC196:
	.string	"\n"
.LC197:
	.string	"\n\tDEVICE: "
.LC198:
	.string	" CLASS: "
.LC199:
	.string	" subCLASS: "
.LC200:
	.string	"\n\n"
	.text
	.align 16
	.globl	checkAllBuses
	.type	checkAllBuses, @function
checkAllBuses:
.LFB183:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$72, %esp
	.cfi_def_cfa_offset 92
	pushl	$64
	.cfi_def_cfa_offset 96
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, tempPCS
	movl	$0, 12(%esp)
	movl	$0, (%esp)
	.align 16
.L993:
	movl	12(%esp), %eax
	xorl	%ebp, %ebp
	movb	$0, 10(%esp)
	leal	devices(%eax), %ebx
	imull	$896, (%esp), %eax
	movl	%eax, 4(%esp)
	jmp	.L998
	.align 16
.L994:
	cmpl	$-1, 4(%ebx)
	je	.L995
	movl	16(%ebx), %edx
	cmpb	$-1, 11(%edx)
	je	.L995
	movzbl	11(%eax), %esi
	movzbl	10(%eax), %edx
	movl	%esi, %ecx
	movb	%dl, 11(%esp)
	cmpb	$1, %cl
	jne	.L996
	cmpb	$6, %dl
	jne	.L996
	imull	$28, TotalAHCIDevices, %ecx
	movl	(%ebx), %edi
	leal	AHCI_Devices(%ecx), %edx
	movl	%edi, AHCI_Devices(%ecx)
	movl	4(%ebx), %ecx
	movl	%eax, 16(%edx)
	movl	20(%ebx), %eax
	movl	%ecx, 4(%edx)
	movl	8(%ebx), %ecx
	movl	%eax, 20(%edx)
	movl	24(%ebx), %eax
	movl	%ecx, 8(%edx)
	movl	12(%ebx), %ecx
	movl	%eax, 24(%edx)
	movl	TotalAHCIDevices, %eax
	movl	%ecx, 12(%edx)
	addl	$1, %eax
	movl	%eax, TotalAHCIDevices
.L996:
	cmpb	$0, 10(%esp)
	je	.L1008
.L997:
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$.LC197
	.cfi_def_cfa_offset 96
	call	printf
	movl	$.LC48, (%esp)
	call	printf
	movl	$.LC198, (%esp)
	call	printf
	movl	%esi, %eax
	movzbl	%al, %esi
	movl	%esi, (%esp)
	call	console_write_dec
	movl	$.LC199, (%esp)
	call	printf
	movzbl	27(%esp), %eax
	movzbl	%al, %edi
	movl	%edi, (%esp)
	call	console_write_dec
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movb	$1, 10(%esp)
.L995:
	addl	$1, %ebp
	addl	$28, %ebx
	cmpl	$32, %ebp
	je	.L1009
.L998:
	subl	$4, %esp
	.cfi_def_cfa_offset 84
	imull	$28, %ebp, %edi
	addl	8(%esp), %edi
	pushl	%ebp
	.cfi_def_cfa_offset 88
	pushl	8(%esp)
	.cfi_def_cfa_offset 92
	leal	28(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 96
	call	checkDevice
	.cfi_def_cfa_offset 92
	movl	28(%esp), %eax
	movl	%eax, devices(%edi)
	movl	32(%esp), %eax
	movl	%eax, devices+4(%edi)
	movl	36(%esp), %eax
	movl	%eax, devices+8(%edi)
	movl	40(%esp), %eax
	movl	%eax, devices+12(%edi)
	movl	44(%esp), %eax
	movl	%eax, devices+16(%edi)
	movl	48(%esp), %eax
	movl	%eax, devices+20(%edi)
	movl	52(%esp), %eax
	addl	$12, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, devices+24(%edi)
	movl	16(%ebx), %eax
	cmpb	$1, 14(%eax)
	jne	.L994
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	addl	$28, %ebx
	pushl	$.LC194
	.cfi_def_cfa_offset 96
	call	printf
	popl	%edx
	.cfi_def_cfa_offset 92
	popl	%ecx
	.cfi_def_cfa_offset 88
	pushl	%ebp
	.cfi_def_cfa_offset 92
	pushl	12(%esp)
	.cfi_def_cfa_offset 96
	addl	$1, %ebp
	call	checkSecBus
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	cmpl	$32, %ebp
	jne	.L998
	.align 16
.L1009:
	addl	$1, (%esp)
	addl	$896, 12(%esp)
	movl	(%esp), %eax
	cmpl	$255, %eax
	jne	.L993
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$.LC200
	.cfi_def_cfa_offset 96
	call	printf
	addl	$76, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1008:
	.cfi_def_cfa_offset 80
	.cfi_offset 3, -20
	.cfi_offset 5, -8
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$.LC195
	.cfi_def_cfa_offset 96
	call	printf
	popl	%eax
	.cfi_def_cfa_offset 92
	pushl	12(%esp)
	.cfi_def_cfa_offset 96
	call	console_write_dec
	movl	$.LC196, (%esp)
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	jmp	.L997
	.cfi_endproc
.LFE183:
	.size	checkAllBuses, .-checkAllBuses
	.align 16
	.globl	PciGetBar_1
	.type	PciGetBar_1, @function
PciGetBar_1:
.LFB184:
	.cfi_startproc
	movl	12(%esp), %edx
	testl	%edx, %edx
	je	.L1018
	cmpl	$1, %edx
	je	.L1019
	cmpl	$2, %edx
	je	.L1020
	cmpl	$3, %edx
	je	.L1021
	cmpl	$4, %edx
	je	.L1022
	cmpl	$5, %edx
	movl	$-1, %eax
	je	.L1023
	rep ret
	.align 16
.L1018:
	imull	$28, 8(%esp), %edx
	imull	$896, 4(%esp), %eax
	movl	devices+16(%edx,%eax), %eax
	movl	16(%eax), %eax
	ret
	.align 16
.L1019:
	imull	$28, 8(%esp), %edx
	imull	$896, 4(%esp), %eax
	movl	devices+16(%edx,%eax), %eax
	movl	20(%eax), %eax
	ret
	.align 16
.L1020:
	imull	$28, 8(%esp), %edx
	imull	$896, 4(%esp), %eax
	movl	devices+16(%edx,%eax), %eax
	movl	24(%eax), %eax
	ret
	.align 16
.L1021:
	imull	$28, 8(%esp), %edx
	imull	$896, 4(%esp), %eax
	movl	devices+16(%edx,%eax), %eax
	movl	28(%eax), %eax
	ret
	.align 16
.L1022:
	imull	$28, 8(%esp), %edx
	imull	$896, 4(%esp), %eax
	movl	devices+16(%edx,%eax), %eax
	movl	32(%eax), %eax
	ret
	.align 16
.L1023:
	imull	$28, 8(%esp), %edx
	imull	$896, 4(%esp), %eax
	movl	devices+16(%edx,%eax), %eax
	movl	36(%eax), %eax
	ret
	.cfi_endproc
.LFE184:
	.size	PciGetBar_1, .-PciGetBar_1
	.align 16
	.globl	PciGetBar_2
	.type	PciGetBar_2, @function
PciGetBar_2:
.LFB185:
	.cfi_startproc
	movl	8(%esp), %edx
	testl	%edx, %edx
	je	.L1032
	cmpl	$1, %edx
	je	.L1033
	cmpl	$2, %edx
	je	.L1034
	cmpl	$3, %edx
	je	.L1035
	cmpl	$4, %edx
	je	.L1036
	cmpl	$5, %edx
	movl	$-1, %eax
	je	.L1037
	rep ret
	.align 16
.L1032:
	movl	4(%esp), %eax
	movl	16(%eax), %eax
	movl	16(%eax), %eax
	ret
	.align 16
.L1033:
	movl	4(%esp), %eax
	movl	16(%eax), %eax
	movl	20(%eax), %eax
	ret
	.align 16
.L1034:
	movl	4(%esp), %eax
	movl	16(%eax), %eax
	movl	24(%eax), %eax
	ret
	.align 16
.L1035:
	movl	4(%esp), %eax
	movl	16(%eax), %eax
	movl	28(%eax), %eax
	ret
	.align 16
.L1036:
	movl	4(%esp), %eax
	movl	16(%eax), %eax
	movl	32(%eax), %eax
	ret
	.align 16
.L1037:
	movl	4(%esp), %eax
	movl	16(%eax), %eax
	movl	36(%eax), %eax
	ret
	.cfi_endproc
.LFE185:
	.size	PciGetBar_2, .-PciGetBar_2
	.comm	test1,4,4
	.comm	disks,4,4
	.comm	controllers,4,4
	.comm	bus,4,4
	.comm	device,4,4
	.comm	func,4,4
	.comm	abar,4,4
	.comm	AHCI,1,1
	.align 16
	.globl	get_device_info
	.type	get_device_info, @function
get_device_info:
.LFB186:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	$4098, %edx
	movzwl	8(%esp), %ecx
	movzwl	12(%esp), %ebx
	movl	$kSupportedDevices, %eax
	jmp	.L1041
	.align 16
.L1039:
	addl	$8, %eax
	movzwl	(%eax), %edx
	testw	%dx, %dx
	je	.L1044
.L1041:
	cmpw	%dx, %cx
	jne	.L1039
	cmpw	%bx, 2(%eax)
	jne	.L1039
	movl	4(%eax), %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1044:
	.cfi_restore_state
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE186:
	.size	get_device_info, .-get_device_info
	.globl	ahci_found
	.section	.bss
	.type	ahci_found, @object
	.size	ahci_found, 1
ahci_found:
	.zero	1
	.globl	data_base
	.align 4
	.type	data_base, @object
	.size	data_base, 4
data_base:
	.zero	4
	.section	.rodata
.LC201:
	.string	"\nAHCI disk sent an intterupt\n"
	.text
	.align 16
	.globl	ahci_handler
	.type	ahci_handler, @function
ahci_handler:
.LFB187:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC201
	.cfi_def_cfa_offset 32
	call	printf
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE187:
	.size	ahci_handler, .-ahci_handler
	.section	.rodata
.LC202:
	.string	"\n\tAHCI CONTROLLER #"
	.align 4
.LC203:
	.string	" FOUND, INITIALIZING AHCI CONTROLLER and Disks"
	.align 4
.LC204:
	.string	"\n\tAHCI CONTROLLER Initialized\n"
	.text
	.align 16
	.globl	checkAHCI
	.type	checkAHCI, @function
checkAHCI:
.LFB188:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$4096
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	$4096, (%esp)
	movl	%eax, ahci
	movl	%eax, ahci_start
	call	*kmalloc
	movl	$1048576, (%esp)
	movl	%eax, Disk_dev
	movl	%eax, Disk_dev_start
	call	*kmalloc
	movl	TotalAHCIDevices, %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%eax, AHCI_BASE
	testl	%ecx, %ecx
	je	.L1053
	movl	controllers, %edx
	movl	ahci, %eax
	movl	$AHCI_Devices, %ebx
	xorl	%esi, %esi
	.align 16
.L1052:
	addl	$1, %edx
	movl	%edx, controllers
	movl	%ebx, (%eax)
	movl	$4098, %edx
	movl	16(%ebx), %eax
	movzwl	2(%eax), %edi
	movzwl	(%eax), %ecx
	movl	$kSupportedDevices, %eax
	jmp	.L1051
	.align 16
.L1049:
	addl	$8, %eax
	movzwl	(%eax), %edx
	testw	%dx, %dx
	je	.L1058
.L1051:
	cmpw	%dx, %cx
	jne	.L1049
	cmpw	2(%eax), %di
	jne	.L1049
	movl	4(%eax), %eax
.L1050:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	addl	$1, %esi
	addl	$28, %ebx
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	printf
	movl	$.LC202, (%esp)
	call	printf
	popl	%eax
	.cfi_def_cfa_offset 28
	pushl	controllers
	.cfi_def_cfa_offset 32
	call	console_write_dec
	movl	$.LC203, (%esp)
	call	printf
	popl	%edx
	.cfi_def_cfa_offset 28
	pushl	ahci
	.cfi_def_cfa_offset 32
	call	probe_port
	movl	$.LC204, (%esp)
	call	printf
	movl	ahci, %eax
	movl	controllers, %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movb	$1, ahci_found
	movw	%dx, 4(%eax)
	addl	$520, %eax
	cmpl	%esi, TotalAHCIDevices
	movl	%eax, ahci
	ja	.L1052
.L1053:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC196
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	controllers, %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1058:
	.cfi_restore_state
	xorl	%eax, %eax
	jmp	.L1050
	.cfi_endproc
.LFE188:
	.size	checkAHCI, .-checkAHCI
	.comm	satatest,4,4
	.section	.rodata
	.align 4
.LC205:
	.string	"\n\t The Data we Recieved from identity command:\n\t"
	.align 4
.LC206:
	.string	"\n\t\tSerial Number: %s \n\t\tModel Number: %s\n\t\tFirmware: %s\n"
.LC207:
	.string	"\t\tTotal Logical Sectors: %x "
	.align 4
.LC208:
	.string	"  Heads: %x Sectors: %x Cyclinders: %x Bytes per sectors: %x Bytes per track: %x"
	.text
	.align 16
	.globl	IDENTIFYdrive
	.type	IDENTIFYdrive, @function
IDENTIFYdrive:
.LFB189:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	28(%esp), %ebx
	pushl	$512
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	%eax, 8(%ebx)
	movl	$0, (%esp)
	pushl	$0
	.cfi_def_cfa_offset 36
	pushl	$0
	.cfi_def_cfa_offset 40
	pushl	$512
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%eax
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$236
	.cfi_def_cfa_offset 60
	pushl	4(%ebx)
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	8(%ebx), %ebx
	addl	$36, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC205
	.cfi_def_cfa_offset 32
	call	printf
	leal	46(%ebx), %eax
	pushl	%eax
	.cfi_def_cfa_offset 36
	leal	54(%ebx), %eax
	pushl	%eax
	.cfi_def_cfa_offset 40
	leal	20(%ebx), %eax
	pushl	%eax
	.cfi_def_cfa_offset 44
	pushl	$.LC206
	.cfi_def_cfa_offset 48
	call	printf
	addl	$24, %esp
	.cfi_def_cfa_offset 24
	pushl	120(%ebx)
	.cfi_def_cfa_offset 28
	pushl	$.LC207
	.cfi_def_cfa_offset 32
	call	printf
	popl	%eax
	.cfi_def_cfa_offset 28
	movzwl	8(%ebx), %eax
	popl	%edx
	.cfi_def_cfa_offset 24
	pushl	%eax
	.cfi_def_cfa_offset 28
	movzwl	10(%ebx), %eax
	pushl	%eax
	.cfi_def_cfa_offset 32
	movzwl	2(%ebx), %eax
	pushl	%eax
	.cfi_def_cfa_offset 36
	movzwl	12(%ebx), %eax
	pushl	%eax
	.cfi_def_cfa_offset 40
	movzwl	6(%ebx), %eax
	pushl	%eax
	.cfi_def_cfa_offset 44
	pushl	$.LC208
	.cfi_def_cfa_offset 48
	call	printf
	addl	$40, %esp
	.cfi_def_cfa_offset 8
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE189:
	.size	IDENTIFYdrive, .-IDENTIFYdrive
	.section	.rodata
.LC209:
	.string	"\n\t\tSATAPI drive found \n"
.LC210:
	.string	"\n\t\tSEMB drive found \n"
.LC211:
	.string	"\n\t\tPM drive found \n"
	.align 4
.LC212:
	.string	"\n\t\tNo Drives Recognized on this controller\n"
	.align 4
.LC213:
	.string	"\n\n\t\tSata Disk #%x at port %x Found!\n"
	.text
	.align 16
	.globl	probe_port
	.type	probe_port, @function
probe_port:
.LFB190:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	xorl	%ebx, %ebx
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	movl	60(%esp), %eax
	movl	%eax, %edi
	movl	%eax, 16(%esp)
	pushl	$4096
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%eax, Disk_dev
	movl	(%edi), %eax
	movl	16(%eax), %eax
	movl	36(%eax), %eax
	movl	%eax, abar
	movl	12(%eax), %esi
	movl	%edi, %eax
	addl	$8, %eax
	movl	%eax, 28(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, %ebp
	movl	$0, 8(%esp)
	jmp	.L1067
	.align 16
.L1062:
	addl	$1, %ebx
	shrl	%esi
	addl	$16, %ebp
	cmpl	$32, %ebx
	je	.L1085
.L1067:
	testl	$1, %esi
	je	.L1062
	movl	abar, %eax
	leal	2(%ebx), %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	sall	$7, %edi
	addl	%edi, %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	check_type
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	je	.L1062
	movl	abar, %ecx
	movl	%ebx, %edx
	sall	$7, %edx
	addl	%ecx, %edx
	movl	%ecx, (%esp)
	movl	280(%edx), %ecx
	andl	$1, %ecx
	jne	.L1063
	movl	280(%edx), %ecx
	orl	$1, %ecx
	movl	%ecx, 280(%edx)
.L1063:
	cmpl	$1, %eax
	jne	.L1064
	movl	sata, %ecx
	addl	(%esp), %edi
	shrl	%esi
	movb	$1, AHCI
	addl	$16, %ebp
	movl	%ecx, %edx
	addl	$1, %ecx
	sall	$4, %edx
	addl	Disk_dev, %edx
	movl	$1, (%edx)
	movl	%edi, 4(%edx)
	movl	$1, -16(%ebp)
	movl	4(%edx), %edi
	movl	%edi, -12(%ebp)
	movl	8(%edx), %edi
	movl	%edi, -8(%ebp)
	movl	12(%edx), %edx
	movl	4(%esp), %edi
	movl	%edx, -4(%ebp)
	movl	disks, %edx
	movl	%ecx, sata
	movl	%ebx, %ecx
	addl	$1, %ebx
	sall	%cl, %eax
	orl	%eax, 8(%esp)
	movw	%dx, 6(%edi)
	addl	$1, %edx
	cmpl	$32, %ebx
	movl	%edx, disks
	jne	.L1067
	.align 16
.L1085:
	xorl	%ebx, %ebx
	cmpb	$0, AHCI
	movl	$1, %eax
	movl	$1, %edi
	je	.L1086
	movl	8(%esp), %ebp
	movl	12(%esp), %esi
	jmp	.L1077
	.align 16
.L1069:
	addl	$1, %ebx
	addl	$16, %esi
	cmpl	$32, %ebx
	je	.L1087
.L1077:
	movl	%edi, %edx
	movl	%ebx, %ecx
	sall	%cl, %edx
	testl	%ebp, %edx
	je	.L1069
	leal	1(%eax), %ecx
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	movl	%ecx, 4(%esp)
	pushl	%ebx
	.cfi_def_cfa_offset 56
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	$.LC213
	.cfi_def_cfa_offset 64
	call	printf
	popl	%eax
	.cfi_def_cfa_offset 60
	leal	2(%ebx), %eax
	popl	%edx
	.cfi_def_cfa_offset 56
	sall	$7, %eax
	addl	abar, %eax
	pushl	%ebx
	.cfi_def_cfa_offset 60
	addl	$1, %ebx
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	port_rebase
	movl	%esi, (%esp)
	addl	$16, %esi
	call	IDENTIFYdrive
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	$32, %ebx
	movl	(%esp), %eax
	jne	.L1077
.L1087:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1064:
	.cfi_restore_state
	cmpl	$4, %eax
	je	.L1088
	cmpl	$2, %eax
	jne	.L1066
	addl	(%esp), %edi
	movl	Disk_dev, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movb	$1, AHCI
	movl	$3, (%eax)
	addl	$16, %eax
	movl	%edi, -12(%eax)
	movl	$3, 0(%ebp)
	movl	-12(%eax), %edx
	movl	16(%esp), %edi
	movl	%edx, 4(%ebp)
	movl	-8(%eax), %edx
	movl	%edx, 8(%ebp)
	movl	-4(%eax), %edx
	movl	%edx, 12(%ebp)
	movl	disks, %edx
	movl	%eax, Disk_dev
	movw	%dx, 6(%edi)
	pushl	$.LC210
	.cfi_def_cfa_offset 64
	addl	$1, %edx
	movl	%edx, disks
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1062
	.align 16
.L1088:
	addl	(%esp), %edi
	movl	Disk_dev, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movb	$1, AHCI
	movl	$2, (%eax)
	addl	$16, %eax
	movl	%edi, -12(%eax)
	movl	$2, 0(%ebp)
	movl	-12(%eax), %edx
	movl	16(%esp), %edi
	movl	%edx, 4(%ebp)
	movl	-8(%eax), %edx
	movl	%edx, 8(%ebp)
	movl	-4(%eax), %edx
	movl	%edx, 12(%ebp)
	movl	disks, %edx
	movl	%eax, Disk_dev
	movw	%dx, 6(%edi)
	pushl	$.LC209
	.cfi_def_cfa_offset 64
	addl	$1, %edx
	movl	%edx, disks
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1062
	.align 16
.L1066:
	cmpl	$3, %eax
	jne	.L1062
	addl	(%esp), %edi
	movl	Disk_dev, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movb	$1, AHCI
	movl	$4, (%eax)
	addl	$16, %eax
	movl	%edi, -12(%eax)
	movl	$4, 0(%ebp)
	movl	-12(%eax), %edx
	movl	16(%esp), %edi
	movl	%edx, 4(%ebp)
	movl	-8(%eax), %edx
	movl	%edx, 8(%ebp)
	movl	-4(%eax), %edx
	movl	%edx, 12(%ebp)
	movl	disks, %edx
	movl	%eax, Disk_dev
	movw	%dx, 6(%edi)
	pushl	$.LC211
	.cfi_def_cfa_offset 64
	addl	$1, %edx
	movl	%edx, disks
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1062
.L1086:
	movl	$.LC212, 48(%esp)
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	printf
	.cfi_endproc
.LFE190:
	.size	probe_port, .-probe_port
	.align 16
	.globl	check_type
	.type	check_type, @function
check_type:
.LFB191:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %edx
	movl	52(%edx), %ecx
	movl	40(%edx), %eax
	movl	%eax, %ebx
	andl	$15, %ebx
	cmpb	$3, %bl
	setne	%bl
	testl	%ecx, %ecx
	sete	%cl
	orb	%cl, %bl
	jne	.L1094
	shrl	$8, %eax
	andl	$15, %eax
	cmpb	$1, %al
	jne	.L1094
	movl	36(%edx), %edx
	movl	$2, %eax
	cmpl	$-1019477759, %edx
	je	.L1089
	cmpl	$-351010559, %edx
	movl	$4, %eax
	je	.L1089
	xorl	%eax, %eax
	cmpl	$-1771503359, %edx
	sete	%al
	leal	1(%eax,%eax), %eax
.L1089:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1094:
	.cfi_restore_state
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE191:
	.size	check_type, .-check_type
	.align 16
	.globl	port_rebase
	.type	port_rebase, @function
port_rebase:
.LFB192:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	abar, %eax
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	20(%esp), %esi
	movl	$-2147483648, 4(%eax)
	movl	$1, 4(%eax)
	movl	$-2147483648, 4(%eax)
	movl	$2, 4(%eax)
	movl	24(%esi), %eax
	movl	24(%esp), %edi
	andl	$-2, %eax
	movl	%eax, 24(%esi)
	movl	24(%esi), %eax
	andl	$-17, %eax
	movl	%eax, 24(%esi)
.L1100:
	movl	24(%esi), %eax
	testb	$64, %ah
	jne	.L1100
	movl	24(%esi), %ebx
	andl	$32768, %ebx
	jne	.L1100
	movl	24(%esi), %eax
	andl	$-17, %eax
	movl	%eax, 24(%esi)
	movl	24(%esi), %eax
	andb	$127, %ah
	movl	%eax, 24(%esi)
	movl	24(%esi), %eax
	andb	$191, %ah
	movl	%eax, 24(%esi)
	movl	24(%esi), %eax
	andl	$-2, %eax
	movl	%eax, 24(%esi)
	movl	24(%esi), %eax
	andl	$-9, %eax
	movl	%eax, 24(%esi)
	movl	%edi, %eax
	movl	$65535, 48(%esi)
	sall	$10, %eax
	addl	AHCI_BASE, %eax
	movl	$0, 16(%esi)
	movl	%eax, (%esi)
	movl	$0, 4(%esi)
	movl	(%esi), %eax
	leal	1024(%eax), %edx
	.align 16
.L1102:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L1102
	movl	AHCI_BASE, %edx
	movl	%edi, %eax
	sall	$8, %eax
	leal	32768(%edx,%eax), %eax
	movl	%eax, 8(%esi)
	movl	$0, 12(%esi)
	movl	8(%esi), %eax
	leal	256(%eax), %edx
	.align 16
.L1103:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L1103
	movl	(%esi), %ebp
	sall	$13, %edi
	leal	2(%ebp), %ecx
	addl	$1026, %ebp
	.align 16
.L1105:
	movl	$8, %eax
	movw	%ax, (%ecx)
	movl	AHCI_BASE, %eax
	movl	$0, 10(%ecx)
	leal	40960(%edi,%eax), %eax
	addl	%ebx, %eax
	leal	256(%eax), %edx
	movl	%eax, 6(%ecx)
	.align 16
.L1104:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %edx
	jne	.L1104
	addl	$32, %ecx
	addl	$256, %ebx
	cmpl	%ebp, %ecx
	jne	.L1105
	movl	24(%esi), %eax
	orl	$16, %eax
	movl	%eax, 24(%esi)
	movl	24(%esi), %eax
	orl	$1, %eax
	movl	%eax, 24(%esi)
	movl	$0, 16(%esi)
	movl	$0, 20(%esi)
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE192:
	.size	port_rebase, .-port_rebase
	.align 16
	.globl	start_cmd
	.type	start_cmd, @function
start_cmd:
.LFB193:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	24(%eax), %edx
	orl	$16, %edx
	movl	%edx, 24(%eax)
	movl	24(%eax), %edx
	orl	$1, %edx
	movl	%edx, 24(%eax)
	ret
	.cfi_endproc
.LFE193:
	.size	start_cmd, .-start_cmd
	.align 16
	.globl	stop_cmd
	.type	stop_cmd, @function
stop_cmd:
.LFB194:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	24(%eax), %edx
	andl	$-2, %edx
	movl	%edx, 24(%eax)
	movl	24(%eax), %edx
	andl	$-17, %edx
	movl	%edx, 24(%eax)
	.align 16
.L1115:
	movl	24(%eax), %edx
	andb	$64, %dh
	jne	.L1115
	movl	24(%eax), %edx
	andb	$128, %dh
	jne	.L1115
	movl	24(%eax), %edx
	andl	$-17, %edx
	movl	%edx, 24(%eax)
	ret
	.cfi_endproc
.LFE194:
	.size	stop_cmd, .-stop_cmd
	.section	.rodata
.LC214:
	.string	"Read disk error\n"
	.text
	.align 16
	.globl	read
	.type	read, @function
read:
.LFB195:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	40(%esp), %eax
	movl	28(%esp), %ebx
	movl	%eax, %edx
	pushl	%eax
	.cfi_def_cfa_offset 32
	subl	$1, %eax
	shrl	$4, %eax
	sall	$9, %edx
	pushl	40(%esp)
	.cfi_def_cfa_offset 36
	movzwl	%ax, %eax
	pushl	40(%esp)
	.cfi_def_cfa_offset 40
	pushl	%edx
	.cfi_def_cfa_offset 44
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	64(%esp)
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$32
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%ebx), %edx
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	movl	$1, %eax
	andl	$1073741824, %edx
	je	.L1119
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC214
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
.L1119:
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE195:
	.size	read, .-read
	.section	.rodata
.LC215:
	.string	"Write disk error\n"
	.text
	.align 16
	.globl	write
	.type	write, @function
write:
.LFB196:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	40(%esp), %eax
	movl	28(%esp), %ebx
	movl	%eax, %edx
	pushl	%eax
	.cfi_def_cfa_offset 32
	subl	$1, %eax
	shrl	$4, %eax
	sall	$9, %edx
	pushl	40(%esp)
	.cfi_def_cfa_offset 36
	movzwl	%ax, %eax
	pushl	40(%esp)
	.cfi_def_cfa_offset 40
	pushl	%edx
	.cfi_def_cfa_offset 44
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	64(%esp)
	.cfi_def_cfa_offset 52
	pushl	$1
	.cfi_def_cfa_offset 56
	pushl	$48
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%ebx), %edx
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	movl	$1, %eax
	andl	$1073741824, %edx
	je	.L1125
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC215
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
.L1125:
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE196:
	.size	write, .-write
	.align 16
	.globl	sec_read_static
	.type	sec_read_static, @function
sec_read_static:
.LFB199:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	40(%esp), %eax
	movl	28(%esp), %ebx
	movl	%eax, %edx
	pushl	%eax
	.cfi_def_cfa_offset 32
	subl	$1, %eax
	shrl	$4, %eax
	sall	$9, %edx
	pushl	40(%esp)
	.cfi_def_cfa_offset 36
	movzwl	%ax, %eax
	pushl	40(%esp)
	.cfi_def_cfa_offset 40
	pushl	%edx
	.cfi_def_cfa_offset 44
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	64(%esp)
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$32
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%ebx), %edx
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	movl	$1, %eax
	andl	$1073741824, %edx
	je	.L1131
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC214
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
.L1131:
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE199:
	.size	sec_read_static, .-sec_read_static
	.align 16
	.globl	sec_write_static
	.type	sec_write_static, @function
sec_write_static:
.LFB200:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	40(%esp), %eax
	movl	28(%esp), %ebx
	movl	%eax, %edx
	pushl	%eax
	.cfi_def_cfa_offset 32
	subl	$1, %eax
	shrl	$4, %eax
	sall	$9, %edx
	pushl	40(%esp)
	.cfi_def_cfa_offset 36
	movzwl	%ax, %eax
	pushl	40(%esp)
	.cfi_def_cfa_offset 40
	pushl	%edx
	.cfi_def_cfa_offset 44
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	64(%esp)
	.cfi_def_cfa_offset 52
	pushl	$1
	.cfi_def_cfa_offset 56
	pushl	$48
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%ebx), %edx
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	movl	$1, %eax
	andl	$1073741824, %edx
	je	.L1137
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC215
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
.L1137:
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE200:
	.size	sec_write_static, .-sec_write_static
	.align 16
	.globl	SATA_Commander
	.type	SATA_Commander, @function
SATA_Commander:
.LFB201:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	movl	64(%esp), %eax
	movl	68(%esp), %ebx
	movl	%eax, 24(%esp)
	movl	60(%esp), %eax
	andl	$1, %ebx
	sall	$6, %ebx
	orl	$-123, %ebx
	movl	(%eax), %esi
	pushl	60(%esp)
	.cfi_def_cfa_offset 64
	call	find_cmdslot
	sall	$5, %eax
	addl	%eax, %esi
	movzwl	80(%esp), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	64(%esp), %ebp
	orb	$4, 1(%esi)
	movb	%bl, (%esi)
	movl	8(%esi), %edi
	subl	$1, %ebp
	movw	%ax, 2(%esi)
	je	.L1144
	movl	60(%esp), %esi
	leal	128(%edi), %ebx
	xorl	%ecx, %ecx
	.align 16
.L1145:
	leal	8(%ecx), %edx
	movl	%esi, (%ebx)
	movl	$0, 4(%ebx)
	addl	$1, %ecx
	addl	$8192, %esi
	addl	$16, %ebx
	sall	$4, %edx
	addl	%edi, %edx
	movl	12(%edx), %eax
	andl	$-4194304, %eax
	orb	$32, %ah
	movl	%eax, 12(%edx)
	shrl	$24, %eax
	andl	$127, %eax
	cmpl	%ebp, %ecx
	movb	%al, 15(%edx)
	jne	.L1145
	movl	64(%esp), %eax
	movl	60(%esp), %esi
	addl	$524286, %eax
	sall	$13, %eax
	leal	8192(%esi,%eax), %eax
	movl	%eax, 60(%esp)
.L1144:
	movl	60(%esp), %esi
	movl	%ebp, %eax
	addl	$8, %ebp
	sall	$4, %eax
	sall	$4, %ebp
	movl	68(%esp), %ecx
	addl	%edi, %eax
	movl	%esi, 128(%eax)
	movl	$0, 132(%eax)
	leal	(%edi,%ebp), %eax
	andl	$4194303, %ecx
	movl	12(%eax), %edx
	andl	$-4194304, %edx
	orl	%ecx, %edx
	movl	%edx, 12(%eax)
	shrl	$24, %edx
	andl	$127, %edx
	movb	%dl, 15(%eax)
	leal	20(%edi), %edx
	movl	%edi, %eax
	.align 16
.L1146:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L1146
	movzbl	12(%esp), %eax
	movb	$39, (%edi)
	orb	$-128, 1(%edi)
	movb	$64, 7(%edi)
	movb	%al, 2(%edi)
	movzbl	72(%esp), %eax
	movb	%al, 4(%edi)
	movl	72(%esp), %eax
	shrl	$8, %eax
	movb	%al, 5(%edi)
	movl	72(%esp), %eax
	shrl	$16, %eax
	movb	%al, 6(%edi)
	movl	72(%esp), %eax
	shrl	$24, %eax
	movb	%al, 8(%edi)
	movzbl	76(%esp), %eax
	movb	%al, 9(%edi)
	movl	76(%esp), %eax
	shrl	$8, %eax
	movb	%al, 10(%edi)
	movzbl	80(%esp), %eax
	movb	%al, 12(%edi)
	movl	80(%esp), %eax
	shrl	$8, %eax
	movb	%al, 13(%edi)
	movl	48(%esp), %eax
	movl	$1, 56(%eax)
	movl	%eax, %edx
	.align 16
.L1147:
	movl	56(%edx), %eax
	testl	%eax, %eax
	jne	.L1147
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE201:
	.size	SATA_Commander, .-SATA_Commander
	.section	.rodata
	.align 4
.LC216:
	.string	"Cannot find free command list entry\n"
	.text
	.align 16
	.globl	find_cmdslot
	.type	find_cmdslot, @function
find_cmdslot:
.LFB202:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	movl	16(%esp), %eax
	movl	52(%eax), %edx
	movl	56(%eax), %eax
	orl	%eax, %edx
	movl	abar, %eax
	movl	(%eax), %ecx
	shrl	$8, %ecx
	andl	$15, %ecx
	je	.L1156
	xorl	%eax, %eax
	testb	$1, %dl
	jne	.L1159
	jmp	.L1155
	.align 16
.L1160:
	testb	$1, %dl
	je	.L1155
.L1159:
	addl	$1, %eax
	shrl	%edx
	cmpl	%eax, %ecx
	jne	.L1160
.L1156:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC216
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$-1, %eax
.L1155:
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE202:
	.size	find_cmdslot, .-find_cmdslot
	.comm	buffer0,8,4
	.globl	seed1
	.data
	.align 4
	.type	seed1, @object
	.size	seed1, 4
seed1:
	.long	9
	.comm	buffer1,8,4
	.globl	seed2
	.align 4
	.type	seed2, @object
	.size	seed2, 4
seed2:
	.long	8
	.comm	buffer2,8,4
	.globl	seed3
	.align 4
	.type	seed3, @object
	.size	seed3, 4
seed3:
	.long	7
	.comm	buffer3,8,4
	.text
	.align 16
	.globl	Init
	.type	Init, @function
Init:
.LFB578:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE578:
	.size	Init, .-Init
	.align 16
	.globl	custom_memcpy
	.type	custom_memcpy, @function
custom_memcpy:
.LFB204:
	.cfi_startproc
	subl	$16, %esp
	.cfi_def_cfa_offset 20
	movl	24(%esp), %eax
	movl	%eax, 12(%esp)
	movl	20(%esp), %eax
	movl	%eax, 8(%esp)
	jmp	.L1171
.L1172:
	movl	8(%esp), %eax
	leal	4(%eax), %edx
	movl	%edx, 8(%esp)
	movl	12(%esp), %edx
	leal	4(%edx), %ecx
	movl	%ecx, 12(%esp)
	movl	(%edx), %edx
	movl	%edx, (%eax)
	subl	$1, 28(%esp)
.L1171:
	cmpl	$0, 28(%esp)
	jne	.L1172
	movl	20(%esp), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE204:
	.size	custom_memcpy, .-custom_memcpy
	.align 16
	.globl	RectD
	.type	RectD, @function
RectD:
.LFB209:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	movl	48(%esp), %eax
	addl	56(%esp), %eax
	cmpl	%eax, 48(%esp)
	movl	60(%esp), %esi
	movl	%eax, (%esp)
	jge	.L1188
	movl	depthVESA, %eax
	movl	44(%esp), %ecx
	addl	52(%esp), %ecx
	movl	48(%esp), %ebp
	leal	7(%eax), %ebx
	testl	%eax, %eax
	cmovns	%eax, %ebx
	movl	widthVESA, %eax
	sarl	$3, %ebx
	imull	%ebx, %eax
	movl	%eax, %edx
	movl	%eax, 4(%esp)
	movl	44(%esp), %eax
	imull	48(%esp), %edx
	imull	%ebx, %eax
	addl	%edx, %eax
	addl	buff, %eax
	movl	%eax, %edi
	.align 16
.L1176:
	cmpl	%ecx, 44(%esp)
	jge	.L1178
	movl	44(%esp), %eax
	movl	%edi, %edx
	.align 16
.L1177:
	addl	$1, %eax
	movl	%esi, (%edx)
	addl	%ebx, %edx
	cmpl	%ecx, %eax
	jne	.L1177
.L1178:
	addl	$1, %ebp
	addl	4(%esp), %edi
	cmpl	(%esp), %ebp
	jne	.L1176
.L1175:
	movl	cx0, %edx
	movl	%ecx, %eax
	movl	%edx, 8(%esp)
	movl	8(%esp), %edx
	cmpl	%edx, 44(%esp)
	jl	.L1179
	movl	8(%esp), %edi
	movl	%edi, 44(%esp)
.L1179:
	movl	44(%esp), %edi
	movl	%edi, cx0
	movl	cx1, %edx
	movl	%edx, 12(%esp)
	movl	12(%esp), %edx
	cmpl	%ecx, %edx
	jl	.L1180
	movl	12(%esp), %eax
.L1180:
	movl	%eax, cx1
	movl	cy0, %eax
	movl	%eax, 16(%esp)
	movl	16(%esp), %eax
	cmpl	%eax, 48(%esp)
	jl	.L1181
	movl	16(%esp), %eax
	movl	%eax, 48(%esp)
.L1181:
	movl	48(%esp), %eax
	movl	%eax, cy0
	movl	cy1, %eax
	movl	%eax, 20(%esp)
	movl	20(%esp), %eax
	cmpl	(%esp), %eax
	jl	.L1182
	movl	20(%esp), %eax
	movl	%eax, (%esp)
.L1182:
	movl	(%esp), %eax
	movl	%eax, cy1
	addl	$24, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L1188:
	.cfi_restore_state
	movl	44(%esp), %eax
	movl	52(%esp), %edi
	leal	(%eax,%edi), %ecx
	jmp	.L1175
	.cfi_endproc
.LFE209:
	.size	RectD, .-RectD
	.comm	dbuff_run,4,4
	.globl	xa
	.section	.bss
	.align 4
	.type	xa, @object
	.size	xa, 4
xa:
	.zero	4
	.globl	xb
	.align 4
	.type	xb, @object
	.size	xb, 4
xb:
	.zero	4
	.globl	buffer
	.data
	.type	buffer, @object
	.size	buffer, 1
buffer:
	.byte	90
	.text
	.align 16
	.globl	memcpy_rep2
	.type	memcpy_rep2, @function
memcpy_rep2:
.LFB210:
	.cfi_startproc
	movl	12(%esp), %ecx
	movl	8(%esp), %eax
	movl	4(%esp), %edx
/APP
/  98 "Source/graphics/graphics.c" 1
	mov %eax, %eax;                mov %eax, %edi;                mov %edx, %eax;                mov %eax, %esi;                mov %ecx, %ecx;                cld;                rep movsd;
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE210:
	.size	memcpy_rep2, .-memcpy_rep2
	.align 16
	.globl	DBuff
	.type	DBuff, @function
DBuff:
.LFB211:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$40, %esp
	.cfi_def_cfa_offset 48
	movl	buff, %eax
	movl	%eax, 28(%esp)
	movl	vga_mem, %eax
	movl	%eax, 24(%esp)
	movl	$0, 16(%esp)
	movl	depthVESA, %eax
	leal	7(%eax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$3, %eax
	movl	%eax, 12(%esp)
	call	Enable_SSE
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	jmp	.L1191
.L1192:
/APP
/  124 "Source/graphics/graphics.c" 1
	int $50
/  0 "" 2
/NO_APP
.L1191:
	movl	dbuff_run, %eax
	testl	%eax, %eax
	je	.L1192
/APP
/  126 "Source/graphics/graphics.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	$0, dbuff_run
	movl	cx0, %eax
	imull	12(%esp), %eax
	movl	%eax, %edx
	movl	cy0, %eax
	imull	12(%esp), %eax
	movl	widthVESA, %ecx
	imull	%ecx, %eax
	addl	%edx, %eax
	shrl	$2, %eax
	movl	%eax, 16(%esp)
	movl	cx0, %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, cx0
	movl	cx1, %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, cx1
	movl	buff, %eax
	movl	16(%esp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	%eax, 28(%esp)
	movl	vga_mem, %eax
	movl	16(%esp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	%eax, 24(%esp)
	movl	cy1, %edx
	movl	cy0, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, 20(%esp)
	jmp	.L1193
.L1194:
	movl	cx1, %edx
	movl	cx0, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	24(%esp), %eax
	movl	28(%esp), %edx
	movl	%edx, %ebx
/APP
/  163 "Source/graphics/graphics.c" 1
	mov %eax, %edi;                    mov %ebx, %esi;                    rep movsd;
/  0 "" 2
/NO_APP
	addl	$2048, 24(%esp)
	addl	$2048, 28(%esp)
	subl	$1, 20(%esp)
.L1193:
	cmpl	$0, 20(%esp)
	jne	.L1194
	movl	$512, cx0
	movl	$512, cx1
	movl	$384, cy0
	movl	$384, cy1
/APP
/  176 "Source/graphics/graphics.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L1191
	.cfi_endproc
.LFE211:
	.size	DBuff, .-DBuff
	.align 16
	.globl	RectL
	.type	RectL, @function
RectL:
.LFB212:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$48, %esp
	.cfi_def_cfa_offset 68
	movl	72(%esp), %eax
	movl	80(%esp), %ebx
	movl	76(%esp), %esi
	movl	84(%esp), %edi
	addl	%eax, %ebx
	cmpl	%ebx, %eax
	movl	%ebx, 16(%esp)
	jge	.L1196
	movl	depthVESA, %ecx
	movl	68(%esp), %ebx
	leal	7(%ecx), %edx
	addl	%esi, %ebx
	testl	%ecx, %ecx
	movl	%ebx, (%esp)
	movl	widthVESA, %ebx
	cmovns	%ecx, %edx
	movl	vesa_buff, %ecx
	sarl	$3, %edx
	movl	%edx, 4(%esp)
	movl	%ecx, 28(%esp)
	leal	-1(%esi), %ecx
	movl	4(%esp), %esi
	imull	%edx, %ebx
	movl	offset, %edx
	imull	%esi, %ecx
	movl	%ebx, %ebp
	movl	%ebx, 20(%esp)
	leal	1(%eax), %ebx
	movl	%ecx, 24(%esp)
	movl	68(%esp), %ecx
	imull	%ebp, %eax
	addl	24(%esp), %eax
	imull	%esi, %ecx
	addl	%ecx, %eax
	movl	%eax, 8(%esp)
	xorl	%eax, %eax
	.align 16
.L1198:
	movl	(%esp), %esi
	cmpl	68(%esp), %esi
	movl	%ebx, 12(%esp)
	jle	.L1206
	movl	8(%esp), %ecx
	subl	24(%esp), %ecx
	leal	-2(%ebx), %esi
	addl	28(%esp), %ecx
	movl	68(%esp), %eax
	.align 16
.L1203:
	movl	cx0, %ebp
	addl	$1, %eax
	movl	%edi, (%ecx)
	leal	-2(%eax), %edx
	movl	%ebp, 32(%esp)
	movl	32(%esp), %ebp
	cmpl	%edx, %ebp
	jg	.L1199
	movl	32(%esp), %edx
.L1199:
	movl	%edx, cx0
	movl	cx1, %edx
	movl	%edx, 36(%esp)
	movl	36(%esp), %ebp
	movl	%eax, %edx
	cmpl	%ebp, %eax
	jg	.L1200
	movl	36(%esp), %edx
.L1200:
	movl	%edx, cx1
	movl	cy0, %edx
	movl	%edx, 40(%esp)
	movl	40(%esp), %ebp
	movl	%esi, %edx
	cmpl	%esi, %ebp
	jg	.L1201
	movl	40(%esp), %edx
.L1201:
	movl	%edx, cy0
	movl	cy1, %edx
	movl	%edx, 44(%esp)
	movl	44(%esp), %ebp
	movl	%ebx, %edx
	cmpl	%ebp, %ebx
	jg	.L1202
	movl	44(%esp), %edx
.L1202:
	addl	4(%esp), %ecx
	cmpl	%eax, (%esp)
	movl	%edx, cy1
	jne	.L1203
	movl	8(%esp), %edx
	movl	$1, %eax
.L1206:
	movl	20(%esp), %ecx
	addl	$1, %ebx
	addl	%ecx, 8(%esp)
	movl	12(%esp), %ecx
	cmpl	%ecx, 16(%esp)
	jg	.L1198
	testb	%al, %al
	jne	.L1216
.L1196:
	addl	$48, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L1216:
	.cfi_restore_state
	movl	%edx, offset
	jmp	.L1196
	.cfi_endproc
.LFE212:
	.size	RectL, .-RectL
	.align 16
	.globl	Creater
	.type	Creater, @function
Creater:
.LFB213:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$16, %esp
	.cfi_def_cfa_offset 28
	movl	depthVESA, %ebx
	movl	cx0, %esi
	movl	28(%esp), %edx
	movl	32(%esp), %ecx
	leal	7(%ebx), %eax
	movl	%esi, (%esp)
	movl	(%esp), %esi
	addl	%edx, %ecx
	testl	%ebx, %ebx
	cmovns	%ebx, %eax
	movl	widthVESA, %ebx
	sarl	$3, %eax
	imull	%eax, %ebx
	imull	%edx, %eax
	imull	%ecx, %ebx
	addl	%ebx, %eax
	movl	vesa_buff, %ebx
	movl	%eax, offset
	movl	$1000, (%ebx,%eax)
	leal	1(%edx), %ebx
	subl	$1, %edx
	leal	1(%ecx), %eax
	subl	$1, %ecx
	cmpl	%esi, %edx
	jl	.L1218
	movl	(%esp), %edx
.L1218:
	movl	%edx, cx0
	movl	cx1, %edx
	movl	%edx, 4(%esp)
	movl	4(%esp), %edx
	cmpl	%edx, %ebx
	jg	.L1219
	movl	4(%esp), %ebx
.L1219:
	movl	%ebx, cx1
	movl	cy0, %edx
	movl	%edx, 8(%esp)
	movl	8(%esp), %edx
	cmpl	%edx, %ecx
	jl	.L1220
	movl	8(%esp), %ecx
.L1220:
	movl	%ecx, cy0
	movl	cy1, %edx
	movl	%edx, 12(%esp)
	movl	12(%esp), %edx
	cmpl	%edx, %eax
	jg	.L1221
	movl	12(%esp), %eax
.L1221:
	movl	%eax, cy1
	addl	$16, %esp
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE213:
	.size	Creater, .-Creater
	.align 16
	.globl	Func_Plot
	.type	Func_Plot, @function
Func_Plot:
.LFB214:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$68, %esp
	.cfi_def_cfa_offset 88
	movl	88(%esp), %esi
	movl	96(%esp), %ebx
	movl	92(%esp), %ecx
	movl	100(%esp), %edx
	cmpl	%ebx, %esi
	movl	%ebx, %eax
	cmovle	%esi, %eax
	cmovge	%esi, %ebx
	cmpl	%ebx, %eax
	movl	%ebx, 40(%esp)
	jg	.L1223
	cmpl	%edx, %ecx
	movl	%edx, %ebx
	cmovle	%ecx, %ebx
	cmovge	%ecx, %edx
	movl	depthVESA, %ecx
	movl	%edx, (%esp)
	movl	%ebx, %esi
	movl	%ebx, 28(%esp)
	movl	widthVESA, %ebx
	leal	7(%ecx), %edx
	testl	%ecx, %ecx
	cmovns	%ecx, %edx
	sarl	$3, %edx
	imull	%edx, %ebx
	movl	%edx, %ecx
	movl	%edx, 44(%esp)
	imull	%eax, %ecx
	movl	%ebx, %edx
	movl	%ebx, 8(%esp)
	movl	offset, %ebx
	imull	%esi, %edx
	movl	%ebx, 20(%esp)
	movl	vesa_buff, %ebx
	addl	%ecx, %edx
	movl	%edx, 36(%esp)
	movl	%ebx, 24(%esp)
	leal	1(%eax), %ebx
	notl	%eax
	movl	%eax, 48(%esp)
	movl	%ebx, 12(%esp)
	movl	%esi, %ebx
	xorl	%esi, %esi
	notl	%ebx
	.align 16
.L1234:
	movl	(%esp), %edx
	cmpl	%edx, 28(%esp)
	jg	.L1245
	movl	12(%esp), %ecx
	movl	48(%esp), %edx
	movl	28(%esp), %eax
	addl	%ecx, %edx
	movl	%ecx, 32(%esp)
	subl	$2, %ecx
	movl	%edx, 4(%esp)
	addl	$1, %eax
	movl	36(%esp), %edx
	movl	%ecx, 16(%esp)
	jmp	.L1233
	.align 16
.L1246:
	movl	%eax, %ecx
	addl	8(%esp), %edx
	addl	$1, %eax
	cmpl	(%esp), %ecx
	jg	.L1226
.L1233:
	leal	(%ebx,%eax), %ecx
	cmpl	4(%esp), %ecx
	jne	.L1246
	movl	cx0, %ebp
	movl	16(%esp), %edi
	movl	24(%esp), %esi
	movl	104(%esp), %ecx
	movl	%edx, 20(%esp)
	movl	%ebp, 52(%esp)
	movl	52(%esp), %ebp
	movl	%ecx, (%esi,%edx)
	movl	%eax, %ecx
	leal	-2(%eax), %esi
	cmpl	%edi, %ebp
	jg	.L1229
	movl	52(%esp), %edi
.L1229:
	movl	%edi, cx0
	movl	cx1, %edi
	movl	%edi, 56(%esp)
	movl	12(%esp), %edi
	movl	56(%esp), %ebp
	cmpl	%edi, %ebp
	jl	.L1230
	movl	56(%esp), %edi
.L1230:
	movl	%edi, cx1
	movl	cy0, %edi
	movl	%edi, 60(%esp)
	movl	60(%esp), %edi
	cmpl	%esi, %edi
	jg	.L1231
	movl	60(%esp), %esi
.L1231:
	movl	%esi, cy0
	movl	cy1, %esi
	movl	%esi, 64(%esp)
	movl	64(%esp), %edi
	movl	%eax, %esi
	cmpl	%edi, %eax
	jg	.L1232
	movl	64(%esp), %esi
.L1232:
	addl	$1, %eax
	addl	8(%esp), %edx
	cmpl	(%esp), %ecx
	movl	%esi, cy1
	movl	$1, %esi
	jle	.L1233
.L1226:
	movl	44(%esp), %edx
	addl	$1, 12(%esp)
	addl	%edx, 36(%esp)
	movl	40(%esp), %edx
	cmpl	%edx, 32(%esp)
	jle	.L1234
	movl	%esi, %eax
	testb	%al, %al
	jne	.L1247
.L1223:
	addl	$68, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L1245:
	.cfi_restore_state
	movl	12(%esp), %eax
	movl	%eax, 32(%esp)
	jmp	.L1226
.L1247:
	movl	20(%esp), %eax
	movl	%eax, offset
	jmp	.L1223
	.cfi_endproc
.LFE214:
	.size	Func_Plot, .-Func_Plot
	.align 16
	.globl	line_plot
	.type	line_plot, @function
line_plot:
.LFB215:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$172, %esp
	.cfi_def_cfa_offset 192
	movl	200(%esp), %edx
	movl	204(%esp), %eax
	subl	192(%esp), %edx
	subl	196(%esp), %eax
	movl	%edx, %edi
	movl	%edx, %ecx
	cltd
	orl	$1, %edx
	sarl	$31, %ecx
	orl	$1, %ecx
	movl	%edx, %esi
	movl	%edx, 60(%esp)
	movl	%edi, %edx
	movl	%ecx, 56(%esp)
	imull	%ecx, %edx
	imull	%esi, %eax
	leal	1(%edx), %ecx
	addl	$1, %eax
	cmpl	%eax, %ecx
	movl	%ecx, 24(%esp)
	movl	%eax, 32(%esp)
	jge	.L1251
	testl	%eax, %eax
	jle	.L1248
	movl	depthVESA, %edx
	movl	%esi, %ecx
	movl	$0, 20(%esp)
	movl	$0, 36(%esp)
	leal	7(%edx), %eax
	testl	%edx, %edx
	cmovns	%edx, %eax
	movl	buff, %edx
	sarl	$3, %eax
	movl	%eax, %esi
	movl	%eax, 12(%esp)
	imull	widthVESA, %eax
	movl	%edx, 48(%esp)
	movzwl	208(%esp), %edx
	movw	%dx, (%esp)
	movl	%ecx, %edx
	imull	%eax, %edx
	imull	196(%esp), %eax
	movl	%edx, 64(%esp)
	movl	%eax, 40(%esp)
	movl	196(%esp), %eax
	leal	1(%eax), %edi
	movl	%esi, %eax
	addl	%eax, %eax
	movl	%eax, 68(%esp)
	movl	212(%esp), %eax
	movl	%edi, %esi
	subl	$1, %eax
	movl	%eax, 72(%esp)
	.align 16
.L1253:
	cmpl	$1, 212(%esp)
	movl	%esi, 44(%esp)
	leal	-2(%esi), %eax
	jle	.L1304
	movl	%eax, %edi
	movl	%eax, 28(%esp)
	movl	192(%esp), %eax
	movl	40(%esp), %ebx
	movl	48(%esp), %ebp
	leal	-1(%eax), %edx
	movl	%edx, 52(%esp)
	imull	12(%esp), %edx
	movl	%edx, %ecx
	movl	%ebx, %edx
	addl	%ecx, %edx
	addl	68(%esp), %ecx
	addl	%ebp, %edx
	movl	%edx, 4(%esp)
	movl	%eax, %edx
	subl	$2, %edx
	addl	%ebx, %ecx
	movl	72(%esp), %ebx
	addl	%ebp, %ecx
	movl	%ecx, 8(%esp)
	movl	%edi, %ecx
	addl	%eax, %ebx
	movl	%ebx, 16(%esp)
	.align 16
.L1280:
	movl	cx0, %ebp
	leal	2(%eax), %ebx
	movl	%eax, %edi
	movl	%ebp, 140(%esp)
	movl	140(%esp), %ebp
	cmpl	%ebp, %eax
	jl	.L1272
	movl	140(%esp), %edi
.L1272:
	movl	%edi, cx0
	movl	cx1, %edi
	movl	%edi, 144(%esp)
	movl	144(%esp), %edi
	cmpl	%ebx, %edi
	jl	.L1273
	movl	144(%esp), %ebx
.L1273:
	movl	%ebx, cx1
	movl	cy0, %ebx
	movl	%ebx, 148(%esp)
	movl	148(%esp), %edi
	movl	%ecx, %ebx
	cmpl	%ecx, %edi
	jg	.L1274
	movl	148(%esp), %ebx
.L1274:
	movl	%ebx, cy0
	movl	cy1, %ebx
	movl	%ebx, 152(%esp)
	movl	152(%esp), %edi
	movl	%esi, %ebx
	cmpl	%esi, %edi
	jl	.L1275
	movl	152(%esp), %ebx
.L1275:
	movl	%ebx, cy1
	movl	cx0, %ebp
	movzwl	(%esp), %ebx
	movl	8(%esp), %edi
	movl	%ebp, 124(%esp)
	movl	124(%esp), %ebp
	movw	%bx, (%edi)
	leal	2(%edx), %ebx
	movl	%edx, %edi
	cmpl	%ebp, %edx
	jl	.L1276
	movl	124(%esp), %edi
.L1276:
	movl	%edi, cx0
	movl	cx1, %edi
	movl	%edi, 128(%esp)
	movl	128(%esp), %edi
	cmpl	%ebx, %edi
	jl	.L1277
	movl	128(%esp), %ebx
.L1277:
	movl	%ebx, cx1
	movl	cy0, %ebx
	movl	%ebx, 132(%esp)
	movl	132(%esp), %edi
	movl	%ecx, %ebx
	cmpl	%ecx, %edi
	jg	.L1278
	movl	132(%esp), %ebx
.L1278:
	movl	%ebx, cy0
	movl	cy1, %ebx
	movl	%ebx, 136(%esp)
	movl	136(%esp), %edi
	movl	%esi, %ebx
	cmpl	%esi, %edi
	jl	.L1279
	movl	136(%esp), %ebx
.L1279:
	movzwl	(%esp), %edi
	movl	%ebx, cy1
	addl	$1, %eax
	movl	4(%esp), %ebx
	subl	$1, %edx
	movw	%di, (%ebx)
	movl	12(%esp), %edi
	addl	%edi, 8(%esp)
	subl	%edi, %ebx
	cmpl	16(%esp), %eax
	movl	%ebx, 4(%esp)
	jne	.L1280
.L1287:
	movl	cx0, %ecx
	movl	12(%esp), %ebx
	movl	192(%esp), %eax
	movl	52(%esp), %edx
	imull	192(%esp), %ebx
	movl	%ecx, 156(%esp)
	movl	156(%esp), %ecx
	addl	$1, %eax
	addl	40(%esp), %ebx
	cmpl	%ecx, %edx
	jl	.L1281
	movl	156(%esp), %edx
.L1281:
	movl	%edx, cx0
	movl	cx1, %edx
	movl	%edx, 160(%esp)
	movl	160(%esp), %edx
	cmpl	%edx, %eax
	jg	.L1282
	movl	160(%esp), %eax
.L1282:
	movl	%eax, cx1
	movl	cy0, %eax
	movl	%eax, 164(%esp)
	movl	164(%esp), %eax
	cmpl	28(%esp), %eax
	jg	.L1283
	movl	164(%esp), %eax
	movl	%eax, 28(%esp)
.L1283:
	movl	28(%esp), %eax
	movl	%eax, cy0
	movl	cy1, %eax
	movl	%eax, 168(%esp)
	movl	168(%esp), %eax
	cmpl	%esi, %eax
	jl	.L1284
	movl	168(%esp), %eax
	movl	%eax, 44(%esp)
.L1284:
	movl	44(%esp), %eax
	movzwl	(%esp), %ecx
	movl	%eax, cy1
	movl	48(%esp), %eax
	movw	%cx, (%eax,%ebx)
	movl	24(%esp), %ecx
	addl	%ecx, 20(%esp)
	movl	32(%esp), %ecx
	movl	20(%esp), %eax
	cmpl	%eax, %ecx
	jg	.L1285
	subl	%ecx, %eax
	movl	%eax, 20(%esp)
	movl	56(%esp), %eax
	addl	%eax, 192(%esp)
.L1285:
	addl	$1, 36(%esp)
	movl	64(%esp), %edx
	movl	36(%esp), %eax
	addl	%edx, 40(%esp)
	addl	60(%esp), %esi
	cmpl	32(%esp), %eax
	jne	.L1253
.L1248:
	addl	$172, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L1251:
	.cfi_restore_state
	movl	24(%esp), %eax
	testl	%eax, %eax
	jle	.L1248
	movl	depthVESA, %edx
	movl	widthVESA, %ecx
	movl	$0, 36(%esp)
	movl	$0, 28(%esp)
	leal	7(%edx), %eax
	testl	%edx, %edx
	cmovns	%edx, %eax
	movl	buff, %edx
	sarl	$3, %eax
	imull	%eax, %ecx
	movl	%edx, 44(%esp)
	movzwl	208(%esp), %edx
	movl	%ecx, 16(%esp)
	movw	%dx, 4(%esp)
	movl	56(%esp), %edx
	imull	%eax, %edx
	imull	192(%esp), %eax
	movl	%edx, 48(%esp)
	movl	%eax, 40(%esp)
	movl	192(%esp), %eax
	leal	-1(%eax), %edi
	leal	(%ecx,%ecx), %eax
	movl	%eax, 52(%esp)
	movl	$1, %eax
	subl	212(%esp), %eax
	movl	%edi, %ecx
	movl	%eax, 64(%esp)
	.align 16
.L1271:
	movl	16(%esp), %esi
	movl	40(%esp), %eax
	imull	196(%esp), %esi
	movl	cx0, %edx
	movl	%edx, 76(%esp)
	movl	76(%esp), %ebp
	movl	%ecx, %edx
	leal	(%eax,%esi), %edi
	movl	196(%esp), %eax
	leal	1(%eax), %ebx
	leal	2(%ecx), %eax
	movl	%eax, (%esp)
	movl	196(%esp), %eax
	subl	$1, %eax
	cmpl	%ebp, %ecx
	jl	.L1254
	movl	76(%esp), %edx
.L1254:
	movl	%edx, cx0
	movl	cx1, %edx
	movl	%edx, 80(%esp)
	movl	80(%esp), %ebp
	leal	2(%ecx), %edx
	cmpl	(%esp), %ebp
	jl	.L1255
	movl	80(%esp), %edx
.L1255:
	movl	%edx, cx1
	movl	cy0, %edx
	movl	%edx, 84(%esp)
	movl	84(%esp), %edx
	cmpl	%edx, %eax
	jl	.L1256
	movl	84(%esp), %eax
.L1256:
	movl	%eax, cy0
	movl	cy1, %eax
	movl	%eax, 88(%esp)
	movl	88(%esp), %eax
	cmpl	%eax, %ebx
	jg	.L1257
	movl	88(%esp), %ebx
.L1257:
	movzwl	4(%esp), %edx
	cmpl	$1, 212(%esp)
	movl	44(%esp), %eax
	movl	%ebx, cy1
	movw	%dx, (%eax,%edi)
	jle	.L1270
	subl	16(%esp), %esi
	movl	40(%esp), %edi
	movl	44(%esp), %ebx
	movl	196(%esp), %eax
	movl	%edi, %edx
	addl	%esi, %edx
	addl	52(%esp), %esi
	addl	%ebx, %edx
	movl	%edx, 12(%esp)
	movl	%edi, %edx
	addl	%esi, %edx
	addl	%ebx, %edx
	movl	%edx, 8(%esp)
	movl	64(%esp), %edx
	addl	%eax, %edx
	movl	%edx, 20(%esp)
	movl	%eax, %edx
	.align 16
.L1269:
	movl	cx0, %edi
	leal	2(%edx), %ebx
	movl	%edx, %esi
	movl	%edi, 108(%esp)
	movl	108(%esp), %ebp
	movl	%ecx, %edi
	cmpl	%ebp, %ecx
	jl	.L1261
	movl	108(%esp), %edi
.L1261:
	movl	%edi, cx0
	movl	cx1, %edi
	movl	%edi, 112(%esp)
	movl	(%esp), %edi
	movl	112(%esp), %ebp
	cmpl	%edi, %ebp
	jl	.L1262
	movl	112(%esp), %edi
.L1262:
	movl	%edi, cx1
	movl	cy0, %edi
	movl	%edi, 116(%esp)
	movl	116(%esp), %edi
	cmpl	%edi, %edx
	jl	.L1263
	movl	116(%esp), %esi
.L1263:
	movl	%esi, cy0
	movl	cy1, %esi
	movl	%esi, 120(%esp)
	movl	120(%esp), %esi
	cmpl	%esi, %ebx
	jg	.L1264
	movl	120(%esp), %ebx
.L1264:
	movzwl	4(%esp), %edi
	movl	8(%esp), %esi
	movl	%ebx, cy1
	leal	-2(%eax), %ebx
	movw	%di, (%esi)
	movl	cx0, %edi
	movl	%eax, %esi
	movl	%edi, 92(%esp)
	movl	92(%esp), %ebp
	movl	%ecx, %edi
	cmpl	%ebp, %ecx
	jl	.L1265
	movl	92(%esp), %edi
.L1265:
	movl	%edi, cx0
	movl	cx1, %edi
	movl	%edi, 96(%esp)
	movl	(%esp), %edi
	movl	96(%esp), %ebp
	cmpl	%edi, %ebp
	jl	.L1266
	movl	96(%esp), %edi
.L1266:
	movl	%edi, cx1
	movl	cy0, %edi
	movl	%edi, 100(%esp)
	movl	100(%esp), %edi
	cmpl	%edi, %ebx
	jl	.L1267
	movl	100(%esp), %ebx
.L1267:
	movl	%ebx, cy0
	movl	cy1, %ebx
	movl	%ebx, 104(%esp)
	movl	104(%esp), %ebx
	cmpl	%ebx, %eax
	jg	.L1268
	movl	104(%esp), %esi
.L1268:
	movzwl	4(%esp), %ebx
	movl	%esi, cy1
	subl	$1, %eax
	movl	12(%esp), %esi
	addl	$1, %edx
	movw	%bx, (%esi)
	movl	16(%esp), %ebx
	addl	%ebx, 8(%esp)
	subl	%ebx, %esi
	cmpl	%eax, 20(%esp)
	movl	%esi, 12(%esp)
	jne	.L1269
.L1270:
	movl	32(%esp), %edx
	addl	%edx, 28(%esp)
	movl	28(%esp), %eax
	cmpl	%eax, 24(%esp)
	jg	.L1260
	movl	24(%esp), %edx
	movl	60(%esp), %eax
	subl	%edx, 28(%esp)
	addl	%eax, 196(%esp)
.L1260:
	movl	48(%esp), %esi
	addl	56(%esp), %ecx
	addl	$1, 36(%esp)
	addl	%esi, 40(%esp)
	movl	36(%esp), %eax
	cmpl	24(%esp), %eax
	jne	.L1271
	addl	$172, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L1304:
	.cfi_restore_state
	movl	%eax, 28(%esp)
	movl	192(%esp), %eax
	subl	$1, %eax
	movl	%eax, 52(%esp)
	jmp	.L1287
	.cfi_endproc
.LFE215:
	.size	line_plot, .-line_plot
	.align 16
	.globl	line_fast
	.type	line_fast, @function
line_fast:
.LFB216:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	xorl	%ebx, %ebx
	subl	$172, %esp
	.cfi_def_cfa_offset 192
	movl	200(%esp), %eax
	subl	192(%esp), %eax
	movl	204(%esp), %edx
	subl	196(%esp), %edx
	movl	%eax, %ecx
	sarl	$31, %ecx
	movl	%ecx, %esi
	xorl	%eax, %esi
	subl	%ecx, %esi
	movl	%edx, %ecx
	sarl	$31, %ecx
	movl	%esi, 32(%esp)
	movl	%ecx, %edi
	xorl	%edx, %edi
	subl	%ecx, %edi
	cmpl	$0, %eax
	movl	$-1, %ecx
	setg	%bl
	testl	%eax, %eax
	movl	%edi, 28(%esp)
	cmovns	%ebx, %ecx
	movl	%ecx, %ebx
	movl	%ecx, 60(%esp)
	xorl	%ecx, %ecx
	cmpl	$0, %edx
	setg	%cl
	testl	%edx, %edx
	movl	$-1, %edx
	cmovns	%ecx, %edx
	cmpl	%edi, %esi
	movl	%edx, 56(%esp)
	jl	.L1308
	testl	%eax, %eax
	je	.L1305
	movl	depthVESA, %edx
	sarl	%esi
	movl	widthVESA, %ecx
	movl	%esi, 24(%esp)
	movl	$0, 36(%esp)
	leal	7(%edx), %eax
	testl	%edx, %edx
	cmovns	%edx, %eax
	movl	192(%esp), %edx
	sarl	$3, %eax
	imull	%eax, %ecx
	addl	%ebx, %edx
	leal	1(%edx), %edi
	movl	%ecx, %esi
	movl	%ecx, 16(%esp)
	movl	buff, %ecx
	imull	%eax, %edx
	movl	%ecx, 44(%esp)
	movzwl	208(%esp), %ecx
	movl	%edx, 40(%esp)
	movw	%cx, 4(%esp)
	movl	%ebx, %ecx
	imull	%eax, %ecx
	leal	(%esi,%esi), %eax
	movl	%eax, 52(%esp)
	movl	$1, %eax
	subl	212(%esp), %eax
	movl	%ecx, 48(%esp)
	movl	%edi, %ecx
	movl	%eax, 64(%esp)
	.align 16
.L1316:
	movl	28(%esp), %edx
	addl	%edx, 24(%esp)
	movl	24(%esp), %eax
	movl	32(%esp), %edx
	cmpl	%eax, %edx
	jg	.L1310
	subl	%edx, %eax
	movl	%eax, 24(%esp)
	movl	56(%esp), %eax
	addl	%eax, 196(%esp)
.L1310:
	movl	16(%esp), %esi
	leal	-2(%ecx), %edx
	movl	40(%esp), %eax
	imull	196(%esp), %esi
	movl	%edx, (%esp)
	movl	cx0, %edx
	movl	%edx, 76(%esp)
	movl	76(%esp), %ebp
	leal	-2(%ecx), %edx
	leal	(%eax,%esi), %edi
	movl	196(%esp), %eax
	leal	1(%eax), %ebx
	subl	$1, %eax
	cmpl	%edx, %ebp
	jg	.L1311
	movl	76(%esp), %edx
.L1311:
	movl	%edx, cx0
	movl	cx1, %edx
	movl	%edx, 80(%esp)
	movl	80(%esp), %ebp
	movl	%ecx, %edx
	cmpl	%ebp, %ecx
	jg	.L1312
	movl	80(%esp), %edx
.L1312:
	movl	%edx, cx1
	movl	cy0, %edx
	movl	%edx, 84(%esp)
	movl	84(%esp), %edx
	cmpl	%edx, %eax
	jl	.L1313
	movl	84(%esp), %eax
.L1313:
	movl	%eax, cy0
	movl	cy1, %eax
	movl	%eax, 88(%esp)
	movl	88(%esp), %eax
	cmpl	%eax, %ebx
	jg	.L1314
	movl	88(%esp), %ebx
.L1314:
	movzwl	4(%esp), %edx
	cmpl	$1, 212(%esp)
	movl	44(%esp), %eax
	movl	%ebx, cy1
	movw	%dx, (%eax,%edi)
	jle	.L1327
	subl	16(%esp), %esi
	movl	40(%esp), %edi
	movl	44(%esp), %ebx
	movl	196(%esp), %eax
	movl	%edi, %edx
	addl	%esi, %edx
	addl	52(%esp), %esi
	addl	%ebx, %edx
	movl	%edx, 12(%esp)
	movl	%edi, %edx
	addl	%esi, %edx
	addl	%ebx, %edx
	movl	%edx, 8(%esp)
	movl	64(%esp), %edx
	addl	%eax, %edx
	movl	%edx, 20(%esp)
	movl	%eax, %edx
	.align 16
.L1326:
	movl	cx0, %edi
	leal	2(%edx), %ebx
	movl	%edx, %esi
	movl	%edi, 108(%esp)
	movl	(%esp), %edi
	movl	108(%esp), %ebp
	cmpl	%edi, %ebp
	jg	.L1318
	movl	108(%esp), %edi
.L1318:
	movl	%edi, cx0
	movl	cx1, %edi
	movl	%edi, 112(%esp)
	movl	112(%esp), %ebp
	movl	%ecx, %edi
	cmpl	%ebp, %ecx
	jg	.L1319
	movl	112(%esp), %edi
.L1319:
	movl	%edi, cx1
	movl	cy0, %edi
	movl	%edi, 116(%esp)
	movl	116(%esp), %edi
	cmpl	%edi, %edx
	jl	.L1320
	movl	116(%esp), %esi
.L1320:
	movl	%esi, cy0
	movl	cy1, %esi
	movl	%esi, 120(%esp)
	movl	120(%esp), %esi
	cmpl	%esi, %ebx
	jg	.L1321
	movl	120(%esp), %ebx
.L1321:
	movzwl	4(%esp), %edi
	movl	8(%esp), %esi
	movl	%ebx, cy1
	leal	-2(%eax), %ebx
	movw	%di, (%esi)
	movl	cx0, %edi
	movl	%eax, %esi
	movl	%edi, 92(%esp)
	movl	(%esp), %edi
	movl	92(%esp), %ebp
	cmpl	%edi, %ebp
	jg	.L1322
	movl	92(%esp), %edi
.L1322:
	movl	%edi, cx0
	movl	cx1, %edi
	movl	%edi, 96(%esp)
	movl	96(%esp), %ebp
	movl	%ecx, %edi
	cmpl	%ebp, %ecx
	jg	.L1323
	movl	96(%esp), %edi
.L1323:
	movl	%edi, cx1
	movl	cy0, %edi
	movl	%edi, 100(%esp)
	movl	100(%esp), %edi
	cmpl	%edi, %ebx
	jl	.L1324
	movl	100(%esp), %ebx
.L1324:
	movl	%ebx, cy0
	movl	cy1, %ebx
	movl	%ebx, 104(%esp)
	movl	104(%esp), %ebx
	cmpl	%ebx, %eax
	jg	.L1325
	movl	104(%esp), %esi
.L1325:
	movzwl	4(%esp), %ebx
	movl	%esi, cy1
	subl	$1, %eax
	movl	12(%esp), %esi
	addl	$1, %edx
	movw	%bx, (%esi)
	movl	16(%esp), %ebx
	addl	%ebx, 8(%esp)
	subl	%ebx, %esi
	cmpl	%eax, 20(%esp)
	movl	%esi, 12(%esp)
	jne	.L1326
.L1327:
	addl	$1, 36(%esp)
	addl	60(%esp), %ecx
	movl	36(%esp), %eax
	movl	48(%esp), %esi
	addl	%esi, 40(%esp)
	cmpl	%eax, 32(%esp)
	jne	.L1316
.L1305:
	addl	$172, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L1308:
	.cfi_restore_state
	movl	28(%esp), %eax
	movl	depthVESA, %edx
	movl	buff, %ecx
	movl	56(%esp), %esi
	movl	$0, 36(%esp)
	sarl	%eax
	testl	%edx, %edx
	movl	%eax, 20(%esp)
	leal	7(%edx), %eax
	movl	%ecx, 48(%esp)
	movzwl	208(%esp), %ecx
	cmovns	%edx, %eax
	movl	widthVESA, %edx
	sarl	$3, %eax
	movl	%eax, 12(%esp)
	imull	%eax, %edx
	movw	%cx, (%esp)
	movl	%eax, %ecx
	movl	%esi, %eax
	imull	%edx, %eax
	movl	%eax, 64(%esp)
	movl	196(%esp), %eax
	addl	%esi, %eax
	imull	%eax, %edx
	leal	1(%eax), %edi
	leal	(%ecx,%ecx), %eax
	movl	%eax, 68(%esp)
	movl	212(%esp), %eax
	movl	%edi, %esi
	movl	%edx, 40(%esp)
	subl	$1, %eax
	movl	%eax, 72(%esp)
	.align 16
.L1344:
	movl	32(%esp), %ecx
	addl	%ecx, 20(%esp)
	movl	20(%esp), %eax
	movl	28(%esp), %ecx
	cmpl	%eax, %ecx
	jg	.L1328
	subl	%ecx, %eax
	movl	%eax, 20(%esp)
	movl	60(%esp), %eax
	addl	%eax, 192(%esp)
.L1328:
	cmpl	$1, 212(%esp)
	movl	%esi, 44(%esp)
	leal	-2(%esi), %eax
	jle	.L1362
	movl	%eax, %edi
	movl	%eax, 24(%esp)
	movl	192(%esp), %eax
	movl	40(%esp), %ebx
	movl	48(%esp), %ebp
	leal	-1(%eax), %edx
	movl	%edx, 52(%esp)
	imull	12(%esp), %edx
	movl	%edx, %ecx
	movl	%ebx, %edx
	addl	%ecx, %edx
	addl	68(%esp), %ecx
	addl	%ebp, %edx
	movl	%edx, 4(%esp)
	movl	%eax, %edx
	subl	$2, %edx
	addl	%ebx, %ecx
	movl	72(%esp), %ebx
	addl	%ebp, %ecx
	movl	%ecx, 8(%esp)
	movl	%edi, %ecx
	addl	%eax, %ebx
	movl	%ebx, 16(%esp)
	.align 16
.L1339:
	movl	cx0, %ebp
	leal	2(%eax), %ebx
	movl	%eax, %edi
	movl	%ebp, 140(%esp)
	movl	140(%esp), %ebp
	cmpl	%ebp, %eax
	jl	.L1331
	movl	140(%esp), %edi
.L1331:
	movl	%edi, cx0
	movl	cx1, %edi
	movl	%edi, 144(%esp)
	movl	144(%esp), %edi
	cmpl	%ebx, %edi
	jl	.L1332
	movl	144(%esp), %ebx
.L1332:
	movl	%ebx, cx1
	movl	cy0, %ebx
	movl	%ebx, 148(%esp)
	movl	148(%esp), %edi
	movl	%ecx, %ebx
	cmpl	%ecx, %edi
	jg	.L1333
	movl	148(%esp), %ebx
.L1333:
	movl	%ebx, cy0
	movl	cy1, %ebx
	movl	%ebx, 152(%esp)
	movl	152(%esp), %edi
	movl	%esi, %ebx
	cmpl	%esi, %edi
	jl	.L1334
	movl	152(%esp), %ebx
.L1334:
	movl	%ebx, cy1
	movl	cx0, %ebp
	movzwl	(%esp), %ebx
	movl	8(%esp), %edi
	movl	%ebp, 124(%esp)
	movl	124(%esp), %ebp
	movw	%bx, (%edi)
	leal	2(%edx), %ebx
	movl	%edx, %edi
	cmpl	%ebp, %edx
	jl	.L1335
	movl	124(%esp), %edi
.L1335:
	movl	%edi, cx0
	movl	cx1, %edi
	movl	%edi, 128(%esp)
	movl	128(%esp), %edi
	cmpl	%ebx, %edi
	jl	.L1336
	movl	128(%esp), %ebx
.L1336:
	movl	%ebx, cx1
	movl	cy0, %ebx
	movl	%ebx, 132(%esp)
	movl	132(%esp), %edi
	movl	%ecx, %ebx
	cmpl	%ecx, %edi
	jg	.L1337
	movl	132(%esp), %ebx
.L1337:
	movl	%ebx, cy0
	movl	cy1, %ebx
	movl	%ebx, 136(%esp)
	movl	136(%esp), %edi
	movl	%esi, %ebx
	cmpl	%esi, %edi
	jl	.L1338
	movl	136(%esp), %ebx
.L1338:
	movzwl	(%esp), %edi
	movl	%ebx, cy1
	addl	$1, %eax
	movl	4(%esp), %ebx
	subl	$1, %edx
	movw	%di, (%ebx)
	movl	12(%esp), %edi
	addl	%edi, 8(%esp)
	subl	%edi, %ebx
	cmpl	%eax, 16(%esp)
	movl	%ebx, 4(%esp)
	jne	.L1339
.L1330:
	movl	cx0, %ecx
	movl	12(%esp), %ebx
	movl	192(%esp), %eax
	movl	52(%esp), %edx
	imull	192(%esp), %ebx
	movl	%ecx, 156(%esp)
	movl	156(%esp), %ecx
	addl	$1, %eax
	addl	40(%esp), %ebx
	cmpl	%ecx, %edx
	jl	.L1340
	movl	156(%esp), %edx
.L1340:
	movl	%edx, cx0
	movl	cx1, %edx
	movl	%edx, 160(%esp)
	movl	160(%esp), %edx
	cmpl	%edx, %eax
	jg	.L1341
	movl	160(%esp), %eax
.L1341:
	movl	%eax, cx1
	movl	cy0, %eax
	movl	%eax, 164(%esp)
	movl	164(%esp), %eax
	cmpl	24(%esp), %eax
	jg	.L1342
	movl	164(%esp), %eax
	movl	%eax, 24(%esp)
.L1342:
	movl	24(%esp), %eax
	movl	%eax, cy0
	movl	cy1, %eax
	movl	%eax, 168(%esp)
	movl	168(%esp), %eax
	cmpl	%esi, %eax
	jl	.L1343
	movl	168(%esp), %eax
	movl	%eax, 44(%esp)
.L1343:
	movl	44(%esp), %eax
	movzwl	(%esp), %ecx
	addl	$1, 36(%esp)
	movl	64(%esp), %edx
	addl	56(%esp), %esi
	addl	%edx, 40(%esp)
	movl	%eax, cy1
	movl	48(%esp), %eax
	movw	%cx, (%eax,%ebx)
	movl	36(%esp), %eax
	cmpl	%eax, 28(%esp)
	jne	.L1344
	addl	$172, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L1362:
	.cfi_restore_state
	movl	%eax, 24(%esp)
	movl	192(%esp), %eax
	subl	$1, %eax
	movl	%eax, 52(%esp)
	jmp	.L1330
	.cfi_endproc
.LFE216:
	.size	line_fast, .-line_fast
	.align 16
	.globl	Mouse_Plot
	.type	Mouse_Plot, @function
Mouse_Plot:
.LFB217:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$64, %esp
	.cfi_def_cfa_offset 84
	movl	depthVESA, %edx
	leal	7(%edx), %eax
	testl	%edx, %edx
	cmovns	%edx, %eax
	sarl	$3, %eax
	movl	%eax, 4(%esp)
.L1388:
	call	mouse_handler
/APP
/  371 "Source/graphics/graphics.c" 1
	int $50
/  0 "" 2
/  372 "Source/graphics/graphics.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	widthVESA, %eax
	movl	mouse_buff, %edi
	movl	$-2, %esi
	movl	$20, %ebx
	movl	%eax, 12(%esp)
	movl	buff, %eax
	movl	%eax, (%esp)
	.align 16
.L1366:
	movl	mousey, %edx
	movl	mousex, %eax
	addl	%ebx, %edx
	imull	12(%esp), %edx
	addl	%eax, %edx
	imull	4(%esp), %edx
	leal	(%edi,%edx), %ecx
	addl	(%esp), %edx
	cmpl	$12, %ebx
	jg	.L1364
	leal	(%ebx,%ebx), %ebp
	xorl	%eax, %eax
	movl	%ebx, 8(%esp)
	.align 16
.L1365:
	movzwl	(%ecx,%eax), %ebx
	movw	%bx, (%edx,%eax)
	addl	$2, %eax
	cmpl	%ebp, %eax
	jne	.L1365
.L1402:
	movl	8(%esp), %ebx
.L1368:
	addl	$1, %esi
	subl	$1, %ebx
	jne	.L1366
	movl	mousey, %esi
	movl	mousex, %eax
	movl	mousey, %ebx
	movl	mousex, %edx
	movl	cx0, %ecx
	addl	$25, %esi
	addl	$25, %eax
	subl	$5, %edx
	subl	$5, %ebx
	movl	%ecx, 32(%esp)
	movl	32(%esp), %ecx
	cmpl	%ecx, %edx
	jl	.L1370
	movl	32(%esp), %edx
.L1370:
	movl	%edx, cx0
	movl	cx1, %edx
	movl	%edx, 36(%esp)
	movl	36(%esp), %edx
	cmpl	%edx, %eax
	jg	.L1371
	movl	36(%esp), %eax
.L1371:
	movl	%eax, cx1
	movl	cy0, %eax
	movl	%eax, 40(%esp)
	movl	40(%esp), %eax
	cmpl	%eax, %ebx
	jl	.L1372
	movl	40(%esp), %ebx
.L1372:
	movl	%ebx, cy0
	movl	cy1, %eax
	movl	%eax, 44(%esp)
	movl	44(%esp), %eax
	cmpl	%eax, %esi
	jg	.L1373
	movl	44(%esp), %esi
.L1373:
	movl	%esi, cy1
	movl	mousedeltax, %eax
	movl	mousex, %edx
	addl	%edx, %eax
	movl	%eax, mousex
	movl	mousedeltay, %edx
	movl	mousey, %eax
	subl	%edx, %eax
	movl	%eax, mousey
	movl	mousex, %eax
	movl	%eax, 20(%esp)
	movl	20(%esp), %edx
	xorl	%eax, %eax
	testl	%edx, %edx
	js	.L1374
	movl	20(%esp), %eax
.L1374:
	movl	%eax, 16(%esp)
	movl	12(%esp), %esi
	movl	16(%esp), %edx
	movl	%esi, %eax
	cmpl	%edx, %esi
	jl	.L1375
	movl	16(%esp), %eax
.L1375:
	movl	%eax, mousex
	movl	mousey, %eax
	movl	%eax, 28(%esp)
	movl	28(%esp), %edx
	xorl	%eax, %eax
	testl	%edx, %edx
	js	.L1376
	movl	28(%esp), %eax
.L1376:
	movl	%eax, 24(%esp)
	movl	heightVESA, %eax
	movl	24(%esp), %edx
	cmpl	%edx, %eax
	jl	.L1377
	movl	24(%esp), %eax
.L1377:
	movl	%eax, mousey
	movl	mousey, %esi
	movl	mousex, %eax
	movl	mousey, %ebx
	movl	mousex, %edx
	movl	cx0, %ecx
	addl	$25, %esi
	addl	$25, %eax
	subl	$5, %ebx
	movl	%ecx, 48(%esp)
	movl	48(%esp), %ecx
	subl	$5, %edx
	cmpl	%ecx, %edx
	jl	.L1378
	movl	48(%esp), %edx
.L1378:
	movl	%edx, cx0
	movl	cx1, %edx
	movl	%edx, 52(%esp)
	movl	52(%esp), %edx
	cmpl	%edx, %eax
	jg	.L1379
	movl	52(%esp), %eax
.L1379:
	movl	%eax, cx1
	movl	cy0, %eax
	movl	%eax, 56(%esp)
	movl	56(%esp), %eax
	cmpl	%eax, %ebx
	jl	.L1380
	movl	56(%esp), %ebx
.L1380:
	movl	%ebx, cy0
	movl	cy1, %eax
	movl	%eax, 60(%esp)
	movl	60(%esp), %eax
	cmpl	%eax, %esi
	jg	.L1381
	movl	60(%esp), %esi
.L1381:
	movl	12(%esp), %ebp
	movl	%esi, cy1
	movl	$20, %ebx
	movl	$-2, %esi
	.align 16
.L1384:
	movl	mousey, %eax
	movl	mousex, %edx
	addl	%ebx, %eax
	imull	%ebp, %eax
	addl	%edx, %eax
	imull	4(%esp), %eax
	leal	(%edi,%eax), %edx
	addl	(%esp), %eax
	cmpl	$12, %ebx
	jg	.L1382
	leal	(%edx,%ebx,2), %ecx
	movl	%ebx, 8(%esp)
	.align 16
.L1383:
	movzwl	(%eax), %ebx
	addl	$2, %edx
	addl	$2, %eax
	cmpl	%edx, %ecx
	movw	%bx, -2(%edx)
	movl	$-1, %ebx
	movw	%bx, -2(%eax)
	jne	.L1383
.L1403:
	movl	8(%esp), %ebx
.L1386:
	addl	$1, %esi
	subl	$1, %ebx
	jne	.L1384
	movl	$0, mousedeltax
	movl	$0, mousedeltay
/APP
/  437 "Source/graphics/graphics.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L1388
	.align 16
.L1382:
	testl	%esi, %esi
	jle	.L1386
	leal	(%edx,%esi,2), %ecx
	movl	%ebx, 8(%esp)
	.align 16
.L1387:
	movzwl	(%eax), %ebx
	addl	$2, %edx
	addl	$2, %eax
	cmpl	%ecx, %edx
	movw	%bx, -2(%edx)
	movl	$-1, %ebx
	movw	%bx, -2(%eax)
	jne	.L1387
	jmp	.L1403
	.align 16
.L1364:
	testl	%esi, %esi
	jle	.L1368
	leal	(%esi,%esi), %ebp
	xorl	%eax, %eax
	movl	%ebx, 8(%esp)
	.align 16
.L1369:
	movzwl	(%ecx,%eax), %ebx
	movw	%bx, (%edx,%eax)
	addl	$2, %eax
	cmpl	%ebp, %eax
	jne	.L1369
	jmp	.L1402
	.cfi_endproc
.LFE217:
	.size	Mouse_Plot, .-Mouse_Plot
	.comm	prime_list,4,4
	.globl	len_plist
	.data
	.align 4
	.type	len_plist, @object
	.size	len_plist, 4
len_plist:
	.long	1
	.text
	.align 16
	.globl	is_prime
	.type	is_prime, @function
is_prime:
.LFB219:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	movl	len_plist, %edi
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	prime_list, %ebx
	movl	16(%esp), %esi
	testl	%edi, %edi
	je	.L1408
	xorl	%edx, %edx
	movl	%esi, %eax
	divl	(%ebx)
	testl	%edx, %edx
	je	.L1414
	xorl	%ecx, %ecx
	jmp	.L1411
	.align 16
.L1412:
	xorl	%edx, %edx
	movl	%esi, %eax
	divl	(%ebx,%ecx,4)
	testl	%edx, %edx
	je	.L1414
.L1411:
	addl	$1, %ecx
	cmpl	%edi, %ecx
	jne	.L1412
	sall	$2, %edi
.L1408:
	movl	%esi, (%ebx,%edi)
	movl	$1, %eax
	addl	$1, len_plist
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1414:
	.cfi_restore_state
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	xorl	%eax, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE219:
	.size	is_prime, .-is_prime
	.align 16
	.globl	prime_diff_graph
	.type	prime_diff_graph, @function
prime_diff_graph:
.LFB220:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	movl	$1, %edi
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	xorl	%esi, %esi
	movl	$3, %ebx
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	pushl	$8192
	.cfi_def_cfa_offset 48
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, prime_list
	movl	$2, (%eax)
	jmp	.L1418
	.align 16
.L1417:
	addl	$1, %ebx
	cmpl	$999, %esi
	ja	.L1424
.L1418:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	%ebx
	.cfi_def_cfa_offset 48
	call	is_prime
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L1417
	movl	%ebx, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	leal	1(%esi), %ebp
	subl	%edi, %eax
	movl	%ebx, %edi
	addl	$1, %ebx
	sall	$2, %eax
	pushl	%eax
	.cfi_def_cfa_offset 44
	pushl	%esi
	.cfi_def_cfa_offset 48
	movl	%ebp, %esi
	call	graph_plot
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	cmpl	$999, %esi
	jbe	.L1418
.L1424:
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE220:
	.size	prime_diff_graph, .-prime_diff_graph
	.align 16
	.globl	graph_plot
	.type	graph_plot, @function
graph_plot:
.LFB221:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	movl	$760, %eax
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	depthVESA, %ebx
	movl	cx0, %edi
	subl	36(%esp), %eax
	movl	32(%esp), %ecx
	leal	7(%ebx), %edx
	testl	%ebx, %ebx
	movl	%edi, (%esp)
	movl	(%esp), %edi
	leal	1(%ecx), %esi
	cmovns	%ebx, %edx
	movl	widthVESA, %ebx
	sarl	$3, %edx
	imull	%edx, %ebx
	imull	%ecx, %edx
	subl	$1, %ecx
	imull	%eax, %ebx
	addl	%ebx, %edx
	leal	1(%eax), %ebx
	subl	$1, %eax
	cmpl	%edi, %ecx
	jl	.L1426
	movl	(%esp), %ecx
.L1426:
	movl	%ecx, cx0
	movl	cx1, %ecx
	movl	%ecx, 4(%esp)
	movl	4(%esp), %ecx
	cmpl	%ecx, %esi
	jg	.L1427
	movl	4(%esp), %esi
.L1427:
	movl	%esi, cx1
	movl	cy0, %ecx
	movl	%ecx, 8(%esp)
	movl	8(%esp), %ecx
	cmpl	%ecx, %eax
	jl	.L1428
	movl	8(%esp), %eax
.L1428:
	movl	%eax, cy0
	movl	cy1, %eax
	movl	%eax, 12(%esp)
	movl	12(%esp), %eax
	cmpl	%eax, %ebx
	jg	.L1429
	movl	12(%esp), %ebx
.L1429:
	movl	buff, %eax
	movl	$-16, %ecx
	movl	%ebx, cy1
	movw	%cx, (%eax,%edx)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE221:
	.size	graph_plot, .-graph_plot
	.align 16
	.globl	random_plotter
	.type	random_plotter, @function
random_plotter:
.LFB222:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	xorl	%eax, %eax
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
/APP
/  37 "Library/sys.h" 1
	outb %al, $112
/  0 "" 2
/  45 "Library/sys.h" 1
	inb $113, %al
/  0 "" 2
/NO_APP
	movzbl	%al, %eax
	movl	$100000, %ebx
	movl	$274877907, %esi
	addl	$9, %eax
	movl	%eax, seed1
	movl	HPET_main_counter, %eax
	movl	(%eax), %eax
	leal	3(%eax,%eax,2), %eax
	movl	%eax, seed2
	movl	Lapic, %eax
	movl	912(%eax), %eax
	pushl	$4096
	.cfi_def_cfa_offset 48
	addl	$999, %eax
	movl	%eax, seed3
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, %edi
	.align 16
.L1432:
	call	random
	movl	%eax, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	mull	%esi
	shrl	$6, %edx
	imull	$1000, %edx, %edx
	subl	%edx, %ecx
	leal	(%edi,%ecx,4), %ebp
	movl	0(%ebp), %eax
	addl	$100, %eax
	pushl	%eax
	.cfi_def_cfa_offset 44
	pushl	%ecx
	.cfi_def_cfa_offset 48
	call	graph_plot
	addl	$1, 0(%ebp)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	subl	$1, %ebx
	jne	.L1432
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE222:
	.size	random_plotter, .-random_plotter
	.globl	next
	.section	.bss
	.align 4
	.type	next, @object
	.size	next, 4
next:
	.zero	4
	.comm	time,4,4
	.globl	ticks
	.data
	.align 4
	.type	ticks, @object
	.size	ticks, 4
ticks:
	.long	5
	.text
	.align 16
	.globl	rand
	.type	rand, @function
rand:
.LFB566:
	.cfi_startproc
	jmp	seeder
	.cfi_endproc
.LFE566:
	.size	rand, .-rand
	.align 16
	.globl	srand
	.type	srand, @function
srand:
.LFB224:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	next, %ecx
	movl	$175921861, %ebx
	addl	8(%esp), %ecx
	movl	%ecx, %edx
	shrl	$5, %edx
	movl	%edx, %eax
	mull	%ebx
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	shrl	$7, %edx
	imull	$100000, %edx, %edx
	subl	%edx, %ecx
	movl	%ecx, next
	ret
	.cfi_endproc
.LFE224:
	.size	srand, .-srand
	.align 16
	.globl	srandInit
	.type	srandInit, @function
srandInit:
.LFB225:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	xorl	%eax, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $112
/  0 "" 2
/  45 "Library/sys.h" 1
	inb $113, %al
/  0 "" 2
/NO_APP
	movzbl	%al, %ecx
	movl	next, %eax
	movl	$175921861, %ebx
	movl	%ecx, time
	addl	%eax, %ecx
	movl	%ecx, %edx
	shrl	$5, %edx
	movl	%edx, %eax
	mull	%ebx
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	shrl	$7, %edx
	imull	$100000, %edx, %edx
	subl	%edx, %ecx
	movl	%ecx, next
	ret
	.cfi_endproc
.LFE225:
	.size	srandInit, .-srandInit
	.align 16
	.globl	seeder
	.type	seeder, @function
seeder:
.LFB226:
	.cfi_startproc
	movl	next, %eax
	imull	$1103515245, %eax, %eax
	addl	$12345, %eax
	movl	%eax, next
	movl	next, %eax
	shrl	$16, %eax
	ret
	.cfi_endproc
.LFE226:
	.size	seeder, .-seeder
	.align 16
	.globl	init_timer_RTC
	.type	init_timer_RTC, @function
init_timer_RTC:
.LFB227:
	.cfi_startproc
/APP
/  9 "Source/Drivers/timer/timer.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	$-118, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $112
/  0 "" 2
/  45 "Library/sys.h" 1
	inb $113, %al
/  0 "" 2
/NO_APP
	movl	%eax, %edx
	movl	$-118, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $112
/  0 "" 2
/NO_APP
	movl	%edx, %eax
	andl	$-16, %eax
	orl	$5, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $113
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE227:
	.size	init_timer_RTC, .-init_timer_RTC
	.align 16
	.globl	apic_start_timer
	.type	apic_start_timer, @function
apic_start_timer:
.LFB228:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$12, %esp
	.cfi_def_cfa_offset 20
	movl	20(%esp), %ebx
	pushl	$3
	.cfi_def_cfa_offset 24
	pushl	$992
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	localapic_write
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	$-1
	.cfi_def_cfa_offset 24
	pushl	$896
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	localapic_write
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	$65536
	.cfi_def_cfa_offset 24
	pushl	$800
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	localapic_write
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	$51
	.cfi_def_cfa_offset 24
	pushl	$800
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	localapic_write
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	$3
	.cfi_def_cfa_offset 24
	pushl	$992
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	localapic_write
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	$1
	.cfi_def_cfa_offset 24
	pushl	$896
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	localapic_write
	addl	$24, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE228:
	.size	apic_start_timer, .-apic_start_timer
	.align 16
	.globl	init_timer
	.type	init_timer, @function
init_timer:
.LFB229:
	.cfi_startproc
/APP
/  58 "Source/Drivers/timer/timer.c" 1
	cli
/  0 "" 2
/NO_APP
	xorl	%edx, %edx
	movl	$1193180, %eax
	divl	4(%esp)
	movl	%eax, %ecx
	movl	$54, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $67
/  0 "" 2
/NO_APP
	movl	%ecx, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $64
/  0 "" 2
/NO_APP
	movl	%ecx, %eax
	shrl	$8, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $64
/  0 "" 2
/  45 "Library/sys.h" 1
	inb $33, %al
/  0 "" 2
/NO_APP
	andl	$-2, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE229:
	.size	init_timer, .-init_timer
	.globl	delay_dump
	.section	.bss
	.align 4
	.type	delay_dump, @object
	.size	delay_dump, 4
delay_dump:
	.zero	4
	.text
	.align 16
	.globl	delay1
	.type	delay1, @function
delay1:
.LFB230:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	xorl	%eax, %eax
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subl	$20, %esp
	.cfi_def_cfa_offset 36
/APP
/  37 "Library/sys.h" 1
	outb %al, $112
/  0 "" 2
/  45 "Library/sys.h" 1
	inb $113, %al
/  0 "" 2
/NO_APP
	movzbl	%al, %ecx
	movl	next, %eax
	movl	$175921861, %edx
	movl	%ecx, time
	movl	36(%esp), %ebx
	fldz
	movl	$175921861, %esi
	addl	%eax, %ecx
	movl	%ecx, %eax
	sall	$9, %ebx
	shrl	$5, %eax
	mull	%edx
	movl	%edx, %eax
	shrl	$7, %eax
	imull	$100000, %eax, %eax
	subl	%eax, %ecx
	movl	%ecx, next
	xorl	%ecx, %ecx
	testl	%ebx, %ebx
	fstps	16(%esp)
	je	.L1449
	.align 16
.L1450:
	movl	next, %eax
	flds	.LC218
	leal	100(%ecx,%eax), %edi
	movl	%edi, %edx
	shrl	$5, %edx
	movl	%edx, %eax
	mull	%esi
	shrl	$7, %edx
	imull	$100000, %edx, %edx
	subl	%edx, %edi
	movl	$500, %edx
	movl	%edi, next
	.align 16
.L1447:
	movl	next, %eax
	imull	$1103515245, %eax, %eax
	addl	$12345, %eax
	movl	%eax, next
	movl	next, %eax
	shrl	$16, %eax
	subl	$1, %edx
	movl	%eax, (%esp)
	fildl	(%esp)
	fstps	16(%esp)
	flds	16(%esp)
	fdiv	%st(1), %st
	fstps	16(%esp)
	jne	.L1447
	fstp	%st(0)
	addl	$1, %ecx
	cmpl	%ebx, %ecx
	jne	.L1450
.L1449:
	flds	16(%esp)
	flds	delay_dump
	fxch	%st(1)
	fdivs	.LC217
	faddp	%st, %st(1)
	fstps	delay_dump
	addl	$20, %esp
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE230:
	.size	delay1, .-delay1
	.align 16
	.globl	check_apic
	.type	check_apic, @function
check_apic:
.LFB231:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	$1, %eax
/APP
/  13 "Source/arch/x86/APIC/apic.c" 1
	cpuid
/  0 "" 2
/NO_APP
	movl	%edx, %eax
	shrl	$9, %eax
	andl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE231:
	.size	check_apic, .-check_apic
	.align 16
	.globl	cpu_get_apic_base
	.type	cpu_get_apic_base, @function
cpu_get_apic_base:
.LFB232:
	.cfi_startproc
	movl	$27, %ecx
/APP
/  25 "Source/arch/x86/cpu/cpu.c" 1
	rdmsr
/  0 "" 2
/NO_APP
	andl	$-4096, %eax
	ret
	.cfi_endproc
.LFE232:
	.size	cpu_get_apic_base, .-cpu_get_apic_base
	.align 16
	.globl	BSP_init_LAPIC
	.type	BSP_init_LAPIC, @function
BSP_init_LAPIC:
.LFB233:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	pushl	$31
	.cfi_def_cfa_offset 40
	pushl	$880
	.cfi_def_cfa_offset 44
	pushl	44(%esp)
	.cfi_def_cfa_offset 48
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	pushl	$0
	.cfi_def_cfa_offset 40
	pushl	$128
	.cfi_def_cfa_offset 44
	pushl	44(%esp)
	.cfi_def_cfa_offset 48
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	pushl	$-1
	.cfi_def_cfa_offset 40
	pushl	$224
	.cfi_def_cfa_offset 44
	pushl	44(%esp)
	.cfi_def_cfa_offset 48
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	pushl	$16777216
	.cfi_def_cfa_offset 40
	pushl	$208
	.cfi_def_cfa_offset 44
	pushl	44(%esp)
	.cfi_def_cfa_offset 48
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	pushl	$511
	.cfi_def_cfa_offset 40
	pushl	$240
	.cfi_def_cfa_offset 44
	pushl	44(%esp)
	.cfi_def_cfa_offset 48
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	32(%esp), %eax
	movl	%eax, Lapic
	movl	Lapic, %eax
	movl	48(%eax), %eax
	movl	%eax, 12(%esp)
	andl	$255, 12(%esp)
	nop
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE233:
	.size	BSP_init_LAPIC, .-BSP_init_LAPIC
	.align 16
	.globl	AP_init_LAPIC
	.type	AP_init_LAPIC, @function
AP_init_LAPIC:
.LFB234:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	movl	APIC_LOCAL_BASE, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 20
	pushl	$31
	.cfi_def_cfa_offset 24
	pushl	$880
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	APIC_LOCAL_BASE, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 20
	pushl	$0
	.cfi_def_cfa_offset 24
	pushl	$128
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	APIC_LOCAL_BASE, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 20
	pushl	$-1
	.cfi_def_cfa_offset 24
	pushl	$224
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	APIC_LOCAL_BASE, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 20
	pushl	$16777216
	.cfi_def_cfa_offset 24
	pushl	$208
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	APIC_LOCAL_BASE, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 20
	pushl	$511
	.cfi_def_cfa_offset 24
	pushl	$240
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	localapic_write
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	nop
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE234:
	.size	AP_init_LAPIC, .-AP_init_LAPIC
	.section	.rodata
.LC219:
	.string	"\nInitializing IO APIC!!!"
.LC220:
	.string	"RSDT"
.LC221:
	.string	"APIC"
.LC222:
	.string	"\nGot the MADT Structure"
.LC223:
	.string	"\n\tLocal APIC Found"
.LC224:
	.string	"\t\t\t%gLAPIC USABILITY: %x%g"
.LC225:
	.string	"\n\tIO APIC Found"
	.align 4
.LC226:
	.string	" id: %x, address: %x, GSIB: %x"
	.align 4
.LC227:
	.string	"\n\tInterrupt Source Override Found"
	.text
	.align 16
	.globl	MADTapic_parse
	.type	MADTapic_parse, @function
MADTapic_parse:
.LFB235:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC219
	.cfi_def_cfa_offset 48
	call	printf
	call	acpiGetRSDPtr
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L1461
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	movl	%eax, %esi
	pushl	$.LC220
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	acpiCheckHeader
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	jne	.L1461
	movl	4(%esi), %eax
	addl	$36, %esi
	subl	$36, %eax
	shrl	$2, %eax
	testl	%eax, %eax
	je	.L1461
	leal	(%esi,%eax,4), %edi
	jmp	.L1470
	.align 16
.L1464:
	addl	$4, %esi
	cmpl	%esi, %edi
	je	.L1461
.L1470:
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	$.LC221
	.cfi_def_cfa_offset 44
	pushl	(%esi)
	.cfi_def_cfa_offset 48
	call	acpiCheckHeader
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	jne	.L1464
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC222
	.cfi_def_cfa_offset 48
	call	printf
	movl	(%esi), %ebx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%ebx, madt
	addl	$44, %ebx
	.align 16
.L1465:
	movzbl	(%ebx), %eax
	cmpb	$1, %al
	je	.L1468
.L1476:
	jb	.L1466
	cmpb	$2, %al
	jne	.L1464
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC227
	.cfi_def_cfa_offset 48
	call	printf
	leal	2(%ebx), %eax
	addl	$10, %ebx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, ISD_entry
	movzbl	(%ebx), %eax
	cmpb	$1, %al
	jne	.L1476
.L1468:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC225
	.cfi_def_cfa_offset 48
	call	printf
	leal	2(%ebx), %eax
	movl	8(%ebx), %ecx
	movl	4(%ebx), %edx
	movl	%eax, IOAPIC_entry
	movzbl	2(%ebx), %eax
	pushl	%ecx
	.cfi_def_cfa_offset 52
	pushl	%edx
	.cfi_def_cfa_offset 56
	movzbl	%al, %eax
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	$.LC226
	.cfi_def_cfa_offset 64
	call	printf
	movl	IOAPIC_entry, %ebx
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	2(%ebx), %eax
	addl	$10, %ebx
	movl	%eax, APIC_IO_BASE
	jmp	.L1465
	.align 16
.L1466:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	addl	$1, total_CPU_Cores
	leal	8(%ebx), %ebp
	pushl	$.LC223
	.cfi_def_cfa_offset 48
	call	printf
	leal	2(%ebx), %eax
	movl	%eax, LAPIC_entry
	movl	4(%ebx), %eax
	movl	%ebp, %ebx
	pushl	$0
	.cfi_def_cfa_offset 52
	andl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$10
	.cfi_def_cfa_offset 60
	pushl	$.LC224
	.cfi_def_cfa_offset 64
	call	printf
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1465
	.align 16
.L1461:
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE235:
	.size	MADTapic_parse, .-MADTapic_parse
	.section	.rodata
.LC228:
	.string	"Unknown x86 CPU Detected\n"
	.text
	.align 16
	.globl	detect_cpu
	.type	detect_cpu, @function
detect_cpu:
.LFB237:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	xorl	%eax, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 16
/APP
/  31 "Source/arch/x86/cpuid/cpuid.c" 1
	cpuid
/  0 "" 2
/NO_APP
	cmpl	$1752462657, %ebx
	je	.L1479
	cmpl	$1970169159, %ebx
	je	.L1484
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC228
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1484:
	.cfi_restore_state
	call	do_intel
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1479:
	.cfi_restore_state
	call	do_amd
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE237:
	.size	detect_cpu, .-detect_cpu
	.globl	Intel
	.section	.rodata
.LC229:
	.string	"Brand ID Not Supported."
.LC230:
	.string	"Intel(R) Celeron(R) processor"
	.align 4
.LC231:
	.string	"Intel(R) Pentium(R) III processor"
	.align 4
.LC232:
	.string	"Intel(R) Pentium(R) III Xeon(R) processor"
	.align 4
.LC233:
	.string	"Mobile Intel(R) Pentium(R) III processor-M"
	.align 4
.LC234:
	.string	"Mobile Intel(R) Celeron(R) processor"
	.align 4
.LC235:
	.string	"Intel(R) Pentium(R) 4 processor"
.LC236:
	.string	"Intel(R) Xeon(R) Processor"
.LC237:
	.string	"Intel(R) Xeon(R) processor MP"
	.align 4
.LC238:
	.string	"Mobile Intel(R) Pentium(R) 4 processor-M"
	.align 4
.LC239:
	.string	"Mobile Intel(R) Pentium(R) Celeron(R) processor"
	.align 4
.LC240:
	.string	"Mobile Genuine Intel(R) processor"
	.align 4
.LC241:
	.string	"Intel(R) Celeron(R) M processor"
	.align 4
.LC242:
	.string	"Mobile Geniune Intel(R) processor"
	.align 4
.LC243:
	.string	"Intel(R) Pentium(R) M processor"
	.data
	.align 32
	.type	Intel, @object
	.size	Intel, 96
Intel:
	.long	.LC229
	.long	.LC230
	.long	.LC231
	.long	.LC232
	.long	.LC231
	.long	.LC6
	.long	.LC233
	.long	.LC234
	.long	.LC235
	.long	.LC235
	.long	.LC230
	.long	.LC236
	.long	.LC237
	.long	.LC6
	.long	.LC238
	.long	.LC239
	.long	.LC6
	.long	.LC240
	.long	.LC241
	.long	.LC234
	.long	.LC230
	.long	.LC242
	.long	.LC243
	.long	.LC234
	.globl	Intel_Other
	.section	.rodata
.LC244:
	.string	"Intel(R) Xeon(R) processor"
	.data
	.align 32
	.type	Intel_Other, @object
	.size	Intel_Other, 96
Intel_Other:
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC230
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC237
	.long	.LC6
	.long	.LC6
	.long	.LC244
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.long	.LC6
	.section	.rodata
.LC245:
	.string	"\nIntel Specific Features:\n"
.LC246:
	.string	"\nAPIC Supported\n"
.LC247:
	.string	"Type %d - "
.LC248:
	.string	"Original OEM"
.LC249:
	.string	"Overdrive"
.LC250:
	.string	"Dual-capable"
.LC251:
	.string	"Family %d - "
.LC252:
	.string	"Pentium Pro"
.LC253:
	.string	"Pentium 4"
.LC254:
	.string	"Extended family %d\n"
.LC255:
	.string	"Model %d - "
.LC256:
	.string	"DX"
.LC257:
	.string	"SX"
.LC258:
	.string	"487/DX2"
.LC259:
	.string	"SL"
.LC260:
	.string	"SX2"
.LC261:
	.string	"Write-back enhanced DX2"
.LC262:
	.string	"DX4"
.LC263:
	.string	"60/66"
.LC264:
	.string	"75-200"
.LC265:
	.string	"for 486 system"
.LC266:
	.string	"MMX"
.LC267:
	.string	"Pentium II Model 3"
	.align 4
.LC268:
	.string	"Pentium II Model 5/Xeon/Celeron"
.LC269:
	.string	"Celeron"
	.align 4
.LC270:
	.string	"Pentium III/Pentium III Xeon - external L2 cache"
	.align 4
.LC271:
	.string	"Pentium III/Pentium III Xeon - internal L2 cache"
.LC272:
	.string	"Brand: "
.LC273:
	.string	"Brand %d - "
.LC274:
	.string	"Reserved\n"
.LC275:
	.string	"Stepping: %d Reserved: %d\n"
	.text
	.align 16
	.globl	do_intel
	.type	do_intel, @function
do_intel:
.LFB238:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC245
	.cfi_def_cfa_offset 64
	call	printf
	movl	$1, %eax
/APP
/  110 "Source/arch/x86/cpuid/cpuid.c" 1
	cpuid
/  0 "" 2
/NO_APP
	movl	%ebx, 28(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	andb	$2, %dh
	movl	%eax, %esi
	jne	.L1540
.L1486:
	movl	%esi, %edx
	movl	%esi, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	andl	$15, %eax
	shrl	$12, %edx
	movl	%esi, %ebp
	movl	%eax, 12(%esp)
	andl	$3, %edx
	movl	%esi, %ebx
	pushl	%edx
	.cfi_def_cfa_offset 60
	shrl	$4, %ebp
	shrl	$8, %ebx
	movl	%esi, %edi
	andl	$15, %ebp
	andl	$15, %ebx
	movl	%edx, 20(%esp)
	pushl	$.LC247
	.cfi_def_cfa_offset 64
	shrl	$14, %edi
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	8(%esp), %edx
	cmpl	$2, %edx
	je	.L1488
	cmpl	$3, %edx
	je	.L1489
	cmpl	$1, %edx
	je	.L1490
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC248
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L1491:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC196
	.cfi_def_cfa_offset 64
	call	printf
	popl	%eax
	.cfi_def_cfa_offset 60
	popl	%edx
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	$.LC251
	.cfi_def_cfa_offset 64
	call	printf
	leal	-3(%ebx), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	$12, %eax
	ja	.L1492
	jmp	*.L1494(,%eax,4)
	.section	.rodata
	.align 4
	.align 4
.L1494:
	.long	.L1493
	.long	.L1495
	.long	.L1496
	.long	.L1497
	.long	.L1492
	.long	.L1492
	.long	.L1492
	.long	.L1492
	.long	.L1492
	.long	.L1492
	.long	.L1492
	.long	.L1492
	.long	.L1498
	.text
	.align 16
.L1490:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC249
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1491
	.align 16
.L1540:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC246
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1486
	.align 16
.L1489:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC6
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1491
	.align 16
.L1488:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC250
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1491
	.align 16
.L1498:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC253
	.cfi_def_cfa_offset 64
	call	printf
	movl	$.LC196, (%esp)
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L1500:
	movl	%esi, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	shrl	$20, %eax
	movzbl	%al, %eax
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	$.LC254
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	.align 16
.L1501:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%ebp
	.cfi_def_cfa_offset 60
	pushl	$.LC255
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	$5, %ebx
	je	.L1503
	cmpl	$6, %ebx
	je	.L1504
	cmpl	$4, %ebx
	je	.L1541
.L1502:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC196
	.cfi_def_cfa_offset 64
	call	printf
	movl	$-2147483648, %eax
/APP
/  232 "Source/arch/x86/cpuid/cpuid.c" 1
	cpuid
/  0 "" 2
/NO_APP
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	$-2147483645, %eax
	ja	.L1542
	movzbl	12(%esp), %ebx
	testl	%ebx, %ebx
	je	.L1527
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	$.LC273
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	$23, %ebx
	jg	.L1528
	cmpl	$1713, %esi
	je	.L1531
	cmpl	$3859, %esi
	je	.L1531
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	Intel(,%ebx,4)
	.cfi_def_cfa_offset 60
	pushl	$.LC3
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L1527:
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	pushl	%edi
	.cfi_def_cfa_offset 56
	pushl	12(%esp)
	.cfi_def_cfa_offset 60
	pushl	$.LC275
	.cfi_def_cfa_offset 64
	call	printf
	addl	$44, %esp
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1497:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -20
	.cfi_offset 5, -8
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC252
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L1499:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC196
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1501
	.align 16
.L1496:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC109
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1499
	.align 16
.L1495:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC108
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1499
	.align 16
.L1493:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC107
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1499
	.align 16
.L1492:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC196
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	$15, %ebx
	jne	.L1501
	jmp	.L1500
	.align 16
.L1542:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC272
	.cfi_def_cfa_offset 64
	call	printf
	movl	$-2147483646, %eax
/APP
/  241 "Source/arch/x86/cpuid/cpuid.c" 1
	cpuid
/  0 "" 2
/NO_APP
	pushl	%edx
	.cfi_def_cfa_offset 68
	pushl	%ecx
	.cfi_def_cfa_offset 72
	pushl	%ebx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	printregs
	movl	$-2147483645, %eax
/APP
/  245 "Source/arch/x86/cpuid/cpuid.c" 1
	cpuid
/  0 "" 2
/NO_APP
	addl	$32, %esp
	.cfi_def_cfa_offset 48
	pushl	%edx
	.cfi_def_cfa_offset 52
	pushl	%ecx
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	printregs
	movl	$-2147483644, %eax
/APP
/  249 "Source/arch/x86/cpuid/cpuid.c" 1
	cpuid
/  0 "" 2
/NO_APP
	pushl	%edx
	.cfi_def_cfa_offset 68
	pushl	%ecx
	.cfi_def_cfa_offset 72
	pushl	%ebx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	printregs
	addl	$20, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC196
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1527
	.align 16
.L1503:
	cmpl	$2, %ebp
	je	.L1514
	jle	.L1543
	cmpl	$3, %ebp
	je	.L1517
	cmpl	$4, %ebp
	jne	.L1502
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC266
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1541:
	cmpl	$8, %ebp
	ja	.L1502
	jmp	*.L1507(,%ebp,4)
	.section	.rodata
	.align 4
	.align 4
.L1507:
	.long	.L1506
	.long	.L1506
	.long	.L1508
	.long	.L1509
	.long	.L1510
	.long	.L1511
	.long	.L1502
	.long	.L1512
	.long	.L1513
	.text
	.align 16
.L1504:
	cmpl	$8, %ebp
	ja	.L1502
	jmp	*.L1520(,%ebp,4)
	.section	.rodata
	.align 4
	.align 4
.L1520:
	.long	.L1502
	.long	.L1519
	.long	.L1502
	.long	.L1521
	.long	.L1502
	.long	.L1522
	.long	.L1523
	.long	.L1524
	.long	.L1525
	.text
	.align 16
.L1531:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	Intel_Other(,%ebx,4)
	.cfi_def_cfa_offset 60
	pushl	$.LC3
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1527
	.align 16
.L1528:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC274
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1527
	.align 16
.L1543:
	cmpl	$1, %ebp
	jne	.L1502
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC263
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1517:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC265
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1514:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC264
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1525:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC271
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1524:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC270
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1523:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC269
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1522:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC268
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1521:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC267
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1519:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC252
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1513:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC262
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1512:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC261
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1511:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC260
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1510:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC259
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1509:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC258
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1508:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC257
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.align 16
.L1506:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC256
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L1502
	.cfi_endproc
.LFE238:
	.size	do_intel, .-do_intel
	.section	.rodata
.LC276:
	.string	"%s"
	.text
	.align 16
	.globl	printregs
	.type	printregs, @function
printregs:
.LFB239:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	xorl	%eax, %eax
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	64(%esp), %edi
	movl	68(%esp), %esi
	leal	15(%esp), %edx
	movb	$0, 31(%esp)
.L1545:
	leal	0(,%eax,8), %ecx
	movl	%edi, %ebp
	sarl	%cl, %ebp
	movl	%ebp, %ebx
	movl	%esi, %ebp
	sarl	%cl, %ebp
	movb	%bl, (%edx,%eax)
	movl	%ebp, %ebx
	movl	72(%esp), %ebp
	movb	%bl, 19(%esp,%eax)
	sarl	%cl, %ebp
	movl	%ebp, %ebx
	movb	%bl, 23(%esp,%eax)
	movl	76(%esp), %ebx
	sarl	%cl, %ebx
	movb	%bl, 27(%esp,%eax)
	addl	$1, %eax
	cmpl	$4, %eax
	jne	.L1545
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	$.LC276
	.cfi_def_cfa_offset 80
	call	printf
	addl	$60, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE239:
	.size	printregs, .-printregs
	.section	.rodata
.LC277:
	.string	"AMD Specific Features:\n"
.LC278:
	.string	"Family: %d Model: %d ["
.LC279:
	.string	"486 Model %d"
.LC280:
	.string	"K6 Model %d"
.LC281:
	.string	"K6-2 Model 8"
.LC282:
	.string	"K6-III Model 9"
.LC283:
	.string	"K5/K6 Model %d"
.LC284:
	.string	"Athlon Model %d"
.LC285:
	.string	"Duron Model 3"
	.align 4
.LC286:
	.string	"Athlon MP/Mobile Athlon Model 6"
.LC287:
	.string	"Mobile Duron Model 7"
.LC288:
	.string	"Duron/Athlon Model %d"
.LC289:
	.string	"]\n"
.LC290:
	.string	"Detected Processor Name: "
	.align 4
.LC291:
	.string	"Temperature Sensing Diode Detected!\n"
	.text
	.align 16
	.globl	do_amd
	.type	do_amd, @function
do_amd:
.LFB240:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	movl	$1, %edi
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC277
	.cfi_def_cfa_offset 48
	call	printf
	movl	%edi, %eax
/APP
/  289 "Source/arch/x86/cpuid/cpuid.c" 1
	cpuid
/  0 "" 2
/NO_APP
	movl	%eax, %ebp
	movl	%eax, %ebx
	addl	$12, %esp
	.cfi_def_cfa_offset 36
	shrl	$4, %ebp
	shrl	$8, %ebx
	movl	%eax, %esi
	andl	$15, %ebp
	andl	$15, %ebx
	pushl	%ebp
	.cfi_def_cfa_offset 40
	pushl	%ebx
	.cfi_def_cfa_offset 44
	pushl	$.LC278
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	cmpl	$5, %ebx
	je	.L1550
	cmpl	$6, %ebx
	je	.L1551
	cmpl	$4, %ebx
	je	.L1577
.L1549:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC289
	.cfi_def_cfa_offset 48
	call	printf
	movl	$-2147483648, %eax
/APP
/  343 "Source/arch/x86/cpuid/cpuid.c" 1
	cpuid
/  0 "" 2
/NO_APP
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	movl	%eax, %edi
	je	.L1563
	cmpl	$-2147483647, %eax
	ja	.L1578
.L1565:
	movl	%esi, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	andl	$15, %esi
	shrl	$12, %eax
	pushl	%eax
	.cfi_def_cfa_offset 40
	pushl	%esi
	.cfi_def_cfa_offset 44
	pushl	$.LC275
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L1563:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1577:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ebp
	.cfi_def_cfa_offset 44
	pushl	$.LC279
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1549
	.align 16
.L1551:
	cmpl	$7, %ebp
	ja	.L1557
	jmp	*.L1559(,%ebp,4)
	.section	.rodata
	.align 4
	.align 4
.L1559:
	.long	.L1557
	.long	.L1558
	.long	.L1558
	.long	.L1560
	.long	.L1558
	.long	.L1557
	.long	.L1561
	.long	.L1562
	.text
	.align 16
.L1550:
	cmpl	$9, %ebp
	jg	.L1553
	movl	%ebp, %ecx
	sall	%cl, %edi
	testl	$207, %edi
	jne	.L1554
	testl	$512, %edi
	jne	.L1555
	andl	$256, %edi
	jne	.L1579
.L1553:
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ebp
	.cfi_def_cfa_offset 44
	pushl	$.LC283
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1549
	.align 16
.L1578:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	movl	$-2147483646, %ebp
	pushl	$.LC290
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L1566:
	movl	%ebp, %eax
/APP
/  351 "Source/arch/x86/cpuid/cpuid.c" 1
	cpuid
/  0 "" 2
/NO_APP
	pushl	%edx
	.cfi_def_cfa_offset 36
	pushl	%ecx
	.cfi_def_cfa_offset 40
	addl	$1, %ebp
	pushl	%ebx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	printregs
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	cmpl	$-2147483643, %ebp
	jne	.L1566
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC196
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	cmpl	$-2147483642, %edi
	jbe	.L1565
	movl	$-2147483641, %eax
/APP
/  357 "Source/arch/x86/cpuid/cpuid.c" 1
	cpuid
/  0 "" 2
/NO_APP
	andl	$1, %edx
	je	.L1565
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC291
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1565
	.align 16
.L1562:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC287
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1549
	.align 16
.L1561:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC286
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1549
	.align 16
.L1560:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC285
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1549
	.align 16
.L1558:
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ebp
	.cfi_def_cfa_offset 44
	pushl	$.LC284
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1549
	.align 16
.L1557:
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ebp
	.cfi_def_cfa_offset 44
	pushl	$.LC288
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1549
	.align 16
.L1579:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC281
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1549
	.align 16
.L1554:
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ebp
	.cfi_def_cfa_offset 44
	pushl	$.LC280
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1549
	.align 16
.L1555:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC282
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1549
	.cfi_endproc
.LFE240:
	.size	do_amd, .-do_amd
	.comm	mmads_stack_end,4,4
	.comm	mmads_stack_start,4,4
	.globl	mmads_stack_size
	.section	.bss
	.align 4
	.type	mmads_stack_size, @object
	.size	mmads_stack_size, 4
mmads_stack_size:
	.zero	4
	.comm	MMADS_Process,4,4
	.comm	MMADS_csrbTweakerThread,4,4
	.section	.rodata
	.align 4
.LC292:
	.string	"\nOS Specific Memory Regions Preallocated!"
	.text
	.align 16
	.globl	memmap_generator
	.type	memmap_generator, @function
memmap_generator:
.LFB241:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	phy_alloc4K
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	%eax, Fmemmap
	movl	$0, 4(%eax)
	movl	$4194304, 12(%eax)
	movl	$7, 20(%eax)
	movl	$1048130, 16(%eax)
	movl	$4194304, 28(%eax)
	movl	$4194304, 36(%eax)
	movl	$8, 44(%eax)
	movl	$1048130, 40(%eax)
	movl	$8388608, 52(%eax)
	movl	$6291456, 60(%eax)
	movl	$9, 68(%eax)
	movl	$1048130, 64(%eax)
	movl	$14680064, 76(%eax)
	movl	$2097152, 84(%eax)
	movl	$10, 92(%eax)
	movl	$1048130, 88(%eax)
	movl	$16777216, 100(%eax)
	movl	$8388608, 108(%eax)
	movl	$9, 116(%eax)
	movl	$1048130, 112(%eax)
	movl	$25165824, 124(%eax)
	movl	$6291456, 132(%eax)
	movl	$11, 140(%eax)
	movl	$1048130, 136(%eax)
	movl	$31457280, 148(%eax)
	movl	$597688320, 156(%eax)
	movl	$11, 164(%eax)
	movl	$1048130, 160(%eax)
	movl	$734003200, 172(%eax)
	movl	$104857600, 180(%eax)
	movl	$1, 188(%eax)
	movl	$1048130, 184(%eax)
	movl	$838860800, 196(%eax)
	movl	$1308622848, 204(%eax)
	movl	$1, 212(%eax)
	movl	$1048130, 208(%eax)
	pushl	$.LC292
	.cfi_def_cfa_offset 32
	call	printf
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE241:
	.size	memmap_generator, .-memmap_generator
	.section	.rodata
.LC293:
	.string	"\nSetting up Frame Stack!"
.LC294:
	.string	"\nSize of Frame Stack = %d"
	.text
	.align 16
	.globl	setup_frameStack
	.type	setup_frameStack, @function
setup_frameStack:
.LFB242:
	.cfi_startproc
	subl	$44, %esp
	.cfi_def_cfa_offset 48
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC293
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	$10485760, 28(%esp)
	movl	$10485760, 12(%esp)
	movl	Fmemmap, %eax
	movl	%eax, 24(%esp)
	addl	$24, 24(%esp)
.L1589:
	movl	24(%esp), %eax
	movl	20(%eax), %eax
	cmpl	$1, %eax
	je	.L1583
	movl	24(%esp), %eax
	movl	20(%eax), %eax
	cmpl	$12, %eax
	jne	.L1584
.L1583:
	movl	24(%esp), %eax
	movl	4(%eax), %eax
	movl	%eax, 20(%esp)
	jmp	.L1585
.L1586:
	movl	20(%esp), %eax
	shrl	$12, %eax
	movl	%eax, %edx
	movl	28(%esp), %eax
	movl	%edx, (%eax)
	addl	$4, 28(%esp)
	addl	$4096, 20(%esp)
.L1585:
	movl	24(%esp), %eax
	movl	4(%eax), %edx
	movl	24(%esp), %eax
	movl	12(%eax), %eax
	addl	%edx, %eax
	cmpl	20(%esp), %eax
	ja	.L1586
.L1584:
	addl	$24, 24(%esp)
	movl	24(%esp), %eax
	movl	16(%eax), %eax
	cmpl	$1048130, %eax
	jne	.L1593
	jmp	.L1589
.L1593:
	nop
	movl	28(%esp), %eax
	movl	%eax, frame_stack_end
	movl	frame_stack_end, %eax
	subl	$4, %eax
	movl	%eax, frame_stack_end
	movl	frame_stack_end, %eax
	movl	%eax, %edx
	movl	12(%esp), %eax
	subl	%eax, %edx
	movl	%edx, %eax
	sarl	$2, %eax
	movl	%eax, 8(%esp)
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	16(%esp)
	.cfi_def_cfa_offset 60
	pushl	$.LC294
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	$0, 16(%esp)
	jmp	.L1590
.L1591:
	movl	16(%esp), %eax
	leal	0(,%eax,4), %edx
	movl	12(%esp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	16(%esp), %eax
	leal	0(,%eax,4), %edx
	movl	12(%esp), %eax
	addl	%eax, %edx
	movl	8(%esp), %eax
	subl	16(%esp), %eax
	addl	$1073741823, %eax
	leal	0(,%eax,4), %ecx
	movl	12(%esp), %eax
	addl	%ecx, %eax
	movl	(%eax), %eax
	movl	%eax, (%edx)
	movl	8(%esp), %eax
	subl	16(%esp), %eax
	addl	$1073741823, %eax
	leal	0(,%eax,4), %edx
	movl	12(%esp), %eax
	addl	%eax, %edx
	movl	4(%esp), %eax
	movl	%eax, (%edx)
	addl	$1, 16(%esp)
.L1590:
	movl	8(%esp), %eax
	shrl	%eax
	cmpl	16(%esp), %eax
	ja	.L1591
	nop
	addl	$44, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE242:
	.size	setup_frameStack, .-setup_frameStack
	.section	.rodata
.LC295:
	.string	"\nNo memory left! %x"
	.text
	.align 16
	.globl	pop_frameStack
	.type	pop_frameStack, @function
pop_frameStack:
.LFB243:
	.cfi_startproc
.L1595:
	movl	frame_stack_end, %edx
	movl	(%edx), %eax
	subl	$4, %edx
	movl	%edx, frame_stack_end
	testl	%eax, %eax
	jne	.L1603
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	.align 16
.L1597:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$0
	.cfi_def_cfa_offset 28
	pushl	$.LC295
	.cfi_def_cfa_offset 32
	call	printf
	movl	frame_stack_end, %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	(%edx), %eax
	subl	$4, %edx
	movl	%edx, frame_stack_end
	testl	%eax, %eax
	je	.L1597
	addl	$12, %esp
	.cfi_def_cfa_offset 4
.L1603:
	rep ret
	.cfi_endproc
.LFE243:
	.size	pop_frameStack, .-pop_frameStack
	.align 16
	.globl	push_frameStack
	.type	push_frameStack, @function
push_frameStack:
.LFB244:
	.cfi_startproc
	movl	frame_stack_end, %eax
	leal	4(%eax), %edx
	movl	%edx, frame_stack_end
	movl	4(%esp), %edx
	movl	%edx, 4(%eax)
	ret
	.cfi_endproc
.LFE244:
	.size	push_frameStack, .-push_frameStack
	.align 16
	.globl	phy_alloc4K
	.type	phy_alloc4K, @function
phy_alloc4K:
.LFB245:
	.cfi_startproc
	movl	frame_stack_end, %edx
	movl	(%edx), %eax
	subl	$4, %edx
	movl	%edx, frame_stack_end
	testl	%eax, %eax
	jne	.L1615
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	.align 16
.L1608:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$0
	.cfi_def_cfa_offset 28
	pushl	$.LC295
	.cfi_def_cfa_offset 32
	call	printf
	movl	frame_stack_end, %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	(%edx), %eax
	subl	$4, %edx
	movl	%edx, frame_stack_end
	testl	%eax, %eax
	je	.L1608
	sall	$12, %eax
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
.L1615:
	sall	$12, %eax
	ret
	.cfi_endproc
.LFE245:
	.size	phy_alloc4K, .-phy_alloc4K
	.section	.rodata
.LC296:
	.string	"\n%x %x %x"
	.text
	.align 16
	.globl	Setup_PhyMEM
	.type	Setup_PhyMEM, @function
Setup_PhyMEM:
.LFB246:
	.cfi_startproc
	subl	$44, %esp
	.cfi_def_cfa_offset 48
	movl	system_pdirCap, %eax
	addl	$4096, %eax
	movl	%eax, 16(%esp)
	movl	16(%esp), %eax
	movl	%eax, 12(%esp)
	movl	16(%esp), %eax
	addl	$16, %eax
	movl	%eax, %edx
	movl	12(%esp), %eax
	movl	%edx, 12(%eax)
	movl	system_pdirCap, %eax
	addl	$8192, %eax
	movl	%eax, 16(%esp)
	movl	16(%esp), %eax
	movl	%eax, 8(%esp)
	movl	16(%esp), %eax
	addl	$16, %eax
	movl	%eax, %edx
	movl	8(%esp), %eax
	movl	%edx, 12(%eax)
	movl	8(%esp), %eax
	movl	$0, 4(%eax)
	movl	12(%esp), %eax
	movl	$0, 4(%eax)
	movl	12(%esp), %eax
	movl	$0, (%eax)
	movl	8(%esp), %eax
	movl	$0, (%eax)
	movl	Fmemmap, %eax
	movl	%eax, 28(%esp)
	movl	28(%esp), %eax
	movl	%eax, mmap_info
	movl	12(%esp), %eax
	movl	12(%eax), %eax
	movl	%eax, 24(%esp)
	movl	8(%esp), %eax
	movl	12(%eax), %eax
	movl	%eax, 20(%esp)
	addl	$24, 28(%esp)
.L1621:
	movl	28(%esp), %eax
	movl	20(%eax), %eax
	cmpl	$11, %eax
	jne	.L1617
	movl	24(%esp), %eax
	movl	24(%esp), %edx
	movl	%edx, (%eax)
	movl	28(%esp), %eax
	movl	12(%eax), %edx
	movl	24(%esp), %eax
	movl	%edx, 8(%eax)
	movl	28(%esp), %eax
	movl	4(%eax), %edx
	movl	24(%esp), %eax
	movl	%edx, 4(%eax)
	movl	24(%esp), %eax
	movl	4(%eax), %ecx
	movl	24(%esp), %eax
	movl	8(%eax), %edx
	movl	24(%esp), %eax
	movl	(%eax), %eax
	pushl	%ecx
	.cfi_def_cfa_offset 52
	pushl	%edx
	.cfi_def_cfa_offset 56
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	$.LC296
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	addl	$16, 24(%esp)
	movl	12(%esp), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	12(%esp), %eax
	movl	%edx, (%eax)
	jmp	.L1618
.L1617:
	movl	20(%esp), %eax
	movl	20(%esp), %edx
	movl	%edx, (%eax)
	movl	28(%esp), %eax
	movl	12(%eax), %edx
	movl	20(%esp), %eax
	movl	%edx, 8(%eax)
	movl	28(%esp), %eax
	movl	4(%eax), %edx
	movl	20(%esp), %eax
	movl	%edx, 4(%eax)
	addl	$16, 20(%esp)
	movl	8(%esp), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	8(%esp), %eax
	movl	%edx, (%eax)
.L1618:
	addl	$24, 28(%esp)
	movl	28(%esp), %eax
	movl	16(%eax), %eax
	cmpl	$1048130, %eax
	jne	.L1623
	jmp	.L1621
.L1623:
	nop
	movl	12(%esp), %eax
	movl	24(%esp), %edx
	movl	%edx, 8(%eax)
	movl	12(%esp), %eax
	movl	12(%eax), %edx
	movl	24(%esp), %eax
	movl	%edx, (%eax)
	movl	8(%esp), %eax
	movl	20(%esp), %edx
	movl	%edx, 8(%eax)
	movl	8(%esp), %eax
	movl	12(%eax), %edx
	movl	20(%esp), %eax
	movl	%edx, (%eax)
	nop
	addl	$44, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE246:
	.size	Setup_PhyMEM, .-Setup_PhyMEM
	.align 16
	.globl	pmem
	.type	pmem, @function
pmem:
.LFB247:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	system_pdirCap, %edi
	movl	48(%esp), %eax
	addl	$3, %eax
	movl	%eax, %esi
	call	Get_Scheduler
	movl	multitasking_ON, %edx
	andl	$-4, %esi
	testl	%edx, %edx
	je	.L1625
	movl	44(%eax), %eax
	addl	$1, mmads_stack_size
	movl	8(%eax), %eax
	addl	$4, %eax
	movl	%eax, mmads_stack_end
.L1625:
	movl	4108(%edi), %ebx
	movl	$1048576, %eax
	jmp	.L1630
	.align 16
.L1626:
	addl	$16, %ebx
	subl	$1, %eax
	je	.L1636
.L1630:
	movl	(%ebx), %ebx
	movl	8(%ebx), %edx
	cmpl	%edx, %esi
	ja	.L1626
	subl	%esi, %edx
	testl	%edx, %edx
	movl	%edx, 8(%ebx)
	jne	.L1627
	movl	$1, 12(%ebx)
	subl	$1, 4096(%edi)
.L1627:
	movl	8200(%edi), %ebp
	movl	4(%ebx), %edx
	leal	32(%ebp), %eax
	movl	%esi, 8(%ebp)
	leal	16(%ebp), %ecx
	movl	%edx, 4(%ebp)
	movl	%ebp, 0(%ebp)
	addl	$1, 8192(%edi)
	testl	$4095, %eax
	je	.L1637
.L1628:
	movl	8204(%edi), %eax
	movl	%eax, (%ecx)
	movl	%ecx, 8200(%edi)
	movl	%edx, %eax
	addl	%esi, 4(%ebx)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1637:
	.cfi_restore_state
	movl	%edx, 12(%esp)
	movl	%ecx, 8(%esp)
	call	phy_alloc4K
	movl	8(%esp), %ecx
	movl	%eax, 16(%ebp)
	movl	12(%esp), %edx
	movl	%ecx, (%eax)
	leal	16(%eax), %ecx
	addl	$1, 8192(%edi)
	jmp	.L1628
	.align 16
.L1636:
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE247:
	.size	pmem, .-pmem
	.align 16
	.globl	pfree
	.type	pfree, @function
pfree:
.LFB248:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	system_pdirCap, %eax
	movl	48(%esp), %ecx
	movl	%eax, 8(%esp)
	movl	8204(%eax), %ebx
	movl	8192(%eax), %eax
	movl	%eax, %esi
	movl	%eax, 12(%esp)
	addl	%esi, %esi
	je	.L1638
	xorl	%eax, %eax
	.align 16
.L1645:
	movl	(%ebx), %ebx
	movl	4(%ebx), %edx
	cmpl	%edx, %ecx
	jb	.L1640
	movl	8(%ebx), %edi
	leal	(%edx,%edi), %ebp
	cmpl	%ebp, %ecx
	jb	.L1654
.L1640:
	addl	$1, %eax
	addl	$16, %ebx
	cmpl	%esi, %eax
	jne	.L1645
.L1638:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1654:
	.cfi_restore_state
	movl	8(%esp), %eax
	movl	4104(%eax), %esi
	movl	%edi, 8(%esi)
	movl	%esi, (%esi)
	leal	16(%esi), %edi
	movl	%edx, 4(%esi)
	movl	$0, 12(%esi)
	addl	$1, 4096(%eax)
	leal	32(%esi), %eax
	testl	$4095, %eax
	je	.L1655
.L1641:
	movl	8(%esp), %esi
	movl	8(%ebx), %ecx
	movl	4108(%esi), %eax
	testl	%ecx, %ecx
	movl	%eax, (%edi)
	movl	4(%ebx), %eax
	movl	%edi, 4104(%esi)
	leal	(%ecx,%eax), %edx
	je	.L1644
	.align 16
.L1648:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L1648
.L1644:
	movl	8(%esp), %eax
	movl	$0, 4(%ebx)
	movl	$0, 8(%ebx)
	movl	$1, 12(%ebx)
	subl	$1, 8192(%eax)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L1655:
	.cfi_restore_state
	call	phy_alloc4K
	movl	%eax, 16(%esi)
	movl	%edi, (%eax)
	leal	16(%eax), %edi
	movl	8(%esp), %eax
	addl	$1, 8192(%eax)
	jmp	.L1641
	.cfi_endproc
.LFE248:
	.size	pfree, .-pfree
	.globl	mtalc_start
	.section	.rodata
	.align 4
	.type	mtalc_start, @object
	.size	mtalc_start, 4
mtalc_start:
	.long	545259520
	.globl	mtalc_end
	.align 4
	.type	mtalc_end, @object
	.size	mtalc_end, 4
mtalc_end:
	.long	734003200
	.align 4
.LC297:
	.string	"\nCould not find memory, sorry. Try kmalloc()"
	.text
	.align 16
	.globl	mtalloc
	.type	mtalloc, @function
mtalloc:
.LFB250:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	xorl	%ecx, %ecx
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	movl	system_dir, %eax
	movl	32(%esp), %esi
	leal	520(%eax), %edi
	leal	804(%eax), %ebp
.L1659:
	movl	(%edi), %edx
	andl	$-4096, %edx
	leal	4096(%edx), %ebx
	jmp	.L1658
	.align 16
.L1670:
	addl	$1, %ecx
	cmpl	%esi, %ecx
	je	.L1668
	addl	$4, %edx
	cmpl	%edx, %ebx
	je	.L1669
.L1658:
	movl	(%edx), %eax
	testb	$4, %ah
	je	.L1670
	addl	$4, %edx
	xorl	%ecx, %ecx
	cmpl	%edx, %ebx
	jne	.L1658
.L1669:
	addl	$4, %edi
	cmpl	%edi, %ebp
	jne	.L1659
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC297
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L1660:
	jmp	.L1660
	.align 16
.L1668:
	movl	%eax, %ecx
	andl	$-4096, %eax
	orb	$4, %ch
	movl	%ecx, (%edx)
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE250:
	.size	mtalloc, .-mtalloc
	.align 16
	.globl	mtfree
	.type	mtfree, @function
mtfree:
.LFB251:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %ecx
	movl	system_dir, %edx
	movl	12(%esp), %ebx
	shrl	$10, %ecx
	movl	%ecx, %eax
	movl	_cur_dir, %ecx
	movl	2129920(%edx), %edx
	andl	$4092, %eax
	movl	2129920(%ecx), %ecx
	andl	$-4096, %edx
	addl	%eax, %edx
	andl	$-4096, %ecx
	addl	%eax, %ecx
	testl	%ebx, %ebx
	je	.L1671
	xorl	%eax, %eax
	.align 16
.L1675:
	andl	$-1025, (%edx,%eax,4)
	movl	$0, (%ecx,%eax,4)
	addl	$1, %eax
	cmpl	%ebx, %eax
	jne	.L1675
.L1671:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE251:
	.size	mtfree, .-mtfree
	.align 16
	.globl	fsalloc
	.type	fsalloc, @function
fsalloc:
.LFB252:
	.cfi_startproc
	jmp	pmem
	.cfi_endproc
.LFE252:
	.size	fsalloc, .-fsalloc
	.comm	main_prdir,4,4
	.section	.rodata
.LC298:
	.string	" Cant get the Page! "
	.text
	.align 16
	.globl	SystemDir_Mapper
	.type	SystemDir_Mapper, @function
SystemDir_Mapper:
.LFB253:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	xorl	%edx, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 16
	movl	system_dir, %ecx
	jmp	.L1681
	.align 16
.L1688:
	andl	$-4096, %eax
	andl	$1023, %ebx
	leal	(%eax,%ebx,4), %ebx
	movl	(%ebx), %eax
	andl	$-2147479553, %eax
	orl	%edx, %eax
	addl	$4096, %edx
	andb	$243, %ah
	orl	$7, %eax
	cmpl	$-4096, %edx
	movl	%eax, (%ebx)
	je	.L1687
.L1681:
	movl	%edx, %eax
	movl	%edx, %ebx
	shrl	$22, %eax
	shrl	$12, %ebx
	movl	(%ecx,%eax,4), %eax
	testl	%eax, %eax
	jne	.L1688
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 28
	pushl	$.LC298
	.cfi_def_cfa_offset 32
	call	printf
	movl	0, %eax
	ud2
	.align 16
.L1687:
	.cfi_restore_state
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE253:
	.size	SystemDir_Mapper, .-SystemDir_Mapper
	.align 16
	.globl	Setup_SystemDir
	.type	Setup_SystemDir, @function
Setup_SystemDir:
.LFB254:
	.cfi_startproc
	movl	$4194304, system_pdirCap
	movl	$4194304, system_dir
	movl	$4194304, %ecx
	movl	$4206592, %eax
	.align 16
.L1690:
	movl	%eax, %edx
	addl	$4096, %eax
	addl	$4, %ecx
	andb	$243, %dh
	orl	$3, %edx
	movl	%edx, -4(%ecx)
	cmpl	$8400896, %eax
	jne	.L1690
	jmp	SystemDir_Mapper
	.cfi_endproc
.LFE254:
	.size	Setup_SystemDir, .-Setup_SystemDir
	.align 16
	.globl	pgdir_maker
	.type	pgdir_maker, @function
pgdir_maker:
.LFB255:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$20, %esp
	.cfi_def_cfa_offset 28
	pushl	$4
	.cfi_def_cfa_offset 32
	call	mtalloc
	movl	%eax, %ebx
	movl	%eax, (%esp)
	call	Setup_VMEM
	addl	$24, %esp
	.cfi_def_cfa_offset 8
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE255:
	.size	pgdir_maker, .-pgdir_maker
	.align 16
	.globl	flush_tlb_entry
	.type	flush_tlb_entry, @function
flush_tlb_entry:
.LFB258:
	.cfi_startproc
/APP
/  96 "Source/MemManagement/virt_mm/paging.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	4(%esp), %eax
/APP
/  97 "Source/MemManagement/virt_mm/paging.c" 1
	invlpg (%eax)
/  0 "" 2
/  98 "Source/MemManagement/virt_mm/paging.c" 1
	sti
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE258:
	.size	flush_tlb_entry, .-flush_tlb_entry
	.align 16
	.globl	map
	.type	map, @function
map:
.LFB260:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %edi
	movl	20(%esp), %ebx
	movl	24(%esp), %esi
	addl	%edi, %ebx
	cmpl	%ebx, %edi
	jnb	.L1695
	.align 16
.L1699:
	subl	$4, %esp
	.cfi_def_cfa_offset 20
	pushl	%esi
	.cfi_def_cfa_offset 24
	pushl	$1
	.cfi_def_cfa_offset 28
	pushl	%edi
	.cfi_def_cfa_offset 32
	call	get_page
	movl	(%eax), %edx
	movl	%edi, %ecx
	addl	$4096, %edi
	orl	$3079, %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	andl	$-2147479553, %edx
	orl	%ecx, %edx
	cmpl	%ebx, %edi
	movl	%edx, (%eax)
	jb	.L1699
.L1695:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE260:
	.size	map, .-map
	.align 16
	.globl	get_page
	.type	get_page, @function
get_page:
.LFB261:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$4, %esp
	.cfi_def_cfa_offset 16
	movl	16(%esp), %ebx
	movl	24(%esp), %edx
	movl	%ebx, %esi
	shrl	$22, %ebx
	leal	(%edx,%ebx,4), %ebx
	shrl	$12, %esi
	movl	(%ebx), %eax
	testl	%eax, %eax
	jne	.L1709
	movl	20(%esp), %eax
	testl	%eax, %eax
	je	.L1705
	call	phy_alloc4K
	leal	4096(%eax), %edx
	movl	%eax, (%ebx)
	.align 16
.L1706:
	addl	$4, %eax
	movl	$0, -4(%eax)
	cmpl	%edx, %eax
	jne	.L1706
	movl	(%ebx), %eax
	movl	%eax, %edx
	orl	$3, %edx
	movl	%edx, (%ebx)
.L1709:
	andl	$-4096, %eax
	andl	$1023, %esi
	leal	(%eax,%esi,4), %eax
.L1702:
	addl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
.L1705:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC298
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	jmp	.L1702
	.cfi_endproc
.LFE261:
	.size	get_page, .-get_page
	.align 16
	.globl	SwitchTo_SysDir
	.type	SwitchTo_SysDir, @function
SwitchTo_SysDir:
.LFB576:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE576:
	.size	SwitchTo_SysDir, .-SwitchTo_SysDir
	.align 16
	.globl	SwitchFrom_SysDir
	.type	SwitchFrom_SysDir, @function
SwitchFrom_SysDir:
.LFB574:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE574:
	.size	SwitchFrom_SysDir, .-SwitchFrom_SysDir
	.align 16
	.globl	switch_directory
	.type	switch_directory, @function
switch_directory:
.LFB264:
	.cfi_startproc
	movl	4(%esp), %eax
/APP
/  209 "Source/MemManagement/virt_mm/paging.c" 1
	mov %eax, %cr3
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE264:
	.size	switch_directory, .-switch_directory
	.align 16
	.globl	Setup_Paging
	.type	Setup_Paging, @function
Setup_Paging:
.LFB269:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	memmap_generator
	call	setup_frameStack
	call	Setup_SystemDir
	call	Setup_PhyMEM
	movl	system_pdirCap, %eax
	movl	system_dir, %ecx
	movl	$pmem, kmalloc
	movl	$vmem, malloc
	movl	$vfree, free
	movl	$pfree, kfree
	movl	%eax, _cur_pdirCap
	xorl	%eax, %eax
	cmpl	$1, total_CPU_Cores
	movl	%ecx, _cur_dir
	movl	%ecx, _prev_dir
	je	.L1716
	.align 16
.L1717:
	movl	%eax, %edx
	addl	$1, %eax
	sall	$13, %edx
	addl	AP_startup_Code_sz, %edx
	movl	$17028, 12296(%edx)
	movl	total_CPU_Cores, %edx
	subl	$1, %edx
	cmpl	%eax, %edx
	ja	.L1717
.L1716:
/APP
/  209 "Source/MemManagement/virt_mm/paging.c" 1
	mov %ecx, %cr3
/  0 "" 2
/  257 "Source/MemManagement/virt_mm/paging.c" 1
	mov %cr0, %eax;                or $0x80000000, %eax;                mov %eax, %cr0;
/  0 "" 2
/NO_APP
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE269:
	.size	Setup_Paging, .-Setup_Paging
	.section	.rodata
	.align 4
.LC299:
	.string	"\nSecondary Hard Disk sent you an Interrupt\n"
	.text
	.align 16
	.globl	HDD_A_Handler
	.type	HDD_A_Handler, @function
HDD_A_Handler:
.LFB273:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC299
	.cfi_def_cfa_offset 32
	call	printf
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE273:
	.size	HDD_A_Handler, .-HDD_A_Handler
	.section	.rodata
	.align 4
.LC300:
	.string	"\nPrimary Hard Disk sent you an Interrupt\n"
	.text
	.align 16
	.globl	HDD_B_Handler
	.type	HDD_B_Handler, @function
HDD_B_Handler:
.LFB274:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC300
	.cfi_def_cfa_offset 32
	call	printf
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE274:
	.size	HDD_B_Handler, .-HDD_B_Handler
	.section	.rodata
.LC301:
	.string	" Drive 0xA0 exists "
	.align 4
.LC302:
	.string	" Drive 0xA0 not compatible with ATA SPECS "
.LC303:
	.string	"\nHard Disk Initialized\n"
.LC304:
	.string	" PATA Info: Heads: "
.LC305:
	.string	" Sectors: "
.LC306:
	.string	" Cylinders: "
.LC307:
	.string	" Bytes per Sector "
.LC308:
	.string	" Bytes per Track "
.LC309:
	.string	" Serial No: "
.LC310:
	.string	" Drive 0xB0 exists "
	.align 4
.LC311:
	.string	" Drive 0xB0 not compatible with ATA SPECS "
.LC312:
	.string	"\nNO HARDDISK FOUND!!!\n"
	.text
	.align 16
	.globl	init_ata
	.type	init_ata, @function
init_ata:
.LFB275:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	$503, %ebx
	movl	$-96, %eax
	movl	%ebx, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 16
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	xorl	%eax, %eax
	movl	$498, %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$499, %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$500, %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$501, %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$-20, %eax
	movl	%ebx, %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	%al, %al
	je	.L1727
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	$1, %al
	je	.L1767
.L1727:
	movl	$503, %ebx
	movl	$-80, %eax
	movl	%ebx, %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	xorl	%eax, %eax
	movl	$498, %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$499, %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$500, %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$501, %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$-20, %eax
	movl	%ebx, %edx
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	%al, %al
	je	.L1737
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	$1, %al
	je	.L1768
.L1737:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC312
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L1726:
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1768:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC310
	.cfi_def_cfa_offset 32
	call	printf
	movl	%ebx, %edx
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testb	%al, %al
	jns	.L1738
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	$1, %al
	movl	$1000, %ecx
	jne	.L1764
	jmp	.L1738
	.align 16
.L1769:
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	$1, %al
	je	.L1738
	subl	$1, %ecx
	je	.L1726
.L1764:
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	%al, %al
	js	.L1769
.L1738:
	movl	$501, %edx
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	%al, %al
	jne	.L1742
	movl	$499, %edx
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	%al, %al
	jne	.L1742
	movl	$503, %edx
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	$8, %al
	je	.L1726
	movl	$ident, %ecx
	movl	$496, %edx
	.align 16
.L1744:
/APP
/  61 "Library/sys.h" 1
	inw %dx, %ax
/  0 "" 2
/NO_APP
	movw	%ax, (%ecx)
	addl	$2, %ecx
	cmpl	$ident+510, %ecx
	jne	.L1744
	movb	$83, HDD
.L1766:
	movl	$508, %edx
	movl	$2, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC303
	.cfi_def_cfa_offset 32
	call	printf
	movl	$.LC304, (%esp)
	call	printf
	movzwl	ident+6, %eax
	movl	%eax, (%esp)
	call	console_write_dec
	movl	$.LC305, (%esp)
	call	printf
	movzwl	ident+12, %eax
	movl	%eax, (%esp)
	call	console_write_dec
	movl	$.LC306, (%esp)
	call	printf
	movzwl	ident+2, %eax
	movl	%eax, (%esp)
	call	console_write_dec
	movl	$.LC307, (%esp)
	call	printf
	movzwl	ident+10, %eax
	movl	%eax, (%esp)
	call	console_write_dec
	movl	$.LC308, (%esp)
	call	printf
	movzwl	ident+8, %eax
	movl	%eax, (%esp)
	call	console_write_dec
	movl	$.LC309, (%esp)
	call	printf
	movl	$ident+20, (%esp)
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L1726
	.align 16
.L1767:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC301
	.cfi_def_cfa_offset 32
	call	printf
	movl	%ebx, %edx
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testb	%al, %al
	movl	$1000, %ecx
	js	.L1755
	jmp	.L1729
	.align 16
.L1732:
	subl	$1, %ecx
	je	.L1726
.L1755:
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	%al, %al
	js	.L1732
.L1729:
	movl	$501, %edx
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	%al, %al
	jne	.L1733
	movl	$499, %edx
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	%al, %al
	jne	.L1733
	movl	$503, %edx
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	$8, %al
	je	.L1726
	movl	$ident, %ecx
	movl	$ident+510, %ebx
	movl	$496, %edx
	.align 16
.L1736:
/APP
/  61 "Library/sys.h" 1
	inw %dx, %ax
/  0 "" 2
/NO_APP
	movw	%ax, (%ecx)
	addl	$2, %ecx
	cmpl	%ecx, %ebx
	jne	.L1736
	movb	$80, HDD
	jmp	.L1766
	.align 16
.L1742:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC311
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L1726
	.align 16
.L1733:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC302
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L1726
	.cfi_endproc
.LFE275:
	.size	init_ata, .-init_ata
	.align 16
	.globl	read_ata
	.type	read_ata, @function
read_ata:
.LFB276:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	$497, %edx
	movl	8(%esp), %ecx
	xorl	%eax, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$498, %edx
	movl	$1, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$499, %edx
	movl	%ecx, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	%ecx, %eax
	movl	$500, %edx
	shrl	$8, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	%ecx, %eax
	movl	$501, %edx
	shrl	$16, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movzbl	drive, %eax
	shrl	$24, %ecx
	movl	$502, %edx
	andl	$15, %ecx
	sall	$4, %eax
	orl	$-32, %eax
	orl	%ecx, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$503, %edx
	movl	$32, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
	.align 16
/NO_APP
.L1771:
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	$8, %al
	je	.L1771
	movl	12(%esp), %ecx
	movl	$496, %edx
	leal	512(%ecx), %ebx
	.align 16
.L1772:
/APP
/  61 "Library/sys.h" 1
	inw %dx, %ax
/  0 "" 2
/NO_APP
	movb	%al, (%ecx)
	movb	$0, 1(%ecx)
	addl	$2, %ecx
	cmpl	%ecx, %ebx
	jne	.L1772
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE276:
	.size	read_ata, .-read_ata
	.align 16
	.globl	write_ata
	.type	write_ata, @function
write_ata:
.LFB277:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	$497, %edx
	movl	8(%esp), %ebx
	movl	12(%esp), %ecx
	xorl	%eax, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$498, %edx
	movl	$1, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$499, %edx
	movl	%ecx, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	%ecx, %eax
	movl	$500, %edx
	shrl	$8, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	%ecx, %eax
	movl	$501, %edx
	shrl	$16, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movzbl	drive, %eax
	shrl	$24, %ecx
	movl	$502, %edx
	andl	$15, %ecx
	sall	$4, %eax
	orl	$-32, %eax
	orl	%ecx, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	movl	$503, %edx
	movl	$48, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
	.align 16
/NO_APP
.L1778:
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	testb	$8, %al
	je	.L1778
	leal	8(%ebx), %ecx
	movl	$496, %edx
	addl	$520, %ebx
	.align 16
.L1779:
	movzbl	(%ecx), %eax
/APP
/  53 "Library/sys.h" 1
	outw %ax, %dx
/  0 "" 2
/NO_APP
	addl	$2, %ecx
	cmpl	%ecx, %ebx
	jne	.L1779
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE277:
	.size	write_ata, .-write_ata
	.comm	mbr,512,32
	.section	.rodata
.LC313:
	.string	"Partition table found."
.LC314:
	.string	"Partition #%d: @%d+%d"
.LC315:
	.string	"Partition #%d: inactive"
.LC316:
	.string	"Did not find partition table "
.LC317:
	.string	" "
.LC318:
	.string	"Parsing anyone yields:"
.LC319:
	.string	" Partition "
.LC320:
	.string	" : "
.LC321:
	.string	"Partition inactive:"
	.text
	.align 16
	.globl	read_partition_map
	.type	read_partition_map, @function
read_partition_map:
.LFB278:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$4, %esp
	.cfi_def_cfa_offset 16
	pushl	$mbr
	.cfi_def_cfa_offset 20
	pushl	$0
	.cfi_def_cfa_offset 24
	call	read_ata
	cmpb	$85, mbr+510
	popl	%ecx
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_def_cfa_offset 16
	je	.L1796
.L1785:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	$mbr, %esi
	movl	$1, %ebx
	pushl	$.LC316
	.cfi_def_cfa_offset 32
	call	printf
	movzbl	mbr+510, %eax
	movl	%eax, (%esp)
	call	console_write_dec
	movl	$.LC317, (%esp)
	call	printf
	movzbl	mbr+511, %eax
	movl	%eax, (%esp)
	call	console_write_dec
	movl	$.LC318, (%esp)
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L1792:
	cmpb	$0, 446(%esi)
	js	.L1797
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC321
	.cfi_def_cfa_offset 32
	call	printf
	movl	%ebx, (%esp)
	call	console_write_dec
	movl	$.LC317, (%esp)
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L1791:
	addl	$1, %ebx
	addl	$16, %esi
	cmpl	$5, %ebx
	jne	.L1792
	movl	$1, %eax
.L1784:
	addl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1797:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC319
	.cfi_def_cfa_offset 32
	call	printf
	movl	%ebx, (%esp)
	call	console_write_dec
	movl	$.LC320, (%esp)
	call	printf
	popl	%eax
	.cfi_def_cfa_offset 28
	pushl	454(%esi)
	.cfi_def_cfa_offset 32
	call	console_write_dec
	movl	$.LC317, (%esp)
	call	printf
	popl	%edx
	.cfi_def_cfa_offset 28
	pushl	458(%esi)
	.cfi_def_cfa_offset 32
	call	console_write_dec
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L1791
	.align 16
.L1796:
	cmpb	$-86, mbr+511
	jne	.L1785
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	$mbr, %ebx
	movl	$mbr+64, %esi
	pushl	$.LC313
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L1788:
	cmpb	$0, 446(%ebx)
	js	.L1798
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC315
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L1787:
	addl	$16, %ebx
	cmpl	%ebx, %esi
	jne	.L1788
	xorl	%eax, %eax
	jmp	.L1784
	.align 16
.L1798:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC314
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L1787
	.cfi_endproc
.LFE278:
	.size	read_partition_map, .-read_partition_map
	.section	.rodata
.LC322:
	.string	"RSD PTR"
.LC323:
	.string	"\nACPI 1.0 Found\n"
.LC324:
	.string	"\nACPI 2.0 Found\n"
	.text
	.align 16
	.globl	acpiCheckRSDPtr
	.type	acpiCheckRSDPtr, @function
acpiCheckRSDPtr:
.LFB279:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	movl	$.LC322+1, %edx
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %esi
	movzbl	(%esi), %ecx
	leal	1(%esi), %eax
	leal	7(%esi), %edi
	cmpb	$82, %cl
	jne	.L1809
.L1802:
	movzbl	(%eax), %ebx
	cmpb	%bl, (%edx)
	jne	.L1809
	addl	$1, %eax
	addl	$1, %edx
	cmpl	%edi, %eax
	jne	.L1802
	leal	20(%esi), %ebx
	movl	%esi, %eax
	xorl	%edx, %edx
	jmp	.L1804
	.align 16
.L1813:
	movzbl	(%eax), %ecx
.L1804:
	addl	$1, %eax
	addl	%ecx, %edx
	cmpl	%ebx, %eax
	jne	.L1813
	testb	%dl, %dl
	jne	.L1809
	cmpb	$0, 15(%esi)
	je	.L1814
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC324
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L1806:
	movl	16(%esi), %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1814:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC323
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L1806
	.align 16
.L1809:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	xorl	%eax, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE279:
	.size	acpiCheckRSDPtr, .-acpiCheckRSDPtr
	.section	.rodata
.LC325:
	.string	"\nNO RSDPTR FOUND!!!"
	.text
	.align 16
	.globl	acpiGetRSDPtr
	.type	acpiGetRSDPtr, @function
acpiGetRSDPtr:
.LFB280:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$917504, %ebx
	jmp	.L1817
	.align 16
.L1823:
	addl	$4, %ebx
	cmpl	$1048576, %ebx
	je	.L1822
.L1817:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	acpiCheckRSDPtr
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	movl	%eax, %esi
	je	.L1823
.L1815:
	movl	%esi, %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
.L1822:
	.cfi_restore_state
	movswl	1038, %ebx
	sall	$4, %ebx
	andl	$1048560, %ebx
	leal	1023(%ebx), %edi
	jmp	.L1818
	.align 16
.L1825:
	addl	$16, %ebx
	cmpl	%edi, %ebx
	jg	.L1824
.L1818:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	acpiCheckRSDPtr
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	movl	%eax, %esi
	je	.L1825
	jmp	.L1815
.L1824:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC325
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L1815
	.cfi_endproc
.LFE280:
	.size	acpiGetRSDPtr, .-acpiGetRSDPtr
	.align 16
	.globl	acpiCheckHeader
	.type	acpiCheckHeader, @function
acpiCheckHeader:
.LFB281:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	12(%esp), %ecx
	movl	8(%esp), %eax
	movzbl	(%ecx), %ebx
	cmpb	%bl, (%eax)
	jne	.L1834
	movl	$1, %edx
.L1828:
	movzbl	(%ecx,%edx), %ebx
	cmpb	%bl, (%eax,%edx)
	jne	.L1834
	addl	$1, %edx
	cmpl	$4, %edx
	jne	.L1828
	movl	4(%eax), %ebx
	xorl	%edx, %edx
	testl	%ebx, %ebx
	leal	(%eax,%ebx), %ecx
	jle	.L1841
	.align 16
.L1835:
	addb	(%eax), %dl
	addl	$1, %eax
	cmpl	%ecx, %eax
	jne	.L1835
	xorl	%eax, %eax
	testb	%dl, %dl
	setne	%al
	negl	%eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1834:
	.cfi_restore_state
	movl	$-1, %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L1841:
	.cfi_restore_state
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE281:
	.size	acpiCheckHeader, .-acpiCheckHeader
	.section	.rodata
.LC326:
	.string	"enabled acpi.\n"
.LC327:
	.string	"couldn't enable acpi.\n"
.LC328:
	.string	"no known way to enable acpi.\n"
.LC329:
	.string	"acpi was already enabled.\n"
	.text
	.align 16
	.globl	acpiEnable
	.type	acpiEnable, @function
acpiEnable:
.LFB282:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$4, %esp
	.cfi_def_cfa_offset 16
	movl	PM1a_CNT, %edx
/APP
/  61 "Library/sys.h" 1
	inw %dx, %ax
/  0 "" 2
/NO_APP
	movzwl	SCI_EN, %ebx
	testw	%ax, %bx
	jne	.L1843
	movl	%edx, %esi
	movl	SMI_CMD, %edx
	testl	%edx, %edx
	je	.L1844
	movzbl	ACPI_ENABLE, %eax
	testb	%al, %al
	je	.L1844
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	xorl	%ecx, %ecx
	jmp	.L1846
	.align 16
.L1868:
	addl	$1, %ecx
	cmpl	$300, %ecx
	je	.L1847
.L1846:
	movl	%esi, %edx
/APP
/  61 "Library/sys.h" 1
	inw %dx, %ax
/  0 "" 2
/NO_APP
	andl	%ebx, %eax
	cmpw	$1, %ax
	jne	.L1868
	movl	PM1b_CNT, %edx
	testl	%edx, %edx
	je	.L1848
	movl	%edx, %esi
/APP
/  61 "Library/sys.h" 1
	inw %dx, %ax
/  0 "" 2
/NO_APP
	andl	%ebx, %eax
	cmpw	$1, %ax
	jne	.L1850
	jmp	.L1848
	.align 16
.L1851:
	movl	%esi, %edx
/APP
/  61 "Library/sys.h" 1
	inw %dx, %ax
/  0 "" 2
/NO_APP
	andl	%ebx, %eax
	cmpw	$1, %ax
	je	.L1848
.L1850:
	addl	$1, %ecx
	cmpl	$300, %ecx
	jne	.L1851
.L1847:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC327
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$-1, %eax
	jmp	.L1842
	.align 16
.L1848:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC326
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
.L1842:
	addl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1843:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC329
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	addl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
.L1844:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC328
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$-1, %eax
	jmp	.L1842
	.cfi_endproc
.LFE282:
	.size	acpiEnable, .-acpiEnable
	.section	.rodata
.LC330:
	.string	"FACP"
.LC331:
	.string	"DSDT"
.LC332:
	.string	"\\_S5 parse error.\n"
.LC333:
	.string	"\\_S5 not present.\n"
.LC334:
	.string	"DSDT invalid.\n"
.LC335:
	.string	"no valid FACP present.\n"
.LC336:
	.string	"no acpi.\n"
	.text
	.align 16
	.globl	initAcpi
	.type	initAcpi, @function
initAcpi:
.LFB283:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	call	acpiGetRSDPtr
	testl	%eax, %eax
	je	.L1870
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	movl	%eax, %esi
	pushl	$.LC220
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	acpiCheckHeader
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	jne	.L1870
	movl	4(%esi), %edx
	leal	36(%esi), %ebx
	leal	-33(%edx), %eax
	subl	$36, %edx
	cmovns	%edx, %eax
	sarl	$2, %eax
	testl	%eax, %eax
	jle	.L1871
	leal	36(%esi,%eax,4), %edi
	.align 16
.L1885:
	movl	(%ebx), %esi
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	$.LC330
	.cfi_def_cfa_offset 44
	pushl	%esi
	.cfi_def_cfa_offset 48
	call	acpiCheckHeader
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	jne	.L1872
	movl	40(%esi), %ebx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	$.LC331
	.cfi_def_cfa_offset 44
	pushl	%ebx
	.cfi_def_cfa_offset 48
	call	acpiCheckHeader
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	jne	.L1873
	movl	4(%ebx), %ecx
	leal	36(%ebx), %edi
	leal	37(%ebx), %edx
	leal	-36(%ecx), %ebp
	subl	$37, %ecx
	testl	%ebp, %ebp
	jle	.L1875
	.align 16
.L1894:
	cmpb	$95, -1(%edx)
	leal	-1(%edx), %ebx
	jne	.L1878
	cmpb	$83, (%edx)
	jne	.L1878
	cmpb	$53, 1(%edx)
	jne	.L1878
	cmpb	$95, 2(%edx)
	jne	.L1878
	movl	%ebx, %edi
.L1875:
	testl	%ecx, %ecx
	jle	.L1877
	movzbl	-1(%edi), %edx
	cmpb	$8, %dl
	je	.L1880
	cmpb	$8, -2(%edi)
	jne	.L1881
	cmpb	$92, %dl
	je	.L1880
.L1881:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC332
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L1871:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC335
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	$-1, %eax
.L1869:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1878:
	.cfi_restore_state
	subl	$1, %ecx
	addl	$1, %edx
	cmpl	$-1, %ecx
	jne	.L1894
.L1877:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC333
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1871
	.align 16
.L1872:
	addl	$4, %ebx
	cmpl	%ebx, %edi
	jne	.L1885
	jmp	.L1871
	.align 16
.L1873:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC334
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L1871
	.align 16
.L1880:
	cmpb	$18, 4(%edi)
	jne	.L1881
	movsbl	5(%edi), %edx
	sarl	$6, %edx
	andl	$3, %edx
	leal	7(%edi,%edx), %edx
	movzbl	(%edx), %ecx
	cmpb	$10, %cl
	jne	.L1882
	movzbl	1(%edx), %ecx
	addl	$1, %edx
.L1882:
	sall	$10, %ecx
	movw	%cx, SLP_TYPa
	movzbl	1(%edx), %ecx
	cmpb	$10, %cl
	jne	.L1883
	movzbl	2(%edx), %ecx
.L1883:
	movl	48(%esi), %edx
	sall	$10, %ecx
	movw	%cx, SLP_TYPb
	movl	$1, %ecx
	movw	%cx, SCI_EN
	movl	%edx, SMI_CMD
	movzbl	52(%esi), %edx
	movb	%dl, ACPI_ENABLE
	movzbl	53(%esi), %edx
	movb	%dl, ACPI_DISABLE
	movl	64(%esi), %edx
	movl	%edx, PM1a_CNT
	movl	68(%esi), %edx
	movl	%edx, PM1b_CNT
	movzbl	89(%esi), %edx
	movb	%dl, PM1_CNT_LEN
	movl	$8192, %edx
	movw	%dx, SLP_EN
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L1870:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC336
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	$-1, %eax
	jmp	.L1869
	.cfi_endproc
.LFE283:
	.size	initAcpi, .-initAcpi
	.align 16
	.globl	acpiPowerOff
	.type	acpiPowerOff, @function
acpiPowerOff:
.LFB284:
	.cfi_startproc
	cmpw	$0, SCI_EN
	jne	.L1905
	rep ret
	.align 16
.L1905:
	jmp	acpiPowerOff.part.8
	.cfi_endproc
.LFE284:
	.size	acpiPowerOff, .-acpiPowerOff
	.section	.rodata
.LC337:
	.string	"\nAddr: %x Size: %x"
	.text
	.align 16
	.globl	Setup_VMEM
	.type	Setup_VMEM, @function
Setup_VMEM:
.LFB285:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %esi
	movl	mmap_info, %eax
	leal	4112(%esi), %edi
	leal	8208(%esi), %ebx
	movl	$0, 8196(%esi)
	movl	$0, 4100(%esi)
	movl	%edi, 4108(%esi)
	movl	%ebx, 8204(%esi)
	jmp	.L1907
	.align 16
.L1908:
	movl	12(%eax), %edx
	movl	%ebx, (%ebx)
	addl	$16, %ebx
	movl	%edx, -8(%ebx)
	movl	4(%eax), %edx
	movl	%edx, -12(%ebx)
	addl	$1, 8192(%esi)
	movl	%eax, %edx
.L1909:
	cmpl	$1048130, 40(%edx)
	leal	24(%edx), %eax
	movl	%eax, mmap_info
	jne	.L1910
.L1907:
	testl	%eax, %eax
	je	.L1910
	cmpl	$6, 20(%eax)
	ja	.L1908
	movl	12(%eax), %edx
	movl	4(%eax), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 20
	movl	%edi, (%edi)
	addl	$16, %edi
	movl	%edx, -8(%edi)
	movl	%eax, -12(%edi)
	pushl	%edx
	.cfi_def_cfa_offset 24
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC337
	.cfi_def_cfa_offset 32
	call	printf
	addl	$1, 4096(%esi)
	movl	mmap_info, %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L1909
	.align 16
.L1910:
	movl	4108(%esi), %eax
	movl	%edi, 4104(%esi)
	movl	%eax, (%edi)
	movl	8204(%esi), %eax
	movl	%ebx, 8200(%esi)
	movl	%eax, (%ebx)
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE285:
	.size	Setup_VMEM, .-Setup_VMEM
	.align 16
	.globl	vmem
	.type	vmem, @function
vmem:
.LFB286:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	48(%esp), %ebx
	call	Get_Scheduler
	movl	56(%eax), %esi
	movl	system_dir, %eax
/APP
/  209 "Source/MemManagement/virt_mm/paging.c" 1
	mov %eax, %cr3
/  0 "" 2
/NO_APP
	movl	4108(%esi), %eax
	movl	(%eax), %edi
	movl	8(%edi), %eax
	cmpl	%eax, %ebx
	jbe	.L1918
	.align 16
.L1930:
	movl	16(%edi), %edi
	movl	8(%edi), %eax
	cmpl	%ebx, %eax
	jb	.L1930
.L1918:
	movl	8200(%esi), %ebp
	subl	%ebx, %eax
	movl	%eax, 8(%edi)
	movl	4(%edi), %eax
	leal	32(%ebp), %ecx
	movl	%ebx, 8(%ebp)
	movl	%ebp, 0(%ebp)
	movl	%eax, 4(%ebp)
	addl	$1, 8192(%esi)
	andl	$4095, %ecx
	movl	%eax, 4(%esp)
	leal	16(%ebp), %eax
	je	.L1936
.L1915:
	movl	8204(%esi), %edx
	movl	%edx, (%eax)
	movl	4(%esp), %eax
	addl	%ebx, 4(%edi)
	movl	%eax, %ebp
	shrl	$22, %eax
	leal	(%esi,%eax,4), %eax
	shrl	$12, %ebp
	movl	%ebp, %ecx
	movl	%eax, 12(%esp)
	movl	(%eax), %eax
	andl	$1023, %ecx
	movl	%ecx, 8(%esp)
	testl	%eax, %eax
	je	.L1937
	movl	8(%esp), %ecx
	andl	$-4096, %eax
	leal	(%eax,%ecx,4), %edi
	movl	%ebx, %eax
	shrl	$12, %eax
	addl	%ebp, %eax
	cmpl	%eax, %ebp
	jnb	.L1920
	movl	%ebx, 48(%esp)
	movl	%edi, %ebx
	movl	%eax, %edi
	.align 16
.L1928:
	testl	$1023, %ebp
	jne	.L1922
	movl	%ebp, %eax
	shrl	$10, %eax
	movl	(%esi,%eax,4), %edx
	andl	$-4096, %edx
	movl	%edx, %ebx
.L1922:
	call	phy_alloc4K
	addl	$1, %ebp
	orl	$3075, %eax
	addl	$4, %ebx
	movl	%eax, -4(%ebx)
	cmpl	%edi, %ebp
	jne	.L1928
	movl	48(%esp), %ebx
.L1920:
	andl	$4095, %ebx
	je	.L1925
	movl	12(%esp), %eax
	movl	8(%esp), %ecx
	movl	(%eax), %eax
	andl	$-4096, %eax
	leal	(%eax,%ecx,4), %ebx
	movl	(%ebx), %eax
	testl	%eax, %eax
	je	.L1938
.L1925:
/APP
/  209 "Source/MemManagement/virt_mm/paging.c" 1
	mov %esi, %cr3
/  0 "" 2
/NO_APP
	movl	4(%esp), %eax
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1938:
	.cfi_restore_state
	call	phy_alloc4K
	orl	$1027, %eax
	movl	%eax, (%ebx)
	jmp	.L1925
	.align 16
.L1937:
	call	phy_alloc4K
	movl	12(%esp), %ecx
	andb	$243, %ah
	orl	$3, %eax
	movl	%eax, (%ecx)
	jmp	.L1920
	.align 16
.L1936:
	call	phy_alloc4K
	movl	%eax, 16(%ebp)
	addl	$1, 8192(%esi)
	jmp	.L1915
	.cfi_endproc
.LFE286:
	.size	vmem, .-vmem
	.align 16
	.globl	vfree
	.type	vfree, @function
vfree:
.LFB287:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	movl	system_pdirCap, %esi
	movl	32(%esp), %ecx
	movl	8192(%esi), %edx
	movl	8204(%esi), %eax
	testl	%edx, %edx
	je	.L1939
	movl	(%eax), %ebx
	cmpl	4(%ebx), %ecx
	je	.L1941
	xorl	%eax, %eax
	jmp	.L1943
	.align 16
.L1948:
	movl	16(%ebx), %ebx
	cmpl	4(%ebx), %ecx
	je	.L1941
.L1943:
	addl	$1, %eax
	cmpl	%edx, %eax
	jne	.L1948
.L1939:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1941:
	.cfi_restore_state
	movl	4104(%esi), %edi
	movl	8(%ebx), %eax
	movl	%eax, 8(%edi)
	leal	32(%edi), %eax
	movl	%edi, (%edi)
	movl	%ecx, 4(%edi)
	movl	$0, 12(%edi)
	leal	16(%edi), %ebp
	addl	$1, 4096(%esi)
	testl	$4095, %eax
	je	.L1957
.L1944:
	movl	4108(%esi), %eax
	movl	8(%ebx), %ecx
	movl	%eax, 0(%ebp)
	movl	4(%ebx), %eax
	testl	%ecx, %ecx
	movl	%ebp, 4104(%esi)
	leal	(%ecx,%eax), %edx
	je	.L1947
	.align 16
.L1952:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %edx
	jne	.L1952
.L1947:
	movl	$0, 4(%ebx)
	movl	$0, 8(%ebx)
	movl	$1, 12(%ebx)
	subl	$1, 8192(%esi)
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L1957:
	.cfi_restore_state
	call	phy_alloc4K
	movl	%eax, 16(%edi)
	movl	%ebp, (%eax)
	leal	16(%eax), %ebp
	addl	$1, 8192(%esi)
	jmp	.L1944
	.cfi_endproc
.LFE287:
	.size	vfree, .-vfree
	.align 16
	.globl	keyboard_scancodes
	.type	keyboard_scancodes, @function
keyboard_scancodes:
.LFB288:
	.cfi_startproc
	movl	_capslock, %eax
	movl	4(%esp), %ecx
	testl	%eax, %eax
	jne	.L1959
	movl	_shift, %eax
	testl	%eax, %eax
	je	.L1997
	leal	-97(%ecx), %eax
	cmpl	$25, %eax
	jbe	.L1999
.L1991:
	movl	%ecx, %eax
.L1992:
	subl	$48, %eax
	cmpl	$9, %eax
	ja	.L1963
.L2000:
	leal	-49(%ecx), %eax
	cmpl	$8, %eax
	ja	.L1964
	jmp	*.L1966(,%eax,4)
	.section	.rodata
	.align 4
	.align 4
.L1966:
	.long	.L1994
	.long	.L1967
	.long	.L1968
	.long	.L1969
	.long	.L1970
	.long	.L1971
	.long	.L1972
	.long	.L1973
	.long	.L1974
	.text
	.align 16
.L1959:
	leal	-97(%ecx), %eax
	leal	-32(%ecx), %edx
	cmpl	$26, %eax
	movl	%ecx, %eax
	cmovb	%edx, %eax
	movl	_shift, %edx
	testl	%edx, %edx
	je	.L1958
	leal	-65(%eax), %edx
	movl	%eax, %ecx
	cmpl	$25, %edx
	ja	.L1992
	leal	32(%eax), %ecx
	movl	%ecx, %eax
	subl	$48, %eax
	cmpl	$9, %eax
	jbe	.L2000
.L1963:
	leal	-13(%ecx), %eax
	cmpl	$83, %eax
	ja	.L1997
	jmp	*.L1977(,%eax,4)
	.section	.rodata
	.align 4
	.align 4
.L1977:
	.long	.L1976
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1978
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1979
	.long	.L1995
	.long	.L1980
	.long	.L1981
	.long	.L1982
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1983
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1997
	.long	.L1984
	.long	.L1985
	.long	.L1986
	.long	.L1997
	.long	.L1997
	.long	.L1987
	.text
.L1999:
	subl	$32, %ecx
	cmpl	$0, _shift
	jne	.L1991
	.align 16
.L1997:
	movl	%ecx, %eax
.L1958:
	rep ret
	.align 16
.L1994:
	movl	$33, %eax
	ret
	.align 16
.L1973:
	movl	$42, %eax
	ret
	.align 16
.L1968:
	movl	$35, %eax
	ret
	.align 16
.L1969:
	movl	$36, %eax
	ret
	.align 16
.L1967:
	movl	$64, %eax
	ret
	.align 16
.L1972:
	movl	$38, %eax
	ret
	.align 16
.L1970:
	movl	$37, %eax
	ret
	.align 16
.L1971:
	movl	$94, %eax
	ret
	.align 16
.L1995:
	movl	$60, %eax
	ret
	.align 16
.L1980:
	movl	$95, %eax
	ret
	.align 16
.L1981:
	movl	$62, %eax
	ret
	.align 16
.L1982:
	movl	$63, %eax
	ret
	.align 16
.L1983:
	movl	$58, %eax
	ret
	.align 16
.L1976:
	movl	$10, %eax
	ret
	.align 16
.L1978:
	movl	$34, %eax
	ret
	.align 16
.L1979:
	movl	$61, %eax
	ret
	.align 16
.L1984:
	movl	$123, %eax
	ret
	.align 16
.L1985:
	movl	$124, %eax
	ret
	.align 16
.L1986:
	movl	$125, %eax
	ret
	.align 16
.L1987:
	movl	$126, %eax
	ret
.L1974:
	movl	$40, %eax
	ret
	.align 16
.L1964:
	movl	$41, %eax
	ret
	.cfi_endproc
.LFE288:
	.size	keyboard_scancodes, .-keyboard_scancodes
	.align 16
	.globl	keyboard_init
	.type	keyboard_init, @function
keyboard_init:
.LFB289:
	.cfi_startproc
	.align 16
.L2002:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L2002
	movl	$-19, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
	.align 16
/NO_APP
.L2003:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L2003
	movl	$5, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
/NO_APP
	movl	$Main_key_codes, scancodes
	ret
	.cfi_endproc
.LFE289:
	.size	keyboard_init, .-keyboard_init
	.align 16
	.globl	create_task
	.type	create_task, @function
create_task:
.LFB290:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	movl	44(%esp), %esi
	movl	52(%esp), %ebx
	movl	60(%esp), %edi
	pushl	$4
	.cfi_def_cfa_offset 48
	call	mtalloc
	leal	16384(%eax), %ecx
	movl	%eax, %ebp
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, %edx
	.align 16
.L2007:
	addl	$4, %edx
	movl	$0, -4(%edx)
	cmpl	%edx, %ecx
	jne	.L2007
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	movl	$0, 102(%ebp)
	pushl	(%edi)
	.cfi_def_cfa_offset 40
	pushl	$16384
	.cfi_def_cfa_offset 44
	pushl	%ebp
	.cfi_def_cfa_offset 48
	call	map
	movl	48(%edi), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L2014
	movl	52(%edi), %eax
	movl	%ebp, 102(%eax)
	movl	%eax, 106(%ebp)
	movl	%ebp, 52(%edi)
.L2009:
	addl	$1, 56(%edi)
	addl	$1, 60(%edi)
	leal	52(%ebp), %eax
	movzbl	(%esi), %ecx
	movl	%esi, %edx
	.align 16
.L2010:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L2010
	movb	$0, (%eax)
	movl	36(%edi), %eax
	movl	60(%edi), %edx
	movl	%ebx, 16(%ebp)
	movl	%edi, 8(%ebp)
	movl	%eax, 4(%ebp)
	movl	(%edi), %eax
	movl	%edx, 40(%ebp)
	movl	$0, 36(%ebp)
	movl	$8, 16372(%ebp)
	movl	$0, 16364(%ebp)
	movl	%eax, 12(%ebp)
	movl	%eax, 32(%ebp)
	movl	44(%esp), %eax
	movl	$0, 16360(%ebp)
	movl	$0, 16356(%ebp)
	movl	$0, 16352(%ebp)
	movl	$0, 16340(%ebp)
	movl	$0, 16336(%ebp)
	movl	%eax, 16376(%ebp)
	movl	36(%esp), %eax
	movl	%ebx, 20(%ebp)
	movl	%eax, 16368(%ebp)
	leal	16364(%ebp), %eax
	movl	%eax, 16348(%ebp)
	movl	%eax, 16344(%ebp)
	leal	16336(%ebp), %eax
	movl	%eax, 0(%ebp)
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebp, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L2014:
	.cfi_restore_state
	movl	%ebp, 48(%edi)
	movl	%ebp, 52(%edi)
	movl	%ebp, 106(%ebp)
	jmp	.L2009
	.cfi_endproc
.LFE290:
	.size	create_task, .-create_task
	.align 16
	.globl	Task_Refresh
	.type	Task_Refresh, @function
Task_Refresh:
.LFB291:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	8(%esp), %edx
	movl	%edx, 16368(%eax)
	leal	16364(%eax), %edx
	movl	$0, 36(%eax)
	movl	$514, 16376(%eax)
	movl	$8, 16372(%eax)
	movl	%edx, 16348(%eax)
	movl	%edx, 16344(%eax)
	leal	16336(%eax), %edx
	movl	$0, 16364(%eax)
	movl	$0, 16360(%eax)
	movl	$0, 16356(%eax)
	movl	$0, 16352(%eax)
	movl	$0, 16340(%eax)
	movl	$0, 16336(%eax)
	movl	%edx, (%eax)
	ret
	.cfi_endproc
.LFE291:
	.size	Task_Refresh, .-Task_Refresh
	.align 16
	.globl	Activate_task
	.type	Activate_task, @function
Activate_task:
.LFB292:
	.cfi_startproc
	subl	$76, %esp
	.cfi_def_cfa_offset 80
	movl	80(%esp), %eax
	movl	%eax, 60(%esp)
	call	FindLightestScheduler
	movl	%eax, 56(%esp)
	call	Get_Scheduler
	cmpl	56(%esp), %eax
	je	.L2017
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$44
	.cfi_def_cfa_offset 96
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, 52(%esp)
	movl	52(%esp), %eax
	movl	$2, (%eax)
	movl	52(%esp), %eax
	movl	$0, 16(%eax)
	call	Get_Scheduler
	movl	%eax, %edx
	movl	52(%esp), %eax
	movl	%edx, 4(%eax)
	movl	52(%esp), %eax
	movl	56(%esp), %edx
	movl	%edx, 8(%eax)
	movl	52(%esp), %eax
	movl	60(%esp), %edx
	movl	%edx, 12(%eax)
	movl	52(%esp), %eax
	movl	$0, 36(%eax)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	64(%esp)
	.cfi_def_cfa_offset 96
	call	IPCdispatch
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	nop
	jmp	.L2016
.L2017:
	movl	60(%esp), %eax
	movl	56(%esp), %edx
	movl	%edx, 44(%eax)
	movl	56(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, 48(%esp)
	movl	60(%esp), %eax
	movl	16(%eax), %edx
	movl	$0, %eax
	subl	%edx, %eax
	sall	$12, %eax
	addl	$81920, %eax
	addl	%eax, 48(%esp)
	movl	48(%esp), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	48(%esp), %eax
	movl	%edx, (%eax)
	movl	48(%esp), %eax
	movl	(%eax), %eax
	leal	0(,%eax,4), %edx
	movl	48(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 44(%esp)
	movl	60(%esp), %edx
	movl	44(%esp), %eax
	movl	%edx, (%eax)
	movl	44(%esp), %edx
	movl	60(%esp), %eax
	movl	%edx, 24(%eax)
	movl	56(%esp), %eax
	movl	16(%eax), %eax
	cmpl	48(%esp), %eax
	jbe	.L2019
	movl	56(%esp), %eax
	movl	48(%esp), %edx
	movl	%edx, 16(%eax)
.L2019:
	movl	56(%esp), %eax
	movl	$0, 20(%eax)
.L2016:
	addl	$76, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE292:
	.size	Activate_task, .-Activate_task
	.align 16
	.globl	Activate_task_direct
	.type	Activate_task_direct, @function
Activate_task_direct:
.LFB293:
	.cfi_startproc
	subl	$76, %esp
	.cfi_def_cfa_offset 80
	call	FindLightestScheduler
	movl	%eax, 60(%esp)
	call	Get_Scheduler
	cmpl	60(%esp), %eax
	je	.L2021
	movl	multitasking_ON, %eax
	testl	%eax, %eax
	je	.L2021
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$44
	.cfi_def_cfa_offset 96
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, 56(%esp)
	movl	56(%esp), %eax
	movl	$2, (%eax)
	movl	56(%esp), %eax
	movl	$0, 16(%eax)
	call	Get_Scheduler
	movl	%eax, %edx
	movl	56(%esp), %eax
	movl	%edx, 4(%eax)
	movl	56(%esp), %eax
	movl	60(%esp), %edx
	movl	%edx, 8(%eax)
	movl	56(%esp), %eax
	movl	80(%esp), %edx
	movl	%edx, 12(%eax)
	movl	56(%esp), %eax
	movl	$0, 36(%eax)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	68(%esp)
	.cfi_def_cfa_offset 96
	call	IPCdispatch
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	nop
	jmp	.L2020
.L2021:
	movl	80(%esp), %eax
	movl	60(%esp), %edx
	movl	%edx, 44(%eax)
	movl	60(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, 52(%esp)
	movl	80(%esp), %eax
	movl	16(%eax), %edx
	movl	$0, %eax
	subl	%edx, %eax
	sall	$12, %eax
	addl	$81920, %eax
	addl	%eax, 52(%esp)
	movl	52(%esp), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	52(%esp), %eax
	movl	%edx, (%eax)
	movl	52(%esp), %eax
	movl	(%eax), %eax
	leal	0(,%eax,4), %edx
	movl	52(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 48(%esp)
	movl	80(%esp), %edx
	movl	48(%esp), %eax
	movl	%edx, (%eax)
	movl	48(%esp), %edx
	movl	80(%esp), %eax
	movl	%edx, 24(%eax)
	movl	60(%esp), %eax
	movl	16(%eax), %eax
	cmpl	52(%esp), %eax
	jbe	.L2023
	movl	60(%esp), %eax
	movl	52(%esp), %edx
	movl	%edx, 16(%eax)
.L2023:
	movl	60(%esp), %eax
	movl	$0, 20(%eax)
	movl	80(%esp), %eax
	movl	16(%eax), %edx
	movl	80(%esp), %eax
	movl	%edx, 20(%eax)
	movl	60(%esp), %eax
	movl	4(%eax), %eax
	leal	1(%eax), %edx
	movl	60(%esp), %eax
	movl	%edx, 4(%eax)
.L2020:
	addl	$76, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE293:
	.size	Activate_task_direct, .-Activate_task_direct
	.align 16
	.globl	Activate_task_direct_SP
	.type	Activate_task_direct_SP, @function
Activate_task_direct_SP:
.LFB294:
	.cfi_startproc
	subl	$76, %esp
	.cfi_def_cfa_offset 80
	call	Get_Scheduler
	cmpl	84(%esp), %eax
	je	.L2025
	movl	multitasking_ON, %eax
	testl	%eax, %eax
	je	.L2025
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$44
	.cfi_def_cfa_offset 96
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, 60(%esp)
	movl	60(%esp), %eax
	movl	$2, (%eax)
	movl	60(%esp), %eax
	movl	$0, 16(%eax)
	call	Get_Scheduler
	movl	%eax, %edx
	movl	60(%esp), %eax
	movl	%edx, 4(%eax)
	movl	60(%esp), %eax
	movl	84(%esp), %edx
	movl	%edx, 8(%eax)
	movl	60(%esp), %eax
	movl	80(%esp), %edx
	movl	%edx, 12(%eax)
	movl	60(%esp), %eax
	movl	$0, 36(%eax)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	72(%esp)
	.cfi_def_cfa_offset 96
	call	IPCdispatch
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	nop
	jmp	.L2024
.L2025:
	movl	80(%esp), %eax
	movl	84(%esp), %edx
	movl	%edx, 44(%eax)
	movl	84(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, 56(%esp)
	movl	80(%esp), %eax
	movl	16(%eax), %edx
	movl	$0, %eax
	subl	%edx, %eax
	sall	$12, %eax
	addl	$81920, %eax
	addl	%eax, 56(%esp)
	movl	56(%esp), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	56(%esp), %eax
	movl	%edx, (%eax)
	movl	56(%esp), %eax
	movl	(%eax), %eax
	leal	0(,%eax,4), %edx
	movl	56(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 52(%esp)
	movl	80(%esp), %edx
	movl	52(%esp), %eax
	movl	%edx, (%eax)
	movl	52(%esp), %edx
	movl	80(%esp), %eax
	movl	%edx, 24(%eax)
	movl	84(%esp), %eax
	movl	16(%eax), %eax
	cmpl	56(%esp), %eax
	jbe	.L2027
	movl	84(%esp), %eax
	movl	56(%esp), %edx
	movl	%edx, 16(%eax)
.L2027:
	movl	84(%esp), %eax
	movl	$0, 20(%eax)
	movl	84(%esp), %eax
	movl	4(%eax), %eax
	leal	1(%eax), %edx
	movl	84(%esp), %eax
	movl	%edx, 4(%eax)
.L2024:
	addl	$76, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE294:
	.size	Activate_task_direct_SP, .-Activate_task_direct_SP
	.align 16
	.globl	Task_Swap
	.type	Task_Swap, @function
Task_Swap:
.LFB295:
	.cfi_startproc
	subl	$76, %esp
	.cfi_def_cfa_offset 80
	movl	84(%esp), %eax
	movl	44(%eax), %eax
	movl	%eax, 60(%esp)
	call	Get_Scheduler
	cmpl	60(%esp), %eax
	je	.L2029
	movl	multitasking_ON, %eax
	testl	%eax, %eax
	je	.L2029
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$44
	.cfi_def_cfa_offset 96
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, 56(%esp)
	movl	56(%esp), %eax
	movl	$8, (%eax)
	movl	56(%esp), %eax
	movl	$1, 16(%eax)
	call	Get_Scheduler
	movl	%eax, %edx
	movl	56(%esp), %eax
	movl	%edx, 4(%eax)
	movl	56(%esp), %eax
	movl	60(%esp), %edx
	movl	%edx, 8(%eax)
	movl	56(%esp), %eax
	movl	80(%esp), %edx
	movl	%edx, 12(%eax)
	movl	84(%esp), %edx
	movl	56(%esp), %eax
	movl	%edx, 20(%eax)
	movl	56(%esp), %eax
	movl	$0, 36(%eax)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	68(%esp)
	.cfi_def_cfa_offset 96
	call	IPCdispatch
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	nop
	jmp	.L2028
.L2029:
	movl	84(%esp), %eax
	movl	24(%eax), %eax
	movl	%eax, %edx
	movl	80(%esp), %eax
	movl	%eax, (%edx)
	movl	84(%esp), %eax
	movl	24(%eax), %edx
	movl	80(%esp), %eax
	movl	%edx, 24(%eax)
	movl	84(%esp), %eax
	movl	$0, 24(%eax)
	movl	80(%esp), %eax
	movl	60(%esp), %edx
	movl	%edx, 44(%eax)
.L2028:
	addl	$76, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE295:
	.size	Task_Swap, .-Task_Swap
	.align 16
	.globl	kill
	.type	kill, @function
kill:
.LFB296:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	nop
.L2032:
	movl	$0, %eax
	movl	$1, %edx
	lock cmpxchgl	%edx, TASK_LOCK_KILLLocked
	sete	%al
	xorl	$1, %eax
	testb	%al, %al
	jne	.L2032
	lock orl	$0, (%esp)
/APP
/  319 "Source/multitasking/task.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	system_dir, %eax
	pushl	%eax
	.cfi_def_cfa_offset 36
	call	switch_directory
	addl	$4, %esp
	.cfi_def_cfa_offset 32
	call	Get_Scheduler
	movl	%eax, 12(%esp)
	movl	12(%esp), %eax
	movl	44(%eax), %eax
	movl	24(%eax), %eax
	movl	%eax, 8(%esp)
	movl	12(%esp), %eax
	movl	60(%eax), %eax
	movl	%eax, %edx
	movl	8(%esp), %eax
	movl	%edx, (%eax)
	movl	12(%esp), %eax
	movl	44(%eax), %eax
	movl	8(%eax), %eax
	movl	%eax, 4(%esp)
	movl	4(%esp), %eax
	movl	56(%eax), %eax
	leal	-1(%eax), %edx
	movl	4(%esp), %eax
	movl	%edx, 56(%eax)
	movl	12(%esp), %eax
	movl	44(%eax), %eax
	movl	%eax, (%esp)
	movl	(%esp), %eax
	movl	106(%eax), %eax
	movl	(%esp), %edx
	movl	102(%edx), %edx
	movl	%edx, 102(%eax)
	movl	(%esp), %eax
	movl	102(%eax), %eax
	movl	(%esp), %edx
	movl	106(%edx), %edx
	movl	%edx, 106(%eax)
	movl	12(%esp), %eax
	movl	44(%eax), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	pushl	$16
	.cfi_def_cfa_offset 40
	pushl	$0
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	memset_fast
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	Idle_task, %eax
	movl	%eax, %edx
	movl	12(%esp), %eax
	movl	%edx, 44(%eax)
	movl	12(%esp), %eax
	movl	4(%eax), %eax
	leal	-1(%eax), %edx
	movl	12(%esp), %eax
	movl	%edx, 4(%eax)
	movl	system_dir, %edx
	movl	12(%esp), %eax
	movl	%edx, 56(%eax)
	lock orl	$0, (%esp)
	movl	$0, TASK_LOCK_KILLLocked
/APP
/  337 "Source/multitasking/task.c" 1
	int $51
/  0 "" 2
/NO_APP
	nop
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE296:
	.size	kill, .-kill
	.align 16
	.globl	_kill
	.type	_kill, @function
_kill:
.LFB297:
	.cfi_startproc
	subl	$76, %esp
	.cfi_def_cfa_offset 80
	movl	80(%esp), %eax
	movl	44(%eax), %eax
	movl	%eax, 60(%esp)
	call	Get_Scheduler
	cmpl	60(%esp), %eax
	je	.L2034
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$44
	.cfi_def_cfa_offset 96
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, 56(%esp)
	movl	56(%esp), %eax
	movl	$7, (%eax)
	movl	56(%esp), %eax
	movl	$0, 16(%eax)
	call	Get_Scheduler
	movl	%eax, %edx
	movl	56(%esp), %eax
	movl	%edx, 4(%eax)
	movl	56(%esp), %eax
	movl	60(%esp), %edx
	movl	%edx, 8(%eax)
	movl	56(%esp), %eax
	movl	80(%esp), %edx
	movl	%edx, 12(%eax)
	movl	56(%esp), %eax
	movl	$0, 36(%eax)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	68(%esp)
	.cfi_def_cfa_offset 96
	call	IPCdispatch
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	nop
	jmp	.L2033
.L2034:
	movl	80(%esp), %eax
	movl	24(%eax), %eax
	movl	%eax, 52(%esp)
	movl	60(%esp), %eax
	movl	60(%eax), %eax
	movl	%eax, %edx
	movl	52(%esp), %eax
	movl	%edx, (%eax)
	movl	80(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, 48(%esp)
	movl	48(%esp), %eax
	movl	56(%eax), %eax
	leal	-1(%eax), %edx
	movl	48(%esp), %eax
	movl	%edx, 56(%eax)
	movl	80(%esp), %eax
	movl	%eax, 44(%esp)
	movl	44(%esp), %eax
	movl	106(%eax), %eax
	movl	44(%esp), %edx
	movl	102(%edx), %edx
	movl	%edx, 102(%eax)
	movl	44(%esp), %eax
	movl	102(%eax), %eax
	movl	44(%esp), %edx
	movl	106(%edx), %edx
	movl	%edx, 106(%eax)
	subl	$4, %esp
	.cfi_def_cfa_offset 84
	pushl	$16
	.cfi_def_cfa_offset 88
	pushl	$0
	.cfi_def_cfa_offset 92
	pushl	92(%esp)
	.cfi_def_cfa_offset 96
	call	memset_fast
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	80(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 88
	pushl	$2
	.cfi_def_cfa_offset 92
	pushl	%eax
	.cfi_def_cfa_offset 96
	call	mtfree
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	60(%esp), %eax
	movl	4(%eax), %eax
	leal	-1(%eax), %edx
	movl	60(%esp), %eax
	movl	%edx, 4(%eax)
.L2033:
	addl	$76, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE297:
	.size	_kill, .-_kill
	.align 16
	.globl	kill_with_func
	.type	kill_with_func, @function
kill_with_func:
.LFB298:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
/APP
/  378 "Source/multitasking/task.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	system_dir, %eax
	pushl	%eax
	.cfi_def_cfa_offset 36
	call	switch_directory
	addl	$4, %esp
	.cfi_def_cfa_offset 32
	call	Get_Scheduler
	movl	%eax, 12(%esp)
	movl	12(%esp), %eax
	movl	44(%eax), %eax
	movl	24(%eax), %eax
	movl	%eax, 8(%esp)
	movl	12(%esp), %eax
	movl	60(%eax), %eax
	movl	%eax, %edx
	movl	8(%esp), %eax
	movl	%edx, (%eax)
	movl	12(%esp), %eax
	movl	44(%eax), %eax
	movl	8(%eax), %eax
	movl	%eax, 4(%esp)
	movl	4(%esp), %eax
	movl	56(%eax), %eax
	leal	-1(%eax), %edx
	movl	4(%esp), %eax
	movl	%edx, 56(%eax)
	movl	12(%esp), %eax
	movl	44(%eax), %eax
	movl	%eax, (%esp)
	movl	(%esp), %eax
	movl	106(%eax), %eax
	movl	(%esp), %edx
	movl	102(%edx), %edx
	movl	%edx, 102(%eax)
	movl	(%esp), %eax
	movl	102(%eax), %eax
	movl	(%esp), %edx
	movl	106(%edx), %edx
	movl	%edx, 106(%eax)
	movl	12(%esp), %eax
	movl	44(%eax), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	pushl	$16
	.cfi_def_cfa_offset 40
	pushl	$0
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	memset_fast
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	12(%esp), %eax
	movl	44(%eax), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	$2
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	mtfree
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	Idle_task, %eax
	movl	%eax, %edx
	movl	12(%esp), %eax
	movl	%edx, 44(%eax)
	movl	12(%esp), %eax
	movl	4(%eax), %eax
	leal	-1(%eax), %edx
	movl	12(%esp), %eax
	movl	%edx, 4(%eax)
	movl	system_dir, %edx
	movl	12(%esp), %eax
	movl	%edx, 56(%eax)
	movl	32(%esp), %eax
	call	*%eax
/APP
/  396 "Source/multitasking/task.c" 1
	int $51
/  0 "" 2
/NO_APP
	nop
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE298:
	.size	kill_with_func, .-kill_with_func
	.align 16
	.globl	Priority_promoter
	.type	Priority_promoter, @function
Priority_promoter:
.LFB299:
	.cfi_startproc
	subl	$76, %esp
	.cfi_def_cfa_offset 80
	movl	80(%esp), %eax
	movl	24(%eax), %eax
	testl	%eax, %eax
	je	.L2037
	movl	80(%esp), %eax
	movl	44(%eax), %eax
	movl	%eax, 60(%esp)
	call	Get_Scheduler
	cmpl	60(%esp), %eax
	je	.L2039
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$44
	.cfi_def_cfa_offset 96
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, 56(%esp)
	movl	56(%esp), %eax
	movl	$1, (%eax)
	movl	56(%esp), %eax
	movl	$0, 16(%eax)
	call	Get_Scheduler
	movl	%eax, %edx
	movl	56(%esp), %eax
	movl	%edx, 4(%eax)
	movl	56(%esp), %eax
	movl	60(%esp), %edx
	movl	%edx, 8(%eax)
	movl	56(%esp), %eax
	movl	80(%esp), %edx
	movl	%edx, 12(%eax)
	movl	56(%esp), %eax
	movl	$0, 36(%eax)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	68(%esp)
	.cfi_def_cfa_offset 96
	call	IPCdispatch
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	nop
	jmp	.L2037
.L2039:
	movl	80(%esp), %eax
	movl	24(%eax), %eax
	movl	%eax, 52(%esp)
	movl	60(%esp), %eax
	movl	60(%eax), %eax
	movl	%eax, %edx
	movl	52(%esp), %eax
	movl	%edx, (%eax)
	movl	60(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, 48(%esp)
	movl	80(%esp), %eax
	movl	16(%eax), %edx
	movl	$0, %eax
	subl	%edx, %eax
	sall	$12, %eax
	addl	$81920, %eax
	addl	%eax, 48(%esp)
	movl	48(%esp), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	48(%esp), %eax
	movl	%edx, (%eax)
	movl	48(%esp), %eax
	movl	(%eax), %eax
	leal	0(,%eax,4), %edx
	movl	48(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 44(%esp)
	movl	80(%esp), %edx
	movl	44(%esp), %eax
	movl	%edx, (%eax)
	movl	44(%esp), %edx
	movl	80(%esp), %eax
	movl	%edx, 24(%eax)
	movl	60(%esp), %eax
	movl	16(%eax), %eax
	cmpl	48(%esp), %eax
	jbe	.L2040
	movl	60(%esp), %eax
	movl	48(%esp), %edx
	movl	%edx, 16(%eax)
.L2040:
	movl	60(%esp), %eax
	movl	$0, 20(%eax)
	movl	80(%esp), %eax
	movl	16(%eax), %edx
	movl	80(%esp), %eax
	movl	%edx, 20(%eax)
.L2037:
	addl	$76, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE299:
	.size	Priority_promoter, .-Priority_promoter
	.align 16
	.globl	Task_sleep
	.type	Task_sleep, @function
Task_sleep:
.LFB300:
	.cfi_startproc
	subl	$76, %esp
	.cfi_def_cfa_offset 80
	movl	80(%esp), %eax
	movl	24(%eax), %eax
	testl	%eax, %eax
	je	.L2041
	movl	80(%esp), %eax
	movl	44(%eax), %eax
	movl	%eax, 60(%esp)
	call	Get_Scheduler
	cmpl	60(%esp), %eax
	je	.L2043
	leal	8(%esp), %eax
	movl	%eax, 56(%esp)
	movl	56(%esp), %eax
	movl	$3, (%eax)
	movl	56(%esp), %eax
	movl	$0, 16(%eax)
	call	Get_Scheduler
	movl	%eax, %edx
	movl	56(%esp), %eax
	movl	%edx, 4(%eax)
	movl	56(%esp), %eax
	movl	60(%esp), %edx
	movl	%edx, 8(%eax)
	movl	56(%esp), %eax
	movl	80(%esp), %edx
	movl	%edx, 12(%eax)
	movl	56(%esp), %eax
	movl	$0, 36(%eax)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	68(%esp)
	.cfi_def_cfa_offset 96
	call	IPCdispatch
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	nop
	jmp	.L2041
.L2043:
	movl	80(%esp), %eax
	movl	24(%eax), %eax
	movl	%eax, 52(%esp)
	movl	60(%esp), %eax
	movl	60(%eax), %eax
	movl	%eax, %edx
	movl	52(%esp), %eax
	movl	%edx, (%eax)
	movl	80(%esp), %eax
	movl	$0, 24(%eax)
.L2041:
	addl	$76, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE300:
	.size	Task_sleep, .-Task_sleep
	.align 16
	.globl	Task_wakeup
	.type	Task_wakeup, @function
Task_wakeup:
.LFB301:
	.cfi_startproc
	subl	$76, %esp
	.cfi_def_cfa_offset 80
	movl	80(%esp), %eax
	movl	24(%eax), %eax
	testl	%eax, %eax
	jne	.L2044
	movl	80(%esp), %eax
	movl	44(%eax), %eax
	movl	%eax, 60(%esp)
	call	Get_Scheduler
	cmpl	60(%esp), %eax
	je	.L2046
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$44
	.cfi_def_cfa_offset 96
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, 56(%esp)
	movl	56(%esp), %eax
	movl	$4, (%eax)
	movl	56(%esp), %eax
	movl	$0, 16(%eax)
	call	Get_Scheduler
	movl	%eax, %edx
	movl	56(%esp), %eax
	movl	%edx, 4(%eax)
	movl	56(%esp), %eax
	movl	60(%esp), %edx
	movl	%edx, 8(%eax)
	movl	56(%esp), %eax
	movl	80(%esp), %edx
	movl	%edx, 12(%eax)
	movl	56(%esp), %eax
	movl	$0, 36(%eax)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	68(%esp)
	.cfi_def_cfa_offset 96
	call	IPCdispatch
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	nop
	jmp	.L2044
.L2046:
	movl	60(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, 52(%esp)
	movl	80(%esp), %eax
	movl	16(%eax), %edx
	movl	$0, %eax
	subl	%edx, %eax
	sall	$12, %eax
	addl	$81920, %eax
	addl	%eax, 52(%esp)
	movl	52(%esp), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	52(%esp), %eax
	movl	%edx, (%eax)
	movl	52(%esp), %eax
	movl	(%eax), %eax
	leal	0(,%eax,4), %edx
	movl	52(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 48(%esp)
	movl	80(%esp), %edx
	movl	48(%esp), %eax
	movl	%edx, (%eax)
	movl	48(%esp), %edx
	movl	80(%esp), %eax
	movl	%edx, 24(%eax)
	movl	60(%esp), %eax
	movl	16(%eax), %eax
	cmpl	52(%esp), %eax
	jbe	.L2047
	movl	60(%esp), %eax
	movl	52(%esp), %edx
	movl	%edx, 16(%eax)
.L2047:
	movl	60(%esp), %eax
	movl	$0, 20(%eax)
	movl	80(%esp), %eax
	movl	16(%eax), %edx
	movl	80(%esp), %eax
	movl	%edx, 20(%eax)
.L2044:
	addl	$76, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE301:
	.size	Task_wakeup, .-Task_wakeup
	.align 16
	.globl	Task_wakeup_direct
	.type	Task_wakeup_direct, @function
Task_wakeup_direct:
.LFB302:
	.cfi_startproc
	subl	$16, %esp
	.cfi_def_cfa_offset 20
	movl	20(%esp), %eax
	movl	24(%eax), %eax
	testl	%eax, %eax
	jne	.L2051
	movl	20(%esp), %eax
	movl	44(%eax), %eax
	movl	%eax, 12(%esp)
	movl	12(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, 8(%esp)
	movl	20(%esp), %eax
	movl	16(%eax), %edx
	movl	$0, %eax
	subl	%edx, %eax
	sall	$12, %eax
	addl	$81920, %eax
	addl	%eax, 8(%esp)
	movl	8(%esp), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	8(%esp), %eax
	movl	%edx, (%eax)
	movl	8(%esp), %eax
	movl	(%eax), %eax
	leal	0(,%eax,4), %edx
	movl	8(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 4(%esp)
	movl	20(%esp), %edx
	movl	4(%esp), %eax
	movl	%edx, (%eax)
	movl	4(%esp), %edx
	movl	20(%esp), %eax
	movl	%edx, 24(%eax)
	movl	12(%esp), %eax
	movl	16(%eax), %eax
	cmpl	8(%esp), %eax
	jbe	.L2050
	movl	12(%esp), %eax
	movl	8(%esp), %edx
	movl	%edx, 16(%eax)
.L2050:
	movl	12(%esp), %eax
	movl	$0, 20(%eax)
	movl	20(%esp), %eax
	movl	16(%eax), %edx
	movl	20(%esp), %eax
	movl	%edx, 20(%eax)
.L2051:
	nop
	addl	$16, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE302:
	.size	Task_wakeup_direct, .-Task_wakeup_direct
	.align 16
	.globl	Priority_changer
	.type	Priority_changer, @function
Priority_changer:
.LFB303:
	.cfi_startproc
	subl	$76, %esp
	.cfi_def_cfa_offset 80
	movl	80(%esp), %eax
	movl	24(%eax), %eax
	testl	%eax, %eax
	je	.L2052
	movl	80(%esp), %eax
	movl	44(%eax), %eax
	movl	%eax, 60(%esp)
	call	Get_Scheduler
	cmpl	60(%esp), %eax
	je	.L2054
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$44
	.cfi_def_cfa_offset 96
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, 56(%esp)
	movl	56(%esp), %eax
	movl	$5, (%eax)
	movl	56(%esp), %eax
	movl	$1, 16(%eax)
	call	Get_Scheduler
	movl	%eax, %edx
	movl	56(%esp), %eax
	movl	%edx, 4(%eax)
	movl	56(%esp), %eax
	movl	60(%esp), %edx
	movl	%edx, 8(%eax)
	movl	56(%esp), %eax
	movl	80(%esp), %edx
	movl	%edx, 12(%eax)
	movl	56(%esp), %eax
	movl	84(%esp), %edx
	movl	%edx, 20(%eax)
	movl	56(%esp), %eax
	movl	$0, 36(%eax)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	68(%esp)
	.cfi_def_cfa_offset 96
	call	IPCdispatch
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	nop
	jmp	.L2052
.L2054:
	movl	80(%esp), %eax
	movl	24(%eax), %eax
	movl	%eax, 52(%esp)
	movl	60(%esp), %eax
	movl	60(%eax), %eax
	movl	%eax, %edx
	movl	52(%esp), %eax
	movl	%edx, (%eax)
	movl	60(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, 48(%esp)
	movl	80(%esp), %eax
	movl	84(%esp), %edx
	movl	%edx, 16(%eax)
	movl	80(%esp), %eax
	movl	16(%eax), %edx
	movl	$0, %eax
	subl	%edx, %eax
	sall	$12, %eax
	addl	$81920, %eax
	addl	%eax, 48(%esp)
	movl	48(%esp), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	48(%esp), %eax
	movl	%edx, (%eax)
	movl	48(%esp), %eax
	movl	(%eax), %eax
	leal	0(,%eax,4), %edx
	movl	48(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 44(%esp)
	movl	80(%esp), %edx
	movl	44(%esp), %eax
	movl	%edx, (%eax)
	movl	44(%esp), %edx
	movl	80(%esp), %eax
	movl	%edx, 24(%eax)
	movl	60(%esp), %eax
	movl	16(%eax), %eax
	cmpl	48(%esp), %eax
	jbe	.L2055
	movl	60(%esp), %eax
	movl	48(%esp), %edx
	movl	%edx, 16(%eax)
.L2055:
	movl	60(%esp), %eax
	movl	$0, 20(%eax)
	movl	80(%esp), %eax
	movl	16(%eax), %edx
	movl	80(%esp), %eax
	movl	%edx, 20(%eax)
.L2052:
	addl	$76, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE303:
	.size	Priority_changer, .-Priority_changer
	.globl	kb_end
	.section	.bss
	.align 4
	.type	kb_end, @object
	.size	kb_end, 4
kb_end:
	.zero	4
	.globl	kb_q_elements
	.align 4
	.type	kb_q_elements, @object
	.size	kb_q_elements, 4
kb_q_elements:
	.zero	4
	.comm	kb_Start_q,4,4
	.comm	kb_Current_q,4,4
	.comm	kb_Last_q,4,4
	.comm	FS_Handling_Process,4,4
	.comm	NeuralBase,4,4
	.comm	NeuralSize,4,4
	.comm	Neurons,4,4
	.globl	annCounter
	.data
	.align 32
	.type	annCounter, @object
	.size	annCounter, 40
annCounter:
	.string	"NeuralNets/annCounter.dat"
	.zero	14
	.globl	annDirName
	.align 4
	.type	annDirName, @object
	.size	annDirName, 20
annDirName:
	.string	"NeuralNets"
	.zero	9
	.globl	annFileNames
	.align 4
	.type	annFileNames, @object
	.size	annFileNames, 20
annFileNames:
	.string	"NeuralNets/ann"
	.zero	5
	.globl	Neuron_New
	.section	.bss
	.align 4
	.type	Neuron_New, @object
	.size	Neuron_New, 4
Neuron_New:
	.zero	4
	.text
	.align 16
	.globl	idle
	.type	idle, @function
idle:
.LFB564:
	.cfi_startproc
	.align 16
.L2057:
/APP
/  277 "Source/multitasking/tasking.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L2057
	.cfi_endproc
.LFE564:
	.size	idle, .-idle
	.globl	tyt
	.section	.bss
	.align 4
	.type	tyt, @object
	.size	tyt, 4
tyt:
	.zero	4
	.comm	test_proc,4,4
	.comm	test_task,4,4
	.globl	tttt
	.align 4
	.type	tttt, @object
	.size	tttt, 4
tttt:
	.zero	4
	.globl	bbb
	.align 4
	.type	bbb, @object
	.size	bbb, 4
bbb:
	.zero	4
	.globl	bby
	.data
	.align 4
	.type	bby, @object
	.size	bby, 4
bby:
	.long	bbb
	.section	.rodata
	.align 4
.LC338:
	.string	"\nProgram to convert a Number to the ratio of 2 Natural Numbers-->\n"
.LC339:
	.string	"\nEnter the Number: "
.LC340:
	.string	"as%d"
.LC341:
	.string	"\nThe Value of Denominator: "
	.align 4
.LC342:
	.string	"The value of Numerator: %x %s %x"
	.align 4
.LC343:
	.string	"\n\nDo you Want to quit now? (y/n) "
	.text
	.align 16
	.globl	test_thread
	.type	test_thread, @function
test_thread:
.LFB309:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$68, %esp
	.cfi_def_cfa_offset 80
	call	Get_Scheduler
/APP
/  67 "Source/multitasking/tasking.c" 1
	cli
/  0 "" 2
/NO_APP
	movb	$110, 34(%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	movl	$0, bbb
	pushl	$.LC338
	.cfi_def_cfa_offset 96
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	cmpb	$121, 34(%esp)
	leal	44(%esp), %ebx
	leal	34(%esp), %esi
	je	.L2062
	.align 16
.L2064:
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$.LC339
	.cfi_def_cfa_offset 96
	call	printf
	popl	%ecx
	.cfi_def_cfa_offset 92
	popl	%eax
	.cfi_def_cfa_offset 88
	pushl	$20
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	kb_getline
	movl	%ebx, (%esp)
	call	StrToInt
	movl	$0, 20(%esp)
	movl	%eax, 16(%esp)
	fildq	16(%esp)
	movl	$.LC340, (%esp)
	fld	%st(0)
	fstpt	4(%esp)
	fstpt	16(%esp)
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	$1, %eax
	fldt	(%esp)
	fnstcw	30(%esp)
	movzwl	30(%esp), %edx
	movb	$12, %dh
	movw	%dx, 28(%esp)
	.align 16
.L2066:
	addl	$1, %eax
	movl	%eax, (%esp)
	fildl	(%esp)
	fmul	%st(1), %st
	fld	%st(0)
	fldcw	28(%esp)
	fistpl	(%esp)
	fldcw	30(%esp)
	fildl	(%esp)
	fucomip	%st(1), %st
	fstp	%st(0)
	jp	.L2066
	jne	.L2066
	fstp	%st(0)
	pushl	%eax
	.cfi_def_cfa_offset 84
	pushl	$.LC341
	.cfi_def_cfa_offset 88
	pushl	8(%esp)
	.cfi_def_cfa_offset 92
	pushl	$.LC342
	.cfi_def_cfa_offset 96
	call	printf
	movl	$.LC343, (%esp)
	call	printf
	popl	%eax
	.cfi_def_cfa_offset 92
	popl	%edx
	.cfi_def_cfa_offset 88
	pushl	$10
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	kb_getline
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	cmpb	$121, 34(%esp)
	jne	.L2064
.L2062:
	movl	$0, tttt
	call	kill
/APP
/  95 "Source/multitasking/tasking.c" 1
	int $50
/  0 "" 2
/NO_APP
.L2060:
	jmp	.L2060
	.cfi_endproc
.LFE309:
	.size	test_thread, .-test_thread
	.comm	testLocked,4,4
	.section	.rodata
.LC344:
	.string	"\n%d %d %d"
.LC345:
	.string	"[%d]\t"
	.text
	.align 16
	.globl	test_process
	.type	test_process, @function
test_process:
.LFB310:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	movl	$256, %esi
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	pushl	$100000
	.cfi_def_cfa_offset 20
	movl	$255, %ebx
	pushl	$10000
	.cfi_def_cfa_offset 24
	pushl	$Randomizer
	.cfi_def_cfa_offset 28
	pushl	$random
	.cfi_def_cfa_offset 32
	call	RandomnessCalculator
	fstp	%st(0)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$2, %ecx
.L2072:
	movl	%esi, %eax
	cltd
	idivl	%ecx
	testl	%edx, %edx
	jne	.L2070
	movl	%ebx, %eax
	cltd
	idivl	%ecx
	testl	%edx, %edx
	jne	.L2070
	movl	$1, %ebx
.L2071:
	movl	$2, %ecx
	movl	$27, %edi
	movl	$128, %esi
.L2073:
	addl	$1, %ecx
	cmpl	$27, %ecx
	je	.L2087
	movl	%edi, %eax
	cltd
	idivl	%ecx
	testl	%edx, %edx
	jne	.L2073
	movl	%esi, %eax
	cltd
	idivl	%ecx
	testl	%edx, %edx
	jne	.L2073
	movl	$1, %eax
.L2075:
	pushl	%ebx
	.cfi_def_cfa_offset 20
	pushl	%eax
	.cfi_def_cfa_offset 24
	xorl	%ebx, %ebx
	pushl	$1
	.cfi_def_cfa_offset 28
	pushl	$.LC344
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	cmpl	$1, total_CPU_Cores
	movl	MotherSpace, %esi
	je	.L2080
	.align 16
.L2083:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	68(%esi)
	.cfi_def_cfa_offset 28
	addl	$1, %ebx
	pushl	$.LC345
	.cfi_def_cfa_offset 32
	addl	$76, %esi
	call	printf
	movl	total_CPU_Cores, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	subl	$1, %eax
	cmpl	%ebx, %eax
	ja	.L2083
.L2080:
	call	kill
/APP
/  183 "Source/multitasking/tasking.c" 1
	int $50
/  0 "" 2
/NO_APP
.L2078:
	jmp	.L2078
	.align 16
.L2070:
	addl	$1, %ecx
	cmpl	$255, %ecx
	jne	.L2072
	xorl	%ebx, %ebx
	jmp	.L2071
.L2087:
	xorl	%eax, %eax
	jmp	.L2075
	.cfi_endproc
.LFE310:
	.size	test_process, .-test_process
	.align 16
	.globl	tvkc1
	.type	tvkc1, @function
tvkc1:
.LFB311:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	kill
/APP
/  192 "Source/multitasking/tasking.c" 1
	int $51
/  0 "" 2
/NO_APP
.L2089:
	jmp	.L2089
	.cfi_endproc
.LFE311:
	.size	tvkc1, .-tvkc1
	.section	.rodata
.LC346:
	.string	"Main_Kernel"
	.align 4
.LC347:
	.string	"\n\n\n\t\t--------------MISSION ACCOMPLISHED--------------\n\n\t--------------Welcome to the MultiThreading World!!!--------------\n"
	.align 4
.LC348:
	.string	"\n\t-----------Launching the Shell and input/output processes-----------\n\t\t\t\tStarting in 3...2...1... GO...\n\n"
	.text
	.align 16
	.globl	tasking_initiator
	.type	tasking_initiator, @function
tasking_initiator:
.LFB312:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
	lock orl	$0, (%esp)
	movl	$0, testLocked
	call	Get_Scheduler
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	kernel_proc
	.cfi_def_cfa_offset 32
	movl	%eax, %ebx
	pushl	$514
	.cfi_def_cfa_offset 36
	pushl	$0
	.cfi_def_cfa_offset 40
	pushl	$kernel_main
	.cfi_def_cfa_offset 44
	pushl	$.LC346
	.cfi_def_cfa_offset 48
	call	create_task
	addl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	$2, 28(%eax)
	movl	%eax, 44(%ebx)
	pushl	$.LC347
	.cfi_def_cfa_offset 32
	movl	%eax, Kernel_task
	call	printf
	movl	$.LC348, (%esp)
	call	printf
	movl	$1, (%esp)
	call	delay1
	movl	$0, kb_Start_q
	movl	$0, kb_Last_q
	call	init_shell
	movl	$1, multitasking_ON
/APP
/  45 "Library/sys.h" 1
	inb $33, %al
/  0 "" 2
/NO_APP
	orl	$1, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/NO_APP
	popl	%eax
	.cfi_def_cfa_offset 28
	pushl	APIC_LOCAL_BASE
	.cfi_def_cfa_offset 32
	call	apic_start_timer
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	cmpl	$1, total_CPU_Cores
	je	.L2095
	.align 16
.L2096:
	movl	%eax, %edx
	addl	$1, %eax
	sall	$13, %edx
	addl	AP_startup_Code_sz, %edx
	movl	$12864, 12296(%edx)
	movl	total_CPU_Cores, %ecx
	leal	-1(%ecx), %edx
	cmpl	%eax, %edx
	ja	.L2096
.L2095:
	lock orl	$0, (%esp)
	movl	$0, TASK_LOCK_KILLLocked
	lock orl	$0, (%esp)
	movl	$0, TASK_LOCK_ATDLocked
/APP
/  224 "Source/multitasking/tasking.c" 1
	sti;
/  0 "" 2
/NO_APP
	call	kill
.L2093:
	jmp	.L2093
	.cfi_endproc
.LFE312:
	.size	tasking_initiator, .-tasking_initiator
	.section	.rodata
.LC349:
	.string	"microkernel"
	.align 4
.LC350:
	.string	"\nSchedulers Created Successfully"
.LC351:
	.string	"Shell"
.LC352:
	.string	"Shell_Ostream"
.LC353:
	.string	"Shell_task"
.LC354:
	.string	"Shell_Istream"
.LC355:
	.string	"System_idle_task"
.LC356:
	.string	"SAS"
	.text
	.align 16
	.globl	init_multitasking
	.type	init_multitasking, @function
init_multitasking:
.LFB313:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  233 "Source/multitasking/tasking.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$4096
	.cfi_def_cfa_offset 32
	call	*kmalloc
	leal	4096(%eax), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%eax, %edx
	.align 16
.L2101:
	addl	$1, %edx
	movb	$0, -1(%edx)
	cmpl	%ecx, %edx
	jne	.L2101
	pushl	$1
	.cfi_def_cfa_offset 20
	pushl	$1
	.cfi_def_cfa_offset 24
	pushl	$0
	.cfi_def_cfa_offset 28
	pushl	$.LC349
	.cfi_def_cfa_offset 32
	movl	%eax, main_prdir
	call	create_process
	movl	system_dir, %edx
	movl	%eax, kernel_proc
	movl	%edx, (%eax)
	call	Init_Scheduler
	movl	$.LC350, (%esp)
	call	printf
	call	Setup_MMADS
	pushl	kernel_proc
	.cfi_def_cfa_offset 36
	pushl	$1
	.cfi_def_cfa_offset 40
	pushl	$0
	.cfi_def_cfa_offset 44
	pushl	$.LC351
	.cfi_def_cfa_offset 48
	call	create_process
	addl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	%eax, Shell_proc
	pushl	%eax
	.cfi_def_cfa_offset 32
	pushl	$514
	.cfi_def_cfa_offset 36
	pushl	$10
	.cfi_def_cfa_offset 40
	pushl	$Shell_Double_buffer
	.cfi_def_cfa_offset 44
	pushl	$.LC352
	.cfi_def_cfa_offset 48
	call	create_task
	addl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	%eax, Shell_Ostream_task
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	Activate_task_direct
	popl	%eax
	.cfi_def_cfa_offset 28
	pushl	Shell_proc
	.cfi_def_cfa_offset 32
	pushl	$514
	.cfi_def_cfa_offset 36
	pushl	$15
	.cfi_def_cfa_offset 40
	pushl	$Shell
	.cfi_def_cfa_offset 44
	pushl	$.LC353
	.cfi_def_cfa_offset 48
	call	create_task
	addl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	%eax, Shell_task
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	Activate_task_direct
	popl	%edx
	.cfi_def_cfa_offset 28
	pushl	Shell_proc
	.cfi_def_cfa_offset 32
	pushl	$514
	.cfi_def_cfa_offset 36
	pushl	$1
	.cfi_def_cfa_offset 40
	pushl	$Shell_Input
	.cfi_def_cfa_offset 44
	pushl	$.LC354
	.cfi_def_cfa_offset 48
	call	create_task
	addl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	%eax, Shell_Istream_task
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	Activate_task_direct
	popl	%ecx
	.cfi_def_cfa_offset 28
	pushl	kernel_proc
	.cfi_def_cfa_offset 32
	pushl	$514
	.cfi_def_cfa_offset 36
	pushl	$0
	.cfi_def_cfa_offset 40
	pushl	$idle
	.cfi_def_cfa_offset 44
	pushl	$.LC355
	.cfi_def_cfa_offset 48
	call	create_task
	addl	$32, %esp
	.cfi_def_cfa_offset 16
	movl	$1, 28(%eax)
	pushl	kernel_proc
	.cfi_def_cfa_offset 20
	pushl	$1
	.cfi_def_cfa_offset 24
	pushl	$0
	.cfi_def_cfa_offset 28
	pushl	$.LC356
	.cfi_def_cfa_offset 32
	movl	%eax, Idle_task
	call	create_process
	movl	%eax, SAS_proc
	call	SAS_init
	call	IPC_init
	call	tasking_initiator
	.cfi_endproc
.LFE313:
	.size	init_multitasking, .-init_multitasking
	.align 16
	.globl	Spurious_task_func
	.type	Spurious_task_func, @function
Spurious_task_func:
.LFB314:
	.cfi_startproc
	.align 16
.L2105:
/APP
/  277 "Source/multitasking/tasking.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L2105
	.cfi_endproc
.LFE314:
	.size	Spurious_task_func, .-Spurious_task_func
	.globl	pidcounter
	.data
	.align 4
	.type	pidcounter, @object
	.size	pidcounter, 4
pidcounter:
	.long	1
	.text
	.align 16
	.globl	create_process
	.type	create_process, @function
create_process:
.LFB315:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	movl	72(%esp), %edi
	movl	60(%esp), %ebx
	pushl	$1
	.cfi_def_cfa_offset 64
	call	mtalloc
	popl	%edx
	.cfi_def_cfa_offset 60
	popl	%ecx
	.cfi_def_cfa_offset 56
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	main_prdir
	.cfi_def_cfa_offset 64
	movl	%eax, %esi
	call	PrTbl_entry
	movl	72(%esp), %eax
	movl	%eax, 40(%esi)
	movl	68(%esp), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	$1, %edi
	movl	%eax, 4(%esi)
	jbe	.L2107
	movl	(%edi), %eax
	movl	%edi, 68(%esi)
	movl	%eax, (%esi)
.L2108:
	movzbl	(%ebx), %ecx
	leal	12(%esi), %edx
	movl	%ebx, %eax
	.align 16
.L2113:
	addl	$1, %edx
	addl	$1, %eax
	movb	%cl, -1(%edx)
	movzbl	(%eax), %ecx
	testb	%cl, %cl
	jne	.L2113
	movb	$0, (%edx)
	movl	pidcounter, %eax
	movl	$0, 56(%esi)
	addl	$1, processes
	movl	%eax, 36(%esi)
	addl	$1, %eax
	movl	%eax, pidcounter
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%esi, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2107:
	.cfi_restore_state
	testl	%edi, %edi
	je	.L2118
	movl	$0, 68(%esi)
	jmp	.L2108
.L2118:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$4
	.cfi_def_cfa_offset 64
	call	mtalloc
	movl	%eax, %ebp
	movl	%eax, (%esp)
	call	Setup_VMEM
	addl	$12, %esp
	.cfi_def_cfa_offset 52
	movl	%ebp, (%esi)
	pushl	%ebp
	.cfi_def_cfa_offset 56
	pushl	$8388608
	.cfi_def_cfa_offset 60
	pushl	$0
	.cfi_def_cfa_offset 64
	call	map
	addl	$12, %esp
	.cfi_def_cfa_offset 52
	pushl	%ebp
	.cfi_def_cfa_offset 56
	pushl	$268431360
	.cfi_def_cfa_offset 60
	pushl	$-268435456
	.cfi_def_cfa_offset 64
	call	map
	addl	$12, %esp
	.cfi_def_cfa_offset 52
	pushl	%ebp
	.cfi_def_cfa_offset 56
	pushl	$16384
	.cfi_def_cfa_offset 60
	pushl	%ebp
	.cfi_def_cfa_offset 64
	call	map
	movl	MotherSpace, %edi
	addl	$12, %esp
	.cfi_def_cfa_offset 52
	pushl	%ebp
	.cfi_def_cfa_offset 56
	pushl	$4096
	.cfi_def_cfa_offset 60
	pushl	%edi
	.cfi_def_cfa_offset 64
	addl	$8, %edi
	call	map
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	xorl	%edx, %edx
	cmpl	$1, total_CPU_Cores
	je	.L2112
	.align 16
.L2114:
	movl	%edx, 12(%esp)
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	addl	$76, %edi
	pushl	%ebp
	.cfi_def_cfa_offset 56
	pushl	$4096
	.cfi_def_cfa_offset 60
	pushl	-48(%edi)
	.cfi_def_cfa_offset 64
	call	map
	addl	$12, %esp
	.cfi_def_cfa_offset 52
	pushl	%ebp
	.cfi_def_cfa_offset 56
	pushl	$16384
	.cfi_def_cfa_offset 60
	pushl	-20(%edi)
	.cfi_def_cfa_offset 64
	call	map
	addl	$12, %esp
	.cfi_def_cfa_offset 52
	pushl	%ebp
	.cfi_def_cfa_offset 56
	pushl	$163840
	.cfi_def_cfa_offset 60
	pushl	-76(%edi)
	.cfi_def_cfa_offset 64
	call	map
	addl	$12, %esp
	.cfi_def_cfa_offset 52
	pushl	%ebp
	.cfi_def_cfa_offset 56
	pushl	$4096
	.cfi_def_cfa_offset 60
	pushl	-24(%edi)
	.cfi_def_cfa_offset 64
	call	map
	movl	28(%esp), %edx
	movl	total_CPU_Cores, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	addl	$1, %edx
	subl	$1, %eax
	cmpl	%eax, %edx
	jb	.L2114
.L2112:
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	pushl	%ebp
	.cfi_def_cfa_offset 56
	pushl	$1073741824
	.cfi_def_cfa_offset 60
	pushl	$-2147483648
	.cfi_def_cfa_offset 64
	call	map
	addl	$12, %esp
	.cfi_def_cfa_offset 52
	pushl	(%esi)
	.cfi_def_cfa_offset 56
	pushl	$4096
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	map
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L2108
	.cfi_endproc
.LFE315:
	.size	create_process, .-create_process
	.align 16
	.globl	init_Processing
	.type	init_Processing, @function
init_Processing:
.LFB316:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$4096
	.cfi_def_cfa_offset 32
	call	*kmalloc
	leal	4096(%eax), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%eax, %edx
	.align 16
.L2120:
	addl	$1, %edx
	movb	$0, -1(%edx)
	cmpl	%ecx, %edx
	jne	.L2120
	movl	%eax, main_prdir
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE316:
	.size	init_Processing, .-init_Processing
	.align 16
	.globl	PrDir_create
	.type	PrDir_create, @function
PrDir_create:
.LFB317:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$4096
	.cfi_def_cfa_offset 32
	call	*kmalloc
	leal	4096(%eax), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%eax, %edx
	.align 16
.L2124:
	addl	$1, %edx
	movb	$0, -1(%edx)
	cmpl	%ecx, %edx
	jne	.L2124
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE317:
	.size	PrDir_create, .-PrDir_create
	.align 16
	.globl	PrTbl_create
	.type	PrTbl_create, @function
PrTbl_create:
.LFB318:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	28(%esp), %ebx
	pushl	$4096
	.cfi_def_cfa_offset 32
	call	*kmalloc
	leal	4096(%eax), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%eax, %edx
	.align 16
.L2128:
	addl	$1, %edx
	movb	$0, -1(%edx)
	cmpl	%ecx, %edx
	jne	.L2128
	xorl	%edx, %edx
	jmp	.L2131
	.align 16
.L2129:
	addl	$1, %edx
	cmpl	$1024, %edx
	je	.L2135
.L2131:
	movl	(%ebx,%edx,4), %ecx
	testl	%ecx, %ecx
	jne	.L2129
	movl	%eax, (%ebx,%edx,4)
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L2135:
	.cfi_restore_state
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE318:
	.size	PrTbl_create, .-PrTbl_create
	.align 16
	.globl	PrTbl_entry_i
	.type	PrTbl_entry_i, @function
PrTbl_entry_i:
.LFB319:
	.cfi_startproc
	movl	4(%esp), %edx
	xorl	%eax, %eax
	jmp	.L2139
	.align 16
.L2137:
	addl	$1, %eax
	cmpl	$1024, %eax
	je	.L2141
.L2139:
	movl	(%edx,%eax,4), %ecx
	testl	%ecx, %ecx
	jne	.L2137
	movl	8(%esp), %ecx
	movl	%ecx, (%edx,%eax,4)
	movl	$1, %eax
	ret
	.align 16
.L2141:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE319:
	.size	PrTbl_entry_i, .-PrTbl_entry_i
	.align 16
	.globl	PrTbl_entry
	.type	PrTbl_entry, @function
PrTbl_entry:
.LFB320:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$4, %esp
	.cfi_def_cfa_offset 16
	movl	16(%esp), %ebx
	leal	4096(%ebx), %esi
.L2143:
.L2148:
	movl	%ebx, %ecx
.L2147:
	movl	(%ecx), %edx
	testl	%edx, %edx
	je	.L2144
	xorl	%eax, %eax
	jmp	.L2146
	.align 16
.L2157:
	addl	$1, %eax
	cmpl	$1024, %eax
	je	.L2144
.L2146:
	cmpl	$0, (%edx,%eax,4)
	jne	.L2157
	movl	20(%esp), %ecx
	movl	%ecx, (%edx,%eax,4)
	addl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2144:
	.cfi_restore_state
	addl	$4, %ecx
	cmpl	%esi, %ecx
	jne	.L2147
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	PrTbl_create
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L2148
	.cfi_endproc
.LFE320:
	.size	PrTbl_entry, .-PrTbl_entry
	.section	.rodata
.LC357:
	.string	"MMADS"
.LC358:
	.string	"MMADS_csrbTweaker"
	.text
	.align 16
	.globl	Setup_MMADS
	.type	Setup_MMADS, @function
Setup_MMADS:
.LFB321:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	pushl	kernel_proc
	.cfi_def_cfa_offset 20
	pushl	$1
	.cfi_def_cfa_offset 24
	pushl	$0
	.cfi_def_cfa_offset 28
	pushl	$.LC357
	.cfi_def_cfa_offset 32
	call	create_process
	movl	%eax, (%esp)
	pushl	$514
	.cfi_def_cfa_offset 36
	pushl	$0
	.cfi_def_cfa_offset 40
	pushl	$MMADS_csrbTweaker
	.cfi_def_cfa_offset 44
	pushl	$.LC358
	.cfi_def_cfa_offset 48
	movl	%eax, MMADS_Process
	call	create_task
	addl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	%eax, MMADS_csrbTweakerThread
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	Activate_task_direct
	movl	$12582912, mmads_stack_start
	movl	$12582912, mmads_stack_end
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE321:
	.size	Setup_MMADS, .-Setup_MMADS
	.align 16
	.globl	MMADS_csrbTweaker
	.type	MMADS_csrbTweaker, @function
MMADS_csrbTweaker:
.LFB322:
	.cfi_startproc
	subl	$52, %esp
	.cfi_def_cfa_offset 56
.L2172:
	movl	mmads_stack_size, %eax
	testl	%eax, %eax
	je	.L2161
	movl	mmads_stack_end, %eax
	subl	$4, %eax
	movl	%eax, mmads_stack_end
	movl	mmads_stack_size, %eax
	subl	$1, %eax
	movl	%eax, mmads_stack_size
	movl	mmads_stack_end, %eax
	movl	%eax, 40(%esp)
	movl	40(%esp), %eax
	movl	(%eax), %eax
	movl	%eax, 36(%esp)
	movl	36(%esp), %eax
	addl	$8192, %eax
	movl	%eax, 32(%esp)
	movl	32(%esp), %eax
	movl	12(%eax), %eax
	movl	%eax, 44(%esp)
	jmp	.L2162
.L2166:
	movl	44(%esp), %eax
	movl	(%eax), %eax
	movl	%eax, 44(%esp)
	movl	44(%esp), %eax
	movl	12(%eax), %eax
	testl	%eax, %eax
	je	.L2163
	movl	32(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, 24(%esp)
	movl	24(%esp), %ecx
	movl	$266354561, %edx
	movl	%ecx, %eax
	mull	%edx
	movl	%edx, %eax
	shrl	$8, %eax
	imull	$4128, %eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %eax
	testl	%eax, %eax
	jne	.L2164
	movl	24(%esp), %eax
	movl	%eax, 20(%esp)
	subl	$16, 24(%esp)
	movl	24(%esp), %eax
	movl	(%eax), %eax
	movl	%eax, 24(%esp)
	subl	$16, 24(%esp)
	movl	32(%esp), %eax
	movl	24(%esp), %edx
	movl	%edx, 8(%eax)
	movl	24(%esp), %eax
	movl	4(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, 4(%eax)
	movl	24(%esp), %eax
	movl	8(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, 8(%eax)
	movl	44(%esp), %eax
	movl	$0, 12(%eax)
	movl	32(%esp), %eax
	movl	12(%eax), %edx
	movl	24(%esp), %eax
	movl	%edx, (%eax)
	movl	20(%esp), %eax
	movl	%eax, (%esp)
	movl	$0, 4(%esp)
	movl	(%esp), %eax
	andl	$-4096, %eax
	pushl	%eax
	.cfi_def_cfa_offset 60
	call	push_frameStack
	addl	$4, %esp
	.cfi_def_cfa_offset 56
	jmp	.L2165
.L2164:
	subl	$16, 24(%esp)
	movl	32(%esp), %eax
	movl	24(%esp), %edx
	movl	%edx, 8(%eax)
	movl	24(%esp), %eax
	movl	4(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, 4(%eax)
	movl	24(%esp), %eax
	movl	8(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, 8(%eax)
	movl	44(%esp), %eax
	movl	$0, 12(%eax)
	movl	32(%esp), %eax
	movl	12(%eax), %edx
	movl	24(%esp), %eax
	movl	%edx, (%eax)
.L2165:
	movl	32(%esp), %eax
	movl	4(%eax), %eax
	leal	-1(%eax), %edx
	movl	32(%esp), %eax
	movl	%edx, 4(%eax)
.L2163:
	addl	$16, 44(%esp)
.L2162:
	movl	32(%esp), %eax
	movl	4(%eax), %eax
	testl	%eax, %eax
	jne	.L2166
	movl	36(%esp), %eax
	addl	$4096, %eax
	movl	%eax, 28(%esp)
	movl	28(%esp), %eax
	movl	12(%eax), %eax
	movl	%eax, 44(%esp)
	jmp	.L2167
.L2171:
	movl	44(%esp), %eax
	movl	(%eax), %eax
	movl	%eax, 44(%esp)
	movl	44(%esp), %eax
	movl	12(%eax), %eax
	testl	%eax, %eax
	je	.L2168
	movl	28(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, 24(%esp)
	movl	24(%esp), %ecx
	movl	$-74193293, %edx
	movl	%ecx, %eax
	mull	%edx
	movl	%edx, %eax
	shrl	$12, %eax
	imull	$4168, %eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %eax
	testl	%eax, %eax
	jne	.L2169
	movl	24(%esp), %eax
	movl	%eax, 20(%esp)
	subl	$16, 24(%esp)
	movl	24(%esp), %eax
	movl	(%eax), %eax
	movl	%eax, 24(%esp)
	subl	$16, 24(%esp)
	movl	28(%esp), %eax
	movl	24(%esp), %edx
	movl	%edx, 8(%eax)
	movl	24(%esp), %eax
	movl	4(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, 4(%eax)
	movl	24(%esp), %eax
	movl	8(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, 8(%eax)
	movl	44(%esp), %eax
	movl	$0, 12(%eax)
	movl	28(%esp), %eax
	movl	12(%eax), %edx
	movl	24(%esp), %eax
	movl	%edx, (%eax)
	movl	20(%esp), %eax
	movl	%eax, 8(%esp)
	movl	$0, 12(%esp)
	movl	8(%esp), %eax
	andl	$-4096, %eax
	pushl	%eax
	.cfi_def_cfa_offset 60
	call	push_frameStack
	addl	$4, %esp
	.cfi_def_cfa_offset 56
	jmp	.L2170
.L2169:
	subl	$16, 24(%esp)
	movl	28(%esp), %eax
	movl	24(%esp), %edx
	movl	%edx, 8(%eax)
	movl	24(%esp), %eax
	movl	4(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, 4(%eax)
	movl	24(%esp), %eax
	movl	8(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, 8(%eax)
	movl	44(%esp), %eax
	movl	$0, 12(%eax)
	movl	28(%esp), %eax
	movl	12(%eax), %edx
	movl	24(%esp), %eax
	movl	%edx, (%eax)
.L2170:
	movl	28(%esp), %eax
	movl	4(%eax), %eax
	leal	-1(%eax), %edx
	movl	28(%esp), %eax
	movl	%edx, 4(%eax)
.L2168:
	addl	$16, 44(%esp)
.L2167:
	movl	28(%esp), %eax
	movl	4(%eax), %eax
	testl	%eax, %eax
	jne	.L2171
.L2161:
/APP
/  108 "Source/MemManagement/MManager/mmanagerSys.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L2172
	.cfi_endproc
.LFE322:
	.size	MMADS_csrbTweaker, .-MMADS_csrbTweaker
	.section	.rodata
	.align 4
.LC359:
	.string	"\n %gInitializing HPET (High Precision Event Timer)!!!"
.LC360:
	.string	"HPET"
.LC361:
	.string	"\nHPET Initialized%g\n"
	.align 4
.LC362:
	.string	"\nGeneral Capabilities reg: %d GISReg: %d\nTimer0 Capabilities: %d %d\n"
	.text
	.align 16
	.globl	init_hpet
	.type	init_hpet, @function
init_hpet:
.LFB325:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$5
	.cfi_def_cfa_offset 28
	pushl	$.LC359
	.cfi_def_cfa_offset 32
	call	printf
	call	acpiGetRSDPtr
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	je	.L2173
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	movl	%eax, %ebx
	pushl	$.LC220
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	acpiCheckHeader
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	jne	.L2173
	movl	4(%ebx), %eax
	addl	$36, %ebx
	subl	$36, %eax
	shrl	$2, %eax
	testl	%eax, %eax
	je	.L2173
	leal	(%ebx,%eax,4), %edi
	jmp	.L2176
	.align 16
.L2175:
	addl	$4, %ebx
	cmpl	%ebx, %edi
	je	.L2173
.L2176:
	movl	(%ebx), %esi
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$.LC360
	.cfi_def_cfa_offset 28
	pushl	%esi
	.cfi_def_cfa_offset 32
	call	acpiCheckHeader
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	jne	.L2175
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	movl	%esi, hpet_sdt
	pushl	$15
	.cfi_def_cfa_offset 28
	pushl	$.LC361
	.cfi_def_cfa_offset 32
	call	printf
	movl	hpet_sdt, %eax
	movl	44(%eax), %eax
	movl	$0, 240(%eax)
	movl	$0, 244(%eax)
	movl	$1, 16(%eax)
	movl	%eax, hpet_base
	popl	%edx
	.cfi_def_cfa_offset 28
	pushl	256(%eax)
	.cfi_def_cfa_offset 32
	pushl	260(%eax)
	.cfi_def_cfa_offset 36
	pushl	32(%eax)
	.cfi_def_cfa_offset 40
	pushl	(%eax)
	.cfi_def_cfa_offset 44
	pushl	$.LC362
	.cfi_def_cfa_offset 48
	movl	%eax, hpet_virt_address
	movl	%eax, hpet
	call	printf
	movl	hpet, %eax
	addl	$32, %esp
	.cfi_def_cfa_offset 16
	addl	$240, %eax
	movl	%eax, HPET_main_counter
.L2173:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE325:
	.size	init_hpet, .-init_hpet
	.comm	bytes,4,4
	.comm	off,4,4
	.comm	start_off,4,4
	.comm	root_location,8,8
	.section	.rodata
.LC363:
	.string	"root"
	.text
	.align 16
	.globl	Setup_fs
	.type	Setup_fs, @function
Setup_fs:
.LFB326:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subl	$28, %esp
	.cfi_def_cfa_offset 44
	pushl	$512
	.cfi_def_cfa_offset 48
	call	pmem
	movl	root_location+4, %edx
	movl	%eax, %esi
	movl	root_location, %eax
	movl	curr_port, %ebx
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%esi
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L2186
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L2186:
	movl	root_location, %edi
	movl	$114, %ecx
	movl	$.LC363, %edx
	andl	$511, %edi
	addl	%esi, %edi
	leal	82(%edi), %eax
	.align 16
.L2187:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L2187
	xorl	%edx, %edx
	movb	$0, (%eax)
	movl	root_location, %eax
	movw	%dx, (%edi)
	movl	root_location+4, %edx
	movl	$1, %ecx
	movl	$2, %ebx
	movw	%cx, curr_dir+4
	movl	$0, 10(%edi)
	movl	%eax, 8(%esp)
	movl	%eax, 26(%edi)
	movl	$114, %ecx
	movl	%edx, 12(%esp)
	movl	%edx, 30(%edi)
	movl	$curr_dir+6, %eax
	movl	$0, 14(%edi)
	movl	$0, 66(%edi)
	movl	$.LC363, %edx
	movl	$0, 70(%edi)
	movl	$0, 58(%edi)
	movl	$0, 62(%edi)
	movl	$0, 34(%edi)
	movl	$0, 38(%edi)
	movl	$0, 42(%edi)
	movl	$0, 46(%edi)
	movl	$0, 50(%edi)
	movl	$0, 54(%edi)
	movl	$11465840, 6(%edi)
	movl	%edi, curr_dir
	movl	%edi, root_dir
	movw	%bx, root_dir+4
	.align 16
.L2188:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L2188
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	movb	$0, (%eax)
	movl	curr_port, %edi
	pushl	$1
	.cfi_def_cfa_offset 48
	movl	28(%esp), %ebx
	movl	24(%esp), %ecx
	movl	%ebx, %eax
	shrdl	$9, %ebx, %ecx
	shrl	$9, %eax
	pushl	%eax
	.cfi_def_cfa_offset 52
	pushl	%ecx
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%esi
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%edi
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L2185
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L2185:
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE326:
	.size	Setup_fs, .-Setup_fs
	.section	.rodata
	.align 4
.LC364:
	.string	"\nDestination Folder %s Not found"
	.align 4
.LC365:
	.string	"\nFolder \"%s\" Created Successfully\n"
	.text
	.align 16
	.globl	create_directory
	.type	create_directory, @function
create_directory:
.LFB327:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	64(%esp), %eax
	movl	68(%esp), %ebp
	movl	%eax, 16(%esp)
	movl	72(%esp), %eax
	testl	%eax, %eax
	je	.L2200
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	search_folderOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	je	.L2231
.L2201:
	movl	30(%eax), %edx
	movl	26(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	%eax, 12(%esp)
	movl	%edx, 16(%esp)
	pushl	$512
	.cfi_def_cfa_offset 80
	call	pmem
	movl	$1, (%esp)
	movl	%eax, 28(%esp)
	call	sec_alloc
	movl	%edx, %edi
	movl	%eax, %esi
	shrl	$9, %edx
	movl	%edx, 44(%esp)
	movl	curr_port, %ebx
	movl	%edx, %ecx
	movl	%esi, %eax
	movl	%edi, %edx
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%eax, 36(%esp)
	pushl	%ecx
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	44(%esp)
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2202
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2202:
	movl	%esi, %ebx
	movl	16(%esp), %eax
	andl	$511, %ebx
	addl	12(%esp), %ebx
	movl	%eax, %edx
	movl	$0, 10(%ebx)
	movl	%esi, 26(%ebx)
	movl	%edi, 30(%ebx)
	movl	$0, 14(%ebx)
	movzbl	(%eax), %ecx
	leal	82(%ebx), %eax
	.align 16
.L2203:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L2203
	movl	4(%esp), %edx
	movb	$0, (%eax)
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	12(%esp), %eax
	movw	%bp, (%ebx)
	movl	$0, 66(%ebx)
	movl	$0, 70(%ebx)
	movl	%edx, 22(%ebx)
	movl	$0, 58(%ebx)
	movl	%eax, 18(%ebx)
	movl	$0, 62(%ebx)
	movl	$11465840, 6(%ebx)
	movl	$0, 34(%ebx)
	movl	$0, 38(%ebx)
	movl	$0, 50(%ebx)
	movl	$0, 54(%ebx)
	movl	$0, 42(%ebx)
	movl	$0, 46(%ebx)
	pushl	$512
	.cfi_def_cfa_offset 80
	call	pmem
	movl	20(%esp), %edx
	movl	%eax, %esi
	movl	16(%esp), %eax
	movl	curr_port, %ebp
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %edi
	shrl	$9, %edi
	movl	%eax, 40(%esp)
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%esi
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%ebp
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2204
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2204:
	movl	(%esp), %ecx
	andl	$511, %ecx
	addl	%esi, %ecx
	movl	62(%ecx), %edx
	movl	58(%ecx), %eax
	movl	%edx, 4(%esp)
	movl	4(%esp), %edx
	movl	%eax, (%esp)
	orl	%eax, %edx
	jne	.L2232
	movl	26(%ebx), %eax
	movl	30(%ebx), %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	%eax, 34(%ecx)
	movl	%edx, 38(%ecx)
	movl	$0, 74(%ebx)
	movl	$0, 78(%ebx)
	movl	curr_port, %ebx
	movl	%eax, 58(%ecx)
	movl	%edx, 62(%ecx)
	addl	$1, 14(%ecx)
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	44(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%esi
	.cfi_def_cfa_offset 100
.L2229:
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2209
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2209:
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	44(%esp)
	.cfi_def_cfa_offset 84
	pushl	40(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	44(%esp)
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2211
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2211:
	movl	16(%esp), %eax
	movl	$.LC365, 64(%esp)
	movl	%eax, 68(%esp)
.L2230:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	printf
	.align 16
.L2232:
	.cfi_restore_state
	movl	(%esp), %eax
	movl	4(%esp), %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	curr_port, %ebp
	movl	%eax, 74(%ebx)
	movl	%edx, 78(%ebx)
	movl	26(%ebx), %eax
	movl	30(%ebx), %edx
	addl	$1, 14(%ecx)
	movl	%eax, 58(%ecx)
	movl	%edx, 62(%ecx)
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	44(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%esi
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebp
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2206
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2206:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$512
	.cfi_def_cfa_offset 80
	call	pmem
	movl	20(%esp), %edx
	movl	%eax, %edi
	movl	16(%esp), %eax
	movl	curr_port, %ebp
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %esi
	shrl	$9, %esi
	movl	%eax, 40(%esp)
	pushl	%esi
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%edi
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%ebp
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2207
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2207:
	movl	(%esp), %eax
	movl	26(%ebx), %ecx
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 76
	movl	30(%ebx), %ebx
	andl	$511, %eax
	movl	%ebx, 70(%eax,%edi)
	movl	%ecx, 66(%eax,%edi)
	movl	curr_port, %ebx
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%esi
	.cfi_def_cfa_offset 84
	pushl	44(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%edi
	.cfi_def_cfa_offset 100
	jmp	.L2229
	.align 16
.L2200:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$16384001
	.cfi_def_cfa_offset 80
	call	get_special_dirs
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L2201
.L2231:
	movl	16(%esp), %eax
	movl	$.LC364, 64(%esp)
	movl	%eax, 68(%esp)
	jmp	.L2230
	.cfi_endproc
.LFE327:
	.size	create_directory, .-create_directory
	.section	.rodata
	.align 4
.LC366:
	.string	"\nFile \"%s\" Created Successfully\n"
	.text
	.align 16
	.globl	create_file
	.type	create_file, @function
create_file:
.LFB328:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	64(%esp), %eax
	movl	68(%esp), %ebx
	movl	%eax, 24(%esp)
	movl	72(%esp), %eax
	testl	%eax, %eax
	je	.L2234
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	search_folderOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	je	.L2267
.L2235:
	movl	30(%eax), %edx
	movl	26(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	%edx, 24(%esp)
	movl	%eax, 20(%esp)
	pushl	$512
	.cfi_def_cfa_offset 80
	call	pmem
	leal	512(%eax), %edx
	movl	%eax, 36(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	.align 16
.L2236:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L2236
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	call	sec_alloc
	movl	%eax, %esi
	movl	%edx, %eax
	movl	$1, (%esp)
	shrl	$9, %eax
	movl	curr_port, %ebp
	movl	%edx, %edi
	pushl	%eax
	.cfi_def_cfa_offset 84
	movl	%esi, %eax
	shrdl	$9, %edx, %eax
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	52(%esp)
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%ebp
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2237
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2237:
	movl	%esi, %ebp
	movl	24(%esp), %eax
	andl	$511, %ebp
	addl	20(%esp), %ebp
	movl	%eax, %edx
	movl	%esi, 26(%ebp)
	movl	%edi, 30(%ebp)
	movw	%bx, 0(%ebp)
	movzbl	(%eax), %ecx
	leal	66(%ebp), %eax
	.align 16
.L2238:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L2238
	movl	12(%esp), %edx
	movb	$0, (%eax)
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	20(%esp), %eax
	movl	$0, 2(%ebp)
	movl	$0, 50(%ebp)
	movl	$0, 54(%ebp)
	movl	%edx, 22(%ebp)
	movl	$0, 34(%ebp)
	movl	%eax, 18(%ebp)
	movl	$0, 38(%ebp)
	movl	$0, 42(%ebp)
	movl	$0, 46(%ebp)
	movl	$0, 14(%ebp)
	movl	$11465841, 6(%ebp)
	pushl	$512
	.cfi_def_cfa_offset 80
	call	pmem
	movl	28(%esp), %edx
	movl	%eax, %ebx
	movl	24(%esp), %eax
	movl	curr_port, %esi
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %edi
	shrl	$9, %edi
	movl	%eax, 44(%esp)
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%ebx
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2239
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2239:
	movl	8(%esp), %ecx
	andl	$511, %ecx
	addl	%ebx, %ecx
	movl	54(%ecx), %edx
	movl	50(%ecx), %eax
	movl	%edx, 12(%esp)
	movl	12(%esp), %edx
	movl	%eax, 8(%esp)
	orl	%eax, %edx
	jne	.L2268
	movl	26(%ebp), %eax
	movl	30(%ebp), %edx
	movl	%eax, 42(%ecx)
	movl	%edx, 46(%ecx)
	movl	%eax, 50(%ecx)
	movl	%edx, 54(%ecx)
	movl	$0, 58(%ebp)
	movl	$0, 62(%ebp)
	addl	$1, 10(%ecx)
.L2265:
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	48(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%ebx
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2244
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2244:
	movl	30(%ebp), %edx
	movl	26(%ebp), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	curr_port, %ebx
	pushl	$1
	.cfi_def_cfa_offset 80
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	52(%esp)
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2246
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2246:
	movl	24(%esp), %eax
	movl	$.LC366, 64(%esp)
	movl	%eax, 68(%esp)
.L2266:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	printf
.L2268:
	.cfi_restore_state
	movl	8(%esp), %eax
	movl	12(%esp), %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	curr_port, %esi
	movl	%eax, 58(%ebp)
	movl	%edx, 62(%ebp)
	movl	26(%ebp), %eax
	movl	30(%ebp), %edx
	addl	$1, 10(%ecx)
	movl	%eax, 50(%ecx)
	movl	%edx, 54(%ecx)
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	48(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%ebx
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2241
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2241:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$512
	.cfi_def_cfa_offset 80
	call	pmem
	movl	28(%esp), %edx
	movl	%eax, %ebx
	movl	24(%esp), %eax
	movl	curr_port, %esi
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %edi
	shrl	$9, %edi
	movl	%eax, 44(%esp)
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%ebx
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L2242
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L2242:
	movl	8(%esp), %eax
	movl	26(%ebp), %edx
	movl	30(%ebp), %ecx
	andl	$511, %eax
	movl	%edx, 50(%eax,%ebx)
	movl	%ecx, 54(%eax,%ebx)
	jmp	.L2265
.L2234:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$16384001
	.cfi_def_cfa_offset 80
	call	get_special_dirs
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L2235
.L2267:
	movl	24(%esp), %eax
	movl	$.LC364, 64(%esp)
	movl	%eax, 68(%esp)
	jmp	.L2266
	.cfi_endproc
.LFE328:
	.size	create_file, .-create_file
	.section	.rodata
.LC367:
	.string	"\nSearched Folder %s Not found"
.LC368:
	.string	"\nShowing %x folders of %s\\"
.LC369:
	.string	"\n\t /%s"
	.text
	.align 16
	.globl	find_friendDirs
	.type	find_friendDirs, @function
find_friendDirs:
.LFB329:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	48(%esp), %ebx
	testl	%ebx, %ebx
	je	.L2270
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	search_folderOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	je	.L2289
.L2271:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	30(%eax), %edi
	movl	26(%eax), %esi
	pushl	$512
	.cfi_def_cfa_offset 64
	call	pmem
	movl	curr_port, %ebx
	movl	%eax, %ebp
	movl	%edi, %eax
	shrdl	$9, %edi, %esi
	shrl	$9, %eax
	movl	$1, (%esp)
	pushl	%eax
	.cfi_def_cfa_offset 68
	pushl	%esi
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ebp
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2273
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2273:
	movl	34(%ebp), %edx
	movl	38(%ebp), %ecx
	leal	82(%ebp), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	xorl	%ebx, %ebx
	movl	%edx, 12(%esp)
	movl	%ecx, 16(%esp)
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	14(%ebp)
	.cfi_def_cfa_offset 60
	pushl	$.LC368
	.cfi_def_cfa_offset 64
	call	printf
	movl	$512, (%esp)
	call	pmem
	movl	14(%ebp), %edx
	movl	%eax, %esi
	leal	82(%eax), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	12(%esp), %ecx
	movl	%eax, 4(%esp)
	testl	%edx, %edx
	movl	8(%esp), %edx
	je	.L2277
	.align 16
.L2282:
	movl	curr_port, %edi
	movl	%ecx, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	shrdl	$9, %ecx, %edx
	shrl	$9, %eax
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%eax
	.cfi_def_cfa_offset 68
	pushl	%edx
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%esi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%edi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2275
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2275:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	addl	$1, %ebx
	pushl	12(%esp)
	.cfi_def_cfa_offset 60
	pushl	$.LC369
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	%ebx, 14(%ebp)
	movl	66(%esi), %edx
	movl	70(%esi), %ecx
	ja	.L2282
.L2277:
	movl	$.LC196, 48(%esp)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	printf
	.align 16
.L2270:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$16384001
	.cfi_def_cfa_offset 64
	call	get_special_dirs
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L2271
.L2289:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	$.LC367
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE329:
	.size	find_friendDirs, .-find_friendDirs
	.section	.rodata
.LC370:
	.string	"\nShowing %x files of %s\\"
.LC371:
	.string	"\n\t\t /%s"
	.text
	.align 16
	.globl	find_childFiles
	.type	find_childFiles, @function
find_childFiles:
.LFB330:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	48(%esp), %ebx
	testl	%ebx, %ebx
	je	.L2291
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	search_folderOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	je	.L2314
.L2292:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	30(%eax), %edi
	movl	26(%eax), %esi
	pushl	$512
	.cfi_def_cfa_offset 64
	call	pmem
	movl	curr_port, %ebx
	movl	%eax, %ebp
	movl	%edi, %eax
	shrdl	$9, %edi, %esi
	shrl	$9, %eax
	movl	$1, (%esp)
	pushl	%eax
	.cfi_def_cfa_offset 68
	pushl	%esi
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ebp
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2294
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2294:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$512
	.cfi_def_cfa_offset 64
	call	pmem
	movl	46(%ebp), %edx
	movl	%eax, %edi
	movl	42(%ebp), %eax
	movl	curr_port, %ebx
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2295
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2295:
	leal	82(%ebp), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	xorl	%ebx, %ebx
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	10(%ebp)
	.cfi_def_cfa_offset 60
	pushl	$.LC370
	.cfi_def_cfa_offset 64
	call	printf
	movl	10(%ebp), %esi
	leal	66(%edi), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, 12(%esp)
	testl	%esi, %esi
	je	.L2299
	.align 16
.L2306:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	20(%esp)
	.cfi_def_cfa_offset 60
	pushl	$.LC371
	.cfi_def_cfa_offset 64
	call	printf
	movl	54(%edi), %edx
	movl	50(%edi), %eax
	movl	curr_port, %esi
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2297
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2297:
	addl	$1, %ebx
	cmpl	%ebx, 10(%ebp)
	ja	.L2306
.L2299:
	movl	$.LC196, 48(%esp)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	printf
	.align 16
.L2291:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$16384001
	.cfi_def_cfa_offset 64
	call	get_special_dirs
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L2292
.L2314:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	$.LC367
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE330:
	.size	find_childFiles, .-find_childFiles
	.section	.rodata
.LC372:
	.string	"/"
	.text
	.align 16
	.globl	search_folderOGP
	.type	search_folderOGP, @function
search_folderOGP:
.LFB331:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	48(%esp), %esi
	testl	%esi, %esi
	je	.L2371
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$512
	.cfi_def_cfa_offset 64
	call	pmem
	movl	$512, (%esp)
	movl	%eax, %edi
	call	pmem
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, %ebx
	xorl	%eax, %eax
	cmpb	$0, (%esi)
	movl	kmalloc, %edx
	je	.L2317
	.align 16
.L2318:
	addl	$1, %eax
	cmpb	$0, (%esi,%eax)
	jne	.L2318
.L2317:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	*%edx
	movzbl	(%esi), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testb	%dl, %dl
	je	.L2319
	xorl	%edx, %edx
	.align 16
.L2320:
	addl	$1, %edx
	cmpb	$0, (%esi,%edx)
	jne	.L2320
	addl	%eax, %edx
	movl	%eax, %ecx
	.align 16
.L2321:
	addl	$1, %ecx
	movb	$0, -1(%ecx)
	cmpl	%edx, %ecx
	jne	.L2321
	movzbl	(%esi), %edx
.L2319:
	movl	%esi, %ebp
	movl	%eax, %ecx
	.align 16
.L2322:
	addl	$1, %ecx
	addl	$1, %ebp
	movb	%dl, -1(%ecx)
	movzbl	0(%ebp), %edx
	testb	%dl, %dl
	jne	.L2322
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	movb	$0, (%ecx)
	pushl	$.LC372
	.cfi_def_cfa_offset 60
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	strtok
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, %ebp
	testl	%eax, %eax
	cmove	%esi, %ebp
	cmpb	$47, (%esi)
	je	.L2324
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$16384001
	.cfi_def_cfa_offset 64
	call	get_special_dirs
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2325:
	movl	30(%eax), %edx
	movl	26(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	curr_port, %esi
	pushl	$1
	.cfi_def_cfa_offset 64
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2326
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2326:
	leal	82(%ebx), %eax
	movl	%eax, 12(%esp)
.L2340:
	movl	38(%edi), %ecx
	movl	34(%edi), %edx
	movl	%ecx, %esi
	orl	%edx, %esi
	je	.L2337
	movl	%ecx, %eax
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	shrl	$9, %eax
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%eax
	.cfi_def_cfa_offset 68
	movl	%edx, %eax
	movl	%ecx, %edx
	.align 16
.L2370:
	shrdl	$9, %edx, %eax
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ebx
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2331
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2331:
	movzbl	82(%ebx), %eax
	cmpb	0(%ebp), %al
	jne	.L2332
	testb	%al, %al
	je	.L2344
	movl	%ebp, %edx
	movl	12(%esp), %eax
	jmp	.L2335
	.align 16
.L2336:
	testb	%cl, %cl
	je	.L2344
.L2335:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L2336
.L2332:
	movl	70(%ebx), %edx
	movl	66(%ebx), %eax
	movl	%edx, %esi
	orl	%eax, %esi
	je	.L2337
	movl	%edx, %ecx
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 60
	movl	curr_port, %esi
	shrl	$9, %ecx
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ecx
	.cfi_def_cfa_offset 68
	jmp	.L2370
	.align 16
.L2344:
	.cfi_restore_state
	xorl	%edx, %edx
	.align 16
.L2339:
	movzbl	(%ebx,%edx), %eax
	movb	%al, (%edi,%edx)
	addl	$1, %edx
	cmpl	$146, %edx
	jne	.L2339
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	$.LC372
	.cfi_def_cfa_offset 60
	pushl	$0
	.cfi_def_cfa_offset 64
	call	strtok
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	movl	%eax, %ebp
	jne	.L2340
	movl	%edi, %eax
	jmp	.L2315
	.align 16
.L2337:
	xorl	%eax, %eax
.L2315:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L2324:
	.cfi_restore_state
	call	get_special_dirs.part.9
	jmp	.L2325
.L2371:
	movl	$16384001, 48(%esp)
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	get_special_dirs
	.cfi_endproc
.LFE331:
	.size	search_folderOGP, .-search_folderOGP
	.align 16
	.globl	search_folderOGP_T
	.type	search_folderOGP_T, @function
search_folderOGP_T:
.LFB332:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	48(%esp), %esi
	testl	%esi, %esi
	je	.L2414
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$512
	.cfi_def_cfa_offset 64
	call	pmem
	movl	$512, (%esp)
	movl	%eax, %edi
	call	pmem
	popl	%ebp
	.cfi_def_cfa_offset 60
	movl	%eax, %ebx
	popl	%eax
	.cfi_def_cfa_offset 56
	pushl	$.LC372
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	strtok
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, %ebp
	testl	%eax, %eax
	cmove	%esi, %ebp
	cmpb	$47, (%esi)
	je	.L2375
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$16384001
	.cfi_def_cfa_offset 64
	call	get_special_dirs
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2376:
	movl	30(%eax), %edx
	movl	26(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	curr_port, %esi
	pushl	$1
	.cfi_def_cfa_offset 64
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2377
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2377:
	leal	82(%ebx), %eax
	movl	%eax, 12(%esp)
.L2391:
	movl	38(%edi), %ecx
	movl	34(%edi), %edx
	movl	%ecx, %esi
	orl	%edx, %esi
	je	.L2388
	movl	%ecx, %eax
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	shrl	$9, %eax
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%eax
	.cfi_def_cfa_offset 68
	movl	%edx, %eax
	movl	%ecx, %edx
	.align 16
.L2413:
	shrdl	$9, %edx, %eax
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ebx
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2382
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2382:
	movzbl	82(%ebx), %eax
	cmpb	0(%ebp), %al
	jne	.L2383
	testb	%al, %al
	je	.L2394
	movl	%ebp, %edx
	movl	12(%esp), %eax
	jmp	.L2386
	.align 16
.L2387:
	testb	%cl, %cl
	je	.L2394
.L2386:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L2387
.L2383:
	movl	70(%ebx), %edx
	movl	66(%ebx), %eax
	movl	%edx, %esi
	orl	%eax, %esi
	je	.L2388
	movl	%edx, %ecx
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 60
	movl	curr_port, %esi
	shrl	$9, %ecx
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ecx
	.cfi_def_cfa_offset 68
	jmp	.L2413
	.align 16
.L2394:
	.cfi_restore_state
	xorl	%edx, %edx
	.align 16
.L2390:
	movzbl	(%ebx,%edx), %eax
	movb	%al, (%edi,%edx)
	addl	$1, %edx
	cmpl	$146, %edx
	jne	.L2390
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	$.LC372
	.cfi_def_cfa_offset 60
	pushl	$0
	.cfi_def_cfa_offset 64
	call	strtok
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	movl	%eax, %ebp
	jne	.L2391
	movl	%edi, %eax
	jmp	.L2372
	.align 16
.L2388:
	xorl	%eax, %eax
.L2372:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L2375:
	.cfi_restore_state
	call	get_special_dirs.part.9
	jmp	.L2376
.L2414:
	movl	$16384001, 48(%esp)
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	get_special_dirs
	.cfi_endproc
.LFE332:
	.size	search_folderOGP_T, .-search_folderOGP_T
	.align 16
	.globl	search_folder
	.type	search_folder, @function
search_folder:
.LFB333:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	pushl	$512
	.cfi_def_cfa_offset 48
	call	pmem
	movl	%eax, %ebx
	movl	curr_dir, %eax
	movl	curr_port, %esi
	movl	30(%eax), %edx
	movl	26(%eax), %eax
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ebx
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%esi
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L2416
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L2416:
	movl	curr_dir, %eax
	movl	26(%eax), %eax
	andl	$511, %eax
	movl	38(%eax,%ebx), %edi
	movl	34(%eax,%ebx), %esi
	movl	%edi, %edx
	orl	%esi, %edx
	je	.L2417
	.align 16
.L2450:
	movl	%edi, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	movl	curr_port, %ebp
	shrl	$9, %eax
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%eax
	.cfi_def_cfa_offset 52
	movl	%esi, %eax
	shrdl	$9, %edi, %eax
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ebx
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%ebp
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L2418
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L2418:
	movl	32(%esp), %eax
	andl	$511, %esi
	addl	%ebx, %esi
	leal	82(%esi), %edx
	movzbl	(%eax), %eax
	cmpb	82(%esi), %al
	jne	.L2419
	testb	%al, %al
	je	.L2415
	movl	32(%esp), %ecx
	jmp	.L2422
	.align 16
.L2423:
	testb	%al, %al
	je	.L2415
.L2422:
	addl	$1, %edx
	addl	$1, %ecx
	movzbl	(%edx), %eax
	cmpb	(%ecx), %al
	je	.L2423
.L2419:
	movl	70(%esi), %edi
	movl	66(%esi), %esi
	movl	%edi, %edx
	orl	%esi, %edx
	jne	.L2450
.L2417:
	xorl	%esi, %esi
.L2415:
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	movl	%esi, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE333:
	.size	search_folder, .-search_folder
	.section	.rodata
.LC373:
	.string	"\n>%s"
	.text
	.align 16
	.globl	file_loaderOGP
	.type	file_loaderOGP, @function
file_loaderOGP:
.LFB334:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	movl	60(%esp), %ebx
	pushl	$512
	.cfi_def_cfa_offset 64
	call	pmem
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpb	$0, (%ebx)
	movl	%eax, %edi
	movl	kmalloc, %ecx
	je	.L2477
	xorl	%eax, %eax
	jmp	.L2453
	.align 16
.L2478:
	movl	%edx, %eax
.L2453:
	cmpb	$0, 1(%ebx,%eax)
	leal	1(%eax), %edx
	jne	.L2478
	addl	$2, %eax
.L2452:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	*%ecx
	movzbl	(%ebx), %ecx
	movl	%eax, %esi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%ebx, %edx
	.align 16
.L2454:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L2454
	movb	$0, (%eax)
	cmpb	$0, (%esi)
	je	.L2513
	leal	1(%esi), %eax
	xorl	%edx, %edx
	jmp	.L2456
	.align 16
.L2479:
	movl	%ecx, %edx
.L2456:
	movl	%eax, %ebp
	addl	$1, %eax
	leal	1(%edx), %ecx
	cmpb	$0, -1(%eax)
	jne	.L2479
	leal	(%esi,%edx), %ecx
	movl	%edx, %eax
	cmpb	$47, (%ecx)
	je	.L2457
	testl	%edx, %edx
	jns	.L2460
	jmp	.L2481
	.align 16
.L2459:
	cmpl	$-1, %eax
	je	.L2513
.L2460:
	subl	$1, %eax
	cmpb	$47, (%esi,%eax)
	leal	(%esi,%eax), %ecx
	jne	.L2459
	leal	1(%esi,%eax), %ebp
.L2457:
	testl	%eax, %eax
	jle	.L2482
	movb	$0, (%ecx)
	jmp	.L2461
.L2481:
	movl	%ebp, %esi
.L2513:
	movl	%esi, %ebp
	xorl	%esi, %esi
.L2461:
	cmpb	$0, (%ebx)
	movl	kmalloc, %edx
	je	.L2483
	xorl	%eax, %eax
	jmp	.L2463
	.align 16
.L2484:
	movl	%ecx, %eax
.L2463:
	cmpb	$0, 1(%ebx,%eax)
	leal	1(%eax), %ecx
	jne	.L2484
	addl	$2, %eax
.L2462:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	*%edx
	movzbl	(%ebx), %ecx
	movl	%eax, 28(%esp)
	movl	%ebx, %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	.align 16
.L2464:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L2464
	movb	$0, (%eax)
	xorl	%eax, %eax
	cmpb	$0, (%ebx)
	je	.L2465
	.align 16
.L2466:
	addl	$1, %eax
	cmpb	$0, (%ebx,%eax)
	jne	.L2466
.L2465:
	movl	12(%esp), %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movb	$0, (%ebx,%eax)
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	search_folderOGP_T
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	je	.L2467
	movl	46(%eax), %edx
	movl	42(%eax), %eax
	movl	%edx, %ebx
	orl	%eax, %ebx
	je	.L2467
	movl	curr_port, %ebx
	movl	%edx, %ecx
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	shrdl	$9, %edx, %eax
	shrl	$9, %ecx
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L2514
.L2468:
	leal	66(%edi), %ebx
	.align 16
.L2474:
	movzbl	66(%edi), %eax
	cmpb	0(%ebp), %al
	jne	.L2469
	testb	%al, %al
	je	.L2470
	movl	%ebp, %edx
	movl	%ebx, %eax
	jmp	.L2472
	.align 16
.L2473:
	testb	%cl, %cl
	je	.L2470
.L2472:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L2473
.L2469:
	movl	54(%edi), %edx
	movl	50(%edi), %eax
	movl	%edx, %esi
	orl	%eax, %esi
	je	.L2467
	movl	curr_port, %esi
	movl	%edx, %ecx
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	shrdl	$9, %edx, %eax
	shrl	$9, %ecx
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2474
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L2474
	.align 16
.L2470:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	$.LC373
	.cfi_def_cfa_offset 64
	call	printf
	movl	$32, (%esp)
	call	pmem
	movl	%edi, 8(%eax)
	movl	28(%esp), %edi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%ebx, (%eax)
	movl	%edi, 4(%eax)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L2467:
	.cfi_restore_state
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L2514:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L2468
.L2483:
	movl	$1, %eax
	jmp	.L2462
.L2477:
	movl	$1, %eax
	jmp	.L2452
.L2482:
	xorl	%esi, %esi
	jmp	.L2461
	.cfi_endproc
.LFE334:
	.size	file_loaderOGP, .-file_loaderOGP
	.align 16
	.globl	file_loader
	.type	file_loader, @function
file_loader:
.LFB335:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	pushl	$512
	.cfi_def_cfa_offset 48
	call	pmem
	movl	%eax, %ebx
	movl	curr_dir, %eax
	movl	curr_port, %esi
	movl	30(%eax), %edx
	movl	26(%eax), %eax
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ebx
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%esi
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L2516
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L2516:
	movl	curr_dir, %eax
	movl	26(%eax), %eax
	andl	$511, %eax
	movl	46(%eax,%ebx), %edi
	movl	42(%eax,%ebx), %esi
	movl	%edi, %edx
	orl	%esi, %edx
	je	.L2525
	.align 16
.L2517:
	movl	%edi, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	movl	curr_port, %ebp
	shrl	$9, %eax
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%eax
	.cfi_def_cfa_offset 52
	movl	%esi, %eax
	shrdl	$9, %edi, %eax
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ebx
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%ebp
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L2519
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L2519:
	movl	32(%esp), %edi
	andl	$511, %esi
	addl	%ebx, %esi
	leal	66(%esi), %eax
	movzbl	(%edi), %edx
	cmpb	66(%esi), %dl
	jne	.L2520
	testb	%dl, %dl
	je	.L2521
	movl	%edi, %edx
	jmp	.L2523
	.align 16
.L2524:
	testb	%cl, %cl
	je	.L2521
.L2523:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L2524
.L2520:
	movl	54(%esi), %edi
	movl	50(%esi), %esi
	movl	%edi, %edx
	orl	%esi, %edx
	jne	.L2517
.L2525:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2521:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$32
	.cfi_def_cfa_offset 48
	call	pmem
	movl	$162, (%esp)
	movl	%eax, %ebx
	call	pmem
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	xorl	%edx, %edx
	.align 16
.L2528:
	movzbl	(%esi,%edx), %ecx
	movb	%cl, (%eax,%edx)
	addl	$1, %edx
	cmpl	$162, %edx
	jne	.L2528
	movl	%eax, 8(%ebx)
	addl	$66, %eax
	movl	%eax, (%ebx)
	movl	%eax, 4(%ebx)
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE335:
	.size	file_loader, .-file_loader
	.align 16
	.globl	file_loadOGP
	.type	file_loadOGP, @function
file_loadOGP:
.LFB336:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	28(%esp)
	.cfi_def_cfa_offset 32
	call	file_loaderOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	je	.L2555
	movl	start_handle, %edx
	testl	%edx, %edx
	je	.L2557
	movl	current, %edx
	movl	%eax, 28(%edx)
.L2554:
	movl	%eax, current
	movl	$0, 28(%eax)
	movl	$1, %eax
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2557:
	.cfi_restore_state
	movl	%eax, start_handle
	jmp	.L2554
	.align 16
.L2555:
	xorl	%eax, %eax
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE336:
	.size	file_loadOGP, .-file_loadOGP
	.align 16
	.globl	file_load
	.type	file_load, @function
file_load:
.LFB337:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	28(%esp)
	.cfi_def_cfa_offset 32
	call	file_loader
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	je	.L2562
	movl	start_handle, %edx
	testl	%edx, %edx
	je	.L2564
	movl	current, %edx
	movl	%eax, 28(%edx)
.L2561:
	movl	%eax, current
	movl	$0, 28(%eax)
	movl	$1, %eax
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2564:
	.cfi_restore_state
	movl	%eax, start_handle
	jmp	.L2561
	.align 16
.L2562:
	xorl	%eax, %eax
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE337:
	.size	file_load, .-file_load
	.align 16
	.globl	file_closeOGP
	.type	file_closeOGP, @function
file_closeOGP:
.LFB338:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	xorl	%eax, %eax
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	kmalloc, %edx
	movl	48(%esp), %esi
	cmpb	$0, (%esi)
	je	.L2566
	.align 16
.L2567:
	addl	$1, %eax
	cmpb	$0, (%esi,%eax)
	jne	.L2567
.L2566:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	*%edx
	movzbl	(%esi), %ebx
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%esi, %ecx
	movl	%eax, %edx
	.align 16
.L2568:
	addl	$1, %edx
	addl	$1, %ecx
	movb	%bl, -1(%edx)
	movzbl	(%ecx), %ebx
	testb	%bl, %bl
	jne	.L2568
	movb	$0, (%edx)
	cmpb	$0, (%eax)
	je	.L2569
	xorl	%edx, %edx
	jmp	.L2570
	.align 16
.L2589:
	movl	%ecx, %edx
.L2570:
	cmpb	$0, 1(%eax,%edx)
	leal	1(%edx), %ecx
	jne	.L2589
	leal	(%eax,%edx), %ecx
	cmpb	$47, (%ecx)
	je	.L2571
	testl	%edx, %edx
	jns	.L2572
	jmp	.L2569
	.align 16
.L2612:
	cmpl	$-1, %edx
	je	.L2569
.L2572:
	subl	$1, %edx
	cmpb	$47, (%eax,%edx)
	leal	(%eax,%edx), %ecx
	jne	.L2612
.L2571:
	testl	%edx, %edx
	jle	.L2569
	movb	$0, (%ecx)
.L2569:
	movl	start_handle, %ebx
	testl	%ebx, %ebx
	je	.L2565
	movzbl	(%esi), %edi
	xorl	%ebp, %ebp
	movl	$0, 12(%esp)
	.align 16
.L2580:
	movl	4(%ebx), %eax
	movl	%edi, %edx
	cmpb	%dl, (%eax)
	jne	.L2575
	testb	%dl, %dl
	je	.L2576
	movl	%esi, %edx
	jmp	.L2578
	.align 16
.L2579:
	testb	%cl, %cl
	je	.L2576
.L2578:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L2579
.L2575:
	movl	28(%ebx), %eax
	addl	$1, %ebp
	movl	%ebx, 12(%esp)
	testl	%eax, %eax
	je	.L2565
	movl	%eax, %ebx
	jmp	.L2580
	.align 16
.L2576:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$512
	.cfi_def_cfa_offset 64
	call	pmem
	movl	8(%ebx), %edi
	movl	%eax, 24(%esp)
	movl	curr_port, %esi
	movl	30(%edi), %edx
	movl	26(%edi), %eax
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	40(%esp)
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L2613
.L2587:
	movl	8(%esp), %edx
	leal	162(%edi), %ecx
	movl	%edi, %eax
	movl	%ebx, %esi
	subl	%edi, %edx
	.align 16
.L2582:
	addl	$1, %eax
	movzbl	-1(%eax), %ebx
	cmpl	%eax, %ecx
	movb	%bl, -1(%eax,%edx)
	jne	.L2582
	movl	30(%edi), %edx
	movl	26(%edi), %eax
	movl	%esi, %ebx
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	40(%esp)
	.cfi_def_cfa_offset 84
	pushl	$1
	.cfi_def_cfa_offset 88
	pushl	$48
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L2614
	testl	%ebp, %ebp
	je	.L2615
.L2584:
	movl	12(%esp), %edi
	testl	%edi, %edi
	je	.L2585
	movl	28(%ebx), %eax
	movl	%eax, 28(%edi)
.L2585:
	cmpl	%ebx, current
	je	.L2616
.L2565:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L2615:
	.cfi_restore_state
	movl	28(%ebx), %eax
	movl	%eax, start_handle
	jmp	.L2585
.L2614:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC215
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%ebp, %ebp
	jne	.L2584
	jmp	.L2615
.L2616:
	movl	12(%esp), %eax
	movl	%eax, current
	jmp	.L2565
	.align 16
.L2613:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L2587
	.cfi_endproc
.LFE338:
	.size	file_closeOGP, .-file_closeOGP
	.align 16
	.globl	file_close
	.type	file_close, @function
file_close:
.LFB339:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	pushl	44(%esp)
	.cfi_def_cfa_offset 48
	call	file_flush
	movl	start_handle, %ebx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%ebx, %ebx
	je	.L2617
	movl	32(%esp), %eax
	xorl	%esi, %esi
	xorl	%ebp, %ebp
	movzbl	(%eax), %edi
	.align 16
.L2625:
	movl	(%ebx), %eax
	movl	%edi, %edx
	cmpb	(%eax), %dl
	jne	.L2620
	testb	%dl, %dl
	je	.L2621
	movl	32(%esp), %edx
	jmp	.L2623
	.align 16
.L2624:
	testb	%cl, %cl
	je	.L2621
.L2623:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L2624
.L2620:
	movl	28(%ebx), %eax
	addl	$1, %esi
	movl	%ebx, %ebp
	testl	%eax, %eax
	je	.L2617
	movl	%eax, %ebx
	jmp	.L2625
	.align 16
.L2621:
	testl	%esi, %esi
	jne	.L2645
	movl	28(%ebx), %eax
	movl	%eax, start_handle
.L2627:
	cmpl	%ebx, current
	je	.L2646
.L2617:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2645:
	.cfi_restore_state
	testl	%ebp, %ebp
	je	.L2627
	cmpl	%ebx, current
	movl	28(%ebx), %eax
	movl	%eax, 28(%ebp)
	jne	.L2617
.L2646:
	movl	%ebp, current
	jmp	.L2617
	.cfi_endproc
.LFE339:
	.size	file_close, .-file_close
	.align 16
	.globl	file_searchOGP
	.type	file_searchOGP, @function
file_searchOGP:
.LFB340:
	.cfi_startproc
	movl	start_handle, %eax
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %edi
	testl	%eax, %eax
	je	.L2647
	movzbl	(%edi), %esi
	.align 16
.L2653:
	movl	4(%eax), %edx
	movl	%esi, %ecx
	cmpb	(%edx), %cl
	jne	.L2649
	testb	%cl, %cl
	je	.L2647
	movl	%edi, %ecx
	jmp	.L2651
	.align 16
.L2652:
	testb	%bl, %bl
	je	.L2647
.L2651:
	addl	$1, %edx
	addl	$1, %ecx
	movzbl	(%edx), %ebx
	cmpb	(%ecx), %bl
	je	.L2652
.L2649:
	movl	28(%eax), %eax
	testl	%eax, %eax
	jne	.L2653
.L2647:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE340:
	.size	file_searchOGP, .-file_searchOGP
	.align 16
	.globl	file_search
	.type	file_search, @function
file_search:
.LFB341:
	.cfi_startproc
	movl	start_handle, %eax
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %edi
	testl	%eax, %eax
	je	.L2665
	movzbl	(%edi), %esi
	.align 16
.L2671:
	movl	(%eax), %edx
	movl	%esi, %ecx
	cmpb	(%edx), %cl
	jne	.L2667
	testb	%cl, %cl
	je	.L2665
	movl	%edi, %ecx
	jmp	.L2669
	.align 16
.L2670:
	testb	%bl, %bl
	je	.L2665
.L2669:
	addl	$1, %edx
	addl	$1, %ecx
	movzbl	(%edx), %ebx
	cmpb	(%ecx), %bl
	je	.L2670
.L2667:
	movl	28(%eax), %eax
	testl	%eax, %eax
	jne	.L2671
.L2665:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE341:
	.size	file_search, .-file_search
	.section	.rodata
.LC374:
	.string	"\n%s"
.LC375:
	.string	"\ncurr dir: %s"
	.text
	.align 16
	.globl	set_curr_dir
	.type	set_curr_dir, @function
set_curr_dir:
.LFB342:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$72, %esp
	.cfi_def_cfa_offset 92
	movl	96(%esp), %ebp
	movl	92(%esp), %edi
	pushl	$4096
	.cfi_def_cfa_offset 96
	call	pmem
	movl	curr_port, %ebx
	movl	%eax, %esi
	movl	%ebp, %eax
	shrdl	$9, %ebp, %edi
	shrl	$9, %eax
	movl	$1, (%esp)
	pushl	%eax
	.cfi_def_cfa_offset 100
	pushl	%edi
	.cfi_def_cfa_offset 104
	pushl	$512
	.cfi_def_cfa_offset 108
	pushl	$1
	.cfi_def_cfa_offset 112
	pushl	%esi
	.cfi_def_cfa_offset 116
	pushl	$0
	.cfi_def_cfa_offset 120
	pushl	$32
	.cfi_def_cfa_offset 124
	pushl	%ebx
	.cfi_def_cfa_offset 128
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 80
	testl	$1073741824, %eax
	je	.L2684
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$.LC214
	.cfi_def_cfa_offset 96
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 80
.L2684:
	movl	%esi, curr_dir
	movl	$0, 16(%esp)
	xorl	%eax, %eax
.L2685:
	movl	$0, 20(%esp,%eax)
	addl	$4, %eax
	cmpl	$28, %eax
	jb	.L2685
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$4096
	.cfi_def_cfa_offset 96
	call	pmem
	movl	22(%esi), %edx
	movl	%eax, %ebx
	movl	18(%esi), %eax
	movl	%edx, 28(%esp)
	movl	%eax, 24(%esp)
	movl	%esi, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	xorl	%esi, %esi
	.align 16
.L2687:
	cmpb	$114, 82(%eax)
	leal	82(%eax), %edi
	movl	$.LC363, %edx
	movl	%edi, %ecx
	je	.L2721
	jmp	.L2689
	.align 16
.L2692:
	testb	%al, %al
	je	.L2690
.L2721:
	addl	$1, %ecx
	addl	$1, %edx
	movzbl	(%ecx), %eax
	cmpb	(%edx), %al
	je	.L2692
.L2689:
	subl	$8, %esp
	.cfi_def_cfa_offset 88
	movl	%edi, 0(,%esi,4)
	pushl	%edi
	.cfi_def_cfa_offset 92
	pushl	$.LC374
	.cfi_def_cfa_offset 96
	call	printf
	movl	28(%esp), %ecx
	movl	24(%esp), %edx
	movl	$1, (%esp)
	movl	curr_port, %edi
	movl	%ecx, %eax
	shrl	$9, %eax
	pushl	%eax
	.cfi_def_cfa_offset 100
	movl	%edx, %eax
	shrdl	$9, %ecx, %eax
	pushl	%eax
	.cfi_def_cfa_offset 104
	pushl	$512
	.cfi_def_cfa_offset 108
	pushl	$1
	.cfi_def_cfa_offset 112
	pushl	%ebx
	.cfi_def_cfa_offset 116
	pushl	$0
	.cfi_def_cfa_offset 120
	pushl	$32
	.cfi_def_cfa_offset 124
	pushl	%edi
	.cfi_def_cfa_offset 128
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 80
	testl	$1073741824, %eax
	je	.L2694
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$.LC214
	.cfi_def_cfa_offset 96
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 80
.L2694:
	movl	8(%esp), %eax
	addl	$1, %esi
	andl	$511, %eax
	addl	%ebx, %eax
	movl	18(%eax), %edi
	movl	22(%eax), %ebp
	movl	%edi, 8(%esp)
	movl	%ebp, 12(%esp)
	jmp	.L2687
	.align 16
.L2690:
	testl	%esi, %esi
	movl	$.LC363, 0(,%esi,4)
	je	.L2706
.L2712:
	cmpb	$0, 16(%esp)
	movl	0(,%esi,4), %ecx
	je	.L2707
	leal	17(%esp), %edx
	.align 16
.L2696:
	movl	%edx, %eax
	addl	$1, %edx
	cmpb	$0, -1(%edx)
	jne	.L2696
.L2695:
	movzbl	(%ecx), %ebx
	movl	%ecx, %edx
	.align 16
.L2697:
	addl	$1, %eax
	addl	$1, %edx
	movb	%bl, -1(%eax)
	movzbl	(%edx), %ebx
	testb	%bl, %bl
	jne	.L2697
	movb	$0, (%eax)
	cmpb	$0, 16(%esp)
	je	.L2708
	leal	17(%esp), %eax
	.align 16
.L2699:
	movl	%eax, %edx
	addl	$1, %eax
	cmpb	$0, -1(%eax)
	jne	.L2699
	subl	$1, %esi
	movb	$92, (%edx)
	movb	$0, 1(%edx)
	jne	.L2712
.L2706:
	cmpb	$0, 16(%esp)
	je	.L2709
	leal	17(%esp), %edx
	.align 16
.L2702:
	movl	%edx, %eax
	addl	$1, %edx
	cmpb	$0, -1(%edx)
	jne	.L2702
.L2701:
	movl	0, %edx
	movzbl	(%edx), %ecx
	.align 16
.L2703:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L2703
	movb	$0, (%eax)
	movzbl	16(%esp), %ecx
	leal	16(%esp), %edx
	movl	$curr_dir+6, %eax
	.align 16
.L2704:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L2704
	subl	$8, %esp
	.cfi_def_cfa_offset 88
	movb	$0, (%eax)
	leal	24(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 92
	pushl	$.LC375
	.cfi_def_cfa_offset 96
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	addl	$60, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L2708:
	.cfi_restore_state
	leal	16(%esp), %edx
	subl	$1, %esi
	movb	$92, (%edx)
	movb	$0, 1(%edx)
	jne	.L2712
	jmp	.L2706
	.align 16
.L2707:
	leal	16(%esp), %eax
	jmp	.L2695
.L2709:
	leal	16(%esp), %eax
	jmp	.L2701
	.cfi_endproc
.LFE342:
	.size	set_curr_dir, .-set_curr_dir
	.align 16
	.globl	get_special_dirs
	.type	get_special_dirs, @function
get_special_dirs:
.LFB343:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$4, %esp
	.cfi_def_cfa_offset 16
	movl	curr_dir, %ebx
	movl	16(%esp), %eax
	cmpl	$16384001, %eax
	je	.L2736
	cmpl	$16384002, %eax
	je	.L2737
.L2728:
	addl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2736:
	.cfi_restore_state
	movl	30(%ebx), %edx
	movl	26(%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	curr_port, %esi
	pushl	$1
	.cfi_def_cfa_offset 32
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 36
	pushl	%eax
	.cfi_def_cfa_offset 40
	pushl	$512
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%ebx
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$32
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	je	.L2728
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC214
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L2728
	.align 16
.L2737:
	addl	$4, %esp
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	jmp	get_special_dirs.part.9
	.cfi_endproc
.LFE343:
	.size	get_special_dirs, .-get_special_dirs
	.align 16
	.globl	get_dir
	.type	get_dir, @function
get_dir:
.LFB344:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	movl	48(%esp), %edi
	movl	44(%esp), %esi
	pushl	$512
	.cfi_def_cfa_offset 48
	call	*kmalloc
	movl	%edi, %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, %ebx
	orl	%esi, %edx
	je	.L2739
	movl	curr_port, %ebp
	movl	%edi, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	shrdl	$9, %edi, %esi
	shrl	$9, %eax
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%eax
	.cfi_def_cfa_offset 52
	pushl	%esi
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ebx
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%ebp
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	jne	.L2750
	cmpl	$11465840, 6(%ebx)
	movl	%ebx, %eax
	jne	.L2739
.L2738:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2750:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	cmpl	$11465840, 6(%ebx)
	movl	%ebx, %eax
	je	.L2738
.L2739:
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE344:
	.size	get_dir, .-get_dir
	.align 16
	.globl	get_file
	.type	get_file, @function
get_file:
.LFB345:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	movl	48(%esp), %edi
	movl	44(%esp), %esi
	pushl	$512
	.cfi_def_cfa_offset 48
	call	*kmalloc
	movl	%edi, %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, %ebx
	orl	%esi, %edx
	je	.L2752
	movl	curr_port, %ebp
	movl	%edi, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	shrdl	$9, %edi, %esi
	shrl	$9, %eax
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%eax
	.cfi_def_cfa_offset 52
	pushl	%esi
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ebx
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%ebp
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	jne	.L2763
	cmpl	$11465841, 6(%ebx)
	movl	%ebx, %eax
	jne	.L2752
.L2751:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2763:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	cmpl	$11465841, 6(%ebx)
	movl	%ebx, %eax
	je	.L2751
.L2752:
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE345:
	.size	get_file, .-get_file
	.align 16
	.globl	flush_dir
	.type	flush_dir, @function
flush_dir:
.LFB346:
	.cfi_startproc
	movl	4(%esp), %eax
	testl	%eax, %eax
	je	.L2764
	cmpl	$11465840, 6(%eax)
	je	.L2769
.L2764:
	xorl	%eax, %eax
	ret
	.align 16
.L2769:
	jmp	flush_dir.part.10
	.cfi_endproc
.LFE346:
	.size	flush_dir, .-flush_dir
	.align 16
	.globl	flush_file
	.type	flush_file, @function
flush_file:
.LFB347:
	.cfi_startproc
	movl	4(%esp), %eax
	testl	%eax, %eax
	je	.L2770
	cmpl	$11465841, 6(%eax)
	je	.L2775
.L2770:
	xorl	%eax, %eax
	ret
	.align 16
.L2775:
	jmp	flush_file.part.11
	.cfi_endproc
.LFE347:
	.size	flush_file, .-flush_file
	.section	.rodata
.LC376:
	.string	"\n%x"
	.text
	.align 16
	.globl	delete_dir
	.type	delete_dir, @function
delete_dir:
.LFB348:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$20, %esp
	.cfi_def_cfa_offset 40
	movl	40(%esp), %ebx
	pushl	78(%ebx)
	.cfi_def_cfa_offset 44
	pushl	74(%ebx)
	.cfi_def_cfa_offset 48
	call	get_dir
	addl	$12, %esp
	.cfi_def_cfa_offset 36
	pushl	70(%ebx)
	.cfi_def_cfa_offset 40
	pushl	66(%ebx)
	.cfi_def_cfa_offset 44
	pushl	$.LC376
	.cfi_def_cfa_offset 48
	movl	%eax, %esi
	call	printf
	popl	%edi
	.cfi_def_cfa_offset 44
	popl	%ebp
	.cfi_def_cfa_offset 40
	pushl	70(%ebx)
	.cfi_def_cfa_offset 44
	pushl	66(%ebx)
	.cfi_def_cfa_offset 48
	call	get_dir
	popl	%edx
	.cfi_def_cfa_offset 44
	popl	%ecx
	.cfi_def_cfa_offset 40
	pushl	%eax
	.cfi_def_cfa_offset 44
	pushl	$.LC376
	.cfi_def_cfa_offset 48
	movl	%eax, %edi
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%esi, %esi
	je	.L2777
	movl	66(%ebx), %eax
	movl	70(%ebx), %edx
	movl	%eax, 66(%esi)
	movl	%edx, 70(%esi)
.L2777:
	testl	%edi, %edi
	je	.L2778
	movl	78(%ebx), %edx
	movl	74(%ebx), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	movl	%edx, 78(%edi)
	movl	%eax, 74(%edi)
	pushl	22(%ebx)
	.cfi_def_cfa_offset 44
	pushl	18(%ebx)
	.cfi_def_cfa_offset 48
	call	get_dir
	movl	%eax, %ebp
	subl	$1, 14(%eax)
	movl	30(%ebx), %edx
	movl	26(%ebx), %eax
	xorl	62(%ebp), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	xorl	58(%ebp), %eax
	orl	%eax, %edx
	je	.L2779
	cmpl	$11465840, 6(%edi)
	je	.L2796
.L2780:
	testl	%esi, %esi
	je	.L2781
	cmpl	$11465840, 6(%esi)
	je	.L2797
.L2781:
	cmpl	$11465840, 6(%ebp)
	je	.L2798
.L2782:
	movl	30(%ebx), %edx
	movl	26(%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	movl	curr_port, %ebx
	pushl	$1
	.cfi_def_cfa_offset 48
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	clear_buf512
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L2789
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L2789:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2779:
	.cfi_restore_state
	cmpl	$11465840, 6(%edi)
	movl	74(%ebx), %eax
	movl	78(%ebx), %edx
	movl	%eax, 58(%ebp)
	movl	%edx, 62(%ebp)
	jne	.L2780
.L2796:
	movl	%edi, %eax
	call	flush_dir.part.10
	jmp	.L2780
	.align 16
.L2798:
	movl	%ebp, %eax
	call	flush_dir.part.10
	jmp	.L2782
	.align 16
.L2778:
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	22(%ebx)
	.cfi_def_cfa_offset 44
	pushl	18(%ebx)
	.cfi_def_cfa_offset 48
	call	get_dir
	movl	%eax, %ebp
	subl	$1, 14(%eax)
	movl	30(%ebx), %edx
	movl	26(%ebx), %eax
	xorl	62(%ebp), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	xorl	58(%ebp), %eax
	orl	%eax, %edx
	jne	.L2780
	movl	74(%ebx), %eax
	movl	78(%ebx), %edx
	movl	%eax, 58(%ebp)
	movl	%edx, 62(%ebp)
	jmp	.L2780
	.align 16
.L2797:
	movl	%esi, %eax
	call	flush_dir.part.10
	jmp	.L2781
	.cfi_endproc
.LFE348:
	.size	delete_dir, .-delete_dir
	.align 16
	.globl	delete_file
	.type	delete_file, @function
delete_file:
.LFB349:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$20, %esp
	.cfi_def_cfa_offset 40
	movl	40(%esp), %ebx
	pushl	62(%ebx)
	.cfi_def_cfa_offset 44
	pushl	58(%ebx)
	.cfi_def_cfa_offset 48
	call	get_file
	movl	%eax, %esi
	popl	%eax
	.cfi_def_cfa_offset 44
	popl	%edx
	.cfi_def_cfa_offset 40
	pushl	54(%ebx)
	.cfi_def_cfa_offset 44
	pushl	50(%ebx)
	.cfi_def_cfa_offset 48
	call	get_file
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%esi, %esi
	movl	%eax, %edi
	je	.L2800
	movl	50(%ebx), %eax
	movl	54(%ebx), %edx
	movl	%eax, 50(%esi)
	movl	%edx, 54(%esi)
.L2800:
	testl	%edi, %edi
	je	.L2801
	movl	62(%ebx), %edx
	movl	58(%ebx), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	movl	%edx, 62(%edi)
	movl	%eax, 58(%edi)
	pushl	22(%ebx)
	.cfi_def_cfa_offset 44
	pushl	18(%ebx)
	.cfi_def_cfa_offset 48
	call	get_dir
	movl	%eax, %ebp
	subl	$1, 10(%eax)
	movl	30(%ebx), %edx
	movl	26(%ebx), %eax
	xorl	54(%ebp), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	xorl	50(%ebp), %eax
	orl	%eax, %edx
	je	.L2820
.L2816:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	addl	$66, %ebx
	pushl	%ebx
	.cfi_def_cfa_offset 48
	call	file_close
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	cmpl	$11465841, 6(%edi)
	je	.L2821
.L2804:
	testl	%esi, %esi
	je	.L2805
	cmpl	$11465841, 6(%esi)
	je	.L2822
.L2805:
	cmpl	$11465840, 6(%ebp)
	je	.L2823
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2820:
	.cfi_restore_state
	movl	58(%ebx), %eax
	movl	62(%ebx), %edx
	movl	%eax, 50(%ebp)
	movl	%edx, 54(%ebp)
	jmp	.L2816
	.align 16
.L2823:
	movl	%ebp, %eax
	call	flush_dir.part.10
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2801:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	22(%ebx)
	.cfi_def_cfa_offset 44
	pushl	18(%ebx)
	.cfi_def_cfa_offset 48
	call	get_dir
	movl	%eax, %ebp
	subl	$1, 10(%eax)
	movl	30(%ebx), %edx
	movl	26(%ebx), %eax
	xorl	54(%ebp), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	xorl	50(%ebp), %eax
	orl	%eax, %edx
	je	.L2824
.L2817:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	addl	$66, %ebx
	pushl	%ebx
	.cfi_def_cfa_offset 48
	call	file_close
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L2804
	.align 16
.L2821:
	movl	%edi, %eax
	call	flush_file.part.11
	jmp	.L2804
	.align 16
.L2822:
	movl	%esi, %eax
	call	flush_file.part.11
	jmp	.L2805
.L2824:
	movl	58(%ebx), %eax
	movl	62(%ebx), %edx
	movl	%eax, 50(%ebp)
	movl	%edx, 54(%ebp)
	jmp	.L2817
	.cfi_endproc
.LFE349:
	.size	delete_file, .-delete_file
	.align 16
	.globl	nx_header
	.type	nx_header, @function
nx_header:
.LFB350:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$4, %esp
	.cfi_def_cfa_offset 16
	movl	16(%esp), %ebx
	testl	%ebx, %ebx
	je	.L2826
	movl	36(%ebx), %eax
	orl	32(%ebx), %eax
	je	.L2826
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$512
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	36(%ebx), %edx
	movl	%eax, %esi
	movl	32(%ebx), %eax
	movl	curr_port, %ebx
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 36
	pushl	%eax
	.cfi_def_cfa_offset 40
	pushl	$512
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%esi
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$32
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	jne	.L2839
	cmpl	$11527282, 12(%esi)
	jne	.L2826
.L2825:
	addl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	movl	%esi, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2839:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC214
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	cmpl	$11527282, 12(%esi)
	je	.L2825
.L2826:
	xorl	%esi, %esi
	addl	$4, %esp
	.cfi_def_cfa_offset 12
	movl	%esi, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE350:
	.size	nx_header, .-nx_header
	.align 16
	.globl	get_header
	.type	get_header, @function
get_header:
.LFB351:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	movl	36(%esp), %edi
	movl	32(%esp), %esi
	movl	%edi, %edx
	orl	%esi, %edx
	je	.L2841
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$512
	.cfi_def_cfa_offset 48
	call	*kmalloc
	movl	curr_port, %ebp
	movl	%eax, %ebx
	movl	%edi, %eax
	shrdl	$9, %edi, %esi
	shrl	$9, %eax
	movl	$1, (%esp)
	pushl	%eax
	.cfi_def_cfa_offset 52
	pushl	%esi
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ebx
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%ebp
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	jne	.L2851
	cmpl	$11527282, 12(%ebx)
	jne	.L2841
.L2840:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2851:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	cmpl	$11527282, 12(%ebx)
	je	.L2840
.L2841:
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	xorl	%ebx, %ebx
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE351:
	.size	get_header, .-get_header
	.align 16
	.globl	del_header
	.type	del_header, @function
del_header:
.LFB352:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$20, %esp
	.cfi_def_cfa_offset 40
	movl	40(%esp), %ebx
	pushl	44(%ebx)
	.cfi_def_cfa_offset 44
	pushl	40(%ebx)
	.cfi_def_cfa_offset 48
	call	get_header
	popl	%edi
	.cfi_def_cfa_offset 44
	popl	%ebp
	.cfi_def_cfa_offset 40
	pushl	36(%ebx)
	.cfi_def_cfa_offset 44
	pushl	32(%ebx)
	.cfi_def_cfa_offset 48
	movl	%eax, %esi
	call	get_header
	movl	24(%esi), %edi
	movl	28(%eax), %ebx
	movl	24(%eax), %ecx
	movl	28(%esi), %ebp
	movl	%ebx, 36(%esi)
	movl	%ebx, %edx
	movl	%ecx, 32(%esi)
	movl	%edi, 40(%eax)
	shrl	$9, %edx
	movl	curr_port, %edi
	movl	%ebp, 44(%eax)
	shrdl	$9, %ebx, %ecx
	movl	$1, (%esp)
	pushl	%edx
	.cfi_def_cfa_offset 52
	pushl	%ecx
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%eax
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%edi
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L2853
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L2853:
	movl	28(%esi), %edx
	movl	24(%esi), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	movl	curr_port, %ebx
	pushl	$1
	.cfi_def_cfa_offset 48
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%esi
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	jne	.L2859
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2859:
	.cfi_restore_state
	movl	$.LC215, 32(%esp)
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	printf
	.cfi_endproc
.LFE352:
	.size	del_header, .-del_header
	.align 16
	.globl	File_Header_Creator
	.type	File_Header_Creator, @function
File_Header_Creator:
.LFB353:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	movl	44(%esp), %esi
	movl	48(%esp), %edi
	pushl	$512
	.cfi_def_cfa_offset 48
	call	pmem
	movl	%eax, %ebx
	movl	%eax, fsbuf
	movl	26(%esi), %eax
	movl	30(%esi), %edx
	movzwl	%di, %edi
	movl	$0, 32(%ebx)
	movl	$0, 36(%ebx)
	movl	$0, (%ebx)
	movl	%eax, 16(%ebx)
	leal	2(%edi), %eax
	sall	$9, %edi
	movl	%edx, 20(%ebx)
	movl	%eax, (%esp)
	call	sec_alloc
	movl	42(%esi), %ecx
	movl	%edi, 4(%ebx)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	46(%esi), %edi
	movl	%eax, 24(%ebx)
	movl	%edx, 28(%ebx)
	movl	$11527282, 12(%ebx)
	movl	%ecx, 40(%ebx)
	movl	%edi, 44(%ebx)
	orl	%ecx, %edi
	jne	.L2873
	movl	%eax, 34(%esi)
	movl	%edx, 38(%esi)
.L2864:
	movl	curr_port, %edi
	movl	%edx, %ecx
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	shrdl	$9, %edx, %eax
	shrl	$9, %ecx
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%ecx
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	fsbuf
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%edi
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L2865
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L2865:
	movl	24(%ebx), %eax
	movl	28(%ebx), %edx
	addl	$1, 14(%esi)
	addl	$512, 2(%esi)
	movl	%eax, 42(%esi)
	movl	%edx, 46(%esi)
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2873:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$512
	.cfi_def_cfa_offset 48
	call	pmem
	movl	46(%esi), %edx
	movl	%eax, %edi
	movl	42(%esi), %eax
	movl	curr_port, %ebp
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%edi
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%ebp
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	jne	.L2874
.L2862:
	movl	24(%ebx), %eax
	movl	28(%ebx), %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	movl	curr_port, %ebp
	movl	%eax, 32(%edi)
	movl	%edx, 36(%edi)
	movl	42(%esi), %eax
	movl	46(%esi), %edx
	pushl	$1
	.cfi_def_cfa_offset 48
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%edi
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%ebp
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	jne	.L2863
	movl	24(%ebx), %eax
	movl	28(%ebx), %edx
	jmp	.L2864
	.align 16
.L2863:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	movl	24(%ebx), %eax
	movl	28(%ebx), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L2864
	.align 16
.L2874:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L2862
	.cfi_endproc
.LFE353:
	.size	File_Header_Creator, .-File_Header_Creator
	.align 16
	.globl	File_Header_Creator_sdw
	.type	File_Header_Creator_sdw, @function
File_Header_Creator_sdw:
.LFB354:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	movl	44(%esp), %esi
	movl	48(%esp), %edi
	movl	52(%esp), %ebp
	pushl	$512
	.cfi_def_cfa_offset 48
	call	pmem
	movl	%eax, %ebx
	movl	%eax, fsbuf
	movl	26(%esi), %eax
	movl	30(%esi), %edx
	movzwl	%bp, %ebp
	movl	%eax, 16(%ebx)
	movl	32(%edi), %eax
	movl	%edx, 20(%ebx)
	movl	36(%edi), %edx
	movl	$0, (%ebx)
	movl	%eax, 32(%ebx)
	leal	2(%ebp), %eax
	sall	$9, %ebp
	movl	%edx, 36(%ebx)
	movl	%eax, (%esp)
	call	sec_alloc
	movl	%eax, 24(%ebx)
	movl	%edx, 28(%ebx)
	movl	28(%edi), %edx
	movl	24(%edi), %eax
	movl	%ebp, 4(%ebx)
	movl	$11527282, 12(%ebx)
	movl	%edx, 44(%ebx)
	movl	%eax, 40(%ebx)
	movl	$512, (%esp)
	call	pmem
	movl	%eax, %ecx
	movl	24(%ebx), %eax
	movl	28(%ebx), %edx
	movl	curr_port, %ebp
	movl	%eax, 32(%edi)
	movl	28(%edi), %eax
	movl	%edx, 36(%edi)
	movl	$1, (%esp)
	movl	28(%edi), %edx
	shrl	$9, %eax
	pushl	%eax
	.cfi_def_cfa_offset 52
	movl	24(%edi), %eax
	shrdl	$9, %edx, %eax
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%ebp
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L2876
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L2876:
	movl	24(%edi), %eax
	movl	28(%edi), %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	movl	curr_port, %edi
	movl	%eax, 40(%ebx)
	movl	%edx, 44(%ebx)
	movl	24(%ebx), %eax
	movl	28(%ebx), %edx
	pushl	$1
	.cfi_def_cfa_offset 48
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	fsbuf
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%edi
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L2877
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L2877:
	movl	38(%esi), %eax
	orl	34(%esi), %eax
	movl	28(%ebx), %edx
	movl	24(%ebx), %eax
	jne	.L2879
	movl	%eax, 34(%esi)
	movl	%edx, 38(%esi)
.L2879:
	movl	%eax, 42(%esi)
	movl	%edx, 46(%esi)
	movl	%ebx, %eax
	addl	$1, 14(%esi)
	addl	$512, 2(%esi)
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE354:
	.size	File_Header_Creator_sdw, .-File_Header_Creator_sdw
	.align 16
	.globl	file_flushOGP
	.type	file_flushOGP, @function
file_flushOGP:
.LFB355:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	start_handle, %ebx
	movl	16(%esp), %edi
	testl	%ebx, %ebx
	je	.L2887
	movzbl	(%edi), %esi
	.align 16
.L2895:
	movl	4(%ebx), %eax
	movl	%esi, %edx
	cmpb	%dl, (%eax)
	jne	.L2890
	testb	%dl, %dl
	je	.L2891
	movl	%edi, %edx
	jmp	.L2893
	.align 16
.L2894:
	testb	%cl, %cl
	je	.L2891
.L2893:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L2894
.L2890:
	movl	28(%ebx), %ebx
	testl	%ebx, %ebx
	jne	.L2895
.L2887:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2891:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$512
	.cfi_def_cfa_offset 32
	call	pmem
	movl	8(%ebx), %ebx
	movl	%eax, %esi
	movl	curr_port, %edi
	movl	30(%ebx), %edx
	movl	26(%ebx), %eax
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 36
	pushl	%eax
	.cfi_def_cfa_offset 40
	pushl	$512
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%esi
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$32
	.cfi_def_cfa_offset 60
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	jne	.L2914
.L2897:
	movl	26(%ebx), %ecx
	leal	162(%ebx), %edi
	movl	%ebx, %eax
	andl	$511, %ecx
	addl	%esi, %ecx
	subl	%ebx, %ecx
	.align 16
.L2898:
	addl	$1, %eax
	movzbl	-1(%eax), %edx
	cmpl	%eax, %edi
	movb	%dl, -1(%eax,%ecx)
	jne	.L2898
	movl	30(%ebx), %edx
	movl	26(%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	curr_port, %ebx
	pushl	$1
	.cfi_def_cfa_offset 32
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 36
	pushl	%eax
	.cfi_def_cfa_offset 40
	pushl	$512
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%esi
	.cfi_def_cfa_offset 52
	pushl	$1
	.cfi_def_cfa_offset 56
	pushl	$48
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	je	.L2887
	movl	$.LC215, 16(%esp)
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	jmp	printf
.L2914:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC214
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L2897
	.cfi_endproc
.LFE355:
	.size	file_flushOGP, .-file_flushOGP
	.align 16
	.globl	file_flush
	.type	file_flush, @function
file_flush:
.LFB356:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	start_handle, %ebx
	movl	16(%esp), %edi
	testl	%ebx, %ebx
	je	.L2915
	movzbl	(%edi), %esi
	.align 16
.L2923:
	movl	(%ebx), %eax
	movl	%esi, %edx
	cmpb	%dl, (%eax)
	jne	.L2918
	testb	%dl, %dl
	je	.L2919
	movl	%edi, %edx
	jmp	.L2921
	.align 16
.L2922:
	testb	%cl, %cl
	je	.L2919
.L2921:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L2922
.L2918:
	movl	28(%ebx), %ebx
	testl	%ebx, %ebx
	jne	.L2923
.L2915:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2919:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$512
	.cfi_def_cfa_offset 32
	call	pmem
	movl	8(%ebx), %ebx
	movl	%eax, %esi
	movl	curr_port, %edi
	movl	30(%ebx), %edx
	movl	26(%ebx), %eax
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 36
	pushl	%eax
	.cfi_def_cfa_offset 40
	pushl	$512
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%esi
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$32
	.cfi_def_cfa_offset 60
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	jne	.L2942
.L2925:
	movl	26(%ebx), %ecx
	leal	162(%ebx), %edi
	movl	%ebx, %eax
	andl	$511, %ecx
	addl	%esi, %ecx
	subl	%ebx, %ecx
	.align 16
.L2926:
	addl	$1, %eax
	movzbl	-1(%eax), %edx
	cmpl	%eax, %edi
	movb	%dl, -1(%eax,%ecx)
	jne	.L2926
	movl	30(%ebx), %edx
	movl	26(%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	curr_port, %ebx
	pushl	$1
	.cfi_def_cfa_offset 32
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 36
	pushl	%eax
	.cfi_def_cfa_offset 40
	pushl	$512
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%esi
	.cfi_def_cfa_offset 52
	pushl	$1
	.cfi_def_cfa_offset 56
	pushl	$48
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	je	.L2915
	movl	$.LC215, 16(%esp)
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	jmp	printf
.L2942:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC214
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L2925
	.cfi_endproc
.LFE356:
	.size	file_flush, .-file_flush
	.align 16
	.globl	file_truncate
	.type	file_truncate, @function
file_truncate:
.LFB357:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	movl	60(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, 20(%esp)
	pushl	$512
	.cfi_def_cfa_offset 64
	call	pmem
	movl	$512, (%esp)
	movl	%eax, %edi
	call	pmem
	leal	1024(%eax), %edx
	movl	%eax, 28(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	.align 16
.L2944:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %edx
	jne	.L2944
	movl	8(%esp), %ecx
	xorl	%ebp, %ebp
	movl	%edi, %esi
	movl	14(%ecx), %ebx
	movl	38(%ecx), %edx
	movl	34(%ecx), %eax
	testl	%ebx, %ebx
	je	.L2949
	.align 16
.L2954:
	shrdl	$9, %edx, %eax
	movl	curr_port, %edi
	movl	%edx, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	%eax, 16(%esp)
	shrl	$9, %ebx
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ebx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%esi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%edi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2946
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2946:
	movl	curr_port, %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ebx
	.cfi_def_cfa_offset 68
	pushl	24(%esp)
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	44(%esp)
	.cfi_def_cfa_offset 84
	pushl	$1
	.cfi_def_cfa_offset 88
	pushl	$48
	.cfi_def_cfa_offset 92
	pushl	%edi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2947
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC215
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2947:
	movl	8(%esp), %ecx
	addl	$1, %ebp
	movl	32(%esi), %eax
	movl	36(%esi), %edx
	cmpl	%ebp, 14(%ecx)
	ja	.L2954
.L2949:
	movl	8(%esp), %eax
	movl	$0, 2(%eax)
	movl	$0, 34(%eax)
	movl	$0, 38(%eax)
	movl	$0, 14(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE357:
	.size	file_truncate, .-file_truncate
	.align 16
	.globl	file_header_search
	.type	file_header_search, @function
file_header_search:
.LFB358:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	movl	64(%esp), %ebp
	pushl	$512
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%eax, 28(%esp)
	movl	%eax, %edi
	movl	curr_port, %ebx
	movl	38(%ebp), %edx
	movl	34(%ebp), %eax
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2963
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2963:
	movl	14(%ebp), %edi
	testl	%edi, %edi
	jle	.L2967
	movl	12(%esp), %eax
	movl	(%eax), %ebx
	cmpl	%ebx, 48(%esp)
	jnb	.L2985
	jmp	.L2987
	.align 16
.L2988:
	movl	36(%eax), %edx
	movl	32(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	curr_port, %esi
	pushl	$1
	.cfi_def_cfa_offset 64
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	44(%esp)
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2970
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2970:
	movl	34(%ebp), %eax
	subl	$1, %edi
	andl	$511, %eax
	addl	12(%esp), %eax
	addl	(%eax), %ebx
	cmpl	48(%esp), %ebx
	ja	.L2966
.L2985:
	cmpl	$1, %edi
	jne	.L2988
.L2967:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L2987:
	.cfi_restore_state
	movl	12(%esp), %eax
.L2966:
	subl	48(%esp), %ebx
	movl	%ebx, 8(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE358:
	.size	file_header_search, .-file_header_search
	.align 16
	.globl	file_readTM
	.type	file_readTM, @function
file_readTM:
.LFB360:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	start_handle, %ecx
	movl	60(%esp), %esi
	testl	%ecx, %ecx
	je	.L3013
	movzbl	(%esi), %edi
	.align 16
.L2996:
	movl	4(%ecx), %eax
	movl	%edi, %edx
	cmpb	(%eax), %dl
	jne	.L2991
	testb	%dl, %dl
	je	.L2992
	movl	%esi, %edx
	jmp	.L2994
	.align 16
.L2995:
	testb	%bl, %bl
	je	.L2992
.L2994:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ebx
	cmpb	(%edx), %bl
	je	.L2995
.L2991:
	movl	28(%ecx), %ecx
	testl	%ecx, %ecx
	jne	.L2996
.L3013:
	movl	$1, %eax
.L2989:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L2992:
	.cfi_restore_state
	movl	8(%ecx), %ebx
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	64(%esp)
	.cfi_def_cfa_offset 64
	call	file_header_search
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	56(%esp), %edi
	movl	%eax, %ebp
	testl	%edi, %edi
	je	.L3053
.L2997:
	testl	%ebp, %ebp
	movl	$2, %eax
	je	.L2989
	cmpl	$11527282, 12(%ebp)
	movl	$3, %eax
	jne	.L2989
	movl	8(%ebp), %eax
	cmpl	%eax, 56(%esp)
	movl	0(%ebp), %edi
	movl	%eax, %esi
	cmovbe	56(%esp), %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	subl	%eax, %edi
	movl	%edi, 12(%esp)
	leal	1023(%esi), %ebx
	movl	%esi, 16(%esp)
	andl	$-1024, %ebx
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%ebx, %ecx
	movl	%eax, %edi
	xorl	%edx, %edx
	movl	16(%esp), %eax
	shrl	$9, %ecx
	addl	24(%ebp), %eax
	adcl	28(%ebp), %edx
	movl	curr_port, %esi
	movl	%ecx, (%esp)
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	addl	$1, %eax
	movl	%eax, 24(%esp)
	leal	-1(%ecx), %eax
	adcl	$0, %edx
	movl	%edx, 28(%esp)
	pushl	%edx
	.cfi_def_cfa_offset 68
	shrl	$4, %eax
	pushl	28(%esp)
	.cfi_def_cfa_offset 72
	pushl	%ebx
	.cfi_def_cfa_offset 76
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L2998
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L2998:
	movl	(%esp), %eax
	movl	4(%esp), %esi
	movl	48(%esp), %ecx
	andl	$511, %eax
	addl	%eax, %edi
	addl	%esi, %ecx
	testl	%esi, %esi
	movl	48(%esp), %eax
	je	.L3003
.L3035:
	addl	$1, %eax
	movl	%eax, %edx
	subl	48(%esp), %edx
	cmpl	%ecx, %eax
	movzbl	-1(%edx,%edi), %edx
	movb	%dl, -1(%eax)
	jne	.L3035
.L3003:
	movl	56(%esp), %eax
	subl	4(%esp), %eax
	movl	%eax, (%esp)
	je	.L3001
.L3000:
	movl	36(%ebp), %edx
	movl	32(%ebp), %eax
	movl	%edx, %ebx
	orl	%eax, %ebx
	je	.L3054
	movl	curr_port, %ebx
	movl	%edx, %ecx
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	shrdl	$9, %edx, %eax
	shrl	$9, %ecx
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%ecx
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ebp
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L3006
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L3006:
	cmpl	$11527282, 12(%ebp)
	jne	.L3016
	movl	0(%ebp), %eax
	cmpl	(%esp), %eax
	ja	.L3055
	addl	$511, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	andl	$-512, %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	0(%ebp), %edi
	movl	28(%ebp), %edx
	movl	%eax, %ebx
	movl	24(%ebp), %eax
	movl	curr_port, %esi
	movl	%edi, %ecx
	shrdl	$9, %edx, %eax
	shrl	$9, %ecx
	shrl	$9, %edx
	addl	$1, %ecx
	addl	$1, %eax
	adcl	$0, %edx
	shrl	$13, %edi
	movl	%eax, 24(%esp)
	movzwl	%di, %eax
	movl	%ecx, (%esp)
	sall	$9, %ecx
	addl	$1, %eax
	movl	%edx, 28(%esp)
	pushl	%edx
	.cfi_def_cfa_offset 68
	pushl	28(%esp)
	.cfi_def_cfa_offset 72
	pushl	%ecx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	%ebx
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L3010
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L3010:
	movl	0(%ebp), %ecx
	movl	4(%esp), %esi
	addl	48(%esp), %esi
	testl	%ecx, %ecx
	je	.L3000
	addl	%ebx, %ecx
	movl	%ebx, %eax
	movl	%ecx, %edi
	.align 16
.L3012:
	addl	$1, %eax
	movzbl	-1(%eax), %ecx
	movl	%eax, %edx
	subl	%ebx, %edx
	cmpl	%edi, %eax
	movb	%cl, -1(%edx,%esi)
	jne	.L3012
	movl	0(%ebp), %eax
	addl	%eax, 4(%esp)
	subl	%eax, (%esp)
	jne	.L3000
.L3001:
	xorl	%eax, %eax
	jmp	.L2989
	.align 16
.L3053:
	movl	14(%ebx), %eax
	movl	2(%ebx), %edx
	sall	$9, %eax
	subl	%eax, %edx
	movl	%edx, 56(%esp)
	jmp	.L2997
.L3054:
	movl	$4, %eax
	jmp	.L2989
.L3055:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	12(%esp), %edi
	movl	%edi, %eax
	addl	$511, %eax
	andl	$-512, %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	28(%ebp), %ebx
	movl	24(%ebp), %ecx
	movl	%eax, %esi
	movl	%edi, %eax
	movl	%edi, 16(%esp)
	movl	curr_port, %edi
	shrl	$9, %eax
	shrdl	$9, %ebx, %ecx
	addl	$1, %eax
	shrl	$9, %ebx
	addl	$1, %ecx
	movl	%eax, (%esp)
	adcl	$0, %ebx
	sall	$9, %eax
	pushl	%ebx
	.cfi_def_cfa_offset 68
	pushl	%ecx
	.cfi_def_cfa_offset 72
	pushl	%eax
	.cfi_def_cfa_offset 76
	movl	28(%esp), %eax
	shrl	$13, %eax
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	%esi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%edi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L3056
.L3008:
	movl	(%esp), %eax
	movl	4(%esp), %edx
	movl	%esi, %ebx
	addl	48(%esp), %edx
	addl	%esi, %eax
	movl	%eax, %edi
.L3009:
	addl	$1, %ebx
	movzbl	-1(%ebx), %eax
	movl	%ebx, %ecx
	subl	%esi, %ecx
	cmpl	%edi, %ebx
	movb	%al, -1(%ecx,%edx)
	jne	.L3009
	xorl	%eax, %eax
	jmp	.L2989
.L3016:
	movl	$5, %eax
	jmp	.L2989
.L3056:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L3008
	.cfi_endproc
.LFE360:
	.size	file_readTM, .-file_readTM
	.align 16
	.globl	file_size
	.type	file_size, @function
file_size:
.LFB361:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	start_handle, %ebx
	movl	16(%esp), %edi
	testl	%ebx, %ebx
	je	.L3065
	movzbl	(%edi), %esi
	.align 16
.L3064:
	movl	4(%ebx), %eax
	movl	%esi, %edx
	cmpb	(%eax), %dl
	jne	.L3059
	testb	%dl, %dl
	je	.L3060
	movl	%edi, %edx
	jmp	.L3062
	.align 16
.L3063:
	testb	%cl, %cl
	je	.L3060
.L3062:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L3063
.L3059:
	movl	28(%ebx), %ebx
	testl	%ebx, %ebx
	jne	.L3064
.L3065:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	xorl	%eax, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3060:
	.cfi_restore_state
	movl	8(%ebx), %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	movl	14(%eax), %edx
	movl	2(%eax), %eax
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	sall	$9, %edx
	subl	%edx, %eax
	ret
	.cfi_endproc
.LFE361:
	.size	file_size, .-file_size
	.align 16
	.globl	file_writeAppend
	.type	file_writeAppend, @function
file_writeAppend:
.LFB362:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	start_handle, %ecx
	movl	72(%esp), %esi
	testl	%ecx, %ecx
	je	.L3099
	movzbl	(%esi), %edi
	.align 16
.L3081:
	movl	4(%ecx), %eax
	movl	%edi, %edx
	cmpb	%dl, (%eax)
	jne	.L3076
	testb	%dl, %dl
	je	.L3077
	movl	%esi, %edx
	jmp	.L3079
	.align 16
.L3080:
	testb	%bl, %bl
	je	.L3077
.L3079:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ebx
	cmpb	(%edx), %bl
	je	.L3080
.L3076:
	movl	28(%ecx), %ecx
	testl	%ecx, %ecx
	jne	.L3081
.L3099:
	movl	$4, %eax
.L3074:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3077:
	.cfi_restore_state
	movl	8(%ecx), %ebx
	movl	68(%esp), %eax
	movl	64(%esp), %edi
	addl	%eax, 2(%ebx)
	movl	46(%ebx), %eax
	orl	42(%ebx), %eax
	jne	.L3142
.L3090:
	movl	68(%esp), %esi
	testl	%esi, %esi
	jne	.L3143
.L3140:
	xorl	%eax, %eax
.L3145:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L3143:
	.cfi_restore_state
	movl	68(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	leal	511(%eax), %esi
	andl	$-512, %esi
	movl	%esi, %ebp
	shrl	$9, %ebp
	movzwl	%bp, %eax
	pushl	%eax
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	File_Header_Creator
	movl	%eax, %ebx
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	$1, %eax
	testl	%ebx, %ebx
	je	.L3074
	movl	28(%ebx), %edx
	movl	24(%ebx), %eax
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	addl	$1, %eax
	adcl	$0, %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	%edx, %ecx
	movl	%eax, %edx
	movl	curr_port, %eax
	movl	%eax, 20(%esp)
	leal	-1(%ebp), %eax
	pushl	%ebp
	.cfi_def_cfa_offset 80
	pushl	%ecx
	.cfi_def_cfa_offset 84
	pushl	%edx
	.cfi_def_cfa_offset 88
	shrl	$4, %eax
	pushl	%esi
	.cfi_def_cfa_offset 92
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 96
	pushl	%edi
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	movl	52(%esp), %esi
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L3093
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3093:
	movl	68(%esp), %esi
	andl	$511, %esi
	jne	.L3144
.L3095:
	movl	68(%esp), %eax
	movl	28(%ebx), %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	curr_port, %esi
	movl	%eax, (%ebx)
	movl	24(%ebx), %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ecx
	.cfi_def_cfa_offset 84
	shrdl	$9, %edx, %eax
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%ebx
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L3140
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	xorl	%eax, %eax
	jmp	.L3145
.L3142:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$512
	.cfi_def_cfa_offset 80
	call	*kmalloc
	movl	46(%ebx), %edx
	movl	%eax, %ebp
	movl	42(%ebx), %eax
	movl	curr_port, %esi
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%ebp
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	jne	.L3146
.L3083:
	cmpl	$11527282, 12(%ebp)
	movl	$3, %eax
	jne	.L3074
	movl	4(%ebp), %eax
	movl	%eax, %esi
	subl	0(%ebp), %esi
	testl	%esi, %esi
	jle	.L3090
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	*kmalloc
	movl	0(%ebp), %ecx
	movl	%eax, %edi
	movl	28(%ebp), %edx
	movl	%ecx, %eax
	shrl	$9, %eax
	addl	$1, %eax
	movl	%eax, 40(%esp)
	movl	24(%ebp), %eax
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	addl	$1, %eax
	adcl	$0, %edx
	movl	%eax, 24(%esp)
	movl	40(%esp), %eax
	movl	%edx, 28(%esp)
	movl	curr_port, %edx
	movl	%eax, (%esp)
	sall	$9, %eax
	movl	%edx, 32(%esp)
	movl	28(%esp), %edx
	pushl	%edx
	.cfi_def_cfa_offset 84
	pushl	28(%esp)
	.cfi_def_cfa_offset 88
	pushl	%eax
	.cfi_def_cfa_offset 92
	movl	%ecx, %eax
	shrl	$13, %eax
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 96
	pushl	%edi
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	60(%esp)
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	64(%esp), %edx
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	movl	16(%edx), %eax
	testl	$1073741824, %eax
	je	.L3084
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3084:
	cmpl	%esi, 68(%esp)
	movl	0(%ebp), %ecx
	cmovbe	68(%esp), %esi
	movl	64(%esp), %eax
	addl	%edi, %ecx
	addl	%esi, %eax
	testl	%esi, %esi
	movl	%eax, 8(%esp)
	movl	64(%esp), %eax
	je	.L3147
	movl	%ebx, 16(%esp)
.L3122:
	addl	$1, %eax
	movzbl	-1(%eax), %ebx
	movl	%eax, %edx
	subl	64(%esp), %edx
	cmpl	8(%esp), %eax
	movb	%bl, -1(%edx,%ecx)
	jne	.L3122
	movl	16(%esp), %ebx
.L3089:
	movl	4(%ebp), %eax
	movl	28(%ebp), %edx
	movl	%eax, 24(%esp)
	movl	%eax, %ecx
	movl	24(%ebp), %eax
	shrl	$9, %ecx
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	addl	$1, %eax
	adcl	$0, %edx
	movl	%eax, 16(%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	%edx, 32(%esp)
	movl	curr_port, %edx
	movl	%edx, 40(%esp)
	pushl	%ecx
	.cfi_def_cfa_offset 80
	movl	36(%esp), %edx
	pushl	%edx
	.cfi_def_cfa_offset 84
	pushl	36(%esp)
	.cfi_def_cfa_offset 88
	movl	48(%esp), %eax
	andl	$-512, %eax
	pushl	%eax
	.cfi_def_cfa_offset 92
	leal	-1(%ecx), %eax
	shrl	$4, %eax
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 96
	pushl	%edi
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	movl	72(%esp), %edi
	pushl	%edi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L3087
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3087:
	movl	28(%ebp), %edx
	movl	24(%ebp), %eax
	addl	%esi, 0(%ebp)
	subl	%esi, 68(%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	curr_port, %esi
	movl	20(%esp), %edi
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ecx
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%ebp
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L3090
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L3090
.L3146:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L3083
.L3144:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$512
	.cfi_def_cfa_offset 80
	call	*kmalloc
	leal	512(%eax), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	%eax, %edx
	.align 16
.L3096:
	addl	$1, %edx
	movb	$0, -1(%edx)
	cmpl	%ecx, %edx
	jne	.L3096
	movl	68(%esp), %edx
	addl	%eax, %esi
	andl	$-512, %edx
	addl	%edx, %edi
	movl	%eax, %edx
.L3097:
	addl	$1, %edx
	movl	%edx, %ecx
	subl	%eax, %ecx
	cmpl	%esi, %edx
	movzbl	-1(%ecx,%edi), %ecx
	movb	%cl, -1(%edx)
	jne	.L3097
	movl	28(%ebx), %ecx
	movl	24(%ebx), %edx
	movl	curr_port, %esi
	shrdl	$9, %ecx, %edx
	shrl	$9, %ecx
	addl	$1, %edx
	adcl	$0, %ecx
	xorl	%edi, %edi
	addl	%ebp, %edx
	adcl	%edi, %ecx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ecx
	.cfi_def_cfa_offset 84
	pushl	%edx
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%eax
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L3095
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L3095
.L3147:
	movl	%eax, 8(%esp)
	jmp	.L3089
	.cfi_endproc
.LFE362:
	.size	file_writeAppend, .-file_writeAppend
	.align 16
	.globl	file_editFM
	.type	file_editFM, @function
file_editFM:
.LFB363:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$76, %esp
	.cfi_def_cfa_offset 96
	movl	start_handle, %ebx
	movl	104(%esp), %esi
	movl	112(%esp), %ebp
	testl	%ebx, %ebx
	je	.L3192
	movzbl	0(%ebp), %edi
	.align 16
.L3155:
	movl	4(%ebx), %eax
	movl	%edi, %edx
	cmpb	(%eax), %dl
	jne	.L3150
	testb	%dl, %dl
	je	.L3151
	movl	%ebp, %edx
	jmp	.L3153
	.align 16
.L3154:
	testb	%cl, %cl
	je	.L3151
.L3153:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L3154
.L3150:
	movl	28(%ebx), %ebx
	testl	%ebx, %ebx
	jne	.L3155
.L3192:
	xorl	%eax, %eax
.L3148:
	addl	$76, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3151:
	.cfi_restore_state
	movl	8(%ebx), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 104
	movl	%eax, 24(%esp)
	pushl	%eax
	.cfi_def_cfa_offset 108
	pushl	108(%esp)
	.cfi_def_cfa_offset 112
	call	file_header_search
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	testl	%eax, %eax
	movl	%eax, %edi
	je	.L3173
	cmpl	$11527282, 12(%eax)
	jne	.L3193
	movl	(%eax), %eax
	movl	8(%edi), %ebp
	movl	4(%edi), %edx
	movl	%eax, %ebx
	subl	%ebp, %ebx
	subl	%ebx, %edx
	movl	%ebx, (%esp)
	movl	%edx, 12(%esp)
	movl	100(%esp), %edx
	addl	%ebx, %edx
	cmpl	%edx, %eax
	movl	%edx, 24(%esp)
	jnb	.L3259
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	%edi
	.cfi_def_cfa_offset 112
	call	nx_header
	movl	%eax, (%esp)
	movl	%eax, 40(%esp)
	call	nx_header
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	movl	24(%esp), %edx
	movl	%eax, %ebx
	testl	%edx, %edx
	je	.L3173
	addl	(%edx), %ebp
	cmpl	%ebp, 100(%esp)
	jbe	.L3260
	movl	%edi, 24(%esp)
	movl	%ebp, %edi
	movl	%esi, %ebp
	movl	100(%esp), %esi
	jmp	.L3176
	.align 16
.L3241:
	addl	(%ebx), %edi
	movl	%ebx, %edx
	cmpl	%edi, %esi
	jbe	.L3246
	movl	%eax, %ebx
.L3176:
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	%edx
	.cfi_def_cfa_offset 112
	call	del_header
	movl	%ebx, (%esp)
	call	nx_header
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	testl	%ebx, %ebx
	jne	.L3241
.L3173:
	addl	$76, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$-1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L3259:
	.cfi_restore_state
	subl	%edx, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	movl	%eax, 40(%esp)
	addl	120(%esp), %eax
	movl	%eax, 68(%esp)
	addl	$1023, %eax
	andl	$-1024, %eax
	pushl	%eax
	.cfi_def_cfa_offset 112
	call	*kmalloc
	xorl	%edx, %edx
	movl	%eax, %ebp
	movl	%ebx, %eax
	addl	24(%edi), %eax
	movl	%edx, 52(%esp)
	adcl	28(%edi), %edx
	movl	%ebx, 48(%esp)
	movl	curr_port, %ebx
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	addl	$1, %eax
	adcl	$0, %edx
	movl	%eax, 56(%esp)
	movl	%edx, 60(%esp)
	pushl	%edx
	.cfi_def_cfa_offset 116
	pushl	60(%esp)
	.cfi_def_cfa_offset 120
	pushl	$512
	.cfi_def_cfa_offset 124
	pushl	$1
	.cfi_def_cfa_offset 128
	pushl	%ebp
	.cfi_def_cfa_offset 132
	pushl	$0
	.cfi_def_cfa_offset 136
	pushl	$32
	.cfi_def_cfa_offset 140
	pushl	%ebx
	.cfi_def_cfa_offset 144
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 96
	testl	$1073741824, %eax
	je	.L3158
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$.LC214
	.cfi_def_cfa_offset 112
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 96
.L3158:
	movl	(%esp), %ebx
	movl	108(%esp), %ecx
	movl	%esi, %eax
	movl	108(%esp), %edx
	andl	$511, %ebx
	addl	%esi, %ecx
	addl	%ebp, %ebx
	testl	%edx, %edx
	je	.L3163
	movl	%ecx, (%esp)
	.align 16
.L3221:
	addl	$1, %eax
	movzbl	-1(%eax), %ecx
	movl	%eax, %edx
	subl	%esi, %edx
	cmpl	(%esp), %eax
	movb	%cl, -1(%edx,%ebx)
	jne	.L3221
.L3163:
	movl	28(%esp), %esi
	testl	%esi, %esi
	jne	.L3261
.L3161:
	movl	12(%esp), %eax
	movl	36(%esp), %edx
	movl	curr_port, %ebx
	leal	511(%eax), %ecx
	movl	32(%esp), %eax
	andl	$-512, %ecx
	movl	%ecx, %esi
	shrl	$9, %esi
	addl	24(%edi), %eax
	adcl	28(%edi), %edx
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	addl	$1, %eax
	adcl	$0, %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	movl	%eax, 16(%esp)
	leal	-1(%esi), %eax
	movl	%edx, 20(%esp)
	pushl	%edx
	.cfi_def_cfa_offset 116
	pushl	20(%esp)
	.cfi_def_cfa_offset 120
	shrl	$4, %eax
	pushl	%ecx
	.cfi_def_cfa_offset 124
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 128
	pushl	%ebp
	.cfi_def_cfa_offset 132
	pushl	$1
	.cfi_def_cfa_offset 136
	pushl	$48
	.cfi_def_cfa_offset 140
	pushl	%ebx
	.cfi_def_cfa_offset 144
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 96
	testl	$1073741824, %eax
	je	.L3165
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$.LC215
	.cfi_def_cfa_offset 112
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 96
.L3165:
	movl	56(%esp), %esi
	cmpl	%esi, 12(%esp)
	jb	.L3262
.L3169:
	movl	28(%edi), %edx
	movl	24(%edi), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	movl	curr_port, %ebx
	pushl	$1
	.cfi_def_cfa_offset 112
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 116
	pushl	%eax
	.cfi_def_cfa_offset 120
	pushl	$512
	.cfi_def_cfa_offset 124
	pushl	$1
	.cfi_def_cfa_offset 128
	pushl	%edi
	.cfi_def_cfa_offset 132
	pushl	$1
	.cfi_def_cfa_offset 136
	pushl	$48
	.cfi_def_cfa_offset 140
	pushl	%ebx
	.cfi_def_cfa_offset 144
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 96
	testl	$1073741824, %eax
	je	.L3249
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$.LC215
	.cfi_def_cfa_offset 112
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 96
.L3249:
	addl	$76, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L3246:
	.cfi_restore_state
	movl	%ebp, %esi
	movl	%edi, %ebp
	movl	24(%esp), %edi
.L3174:
	subl	100(%esp), %ebp
	movl	(%edi), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	movl	%eax, 68(%esp)
	movl	%ebp, 36(%esp)
	addl	120(%esp), %ebp
	leal	1023(%ebp), %eax
	movl	%ebp, 60(%esp)
	andl	$-1024, %eax
	pushl	%eax
	.cfi_def_cfa_offset 112
	call	*kmalloc
	movl	28(%edi), %edx
	movl	%eax, 44(%esp)
	movl	%eax, %ecx
	movl	24(%edi), %eax
	movl	curr_port, %ebp
	movl	$1, (%esp)
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	addl	$1, %eax
	adcl	$0, %edx
	movl	%eax, 48(%esp)
	movl	%edx, 52(%esp)
	pushl	%edx
	.cfi_def_cfa_offset 116
	pushl	52(%esp)
	.cfi_def_cfa_offset 120
	pushl	$512
	.cfi_def_cfa_offset 124
	pushl	$1
	.cfi_def_cfa_offset 128
	pushl	%ecx
	.cfi_def_cfa_offset 132
	pushl	$0
	.cfi_def_cfa_offset 136
	pushl	$32
	.cfi_def_cfa_offset 140
	pushl	%ebp
	.cfi_def_cfa_offset 144
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 96
	testl	$1073741824, %eax
	je	.L3178
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$.LC214
	.cfi_def_cfa_offset 112
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 96
.L3178:
	movl	(%esp), %ebp
	movl	108(%esp), %ecx
	movl	%esi, %eax
	movl	108(%esp), %edx
	andl	$511, %ebp
	addl	%esi, %ecx
	addl	28(%esp), %ebp
	testl	%edx, %edx
	je	.L3183
	movl	%ebx, 32(%esp)
	.align 16
.L3222:
	addl	$1, %eax
	movzbl	-1(%eax), %ebx
	movl	%eax, %edx
	subl	%esi, %edx
	cmpl	%ecx, %eax
	movb	%bl, -1(%edx,%ebp)
	jne	.L3222
	movl	32(%esp), %ebx
.L3183:
	movl	24(%esp), %eax
	testl	%eax, %eax
	jne	.L3263
.L3181:
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	del_header
	movl	28(%esp), %eax
	xorl	%edx, %edx
	movl	curr_port, %ebx
	leal	511(%eax), %ecx
	movl	16(%esp), %eax
	andl	$-512, %ecx
	movl	%ecx, %esi
	shrl	$9, %esi
	addl	24(%edi), %eax
	adcl	28(%edi), %edx
	movl	%esi, (%esp)
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	addl	$1, %eax
	movl	%eax, 16(%esp)
	leal	-1(%esi), %eax
	adcl	$0, %edx
	movl	%edx, 20(%esp)
	pushl	%edx
	.cfi_def_cfa_offset 116
	shrl	$4, %eax
	pushl	20(%esp)
	.cfi_def_cfa_offset 120
	pushl	%ecx
	.cfi_def_cfa_offset 124
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 128
	pushl	60(%esp)
	.cfi_def_cfa_offset 132
	pushl	$1
	.cfi_def_cfa_offset 136
	pushl	$48
	.cfi_def_cfa_offset 140
	pushl	%ebx
	.cfi_def_cfa_offset 144
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 96
	testl	$1073741824, %eax
	je	.L3186
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$.LC215
	.cfi_def_cfa_offset 112
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 96
.L3186:
	movl	12(%esp), %eax
	cmpl	%eax, 108(%esp)
	jbe	.L3169
	movl	108(%esp), %ebp
	subl	$4, %esp
	.cfi_def_cfa_offset 100
	subl	%eax, %ebp
	movl	%ebp, %esi
	shrl	$9, %esi
	leal	1(%esi), %eax
	movzwl	%ax, %eax
	pushl	%eax
	.cfi_def_cfa_offset 104
	pushl	%edi
	.cfi_def_cfa_offset 108
	pushl	28(%esp)
	.cfi_def_cfa_offset 112
	call	File_Header_Creator_sdw
	movl	%eax, %ebx
	movl	%ebp, (%eax)
	movl	24(%eax), %eax
	movl	28(%ebx), %edx
	leal	1(%esi), %ecx
	movl	curr_port, %esi
	movl	%ecx, (%esp)
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	addl	$1, %eax
	adcl	$0, %edx
	shrl	$13, %ebp
	movl	%eax, 16(%esp)
	movzwl	%bp, %eax
	sall	$9, %ecx
	movl	%edx, 20(%esp)
	addl	$1, %eax
	pushl	%edx
	.cfi_def_cfa_offset 116
	pushl	20(%esp)
	.cfi_def_cfa_offset 120
	pushl	%ecx
	.cfi_def_cfa_offset 124
	pushl	%eax
	.cfi_def_cfa_offset 128
	movl	44(%esp), %eax
	addl	60(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 132
.L3254:
	pushl	$1
	.cfi_def_cfa_offset 136
	pushl	$48
	.cfi_def_cfa_offset 140
	pushl	%esi
	.cfi_def_cfa_offset 144
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 96
	testl	$1073741824, %eax
	je	.L3170
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$.LC215
	.cfi_def_cfa_offset 112
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 96
.L3170:
	movl	28(%ebx), %edx
	movl	24(%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	movl	curr_port, %esi
	pushl	$1
	.cfi_def_cfa_offset 112
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 116
	pushl	%eax
	.cfi_def_cfa_offset 120
	pushl	$512
	.cfi_def_cfa_offset 124
	pushl	$1
	.cfi_def_cfa_offset 128
	pushl	%ebx
	.cfi_def_cfa_offset 132
	pushl	$1
	.cfi_def_cfa_offset 136
	pushl	$48
	.cfi_def_cfa_offset 140
	pushl	%esi
	.cfi_def_cfa_offset 144
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 96
	testl	$1073741824, %eax
	je	.L3169
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$.LC215
	.cfi_def_cfa_offset 112
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	jmp	.L3169
.L3262:
	movl	%esi, %eax
	subl	12(%esp), %eax
	subl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 100
	movl	%eax, %esi
	movl	%eax, 4(%esp)
	shrl	$9, %esi
	leal	1(%esi), %eax
	movzwl	%ax, %eax
	pushl	%eax
	.cfi_def_cfa_offset 104
	pushl	%edi
	.cfi_def_cfa_offset 108
	pushl	28(%esp)
	.cfi_def_cfa_offset 112
	call	File_Header_Creator_sdw
	movl	%eax, %ebx
	movl	16(%esp), %eax
	leal	1(%esi), %ecx
	movl	28(%ebx), %edx
	movl	curr_port, %esi
	movl	%eax, (%ebx)
	movl	24(%ebx), %eax
	movl	%ecx, (%esp)
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	addl	$1, %eax
	adcl	$0, %edx
	sall	$9, %ecx
	movl	%eax, 32(%esp)
	movl	%edx, 36(%esp)
	pushl	%edx
	.cfi_def_cfa_offset 116
	pushl	36(%esp)
	.cfi_def_cfa_offset 120
	pushl	%ecx
	.cfi_def_cfa_offset 124
	movl	28(%esp), %eax
	shrl	$13, %eax
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 128
	addl	44(%esp), %ebp
	pushl	%ebp
	.cfi_def_cfa_offset 132
	jmp	.L3254
.L3261:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	movl	40(%esp), %esi
	movl	%esi, %eax
	addl	$1023, %eax
	andl	$-1024, %eax
	pushl	%eax
	.cfi_def_cfa_offset 112
	call	*kmalloc
	movl	%eax, 16(%esp)
	movl	%esi, %eax
	xorl	%edx, %edx
	shrl	$9, %eax
	movl	curr_port, %esi
	movl	%eax, 56(%esp)
	leal	2(%eax), %ecx
	movl	40(%esp), %eax
	addl	24(%edi), %eax
	adcl	28(%edi), %edx
	movl	%ecx, (%esp)
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	addl	$1, %eax
	adcl	$0, %edx
	sall	$9, %ecx
	movl	%eax, 64(%esp)
	movl	%edx, 68(%esp)
	pushl	%edx
	.cfi_def_cfa_offset 116
	pushl	68(%esp)
	.cfi_def_cfa_offset 120
	pushl	%ecx
	.cfi_def_cfa_offset 124
	movl	68(%esp), %eax
	addl	$1, %eax
	shrl	$4, %eax
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 128
	pushl	32(%esp)
	.cfi_def_cfa_offset 132
	pushl	$0
	.cfi_def_cfa_offset 136
	pushl	$32
	.cfi_def_cfa_offset 140
	pushl	%esi
	.cfi_def_cfa_offset 144
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 96
	testl	$1073741824, %eax
	je	.L3166
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$.LC214
	.cfi_def_cfa_offset 112
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 96
.L3166:
	movl	24(%esp), %eax
	addl	108(%esp), %ebx
	movl	28(%esp), %edx
	andl	$511, %eax
	addl	(%esp), %eax
	addl	%eax, %edx
	subl	%eax, %ebx
.L3167:
	addl	$1, %eax
	movzbl	-1(%eax), %ecx
	cmpl	%edx, %eax
	movb	%cl, -1(%eax,%ebx)
	jne	.L3167
	jmp	.L3161
.L3193:
	movl	$-2, %eax
	jmp	.L3148
.L3263:
	movl	24(%esp), %eax
	movl	56(%esp), %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	subl	%eax, %esi
	movl	%esi, 68(%esp)
	movl	%eax, %esi
	addl	$1023, %eax
	andl	$-1024, %eax
	pushl	%eax
	.cfi_def_cfa_offset 112
	call	*kmalloc
	movl	%eax, 48(%esp)
	movl	%esi, %eax
	movl	curr_port, %esi
	shrl	$9, %eax
	leal	2(%eax), %edx
	movl	%eax, 76(%esp)
	movl	72(%esp), %eax
	movl	%edx, 56(%esp)
	xorl	%edx, %edx
	addl	24(%ebx), %eax
	adcl	28(%ebx), %edx
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	movl	%edx, %ecx
	movl	%eax, %edx
	addl	$1, %edx
	popl	%eax
	.cfi_def_cfa_offset 108
	adcl	$0, %ecx
	pushl	52(%esp)
	.cfi_def_cfa_offset 112
	pushl	%ecx
	.cfi_def_cfa_offset 116
	pushl	%edx
	.cfi_def_cfa_offset 120
	movl	64(%esp), %edx
	sall	$9, %edx
	pushl	%edx
	.cfi_def_cfa_offset 124
	movl	88(%esp), %eax
	addl	$1, %eax
	shrl	$4, %eax
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 128
	pushl	64(%esp)
	.cfi_def_cfa_offset 132
	pushl	$0
	.cfi_def_cfa_offset 136
	pushl	$32
	.cfi_def_cfa_offset 140
	pushl	%esi
	.cfi_def_cfa_offset 144
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 96
	testl	$1073741824, %eax
	jne	.L3264
.L3184:
	movl	56(%esp), %eax
	movl	108(%esp), %edx
	andl	$511, %eax
	addl	32(%esp), %eax
	addl	%ebp, %edx
	movl	24(%esp), %ebp
	subl	%eax, %edx
	addl	%eax, %ebp
.L3185:
	addl	$1, %eax
	movzbl	-1(%eax), %ecx
	cmpl	%ebp, %eax
	movb	%cl, -1(%eax,%edx)
	jne	.L3185
	movl	48(%esp), %eax
	movl	%eax, 108(%esp)
	jmp	.L3181
.L3264:
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$.LC214
	.cfi_def_cfa_offset 112
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	jmp	.L3184
.L3260:
	movl	%edx, %ebx
	jmp	.L3174
	.cfi_endproc
.LFE363:
	.size	file_editFM, .-file_editFM
	.section	.rodata
.LC377:
	.string	"AqFS472"
	.text
	.align 16
	.globl	make_boot_sector
	.type	make_boot_sector, @function
make_boot_sector:
.LFB364:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$1024
	.cfi_def_cfa_offset 32
	call	pmem
	movl	%eax, %esi
	leal	1024(%eax), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%esi, %ebx
	.align 16
.L3266:
	addl	$1, %ebx
	movb	$0, -1(%ebx)
	cmpl	%ebx, %eax
	jne	.L3266
	movl	curr_port, %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$2
	.cfi_def_cfa_offset 32
	pushl	$0
	.cfi_def_cfa_offset 36
	pushl	$0
	.cfi_def_cfa_offset 40
	pushl	$1024
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%esi
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$32
	.cfi_def_cfa_offset 60
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	je	.L3267
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC214
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L3267:
	leal	436(%esi), %edi
	movl	$65, %ecx
	movl	$.LC377, %edx
	movl	%edi, %eax
	.align 16
.L3268:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L3268
	movb	$0, (%eax)
	movl	$446, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movw	%ax, 8(%edi)
	movl	curr_port, %edi
	movb	$85, 510(%esi)
	movb	$-86, 511(%esi)
	pushl	$2
	.cfi_def_cfa_offset 32
	pushl	$0
	.cfi_def_cfa_offset 36
	pushl	$0
	.cfi_def_cfa_offset 40
	pushl	$1024
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%esi
	.cfi_def_cfa_offset 52
	pushl	$1
	.cfi_def_cfa_offset 56
	pushl	$48
	.cfi_def_cfa_offset 60
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	jne	.L3285
.L3269:
	movl	%esi, %eax
	.align 16
.L3270:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%ebx, %eax
	jne	.L3270
	movl	root_location, %eax
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	%eax, (%esi)
	movl	start_off, %eax
	pushl	$2
	.cfi_def_cfa_offset 32
	pushl	$0
	.cfi_def_cfa_offset 36
	subl	$2, %eax
	pushl	%eax
	.cfi_def_cfa_offset 40
	pushl	$1024
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%esi
	.cfi_def_cfa_offset 52
	pushl	$1
	.cfi_def_cfa_offset 56
	pushl	$48
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	je	.L3265
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC215
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L3265:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
.L3285:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC215
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L3269
	.cfi_endproc
.LFE364:
	.size	make_boot_sector, .-make_boot_sector
	.section	.rodata
.LC378:
	.string	"no_disk"
.LC379:
	.string	"\nNo DISK Found"
.LC380:
	.string	"\nTotal Sectors: %x\n"
	.align 4
.LC381:
	.string	"Filesystem Not supported/Disk not partitioned Correctly\n"
.LC382:
	.string	"\n %s %x"
	.text
	.align 16
	.globl	Init_fs
	.type	Init_fs, @function
Init_fs:
.LFB365:
	.cfi_startproc
	movl	ahci_start, %edx
	movl	abar, %eax
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	leal	24(%edx), %ecx
	addl	$384, %eax
	movl	%edx, curr_ahci
	movl	%eax, curr_port
	testl	%ecx, %ecx
	movl	%ecx, curr_disk
	je	.L3287
	testl	%eax, %eax
	je	.L3287
	movl	sata, %esi
	testl	%esi, %esi
	jne	.L3288
.L3287:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	$110, %ebx
	pushl	$.LC379
	.cfi_def_cfa_offset 32
	call	printf
	movl	$4096, (%esp)
	call	pmem
	leal	82(%eax), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$.LC378, %ecx
	.align 16
.L3289:
	addl	$1, %edx
	addl	$1, %ecx
	movb	%bl, -1(%edx)
	movzbl	(%ecx), %ebx
	testb	%bl, %bl
	jne	.L3289
	movb	$0, (%edx)
	movl	root_location, %ecx
	xorl	%edx, %edx
	movl	root_location+4, %ebx
	movw	%dx, (%eax)
	movl	$.LC378, %edx
	movl	$0, 10(%eax)
	movl	$0, 14(%eax)
	movl	%ecx, 26(%eax)
	movl	$1, %ecx
	movl	$0, 66(%eax)
	movl	%ebx, 30(%eax)
	movl	$2, %ebx
	movl	$0, 70(%eax)
	movl	$0, 58(%eax)
	movl	$0, 62(%eax)
	movl	$0, 34(%eax)
	movl	$0, 38(%eax)
	movl	$0, 42(%eax)
	movl	$0, 46(%eax)
	movl	$0, 50(%eax)
	movl	$0, 54(%eax)
	movl	%eax, curr_dir
	movw	%cx, curr_dir+4
	movl	$110, %ecx
	movl	%eax, root_dir
	movw	%bx, root_dir+4
	movl	$curr_dir+6, %eax
	.align 16
.L3290:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L3290
	movb	$0, (%eax)
.L3286:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3288:
	.cfi_restore_state
	movl	32(%edx), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	movl	120(%eax), %eax
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC380
	.cfi_def_cfa_offset 32
	movl	%eax, sectors
	call	printf
	movl	sectors, %eax
	movl	$10240, start_off
	movl	$1024, (%esp)
	movl	%eax, bytes
	movl	bytes, %eax
	shrl	$18, %eax
	movl	%eax, off
	movl	off, %eax
	addl	$16, %eax
	movl	%eax, off
	movl	off, %eax
	addl	$1, %eax
	movl	%eax, off
	movl	off, %eax
	movl	start_off, %edx
	addl	%edx, %eax
	movl	%eax, bytemap_off
	movl	bytes, %eax
	movl	start_off, %edx
	shrl	$9, %eax
	addl	%edx, %eax
	movl	%eax, bytemap_end
	call	pmem
	movl	curr_port, %esi
	movl	$2, (%esp)
	movl	%eax, %ebx
	pushl	$0
	.cfi_def_cfa_offset 36
	pushl	$0
	.cfi_def_cfa_offset 40
	pushl	$1024
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%eax
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$32
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	jne	.L3308
.L3292:
	leal	436(%ebx), %esi
	movl	$7, %ecx
	movl	$.LC377, %edx
	movl	%esi, %eax
	call	strncmp.part.0
	testl	%eax, %eax
	jne	.L3309
	movl	start_off, %eax
	movl	curr_port, %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$2
	.cfi_def_cfa_offset 32
	pushl	$0
	.cfi_def_cfa_offset 36
	subl	$2, %eax
	pushl	%eax
	.cfi_def_cfa_offset 40
	pushl	$1024
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%ebx
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$32
	.cfi_def_cfa_offset 60
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	je	.L3294
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC214
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L3294:
	movl	(%ebx), %eax
	xorl	%edx, %edx
	pushl	%edx
	.cfi_def_cfa_offset 20
	movl	%edx, root_location+4
	pushl	%eax
	.cfi_def_cfa_offset 24
	pushl	%esi
	.cfi_def_cfa_offset 28
	pushl	$.LC382
	.cfi_def_cfa_offset 32
	movl	%eax, root_location
	call	printf
	movl	root_location+4, %edx
	movl	root_location, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%edx, %ebx
	orl	%eax, %ebx
	je	.L3286
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	%edx
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	set_curr_dir
	movl	$0, (%esp)
	call	find_friendDirs
	movl	$0, (%esp)
	call	find_childFiles
	movl	$512, (%esp)
	call	*kmalloc
	leal	512(%eax), %edx
	movl	%eax, clear_buf512
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	.align 16
.L3296:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L3296
	jmp	.L3286
	.align 16
.L3309:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC381
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L3286
	.align 16
.L3308:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC214
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L3292
	.cfi_endproc
.LFE365:
	.size	Init_fs, .-Init_fs
	.section	.rodata
	.align 4
.LC383:
	.string	"\nFormating and Partitioning the Disk, May take a few minutes...\n"
	.align 4
.LC384:
	.string	"\nFile System formated successfully"
	.align 4
.LC385:
	.string	"\nMaster Partition made successfully"
.LC386:
	.string	"\nLoaded the root directory"
.LC387:
	.string	"Aqeous"
.LC388:
	.string	"Programs"
.LC389:
	.string	"user"
.LC390:
	.string	"System"
.LC391:
	.string	"test.txt"
	.text
	.align 16
	.globl	AqFS_burn
	.type	AqFS_burn, @function
AqFS_burn:
.LFB366:
	.cfi_startproc
	subl	$20, %esp
	.cfi_def_cfa_offset 24
	movl	ahci_start, %eax
	movl	abar, %ecx
	leal	24(%eax), %edx
	movl	%eax, curr_ahci
	movl	32(%eax), %eax
	movl	%edx, curr_disk
	leal	384(%ecx), %edx
	movl	120(%eax), %eax
	movl	%edx, curr_port
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC380
	.cfi_def_cfa_offset 32
	movl	%eax, sectors
	call	printf
	movl	sectors, %eax
	movl	$10240, start_off
	movl	$.LC383, (%esp)
	movl	$0, start_handle
	movl	%eax, bytes
	movl	bytes, %eax
	shrl	$18, %eax
	movl	%eax, off
	movl	off, %eax
	addl	$16, %eax
	movl	%eax, off
	movl	off, %eax
	addl	$1, %eax
	movl	%eax, off
	movl	off, %eax
	movl	start_off, %edx
	addl	%edx, %eax
	movl	%eax, bytemap_off
	movl	bytes, %eax
	movl	start_off, %edx
	shrl	$9, %eax
	addl	%edx, %eax
	movl	%eax, bytemap_end
	call	printf
	call	fs_alloc_init
	movl	$.LC384, (%esp)
	call	printf
	movl	$1, (%esp)
	call	sec_alloc
	movl	%edx, root_location+4
	movl	%eax, root_location
	call	make_boot_sector
	movl	$.LC385, (%esp)
	call	printf
	call	Setup_fs
	movl	$.LC386, (%esp)
	call	printf
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	$0
	.cfi_def_cfa_offset 24
	pushl	$1
	.cfi_def_cfa_offset 28
	pushl	$.LC387
	.cfi_def_cfa_offset 32
	call	create_directory
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	$0
	.cfi_def_cfa_offset 24
	pushl	$1
	.cfi_def_cfa_offset 28
	pushl	$.LC388
	.cfi_def_cfa_offset 32
	call	create_directory
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	$0
	.cfi_def_cfa_offset 24
	pushl	$1
	.cfi_def_cfa_offset 28
	pushl	$.LC389
	.cfi_def_cfa_offset 32
	call	create_directory
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	$0
	.cfi_def_cfa_offset 24
	pushl	$1
	.cfi_def_cfa_offset 28
	pushl	$.LC390
	.cfi_def_cfa_offset 32
	call	create_directory
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	$.LC387
	.cfi_def_cfa_offset 24
	pushl	$1
	.cfi_def_cfa_offset 28
	pushl	$.LC390
	.cfi_def_cfa_offset 32
	call	create_directory
	movl	$.LC196, (%esp)
	call	printf
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	$0
	.cfi_def_cfa_offset 24
	pushl	$1
	.cfi_def_cfa_offset 28
	pushl	$.LC391
	.cfi_def_cfa_offset 32
	call	create_file
	movl	$0, (%esp)
	call	find_friendDirs
	movl	$0, (%esp)
	call	find_childFiles
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE366:
	.size	AqFS_burn, .-AqFS_burn
	.align 16
	.globl	ByteSequence_Replace
	.type	ByteSequence_Replace, @function
ByteSequence_Replace:
.LFB367:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$32, %esp
	.cfi_def_cfa_offset 52
	movl	52(%esp), %edx
	movl	56(%esp), %esi
	movl	72(%esp), %eax
	movl	68(%esp), %edi
	movl	64(%esp), %ebp
	movl	%edx, 24(%esp)
	movl	60(%esp), %edx
	movl	%edx, 28(%esp)
	leal	0(,%esi,4), %edx
	movl	%eax, %esi
	subl	%edx, %esi
	cmpl	%esi, %edi
	ja	.L3325
	subl	56(%esp), %eax
	cmpl	%eax, %edi
	ja	.L3336
	movzbl	24(%esp), %ecx
	addl	$1, %eax
	movl	$0, 12(%esp)
	movl	%eax, 4(%esp)
	movb	%cl, 11(%esp)
	.align 16
.L3322:
	movl	56(%esp), %eax
	testl	%eax, %eax
	je	.L3337
	movzbl	11(%esp), %eax
	cmpb	%al, (%edi)
	leal	1(%edi), %eax
	movl	%eax, (%esp)
	jne	.L3316
	movl	56(%esp), %esi
	leal	25(%esp), %edx
	addl	%edi, %esi
	jmp	.L3317
	.align 16
.L3318:
	movzbl	(%eax), %ebx
	movzbl	(%edx), %ecx
	addl	$1, %eax
	addl	$1, %edx
	cmpb	%cl, %bl
	jne	.L3316
.L3317:
	cmpl	%eax, %esi
	jne	.L3318
.L3323:
	xorl	%eax, %eax
	testl	%ebp, %ebp
	je	.L3321
	.align 16
.L3329:
	movzbl	28(%esp,%eax), %edx
	movb	%dl, (%edi,%eax)
	addl	$1, %eax
	cmpl	%ebp, %eax
	jne	.L3329
.L3321:
	addl	$1, 12(%esp)
.L3316:
	movl	(%esp), %edi
	cmpl	4(%esp), %edi
	jne	.L3322
	movl	12(%esp), %eax
.L3312:
	addl	$32, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L3337:
	.cfi_restore_state
	leal	1(%edi), %eax
	movl	%eax, (%esp)
	jmp	.L3323
.L3336:
	xorl	%eax, %eax
	jmp	.L3312
.L3325:
	movl	$-1, %eax
	jmp	.L3312
	.cfi_endproc
.LFE367:
	.size	ByteSequence_Replace, .-ByteSequence_Replace
	.align 16
	.globl	fs_alloc_init
	.type	fs_alloc_init, @function
fs_alloc_init:
.LFB368:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	pushl	$512
	.cfi_def_cfa_offset 48
	call	pmem
	movl	%eax, %esi
	leal	512(%eax), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%esi, %ebx
	.align 16
.L3339:
	addl	$1, %ebx
	movb	$0, -1(%ebx)
	cmpl	%eax, %ebx
	jne	.L3339
	movl	start_off, %edi
	cmpl	bytemap_end, %edi
	ja	.L3344
	.align 16
.L3354:
	movl	curr_port, %ebp
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	$0
	.cfi_def_cfa_offset 52
	pushl	%edi
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%esi
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%ebp
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L3342
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L3342:
	addl	$1, %edi
	cmpl	%edi, bytemap_end
	jnb	.L3354
.L3344:
	movl	%esi, %eax
	.align 16
.L3341:
	addl	$1, %eax
	movb	$-1, -1(%eax)
	cmpl	%eax, %ebx
	jne	.L3341
	movl	start_off, %ebx
	cmpl	bytemap_off, %ebx
	jnb	.L3338
	.align 16
.L3353:
	movl	curr_port, %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	$0
	.cfi_def_cfa_offset 52
	pushl	%ebx
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%esi
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%edi
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L3346
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L3346:
	addl	$1, %ebx
	cmpl	%ebx, bytemap_off
	ja	.L3353
.L3338:
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE368:
	.size	fs_alloc_init, .-fs_alloc_init
	.section	.rodata
	.align 4
.LC392:
	.string	"\nNo Memory left in the Hard Disk!"
	.text
	.align 16
	.globl	fs_alloc
	.type	fs_alloc, @function
fs_alloc:
.LFB369:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$56, %esp
	.cfi_def_cfa_offset 76
	pushl	$512
	.cfi_def_cfa_offset 80
	call	pmem
	movl	%eax, 32(%esp)
	movl	bytemap_off, %eax
	xorl	%edx, %edx
	movl	%edx, 28(%esp)
	movl	%eax, 24(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	cmpl	$0, %edx
	movl	%eax, %ebx
	movl	bytemap_end, %eax
	jbe	.L3390
.L3376:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC392
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3365:
	jmp	.L3365
	.align 16
.L3390:
	cmpl	%eax, %ebx
	ja	.L3376
	movl	16(%esp), %eax
	leal	512(%eax), %esi
	movl	64(%esp), %eax
	leal	-1(%eax), %ebx
.L3385:
	movl	16(%esp), %eax
	.align 16
.L3366:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%esi, %eax
	jne	.L3366
	movl	8(%esp), %eax
	movl	curr_port, %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	%eax, 40(%esp)
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	48(%esp)
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%edi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L3367
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3367:
	movl	16(%esp), %edx
	xorl	%eax, %eax
	movl	%esi, 20(%esp)
	.align 16
.L3368:
	movzbl	(%edx), %ebp
	xorl	%esi, %esi
	movl	%ebp, %ecx
	movzbl	%cl, %edi
	xorl	%ecx, %ecx
	jmp	.L3372
	.align 16
.L3393:
	cmpl	%esi, %ebx
	je	.L3391
	addl	$1, %ecx
	addl	$1, %esi
	cmpl	$8, %ecx
	je	.L3392
.L3372:
	btl	%ecx, %edi
	jnc	.L3393
	addl	$1, %ecx
	xorl	%esi, %esi
	cmpl	$8, %ecx
	jne	.L3372
.L3392:
	addl	$1, %eax
	addl	$1, %edx
	cmpl	$512, %eax
	jne	.L3368
	addl	$1, 8(%esp)
	movl	20(%esp), %esi
	adcl	$0, 12(%esp)
	xorl	%edx, %edx
	movl	bytemap_end, %eax
	movl	12(%esp), %ebp
	movl	8(%esp), %edi
	cmpl	%ebp, %edx
	ja	.L3385
	jb	.L3376
	cmpl	%edi, %eax
	jnb	.L3385
	jmp	.L3376
	.align 16
.L3391:
	movl	%eax, 24(%esp)
	movl	start_off, %esi
	movl	%ebp, %ebx
	movl	8(%esp), %eax
	movl	%edx, 20(%esp)
	xorl	%edi, %edi
	movl	12(%esp), %edx
	movl	64(%esp), %ebp
	subl	%esi, %eax
	sbbl	%edi, %edx
	movl	%eax, %esi
	notl	%ebp
	movl	%edx, %edi
	sall	$12, %esi
	shldl	$12, %eax, %edi
	movl	%esi, %eax
	movl	%ebp, %esi
	addl	%eax, %esi
	movl	%edi, %edx
	movl	$-1, %edi
	adcl	%edx, %edi
	movl	%esi, %eax
	movl	%edi, %edx
	movl	24(%esp), %edi
	leal	(%ecx,%edi,8), %esi
	xorl	%edi, %edi
	addl	%eax, %esi
	movl	$1, %eax
	adcl	%edx, %edi
	movl	20(%esp), %edx
	sall	%cl, %eax
	orl	%ebx, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movb	%al, (%edx)
	movl	curr_port, %ebx
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	48(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	48(%esp)
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L3371
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3371:
	addl	$44, %esp
	.cfi_def_cfa_offset 20
	movl	%esi, %eax
	movl	%edi, %edx
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	shldl	$6, %esi, %edx
	sall	$6, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE369:
	.size	fs_alloc, .-fs_alloc
	.section	.rodata
.LC393:
	.string	"Aa%d\n"
	.text
	.align 16
	.globl	sec_alloc
	.type	sec_alloc, @function
sec_alloc:
.LFB370:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$52, %esp
	.cfi_def_cfa_offset 72
	movl	72(%esp), %edx
	pushl	%edx
	.cfi_def_cfa_offset 76
	movl	%edx, 28(%esp)
	pushl	$.LC393
	.cfi_def_cfa_offset 80
	call	printf
	movl	$512, (%esp)
	call	pmem
	movl	%eax, 28(%esp)
	movl	bytemap_off, %eax
	xorl	%edx, %edx
	movl	%edx, 20(%esp)
	movl	bytemap_end, %ecx
	movl	%eax, 16(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	cmpl	$0, %edx
	jbe	.L3441
.L3415:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC392
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3397:
	jmp	.L3397
	.align 16
.L3441:
	cmpl	%ecx, %eax
	movl	16(%esp), %edx
	ja	.L3415
	movl	12(%esp), %eax
	movl	24(%esp), %ebp
	xorl	%esi, %esi
	movl	%edx, 64(%esp)
	leal	512(%eax), %edi
.L3432:
	movl	12(%esp), %edx
	movl	64(%esp), %ebx
	.align 16
.L3398:
	addl	$1, %edx
	movb	$0, -1(%edx)
	cmpl	%edx, %edi
	jne	.L3398
	movl	(%esp), %eax
	movl	curr_port, %ecx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	%edx, 28(%esp)
	movl	%ebx, 76(%esp)
	movl	%eax, 40(%esp)
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	44(%esp)
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%ecx
	.cfi_def_cfa_offset 112
	movl	%ecx, 72(%esp)
	call	SATA_Commander
	movl	72(%esp), %ecx
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	movl	16(%esp), %edx
	movl	16(%ecx), %eax
	testl	$1073741824, %eax
	je	.L3399
	movl	%edx, 24(%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	24(%esp), %edx
.L3399:
	movl	12(%esp), %eax
	xorl	%ecx, %ecx
	movl	64(%esp), %ebx
	jmp	.L3412
	.align 16
.L3444:
	addl	$1, %esi
	cmpl	$1, %esi
	cmove	%eax, %ebp
	cmpl	%ebx, %esi
	je	.L3442
.L3401:
	addl	$1, %ecx
	addl	$1, %eax
	cmpl	$512, %ecx
	je	.L3443
.L3412:
	cmpb	$0, (%eax)
	je	.L3444
	xorl	%esi, %esi
	cmpl	%ebx, %esi
	jne	.L3401
.L3442:
	movl	start_off, %eax
	movl	(%esp), %edi
	movl	%edx, %ebx
	movl	%ebp, 24(%esp)
	movl	4(%esp), %ebp
	xorl	%edx, %edx
	subl	%eax, %edi
	sbbl	%edx, %ebp
	movl	%edi, %eax
	movl	%ebp, %edx
	sall	$9, %eax
	xorl	%ebp, %ebp
	shldl	$9, %edi, %edx
	subl	%esi, %eax
	movl	12(%esp), %edi
	sbbl	%ebp, %edx
	xorl	%ebp, %ebp
	addl	%ecx, %eax
	adcl	%ebp, %edx
	movl	24(%esp), %ebp
	movl	%eax, 16(%esp)
	movl	%edx, 20(%esp)
	.align 16
.L3402:
	addl	$1, %edi
	movb	$0, -1(%edi)
	cmpl	%edi, %ebx
	jne	.L3402
	movl	20(%esp), %ebx
	movl	16(%esp), %ecx
	movl	start_off, %eax
	movl	%ebp, 24(%esp)
	shrdl	$9, %ebx, %ecx
	movl	curr_port, %ebx
	addl	%eax, %ecx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	%ecx, %eax
	movl	%ecx, 12(%esp)
	pushl	$1
	.cfi_def_cfa_offset 80
	cltd
	pushl	%edx
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	44(%esp)
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L3403
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3403:
	xorl	%ebx, %ebx
	testl	%esi, %esi
	je	.L3411
	movl	24(%esp), %eax
	movl	12(%esp), %ebp
	jmp	.L3426
	.align 16
.L3407:
	addl	$1, %ebx
	movb	$-1, (%eax)
	addl	$1, %eax
	cmpl	%esi, %ebx
	je	.L3411
.L3426:
	movl	%eax, %edx
	subl	%ebp, %edx
	cmpl	$512, %edx
	jne	.L3407
	movl	(%esp), %eax
	movl	curr_port, %ecx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	cltd
	pushl	%edx
	.cfi_def_cfa_offset 84
	pushl	20(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%ebp
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ecx
	.cfi_def_cfa_offset 112
	movl	%ecx, 72(%esp)
	call	SATA_Commander
	movl	72(%esp), %ecx
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	movl	16(%ecx), %eax
	testl	$1073741824, %eax
	jne	.L3445
.L3408:
	movl	%ebp, %eax
	.align 16
.L3409:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %edi
	jne	.L3409
	addl	$8, (%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	12(%esp), %ecx
	pushl	$1
	.cfi_def_cfa_offset 80
	movl	%ecx, %eax
	movl	curr_port, %ecx
	cltd
	pushl	%edx
	.cfi_def_cfa_offset 84
	pushl	20(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%ebp
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%ecx
	.cfi_def_cfa_offset 112
	movl	%ecx, 72(%esp)
	call	SATA_Commander
	movl	72(%esp), %ecx
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	movl	%ebp, %eax
	movl	16(%ecx), %edx
	andl	$1073741824, %edx
	je	.L3407
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	addl	$1, %ebx
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	movl	%ebp, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movb	$-1, (%eax)
	addl	$1, %eax
	cmpl	%esi, %ebx
	jne	.L3426
.L3411:
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	48(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	44(%esp)
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L3406
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3406:
	movl	16(%esp), %eax
	movl	20(%esp), %edx
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	shldl	$9, %eax, %edx
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	sall	$9, %eax
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L3445:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L3408
.L3443:
	addl	$1, (%esp)
	movl	%ebx, 64(%esp)
	adcl	$0, 4(%esp)
	xorl	%edx, %edx
	movl	bytemap_end, %eax
	movl	4(%esp), %ebx
	movl	(%esp), %ecx
	cmpl	%ebx, %edx
	ja	.L3432
	jb	.L3415
	cmpl	%ecx, %eax
	jnb	.L3432
	jmp	.L3415
	.cfi_endproc
.LFE370:
	.size	sec_alloc, .-sec_alloc
	.section	.rodata
.LC394:
	.string	"\nNo Memory left!"
	.text
	.align 16
	.globl	sblk_alloc
	.type	sblk_alloc, @function
sblk_alloc:
.LFB371:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	xorl	%ebx, %ebx
	subl	$56, %esp
	.cfi_def_cfa_offset 76
	movl	76(%esp), %esi
	pushl	$512
	.cfi_def_cfa_offset 80
	call	pmem
	movl	%eax, %ecx
	movl	%eax, 36(%esp)
	movl	bytemap_off, %eax
	movl	$0, 28(%esp)
	leal	512(%ecx), %edi
	movl	%eax, 24(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	cmpl	%eax, bytemap_end
	jb	.L3461
.L3470:
	movl	20(%esp), %eax
	.align 16
.L3449:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edi, %eax
	jne	.L3449
	movl	8(%esp), %eax
	movl	curr_port, %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	%eax, 40(%esp)
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	28(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	52(%esp)
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%edx
	.cfi_def_cfa_offset 112
	movl	%edx, 72(%esp)
	call	SATA_Commander
	movl	72(%esp), %edx
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	movl	16(%edx), %eax
	testl	$1073741824, %eax
	je	.L3450
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3450:
	movl	20(%esp), %edx
	xorl	%eax, %eax
	jmp	.L3458
	.align 16
.L3479:
	addl	$1, %ebx
	cmpl	$1, %ebx
	cmove	%edx, %ebp
	cmpl	%esi, %ebx
	je	.L3477
.L3452:
	addl	$1, %eax
	addl	$2, %edx
	cmpl	$256, %eax
	je	.L3478
.L3458:
	cmpw	$0, (%edx)
	je	.L3479
	xorl	%ebx, %ebx
	cmpl	%esi, %ebx
	jne	.L3452
.L3477:
	movl	start_off, %esi
	movl	8(%esp), %edx
	xorl	%edi, %edi
	movl	12(%esp), %ecx
	subl	%esi, %edx
	sbbl	%edi, %ecx
	movl	%edx, %esi
	movl	%ecx, %edi
	sall	$8, %esi
	shldl	$8, %edx, %edi
	movl	%esi, %edx
	movl	%eax, %esi
	movl	%edi, %ecx
	xorl	%edi, %edi
	addl	$2, %esi
	adcl	$0, %edi
	addl	%edx, %esi
	adcl	%ecx, %edi
	xorl	%edx, %edx
	subl	%ebx, %esi
	sbbl	%edx, %edi
	xorl	%eax, %eax
	testl	%ebx, %ebx
	je	.L3457
	.align 16
.L3466:
	movl	$-1, %edx
	movw	%dx, 0(%ebp,%eax,2)
	addl	$1, %eax
	cmpl	%eax, %ebx
	jne	.L3466
.L3457:
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	48(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	52(%esp)
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L3455
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3455:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%esi, %eax
	movl	%edi, %edx
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	shldl	$10, %esi, %edx
	sall	$10, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L3478:
	.cfi_restore_state
	addl	$1, 8(%esp)
	movl	bytemap_end, %eax
	adcl	$0, 12(%esp)
	xorl	%edx, %edx
	cmpl	12(%esp), %edx
	ja	.L3470
	jb	.L3461
	cmpl	8(%esp), %eax
	jnb	.L3470
.L3461:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC394
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3448:
	jmp	.L3448
	.cfi_endproc
.LFE371:
	.size	sblk_alloc, .-sblk_alloc
	.align 16
	.globl	del_blocks
	.type	del_blocks, @function
del_blocks:
.LFB372:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$56, %esp
	.cfi_def_cfa_offset 76
	movl	76(%esp), %edi
	movl	84(%esp), %eax
	movl	80(%esp), %ebp
	movl	88(%esp), %ebx
	movl	%eax, 20(%esp)
	movl	%edi, %eax
	shrdl	$9, %ebp, %eax
	movl	%ebp, %edx
	shrl	$9, %edx
	movl	%eax, 28(%esp)
	movl	%edx, 32(%esp)
	pushl	$512
	.cfi_def_cfa_offset 80
	call	pmem
	movl	curr_port, %esi
	movl	%eax, 28(%esp)
	movl	%edi, %eax
	shrdl	$18, %ebp, %eax
	movl	%ebp, %ecx
	movl	$1, (%esp)
	shrl	$18, %ecx
	movl	%eax, 40(%esp)
	movl	%ecx, 44(%esp)
	pushl	%ecx
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	44(%esp)
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L3481
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3481:
	movzwl	16(%esp), %edx
	movl	%edi, %ecx
	movl	%edx, %eax
	andw	$511, %ax
	addl	%edx, %eax
	movzwl	%ax, %eax
	addl	12(%esp), %eax
	movl	%eax, 16(%esp)
	movl	8(%esp), %eax
	orl	%ebx, %eax
	je	.L3487
	movl	16(%esp), %eax
	movl	$1, %ebp
	movzbl	(%eax), %edx
	xorl	%eax, %eax
	.align 16
.L3491:
	movl	%ebp, %esi
	addl	$1, %eax
	sall	%cl, %esi
	addl	$1, %ecx
	xorl	%esi, %edx
	cmpl	$0, %ebx
	ja	.L3491
	cmpl	%eax, 8(%esp)
	ja	.L3491
	movl	16(%esp), %eax
	movb	%dl, (%eax)
.L3487:
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	44(%esp)
	.cfi_def_cfa_offset 84
	pushl	44(%esp)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	44(%esp)
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	jne	.L3496
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3496:
	.cfi_restore_state
	movl	$.LC215, 64(%esp)
	addl	$44, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	printf
	.cfi_endproc
.LFE372:
	.size	del_blocks, .-del_blocks
	.align 16
	.globl	abc1
	.type	abc1, @function
abc1:
.LFB373:
	.cfi_startproc
	.align 16
.L3498:
	jmp	.L3498
	.cfi_endproc
.LFE373:
	.size	abc1, .-abc1
	.align 16
	.globl	test_exit
	.type	test_exit, @function
test_exit:
.LFB374:
	.cfi_startproc
/APP
/  47 "Source/multitasking/Scheduler/Scheduler.c" 1
	sti
/  0 "" 2
/NO_APP
.L3500:
	jmp	.L3500
	.cfi_endproc
.LFE374:
	.size	test_exit, .-test_exit
	.comm	CORE_INFOLocked,4,4
	.section	.rodata
.LC395:
	.string	"\nCore #%x Initialized"
.LC396:
	.string	"Spurious_task"
	.text
	.align 16
	.globl	Init_Scheduler
	.type	Init_Scheduler, @function
Init_Scheduler:
.LFB375:
	.cfi_startproc
	subl	$60, %esp
	.cfi_def_cfa_offset 64
	lock orl	$0, (%esp)
	movl	$0, CORE_INFOLocked
	movl	$Scheduler_end_t, %edx
	movl	$Scheduler_t, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, 36(%esp)
	movl	$switcher_end_t, %edx
	movl	$switcher_ksp_t, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, 32(%esp)
	movl	$Scheduler_init_end_t, %edx
	movl	$Scheduler_init_t, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, 28(%esp)
	movl	36(%esp), %edx
	movl	32(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 24(%esp)
	call	phy_alloc4K
	movl	%eax, MotherSpace
	movl	MotherSpace, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	$4096
	.cfi_def_cfa_offset 72
	pushl	$0
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	memset
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	MotherSpace, %eax
	movl	%eax, 44(%esp)
	movl	$0, 40(%esp)
	jmp	.L3502
.L3505:
	movl	40(%esp), %eax
	addl	$1, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%eax
	.cfi_def_cfa_offset 76
	pushl	$.LC395
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	call	phy_alloc4K
	movl	%eax, 20(%esp)
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	$4096
	.cfi_def_cfa_offset 72
	pushl	$0
	.cfi_def_cfa_offset 76
	pushl	32(%esp)
	.cfi_def_cfa_offset 80
	call	memset
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	40(%esp), %edx
	movl	44(%esp), %eax
	movl	%edx, (%eax)
	movl	44(%esp), %eax
	movl	$0, 4(%eax)
	movl	40(%esp), %eax
	addl	$1, %eax
	sall	$13, %eax
	subl	$4096, %eax
	movl	%eax, 16(%esp)
	movl	20(%esp), %eax
	movl	%eax, 12(%esp)
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	36(%esp)
	.cfi_def_cfa_offset 72
	pushl	$switcher_ksp_t
	.cfi_def_cfa_offset 76
	pushl	24(%esp)
	.cfi_def_cfa_offset 80
	call	memcpy
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	12(%esp), %edx
	movl	%edx, 36(%eax)
	movl	32(%esp), %eax
	addl	%eax, 12(%esp)
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	40(%esp)
	.cfi_def_cfa_offset 72
	pushl	$Scheduler_t
	.cfi_def_cfa_offset 76
	pushl	24(%esp)
	.cfi_def_cfa_offset 80
	call	memcpy
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	12(%esp), %eax
	movl	%eax, 8(%esp)
	movl	44(%esp), %eax
	movl	12(%esp), %edx
	movl	%edx, 32(%eax)
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$16384
	.cfi_def_cfa_offset 80
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	%eax, 4(%esp)
	movl	44(%esp), %eax
	movl	4(%esp), %edx
	movl	%edx, 64(%eax)
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	32(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	44(%esp), %ecx
	addl	$56, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115991249
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	system_dir, %edx
	movl	44(%esp), %eax
	movl	%edx, 56(%eax)
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	32(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	$system_dir, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115991250
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	32(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	44(%esp), %ecx
	addl	$44, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115991251
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	36(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	44(%esp), %ecx
	addl	$44, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115990996
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	32(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	44(%esp), %ecx
	addl	$48, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115991252
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	32(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	44(%esp), %ecx
	addl	$32, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115991253
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	32(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	44(%esp), %ecx
	addl	$52, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115991254
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	32(%esp), %edx
	sall	$2, %edx
	leal	(%eax,%edx), %ecx
	movl	44(%esp), %eax
	movl	36(%eax), %edx
	movl	4(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%ecx
	.cfi_def_cfa_offset 76
	pushl	%edx
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115999488
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	36(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	44(%esp), %ecx
	addl	$52, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115990998
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	36(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	44(%esp), %ecx
	addl	$16, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115990993
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	36(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	44(%esp), %ecx
	addl	$20, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115990994
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	36(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	44(%esp), %ecx
	addl	$24, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115990995
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	36(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	44(%esp), %ecx
	addl	$12, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115990997
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	32(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	36(%eax), %eax
	movl	44(%esp), %ecx
	addl	$68, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115995345
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	36(%esp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	44(%esp), %eax
	movl	32(%eax), %eax
	movl	44(%esp), %ecx
	addl	$68, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$4
	.cfi_def_cfa_offset 84
	pushl	%ecx
	.cfi_def_cfa_offset 88
	pushl	$4
	.cfi_def_cfa_offset 92
	pushl	$1115995345
	.cfi_def_cfa_offset 96
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	$0, 68(%eax)
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$163840
	.cfi_def_cfa_offset 80
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	%eax, %edx
	movl	44(%esp), %eax
	movl	%edx, 8(%eax)
	movl	44(%esp), %eax
	movl	8(%eax), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	$163840
	.cfi_def_cfa_offset 72
	pushl	$0
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	memset
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	44(%esp), %eax
	movl	8(%eax), %eax
	addl	$81920, %eax
	movl	%eax, %edx
	movl	44(%esp), %eax
	movl	%edx, 12(%eax)
	movl	44(%esp), %eax
	movl	12(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, 16(%eax)
	movl	44(%esp), %eax
	movl	$0, 20(%eax)
	movl	44(%esp), %eax
	movl	12(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, 28(%eax)
	movl	44(%esp), %eax
	movl	$1, 24(%eax)
	movl	kernel_proc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	$514
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$Spurious_task_func
	.cfi_def_cfa_offset 92
	pushl	$.LC396
	.cfi_def_cfa_offset 96
	call	create_task
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	movl	%eax, %edx
	movl	44(%esp), %eax
	movl	%edx, 60(%eax)
	movl	44(%esp), %eax
	movl	60(%eax), %eax
	movl	$1, 28(%eax)
	movl	BSP_id, %edx
	movl	40(%esp), %eax
	cmpl	%eax, %edx
	jne	.L3503
	movl	$0, %edx
	movl	$1, %eax
	call	makeFlagByte
	movzbl	%al, %eax
	movl	44(%esp), %edx
	movl	36(%edx), %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	$idt_entries
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	$8, %ecx
	movl	$51, %eax
	call	idtSetEntry
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	$0, %edx
	movl	$1, %eax
	call	makeFlagByte
	movzbl	%al, %eax
	movl	44(%esp), %edx
	movl	36(%edx), %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	$idt_entries
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	$8, %ecx
	movl	$50, %eax
	call	idtSetEntry
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	$0, %edx
	movl	$1, %eax
	call	makeFlagByte
	movzbl	%al, %eax
	movl	44(%esp), %edx
	movl	36(%edx), %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	$idt_entries
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	$8, %ecx
	movl	$52, %eax
	call	idtSetEntry
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L3504
.L3503:
	movl	AP_startup_Code_sz, %edx
	movl	16(%esp), %eax
	addl	%eax, %edx
	movl	pmode_code_size, %eax
	addl	%edx, %eax
	addl	$72, %eax
	movl	%eax, (%esp)
	movl	$0, %edx
	movl	$1, %eax
	call	makeFlagByte
	movzbl	%al, %eax
	movl	44(%esp), %edx
	movl	36(%edx), %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	8(%esp)
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	$8, %ecx
	movl	$50, %eax
	call	idtSetEntry
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	$0, %edx
	movl	$1, %eax
	call	makeFlagByte
	movzbl	%al, %eax
	movl	44(%esp), %edx
	movl	36(%edx), %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	8(%esp)
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	$8, %ecx
	movl	$51, %eax
	call	idtSetEntry
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	$0, %edx
	movl	$1, %eax
	call	makeFlagByte
	movzbl	%al, %eax
	movl	$generalProtectionFault_handler, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	8(%esp)
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	$8, %ecx
	movl	$13, %eax
	call	idtSetEntry
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L3504:
	addl	$76, 44(%esp)
	addl	$1, 40(%esp)
.L3502:
	movl	total_CPU_Cores, %eax
	leal	-1(%eax), %edx
	movl	40(%esp), %eax
	cmpl	%eax, %edx
	ja	.L3505
	movl	MotherSpace, %eax
	movl	%eax, KitList
	nop
	addl	$60, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE375:
	.size	Init_Scheduler, .-Init_Scheduler
	.align 16
	.globl	FindLightestScheduler
	.type	FindLightestScheduler, @function
FindLightestScheduler:
.LFB376:
	.cfi_startproc
	subl	$16, %esp
	.cfi_def_cfa_offset 20
	movl	MotherSpace, %eax
	movl	%eax, 12(%esp)
	movl	12(%esp), %eax
	movl	%eax, 8(%esp)
	movl	$0, 4(%esp)
	jmp	.L3507
.L3509:
	movl	8(%esp), %eax
	movl	4(%eax), %edx
	movl	12(%esp), %eax
	movl	4(%eax), %eax
	cmpl	%eax, %edx
	jbe	.L3508
	movl	12(%esp), %eax
	movl	%eax, 8(%esp)
.L3508:
	addl	$76, 12(%esp)
	addl	$1, 4(%esp)
.L3507:
	movl	total_CPU_Cores, %eax
	leal	-1(%eax), %edx
	movl	4(%esp), %eax
	cmpl	%eax, %edx
	ja	.L3509
	movl	8(%esp), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE376:
	.size	FindLightestScheduler, .-FindLightestScheduler
	.align 16
	.globl	Get_Scheduler
	.type	Get_Scheduler, @function
Get_Scheduler:
.LFB377:
	.cfi_startproc
	subl	$16, %esp
	.cfi_def_cfa_offset 20
	nop
.L3512:
	movl	$0, %eax
	movl	$1, %edx
	lock cmpxchgl	%edx, CORE_INFOLocked
	sete	%al
	xorl	$1, %eax
	testb	%al, %al
	jne	.L3512
	lock orl	$0, (%esp)
	movl	$-18874336, %eax
	movl	(%eax), %eax
	movl	%eax, 12(%esp)
	shrl	$24, 12(%esp)
	lock orl	$0, (%esp)
	movl	$0, CORE_INFOLocked
	movl	KitList, %edx
	movl	12(%esp), %eax
	imull	$76, %eax, %eax
	addl	%edx, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE377:
	.size	Get_Scheduler, .-Get_Scheduler
	.section	.rodata
.LC397:
	.string	"SAS_task_booster"
.LC398:
	.string	"SAS_void_eraser"
	.text
	.align 16
	.globl	SAS_init
	.type	SAS_init, @function
SAS_init:
.LFB378:
	.cfi_startproc
	subl	$76, %esp
	.cfi_def_cfa_offset 80
	movl	$SAS_end_t, %edx
	movl	$SAS_task_booster_t, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, 56(%esp)
	movl	$SAS_task_booster_end, %edx
	movl	$SAS_task_booster_t, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, 52(%esp)
	movl	$SAS_end_t, %edx
	movl	$SAS_void_eraser_t, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, 48(%esp)
	movl	$0, 60(%esp)
	jmp	.L3515
.L3516:
	call	phy_alloc4K
	movl	%eax, 44(%esp)
	subl	$4, %esp
	.cfi_def_cfa_offset 84
	pushl	$4096
	.cfi_def_cfa_offset 88
	pushl	$0
	.cfi_def_cfa_offset 92
	pushl	56(%esp)
	.cfi_def_cfa_offset 96
	call	memset
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	44(%esp), %eax
	movl	%eax, 40(%esp)
	movl	44(%esp), %eax
	addl	$4, %eax
	movl	%eax, 36(%esp)
	movl	44(%esp), %eax
	addl	$8, %eax
	movl	%eax, 32(%esp)
	movl	44(%esp), %eax
	addl	$12, %eax
	movl	%eax, 28(%esp)
	movl	44(%esp), %eax
	addl	$16, %eax
	movl	%eax, 24(%esp)
	movl	44(%esp), %eax
	addl	$20, %eax
	movl	%eax, 20(%esp)
	movl	44(%esp), %eax
	addl	$24, %eax
	movl	%eax, 16(%esp)
	movl	52(%esp), %eax
	andl	$-4, %eax
	leal	24(%eax), %edx
	movl	44(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 12(%esp)
	subl	$4, %esp
	.cfi_def_cfa_offset 84
	pushl	56(%esp)
	.cfi_def_cfa_offset 88
	pushl	$SAS_task_booster_t
	.cfi_def_cfa_offset 92
	pushl	28(%esp)
	.cfi_def_cfa_offset 96
	call	memcpy
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	subl	$4, %esp
	.cfi_def_cfa_offset 84
	pushl	52(%esp)
	.cfi_def_cfa_offset 88
	pushl	$SAS_void_eraser_t
	.cfi_def_cfa_offset 92
	pushl	24(%esp)
	.cfi_def_cfa_offset 96
	call	memcpy
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	56(%esp), %eax
	leal	0(,%eax,4), %edx
	movl	16(%esp), %eax
	addl	%eax, %edx
	movl	32(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 88
	pushl	%edx
	.cfi_def_cfa_offset 92
	pushl	28(%esp)
	.cfi_def_cfa_offset 96
	pushl	$4
	.cfi_def_cfa_offset 100
	pushl	%eax
	.cfi_def_cfa_offset 104
	pushl	$4
	.cfi_def_cfa_offset 108
	pushl	$1115999649
	.cfi_def_cfa_offset 112
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 80
	movl	56(%esp), %eax
	leal	0(,%eax,4), %edx
	movl	16(%esp), %eax
	addl	%eax, %edx
	movl	28(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 88
	pushl	%edx
	.cfi_def_cfa_offset 92
	pushl	28(%esp)
	.cfi_def_cfa_offset 96
	pushl	$4
	.cfi_def_cfa_offset 100
	pushl	%eax
	.cfi_def_cfa_offset 104
	pushl	$4
	.cfi_def_cfa_offset 108
	pushl	$1115999650
	.cfi_def_cfa_offset 112
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 80
	movl	56(%esp), %eax
	leal	0(,%eax,4), %edx
	movl	16(%esp), %eax
	addl	%eax, %edx
	movl	24(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 88
	pushl	%edx
	.cfi_def_cfa_offset 92
	pushl	28(%esp)
	.cfi_def_cfa_offset 96
	pushl	$4
	.cfi_def_cfa_offset 100
	pushl	%eax
	.cfi_def_cfa_offset 104
	pushl	$4
	.cfi_def_cfa_offset 108
	pushl	$1115999651
	.cfi_def_cfa_offset 112
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 80
	movl	56(%esp), %eax
	leal	0(,%eax,4), %edx
	movl	16(%esp), %eax
	addl	%eax, %edx
	movl	20(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 88
	pushl	%edx
	.cfi_def_cfa_offset 92
	pushl	28(%esp)
	.cfi_def_cfa_offset 96
	pushl	$4
	.cfi_def_cfa_offset 100
	pushl	%eax
	.cfi_def_cfa_offset 104
	pushl	$4
	.cfi_def_cfa_offset 108
	pushl	$1115999652
	.cfi_def_cfa_offset 112
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 80
	movl	56(%esp), %eax
	leal	0(,%eax,4), %edx
	movl	16(%esp), %eax
	addl	%edx, %eax
	movl	$printf, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 88
	pushl	%eax
	.cfi_def_cfa_offset 92
	pushl	28(%esp)
	.cfi_def_cfa_offset 96
	pushl	$4
	.cfi_def_cfa_offset 100
	pushl	%edx
	.cfi_def_cfa_offset 104
	pushl	$4
	.cfi_def_cfa_offset 108
	pushl	$1115992049
	.cfi_def_cfa_offset 112
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 80
	movl	32(%esp), %eax
	movl	$1, (%eax)
	movl	28(%esp), %eax
	movl	$0, (%eax)
	movl	KitList, %edx
	movl	60(%esp), %eax
	imull	$76, %eax, %eax
	addl	%edx, %eax
	movl	%eax, %edx
	movl	24(%esp), %eax
	movl	%edx, (%eax)
	movl	20(%esp), %eax
	movl	$0, (%eax)
	movl	SAS_proc, %edx
	movl	16(%esp), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	%edx
	.cfi_def_cfa_offset 96
	pushl	$514
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	%eax
	.cfi_def_cfa_offset 108
	pushl	$.LC397
	.cfi_def_cfa_offset 112
	call	create_task
	addl	$32, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, %edx
	movl	40(%esp), %eax
	movl	%edx, (%eax)
	movl	SAS_proc, %edx
	movl	12(%esp), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	%edx
	.cfi_def_cfa_offset 96
	pushl	$514
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	%eax
	.cfi_def_cfa_offset 108
	pushl	$.LC398
	.cfi_def_cfa_offset 112
	call	create_task
	addl	$32, %esp
	.cfi_def_cfa_offset 80
	movl	%eax, %edx
	movl	36(%esp), %eax
	movl	%edx, (%eax)
	movl	KitList, %edx
	movl	60(%esp), %eax
	imull	$76, %eax, %eax
	addl	%eax, %edx
	movl	40(%esp), %eax
	movl	(%eax), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 88
	pushl	%edx
	.cfi_def_cfa_offset 92
	pushl	%eax
	.cfi_def_cfa_offset 96
	call	Activate_task_direct_SP
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	KitList, %edx
	movl	60(%esp), %eax
	imull	$76, %eax, %eax
	addl	%eax, %edx
	movl	36(%esp), %eax
	movl	(%eax), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 88
	pushl	%edx
	.cfi_def_cfa_offset 92
	pushl	%eax
	.cfi_def_cfa_offset 96
	call	Activate_task_direct_SP
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	40(%esp), %eax
	movl	(%eax), %eax
	movl	$1, 28(%eax)
	movl	36(%esp), %eax
	movl	(%eax), %eax
	movl	$1, 28(%eax)
	addl	$1, 60(%esp)
.L3515:
	movl	total_CPU_Cores, %eax
	leal	-1(%eax), %edx
	movl	60(%esp), %eax
	cmpl	%eax, %edx
	ja	.L3516
	nop
	addl	$76, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE378:
	.size	SAS_init, .-SAS_init
	.align 16
	.globl	SAS_task_booster_t
	.type	SAS_task_booster_t, @function
SAS_task_booster_t:
.LFB379:
	.cfi_startproc
	subl	$32, %esp
	.cfi_def_cfa_offset 36
	movl	$1115999651, %eax
	movl	(%eax), %eax
	movl	%eax, 28(%esp)
	movl	$1115992049, 24(%esp)
.L3521:
	movl	$1115999649, %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	.L3518
/APP
/  264 "Source/multitasking/Scheduler/Scheduler.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	28(%esp), %eax
	movl	$1, 68(%eax)
	movl	$1115999650, %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	.L3519
	movl	$1115999650, %eax
	movl	(%eax), %eax
	movl	24(%eax), %eax
	testl	%eax, %eax
	je	.L3519
	movl	$1115999650, %eax
	movl	(%eax), %eax
	movl	%eax, 20(%esp)
	movl	20(%esp), %eax
	movl	16(%eax), %edx
	movl	20(%esp), %eax
	movl	%edx, 20(%eax)
	movl	20(%esp), %eax
	movl	24(%eax), %eax
	movl	%eax, 16(%esp)
	movl	28(%esp), %eax
	movl	60(%eax), %eax
	movl	%eax, %edx
	movl	16(%esp), %eax
	movl	%edx, (%eax)
	movl	28(%esp), %eax
	movl	12(%eax), %eax
	movl	%eax, 12(%esp)
	subl	$16384, 12(%esp)
	movl	12(%esp), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	12(%esp), %eax
	movl	%edx, (%eax)
	movl	12(%esp), %eax
	movl	(%eax), %eax
	leal	0(,%eax,4), %edx
	movl	12(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 8(%esp)
	movl	20(%esp), %edx
	movl	8(%esp), %eax
	movl	%edx, (%eax)
	movl	8(%esp), %edx
	movl	20(%esp), %eax
	movl	%edx, 24(%eax)
	movl	28(%esp), %eax
	movl	16(%eax), %eax
	cmpl	12(%esp), %eax
	jbe	.L3520
	movl	28(%esp), %eax
	movl	12(%esp), %edx
	movl	%edx, 16(%eax)
.L3520:
	movl	28(%esp), %eax
	movl	$0, 20(%eax)
	movl	$1115999650, %eax
	movl	$0, (%eax)
.L3519:
	movl	$1115999649, %eax
	movl	$0, (%eax)
	movl	28(%esp), %eax
	movl	$0, 68(%eax)
.L3518:
/APP
/  299 "Source/multitasking/Scheduler/Scheduler.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L3521
	.cfi_endproc
.LFE379:
	.size	SAS_task_booster_t, .-SAS_task_booster_t
	.align 16
	.globl	SAS_task_booster_end
	.type	SAS_task_booster_end, @function
SAS_task_booster_end:
.LFB568:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	SAS_end_t
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE568:
	.size	SAS_task_booster_end, .-SAS_task_booster_end
	.align 16
	.globl	SAS_void_eraser_t
	.type	SAS_void_eraser_t, @function
SAS_void_eraser_t:
.LFB381:
	.cfi_startproc
	subl	$16, %esp
	.cfi_def_cfa_offset 20
	movl	$1, 12(%esp)
	movl	$1115999651, %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
.L3532:
	movl	$1115999649, %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	.L3524
/APP
/  317 "Source/multitasking/Scheduler/Scheduler.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	8(%esp), %eax
	movl	$1, 68(%eax)
	movl	8(%esp), %eax
	movl	28(%eax), %eax
	movl	(%eax), %eax
	cmpl	12(%esp), %eax
	jb	.L3525
	movl	8(%esp), %eax
	movl	28(%eax), %eax
	movl	12(%esp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	%eax, 4(%esp)
	movl	4(%esp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	.L3526
	movl	4(%esp), %eax
	movl	(%eax), %edx
	movl	8(%esp), %eax
	movl	60(%eax), %eax
	cmpl	%eax, %edx
	jne	.L3527
.L3526:
	movl	8(%esp), %eax
	movl	28(%eax), %edx
	movl	8(%esp), %eax
	movl	28(%eax), %eax
	movl	(%eax), %eax
	sall	$2, %eax
	addl	%edx, %eax
	movl	%eax, (%esp)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	4(%esp), %eax
	movl	%edx, (%eax)
	movl	(%esp), %eax
	movl	$0, (%eax)
	movl	4(%esp), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	movl	4(%esp), %eax
	movl	%eax, 24(%edx)
	movl	8(%esp), %eax
	movl	28(%eax), %eax
	movl	(%eax), %edx
	subl	$1, %edx
	movl	%edx, (%eax)
	movl	8(%esp), %eax
	movl	24(%eax), %edx
	movl	8(%esp), %eax
	movl	28(%eax), %eax
	movl	(%eax), %eax
	cmpl	%eax, %edx
	jbe	.L3533
	movl	8(%esp), %eax
	movl	$1, 24(%eax)
	jmp	.L3533
.L3527:
	movl	4(%esp), %eax
	movl	(%eax), %eax
	movl	28(%eax), %eax
	testl	%eax, %eax
	jne	.L3529
	movl	4(%esp), %eax
	movl	(%eax), %eax
	movl	20(%eax), %edx
	movl	$1115999652, %eax
	movl	(%eax), %eax
	cmpl	%eax, %edx
	jbe	.L3530
	movl	$1115999650, %edx
	movl	4(%esp), %eax
	movl	(%eax), %eax
	movl	%eax, (%edx)
	movl	$1115999652, %edx
	movl	4(%esp), %eax
	movl	(%eax), %eax
	movl	20(%eax), %eax
	movl	%eax, (%edx)
.L3530:
	movl	4(%esp), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	movl	4(%esp), %eax
	movl	(%eax), %eax
	movl	20(%eax), %eax
	addl	$1, %eax
	movl	%eax, 20(%edx)
	jmp	.L3529
.L3533:
	nop
.L3529:
	addl	$1, 12(%esp)
	jmp	.L3531
.L3525:
	movl	$1115999649, %eax
	movl	$1, (%eax)
	movl	$1, 12(%esp)
	movl	$1115999652, %eax
	movl	$0, (%eax)
.L3531:
	movl	8(%esp), %eax
	movl	$0, 68(%eax)
.L3524:
/APP
/  354 "Source/multitasking/Scheduler/Scheduler.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L3532
	.cfi_endproc
.LFE381:
	.size	SAS_void_eraser_t, .-SAS_void_eraser_t
	.align 16
	.globl	SAS_end_t
	.type	SAS_end_t, @function
SAS_end_t:
.LFB382:
	.cfi_startproc
	nop
	ret
	.cfi_endproc
.LFE382:
	.size	SAS_end_t, .-SAS_end_t
	.comm	char_B,4,4
	.comm	char_C,4,4
	.comm	char_E,4,4
	.comm	char_1,4,4
	.comm	char_2,4,4
	.comm	char_3,4,4
	.comm	char_4,4,4
	.comm	char_5,4,4
	.section	.rodata
.LC399:
	.string	"hpet"
.LC400:
	.string	"ext2"
.LC401:
	.string	"aqfs"
.LC402:
	.string	"apic"
.LC403:
	.string	"nn"
	.align 4
.LC404:
	.string	"\nNo such Device \"%s\" found to be initialized"
	.text
	.align 16
	.globl	Command_init
	.type	Command_init, @function
Command_init:
.LFB383:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
/APP
/  13 "Source/Shell/Commands/init/init.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	Main_CSI_struct, %eax
	movl	8(%eax), %edx
	movl	$.LC399, %eax
	movzbl	(%edx), %ecx
	cmpb	$104, %cl
	je	.L3561
	jmp	.L3575
	.align 16
.L3541:
	testb	%cl, %cl
	je	.L3576
.L3561:
	addl	$1, %eax
	movl	%eax, %ecx
	subl	$.LC399, %ecx
	movzbl	(%ecx,%edx), %ecx
	cmpb	(%eax), %cl
	je	.L3541
.L3542:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	%edx
	.cfi_def_cfa_offset 28
	pushl	$.LC404
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3576:
	.cfi_restore_state
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	jmp	init_hpet
.L3575:
	.cfi_restore_state
	cmpb	$101, %cl
	jne	.L3577
	movl	$.LC400, %eax
	jmp	.L3546
	.align 16
.L3547:
	testb	%cl, %cl
	je	.L3578
.L3546:
	addl	$1, %eax
	movzbl	(%eax), %ecx
	movl	%eax, %ebx
	subl	$.LC400, %ebx
	cmpb	(%ebx,%edx), %cl
	je	.L3547
	jmp	.L3542
	.align 16
.L3578:
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movl	$4096, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	jmp	ext2_initialize.part.23
.L3577:
	.cfi_restore_state
	cmpb	$97, %cl
	jne	.L3579
	movl	$.LC401, %eax
	jmp	.L3551
	.align 16
.L3552:
	testb	%cl, %cl
	je	.L3580
.L3551:
	addl	$1, %eax
	movzbl	(%eax), %ecx
	movl	%eax, %ebx
	subl	$.LC401, %ebx
	cmpb	(%ebx,%edx), %cl
	je	.L3552
	movl	$.LC402, %eax
	jmp	.L3555
	.align 16
.L3556:
	testb	%cl, %cl
	je	.L3554
.L3555:
	addl	$1, %eax
	movzbl	(%eax), %ecx
	movl	%eax, %ebx
	subl	$.LC402, %ebx
	cmpb	(%ebx,%edx), %cl
	je	.L3556
	jmp	.L3542
	.align 16
.L3580:
	call	AqFS_burn
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	jmp	Init_fs
	.align 16
.L3554:
	.cfi_restore_state
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	jmp	MADTapic_parse
.L3579:
	.cfi_restore_state
	cmpb	$110, %cl
	jne	.L3542
	movl	$.LC403, %eax
	jmp	.L3559
	.align 16
.L3560:
	testb	%cl, %cl
	je	.L3581
.L3559:
	addl	$1, %eax
	movzbl	(%eax), %ecx
	movl	%eax, %ebx
	subl	$.LC403, %ebx
	cmpb	(%ebx,%edx), %cl
	je	.L3560
	jmp	.L3542
	.align 16
.L3581:
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	jmp	NeuralNet_init
	.cfi_endproc
.LFE383:
	.size	Command_init, .-Command_init
	.section	.rodata
.LC405:
	.string	"-d"
.LC406:
	.string	"-s"
.LC407:
	.string	"-r"
.LC408:
	.string	"\nFile copied successfully\n"
	.text
	.align 16
	.globl	Command_cp
	.type	Command_cp, @function
Command_cp:
.LFB384:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
/APP
/  16 "Source/Shell/Commands/filesystem/general.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$.LC405
	.cfi_def_cfa_offset 64
	call	CSI_ReadAPS
	movl	$.LC406, (%esp)
	movl	%eax, %ebx
	call	CSI_ReadAPS
	movl	%eax, %ebp
	movl	$.LC407, (%esp)
	call	CSI_ReadAPS
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpb	$0, 0(%ebp)
	movl	%eax, %edi
	je	.L3598
	leal	1(%ebp), %edx
	xorl	%eax, %eax
	jmp	.L3584
	.align 16
.L3599:
	movl	%esi, %eax
.L3584:
	movl	%edx, %ecx
	leal	1(%edx), %edx
	cmpb	$0, -1(%edx)
	leal	1(%eax), %esi
	jne	.L3599
	cmpb	$47, 0(%ebp,%eax)
	je	.L3583
	testl	%eax, %eax
	jns	.L3585
	jmp	.L3583
	.align 16
.L3621:
	testl	%eax, %eax
	js	.L3608
.L3585:
	subl	$1, %eax
	cmpb	$47, 0(%ebp,%eax)
	jne	.L3621
.L3608:
	leal	1(%ebp,%eax), %ecx
.L3583:
	testl	%edi, %edi
	cmove	%ecx, %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	%ebp
	.cfi_def_cfa_offset 64
	call	file_loadOGP
	movl	%ebp, (%esp)
	call	file_size
	movl	%eax, %esi
	movl	%eax, (%esp)
	call	*kmalloc
	movl	%eax, 28(%esp)
	pushl	%ebp
	.cfi_def_cfa_offset 68
	pushl	%esi
	.cfi_def_cfa_offset 72
	pushl	$0
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	file_readTM
	addl	$20, %esp
	.cfi_def_cfa_offset 60
	pushl	%ebp
	.cfi_def_cfa_offset 64
	call	file_closeOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpb	$0, (%ebx)
	movl	kmalloc, %edx
	je	.L3602
	xorl	%eax, %eax
	jmp	.L3589
	.align 16
.L3603:
	movl	%ecx, %eax
.L3589:
	cmpb	$0, 1(%ebx,%eax)
	leal	1(%eax), %ecx
	jne	.L3603
	leal	2(%eax), %ecx
.L3588:
	xorl	%eax, %eax
	cmpb	$0, (%edi)
	je	.L3590
	.align 16
.L3591:
	addl	$1, %eax
	cmpb	$0, (%edi,%eax)
	jne	.L3591
.L3590:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	addl	%ecx, %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	*%edx
	movzbl	(%ebx), %ecx
	movl	%eax, %ebp
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%ebx, %edx
	.align 16
.L3592:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L3592
	movb	$0, (%eax)
	xorl	%eax, %eax
	cmpb	$0, (%ebx)
	je	.L3593
	.align 16
.L3594:
	addl	$1, %eax
	cmpb	$0, (%ebx,%eax)
	jne	.L3594
.L3593:
	movb	$47, 0(%ebp,%eax)
	cmpb	$0, (%ebx)
	je	.L3606
	xorl	%eax, %eax
	jmp	.L3596
	.align 16
.L3607:
	movl	%edx, %eax
.L3596:
	cmpb	$0, 1(%ebx,%eax)
	leal	1(%eax), %edx
	jne	.L3607
	addl	$2, %eax
.L3595:
	movzbl	(%edi), %ecx
	addl	%ebp, %eax
	movl	%edi, %edx
	.align 16
.L3597:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L3597
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	movb	$0, (%eax)
	pushl	%ebx
	.cfi_def_cfa_offset 56
	pushl	$1
	.cfi_def_cfa_offset 60
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	create_file
	movl	%ebp, (%esp)
	call	file_loadOGP
	addl	$12, %esp
	.cfi_def_cfa_offset 52
	pushl	%ebp
	.cfi_def_cfa_offset 56
	pushl	%esi
	.cfi_def_cfa_offset 60
	pushl	24(%esp)
	.cfi_def_cfa_offset 64
	call	file_writeAppend
	movl	%ebp, (%esp)
	call	file_closeOGP
	movl	$.LC408, (%esp)
	call	printf
	addl	$44, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L3598:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -20
	.cfi_offset 5, -8
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	movl	%ebp, %ecx
	jmp	.L3583
.L3602:
	movl	$1, %ecx
	jmp	.L3588
.L3606:
	movl	$1, %eax
	jmp	.L3595
	.cfi_endproc
.LFE384:
	.size	Command_cp, .-Command_cp
	.section	.rodata
.LC409:
	.string	"\nThe Folder is corrupt.\n"
	.align 4
.LC410:
	.string	"\n Folder Successfully deleted!\n"
	.align 4
.LC411:
	.string	"\n%s is not a file, nor a folder!\n"
.LC412:
	.string	"\nThe File is corrupt.\n"
.LC413:
	.string	"\n File Successfully deleted!\n"
	.text
	.align 16
	.globl	Command_del
	.type	Command_del, @function
Command_del:
.LFB385:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
/APP
/  63 "Source/Shell/Commands/filesystem/general.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	Main_CSI_struct, %eax
	movl	8(%eax), %esi
	pushl	%esi
	.cfi_def_cfa_offset 48
	call	search_folderOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L3623
	cmpl	$11465840, 6(%eax)
	je	.L3624
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC409
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L3622:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L3623:
	.cfi_restore_state
	movl	start_handle, %edi
	testl	%edi, %edi
	je	.L3627
	movzbl	(%esi), %ebp
	.align 16
.L3633:
	movl	4(%edi), %ebx
	movl	%ebp, %eax
	cmpb	(%ebx), %al
	jne	.L3628
	testb	%al, %al
	je	.L3629
	movl	%ebx, %eax
	jmp	.L3631
	.align 16
.L3632:
	testb	%dl, %dl
	je	.L3629
.L3631:
	addl	$1, %eax
	movzbl	(%eax), %edx
	movl	%eax, %ecx
	subl	%ebx, %ecx
	cmpb	(%ecx,%esi), %dl
	je	.L3632
.L3628:
	movl	28(%edi), %edi
	testl	%edi, %edi
	jne	.L3633
.L3627:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	%esi
	.cfi_def_cfa_offset 48
	call	file_loadOGP
	movl	start_handle, %edi
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%edi, %edi
	je	.L3634
	movzbl	(%esi), %ebp
.L3639:
	movl	4(%edi), %ebx
	movl	%ebp, %eax
	cmpb	(%ebx), %al
	jne	.L3635
	testb	%al, %al
	je	.L3629
	movl	%ebx, %eax
	jmp	.L3637
	.align 16
.L3638:
	testb	%dl, %dl
	je	.L3629
.L3637:
	addl	$1, %eax
	movzbl	(%eax), %edx
	movl	%eax, %ecx
	subl	%ebx, %ecx
	cmpb	(%ecx,%esi), %dl
	je	.L3638
.L3635:
	movl	28(%edi), %edi
	testl	%edi, %edi
	jne	.L3639
.L3634:
	movl	8, %eax
	ud2
	.align 16
.L3629:
	movl	8(%edi), %eax
	testl	%eax, %eax
	je	.L3667
	cmpl	$11465841, 6(%eax)
	je	.L3641
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC412
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L3624:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	delete_dir
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L3622
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC410
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L3622
.L3641:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	delete_file
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L3622
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC413
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L3622
.L3667:
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%esi
	.cfi_def_cfa_offset 44
	pushl	$.LC411
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L3622
	.cfi_endproc
.LFE385:
	.size	Command_del, .-Command_del
	.section	.rodata
.LC414:
	.string	"\n%s directory created\n"
	.text
	.align 16
	.globl	Command_mkdir
	.type	Command_mkdir, @function
Command_mkdir:
.LFB386:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
/APP
/  107 "Source/Shell/Commands/filesystem/general.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	Main_CSI_struct, %eax
	movl	kmalloc, %edx
	movl	8(%eax), %ebx
	cmpb	$0, (%ebx)
	je	.L3679
	xorl	%eax, %eax
	.align 16
.L3670:
	addl	$1, %eax
	cmpb	$0, (%eax,%ebx)
	jne	.L3670
	addl	$1, %eax
.L3669:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	*%edx
	movzbl	(%ebx), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%eax, %edx
	.align 16
.L3671:
	addl	$1, %edx
	movb	%cl, -1(%edx)
	movl	%edx, %ecx
	subl	%eax, %ecx
	movzbl	(%ecx,%ebx), %ecx
	testb	%cl, %cl
	jne	.L3671
	movb	$0, (%edx)
	cmpb	$0, (%eax)
	je	.L3689
	leal	1(%eax), %edx
	xorl	%ecx, %ecx
	jmp	.L3673
	.align 16
.L3680:
	movl	%esi, %ecx
.L3673:
	movl	%edx, %edi
	addl	$1, %edx
	leal	1(%ecx), %esi
	cmpb	$0, -1(%edx)
	jne	.L3680
	leal	(%eax,%ecx), %esi
	movl	%ecx, %edx
	cmpb	$47, (%esi)
	je	.L3674
	testl	%ecx, %ecx
	jns	.L3677
	jmp	.L3682
	.align 16
.L3676:
	cmpl	$-1, %edx
	je	.L3689
.L3677:
	subl	$1, %edx
	cmpb	$47, (%eax,%edx)
	leal	(%eax,%edx), %esi
	jne	.L3676
	leal	1(%eax,%edx), %edi
.L3674:
	testl	%edx, %edx
	jle	.L3683
	movb	$0, (%esi)
	jmp	.L3678
.L3682:
	movl	%edi, %eax
	.align 16
.L3689:
	movl	%eax, %edi
	xorl	%eax, %eax
.L3678:
	subl	$4, %esp
	.cfi_def_cfa_offset 20
	pushl	%eax
	.cfi_def_cfa_offset 24
	pushl	$1
	.cfi_def_cfa_offset 28
	pushl	%edi
	.cfi_def_cfa_offset 32
	call	create_directory
	popl	%eax
	.cfi_def_cfa_offset 28
	popl	%edx
	.cfi_def_cfa_offset 24
	pushl	%ebx
	.cfi_def_cfa_offset 28
	pushl	$.LC414
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
.L3679:
	.cfi_restore_state
	movl	$1, %eax
	jmp	.L3669
.L3683:
	xorl	%eax, %eax
	jmp	.L3678
	.cfi_endproc
.LFE386:
	.size	Command_mkdir, .-Command_mkdir
	.section	.rodata
.LC415:
	.string	"\n%s file created\n"
	.text
	.align 16
	.globl	Command_mkfl
	.type	Command_mkfl, @function
Command_mkfl:
.LFB387:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
/APP
/  129 "Source/Shell/Commands/filesystem/general.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	Main_CSI_struct, %eax
	movl	kmalloc, %edx
	movl	8(%eax), %ebx
	cmpb	$0, (%ebx)
	je	.L3701
	xorl	%eax, %eax
	.align 16
.L3692:
	addl	$1, %eax
	cmpb	$0, (%eax,%ebx)
	jne	.L3692
	addl	$1, %eax
.L3691:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	*%edx
	movzbl	(%ebx), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%eax, %edx
	.align 16
.L3693:
	addl	$1, %edx
	movb	%cl, -1(%edx)
	movl	%edx, %ecx
	subl	%eax, %ecx
	movzbl	(%ecx,%ebx), %ecx
	testb	%cl, %cl
	jne	.L3693
	movb	$0, (%edx)
	cmpb	$0, (%eax)
	je	.L3711
	leal	1(%eax), %edx
	xorl	%ecx, %ecx
	jmp	.L3695
	.align 16
.L3702:
	movl	%esi, %ecx
.L3695:
	movl	%edx, %edi
	addl	$1, %edx
	leal	1(%ecx), %esi
	cmpb	$0, -1(%edx)
	jne	.L3702
	leal	(%eax,%ecx), %esi
	movl	%ecx, %edx
	cmpb	$47, (%esi)
	je	.L3696
	testl	%ecx, %ecx
	jns	.L3699
	jmp	.L3704
	.align 16
.L3698:
	cmpl	$-1, %edx
	je	.L3711
.L3699:
	subl	$1, %edx
	cmpb	$47, (%eax,%edx)
	leal	(%eax,%edx), %esi
	jne	.L3698
	leal	1(%eax,%edx), %edi
.L3696:
	testl	%edx, %edx
	jle	.L3705
	movb	$0, (%esi)
	jmp	.L3700
.L3704:
	movl	%edi, %eax
	.align 16
.L3711:
	movl	%eax, %edi
	xorl	%eax, %eax
.L3700:
	subl	$4, %esp
	.cfi_def_cfa_offset 20
	pushl	%eax
	.cfi_def_cfa_offset 24
	pushl	$1
	.cfi_def_cfa_offset 28
	pushl	%edi
	.cfi_def_cfa_offset 32
	call	create_file
	popl	%eax
	.cfi_def_cfa_offset 28
	popl	%edx
	.cfi_def_cfa_offset 24
	pushl	%ebx
	.cfi_def_cfa_offset 28
	pushl	$.LC415
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
.L3701:
	.cfi_restore_state
	movl	$1, %eax
	jmp	.L3691
.L3705:
	xorl	%eax, %eax
	jmp	.L3700
	.cfi_endproc
.LFE387:
	.size	Command_mkfl, .-Command_mkfl
	.section	.rodata
.LC416:
	.string	"app"
.LC417:
	.string	"edit"
.LC418:
	.string	"-f"
.LC419:
	.string	"\nPlease Specify a File Name."
.LC420:
	.string	"\n%s file dosent exist!\n"
.LC421:
	.string	"-a"
.LC422:
	.string	"\n Data written!"
.LC423:
	.string	" %x\n"
	.align 4
.LC424:
	.string	"\n Edit Type not recognized! %x\n"
.LC425:
	.string	"\n File edited Successfully!\n"
	.align 4
.LC426:
	.string	"\nEnter the data (max size: 4096 bytes)->\n"
.LC427:
	.string	"\nWritting the Data"
	.align 4
.LC428:
	.string	"\n Edit Type not recognized! %s\n"
.LC429:
	.string	"-x"
.LC430:
	.string	"-z"
	.text
	.align 16
	.globl	Command_editfl
	.type	Command_editfl, @function
Command_editfl:
.LFB388:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
/APP
/  153 "Source/Shell/Commands/filesystem/general.c" 1
	cli
/  0 "" 2
/NO_APP
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC418
	.cfi_def_cfa_offset 32
	call	CSI_ReadAPS
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	movl	%eax, %ebx
	je	.L3792
.L3713:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	file_loadOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	je	.L3793
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC405
	.cfi_def_cfa_offset 32
	call	CSI_ReadAPS
	movl	%eax, %esi
	movl	$.LC421, (%esp)
	call	CSI_ReadAPS
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%esi, %esi
	movl	%eax, %edi
	je	.L3716
	testl	%eax, %eax
	je	.L3717
	movzbl	(%eax), %ecx
	movl	%eax, %edx
	movl	$.LC416, %eax
	cmpb	$97, %cl
	je	.L3776
	jmp	.L3794
	.align 16
.L3721:
	testb	%cl, %cl
	je	.L3717
.L3776:
	addl	$1, %edx
	addl	$1, %eax
	movzbl	(%edx), %ecx
	cmpb	(%eax), %cl
	je	.L3721
.L3722:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	%edi
	.cfi_def_cfa_offset 28
	pushl	$.LC424
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L3726
	.align 16
.L3717:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC422
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	cmpb	$0, (%esi)
	je	.L3724
	.align 16
.L3725:
	addl	$1, %eax
	cmpb	$0, (%esi,%eax)
	jne	.L3725
.L3724:
	subl	$4, %esp
	.cfi_def_cfa_offset 20
	pushl	%ebx
	.cfi_def_cfa_offset 24
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	%esi
	.cfi_def_cfa_offset 32
	call	file_writeAppend
	popl	%ecx
	.cfi_def_cfa_offset 28
	popl	%esi
	.cfi_def_cfa_offset 24
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC423
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L3726:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	file_closeOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L3712:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3792:
	.cfi_restore_state
	movl	Main_CSI_struct, %eax
	movl	8(%eax), %ebx
	testl	%ebx, %ebx
	jne	.L3713
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC419
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L3712
	.align 16
.L3793:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	%ebx
	.cfi_def_cfa_offset 28
	pushl	$.LC420
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
.L3716:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC426
	.cfi_def_cfa_offset 32
	call	printf
	movl	$4096, (%esp)
	call	*kmalloc
	movl	%eax, %esi
	popl	%eax
	.cfi_def_cfa_offset 28
	popl	%edx
	.cfi_def_cfa_offset 24
	pushl	$4096
	.cfi_def_cfa_offset 28
	pushl	%esi
	.cfi_def_cfa_offset 32
	call	kb_getline
	movl	$.LC427, (%esp)
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%edi, %edi
	je	.L3734
	movzbl	(%edi), %ecx
	movl	%edi, %edx
	movl	$.LC416, %eax
	cmpb	$97, %cl
	je	.L3777
	jmp	.L3795
	.align 16
.L3738:
	testb	%cl, %cl
	je	.L3734
.L3777:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L3738
.L3739:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	%edi
	.cfi_def_cfa_offset 28
	pushl	$.LC428
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L3743:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%esi
	.cfi_def_cfa_offset 32
	call	*kfree
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L3726
.L3794:
	cmpb	$101, %cl
	jne	.L3722
	movl	%edi, %edx
	movl	$.LC417, %eax
	jmp	.L3728
	.align 16
.L3729:
	testb	%cl, %cl
	je	.L3796
.L3728:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L3729
	jmp	.L3722
	.align 16
.L3796:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC429
	.cfi_def_cfa_offset 32
	call	CSI_ReadAPS
	movl	%eax, (%esp)
	call	StrToInt
	movl	$.LC430, (%esp)
	movl	%eax, %edi
	call	CSI_ReadAPS
	movl	%eax, (%esp)
	call	StrToInt
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	jne	.L3751
	cmpb	$0, (%esi)
	je	.L3757
	.align 16
.L3768:
	addl	$1, %eax
	cmpb	$0, (%esi,%eax)
	jne	.L3768
.L3731:
	xorl	%edx, %edx
	.align 16
.L3733:
	addl	$1, %edx
	cmpb	$0, (%esi,%edx)
	jne	.L3733
.L3732:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	pushl	%edx
	.cfi_def_cfa_offset 36
	pushl	%esi
	.cfi_def_cfa_offset 40
	pushl	%eax
	.cfi_def_cfa_offset 44
	pushl	%edi
	.cfi_def_cfa_offset 48
	call	file_editFM
	addl	$20, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC425
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L3726
	.align 16
.L3751:
	cmpb	$0, (%esi)
	jne	.L3731
.L3757:
	xorl	%edx, %edx
	jmp	.L3732
.L3734:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC422
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	cmpb	$0, (%esi)
	je	.L3741
	.align 16
.L3742:
	addl	$1, %eax
	cmpb	$0, (%esi,%eax)
	jne	.L3742
.L3741:
	subl	$4, %esp
	.cfi_def_cfa_offset 20
	pushl	%ebx
	.cfi_def_cfa_offset 24
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	%esi
	.cfi_def_cfa_offset 32
	call	file_writeAppend
	popl	%edx
	.cfi_def_cfa_offset 28
	popl	%ecx
	.cfi_def_cfa_offset 24
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC423
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L3743
.L3795:
	cmpb	$101, %cl
	jne	.L3739
	movl	%edi, %edx
	movl	$.LC417, %eax
	jmp	.L3745
	.align 16
.L3746:
	testb	%cl, %cl
	je	.L3797
.L3745:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L3746
	jmp	.L3739
.L3797:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC429
	.cfi_def_cfa_offset 32
	call	CSI_ReadAPS
	movl	%eax, (%esp)
	call	StrToInt
	movl	$.LC430, (%esp)
	movl	%eax, %edi
	call	CSI_ReadAPS
	movl	%eax, (%esp)
	call	StrToInt
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	jne	.L3753
	cmpb	$0, (%esi)
	je	.L3759
.L3769:
	addl	$1, %eax
	cmpb	$0, (%esi,%eax)
	jne	.L3769
.L3748:
	xorl	%edx, %edx
	.align 16
.L3750:
	addl	$1, %edx
	cmpb	$0, (%esi,%edx)
	jne	.L3750
.L3749:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	pushl	%edx
	.cfi_def_cfa_offset 36
	pushl	%esi
	.cfi_def_cfa_offset 40
	pushl	%eax
	.cfi_def_cfa_offset 44
	pushl	%edi
	.cfi_def_cfa_offset 48
	call	file_editFM
	addl	$20, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC425
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L3743
.L3753:
	cmpb	$0, (%esi)
	jne	.L3748
.L3759:
	xorl	%edx, %edx
	jmp	.L3749
	.cfi_endproc
.LFE388:
	.size	Command_editfl, .-Command_editfl
	.section	.rodata
.LC431:
	.string	"\nSize of the file: %x \n"
.LC432:
	.string	"\t\tError: %x\n"
	.text
	.align 16
	.globl	Command_rfl
	.type	Command_rfl, @function
Command_rfl:
.LFB389:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
/APP
/  226 "Source/Shell/Commands/filesystem/general.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	Main_CSI_struct, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	8(%eax), %ebx
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	file_loadOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	je	.L3803
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC429
	.cfi_def_cfa_offset 32
	call	CSI_ReadAPS
	movl	$.LC430, (%esp)
	call	CSI_ReadAPS
	movl	%eax, (%esp)
	call	StrToInt
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	movl	%eax, %esi
	je	.L3804
.L3801:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	%esi
	.cfi_def_cfa_offset 28
	pushl	$.LC431
	.cfi_def_cfa_offset 32
	call	printf
	movl	$8192, (%esp)
	call	*kmalloc
	pushl	%ebx
	.cfi_def_cfa_offset 36
	pushl	%esi
	.cfi_def_cfa_offset 40
	movl	%eax, %edi
	pushl	$0
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	file_readTM
	addl	$24, %esp
	.cfi_def_cfa_offset 24
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC432
	.cfi_def_cfa_offset 32
	call	printf
	movl	%ebx, (%esp)
	call	file_closeOGP
	popl	%eax
	.cfi_def_cfa_offset 28
	popl	%edx
	.cfi_def_cfa_offset 24
	pushl	%edi
	.cfi_def_cfa_offset 28
	pushl	$.LC3
	.cfi_def_cfa_offset 32
	call	printf
	movl	%edi, (%esp)
	call	*kfree
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3803:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	%ebx
	.cfi_def_cfa_offset 28
	pushl	$.LC420
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3804:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	file_size
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%eax, %esi
	jmp	.L3801
	.cfi_endproc
.LFE389:
	.size	Command_rfl, .-Command_rfl
	.align 16
	.globl	Command_ls
	.type	Command_ls, @function
Command_ls:
.LFB390:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
/APP
/  10 "Source/Shell/Commands/filesystem/ls.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$0
	.cfi_def_cfa_offset 32
	call	find_friendDirs
	movl	$0, (%esp)
	call	find_childFiles
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE390:
	.size	Command_ls, .-Command_ls
	.section	.rodata
.LC433:
	.string	".."
	.text
	.align 16
	.globl	Command_cd
	.type	Command_cd, @function
Command_cd:
.LFB391:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
/APP
/  10 "Source/Shell/Commands/filesystem/cd.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	Main_CSI_struct, %eax
	movl	$2, %ecx
	movl	$.LC433, %edx
	movl	8(%eax), %ebx
	movl	%ebx, %eax
	call	strncmp.part.0
	testl	%eax, %eax
	je	.L3814
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	search_folderOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	je	.L3807
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	30(%eax)
	.cfi_def_cfa_offset 28
	pushl	26(%eax)
	.cfi_def_cfa_offset 32
	call	set_curr_dir
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L3807:
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3814:
	.cfi_restore_state
	movl	curr_dir, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	22(%eax)
	.cfi_def_cfa_offset 28
	pushl	18(%eax)
	.cfi_def_cfa_offset 32
	call	set_curr_dir
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE391:
	.size	Command_cd, .-Command_cd
	.align 16
	.globl	Command_rn
	.type	Command_rn, @function
Command_rn:
.LFB392:
	.cfi_startproc
	movl	Main_CSI_struct, %eax
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	8(%eax), %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	file_loadOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	je	.L3820
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC429
	.cfi_def_cfa_offset 32
	call	CSI_ReadAPS
	movl	$.LC430, (%esp)
	call	CSI_ReadAPS
	movl	%eax, (%esp)
	call	StrToInt
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	movl	%eax, %esi
	je	.L3821
.L3818:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	%esi
	.cfi_def_cfa_offset 28
	pushl	$.LC431
	.cfi_def_cfa_offset 32
	call	printf
	movl	$8192, (%esp)
	call	*kmalloc
	pushl	%ebx
	.cfi_def_cfa_offset 36
	pushl	%esi
	.cfi_def_cfa_offset 40
	movl	%eax, %edi
	pushl	$0
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	file_readTM
	addl	$20, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	file_closeOGP
	popl	%eax
	.cfi_def_cfa_offset 28
	popl	%edx
	.cfi_def_cfa_offset 24
	pushl	%esi
	.cfi_def_cfa_offset 28
	pushl	%edi
	.cfi_def_cfa_offset 32
	call	Shell_CFexecute
	movl	%edi, (%esp)
	call	*kfree
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3820:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	%ebx
	.cfi_def_cfa_offset 28
	pushl	$.LC420
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3821:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	file_size
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%eax, %esi
	jmp	.L3818
	.cfi_endproc
.LFE392:
	.size	Command_rn, .-Command_rn
	.section	.rodata
.LC434:
	.string	"%x=%x--"
.LC435:
	.string	"-o"
.LC436:
	.string	"\nUpdating Neural Network"
	.text
	.align 16
	.globl	Command_ann
	.type	Command_ann, @function
Command_ann:
.LFB393:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	Main_CSI_struct, %eax
	movl	8(%eax), %edi
/APP
/  14 "Source/Shell/Commands/ANN/ann.c" 1
	cli
/  0 "" 2
/NO_APP
	testl	%edi, %edi
	je	.L3839
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	file_loadOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	je	.L3840
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	file_size
	movl	$.LC435, (%esp)
	movl	%eax, %esi
	call	CSI_ReadAPS
	popl	%ecx
	.cfi_def_cfa_offset 60
	popl	%ebx
	.cfi_def_cfa_offset 56
	pushl	%esi
	.cfi_def_cfa_offset 60
	pushl	$.LC431
	.cfi_def_cfa_offset 64
	call	printf
	movl	%esi, (%esp)
	call	*kmalloc
	movl	%eax, 28(%esp)
	pushl	%edi
	.cfi_def_cfa_offset 68
	pushl	%esi
	.cfi_def_cfa_offset 72
	pushl	$0
	.cfi_def_cfa_offset 76
	shrl	$4, %esi
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	file_readTM
	movl	%esi, %eax
	movl	$277094665, %edx
	addl	$32, %esp
	.cfi_def_cfa_offset 48
	mull	%edx
	shrl	%edx
	subl	$1, %edx
	movl	%edx, %esi
	je	.L3830
	movl	12(%esp), %ebx
	xorl	%ebp, %ebp
	movl	12(%ebx), %eax
	jmp	.L3831
	.align 16
.L3841:
	addl	$1, %ebp
	cmpl	%ebp, %esi
	je	.L3830
.L3831:
	movzwl	10(%ebx), %ecx
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	addl	$496, %ebx
	pushl	%ecx
	.cfi_def_cfa_offset 56
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	$.LC434
	.cfi_def_cfa_offset 64
	call	printf
	movl	12(%ebx), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	jne	.L3841
.L3830:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC436
	.cfi_def_cfa_offset 64
	call	printf
	call	NeuralForwardUpdater
	movl	%edi, (%esp)
	call	file_closeOGP
	popl	%eax
	.cfi_def_cfa_offset 60
	popl	%edx
	.cfi_def_cfa_offset 56
	movl	20(%esp), %edi
	pushl	%edi
	.cfi_def_cfa_offset 60
	pushl	$.LC3
	.cfi_def_cfa_offset 64
	call	printf
	movl	%edi, (%esp)
	call	*kfree
/APP
/  64 "Source/Shell/Commands/ANN/ann.c" 1
	sti
/  0 "" 2
/NO_APP
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3840:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%edi
	.cfi_def_cfa_offset 60
	pushl	$.LC420
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3839:
	.cfi_restore_state
	movl	Neuron_New, %ebx
	movl	Neurons, %eax
	xorl	%esi, %esi
	cmpl	$1, %eax
	je	.L3827
	.align 16
.L3834:
	movzwl	10(%ebx), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	addl	$1, %esi
	addl	$496, %ebx
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	-484(%ebx)
	.cfi_def_cfa_offset 60
	pushl	$.LC434
	.cfi_def_cfa_offset 64
	call	printf
	movl	Neurons, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	subl	$1, %eax
	cmpl	%esi, %eax
	ja	.L3834
.L3827:
/APP
/  27 "Source/Shell/Commands/ANN/ann.c" 1
	sti
/  0 "" 2
/NO_APP
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE393:
	.size	Command_ann, .-Command_ann
	.globl	test_counter
	.section	.bss
	.align 4
	.type	test_counter, @object
	.size	test_counter, 4
test_counter:
	.zero	4
	.globl	pit_freq
	.data
	.align 4
	.type	pit_freq, @object
	.size	pit_freq, 4
pit_freq:
	.long	1000
	.section	.rodata
	.align 4
.LC437:
	.string	"Task Occured: %x\nTimer ticks: %x"
	.text
	.align 16
	.globl	Test_counter
	.type	Test_counter, @function
Test_counter:
.LFB394:
	.cfi_startproc
	.align 16
.L3844:
	movl	test_counter, %eax
	addl	$1, %eax
	movl	%eax, test_counter
	movl	test_counter, %eax
	cmpl	$10000, %eax
	ja	.L3845
.L3843:
/APP
/  59 "Source/Shell/Commands/Default_Commands.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L3844
	.align 16
.L3845:
	movl	test_counter, %eax
	movl	HPET_main_counter, %edx
	pushl	(%edx)
	.cfi_def_cfa_offset 8
	pushl	%eax
	.cfi_def_cfa_offset 12
	pushl	$.LC437
	.cfi_def_cfa_offset 16
	call	_printf
	movl	$0, test_counter
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	jmp	.L3843
	.cfi_endproc
.LFE394:
	.size	Test_counter, .-Test_counter
	.section	.rodata
.LC438:
	.string	"\nAvialable Commands are:\n"
	.align 4
.LC439:
	.string	"\n\thelp         To get all the available commands"
	.align 4
.LC440:
	.string	"\n\tmdbug        To test the Memory Managment System"
	.align 4
.LC441:
	.string	"\n\tshutdown     To ACPI Power off the system (may not work on few machines)"
	.align 4
.LC442:
	.string	"\n\tstart vesa   To enter VESA Super VGA Mode"
	.align 4
.LC443:
	.string	"\n\tmemmap       To view the Physical Memory Map of the system"
	.align 4
.LC444:
	.string	"\n\ttest         To Test multitasking"
	.align 4
.LC445:
	.string	"\n\tls           To show the current directory's contents"
	.align 4
.LC446:
	.string	"\n\tcd <dir>     To move to a directory ('0' for root, '..' to move to parent)"
	.align 4
.LC447:
	.string	"\n\tmkdir <name>            To create a directory"
	.align 4
.LC448:
	.string	"\n\tmkfl <name>             To create a file"
	.align 4
.LC449:
	.string	"\n\tcreate <type> <name>    To create a file or a directory ('file' for file, 'dir' for directory)\n"
	.align 4
.LC450:
	.string	"\n\teditfl <name> {<data>}  To edit a file"
	.align 4
.LC451:
	.string	"\n\tdel <name>    To delete a folder/file (by default folder)"
	.align 4
.LC452:
	.string	"\n\tremove        To remove a directory"
.LC453:
	.string	"\n\tother commands\n"
	.text
	.align 16
	.globl	Command_help
	.type	Command_help, @function
Command_help:
.LFB395:
	.cfi_startproc
	pushl	$.LC438
	.cfi_def_cfa_offset 8
	call	_printf
	pushl	$.LC439
	.cfi_def_cfa_offset 12
	call	_printf
	pushl	$.LC440
	.cfi_def_cfa_offset 16
	call	_printf
	pushl	$.LC441
	.cfi_def_cfa_offset 20
	call	_printf
	pushl	$.LC442
	.cfi_def_cfa_offset 24
	call	_printf
	pushl	$.LC443
	.cfi_def_cfa_offset 28
	call	_printf
	pushl	$.LC444
	.cfi_def_cfa_offset 32
	call	_printf
	pushl	$.LC445
	.cfi_def_cfa_offset 36
	call	_printf
	addl	$32, %esp
	.cfi_def_cfa_offset 4
	pushl	$.LC446
	.cfi_def_cfa_offset 8
	call	_printf
	pushl	$.LC447
	.cfi_def_cfa_offset 12
	call	_printf
	pushl	$.LC448
	.cfi_def_cfa_offset 16
	call	_printf
	pushl	$.LC449
	.cfi_def_cfa_offset 20
	call	_printf
	pushl	$.LC450
	.cfi_def_cfa_offset 24
	call	_printf
	pushl	$.LC451
	.cfi_def_cfa_offset 28
	call	_printf
	pushl	$.LC452
	.cfi_def_cfa_offset 32
	call	_printf
	pushl	$.LC453
	.cfi_def_cfa_offset 36
	call	_printf
	addl	$32, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE395:
	.size	Command_help, .-Command_help
	.section	.rodata
.LC454:
	.string	"\n\t%x"
	.text
	.align 16
	.globl	Command_aptest
	.type	Command_aptest, @function
Command_aptest:
.LFB396:
	.cfi_startproc
	subl	$20, %esp
	.cfi_def_cfa_offset 24
	pushl	1280
	.cfi_def_cfa_offset 28
	pushl	$.LC454
	.cfi_def_cfa_offset 32
	call	printf
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE396:
	.size	Command_aptest, .-Command_aptest
	.section	.rodata
.LC455:
	.string	"\n Turning Power off"
	.text
	.align 16
	.globl	Command_shutdown
	.type	Command_shutdown, @function
Command_shutdown:
.LFB397:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	pushl	$.LC455
	.cfi_def_cfa_offset 20
	call	_printf
	cmpw	$0, SCI_EN
	popl	%eax
	.cfi_def_cfa_offset 16
	je	.L3850
	call	acpiPowerOff.part.8
.L3850:
/APP
/  92 "Source/Shell/Commands/Default_Commands.c" 1
	cli;      hlt
/  0 "" 2
/NO_APP
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE397:
	.size	Command_shutdown, .-Command_shutdown
	.section	.rodata
	.align 4
.LC456:
	.string	"\n Testing Virtual Memory Manager"
	.text
	.align 16
	.globl	Command_mdbug
	.type	Command_mdbug, @function
Command_mdbug:
.LFB398:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	pushl	$.LC456
	.cfi_def_cfa_offset 20
	call	_printf
	addl	$16, %esp
	.cfi_def_cfa_offset 4
	jmp	dbug
	.cfi_endproc
.LFE398:
	.size	Command_mdbug, .-Command_mdbug
	.section	.rodata
	.align 4
.LC457:
	.string	"\n Entering VESA SVGA mode 1024*768"
.LC458:
	.string	"Vesa_test_buf"
.LC459:
	.string	"Mouse_Pointer"
	.text
	.align 16
	.globl	Command_start_vesa
	.type	Command_start_vesa, @function
Command_start_vesa:
.LFB399:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	pushl	$.LC457
	.cfi_def_cfa_offset 36
	call	_printf
	subl	$8, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC457
	.cfi_def_cfa_offset 48
	call	printf
	movl	$279, (%esp)
	call	setVesa
	popl	%eax
	.cfi_def_cfa_offset 44
	pushl	Shell_Ostream_task
	.cfi_def_cfa_offset 48
	call	Task_sleep
	popl	%eax
	.cfi_def_cfa_offset 44
	pushl	Shell_proc
	.cfi_def_cfa_offset 48
	pushl	$514
	.cfi_def_cfa_offset 52
	pushl	$1
	.cfi_def_cfa_offset 56
	pushl	$DBuff
	.cfi_def_cfa_offset 60
	pushl	$.LC458
	.cfi_def_cfa_offset 64
	call	create_task
	addl	$20, %esp
	.cfi_def_cfa_offset 44
	movl	%eax, Shell_Ostream_task
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	Activate_task_direct
	movl	$65280, (%esp)
	pushl	$10
	.cfi_def_cfa_offset 52
	pushl	$50
	.cfi_def_cfa_offset 56
	pushl	$300
	.cfi_def_cfa_offset 60
	pushl	$400
	.cfi_def_cfa_offset 64
	call	RectD
	addl	$20, %esp
	.cfi_def_cfa_offset 44
	pushl	$16711680
	.cfi_def_cfa_offset 48
	pushl	$10
	.cfi_def_cfa_offset 52
	pushl	$50
	.cfi_def_cfa_offset 56
	pushl	$300
	.cfi_def_cfa_offset 60
	pushl	$500
	.cfi_def_cfa_offset 64
	call	RectD
	addl	$20, %esp
	.cfi_def_cfa_offset 44
	pushl	$255
	.cfi_def_cfa_offset 48
	pushl	$10
	.cfi_def_cfa_offset 52
	pushl	$50
	.cfi_def_cfa_offset 56
	pushl	$300
	.cfi_def_cfa_offset 60
	pushl	$600
	.cfi_def_cfa_offset 64
	call	RectD
	addl	$20, %esp
	.cfi_def_cfa_offset 44
	pushl	$3840
	.cfi_def_cfa_offset 48
	pushl	$10
	.cfi_def_cfa_offset 52
	pushl	$50
	.cfi_def_cfa_offset 56
	pushl	$400
	.cfi_def_cfa_offset 60
	pushl	$400
	.cfi_def_cfa_offset 64
	call	RectD
	addl	$20, %esp
	.cfi_def_cfa_offset 44
	pushl	$61440
	.cfi_def_cfa_offset 48
	pushl	$10
	.cfi_def_cfa_offset 52
	pushl	$50
	.cfi_def_cfa_offset 56
	pushl	$400
	.cfi_def_cfa_offset 60
	pushl	$500
	.cfi_def_cfa_offset 64
	call	RectD
	addl	$20, %esp
	.cfi_def_cfa_offset 44
	pushl	$240
	.cfi_def_cfa_offset 48
	pushl	$10
	.cfi_def_cfa_offset 52
	pushl	$50
	.cfi_def_cfa_offset 56
	pushl	$400
	.cfi_def_cfa_offset 60
	pushl	$600
	.cfi_def_cfa_offset 64
	call	RectD
	addl	$20, %esp
	.cfi_def_cfa_offset 44
	pushl	$15
	.cfi_def_cfa_offset 48
	pushl	$10
	.cfi_def_cfa_offset 52
	pushl	$50
	.cfi_def_cfa_offset 56
	pushl	$400
	.cfi_def_cfa_offset 60
	pushl	$700
	.cfi_def_cfa_offset 64
	call	RectD
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	call	font_maker
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	$3
	.cfi_def_cfa_offset 44
	pushl	$10
	.cfi_def_cfa_offset 48
	pushl	$61440
	.cfi_def_cfa_offset 52
	pushl	$100
	.cfi_def_cfa_offset 56
	pushl	$100
	.cfi_def_cfa_offset 60
	pushl	char_B
	.cfi_def_cfa_offset 64
	call	font_renderer
	addl	$24, %esp
	.cfi_def_cfa_offset 40
	pushl	$3
	.cfi_def_cfa_offset 44
	pushl	$10
	.cfi_def_cfa_offset 48
	pushl	$61695
	.cfi_def_cfa_offset 52
	pushl	$100
	.cfi_def_cfa_offset 56
	pushl	$160
	.cfi_def_cfa_offset 60
	pushl	char_C
	.cfi_def_cfa_offset 64
	call	font_renderer
	addl	$24, %esp
	.cfi_def_cfa_offset 40
	pushl	$3
	.cfi_def_cfa_offset 44
	pushl	$10
	.cfi_def_cfa_offset 48
	pushl	$3840
	.cfi_def_cfa_offset 52
	pushl	$100
	.cfi_def_cfa_offset 56
	pushl	$240
	.cfi_def_cfa_offset 60
	pushl	char_E
	.cfi_def_cfa_offset 64
	call	font_renderer
	addl	$24, %esp
	.cfi_def_cfa_offset 40
	pushl	$3
	.cfi_def_cfa_offset 44
	pushl	$10
	.cfi_def_cfa_offset 48
	pushl	$65295
	.cfi_def_cfa_offset 52
	pushl	$100
	.cfi_def_cfa_offset 56
	pushl	$320
	.cfi_def_cfa_offset 60
	pushl	char_1
	.cfi_def_cfa_offset 64
	call	font_renderer
	addl	$24, %esp
	.cfi_def_cfa_offset 40
	pushl	$3
	.cfi_def_cfa_offset 44
	pushl	$10
	.cfi_def_cfa_offset 48
	pushl	$3855
	.cfi_def_cfa_offset 52
	pushl	$100
	.cfi_def_cfa_offset 56
	pushl	$380
	.cfi_def_cfa_offset 60
	pushl	char_2
	.cfi_def_cfa_offset 64
	call	font_renderer
	addl	$24, %esp
	.cfi_def_cfa_offset 40
	pushl	$3
	.cfi_def_cfa_offset 44
	pushl	$10
	.cfi_def_cfa_offset 48
	pushl	$65520
	.cfi_def_cfa_offset 52
	pushl	$100
	.cfi_def_cfa_offset 56
	pushl	$460
	.cfi_def_cfa_offset 60
	pushl	char_3
	.cfi_def_cfa_offset 64
	call	font_renderer
	addl	$24, %esp
	.cfi_def_cfa_offset 40
	pushl	$3
	.cfi_def_cfa_offset 44
	pushl	$10
	.cfi_def_cfa_offset 48
	pushl	$64015
	.cfi_def_cfa_offset 52
	pushl	$100
	.cfi_def_cfa_offset 56
	pushl	$520
	.cfi_def_cfa_offset 60
	pushl	char_4
	.cfi_def_cfa_offset 64
	call	font_renderer
	addl	$24, %esp
	.cfi_def_cfa_offset 40
	pushl	$3
	.cfi_def_cfa_offset 44
	pushl	$10
	.cfi_def_cfa_offset 48
	pushl	$64015
	.cfi_def_cfa_offset 52
	pushl	$100
	.cfi_def_cfa_offset 56
	pushl	$580
	.cfi_def_cfa_offset 60
	pushl	char_5
	.cfi_def_cfa_offset 64
	call	font_renderer
	movl	depthVESA, %edx
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	leal	7(%edx), %eax
	testl	%edx, %edx
	cmovns	%edx, %eax
	movl	widthVESA, %edx
	sarl	$3, %eax
	imull	%eax, %edx
	addl	%edx, %eax
	movl	cx0, %edx
	imull	$500, %eax, %eax
	movl	%edx, (%esp)
	movl	(%esp), %ecx
	movl	$499, %edx
	cmpl	$499, %ecx
	jg	.L3855
	movl	(%esp), %edx
.L3855:
	movl	%edx, cx0
	movl	cx1, %edx
	movl	%edx, 4(%esp)
	movl	4(%esp), %ecx
	movl	$501, %edx
	cmpl	$500, %ecx
	jle	.L3856
	movl	4(%esp), %edx
.L3856:
	movl	%edx, cx1
	movl	cy0, %edx
	movl	%edx, 8(%esp)
	movl	8(%esp), %ecx
	movl	$499, %edx
	cmpl	$499, %ecx
	jg	.L3857
	movl	8(%esp), %edx
.L3857:
	movl	%edx, cy0
	movl	cy1, %edx
	movl	%edx, 12(%esp)
	movl	12(%esp), %ecx
	movl	$501, %edx
	cmpl	$500, %ecx
	jle	.L3858
	movl	12(%esp), %edx
.L3858:
	movl	%edx, cy1
	movl	buff, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	movl	$255, %ecx
	movw	%cx, (%edx,%eax)
	pushl	$1
	.cfi_def_cfa_offset 44
	pushl	$65535
	.cfi_def_cfa_offset 48
	pushl	$500
	.cfi_def_cfa_offset 52
	pushl	$100
	.cfi_def_cfa_offset 56
	pushl	$400
	.cfi_def_cfa_offset 60
	pushl	$100
	.cfi_def_cfa_offset 64
	call	line_fast
	addl	$24, %esp
	.cfi_def_cfa_offset 40
	pushl	$10
	.cfi_def_cfa_offset 44
	pushl	$65535
	.cfi_def_cfa_offset 48
	pushl	$500
	.cfi_def_cfa_offset 52
	pushl	$130
	.cfi_def_cfa_offset 56
	pushl	$400
	.cfi_def_cfa_offset 60
	pushl	$130
	.cfi_def_cfa_offset 64
	call	line_fast
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	call	random_plotter
	call	mouseinit
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	Shell_proc
	.cfi_def_cfa_offset 48
	pushl	$514
	.cfi_def_cfa_offset 52
	pushl	$2
	.cfi_def_cfa_offset 56
	pushl	$Mouse_Plot
	.cfi_def_cfa_offset 60
	pushl	$.LC459
	.cfi_def_cfa_offset 64
	call	create_task
	addl	$20, %esp
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	Activate_task_direct
	addl	$44, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE399:
	.size	Command_start_vesa, .-Command_start_vesa
	.section	.rodata
.LC460:
	.string	"\nMemory Map:"
	.align 4
.LC461:
	.string	"region %i address: %x size: %x Bytes Type: %i (%s)\n"
	.text
	.align 16
	.globl	Command_memmap
	.type	Command_memmap, @function
Command_memmap:
.LFB400:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	xorl	%esi, %esi
	subl	$16, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC460
	.cfi_def_cfa_offset 32
	call	printf
	movl	mmap_info, %ebx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	.align 16
.L3867:
	movl	(%ebx), %eax
	testl	%eax, %eax
	je	.L3865
	movl	20(%ebx), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	addl	$24, %ebx
	pushl	strMemoryTypes-4(,%eax,4)
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	pushl	-12(%ebx)
	.cfi_def_cfa_offset 36
	pushl	-20(%ebx)
	.cfi_def_cfa_offset 40
	pushl	%esi
	.cfi_def_cfa_offset 44
	pushl	$.LC461
	.cfi_def_cfa_offset 48
	addl	$1, %esi
	call	printf
	addl	$32, %esp
	.cfi_def_cfa_offset 16
	cmpl	$15, %esi
	jne	.L3867
.L3865:
	addl	$4, %esp
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE400:
	.size	Command_memmap, .-Command_memmap
	.section	.rodata
.LC462:
	.string	"\na->esp: %x"
	.align 4
.LC463:
	.string	"\n &a: %x, &b: %x; &a->esp: %x, &b->name: %x; a->esp: %x, b->esp: %x, a->special: %x, b->special: %x"
	.align 4
.LC464:
	.string	"\n a->esp: %x, b->esp: %x, a->special: %x, b->special: %x"
	.text
	.align 16
	.globl	Command_test_structs
	.type	Command_test_structs, @function
Command_test_structs:
.LFB401:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
/APP
/  173 "Source/Shell/Commands/Default_Commands.c" 1
	cli
/  0 "" 2
/NO_APP
	pushl	$102
	.cfi_def_cfa_offset 48
	call	*kmalloc
	movl	$102, (%esp)
	movl	%eax, %ebx
	call	*kmalloc
	movl	%eax, %ebp
	popl	%eax
	.cfi_def_cfa_offset 44
	popl	%edx
	.cfi_def_cfa_offset 40
	pushl	$102
	.cfi_def_cfa_offset 44
	pushl	$.LC376
	.cfi_def_cfa_offset 48
	call	_printf
	movl	$10, (%ebx)
	popl	%ecx
	.cfi_def_cfa_offset 44
	popl	%esi
	.cfi_def_cfa_offset 40
	pushl	$10
	.cfi_def_cfa_offset 44
	pushl	$.LC462
	.cfi_def_cfa_offset 48
	call	_printf
	movl	$2, 0(%ebp)
	popl	%edi
	.cfi_def_cfa_offset 44
	popl	%eax
	.cfi_def_cfa_offset 40
	pushl	(%ebx)
	.cfi_def_cfa_offset 44
	pushl	$.LC462
	.cfi_def_cfa_offset 48
	call	_printf
	movl	$1, 28(%ebx)
	popl	%eax
	.cfi_def_cfa_offset 44
	popl	%edx
	.cfi_def_cfa_offset 40
	pushl	(%ebx)
	.cfi_def_cfa_offset 44
	pushl	$.LC462
	.cfi_def_cfa_offset 48
	call	_printf
	movl	$2, 28(%ebp)
	popl	%ecx
	.cfi_def_cfa_offset 44
	popl	%esi
	.cfi_def_cfa_offset 40
	pushl	(%ebx)
	.cfi_def_cfa_offset 44
	pushl	$.LC462
	.cfi_def_cfa_offset 48
	movl	%ebp, %esi
	call	_printf
	leal	52(%ebp), %eax
	popl	%edi
	.cfi_def_cfa_offset 44
	pushl	28(%ebp)
	.cfi_def_cfa_offset 48
	pushl	28(%ebx)
	.cfi_def_cfa_offset 52
	leal	4(%ebx), %edi
	pushl	0(%ebp)
	.cfi_def_cfa_offset 56
	pushl	(%ebx)
	.cfi_def_cfa_offset 60
	pushl	%eax
	.cfi_def_cfa_offset 64
	pushl	%ebx
	.cfi_def_cfa_offset 68
	andl	$-4, %edi
	pushl	%ebp
	.cfi_def_cfa_offset 72
	pushl	%ebx
	.cfi_def_cfa_offset 76
	pushl	$.LC463
	.cfi_def_cfa_offset 80
	call	_printf
	movl	0(%ebp), %eax
	movl	%ebx, %ecx
	addl	$36, %esp
	.cfi_def_cfa_offset 44
	subl	%edi, %ecx
	subl	%ecx, %esi
	addl	$102, %ecx
	movl	%eax, (%ebx)
	movl	98(%ebp), %eax
	shrl	$2, %ecx
	movl	%eax, 98(%ebx)
	rep movsl
	pushl	28(%ebp)
	.cfi_def_cfa_offset 48
	pushl	28(%ebx)
	.cfi_def_cfa_offset 52
	pushl	0(%ebp)
	.cfi_def_cfa_offset 56
	pushl	(%ebx)
	.cfi_def_cfa_offset 60
	pushl	$.LC464
	.cfi_def_cfa_offset 64
	call	_printf
	addl	$44, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE401:
	.size	Command_test_structs, .-Command_test_structs
	.section	.rodata
.LC465:
	.string	"Test_counter"
	.text
	.align 16
	.globl	Command_start_counter
	.type	Command_start_counter, @function
Command_start_counter:
.LFB402:
	.cfi_startproc
	subl	$20, %esp
	.cfi_def_cfa_offset 24
/APP
/  192 "Source/Shell/Commands/Default_Commands.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	hpet, %eax
	movl	4(%eax), %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC376
	.cfi_def_cfa_offset 32
	call	printf
	popl	%eax
	.cfi_def_cfa_offset 28
	pushl	kernel_proc
	.cfi_def_cfa_offset 32
	pushl	$514
	.cfi_def_cfa_offset 36
	pushl	$10
	.cfi_def_cfa_offset 40
	pushl	$Test_counter
	.cfi_def_cfa_offset 44
	pushl	$.LC465
	.cfi_def_cfa_offset 48
	call	create_task
	addl	$20, %esp
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	Activate_task_direct
	movl	HPET_main_counter, %eax
	movl	$0, test_pit_timer
	movl	$0, (%eax)
	movl	hpet, %eax
	movl	$0, 240(%eax)
	movl	$0, 244(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE402:
	.size	Command_start_counter, .-Command_start_counter
	.section	.rodata
.LC466:
	.string	"%x %x"
	.text
	.align 16
	.globl	Command_counter
	.type	Command_counter, @function
Command_counter:
.LFB403:
	.cfi_startproc
	movl	test_counter, %eax
	pushl	test_pit_timer
	.cfi_def_cfa_offset 8
	pushl	%eax
	.cfi_def_cfa_offset 12
	pushl	$.LC466
	.cfi_def_cfa_offset 16
	call	_printf
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE403:
	.size	Command_counter, .-Command_counter
	.align 16
	.globl	Command_trate
	.type	Command_trate, @function
Command_trate:
.LFB404:
	.cfi_startproc
/APP
/  208 "Source/Shell/Commands/Default_Commands.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	test_counter, %eax
	movl	HPET_main_counter, %edx
	pushl	(%edx)
	.cfi_def_cfa_offset 8
	pushl	%eax
	.cfi_def_cfa_offset 12
	pushl	$.LC437
	.cfi_def_cfa_offset 16
	call	_printf
	movl	HPET_main_counter, %eax
	movl	$0, test_counter
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	movl	$0, (%eax)
	movl	hpet, %eax
	movl	$0, 240(%eax)
	movl	$0, 244(%eax)
	ret
	.cfi_endproc
.LFE404:
	.size	Command_trate, .-Command_trate
	.section	.rodata
.LC467:
	.string	"%x"
	.text
	.align 16
	.globl	Command_timeslice
	.type	Command_timeslice, @function
Command_timeslice:
.LFB405:
	.cfi_startproc
/APP
/  220 "Source/Shell/Commands/Default_Commands.c" 1
	cli
/  0 "" 2
/NO_APP
	call	Get_Scheduler
	pushl	52(%eax)
	.cfi_def_cfa_offset 8
	pushl	$.LC467
	.cfi_def_cfa_offset 12
	call	_printf
	popl	%eax
	.cfi_def_cfa_offset 8
	popl	%edx
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE405:
	.size	Command_timeslice, .-Command_timeslice
	.align 16
	.globl	Command_topq
	.type	Command_topq, @function
Command_topq:
.LFB406:
	.cfi_startproc
	call	Get_Scheduler
	pushl	16(%eax)
	.cfi_def_cfa_offset 8
	pushl	$.LC467
	.cfi_def_cfa_offset 12
	call	_printf
	popl	%eax
	.cfi_def_cfa_offset 8
	popl	%edx
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE406:
	.size	Command_topq, .-Command_topq
	.section	.rodata
.LC468:
	.string	"Test_process"
	.text
	.align 16
	.globl	Command_test
	.type	Command_test, @function
Command_test:
.LFB407:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
/APP
/  233 "Source/Shell/Commands/Default_Commands.c" 1
	cli
/  0 "" 2
/NO_APP
	call	Get_Scheduler
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	kernel_proc
	.cfi_def_cfa_offset 32
	movl	%eax, %ebx
	pushl	$514
	.cfi_def_cfa_offset 36
	pushl	$10
	.cfi_def_cfa_offset 40
	pushl	$test_process
	.cfi_def_cfa_offset 44
	pushl	$.LC468
	.cfi_def_cfa_offset 48
	call	create_task
	addl	$24, %esp
	.cfi_def_cfa_offset 24
	pushl	%ebx
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	Activate_task_direct_SP
/APP
/  246 "Source/Shell/Commands/Default_Commands.c" 1
	int $50
/  0 "" 2
/NO_APP
	addl	$24, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE407:
	.size	Command_test, .-Command_test
	.align 16
	.globl	Command_test_multi
	.type	Command_test_multi, @function
Command_test_multi:
.LFB408:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
/APP
/  252 "Source/Shell/Commands/Default_Commands.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	$5, %ebx
.L3882:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	kernel_proc
	.cfi_def_cfa_offset 32
	pushl	$514
	.cfi_def_cfa_offset 36
	pushl	$10
	.cfi_def_cfa_offset 40
	pushl	$test_process
	.cfi_def_cfa_offset 44
	pushl	$.LC468
	.cfi_def_cfa_offset 48
	call	create_task
	addl	$20, %esp
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	Activate_task_direct
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	subl	$1, %ebx
	jne	.L3882
	call	Shell_sleep
/APP
/  258 "Source/Shell/Commands/Default_Commands.c" 1
	int $50
/  0 "" 2
/NO_APP
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE408:
	.size	Command_test_multi, .-Command_test_multi
	.section	.rodata
.LC469:
	.string	"\n%s\n"
	.align 4
.LC470:
	.string	"\nsizeof(File_Header_t) %x, sizeof(File_t) %x\n"
.LC472:
	.string	"\nWritten Successfully"
.LC473:
	.string	"\nThe file reads as: %s\n"
	.align 4
.LC471:
	.byte	0x48,0x65,0x6c,0x6c,0x6f,0x21,0x21,0x21,0x20,0x54,0x68,0x69,0x73
	.byte	0x20,0x69,0x73,0x20,0x41,0x71,0x65,0x6f,0x75,0x73,0x20,0x4f,0x53
	.byte	0x20,0x53,0x70,0x65,0x61,0x6b,0x69,0x6e,0x67,0x20,0x41,0x6e,0x64
	.byte	0x20,0x49,0x20,0x61,0x6d,0x20,0x54,0x65,0x73,0x74,0x69,0x6e,0x67
	.byte	0x20,0x74,0x68,0x65,0x20,0x46,0x69,0x6c,0x65,0x73,0x79,0x73,0x74
	.byte	0x65,0x6d,0x2e,0x20,0x49,0x20,0x6e,0x65,0x65,0x64,0x20,0x61,0x20
	.byte	0x72,0x65,0x61,0x6c,0x6c,0x79,0x20,0x76,0x65,0x72,0x79,0x20,0x76
	.byte	0x65,0x72,0x79,0x20,0x76,0x65,0x72,0x79,0x20,0x62,0x69,0x67,0x20
	.byte	0x6d,0x65,0x73,0x73,0x61,0x67,0x65,0x20,0x74,0x6f,0x20,0x74,0x65
	.byte	0x73,0x74,0x20,0x74,0x68,0x65,0x20,0x49,0x2f,0x4f,0x20,0x6f,0x70
	.byte	0x65,0x72,0x61,0x74,0x69,0x6f,0x6e,0x73,0x20,0x6f,0x66,0x20,0x6d
	.byte	0x79,0x20,0x66,0x69,0x6c,0x65,0x20,0x73,0x79,0x73,0x74,0x65,0x6d
	.byte	0x27,0x73,0x20,0x66,0x69,0x6c,0x65,0x20,0x68,0x61,0x6e,0x64,0x6c
	.byte	0x69,0x6e,0x67,0x2e,0x20,0x53,0x6f,0x20,0x70,0x6c,0x65,0x61,0x73
	.byte	0x65,0x20,0x6c,0x65,0x74,0x20,0x6d,0x65,0x20,0x64,0x6f,0x20,0x77
	.byte	0x68,0x61,0x74,0x20,0x49,0x20,0x77,0x61,0x6e,0x74,0x2e,0x20,0x54
	.byte	0x68,0x69,0x73,0x20,0x69,0x73,0x20,0x61,0x20,0x63,0x6f,0x6d,0x70
	.byte	0x6c,0x65,0x74,0x65,0x6c,0x79,0x20,0x6e,0x65,0x77,0x20,0x46,0x69
	.byte	0x6c,0x65,0x53,0x79,0x73,0x74,0x65,0x6d,0x20,0x61,0x6e,0x64,0x20
	.byte	0x74,0x68,0x75,0x73,0x20,0x6e,0x65,0x65,0x64,0x73,0x20,0x61,0x20
	.byte	0x6c,0x6f,0x74,0x20,0x6f,0x66,0x20,0x74,0x65,0x73,0x74,0x69,0x6e
	.byte	0x67,0x2e,0x20,0x54,0x68,0x65,0x72,0x65,0x20,0x61,0x72,0x65,0x20
	.byte	0x53,0x65,0x76,0x65,0x72,0x61,0x6c,0x20,0x62,0x75,0x67,0x73,0x20
	.byte	0x61,0x6e,0x64,0x20,0x49,0x20,0x61,0x6d,0x20,0x67,0x6f,0x6e,0x6e
	.byte	0x61,0x20,0x66,0x69,0x78,0x20,0x74,0x68,0x65,0x6d,0x20,0x61,0x6c
	.byte	0x6c,0x21,0x21,0x21,0x20,0x41,0x73,0x20,0x49,0x20,0x53,0x61,0x69
	.byte	0x64,0x20,0x74,0x68,0x61,0x74,0x20,0x49,0x20,0x6e,0x65,0x65,0x64
	.byte	0x20,0x61,0x20,0x72,0x65,0x61,0x6c,0x6c,0x79,0x20,0x76,0x65,0x72
	.byte	0x79,0x20,0x6c,0x6f,0x6e,0x67,0x20,0x6d,0x65,0x73,0x73,0x61,0x67
	.byte	0x65,0x20,0x74,0x6f,0x20,0x74,0x65,0x73,0x74,0x20,0x77,0x65,0x74
	.byte	0x68,0x65,0x72,0x20,0x74,0x68,0x65,0x20,0x66,0x69,0x6c,0x65,0x20
	.byte	0x68,0x61,0x6e,0x64,0x6c,0x69,0x6e,0x67,0x20,0x63,0x61,0x6e,0x20
	.byte	0x61,0x63,0x74,0x75,0x61,0x6c,0x6c,0x79,0x20,0x6d,0x61,0x6e,0x61
	.byte	0x67,0x65,0x20,0x72,0x65,0x61,0x64,0x69,0x6e,0x67,0x20,0x77,0x72
	.byte	0x69,0x74,0x69,0x6e,0x67,0x20,0x6f,0x70,0x65,0x72,0x61,0x74,0x69
	.byte	0x6f,0x6e,0x73,0x20,0x77,0x69,0x74,0x68,0x20,0x64,0x61,0x74,0x61
	.byte	0x20,0x62,0x69,0x67,0x67,0x65,0x72,0x20,0x74,0x68,0x65,0x6e,0x20
	.byte	0x74,0x68,0x65,0x20,0x73,0x65,0x63,0x74,0x6f,0x72,0x20,0x73,0x69
	.byte	0x7a,0x65,0x2c,0x20,0x74,0x68,0x61,0x74,0x20,0x69,0x73,0x20,0x35
	.byte	0x31,0x32,0x20,0x62,0x79,0x74,0x65,0x73,0x2c,0x20,0x73,0x6f,0x20
	.byte	0x74,0x68,0x61,0x74,0x73,0x20,0x77,0x68,0x79,0x20,0x49,0x20,0x61
	.byte	0x6d,0x20,0x77,0x72,0x69,0x74,0x69,0x6e,0x67,0x20,0x74,0x68,0x69
	.byte	0x73,0x20,0x6c,0x6f,0x6e,0x67,0x20,0x74,0x65,0x73,0x74,0x2e,0x20
	.byte	0x54,0x68,0x65,0x20,0x74,0x65,0x78,0x74,0x20,0x73,0x68,0x6f,0x75
	.byte	0x6c,0x64,0x20,0x62,0x65,0x20,0x6c,0x6f,0x6e,0x67,0x65,0x72,0x20
	.byte	0x74,0x68,0x65,0x6e,0x20,0x35,0x31,0x32,0x20,0x62,0x79,0x74,0x65
	.byte	0x73,0x2c,0x20,0x54,0x68,0x61,0x74,0x20,0x69,0x73,0x20,0x74,0x68
	.byte	0x65,0x20,0x74,0x65,0x78,0x74,0x20,0x73,0x68,0x6f
	.string	"uld contains 512 characters. I am writing this in block alignmnet to manage space :3 This filesystem can be potentially better then FAT and so Its necessary to remove allthe bugs before releasing it. Thats obvious LOL xD .So please be patient. Thank you :)"
	.text
	.align 16
	.globl	Command_testfs
	.type	Command_testfs, @function
Command_testfs:
.LFB409:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$904, %esp
	.cfi_def_cfa_offset 924
/APP
/  263 "Source/Shell/Commands/Default_Commands.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	Main_CSI_struct, %eax
	movl	8(%eax), %ebx
	pushl	%ebx
	.cfi_def_cfa_offset 928
	call	file_loadOGP
	popl	%ecx
	.cfi_def_cfa_offset 924
	popl	%esi
	.cfi_def_cfa_offset 920
	pushl	%ebx
	.cfi_def_cfa_offset 924
	pushl	$.LC469
	.cfi_def_cfa_offset 928
	movl	$.LC471, %esi
	call	printf
	addl	$12, %esp
	.cfi_def_cfa_offset 916
	pushl	$162
	.cfi_def_cfa_offset 920
	pushl	$48
	.cfi_def_cfa_offset 924
	pushl	$.LC470
	.cfi_def_cfa_offset 928
	call	printf
	movl	.LC471, %eax
	leal	18(%esp), %ebp
	leal	20(%esp), %edi
	movl	%ebp, %ecx
	movl	%eax, 18(%esp)
	movl	.LC471+874, %eax
	subl	%edi, %ecx
	subl	%ecx, %esi
	addl	$878, %ecx
	shrl	$2, %ecx
	movl	%eax, 892(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 912
	xorl	%eax, %eax
	rep movsl
	cmpb	$0, 2(%esp)
	je	.L3886
	.align 16
.L3887:
	addl	$1, %eax
	cmpb	$0, 0(%ebp,%eax)
	jne	.L3887
.L3886:
	subl	$8, %esp
	.cfi_def_cfa_offset 920
	pushl	%eax
	.cfi_def_cfa_offset 924
	pushl	$.LC467
	.cfi_def_cfa_offset 928
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 912
	xorl	%eax, %eax
	cmpb	$0, 2(%esp)
	je	.L3888
	.align 16
.L3889:
	addl	$1, %eax
	cmpb	$0, 0(%ebp,%eax)
	jne	.L3889
.L3888:
	subl	$4, %esp
	.cfi_def_cfa_offset 916
	pushl	%ebx
	.cfi_def_cfa_offset 920
	pushl	%eax
	.cfi_def_cfa_offset 924
	pushl	%ebp
	.cfi_def_cfa_offset 928
	call	file_writeAppend
	addl	$16, %esp
	.cfi_def_cfa_offset 912
	testl	%eax, %eax
	jne	.L3890
	subl	$12, %esp
	.cfi_def_cfa_offset 924
	pushl	$.LC472
	.cfi_def_cfa_offset 928
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 912
.L3890:
	subl	$12, %esp
	.cfi_def_cfa_offset 924
	pushl	%ebx
	.cfi_def_cfa_offset 928
	call	file_flushOGP
	movl	$8192, (%esp)
	call	pmem
	pushl	%ebx
	.cfi_def_cfa_offset 932
	pushl	$2048
	.cfi_def_cfa_offset 936
	movl	%eax, %esi
	pushl	$0
	.cfi_def_cfa_offset 940
	pushl	%eax
	.cfi_def_cfa_offset 944
	call	file_readTM
	addl	$20, %esp
	.cfi_def_cfa_offset 924
	pushl	%ebx
	.cfi_def_cfa_offset 928
	call	file_closeOGP
	popl	%eax
	.cfi_def_cfa_offset 924
	popl	%edx
	.cfi_def_cfa_offset 920
	pushl	%esi
	.cfi_def_cfa_offset 924
	pushl	$.LC473
	.cfi_def_cfa_offset 928
	call	printf
	addl	$908, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE409:
	.size	Command_testfs, .-Command_testfs
	.align 16
	.globl	Command_secalloc
	.type	Command_secalloc, @function
Command_secalloc:
.LFB410:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$1
	.cfi_def_cfa_offset 32
	call	sec_alloc
	addl	$12, %esp
	.cfi_def_cfa_offset 20
	pushl	%edx
	.cfi_def_cfa_offset 24
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC376
	.cfi_def_cfa_offset 32
	call	printf
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE410:
	.size	Command_secalloc, .-Command_secalloc
	.align 16
	.globl	Command_clrscr
	.type	Command_clrscr, @function
Command_clrscr:
.LFB411:
	.cfi_startproc
/APP
/  304 "Source/Shell/Commands/Default_Commands.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	console_dbuffer_original, %eax
	leal	4194304(%eax), %edx
	movl	%eax, console_dbuffer
	.align 16
.L3899:
	addl	$4, %eax
	movl	$0, -4(%eax)
	cmpl	%edx, %eax
	jne	.L3899
	movl	$0, console_rows_skipped
	movl	$0, consolerow
	movl	$0, consolecolumn
	movl	$0, console_skip
/APP
/  311 "Source/Shell/Commands/Default_Commands.c" 1
	sti
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE411:
	.size	Command_clrscr, .-Command_clrscr
	.align 16
	.globl	Command_baseln
	.type	Command_baseln, @function
Command_baseln:
.LFB412:
	.cfi_startproc
	call	Get_Scheduler
	movl	28(%eax), %eax
	pushl	(%eax)
	.cfi_def_cfa_offset 8
	pushl	$.LC376
	.cfi_def_cfa_offset 12
	call	_printf
	popl	%eax
	.cfi_def_cfa_offset 8
	popl	%edx
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE412:
	.size	Command_baseln, .-Command_baseln
	.section	.rodata
.LC474:
	.string	"\t%s"
	.text
	.align 16
	.globl	Command_baseshow
	.type	Command_baseshow, @function
Command_baseshow:
.LFB413:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
/APP
/  322 "Source/Shell/Commands/Default_Commands.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	Main_CSI_struct, %eax
	movl	8(%eax), %eax
	testl	%eax, %eax
	je	.L3907
	pushl	%eax
	.cfi_def_cfa_offset 20
	call	StrToInt
	popl	%edx
	.cfi_def_cfa_offset 16
	imull	$76, %eax, %esi
.L3903:
	addl	KitList, %esi
	movl	28(%esi), %eax
	pushl	(%eax)
	.cfi_def_cfa_offset 20
	pushl	$.LC376
	.cfi_def_cfa_offset 24
	call	_printf
	movl	28(%esi), %edi
	popl	%ebx
	.cfi_def_cfa_offset 20
	popl	%eax
	.cfi_def_cfa_offset 16
	movl	(%edi), %eax
	xorl	%ebx, %ebx
	testl	%eax, %eax
	je	.L3902
	.align 16
.L3908:
	movl	4(%edi,%ebx,4), %eax
	addl	$1, %ebx
	addl	$52, %eax
	pushl	%eax
	.cfi_def_cfa_offset 20
	pushl	$.LC474
	.cfi_def_cfa_offset 24
	call	_printf
	movl	28(%esi), %eax
	popl	%edx
	.cfi_def_cfa_offset 20
	popl	%ecx
	.cfi_def_cfa_offset 16
	cmpl	%ebx, (%eax)
	ja	.L3908
.L3902:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3907:
	.cfi_restore_state
	xorl	%esi, %esi
	jmp	.L3903
	.cfi_endproc
.LFE413:
	.size	Command_baseshow, .-Command_baseshow
	.align 16
	.globl	Command_qelements
	.type	Command_qelements, @function
Command_qelements:
.LFB414:
	.cfi_startproc
	movl	kb_q_elements, %eax
	pushl	%eax
	.cfi_def_cfa_offset 8
	pushl	$.LC376
	.cfi_def_cfa_offset 12
	call	_printf
	popl	%eax
	.cfi_def_cfa_offset 8
	popl	%edx
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE414:
	.size	Command_qelements, .-Command_qelements
	.align 16
	.globl	Command_dbuffplusplus
	.type	Command_dbuffplusplus, @function
Command_dbuffplusplus:
.LFB415:
	.cfi_startproc
	addl	$160, console_dbuffer
	ret
	.cfi_endproc
.LFE415:
	.size	Command_dbuffplusplus, .-Command_dbuffplusplus
	.section	.rodata
.LC475:
	.string	"\nSSE is Available"
	.text
	.align 16
	.globl	Command_detect_cpu
	.type	Command_detect_cpu, @function
Command_detect_cpu:
.LFB416:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	detect_cpu
	call	check_SSE
	movl	SSE_available, %eax
	testl	%eax, %eax
	je	.L3913
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC475
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L3913:
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE416:
	.size	Command_detect_cpu, .-Command_detect_cpu
	.section	.rodata
.LC476:
	.string	"\nNumber generated: %x\n"
.LC477:
	.string	"\n THIS COMMAND EXISTS\n"
	.align 4
.LC478:
	.string	"\n This Command does not exist\n"
	.text
	.align 16
	.globl	Command_Numerise
	.type	Command_Numerise, @function
Command_Numerise:
.LFB417:
	.cfi_startproc
	movl	Main_CSI_struct, %eax
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	8(%eax), %esi
	movzbl	(%esi), %eax
	testb	%al, %al
	je	.L3917
	movl	$1, %edx
	xorl	%ebx, %ebx
	jmp	.L3920
	.align 16
.L3934:
	subl	$97, %eax
	imull	%edx, %eax
	addl	$1, %edx
	addl	%eax, %ebx
	movzbl	-1(%edx,%esi), %eax
	cmpl	$17, %ecx
	je	.L3925
.L3935:
	testb	%al, %al
	je	.L3925
.L3920:
	cmpb	$96, %al
	movl	%edx, %ecx
	movsbl	%al, %eax
	ja	.L3934
	subl	$65, %eax
	imull	%edx, %eax
	addl	$1, %edx
	addl	%eax, %ebx
	movzbl	-1(%edx,%esi), %eax
	cmpl	$17, %ecx
	jne	.L3935
.L3925:
	pushl	%ebx
	.cfi_def_cfa_offset 16
	pushl	$.LC476
	.cfi_def_cfa_offset 20
	call	_printf
	cmpl	$2048, %ebx
	popl	%esi
	.cfi_def_cfa_offset 16
	popl	%eax
	.cfi_def_cfa_offset 12
	ja	.L3922
	sall	$2, %ebx
.L3924:
	addl	Shell_Commands_list, %ebx
	movl	(%ebx), %eax
	testl	%eax, %eax
	je	.L3922
	pushl	$.LC477
	.cfi_def_cfa_offset 16
	call	_printf
	popl	%ebx
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3922:
	.cfi_restore_state
	pushl	$.LC478
	.cfi_def_cfa_offset 16
	call	_printf
	popl	%ecx
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
.L3917:
	.cfi_restore_state
	pushl	$0
	.cfi_def_cfa_offset 16
	pushl	$.LC476
	.cfi_def_cfa_offset 20
	xorl	%ebx, %ebx
	call	_printf
	popl	%eax
	.cfi_def_cfa_offset 16
	popl	%edx
	.cfi_def_cfa_offset 12
	jmp	.L3924
	.cfi_endproc
.LFE417:
	.size	Command_Numerise, .-Command_Numerise
	.section	.rodata
.LC479:
	.string	"all"
.LC480:
	.string	"start"
.LC481:
	.string	"stop"
.LC482:
	.string	"list"
.LC483:
	.string	"-l"
.LC484:
	.string	"\n%x\t%s\t%d\tthreads: %x\t"
.LC485:
	.string	"\n\t%s\t%d\tpriority: %d\tcore: %d"
	.align 4
.LC486:
	.string	"\nEnter the option:\n1. KILL\n2. SLEEP\n3. WAKE UP\n:"
	.align 4
.LC487:
	.string	"\n%s\t%d\tthreads: %x\t\n\nChild Threads-->"
.LC488:
	.string	"\nEnter the task id: "
	.text
	.align 16
	.globl	Command_proc
	.type	Command_proc, @function
Command_proc:
.LFB418:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$56, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC482
	.cfi_def_cfa_offset 80
	call	CSI_ReadAPS
	movl	%eax, 28(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	je	.L4023
.L3937:
	movl	12(%esp), %edx
	movl	main_prdir, %esi
	movzbl	(%edx), %eax
	movl	%eax, %ebx
	movb	%al, 19(%esp)
	movl	$.LC479, %eax
	cmpb	$97, %bl
	je	.L4002
	jmp	.L3940
	.align 16
.L3943:
	testb	%cl, %cl
	je	.L3941
.L4002:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L3943
.L3940:
	movl	processes, %eax
	movl	%esi, 24(%esp)
	xorl	%ebp, %ebp
	movl	%eax, 20(%esp)
	movl	%esi, %eax
	addl	$4096, %eax
	movl	%eax, 28(%esp)
.L3944:
	movl	24(%esp), %eax
	movl	(%eax), %ebx
	testl	%ebx, %ebx
	je	.L3949
	leal	4096(%ebx), %eax
	movl	%eax, 4(%esp)
	.align 16
.L3957:
	movl	(%ebx), %esi
	testl	%esi, %esi
	je	.L3950
	leal	12(%esi), %eax
	movl	%esi, %edi
	movl	%eax, %edx
	movl	%eax, 8(%esp)
	movzbl	19(%esp), %eax
	cmpb	%al, 12(%esi)
	jne	.L3951
	testb	%al, %al
	je	.L3952
	movl	12(%esp), %eax
	jmp	.L3954
	.align 16
.L3955:
	testb	%cl, %cl
	je	.L3952
.L3954:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L3955
.L3951:
	addl	$1, %ebp
	cmpl	20(%esp), %ebp
	je	.L3938
.L3950:
	addl	$4, %ebx
	cmpl	%ebx, 4(%esp)
	jne	.L3957
.L3949:
	addl	$4, 24(%esp)
	movl	24(%esp), %eax
	cmpl	28(%esp), %eax
	jne	.L3944
	.align 16
.L3938:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC435
	.cfi_def_cfa_offset 80
	call	CSI_ReadAPS
	movzbl	(%eax), %ebx
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	%eax, %ecx
	movl	$.LC480, %edx
	cmpb	$115, %bl
	je	.L3993
	jmp	.L4024
	.align 16
.L3962:
	testb	%bl, %bl
	je	.L3936
.L3993:
	addl	$1, %ecx
	addl	$1, %edx
	movzbl	(%ecx), %ebx
	cmpb	(%edx), %bl
	je	.L3962
	movl	$.LC481, %edx
	jmp	.L3965
	.align 16
.L3966:
	testb	%cl, %cl
	je	.L3936
.L3965:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L3966
.L3936:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L3952:
	.cfi_restore_state
	pushl	56(%esi)
	.cfi_def_cfa_offset 68
	pushl	36(%esi)
	.cfi_def_cfa_offset 72
	xorl	%ebp, %ebp
	pushl	16(%esp)
	.cfi_def_cfa_offset 76
	pushl	$.LC487
	.cfi_def_cfa_offset 80
	call	printf
	movl	56(%esi), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	48(%esi), %ebx
	testl	%ecx, %ecx
	je	.L3938
.L3994:
	movl	44(%ebx), %eax
	subl	MotherSpace, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	sarl	$2, %eax
	leal	15(%eax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	addl	$1, %ebp
	sarl	$4, %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	leal	52(%ebx), %eax
	pushl	16(%ebx)
	.cfi_def_cfa_offset 84
	pushl	40(%ebx)
	.cfi_def_cfa_offset 88
	pushl	%eax
	.cfi_def_cfa_offset 92
	pushl	$.LC485
	.cfi_def_cfa_offset 96
	call	printf
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	cmpl	%ebp, 56(%esi)
	movl	102(%ebx), %ebx
	ja	.L3994
	jmp	.L3938
.L3941:
	movl	%esi, %eax
	movl	%esi, 4(%esp)
	xorl	%ebp, %ebp
	addl	$4096, %eax
	movl	%eax, 8(%esp)
.L3948:
	movl	4(%esp), %eax
	movl	(%eax), %esi
	testl	%esi, %esi
	je	.L3945
	xorl	%ebx, %ebx
	jmp	.L3947
	.align 16
.L3946:
	addl	$1, %ebx
	cmpl	$1024, %ebx
	je	.L3945
.L3947:
	movl	(%esi,%ebx,4), %eax
	testl	%eax, %eax
	je	.L3946
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	%eax, %edi
	pushl	56(%eax)
	.cfi_def_cfa_offset 80
	leal	12(%eax), %eax
	pushl	24(%eax)
	.cfi_def_cfa_offset 84
	addl	$1, %ebp
	pushl	%eax
	.cfi_def_cfa_offset 88
	pushl	%ebx
	.cfi_def_cfa_offset 92
	pushl	$.LC484
	.cfi_def_cfa_offset 96
	call	printf
	addl	$32, %esp
	.cfi_def_cfa_offset 64
	cmpl	processes, %ebp
	je	.L3938
	addl	$1, %ebx
	cmpl	$1024, %ebx
	jne	.L3947
	.align 16
.L3945:
	addl	$4, 4(%esp)
	movl	4(%esp), %eax
	cmpl	8(%esp), %eax
	jne	.L3948
	jmp	.L3938
.L4023:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC483
	.cfi_def_cfa_offset 80
	call	CSI_ReadAPS
	movl	%eax, 28(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	je	.L3938
	jmp	.L3937
	.align 16
.L4024:
	cmpb	$116, %bl
	jne	.L3936
	cmpb	$0, 1(%eax)
	jne	.L3936
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC488
	.cfi_def_cfa_offset 80
	call	printf
	movl	$8, (%esp)
	call	*kmalloc
	movl	%eax, %ebx
	popl	%eax
	.cfi_def_cfa_offset 76
	popl	%edx
	.cfi_def_cfa_offset 72
	pushl	$8
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	kb_getline
	movl	%ebx, (%esp)
	call	StrToInt
	movl	56(%edi), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	48(%edi), %ebx
	testl	%ecx, %ecx
	je	.L3936
	xorl	%edx, %edx
	cmpl	40(%ebx), %eax
	jne	.L3970
	jmp	.L3971
	.align 16
.L3974:
	cmpl	40(%ebx), %eax
	je	.L3971
.L3970:
	addl	$1, %edx
	movl	102(%ebx), %ebx
	cmpl	%ecx, %edx
	jne	.L3974
	jmp	.L3936
.L3971:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC486
	.cfi_def_cfa_offset 80
	call	printf
	movl	$8, (%esp)
	call	*kmalloc
	popl	%edi
	.cfi_def_cfa_offset 76
	popl	%ebp
	.cfi_def_cfa_offset 72
	pushl	$8
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	%eax, %esi
	call	kb_getline
	movl	%esi, (%esp)
	call	StrToInt
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	cmpl	$1, %eax
	je	.L4025
	cmpl	$2, %eax
	je	.L4026
	cmpl	$3, %eax
	jne	.L3936
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	Task_wakeup
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L3936
.L4025:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	_kill
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L3936
.L4026:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	Task_sleep
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L3936
	.cfi_endproc
.LFE418:
	.size	Command_proc, .-Command_proc
	.comm	VGA_size,4,4
	.comm	aLocked,4,4
	.section	.rodata
.LC489:
	.string	"shell_cmdTask"
	.text
	.align 16
	.globl	init_shell
	.type	init_shell, @function
init_shell:
.LFB419:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	movl	console_dbuffer, %eax
	movl	$0, consolerow
	movl	$0, consolecolumn
	leal	4194304(%eax), %edx
	.align 16
.L4028:
	addl	$4, %eax
	movl	$0, -4(%eax)
	cmpl	%edx, %eax
	jne	.L4028
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	$1000, VGA_size
	pushl	$4096
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	$8192, (%esp)
	movl	%eax, shell_buf
	call	*kmalloc
	movl	$4194304, (%esp)
	movl	%eax, Shell_Commands_list
	call	*kmalloc
	movl	%eax, Input_stream
	movl	%eax, Shell_Istream
	addl	$4194304, %eax
	movl	%eax, Istream_end
	movl	Input_stream, %eax
	movl	$4096, (%esp)
	movl	%eax, Istream_ptr
	call	*kmalloc
	movl	%eax, Shell_Istream
	movl	CSI_mem_start, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	leal	72(%eax), %edx
	.align 16
.L4029:
	addl	$4, %eax
	movl	$0, -4(%eax)
	cmpl	%edx, %eax
	jne	.L4029
	movl	Main_CSI_struct, %eax
	leal	8(%eax), %edx
	addl	$4, %eax
	movl	%eax, tot_entries
	movl	VGA_size, %eax
	movl	%edx, CSI_entries_ptr
	leal	0(,%eax,4), %ecx
	movl	console_buffer, %eax
	testl	%ecx, %ecx
	leal	(%eax,%ecx), %edx
	je	.L4032
	.align 16
.L4033:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L4033
.L4032:
	call	Enable_SSE
	call	console_manager_init
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	Shell_proc
	.cfi_def_cfa_offset 32
	pushl	$514
	.cfi_def_cfa_offset 36
	pushl	$16
	.cfi_def_cfa_offset 40
	pushl	$Shell_cmdWrapper
	.cfi_def_cfa_offset 44
	pushl	$.LC489
	.cfi_def_cfa_offset 48
	call	create_task
	movl	%eax, shell_cmdTask
	addl	$32, %esp
	.cfi_def_cfa_offset 16
	lock orl	$0, (%esp)
	movl	$0, aLocked
	lock orl	$0, (%esp)
	movl	$0, SHELL_CMDTASKLocked
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE419:
	.size	init_shell, .-init_shell
	.align 16
	.globl	Shell_Double_buffer
	.type	Shell_Double_buffer, @function
Shell_Double_buffer:
.LFB420:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
.L4041:
	movl	console_buffer, %eax
	movl	%eax, 12(%esp)
	movl	console_dbuffer, %eax
	movl	%eax, 8(%esp)
	movl	VGA_size, %ecx
	movl	8(%esp), %edx
	movl	12(%esp), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	pushl	%ecx
	.cfi_def_cfa_offset 40
	pushl	%edx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	memcpy_rep
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	console_skip, %eax
	testl	%eax, %eax
	je	.L4040
	movl	$80, %edx
	movl	console_skip, %eax
	imull	%eax, %edx
	movl	console_dbuffer, %eax
	addl	%edx, %edx
	addl	%edx, %eax
	movl	%eax, console_dbuffer
	movl	console_skip, %edx
	movl	consolerow, %eax
	subl	%edx, %eax
	movl	%eax, consolerow
	movl	$0, console_skip
	movl	console_dbuffer, %eax
	movl	%eax, %edx
	movl	console_dbuffer_limit, %eax
	cmpl	%eax, %edx
	jb	.L4040
	movl	console_dbuffer_original, %eax
	movl	%eax, console_dbuffer
	movl	console_dbuffer, %eax
	movl	%eax, 12(%esp)
	movl	console_dbuffer_limit, %edx
	movl	VGA_size, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, 8(%esp)
	movl	VGA_size, %ecx
	movl	8(%esp), %edx
	movl	12(%esp), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	pushl	%ecx
	.cfi_def_cfa_offset 40
	pushl	%edx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	memcpy_rep
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	VGA_size, %eax
	movl	$4194304, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	shrl	$3, %eax
	movl	%eax, %edx
	movl	12(%esp), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	pushl	%edx
	.cfi_def_cfa_offset 40
	pushl	$0
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	memset_sse
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L4040:
/APP
/  79 "Source/Shell/Shell.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L4041
	.cfi_endproc
.LFE420:
	.size	Shell_Double_buffer, .-Shell_Double_buffer
	.section	.rodata
.LC490:
	.string	"ALERT %d %d %d"
	.text
	.align 16
	.globl	Shell_Input
	.type	Shell_Input, @function
Shell_Input:
.LFB421:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
	.align 16
.L4049:
/APP
/  87 "Source/Shell/Shell.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	enter_pressed, %eax
	testl	%eax, %eax
	je	.L4043
	movl	kb_buff, %eax
	testl	%eax, %eax
	je	.L4044
	call	_putchar.constprop.24
	movl	Istream_ptr, %eax
	movb	$0, (%eax)
	movl	kb_q_elements, %eax
	testl	%eax, %eax
	je	.L4045
	movl	Istream_ptr, %eax
	movl	kb_buff, %edx
	movl	%eax, %ebx
	subl	%edx, %ebx
	leal	1(%eax), %edx
	movl	%edx, Istream_ptr
	movl	kb_buff, %edx
	movb	%dl, 1(%eax)
	movl	kb_Start_q, %eax
	addl	$1, Istream_ptr
	movzbl	(%ebx), %edx
	movl	(%eax), %ecx
	movl	%ecx, %eax
	.align 16
.L4046:
	addl	$1, %eax
	movb	%dl, -1(%eax)
	movl	%eax, %edx
	subl	%ecx, %edx
	movzbl	(%edx,%ebx), %edx
	testb	%dl, %dl
	jne	.L4046
	movb	$0, (%eax)
	movl	kb_Start_q, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	$0, kb_buff
	movl	8(%eax), %eax
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	Task_wakeup
	movl	kb_Start_q, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, (%eax)
	movl	28(%eax), %edx
	movl	$0, 4(%eax)
	movl	$0, 8(%eax)
	movl	$0, 12(%eax)
	movl	kb_q_elements, %eax
	movl	%edx, kb_Start_q
	subl	$1, %eax
	movl	%eax, kb_q_elements
	movl	(%edx), %eax
	movl	$0, Current_strlen
	movl	%eax, Current_buf
	movl	Istream_ptr, %eax
.L4047:
	leal	1(%eax), %edx
	movl	%edx, Istream_ptr
	movb	$0, 1(%eax)
	addl	$1, Istream_ptr
.L4044:
	movl	$0, enter_pressed
.L4043:
/APP
/  134 "Source/Shell/Shell.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L4049
	.align 16
.L4045:
	movl	shell_awake, %eax
	testl	%eax, %eax
	je	.L4048
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	Shell_task
	.cfi_def_cfa_offset 32
	call	Priority_promoter
	movl	Istream_ptr, %eax
	movl	$1, shell_in
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	kb_buff, %edx
	movl	$0, kb_buff
	movl	%eax, %ecx
	subl	%edx, %ecx
	movl	%ecx, shell_buf
	jmp	.L4047
	.align 16
.L4048:
	movl	shlock, %ecx
	movl	shell_sleeping, %edx
	movl	kb_q_elements, %eax
	pushl	%ecx
	.cfi_def_cfa_offset 20
	pushl	%edx
	.cfi_def_cfa_offset 24
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC490
	.cfi_def_cfa_offset 32
	call	_printf
	movl	Istream_ptr, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L4047
	.cfi_endproc
.LFE421:
	.size	Shell_Input, .-Shell_Input
	.section	.rodata
.LC491:
	.string	"\n->\"%s\""
	.text
	.align 16
	.globl	Shell_CFexecute
	.type	Shell_CFexecute, @function
Shell_CFexecute:
.LFB422:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	52(%esp), %eax
	movl	48(%esp), %esi
	movl	%eax, %edi
	movl	%eax, 12(%esp)
	call	Shell_sleep
	movb	$0, (%esi,%edi)
	movl	tot_entries, %eax
	movl	CSI_mem_start, %ecx
	movl	(%eax), %edx
	addl	$2, %edx
	je	.L4062
	xorl	%eax, %eax
	.align 16
.L4078:
	movl	$0, (%ecx,%eax,4)
	addl	$1, %eax
	cmpl	%eax, %edx
	jne	.L4078
.L4062:
	movl	Main_CSI_struct, %eax
	xorl	%edi, %edi
	addl	$8, %eax
	movl	%eax, CSI_entries_ptr
	movl	12(%esp), %eax
	testl	%eax, %eax
	je	.L4061
	.align 16
.L4076:
	movzbl	(%esi), %eax
	cmpb	$10, %al
	je	.L4071
	testb	%al, %al
	movl	%esi, %ebx
	jne	.L4077
	jmp	.L4071
	.align 16
.L4086:
	testb	%al, %al
	je	.L4070
.L4077:
	addl	$1, %ebx
	movzbl	(%ebx), %eax
	cmpb	$10, %al
	jne	.L4086
.L4070:
	movl	%ebx, %ebp
	subl	%esi, %ebp
	addl	$1, %ebp
.L4068:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	movb	$0, (%ebx)
	addl	%ebp, %edi
	pushl	%esi
	.cfi_def_cfa_offset 60
	pushl	$.LC491
	.cfi_def_cfa_offset 64
	call	printf
	movl	%esi, (%esp)
	call	Shell_command_locator_CC
	movl	Shell_Istream, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%esi, (%eax)
	addl	$4, %eax
	cmpl	%edi, 12(%esp)
	movl	%eax, Shell_Istream
	leal	1(%ebx), %esi
	ja	.L4076
.L4061:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	Shell_wakeup
.L4071:
	.cfi_restore_state
	movl	%esi, %ebx
	movl	$1, %ebp
	jmp	.L4068
	.cfi_endproc
.LFE422:
	.size	Shell_CFexecute, .-Shell_CFexecute
	.section	.rodata
.LC492:
	.string	"\n%g%s%g>%g"
.LC493:
	.string	"%g"
	.text
	.align 16
	.globl	Shell
	.type	Shell, @function
Shell:
.LFB423:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
.L4091:
	movl	shell_awake, %eax
	testl	%eax, %eax
	je	.L4088
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$11
	.cfi_def_cfa_offset 32
	pushl	$10
	.cfi_def_cfa_offset 36
	pushl	$curr_dir+6
	.cfi_def_cfa_offset 40
	pushl	$12
	.cfi_def_cfa_offset 44
	pushl	$.LC492
	.cfi_def_cfa_offset 48
	call	printf
	addl	$32, %esp
	.cfi_def_cfa_offset 16
	jmp	.L4089
.L4090:
/APP
/  189 "Source/Shell/Shell.c" 1
	int $50
/  0 "" 2
/NO_APP
.L4089:
	movl	shell_in, %eax
	testl	%eax, %eax
	je	.L4090
/APP
/  191 "Source/Shell/Shell.c" 1
	cli
/  0 "" 2
/NO_APP
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$15
	.cfi_def_cfa_offset 28
	pushl	$.LC493
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	shell_buf, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	Shell_command_locator
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	Shell_Istream, %eax
	movl	shell_buf, %edx
	movl	%edx, (%eax)
	movl	Shell_Istream, %eax
	addl	$4, %eax
	movl	%eax, Shell_Istream
	movl	$0, shell_in
	movl	$0, shell_buf
	movl	$0, tmpIstream
.L4088:
/APP
/  203 "Source/Shell/Shell.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L4091
	.cfi_endproc
.LFE423:
	.size	Shell, .-Shell
	.section	.rodata
.LC494:
	.string	"->%s"
	.text
	.align 16
	.globl	Shell_scrollUp
	.type	Shell_scrollUp, @function
Shell_scrollUp:
.LFB424:
	.cfi_startproc
	subl	$20, %esp
	.cfi_def_cfa_offset 24
	movl	tmpIstream, %eax
	testl	%eax, %eax
	cmove	Shell_Istream, %eax
	subl	$4, %eax
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC494
	.cfi_def_cfa_offset 32
	movl	%eax, tmpIstream
	movl	%eax, shell_buf
	call	printf
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE424:
	.size	Shell_scrollUp, .-Shell_scrollUp
	.align 16
	.globl	Shell_scrollDown
	.type	Shell_scrollDown, @function
Shell_scrollDown:
.LFB425:
	.cfi_startproc
	movl	Shell_Istream, %eax
	cmpl	%eax, tmpIstream
	je	.L4106
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	movl	shell_buf, %edx
	xorl	%ecx, %ecx
	cmpb	$0, (%edx)
	je	.L4098
	.align 16
.L4107:
	xorl	%eax, %eax
	.align 16
.L4099:
	addl	$1, %eax
	cmpb	$0, (%edx,%eax)
	jne	.L4099
	cmpl	%eax, %ecx
	jnb	.L4098
	call	backspace
	movl	shell_buf, %edx
	addl	$1, %ecx
	cmpb	$0, (%edx)
	jne	.L4107
.L4098:
	movl	tmpIstream, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	addl	$4, %eax
	pushl	%eax
	.cfi_def_cfa_offset 28
	pushl	$.LC276
	.cfi_def_cfa_offset 32
	movl	%eax, tmpIstream
	movl	%eax, shell_buf
	call	printf
	addl	$28, %esp
	.cfi_def_cfa_offset 4
.L4106:
	rep ret
	.cfi_endproc
.LFE425:
	.size	Shell_scrollDown, .-Shell_scrollDown
	.section	.rodata
.LC495:
	.string	"\nAppxx"
.LC496:
	.string	"\nError_537"
	.text
	.align 16
	.globl	Shell_sleep
	.type	Shell_sleep, @function
Shell_sleep:
.LFB426:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	xorl	%ecx, %ecx
	movl	$1, %edx
	.align 16
.L4109:
	movl	%ecx, %eax
	lock cmpxchgl	%edx, aLocked
	jne	.L4109
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	lock orl	$0, (%esp)
	pushl	$.LC495
	.cfi_def_cfa_offset 32
	call	printf
	movl	shell_sleeping, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	je	.L4115
	movl	shell_sleeping, %eax
	testl	%eax, %eax
	js	.L4116
.L4111:
	movl	shell_sleeping, %eax
	addl	$1, %eax
	movl	%eax, shell_sleeping
	lock orl	$0, (%esp)
	movl	$0, aLocked
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
.L4115:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	Shell_task
	.cfi_def_cfa_offset 32
	call	Task_sleep
	movl	$0, shell_awake
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, shell_sleeping
	jmp	.L4111
.L4116:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC496
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  242 "Source/Shell/Shell.c" 1
	hlt
/  0 "" 2
/NO_APP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L4111
	.cfi_endproc
.LFE426:
	.size	Shell_sleep, .-Shell_sleep
	.section	.rodata
.LC497:
	.string	"---IOPPOIOPI--"
	.text
	.align 16
	.globl	Shell_wakeup
	.type	Shell_wakeup, @function
Shell_wakeup:
.LFB427:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	xorl	%ecx, %ecx
	movl	$1, %edx
	.align 16
.L4118:
	movl	%ecx, %eax
	lock cmpxchgl	%edx, aLocked
	jne	.L4118
	lock orl	$0, (%esp)
	movl	shell_sleeping, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC497
	.cfi_def_cfa_offset 32
	subl	$1, %eax
	movl	%eax, shell_sleeping
	call	printf
	movl	shell_sleeping, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	je	.L4124
	movl	shell_sleeping, %eax
	testl	%eax, %eax
	js	.L4125
.L4120:
	lock orl	$0, (%esp)
	movl	$0, aLocked
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
.L4124:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	Shell_task
	.cfi_def_cfa_offset 32
	call	Task_wakeup
	movl	$1, shell_awake
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, shell_sleeping
	jmp	.L4120
.L4125:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC496
	.cfi_def_cfa_offset 32
	call	printf
/APP
/  266 "Source/Shell/Shell.c" 1
	hlt
/  0 "" 2
/NO_APP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L4120
	.cfi_endproc
.LFE427:
	.size	Shell_wakeup, .-Shell_wakeup
	.align 16
	.globl	del_eos
	.type	del_eos, @function
del_eos:
.LFB428:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE428:
	.size	del_eos, .-del_eos
	.globl	shcml
	.section	.bss
	.align 4
	.type	shcml, @object
	.size	shcml, 4
shcml:
	.zero	4
	.text
	.align 16
	.globl	shcmlRemove
	.type	shcmlRemove, @function
shcmlRemove:
.LFB429:
	.cfi_startproc
	movl	$0, shcml
	ret
	.cfi_endproc
.LFE429:
	.size	shcmlRemove, .-shcmlRemove
	.align 16
	.globl	Shell_cmdWrapper
	.type	Shell_cmdWrapper, @function
Shell_cmdWrapper:
.LFB430:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
/APP
/  304 "Source/Shell/Shell.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	$0, shell_awake
	movl	shell_cmdFunc, %eax
	call	*%eax
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	shell_cmdTask
	.cfi_def_cfa_offset 28
	pushl	Shell_task
	.cfi_def_cfa_offset 32
	call	Task_Swap
	popl	%eax
	.cfi_def_cfa_offset 28
	pushl	shell_cmdTask
	.cfi_def_cfa_offset 32
	call	Task_sleep
	movl	$1, shell_awake
/APP
/  312 "Source/Shell/Shell.c" 1
	int $50
/  0 "" 2
/NO_APP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	.align 16
.L4129:
/APP
/  314 "Source/Shell/Shell.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L4129
	.cfi_endproc
.LFE430:
	.size	Shell_cmdWrapper, .-Shell_cmdWrapper
	.globl	space
	.data
	.align 4
	.type	space, @object
	.size	space, 4
space:
	.long	.LC317
	.section	.rodata
	.align 4
.LC498:
	.string	"\n Command Not Recognized! type help for help %x\n"
	.align 4
.LC499:
	.string	"\n Command Not Recognized! type help for help\n"
	.text
	.align 16
	.globl	Shell_command_locator
	.type	Shell_command_locator, @function
Shell_command_locator:
.LFB431:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	pushl	$.LC317
	.cfi_def_cfa_offset 52
	pushl	52(%esp)
	.cfi_def_cfa_offset 56
	call	strtok
	movzbl	(%eax), %edx
	popl	%ecx
	.cfi_def_cfa_offset 52
	popl	%ebx
	.cfi_def_cfa_offset 48
	testb	%dl, %dl
	je	.L4157
	movl	%edx, %ecx
	movl	$1, %ebx
	xorl	%esi, %esi
	jmp	.L4135
	.align 16
.L4174:
	subl	$97, %ecx
	imull	%ebx, %ecx
	addl	%ecx, %esi
	movzbl	(%eax,%ebx), %ecx
	addl	$1, %ebx
	cmpl	$17, %edi
	je	.L4162
.L4175:
	testb	%cl, %cl
	je	.L4162
.L4135:
	cmpb	$96, %cl
	movl	%ebx, %edi
	movsbl	%cl, %ecx
	ja	.L4174
	subl	$65, %ecx
	imull	%ebx, %ecx
	addl	%ecx, %esi
	movzbl	(%eax,%ebx), %ecx
	addl	$1, %ebx
	cmpl	$17, %edi
	jne	.L4175
.L4162:
	cmpl	$2048, %esi
	ja	.L4137
	sall	$2, %esi
.L4132:
	addl	Shell_Commands_list, %esi
	movl	(%esi), %esi
	movl	%esi, %edi
	movl	%esi, 12(%esp)
	movl	%eax, %esi
	leal	8(%edi), %ebx
	jmp	.L4138
	.align 16
.L4177:
	addl	$1, %esi
	movzbl	(%esi), %edx
	addl	$1, %ebx
.L4138:
	movzbl	(%ebx), %ecx
	cmpb	%dl, %cl
	je	.L4140
	movsbl	%cl, %edi
	movsbl	%dl, %edx
	leal	-32(%edi), %ebp
	cmpl	%edx, %ebp
	je	.L4140
	subl	$32, %edx
	cmpl	%edx, %edi
	jne	.L4176
.L4140:
	testb	%cl, %cl
	jne	.L4177
	movl	12(%esp), %eax
	movl	(%eax), %ebx
	movl	tot_entries, %eax
	movl	$0, (%eax)
	jmp	.L4150
	.align 16
.L4143:
	movl	tot_entries, %eax
	addl	$4, %edx
	movl	%edx, CSI_entries_ptr
	addl	$1, (%eax)
.L4150:
	pushl	$.LC317
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	call	strtok
	testl	%eax, %eax
	popl	%esi
	.cfi_def_cfa_offset 52
	popl	%edi
	.cfi_def_cfa_offset 48
	je	.L4142
	movl	CSI_entries_ptr, %edx
	movl	%eax, (%edx)
	cmpb	$34, (%eax)
	jne	.L4143
	addl	$1, %eax
	movl	%eax, (%edx)
	cmpb	$0, (%eax)
	je	.L4146
	.align 16
.L4179:
	xorl	%edx, %edx
	jmp	.L4147
	.align 16
.L4158:
	movl	%ecx, %edx
.L4147:
	cmpb	$0, 1(%eax,%edx)
	leal	1(%edx), %ecx
	jne	.L4158
	cmpb	$34, (%eax,%edx)
	je	.L4178
	addl	$1, %eax
	.align 16
.L4145:
	movl	%eax, %edx
	addl	$1, %eax
	cmpb	$0, -1(%eax)
	jne	.L4145
.L4155:
	movb	$32, (%edx)
	pushl	$.LC317
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	call	strtok
	cmpb	$0, (%eax)
	popl	%edx
	.cfi_def_cfa_offset 52
	popl	%ecx
	.cfi_def_cfa_offset 48
	jne	.L4179
.L4146:
	cmpb	$34, -1(%eax)
	je	.L4180
	movl	%eax, %edx
	jmp	.L4155
.L4178:
	xorl	%edx, %edx
	jmp	.L4149
	.align 16
.L4159:
	movl	%ecx, %edx
.L4149:
	cmpb	$0, 1(%eax,%edx)
	leal	1(%edx), %ecx
	jne	.L4159
	movb	$0, (%eax,%edx)
	movl	CSI_entries_ptr, %edx
	jmp	.L4143
.L4142:
	movl	shell_cmdTask, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	movl	%ebx, shell_cmdFunc
	leal	16364(%eax), %edx
	movl	$0, 36(%eax)
	movl	$514, 16376(%eax)
	movl	$8, 16372(%eax)
	movl	$Shell_cmdWrapper, 16368(%eax)
	movl	%edx, 16348(%eax)
	movl	%edx, 16344(%eax)
	leal	16336(%eax), %edx
	movl	$0, 16364(%eax)
	movl	$0, 16360(%eax)
	movl	$0, 16356(%eax)
	movl	$0, 16352(%eax)
	movl	$0, 16340(%eax)
	movl	$0, 16336(%eax)
	movl	%edx, (%eax)
	pushl	Shell_task
	.cfi_def_cfa_offset 60
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	Task_Swap
/APP
/  395 "Source/Shell/Shell.c" 1
	int $50
/  0 "" 2
/NO_APP
	movl	tot_entries, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	CSI_mem_start, %ecx
	movl	(%eax), %edx
	xorl	%eax, %eax
	addl	$2, %edx
	je	.L4152
	.align 16
.L4165:
	movl	$0, (%ecx,%eax,4)
	addl	$1, %eax
	cmpl	%eax, %edx
	jne	.L4165
.L4152:
	movl	Main_CSI_struct, %eax
	addl	$8, %eax
	movl	%eax, CSI_entries_ptr
	movl	12(%esp), %eax
	movl	4(%eax), %eax
.L4131:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L4176:
	.cfi_restore_state
	pushl	%eax
	.cfi_def_cfa_offset 52
	pushl	$.LC498
	.cfi_def_cfa_offset 56
	call	_printf
	popl	%ebp
	.cfi_def_cfa_offset 52
	popl	%eax
	.cfi_def_cfa_offset 48
	movl	$-1, %eax
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L4180:
	.cfi_restore_state
	movl	$-1, %edx
	movb	$0, (%eax,%edx)
	movl	CSI_entries_ptr, %edx
	jmp	.L4143
.L4157:
	xorl	%esi, %esi
	jmp	.L4132
.L4137:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC499
	.cfi_def_cfa_offset 64
	call	_printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	$-1, %eax
	jmp	.L4131
	.cfi_endproc
.LFE431:
	.size	Shell_command_locator, .-Shell_command_locator
	.align 16
	.globl	Shell_command_locator_CC
	.type	Shell_command_locator_CC, @function
Shell_command_locator_CC:
.LFB432:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	pushl	$.LC317
	.cfi_def_cfa_offset 52
	pushl	52(%esp)
	.cfi_def_cfa_offset 56
	call	strtok
	movzbl	(%eax), %edx
	popl	%ecx
	.cfi_def_cfa_offset 52
	popl	%ebx
	.cfi_def_cfa_offset 48
	testb	%dl, %dl
	je	.L4207
	movl	%edx, %ecx
	movl	$1, %ebx
	xorl	%esi, %esi
	jmp	.L4185
	.align 16
.L4224:
	subl	$97, %ecx
	imull	%ebx, %ecx
	addl	%ecx, %esi
	movzbl	(%eax,%ebx), %ecx
	addl	$1, %ebx
	cmpl	$17, %edi
	je	.L4212
.L4225:
	testb	%cl, %cl
	je	.L4212
.L4185:
	cmpb	$96, %cl
	movl	%ebx, %edi
	movsbl	%cl, %ecx
	ja	.L4224
	subl	$65, %ecx
	imull	%ebx, %ecx
	addl	%ecx, %esi
	movzbl	(%eax,%ebx), %ecx
	addl	$1, %ebx
	cmpl	$17, %edi
	jne	.L4225
.L4212:
	cmpl	$2048, %esi
	ja	.L4187
	sall	$2, %esi
.L4182:
	addl	Shell_Commands_list, %esi
	movl	(%esi), %esi
	movl	%esi, %edi
	movl	%esi, 12(%esp)
	movl	%eax, %esi
	leal	8(%edi), %ebx
	jmp	.L4188
	.align 16
.L4227:
	addl	$1, %esi
	movzbl	(%esi), %edx
	addl	$1, %ebx
.L4188:
	movzbl	(%ebx), %ecx
	cmpb	%dl, %cl
	je	.L4190
	movsbl	%cl, %edi
	movsbl	%dl, %edx
	leal	-32(%edi), %ebp
	cmpl	%edx, %ebp
	je	.L4190
	subl	$32, %edx
	cmpl	%edx, %edi
	jne	.L4226
.L4190:
	testb	%cl, %cl
	jne	.L4227
	movl	12(%esp), %eax
	movl	(%eax), %ebx
	movl	tot_entries, %eax
	movl	$0, (%eax)
	jmp	.L4200
	.align 16
.L4193:
	movl	tot_entries, %eax
	addl	$4, %edx
	movl	%edx, CSI_entries_ptr
	addl	$1, (%eax)
.L4200:
	pushl	$.LC317
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	call	strtok
	testl	%eax, %eax
	popl	%esi
	.cfi_def_cfa_offset 52
	popl	%edi
	.cfi_def_cfa_offset 48
	je	.L4192
	movl	CSI_entries_ptr, %edx
	movl	%eax, (%edx)
	cmpb	$34, (%eax)
	jne	.L4193
	addl	$1, %eax
	movl	%eax, (%edx)
	cmpb	$0, (%eax)
	je	.L4196
	.align 16
.L4229:
	xorl	%edx, %edx
	jmp	.L4197
	.align 16
.L4208:
	movl	%ecx, %edx
.L4197:
	cmpb	$0, 1(%eax,%edx)
	leal	1(%edx), %ecx
	jne	.L4208
	cmpb	$34, (%eax,%edx)
	je	.L4228
	addl	$1, %eax
	.align 16
.L4195:
	movl	%eax, %edx
	addl	$1, %eax
	cmpb	$0, -1(%eax)
	jne	.L4195
.L4205:
	movb	$32, (%edx)
	pushl	$.LC317
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	call	strtok
	cmpb	$0, (%eax)
	popl	%edx
	.cfi_def_cfa_offset 52
	popl	%ecx
	.cfi_def_cfa_offset 48
	jne	.L4229
.L4196:
	cmpb	$34, -1(%eax)
	je	.L4230
	movl	%eax, %edx
	jmp	.L4205
.L4228:
	xorl	%edx, %edx
	jmp	.L4199
	.align 16
.L4209:
	movl	%ecx, %edx
.L4199:
	cmpb	$0, 1(%eax,%edx)
	leal	1(%edx), %ecx
	jne	.L4209
	movb	$0, (%eax,%edx)
	movl	CSI_entries_ptr, %edx
	jmp	.L4193
.L4192:
	call	*%ebx
	movl	tot_entries, %eax
	movl	CSI_mem_start, %ecx
	movl	(%eax), %edx
	xorl	%eax, %eax
	addl	$2, %edx
	je	.L4202
	.align 16
.L4215:
	movl	$0, (%ecx,%eax,4)
	addl	$1, %eax
	cmpl	%eax, %edx
	jne	.L4215
.L4202:
	movl	Main_CSI_struct, %eax
	addl	$8, %eax
	movl	%eax, CSI_entries_ptr
	movl	12(%esp), %eax
	movl	4(%eax), %eax
.L4181:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L4226:
	.cfi_restore_state
	pushl	%eax
	.cfi_def_cfa_offset 52
	pushl	$.LC498
	.cfi_def_cfa_offset 56
	call	_printf
	popl	%ebp
	.cfi_def_cfa_offset 52
	popl	%eax
	.cfi_def_cfa_offset 48
	movl	$-1, %eax
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L4230:
	.cfi_restore_state
	movl	$-1, %edx
	movb	$0, (%eax,%edx)
	movl	CSI_entries_ptr, %edx
	jmp	.L4193
.L4207:
	xorl	%esi, %esi
	jmp	.L4182
.L4187:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC499
	.cfi_def_cfa_offset 64
	call	_printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	$-1, %eax
	jmp	.L4181
	.cfi_endproc
.LFE432:
	.size	Shell_command_locator_CC, .-Shell_command_locator_CC
	.align 16
	.globl	Shell_Add_Commands
	.type	Shell_Add_Commands, @function
Shell_Add_Commands:
.LFB433:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$20, %esp
	.cfi_def_cfa_offset 28
	movl	40(%esp), %ebx
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	32(%esp), %edx
	movl	36(%esp), %ecx
	movl	%edx, (%eax)
	movl	40(%esp), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%edx, 4(%eax)
	movl	Shell_Commands_list, %edx
	leal	(%edx,%ecx,4), %edx
	movl	%eax, (%edx)
	movzbl	(%ebx), %ecx
	movl	%ebx, %edx
	addl	$8, %eax
	.align 16
.L4232:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4232
	movb	$0, (%eax)
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE433:
	.size	Shell_Add_Commands, .-Shell_Add_Commands
	.align 16
	.globl	CSI_Read
	.type	CSI_Read, @function
CSI_Read:
.LFB434:
	.cfi_startproc
	movl	4(%esp), %edx
	movl	Main_CSI_struct, %eax
	addl	$8, %eax
	testl	%edx, %edx
	je	.L4237
	movl	-4(%eax,%edx,4), %eax
	ret
	.align 16
.L4237:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE434:
	.size	CSI_Read, .-CSI_Read
	.align 16
	.globl	CSI_ReadAPS
	.type	CSI_ReadAPS, @function
CSI_ReadAPS:
.LFB435:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$4, %esp
	.cfi_def_cfa_offset 24
	movl	Main_CSI_struct, %edi
	movl	24(%esp), %ebx
	movl	4(%edi), %eax
	testl	%eax, %eax
	je	.L4246
	movzbl	(%ebx), %eax
	leal	8(%edi), %ebp
	xorl	%esi, %esi
	movb	%al, 3(%esp)
	.align 16
.L4245:
	movl	0(%ebp,%esi,4), %ecx
	movzbl	3(%esp), %eax
	cmpb	(%ecx), %al
	jne	.L4240
	testb	%al, %al
	je	.L4241
	movl	%ebx, %eax
	jmp	.L4243
	.align 16
.L4244:
	testb	%dl, %dl
	je	.L4241
.L4243:
	addl	$1, %eax
	movl	%eax, %edx
	subl	%ebx, %edx
	movzbl	(%edx,%ecx), %edx
	cmpb	(%eax), %dl
	je	.L4244
.L4240:
	movl	4(%edi), %eax
	addl	$2, %esi
	cmpl	%esi, %eax
	ja	.L4245
.L4246:
	addl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4241:
	.cfi_restore_state
	movl	4(%ebp,%esi,4), %eax
	addl	$4, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE435:
	.size	CSI_ReadAPS, .-CSI_ReadAPS
	.section	.rodata
.LC500:
	.string	"help"
.LC501:
	.string	"shutdown"
.LC502:
	.string	"mdbug"
.LC503:
	.string	"vesa"
.LC504:
	.string	"memmap"
.LC505:
	.string	"counter_start"
.LC506:
	.string	"counter"
.LC507:
	.string	"timeslice"
.LC508:
	.string	"topq"
.LC509:
	.string	"test_structs"
.LC510:
	.string	"test"
.LC511:
	.string	"test_multi"
.LC512:
	.string	"ls"
.LC513:
	.string	"cd"
.LC514:
	.string	"clrscr"
.LC515:
	.string	"baseln"
.LC516:
	.string	"baseshow"
.LC517:
	.string	"qelements"
.LC518:
	.string	"dbuffplusplus"
.LC519:
	.string	"decpu"
.LC520:
	.string	"trate"
.LC521:
	.string	"init"
.LC522:
	.string	"numerise"
.LC523:
	.string	"testfs"
.LC524:
	.string	"secalloc"
.LC525:
	.string	"mkdir"
.LC526:
	.string	"mkfl"
.LC527:
	.string	"del"
.LC528:
	.string	"rfl"
.LC529:
	.string	"editfl"
.LC530:
	.string	"cp"
.LC531:
	.string	"aptest"
.LC532:
	.string	"proc"
.LC533:
	.string	"rn"
.LC534:
	.string	"ann"
	.text
	.align 16
	.globl	console_manager_init
	.type	console_manager_init, @function
console_manager_init:
.LFB436:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_help, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$104, %ecx
	movl	%eax, 432(%edx)
	addl	$8, %eax
	movl	$.LC500, %edx
	.align 16
.L4258:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4258
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_shutdown, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$115, %ecx
	movl	%eax, 2100(%edx)
	addl	$8, %eax
	movl	$.LC501, %edx
	.align 16
.L4259:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4259
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_mdbug, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$109, %ecx
	movl	%eax, 524(%edx)
	addl	$8, %eax
	movl	$.LC502, %edx
	.align 16
.L4260:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4260
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_start_vesa, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$118, %ecx
	movl	%eax, 332(%edx)
	addl	$8, %eax
	movl	$.LC503, %edx
	.align 16
.L4261:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4261
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_memmap, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$109, %ecx
	movl	%eax, 776(%edx)
	addl	$8, %eax
	movl	$.LC504, %edx
	.align 16
.L4262:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4262
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_start_counter, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$99, %ecx
	movl	%eax, 5692(%edx)
	addl	$8, %eax
	movl	$.LC505, %edx
	.align 16
.L4263:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4263
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_counter, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$99, %ecx
	movl	%eax, 1520(%edx)
	addl	$8, %eax
	movl	$.LC506, %edx
	.align 16
.L4264:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4264
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_timeslice, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$116, %ecx
	movl	%eax, 1404(%edx)
	addl	$8, %eax
	movl	$.LC507, %edx
	.align 16
.L4265:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4265
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_topq, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$116, %ecx
	movl	%eax, 624(%edx)
	addl	$8, %eax
	movl	$.LC508, %edx
	.align 16
.L4266:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4266
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_test_structs, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$116, %ecx
	movl	%eax, 5236(%edx)
	addl	$8, %eax
	movl	$.LC509, %edx
	.align 16
.L4267:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4267
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_test, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$116, %ecx
	movl	%eax, 628(%edx)
	addl	$8, %eax
	movl	$.LC510, %edx
	.align 16
.L4268:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4268
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_test_multi, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$116, %ecx
	movl	%eax, 3432(%edx)
	addl	$8, %eax
	movl	$.LC511, %edx
	.align 16
.L4269:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4269
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_ls, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$108, %ecx
	movl	%eax, 188(%edx)
	addl	$8, %eax
	movl	$.LC512, %edx
	.align 16
.L4270:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4270
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_cd, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$99, %ecx
	movl	%eax, 32(%edx)
	addl	$8, %eax
	movl	$.LC513, %edx
	.align 16
.L4271:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4271
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_clrscr, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$99, %ecx
	movl	%eax, 1036(%edx)
	addl	$8, %eax
	movl	$.LC514, %edx
	.align 16
.L4272:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4272
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_baseln, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$98, %ecx
	movl	%eax, 816(%edx)
	addl	$8, %eax
	movl	$.LC515, %edx
	.align 16
.L4273:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4273
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_baseshow, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$98, %ecx
	movl	%eax, 1908(%edx)
	addl	$8, %eax
	movl	$.LC516, %edx
	.align 16
.L4274:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4274
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_qelements, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$113, %ecx
	movl	%eax, 2248(%edx)
	addl	$8, %eax
	movl	$.LC517, %edx
	.align 16
.L4275:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4275
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_dbuffplusplus, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$100, %ecx
	movl	%eax, 5376(%edx)
	addl	$8, %eax
	movl	$.LC518, %edx
	.align 16
.L4276:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4276
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_detect_cpu, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$100, %ecx
	movl	%eax, 708(%edx)
	addl	$8, %eax
	movl	$.LC519, %edx
	.align 16
.L4277:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4277
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_trate, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$116, %ecx
	movl	%eax, 596(%edx)
	addl	$8, %eax
	movl	$.LC520, %edx
	.align 16
.L4278:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4278
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_init, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$105, %ecx
	movl	%eax, 536(%edx)
	addl	$8, %eax
	movl	$.LC521, %edx
	.align 16
.L4279:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4279
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_Numerise, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$110, %ecx
	movl	%eax, 1584(%edx)
	addl	$8, %eax
	movl	$.LC522, %edx
	.align 16
.L4280:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4280
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_testfs, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$116, %ecx
	movl	%eax, 1160(%edx)
	addl	$8, %eax
	movl	$.LC523, %edx
	.align 16
.L4281:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4281
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_secalloc, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$115, %ecx
	movl	%eax, 1068(%edx)
	addl	$8, %eax
	movl	$.LC524, %edx
	.align 16
.L4282:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4282
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_mkdir, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$109, %ecx
	movl	%eax, 632(%edx)
	addl	$8, %eax
	movl	$.LC525, %edx
	.align 16
.L4283:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4283
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_mkfl, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$109, %ecx
	movl	%eax, 364(%edx)
	addl	$8, %eax
	movl	$.LC526, %edx
	.align 16
.L4284:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4284
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_del, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$100, %ecx
	movl	%eax, 176(%edx)
	addl	$8, %eax
	movl	$.LC527, %edx
	.align 16
.L4285:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4285
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_rfl, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$114, %ecx
	movl	%eax, 240(%edx)
	addl	$8, %eax
	movl	$.LC528, %edx
	.align 16
.L4286:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4286
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_editfl, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$101, %ecx
	movl	%eax, 804(%edx)
	addl	$8, %eax
	movl	$.LC529, %edx
	.align 16
.L4287:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4287
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_cp, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$99, %ecx
	movl	%eax, 128(%edx)
	addl	$8, %eax
	movl	$.LC530, %edx
	.align 16
.L4288:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4288
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_aptest, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$97, %ecx
	movl	%eax, 1228(%edx)
	addl	$8, %eax
	movl	$.LC531, %edx
	.align 16
.L4289:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4289
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_proc, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$112, %ecx
	movl	%eax, 396(%edx)
	addl	$8, %eax
	movl	$.LC532, %edx
	.align 16
.L4290:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4290
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_rn, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$114, %ecx
	movl	%eax, 172(%edx)
	addl	$8, %eax
	movl	$.LC533, %edx
	.align 16
.L4291:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4291
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movb	$0, (%eax)
	pushl	$24
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	Shell_Commands_list, %edx
	movl	$Command_ann, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$0, 4(%eax)
	movl	$97, %ecx
	movl	%eax, 260(%edx)
	addl	$8, %eax
	movl	$.LC534, %edx
	.align 16
.L4292:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4292
	movb	$0, (%eax)
	movl	CSI_mem_start, %eax
	leal	264(%eax), %edx
	.align 16
.L4293:
	addl	$4, %eax
	movl	$0, -4(%eax)
	cmpl	%edx, %eax
	jne	.L4293
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE436:
	.size	console_manager_init, .-console_manager_init
	.align 16
	.globl	kb_io_init
	.type	kb_io_init, @function
kb_io_init:
.LFB437:
	.cfi_startproc
	movl	$0, kb_Start_q
	movl	$0, kb_Last_q
	ret
	.cfi_endproc
.LFE437:
	.size	kb_io_init, .-kb_io_init
	.align 16
	.globl	kb_getline
	.type	kb_getline, @function
kb_getline:
.LFB438:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
	call	Get_Scheduler
	movl	system_dir, %edx
	movl	56(%eax), %ebx
/APP
/  209 "Source/MemManagement/virt_mm/paging.c" 1
	mov %edx, %cr3
/  0 "" 2
/NO_APP
	pushl	$kb_q_elements
	.cfi_def_cfa_offset 20
	pushl	$kb_Last_q
	.cfi_def_cfa_offset 24
	pushl	$kb_Start_q
	.cfi_def_cfa_offset 28
	pushl	$0
	.cfi_def_cfa_offset 32
	pushl	44(%eax)
	.cfi_def_cfa_offset 36
	pushl	$5
	.cfi_def_cfa_offset 40
	pushl	44(%esp)
	.cfi_def_cfa_offset 44
	pushl	44(%esp)
	.cfi_def_cfa_offset 48
	call	Serial_input
/APP
/  24 "Source/IO_Handling/IO_Devices/Keyboard/kb_handle.c" 1
	int $50;
/  0 "" 2
/  209 "Source/MemManagement/virt_mm/paging.c" 1
	mov %ebx, %cr3
/  0 "" 2
/NO_APP
	addl	$40, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE438:
	.size	kb_getline, .-kb_getline
	.align 16
	.globl	Parallel_Proc_spawner
	.type	Parallel_Proc_spawner, @function
Parallel_Proc_spawner:
.LFB572:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE572:
	.size	Parallel_Proc_spawner, .-Parallel_Proc_spawner
	.align 16
	.globl	Serial_input
	.type	Serial_input, @function
Serial_input:
.LFB440:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	movl	72(%esp), %ebx
	movl	84(%esp), %edi
	movl	88(%esp), %esi
	pushl	$104
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	72(%esp), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%ebx, 4(%eax)
	leal	8(%eax), %ebp
	orl	$1, %edx
	movl	%edx, (%eax)
	movl	64(%esp), %edx
	testl	%edx, %edx
	jne	.L4343
.L4336:
	movl	(%esi), %edx
	testl	%edx, %edx
	je	.L4344
	movl	(%edi), %ecx
	movl	%ebp, 28(%ecx)
	movl	%ebp, (%edi)
	movl	$0, 36(%eax)
.L4338:
	addl	$1, %edx
	movl	%edx, (%esi)
	movl	48(%esp), %edx
	movl	%ebx, 16(%eax)
	movl	24(%ebx), %esi
	movl	%edx, 8(%eax)
	movl	52(%esp), %edx
	movl	%edx, 12(%eax)
	call	Get_Scheduler
	movl	60(%eax), %eax
	movl	%eax, (%esi)
	movl	$0, 24(%ebx)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4344:
	.cfi_restore_state
	movl	68(%esp), %ecx
	movl	%ebp, (%ecx)
	movl	%ebp, (%edi)
	movl	(%ecx), %ecx
	movl	$0, 28(%ecx)
	jmp	.L4338
	.align 16
.L4343:
	movl	%eax, 12(%esp)
	call	Shell_sleep
	movl	12(%esp), %eax
	jmp	.L4336
	.cfi_endproc
.LFE440:
	.size	Serial_input, .-Serial_input
	.section	.rodata
.LC535:
	.string	"FS_Handling_Process"
	.text
	.align 16
	.globl	FS_Process_Init
	.type	FS_Process_Init, @function
FS_Process_Init:
.LFB441:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	pushl	kernel_proc
	.cfi_def_cfa_offset 20
	pushl	$1
	.cfi_def_cfa_offset 24
	pushl	$0
	.cfi_def_cfa_offset 28
	pushl	$.LC535
	.cfi_def_cfa_offset 32
	call	create_process
	movl	%eax, FS_Handling_Process
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE441:
	.size	FS_Process_Init, .-FS_Process_Init
	.align 16
	.globl	FS_Worker
	.type	FS_Worker, @function
FS_Worker:
.LFB442:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE442:
	.size	FS_Worker, .-FS_Worker
	.section	.rodata
.LC536:
	.string	"\nThe folder is empty!!!\n"
.LC537:
	.string	"\nFile not found\n"
	.text
	.align 16
	.globl	FS_Load_File
	.type	FS_Load_File, @function
FS_Load_File:
.LFB443:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	call	Get_Scheduler
	movl	44(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	8(%eax), %edx
	movl	40(%eax), %eax
	sall	$4, %eax
	addl	4(%edx), %eax
	movl	40(%eax), %edi
	movl	48(%eax), %ebx
	movl	56(%eax), %esi
	movl	32(%eax), %ebp
	movl	60(%eax), %eax
	movl	%edi, 12(%esp)
	movl	%ebx, 16(%esp)
	movl	%esi, 20(%esp)
	movl	%eax, 24(%esp)
	pushl	$512
	.cfi_def_cfa_offset 64
	call	*%ebx
	movl	%eax, (%esp)
	movl	%eax, %ebx
	movl	curr_dir, %eax
	pushl	$1
	.cfi_def_cfa_offset 68
	movl	30(%eax), %edx
	movl	26(%eax), %eax
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	pushl	%edx
	.cfi_def_cfa_offset 72
	pushl	%eax
	.cfi_def_cfa_offset 76
	pushl	curr_port
	.cfi_def_cfa_offset 80
	call	*%edi
	movl	46(%ebx), %edi
	movl	42(%ebx), %esi
	addl	$32, %esp
	.cfi_def_cfa_offset 48
	movl	%edi, %edx
	orl	%esi, %edx
	je	.L4372
	.align 16
.L4349:
	movl	%esi, %eax
	movl	%edi, %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	shrdl	$9, %edi, %eax
	shrl	$9, %edx
	pushl	%ebx
	.cfi_def_cfa_offset 64
	andl	$511, %esi
	pushl	$1
	.cfi_def_cfa_offset 68
	pushl	%edx
	.cfi_def_cfa_offset 72
	addl	%ebx, %esi
	pushl	%eax
	.cfi_def_cfa_offset 76
	pushl	curr_port
	.cfi_def_cfa_offset 80
	movl	32(%esp), %eax
	call	*%eax
	movzbl	0(%ebp), %edx
	addl	$32, %esp
	.cfi_def_cfa_offset 48
	leal	66(%esi), %eax
	cmpb	66(%esi), %dl
	jne	.L4350
	testb	%dl, %dl
	je	.L4351
	movl	%ebp, %edx
	jmp	.L4353
	.align 16
.L4354:
	testb	%cl, %cl
	je	.L4351
.L4353:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L4354
.L4350:
	movl	54(%esi), %edi
	movl	50(%esi), %esi
	movl	%edi, %edx
	orl	%esi, %edx
	jne	.L4349
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC537
	.cfi_def_cfa_offset 64
.L4371:
	call	printf
	movl	24(%esp), %eax
	addl	$44, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	*%eax
	.align 16
.L4351:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -20
	.cfi_offset 5, -8
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$32
	.cfi_def_cfa_offset 64
	movl	20(%esp), %edi
	call	*%edi
	movl	$162, (%esp)
	movl	%eax, %ebx
	call	*%edi
	leal	162(%eax), %edi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, %edx
	subl	%eax, %esi
	.align 16
.L4357:
	addl	$1, %edx
	movzbl	-1(%edx,%esi), %ecx
	cmpl	%edi, %edx
	movb	%cl, -1(%edx)
	jne	.L4357
	movl	%eax, 8(%ebx)
	addl	$66, %eax
	movl	%eax, (%ebx)
	movl	start_handle, %eax
	testl	%eax, %eax
	je	.L4373
	movl	current, %eax
	movl	%ebx, 28(%eax)
.L4359:
	movzbl	12(%esp), %eax
	movl	$0, 28(%ebx)
	movl	%ebx, current
	movl	%eax, 16(%ebx)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L4372:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 60
	pushl	$.LC536
	.cfi_def_cfa_offset 64
	jmp	.L4371
.L4373:
	.cfi_restore_state
	movl	%ebx, start_handle
	jmp	.L4359
	.cfi_endproc
.LFE443:
	.size	FS_Load_File, .-FS_Load_File
	.section	.rodata
.LC538:
	.string	"\nFile %s not loaded yet!\n"
	.align 4
.LC539:
	.string	"\nThe Requested offset is beyond the actual file size"
	.align 4
.LC540:
	.string	"\nThe Requested size is beyond the actual file size"
	.align 4
.LC541:
	.string	"\nThe offset is beyond the size of the file"
	.align 4
.LC542:
	.string	"\nFile reads as: %s \nFile size: %x"
	.text
	.align 16
	.globl	FS_Read_Proc
	.type	FS_Read_Proc, @function
FS_Read_Proc:
.LFB444:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$60, %esp
	.cfi_def_cfa_offset 80
	call	Get_Scheduler
	movl	44(%eax), %eax
	movl	8(%eax), %edx
	movl	40(%eax), %eax
	sall	$4, %eax
	addl	4(%edx), %eax
	movl	52(%eax), %ebx
	movl	8(%eax), %esi
	movl	56(%eax), %edi
	movl	12(%eax), %ebp
	movl	%ebx, 8(%esp)
	movl	48(%eax), %ebx
	movl	%esi, 16(%esp)
	movl	40(%eax), %esi
	movl	%edi, 24(%esp)
	movl	%ebx, 28(%esp)
	movl	start_handle, %ebx
	movl	%esi, 20(%esp)
	movl	32(%eax), %esi
	testl	%ebx, %ebx
	je	.L4382
	movzbl	(%esi), %edi
	.align 16
.L4383:
	movl	(%ebx), %eax
	movl	%edi, %ecx
	cmpb	%cl, (%eax)
	jne	.L4377
	testb	%cl, %cl
	je	.L4378
	movl	%esi, %edx
	jmp	.L4380
	.align 16
.L4381:
	testb	%cl, %cl
	je	.L4378
.L4380:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L4381
.L4377:
	movl	28(%ebx), %ebx
	testl	%ebx, %ebx
	jne	.L4383
.L4382:
	subl	$8, %esp
	.cfi_def_cfa_offset 88
	pushl	%esi
	.cfi_def_cfa_offset 92
	pushl	$.LC538
	.cfi_def_cfa_offset 96
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 80
.L4376:
	jmp	.L4376
	.align 16
.L4378:
	movl	8(%ebx), %eax
	movl	%eax, %edi
	movl	%eax, 32(%esp)
	movl	2(%eax), %eax
	movl	14(%edi), %edx
	leal	(%edx,%edx,2), %edx
	movl	%edx, %ecx
	movl	%eax, %edx
	sall	$4, %ecx
	subl	%ecx, %edx
	cmpl	%edx, 8(%esp)
	ja	.L4426
	movl	8(%esp), %edi
	movl	%eax, %edx
	subl	%edi, %edx
	subl	%ecx, %edx
	testl	%ebp, %ebp
	cmove	%edx, %ebp
	leal	0(%ebp,%edi), %edx
	cmpl	%edx, %eax
	jb	.L4427
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$512
	.cfi_def_cfa_offset 96
	movl	44(%esp), %eax
	call	*%eax
	movl	%eax, %ebx
	movl	48(%esp), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	38(%eax), %edi
	movl	34(%eax), %esi
	shrdl	$9, %edi, %esi
	shrl	$9, %edi
	testl	%ebx, %ebx
	je	.L4387
	xorl	%edx, %edx
	jmp	.L4388
	.align 16
.L4428:
	movl	36(%ebx), %edi
	movl	32(%ebx), %esi
	movl	%eax, %edx
	shrdl	$9, %edi, %esi
	shrl	$9, %edi
.L4388:
	movl	%edx, 36(%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	pushl	$1
	.cfi_def_cfa_offset 100
	pushl	%edi
	.cfi_def_cfa_offset 104
	pushl	%esi
	.cfi_def_cfa_offset 108
	pushl	curr_port
	.cfi_def_cfa_offset 112
	movl	52(%esp), %eax
	call	*%eax
	movl	4(%ebx), %eax
	movl	68(%esp), %edx
	addl	$32, %esp
	.cfi_def_cfa_offset 80
	sall	$10, %eax
	leal	-512(%edx,%eax), %eax
	cmpl	%eax, 8(%esp)
	ja	.L4428
	movl	8(%esp), %ecx
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	subl	%edx, %ecx
	movl	%ecx, %edx
	shrl	$9, %edx
	subl	%edx, %ecx
	movl	%edx, 52(%esp)
	movl	%ecx, 48(%esp)
	pushl	$512
	.cfi_def_cfa_offset 96
	movl	44(%esp), %eax
	call	*%eax
	addl	$1, %esi
	movl	%eax, (%esp)
	adcl	$0, %edi
	movl	%esi, 24(%esp)
	movl	%edi, 28(%esp)
	pushl	$1
	.cfi_def_cfa_offset 100
	movl	%eax, %edi
	movl	60(%esp), %edx
	movl	%edx, %eax
	xorl	%edx, %edx
	addl	28(%esp), %eax
	adcl	32(%esp), %edx
	pushl	%edx
	.cfi_def_cfa_offset 104
	pushl	%eax
	.cfi_def_cfa_offset 108
	pushl	curr_port
	.cfi_def_cfa_offset 112
	movl	52(%esp), %eax
	call	*%eax
	movl	68(%esp), %edx
	movl	$512, %eax
	addl	$32, %esp
	.cfi_def_cfa_offset 80
	subl	%edx, %eax
	cmpl	%eax, %ebp
	movl	%eax, %ecx
	cmovbe	%ebp, %eax
	addl	%edx, %edi
	movl	%edi, 28(%esp)
	movl	16(%esp), %edi
	testl	%eax, %eax
	leal	(%edi,%eax), %esi
	movl	%edi, %edx
	movl	%esi, 40(%esp)
	je	.L4394
	movl	8(%esp), %esi
	movl	12(%esp), %edi
	movl	%ecx, 44(%esp)
.L4409:
	addl	$1, %edx
	movl	28(%esp), %ecx
	movl	%edx, %eax
	subl	16(%esp), %eax
	cmpl	40(%esp), %edx
	movzbl	-1(%eax,%ecx), %eax
	movb	%al, -1(%edx)
	jne	.L4409
	movl	44(%esp), %ecx
	movl	%esi, 8(%esp)
	movl	%edi, 12(%esp)
.L4394:
	cmpl	%ecx, %ebp
	jnb	.L4429
.L4391:
	subl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 84
	movl	36(%esp), %eax
	pushl	2(%eax)
	.cfi_def_cfa_offset 88
	pushl	24(%esp)
	.cfi_def_cfa_offset 92
	pushl	$.LC542
	.cfi_def_cfa_offset 96
	jmp	.L4423
.L4387:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$.LC541
	.cfi_def_cfa_offset 96
.L4423:
	call	printf
	movl	40(%esp), %eax
	addl	$76, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	*%eax
.L4427:
	.cfi_def_cfa_offset 80
	.cfi_offset 3, -20
	.cfi_offset 5, -8
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$.LC540
	.cfi_def_cfa_offset 96
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	addl	$60, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4426:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 92
	pushl	$.LC539
	.cfi_def_cfa_offset 96
	jmp	.L4423
.L4429:
	.cfi_restore_state
	movl	16(%esp), %eax
	movl	36(%esp), %edi
	movl	8(%esp), %esi
	addl	$512, %eax
	subl	%edi, %eax
	movl	%eax, 36(%esp)
	leal	-512(%ebp,%edi), %eax
	movl	12(%esp), %edi
	shrl	$9, %eax
	addl	$1, %eax
	movl	%eax, 28(%esp)
	movl	4(%ebx), %eax
	leal	-2(%eax,%eax), %ebp
	jmp	.L4396
	.align 16
.L4395:
	movl	36(%ebx), %edi
	movl	32(%ebx), %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	pushl	$1
	.cfi_def_cfa_offset 100
	shrdl	$9, %edi, %esi
	shrl	$9, %edi
	pushl	%edi
	.cfi_def_cfa_offset 104
	pushl	%esi
	.cfi_def_cfa_offset 108
	pushl	curr_port
	.cfi_def_cfa_offset 112
	movl	52(%esp), %eax
	call	*%eax
	movl	4(%ebx), %eax
	addl	$32, %esp
	.cfi_def_cfa_offset 80
	leal	-1(%eax,%eax), %ebp
.L4396:
	testl	%ebp, %ebp
	je	.L4395
	movl	28(%esp), %eax
	cmpl	%ebp, %eax
	cmovbe	%eax, %ebp
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	addl	$1, %esi
	adcl	$0, %edi
	pushl	48(%esp)
	.cfi_def_cfa_offset 96
	pushl	%ebp
	.cfi_def_cfa_offset 100
	pushl	%edi
	.cfi_def_cfa_offset 104
	pushl	%esi
	.cfi_def_cfa_offset 108
	sall	$9, %ebp
	pushl	curr_port
	.cfi_def_cfa_offset 112
	movl	52(%esp), %esi
	call	*%esi
	addl	%ebp, 68(%esp)
	addl	$32, %esp
	.cfi_def_cfa_offset 80
	subl	$1, 28(%esp)
	jne	.L4395
	jmp	.L4391
	.cfi_endproc
.LFE444:
	.size	FS_Read_Proc, .-FS_Read_Proc
	.align 16
	.globl	FS_Write_Proc
	.type	FS_Write_Proc, @function
FS_Write_Proc:
.LFB445:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$76, %esp
	.cfi_def_cfa_offset 96
	call	Get_Scheduler
	movl	44(%eax), %eax
	movl	start_handle, %ebx
	movl	8(%eax), %edx
	movl	40(%eax), %eax
	sall	$4, %eax
	addl	4(%edx), %eax
	testl	%ebx, %ebx
	movl	36(%eax), %edi
	movl	40(%eax), %esi
	movl	%edi, 32(%esp)
	movl	12(%eax), %edi
	movl	%esi, %ebp
	movl	48(%eax), %esi
	movl	%edi, 20(%esp)
	movl	8(%eax), %edi
	movl	%esi, 36(%esp)
	movl	32(%eax), %esi
	movl	%edi, 28(%esp)
	movl	%edi, 16(%esp)
	movl	44(%eax), %edi
	movl	%edi, 24(%esp)
	movl	52(%eax), %edi
	movl	%edi, 44(%esp)
	je	.L4438
	movzbl	(%esi), %edi
	.align 16
.L4439:
	movl	(%ebx), %eax
	movl	%edi, %edx
	cmpb	%dl, (%eax)
	jne	.L4433
	testb	%dl, %dl
	je	.L4434
	movl	%esi, %edx
	jmp	.L4436
	.align 16
.L4437:
	testb	%cl, %cl
	je	.L4434
.L4436:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	je	.L4437
.L4433:
	movl	28(%ebx), %ebx
	testl	%ebx, %ebx
	jne	.L4439
.L4438:
	subl	$8, %esp
	.cfi_def_cfa_offset 104
	pushl	%esi
	.cfi_def_cfa_offset 108
	pushl	$.LC538
	.cfi_def_cfa_offset 112
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 96
.L4432:
	jmp	.L4432
	.align 16
.L4434:
	movl	8(%ebx), %ebx
	movl	38(%ebx), %eax
	orl	34(%ebx), %eax
	movl	%ebx, 4(%esp)
	jne	.L4478
	movl	20(%esp), %ebp
	movl	$65535, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 104
	movl	%ebp, %eax
	shrl	$9, %eax
	leal	1(%eax), %edi
	movl	%edi, 16(%esp)
	shrl	%edi
	movl	%edi, %eax
	addl	$1, %eax
	cmpl	$65535, %eax
	cmovg	%edx, %eax
	pushl	%eax
	.cfi_def_cfa_offset 108
	pushl	16(%esp)
	.cfi_def_cfa_offset 112
	movl	48(%esp), %esi
	call	*%esi
	movl	28(%eax), %edi
	movl	24(%eax), %esi
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	movl	%eax, %ebx
	movl	%ebp, 28(%esp)
	shrdl	$9, %edi, %esi
	shrl	$9, %edi
.L4447:
	cmpl	$131069, 8(%esp)
	movl	4(%esp), %ebp
	jbe	.L4440
	.align 16
.L4452:
	movl	%esi, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	movl	%edi, %edx
	addl	$1, %eax
	pushl	28(%esp)
	.cfi_def_cfa_offset 112
	pushl	$65535
	.cfi_def_cfa_offset 116
	adcl	$0, %edx
	pushl	%edx
	.cfi_def_cfa_offset 120
	pushl	%eax
	.cfi_def_cfa_offset 124
	pushl	curr_port
	.cfi_def_cfa_offset 128
	movl	56(%esp), %ecx
	call	*%ecx
	addl	$20, %esp
	.cfi_def_cfa_offset 108
	addl	$65535, (%ebx)
	pushl	%ebx
	.cfi_def_cfa_offset 112
	pushl	$1
	.cfi_def_cfa_offset 116
	pushl	%edi
	.cfi_def_cfa_offset 120
	pushl	%esi
	.cfi_def_cfa_offset 124
	pushl	curr_port
	.cfi_def_cfa_offset 128
	movl	56(%esp), %ecx
	call	*%ecx
	addl	$24, %esp
	.cfi_def_cfa_offset 104
	pushl	$65535
	.cfi_def_cfa_offset 108
	pushl	%ebp
	.cfi_def_cfa_offset 112
	movl	48(%esp), %edi
	call	*%edi
	subl	$131070, 24(%esp)
	movl	28(%eax), %edi
	movl	%eax, %ebx
	movl	24(%eax), %esi
	movl	24(%esp), %eax
	addl	$67107840, 32(%esp)
	subl	$67107850, 44(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	shrdl	$9, %edi, %esi
	shrl	$9, %edi
	cmpl	$131069, %eax
	ja	.L4452
	movl	%ebp, 4(%esp)
.L4440:
	movl	8(%esp), %ebp
	cmpl	$1, %ebp
	movl	%ebp, %eax
	jbe	.L4448
	movl	%esi, %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	subl	$1, %eax
	movl	%edi, %ecx
	addl	$1, %edx
	pushl	28(%esp)
	.cfi_def_cfa_offset 112
	adcl	$0, %ecx
	pushl	%eax
	.cfi_def_cfa_offset 116
	movl	%edx, %eax
	pushl	%ecx
	.cfi_def_cfa_offset 120
	pushl	%eax
	.cfi_def_cfa_offset 124
	pushl	curr_port
	.cfi_def_cfa_offset 128
	movl	56(%esp), %ecx
	call	*%ecx
	movl	%ebp, %eax
	addl	$8388607, %eax
	sall	$9, %eax
	addl	%eax, 48(%esp)
	addl	$32, %esp
	.cfi_def_cfa_offset 96
.L4448:
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$512
	.cfi_def_cfa_offset 112
	movl	52(%esp), %eax
	call	*%eax
	movl	%eax, %ecx
	movl	24(%esp), %eax
	movl	44(%esp), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	leal	-2(%eax,%eax), %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	16(%esp), %edx
	leal	(%edx,%eax), %ebp
	movl	%ebp, 16(%esp)
	movl	%ecx, %ebp
	subl	%edx, %ebp
	testl	%eax, %eax
	movl	%ebp, 32(%esp)
	je	.L4451
	movl	4(%esp), %ebp
	movl	%ecx, 36(%esp)
	.align 16
.L4461:
	addl	$1, %edx
	movzbl	-1(%edx), %eax
	movl	32(%esp), %ecx
	movb	%al, -1(%edx,%ecx)
	cmpl	16(%esp), %edx
	jne	.L4461
	movl	36(%esp), %ecx
	movl	%ebp, 4(%esp)
.L4451:
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	xorl	%edx, %edx
	pushl	%ecx
	.cfi_def_cfa_offset 112
	pushl	$1
	.cfi_def_cfa_offset 116
	movl	28(%esp), %eax
	addl	%esi, %eax
	adcl	%edi, %edx
	pushl	%edx
	.cfi_def_cfa_offset 120
	pushl	%eax
	.cfi_def_cfa_offset 124
	pushl	curr_port
	.cfi_def_cfa_offset 128
	movl	56(%esp), %ebp
	movl	%ebp, %eax
	call	*%eax
	movl	60(%esp), %eax
	addl	$20, %esp
	.cfi_def_cfa_offset 108
	addl	%eax, (%ebx)
	pushl	%ebx
	.cfi_def_cfa_offset 112
	pushl	$1
	.cfi_def_cfa_offset 116
	movl	%ebp, %eax
	pushl	%edi
	.cfi_def_cfa_offset 120
	pushl	%esi
	.cfi_def_cfa_offset 124
	pushl	curr_port
	.cfi_def_cfa_offset 128
	call	*%eax
	addl	$32, %esp
	.cfi_def_cfa_offset 96
.L4443:
	movl	4(%esp), %esi
	movl	20(%esp), %eax
	addl	%eax, 2(%esi)
	movl	44(%esp), %eax
	addl	$76, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	jmp	*%eax
	.align 16
.L4478:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	pushl	$512
	.cfi_def_cfa_offset 112
	movl	52(%esp), %eax
	call	*%eax
	movl	%eax, (%esp)
	movl	%eax, %ebx
	pushl	$1
	.cfi_def_cfa_offset 116
	movl	24(%esp), %eax
	movl	46(%eax), %edx
	movl	42(%eax), %eax
	shrdl	$9, %edx, %eax
	shrl	$9, %edx
	pushl	%edx
	.cfi_def_cfa_offset 120
	pushl	%eax
	.cfi_def_cfa_offset 124
	pushl	curr_port
	.cfi_def_cfa_offset 128
	call	*%ebp
	movl	4(%ebx), %ecx
	movl	28(%ebx), %edi
	movl	24(%ebx), %esi
	sall	$10, %ecx
	subl	(%ebx), %ecx
	shrdl	$9, %edi, %esi
	shrl	$9, %edi
	movl	%esi, 40(%esp)
	movl	%edi, 44(%esp)
	movl	%ecx, 72(%esp)
	addl	$20, %esp
	.cfi_def_cfa_offset 108
	movl	%ecx, %esi
	pushl	$1024
	.cfi_def_cfa_offset 112
	movl	52(%esp), %eax
	call	*%eax
	movl	%eax, 32(%esp)
	movl	%eax, (%esp)
	xorl	%edx, %edx
	pushl	$2
	.cfi_def_cfa_offset 116
	movl	4(%ebx), %eax
	leal	-2(%eax,%eax), %eax
	addl	28(%esp), %eax
	adcl	32(%esp), %edx
	pushl	%edx
	.cfi_def_cfa_offset 120
	pushl	%eax
	.cfi_def_cfa_offset 124
	pushl	curr_port
	.cfi_def_cfa_offset 128
	call	*%ebp
	addl	$32, %esp
	.cfi_def_cfa_offset 96
	movl	20(%esp), %edi
	movl	16(%esp), %ecx
	movl	%esi, %eax
	cmpl	%esi, %edi
	cmovbe	%edi, %eax
	movl	%eax, %ebp
	movl	%eax, 48(%esp)
	leal	1024(%ecx), %eax
	movl	%eax, %edi
	subl	%esi, %edi
	movl	28(%esp), %esi
	movl	%edi, 52(%esp)
	movl	%esi, %edi
	movl	%esi, %eax
	addl	%ebp, %edi
	testl	%ebp, %ebp
	movl	%edi, 16(%esp)
	je	.L4479
	movl	4(%esp), %ebp
	movl	8(%esp), %esi
	movl	12(%esp), %edi
	movl	%ebx, 56(%esp)
	movl	%ecx, 60(%esp)
	.align 16
.L4462:
	addl	$1, %eax
	movzbl	-1(%eax), %ecx
	movl	52(%esp), %ebx
	movl	%eax, %edx
	subl	28(%esp), %edx
	cmpl	16(%esp), %eax
	movb	%cl, -1(%edx,%ebx)
	jne	.L4462
	movl	56(%esp), %ebx
	movl	60(%esp), %ecx
	movl	%ebp, 4(%esp)
	movl	%esi, 8(%esp)
	movl	%edi, 12(%esp)
.L4446:
	subl	$12, %esp
	.cfi_def_cfa_offset 108
	xorl	%edx, %edx
	pushl	%ecx
	.cfi_def_cfa_offset 112
	pushl	$2
	.cfi_def_cfa_offset 116
	movl	4(%ebx), %eax
	leal	-2(%eax,%eax), %eax
	addl	28(%esp), %eax
	adcl	32(%esp), %edx
	pushl	%edx
	.cfi_def_cfa_offset 120
	pushl	%eax
	.cfi_def_cfa_offset 124
	pushl	curr_port
	.cfi_def_cfa_offset 128
	movl	56(%esp), %edi
	movl	%edi, %eax
	call	*%eax
	movl	80(%esp), %eax
	addl	$20, %esp
	.cfi_def_cfa_offset 108
	addl	%eax, (%ebx)
	pushl	%ebx
	.cfi_def_cfa_offset 112
	pushl	$1
	.cfi_def_cfa_offset 116
	movl	%edi, %eax
	pushl	32(%esp)
	.cfi_def_cfa_offset 120
	pushl	32(%esp)
	.cfi_def_cfa_offset 124
	pushl	curr_port
	.cfi_def_cfa_offset 128
	call	*%eax
	addl	$32, %esp
	.cfi_def_cfa_offset 96
	movl	40(%esp), %edi
	cmpl	%edi, 20(%esp)
	jbe	.L4443
	movl	20(%esp), %eax
	subl	40(%esp), %eax
	movl	$65535, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 104
	movl	%eax, 36(%esp)
	shrl	$9, %eax
	addl	$1, %eax
	movl	%eax, 16(%esp)
	shrl	%eax
	addl	$1, %eax
	cmpl	$65535, %eax
	cmovg	%edx, %eax
	pushl	%eax
	.cfi_def_cfa_offset 108
	pushl	16(%esp)
	.cfi_def_cfa_offset 112
	movl	48(%esp), %eax
	call	*%eax
	movl	28(%eax), %edi
	movl	24(%eax), %esi
	movl	%eax, %ebx
	addl	$16, %esp
	.cfi_def_cfa_offset 96
	shrdl	$9, %edi, %esi
	shrl	$9, %edi
	jmp	.L4447
.L4479:
	movl	%esi, 16(%esp)
	jmp	.L4446
	.cfi_endproc
.LFE445:
	.size	FS_Write_Proc, .-FS_Write_Proc
	.globl	ax_add
	.data
	.align 4
	.type	ax_add, @object
	.size	ax_add, 4
ax_add:
	.long	639631360
	.text
	.align 16
	.globl	std_Qalloc
	.type	std_Qalloc, @function
std_Qalloc:
.LFB446:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$104
	.cfi_def_cfa_offset 32
	call	*kmalloc
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE446:
	.size	std_Qalloc, .-std_Qalloc
	.align 16
	.globl	setBank
	.type	setBank, @function
setBank:
.LFB447:
	.cfi_startproc
	subl	$52, %esp
	.cfi_def_cfa_offset 56
	movl	$20229, %eax
	xorl	%edx, %edx
	movw	%ax, 28(%esp)
	movl	56(%esp), %eax
	movw	%dx, 22(%esp)
	movw	%ax, 24(%esp)
	leal	14(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	$16
	.cfi_def_cfa_offset 64
	call	int32
	addl	$60, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE447:
	.size	setBank, .-setBank
	.globl	vesa_alloc_base
	.data
	.align 4
	.type	vesa_alloc_base, @object
	.size	vesa_alloc_base, 4
vesa_alloc_base:
	.long	4096
	.text
	.align 16
	.globl	setVesa
	.type	setVesa, @function
setVesa:
.LFB449:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$828, %esp
	.cfi_def_cfa_offset 848
	movl	vesa_alloc_base, %edi
	leal	22(%esp), %ebx
	leal	48(%esp), %esi
	leal	8192(%edi), %eax
	movl	%ebx, %edx
	movl	%eax, vesa_alloc_base
	movb	$86, (%edi)
	movb	$66, 1(%edi)
	movb	$69, 2(%edi)
	movb	$50, 3(%edi)
	.align 16
.L4485:
	addl	$1, %edx
	movb	$0, -1(%edx)
	cmpl	%esi, %edx
	jne	.L4485
	movl	%edi, %eax
	movl	$20224, %ecx
	subl	$8, %esp
	.cfi_def_cfa_offset 856
	andl	$15, %eax
	movw	%cx, 44(%esp)
	leal	512(%edi), %ebp
	movw	%ax, 30(%esp)
	movl	%edi, %eax
	shrl	$4, %eax
	movw	%ax, 50(%esp)
	pushl	%ebx
	.cfi_def_cfa_offset 860
	pushl	$16
	.cfi_def_cfa_offset 864
	call	int32
	addl	$16, %esp
	.cfi_def_cfa_offset 848
	movl	%edi, %eax
	leal	304(%esp), %edi
	subl	%eax, %edi
	.align 16
.L4486:
	addl	$1, %eax
	movzbl	-1(%eax), %edx
	cmpl	%ebp, %eax
	movb	%dl, -1(%eax,%edi)
	jne	.L4486
	movl	vesa_alloc_base, %edi
	movl	%ebx, %ebp
	leal	8192(%edi), %eax
	movl	%eax, vesa_alloc_base
	.align 16
.L4487:
	addl	$1, %ebp
	movb	$0, -1(%ebp)
	cmpl	%esi, %ebp
	jne	.L4487
	movl	%edi, %eax
	movl	$20225, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 856
	andl	$15, %eax
	movw	%dx, 44(%esp)
	movw	%ax, 30(%esp)
	movl	%edi, %eax
	shrl	$4, %eax
	movw	%ax, 50(%esp)
	movzwl	856(%esp), %eax
	movw	%ax, 42(%esp)
	pushl	%ebx
	.cfi_def_cfa_offset 860
	pushl	$16
	.cfi_def_cfa_offset 864
	call	int32
	movl	%edi, %eax
	leal	256(%edi), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 848
	subl	%eax, %ebp
	.align 16
.L4488:
	addl	$1, %eax
	movzbl	-1(%eax), %edx
	cmpl	%ecx, %eax
	movb	%dl, -1(%eax,%ebp)
	jne	.L4488
	movzwl	66(%esp), %eax
	movl	$32, %edx
	movl	%eax, widthVESA
	movzwl	68(%esp), %eax
	movl	%eax, heightVESA
	movzbl	73(%esp), %eax
	cmpl	$24, %eax
	cmove	%edx, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 860
	movl	%eax, depthVESA
	movl	100(%esp), %eax
	pushl	$3145728
	.cfi_def_cfa_offset 864
	movl	%eax, vga_mem
	call	*kmalloc
	movl	$3145728, (%esp)
	movl	%eax, general_buff
	call	*kmalloc
	movl	%eax, buff
	movl	vga_mem, %eax
	movl	$3145728, (%esp)
	movl	%eax, vesa_buff
	call	*kmalloc
	leal	3145728(%eax), %edx
	movl	%eax, mouse_buff
	addl	$16, %esp
	.cfi_def_cfa_offset 848
	.align 16
.L4491:
	addl	$4, %eax
	movl	$0, -4(%eax)
	cmpl	%edx, %eax
	jne	.L4491
	movl	buff, %eax
	leal	1572864(%eax), %edx
	.align 16
.L4492:
	addl	$1, %eax
	movb	$90, -1(%eax)
	cmpl	%eax, %edx
	jne	.L4492
	movl	cx0, %eax
	movl	%eax, 4(%esp)
	movl	4(%esp), %edx
	xorl	%eax, %eax
	testl	%edx, %edx
	jle	.L4508
.L4493:
	movl	%eax, cx0
	movl	cx1, %eax
	movl	%eax, 8(%esp)
	movl	8(%esp), %edx
	movl	$1024, %eax
	cmpl	$1023, %edx
	jle	.L4494
	movl	8(%esp), %eax
.L4494:
	movl	%eax, cx1
	movl	cy0, %eax
	movl	%eax, 12(%esp)
	movl	12(%esp), %edx
	xorl	%eax, %eax
	testl	%edx, %edx
	jle	.L4509
.L4495:
	movl	%eax, cy0
	movl	cy1, %eax
	movl	%eax, 16(%esp)
	movl	16(%esp), %edx
	movl	$768, %eax
	cmpl	$767, %edx
	jle	.L4496
	movl	16(%esp), %eax
.L4496:
	movzwl	848(%esp), %esi
	movl	%eax, cy1
	movl	$20226, %eax
	movw	%ax, 36(%esp)
	subl	$8, %esp
	.cfi_def_cfa_offset 856
	orw	$16384, %si
	movw	%si, 38(%esp)
	pushl	%ebx
	.cfi_def_cfa_offset 860
	pushl	$16
	.cfi_def_cfa_offset 864
	call	int32
	addl	$844, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L4508:
	.cfi_def_cfa_offset 848
	.cfi_offset 3, -20
	.cfi_offset 5, -8
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	movl	4(%esp), %eax
	jmp	.L4493
.L4509:
	movl	12(%esp), %eax
	jmp	.L4495
	.cfi_endproc
.LFE449:
	.size	setVesa, .-setVesa
	.align 16
	.globl	setIRQMask
	.type	setIRQMask, @function
setIRQMask:
.LFB450:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %ecx
	movl	$33, %edx
	cmpl	$7, %ecx
	jle	.L4511
	subl	$8, %ecx
	movl	$161, %edx
.L4511:
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	movl	$1, %ebx
	sall	%cl, %ebx
	orl	%ebx, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE450:
	.size	setIRQMask, .-setIRQMask
	.align 16
	.globl	clearIRQMask
	.type	clearIRQMask, @function
clearIRQMask:
.LFB451:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %ecx
	movl	$33, %edx
	cmpl	$7, %ecx
	jle	.L4516
	subl	$8, %ecx
	movl	$161, %edx
.L4516:
/APP
/  45 "Library/sys.h" 1
	inb %dx, %al
/  0 "" 2
/NO_APP
	movl	$-2, %ebx
	roll	%cl, %ebx
	andl	%ebx, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, %dx
/  0 "" 2
/NO_APP
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE451:
	.size	clearIRQMask, .-clearIRQMask
	.align 16
	.globl	enable_pic
	.type	enable_pic, @function
enable_pic:
.LFB452:
	.cfi_startproc
	movl	$17, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $32
/  0 "" 2
/  37 "Library/sys.h" 1
	outb %al, $160
/  0 "" 2
/NO_APP
	movl	$32, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/NO_APP
	movl	$40, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $161
/  0 "" 2
/NO_APP
	movl	$4, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/NO_APP
	movl	$2, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $161
/  0 "" 2
/NO_APP
	movl	$1, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/  37 "Library/sys.h" 1
	outb %al, $161
/  0 "" 2
/NO_APP
	xorl	%eax, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/  37 "Library/sys.h" 1
	outb %al, $161
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE452:
	.size	enable_pic, .-enable_pic
	.align 16
	.globl	disable_pic
	.type	disable_pic, @function
disable_pic:
.LFB453:
	.cfi_startproc
	movl	$-1, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/  37 "Library/sys.h" 1
	outb %al, $161
/  0 "" 2
/NO_APP
	ret
	.cfi_endproc
.LFE453:
	.size	disable_pic, .-disable_pic
	.align 16
	.globl	make_B_Font
	.type	make_B_Font, @function
make_B_Font:
.LFB454:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$252
	.cfi_def_cfa_offset 32
	call	*kmalloc
	fld1
	movl	%eax, char_B
	movl	$0, 12(%eax)
	movl	$10, (%eax)
	movl	$0, 20(%eax)
	fstpl	4(%eax)
	movl	$0, 16(%eax)
	movl	$8, 24(%eax)
	movl	$0, 36(%eax)
	movl	$3, 44(%eax)
	movl	$0, 40(%eax)
	movl	$0, 48(%eax)
	movl	$3, 60(%eax)
	movl	$4, 68(%eax)
	movl	$0, 64(%eax)
	movl	$1, 72(%eax)
	movl	$4, 84(%eax)
	movl	$4, 92(%eax)
	movl	$1, 88(%eax)
	movl	$3, 96(%eax)
	movl	$4, 108(%eax)
	movl	$3, 116(%eax)
	movl	$3, 112(%eax)
	movl	$4, 120(%eax)
	movl	$3, 132(%eax)
	movl	$4, 140(%eax)
	movl	$4, 136(%eax)
	movl	$5, 144(%eax)
	movl	$4, 156(%eax)
	movl	$4, 164(%eax)
	movl	$5, 160(%eax)
	movl	$7, 168(%eax)
	movl	$4, 180(%eax)
	movl	$3, 188(%eax)
	movl	$7, 184(%eax)
	movl	$8, 192(%eax)
	movl	$0, 204(%eax)
	movl	$3, 212(%eax)
	movl	$4, 208(%eax)
	movl	$4, 216(%eax)
	movl	$0, 228(%eax)
	movl	$3, 236(%eax)
	movl	$8, 232(%eax)
	movl	$8, 240(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE454:
	.size	make_B_Font, .-make_B_Font
	.align 16
	.globl	make_C_Font
	.type	make_C_Font, @function
make_C_Font:
.LFB455:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$132
	.cfi_def_cfa_offset 32
	call	*kmalloc
	fldl	.LC544
	movl	%eax, char_C
	movl	$1, 12(%eax)
	movl	$5, (%eax)
	fstpl	4(%eax)
	movl	$3, 20(%eax)
	movl	$0, 16(%eax)
	movl	$0, 24(%eax)
	movl	$1, 36(%eax)
	movl	$0, 44(%eax)
	movl	$0, 40(%eax)
	movl	$1, 48(%eax)
	movl	$0, 60(%eax)
	movl	$0, 68(%eax)
	movl	$1, 64(%eax)
	movl	$4, 72(%eax)
	movl	$0, 84(%eax)
	movl	$1, 92(%eax)
	movl	$4, 88(%eax)
	movl	$5, 96(%eax)
	movl	$1, 108(%eax)
	movl	$3, 116(%eax)
	movl	$5, 112(%eax)
	movl	$5, 120(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE455:
	.size	make_C_Font, .-make_C_Font
	.align 16
	.globl	make_E_Font
	.type	make_E_Font, @function
make_E_Font:
.LFB456:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$108
	.cfi_def_cfa_offset 32
	call	*kmalloc
	fldl	.LC545
	movl	%eax, char_E
	movl	$0, 12(%eax)
	movl	$4, (%eax)
	fstpl	4(%eax)
	movl	$0, 20(%eax)
	movl	$0, 16(%eax)
	movl	$7, 24(%eax)
	movl	$0, 36(%eax)
	movl	$4, 44(%eax)
	movl	$0, 40(%eax)
	movl	$0, 48(%eax)
	movl	$0, 60(%eax)
	movl	$3, 68(%eax)
	movl	$4, 64(%eax)
	movl	$4, 72(%eax)
	movl	$0, 84(%eax)
	movl	$4, 92(%eax)
	movl	$7, 88(%eax)
	movl	$7, 96(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE456:
	.size	make_E_Font, .-make_E_Font
	.align 16
	.globl	make_1_Font
	.type	make_1_Font, @function
make_1_Font:
.LFB457:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$84
	.cfi_def_cfa_offset 32
	call	*kmalloc
	fld1
	movl	%eax, char_1
	movl	$0, 12(%eax)
	movl	$3, (%eax)
	movl	$1, 20(%eax)
	fstpl	4(%eax)
	movl	$1, 16(%eax)
	movl	$0, 24(%eax)
	movl	$1, 36(%eax)
	movl	$1, 44(%eax)
	movl	$0, 40(%eax)
	movl	$8, 48(%eax)
	movl	$0, 60(%eax)
	movl	$2, 68(%eax)
	movl	$8, 64(%eax)
	movl	$8, 72(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE457:
	.size	make_1_Font, .-make_1_Font
	.align 16
	.globl	make_2_Font
	.type	make_2_Font, @function
make_2_Font:
.LFB458:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$156
	.cfi_def_cfa_offset 32
	call	*kmalloc
	fldl	.LC544
	movl	%eax, char_2
	movl	$0, 12(%eax)
	movl	$6, (%eax)
	fstpl	4(%eax)
	movl	$1, 20(%eax)
	movl	$1, 16(%eax)
	movl	$0, 24(%eax)
	movl	$1, 36(%eax)
	movl	$2, 44(%eax)
	movl	$0, 40(%eax)
	movl	$0, 48(%eax)
	movl	$2, 60(%eax)
	movl	$3, 68(%eax)
	movl	$0, 64(%eax)
	movl	$1, 72(%eax)
	movl	$3, 84(%eax)
	movl	$3, 92(%eax)
	movl	$1, 88(%eax)
	movl	$2, 96(%eax)
	movl	$3, 108(%eax)
	movl	$0, 116(%eax)
	movl	$2, 112(%eax)
	movl	$5, 120(%eax)
	movl	$0, 132(%eax)
	movl	$3, 140(%eax)
	movl	$5, 136(%eax)
	movl	$5, 144(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE458:
	.size	make_2_Font, .-make_2_Font
	.align 16
	.globl	make_3_Font
	.type	make_3_Font, @function
make_3_Font:
.LFB459:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$228
	.cfi_def_cfa_offset 32
	call	*kmalloc
	fld1
	movl	%eax, char_3
	movl	$0, 12(%eax)
	movl	$9, (%eax)
	movl	$3, 20(%eax)
	fstpl	4(%eax)
	movl	$0, 16(%eax)
	movl	$0, 24(%eax)
	movl	$3, 36(%eax)
	movl	$4, 44(%eax)
	movl	$0, 40(%eax)
	movl	$1, 48(%eax)
	movl	$4, 60(%eax)
	movl	$4, 68(%eax)
	movl	$1, 64(%eax)
	movl	$3, 72(%eax)
	movl	$4, 84(%eax)
	movl	$3, 92(%eax)
	movl	$3, 88(%eax)
	movl	$4, 96(%eax)
	movl	$3, 108(%eax)
	movl	$4, 116(%eax)
	movl	$4, 112(%eax)
	movl	$5, 120(%eax)
	movl	$4, 132(%eax)
	movl	$4, 140(%eax)
	movl	$5, 136(%eax)
	movl	$7, 144(%eax)
	movl	$4, 156(%eax)
	movl	$3, 164(%eax)
	movl	$7, 160(%eax)
	movl	$8, 168(%eax)
	movl	$1, 180(%eax)
	movl	$3, 188(%eax)
	movl	$4, 184(%eax)
	movl	$4, 192(%eax)
	movl	$0, 204(%eax)
	movl	$3, 212(%eax)
	movl	$8, 208(%eax)
	movl	$8, 216(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE459:
	.size	make_3_Font, .-make_3_Font
	.align 16
	.globl	make_4_Font
	.type	make_4_Font, @function
make_4_Font:
.LFB460:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$84
	.cfi_def_cfa_offset 32
	call	*kmalloc
	fldl	.LC546
	movl	%eax, char_4
	movl	$4, 12(%eax)
	movl	$3, (%eax)
	fstpl	4(%eax)
	movl	$4, 20(%eax)
	movl	$0, 16(%eax)
	movl	$8, 24(%eax)
	movl	$0, 36(%eax)
	movl	$4, 44(%eax)
	movl	$6, 40(%eax)
	movl	$0, 48(%eax)
	movl	$0, 60(%eax)
	movl	$5, 68(%eax)
	movl	$6, 64(%eax)
	movl	$6, 72(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE460:
	.size	make_4_Font, .-make_4_Font
	.align 16
	.globl	make_5_Font
	.type	make_5_Font, @function
make_5_Font:
.LFB461:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$180
	.cfi_def_cfa_offset 32
	call	*kmalloc
	fldl	.LC547
	movl	%eax, char_5
	movl	$0, 12(%eax)
	movl	$7, (%eax)
	fstpl	4(%eax)
	movl	$4, 20(%eax)
	movl	$0, 16(%eax)
	movl	$0, 24(%eax)
	movl	$0, 36(%eax)
	movl	$0, 44(%eax)
	movl	$0, 40(%eax)
	movl	$4, 48(%eax)
	movl	$0, 60(%eax)
	movl	$2, 68(%eax)
	movl	$4, 64(%eax)
	movl	$4, 72(%eax)
	movl	$2, 84(%eax)
	movl	$4, 92(%eax)
	movl	$4, 88(%eax)
	movl	$5, 96(%eax)
	movl	$4, 108(%eax)
	movl	$4, 116(%eax)
	movl	$5, 112(%eax)
	movl	$8, 120(%eax)
	movl	$4, 132(%eax)
	movl	$3, 140(%eax)
	movl	$8, 136(%eax)
	movl	$9, 144(%eax)
	movl	$3, 156(%eax)
	movl	$0, 164(%eax)
	movl	$9, 160(%eax)
	movl	$9, 168(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE461:
	.size	make_5_Font, .-make_5_Font
	.align 16
	.globl	make_6_Font
	.type	make_6_Font, @function
make_6_Font:
.LFB462:
	.cfi_startproc
	subl	$24, %esp
	.cfi_def_cfa_offset 28
	pushl	$84
	.cfi_def_cfa_offset 32
	call	*kmalloc
	fld1
	movl	$3, (%eax)
	fstpl	4(%eax)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE462:
	.size	make_6_Font, .-make_6_Font
	.align 16
	.globl	font_maker
	.type	font_maker, @function
font_maker:
.LFB463:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	make_B_Font
	call	make_C_Font
	call	make_E_Font
	call	make_1_Font
	call	make_2_Font
	call	make_3_Font
	call	make_4_Font
	addl	$12, %esp
	.cfi_def_cfa_offset 4
	jmp	make_5_Font
	.cfi_endproc
.LFE463:
	.size	font_maker, .-font_maker
	.align 16
	.globl	font_renderer
	.type	font_renderer, @function
font_renderer:
.LFB464:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$36, %esp
	.cfi_def_cfa_offset 56
	movl	56(%esp), %edi
	movl	72(%esp), %ebp
	movl	(%edi), %eax
	testl	%eax, %eax
	je	.L4547
	movl	64(%esp), %eax
	xorl	%edx, %edx
	leal	12(%edi), %ebx
	movl	%edx, 4(%esp)
	xorl	%edx, %edx
	xorl	%esi, %esi
	movl	%eax, (%esp)
	movl	60(%esp), %eax
	fildq	(%esp)
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	fildq	(%esp)
	jmp	.L4544
	.align 16
.L4549:
	fxch	%st(1)
.L4544:
	movl	12(%ebx), %eax
	fldl	4(%edi)
	addl	$1, %esi
	pushl	76(%esp)
	.cfi_def_cfa_offset 60
	pushl	72(%esp)
	.cfi_def_cfa_offset 64
	addl	$24, %ebx
	imull	%ebp, %eax
	movl	$0, 12(%esp)
	fnstcw	30(%esp)
	movl	%eax, 8(%esp)
	movzwl	30(%esp), %eax
	fildq	8(%esp)
	fmul	%st(1), %st
	movb	$12, %ah
	movw	%ax, 28(%esp)
	fadd	%st(3), %st
	fldcw	28(%esp)
	fistpl	24(%esp)
	fldcw	30(%esp)
	movl	24(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 68
	movl	-16(%ebx), %eax
	movl	$0, 16(%esp)
	imull	%ebp, %eax
	movl	%eax, 12(%esp)
	fildq	12(%esp)
	fmul	%st(1), %st
	fadd	%st(2), %st
	fldcw	32(%esp)
	fistpl	28(%esp)
	fldcw	34(%esp)
	movl	28(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 72
	movl	-20(%ebx), %eax
	movl	$0, 20(%esp)
	imull	%ebp, %eax
	movl	%eax, 16(%esp)
	fildq	16(%esp)
	fmul	%st(1), %st
	fadd	%st(3), %st
	fxch	%st(3)
	fstpl	24(%esp)
	fxch	%st(2)
	fldcw	36(%esp)
	fistpl	32(%esp)
	fldcw	38(%esp)
	movl	32(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 76
	movl	-24(%ebx), %eax
	movl	$0, 24(%esp)
	imull	%ebp, %eax
	movl	%eax, 20(%esp)
	fildq	20(%esp)
	fmulp	%st, %st(2)
	fadd	%st, %st(1)
	fstpl	20(%esp)
	fldcw	40(%esp)
	fistpl	36(%esp)
	fldcw	42(%esp)
	movl	36(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	line_plot
	addl	$24, %esp
	.cfi_def_cfa_offset 56
	cmpl	%esi, (%edi)
	fldl	(%esp)
	fldl	8(%esp)
	ja	.L4549
	fstp	%st(0)
	fstp	%st(0)
.L4547:
	addl	$36, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE464:
	.size	font_renderer, .-font_renderer
	.align 16
	.globl	memcpy_faster
	.type	memcpy_faster, @function
memcpy_faster:
.LFB465:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	20(%esp), %ebx
	movl	12(%esp), %eax
	movl	16(%esp), %esi
	testl	%ebx, %ebx
	je	.L4556
	xorl	%edx, %edx
	.align 16
.L4554:
	movl	(%esi,%edx,4), %ecx
	movl	%ecx, (%eax,%edx,4)
	addl	$1, %edx
	cmpl	%ebx, %edx
	jne	.L4554
.L4556:
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE465:
	.size	memcpy_faster, .-memcpy_faster
	.align 16
	.globl	memset_faster
	.type	memset_faster, @function
memset_faster:
.LFB466:
	.cfi_startproc
	movl	12(%esp), %eax
	movl	4(%esp), %edx
	movl	8(%esp), %ecx
	testl	%eax, %eax
	je	.L4558
	.align 16
.L4562:
	addl	$4, %edx
	subl	$1, %eax
	movl	%ecx, -4(%edx)
	jne	.L4562
.L4558:
	rep ret
	.cfi_endproc
.LFE466:
	.size	memset_faster, .-memset_faster
	.align 16
	.globl	pow
	.type	pow, @function
pow:
.LFB467:
	.cfi_startproc
	movl	8(%esp), %edx
	movl	4(%esp), %ecx
	cmpl	$1, %edx
	movl	%ecx, %eax
	jle	.L4567
	.align 16
.L4566:
	subl	$1, %edx
	imull	%ecx, %eax
	cmpl	$1, %edx
	jne	.L4566
	rep ret
.L4567:
	rep ret
	.cfi_endproc
.LFE467:
	.size	pow, .-pow
	.align 16
	.globl	coprimes
	.type	coprimes, @function
coprimes:
.LFB468:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %ebx
	movl	20(%esp), %esi
	cmpl	%esi, %ebx
	movl	%esi, %edi
	cmovle	%ebx, %edi
	cmpl	$2, %edi
	jle	.L4573
	movl	$2, %ecx
	.align 16
.L4572:
	movl	%ebx, %eax
	cltd
	idivl	%ecx
	testl	%edx, %edx
	jne	.L4571
	movl	%esi, %eax
	cltd
	idivl	%ecx
	testl	%edx, %edx
	je	.L4574
.L4571:
	addl	$1, %ecx
	cmpl	%edi, %ecx
	jne	.L4572
.L4573:
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	xorl	%eax, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4574:
	.cfi_restore_state
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	movl	$1, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE468:
	.size	coprimes, .-coprimes
	.section	.rodata
	.align 32
	.type	gMaskShift, @object
	.size	gMaskShift, 256
gMaskShift:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.byte	13
	.byte	14
	.byte	15
	.byte	16
	.byte	17
	.byte	18
	.byte	19
	.byte	20
	.byte	21
	.byte	22
	.byte	23
	.byte	24
	.byte	25
	.byte	26
	.byte	27
	.byte	28
	.byte	29
	.byte	30
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.text
	.align 16
	.globl	sqrtf
	.type	sqrtf, @function
sqrtf:
.LFB469:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	flds	32(%esp)
	fld	%st(0)
	fsqrt
	fstps	12(%esp)
	fldz
	fucomip	%st(1), %st
	jbe	.L4580
	subl	$16, %esp
	.cfi_def_cfa_offset 48
	fstps	(%esp)
	call	sqrtf
	fstp	%st(0)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L4577
	.align 16
.L4580:
	fstp	%st(0)
.L4577:
	flds	12(%esp)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE469:
	.size	sqrtf, .-sqrtf
	.align 16
	.globl	sqrt
	.type	sqrt, @function
sqrt:
.LFB470:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	flds	32(%esp)
	fld	%st(0)
	fsqrt
	fldz
	fucomip	%st(2), %st
	jbe	.L4584
	fstpl	(%esp)
	subl	$16, %esp
	.cfi_def_cfa_offset 48
	fstpl	(%esp)
	call	sqrt
	fstp	%st(0)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	fldl	(%esp)
	jmp	.L4582
	.align 16
.L4584:
	fstp	%st(1)
.L4582:
	fstps	12(%esp)
	flds	12(%esp)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE470:
	.size	sqrt, .-sqrt
	.align 16
	.globl	powf
	.type	powf, @function
powf:
.LFB471:
	.cfi_startproc
	flds	4(%esp)
	movl	$0, %edx
	flds	8(%esp)
	fld1
	fucomi	%st(2), %st
	setnp	%al
	cmovne	%edx, %eax
	testb	%al, %al
	jne	.L4587
	fucomip	%st(1), %st
	fstp	%st(0)
	setnp	%dl
	cmove	%edx, %eax
	testb	%al, %al
	jne	.L4586
	fstp	%st(0)
	jmp	powf.part.13
	.align 16
.L4587:
	fstp	%st(0)
	fstp	%st(0)
.L4586:
	rep ret
	.cfi_endproc
.LFE471:
	.size	powf, .-powf
	.align 16
	.globl	log10
	.type	log10, @function
log10:
.LFB472:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	flds	32(%esp)
	fld1
	fxch	%st(1)
	fucomi	%st(1), %st
	fstp	%st(1)
	jp	.L4592
	fldz
	jne	.L4594
	fstp	%st(1)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4594:
	.cfi_restore_state
	fstp	%st(0)
.L4592:
	flds	.LC548
	subl	$16, %esp
	.cfi_def_cfa_offset 48
	fsts	4(%esp)
	fstps	24(%esp)
	fstps	(%esp)
	call	powf.part.13
	fsubs	.LC543
	flds	24(%esp)
	fdivrp	%st, %st(1)
	fldl	.LC549
	fmulp	%st, %st(1)
	fstps	28(%esp)
	flds	28(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE472:
	.size	log10, .-log10
	.align 16
	.globl	log
	.type	log, @function
log:
.LFB473:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	flds	32(%esp)
	fld1
	fxch	%st(1)
	fucomi	%st(1), %st
	fstp	%st(1)
	jp	.L4599
	fldz
	jne	.L4601
	fstp	%st(1)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4601:
	.cfi_restore_state
	fstp	%st(0)
.L4599:
	flds	.LC548
	subl	$16, %esp
	.cfi_def_cfa_offset 48
	fsts	4(%esp)
	fstps	28(%esp)
	fstps	(%esp)
	call	powf.part.13
	fsubs	.LC543
	flds	28(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	fdivrp	%st, %st(1)
	ret
	.cfi_endproc
.LFE473:
	.size	log, .-log
	.align 16
	.globl	logx
	.type	logx, @function
logx:
.LFB474:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	flds	32(%esp)
	flds	36(%esp)
	fld1
	fxch	%st(1)
	fucomi	%st(1), %st
	fstp	%st(1)
	jp	.L4610
	fldz
	je	.L4613
	fstp	%st(0)
.L4610:
	fldz
	fucomi	%st(1), %st
	ja	.L4614
	fld1
	fxch	%st(3)
	fucomi	%st(3), %st
	fstp	%st(3)
	jp	.L4615
	jne	.L4616
	fstp	%st(2)
	fxch	%st(1)
.L4605:
	fstps	8(%esp)
	subl	$16, %esp
	.cfi_def_cfa_offset 48
	flds	.LC548
	fsts	4(%esp)
	fstps	20(%esp)
	fstps	(%esp)
	call	powf.part.13
	fsubs	.LC543
	flds	20(%esp)
	fdivrp	%st, %st(1)
	fldl	.LC549
	fmulp	%st, %st(1)
	fstps	28(%esp)
	flds	28(%esp)
	flds	24(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	fdivp	%st, %st(1)
	jmp	.L4602
	.align 16
.L4613:
	fstp	%st(1)
	fstp	%st(1)
	jmp	.L4602
	.align 16
.L4614:
	fstp	%st(1)
	fstp	%st(1)
.L4602:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
.L4615:
	.cfi_restore_state
	fstp	%st(0)
	jmp	.L4611
	.align 16
.L4616:
	fstp	%st(0)
.L4611:
	fstps	8(%esp)
	subl	$16, %esp
	.cfi_def_cfa_offset 48
	flds	.LC548
	fsts	4(%esp)
	fstps	20(%esp)
	fstps	(%esp)
	call	powf.part.13
	fsubs	.LC543
	flds	20(%esp)
	fdivrp	%st, %st(1)
	fldl	.LC549
	fmulp	%st, %st(1)
	fstps	28(%esp)
	flds	28(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	flds	8(%esp)
	fxch	%st(1)
	jmp	.L4605
	.cfi_endproc
.LFE474:
	.size	logx, .-logx
	.align 16
	.globl	exp
	.type	exp, @function
exp:
.LFB475:
	.cfi_startproc
	subl	$44, %esp
	.cfi_def_cfa_offset 48
	flds	48(%esp)
	fstpl	(%esp)
	call	exp
	fstps	28(%esp)
	flds	28(%esp)
	addl	$44, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE475:
	.size	exp, .-exp
	.align 16
	.globl	MPCheckHeader
	.type	MPCheckHeader, @function
MPCheckHeader:
.LFB476:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	8(%esp), %eax
	movl	12(%esp), %ecx
	movzbl	(%eax), %ebx
	cmpb	%bl, (%ecx)
	jne	.L4627
	movl	$1, %edx
.L4621:
	movzbl	(%ecx,%edx), %ebx
	cmpb	%bl, (%eax,%edx)
	jne	.L4627
	addl	$1, %edx
	cmpl	$4, %edx
	jne	.L4621
	movzbl	8(%eax), %edx
	sall	$4, %edx
	testb	%dl, %dl
	leal	-1(%edx), %ecx
	je	.L4633
	movzbl	%cl, %ecx
	xorl	%edx, %edx
	leal	1(%eax,%ecx), %ecx
	.align 16
.L4623:
	addb	(%eax), %dl
	addl	$1, %eax
	cmpl	%ecx, %eax
	jne	.L4623
	xorl	%eax, %eax
	testb	%dl, %dl
	setne	%al
	negl	%eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4627:
	.cfi_restore_state
	movl	$-1, %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L4633:
	.cfi_restore_state
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE476:
	.size	MPCheckHeader, .-MPCheckHeader
	.section	.rodata
	.align 4
.LC550:
	.string	"\nParsing Multi Processing Specification table by Intel!!!\n"
.LC551:
	.string	"_MP_"
	.align 4
.LC552:
	.string	"\n\t%gProcessor #%x Found, LAPIC ID #%x, LAPIC Version #%x%g"
	.align 4
.LC553:
	.string	"\n%gMP Tables NOT FOUND!!! %x%g"
	.align 4
.LC554:
	.string	"\n%gMP Tables FOUND!!! Address: %x%g"
.LC555:
	.string	"\n\t\t%gLAPIC Base address: %x%g"
	.text
	.align 16
	.globl	MPtables_parse
	.type	MPtables_parse, @function
MPtables_parse:
.LFB477:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	$917504, %ebx
	subl	$16, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC550
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L4636
	.align 16
.L4672:
	addl	$4, %ebx
	cmpl	$1048580, %ebx
	je	.L4671
.L4636:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$.LC551
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	MPCheckHeader
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	jne	.L4672
.L4635:
	pushl	$0
	.cfi_def_cfa_offset 20
	pushl	%ebx
	.cfi_def_cfa_offset 24
	xorl	%esi, %esi
	pushl	$10
	.cfi_def_cfa_offset 28
	pushl	$.LC554
	.cfi_def_cfa_offset 32
	call	printf
	movl	4(%ebx), %eax
	pushl	$0
	.cfi_def_cfa_offset 36
	movl	%ebx, mpfp
	pushl	36(%eax)
	.cfi_def_cfa_offset 40
	pushl	$5
	.cfi_def_cfa_offset 44
	leal	44(%eax), %ebx
	pushl	$.LC555
	.cfi_def_cfa_offset 48
	movl	%eax, mpcth
	call	printf
	movl	mpcth, %edx
	addl	$32, %esp
	.cfi_def_cfa_offset 16
	movl	%ebx, Processor_Entries
	cmpw	$0, 34(%edx)
	je	.L4634
	.align 16
.L4658:
	cmpb	$4, (%ebx)
	ja	.L4641
	movzbl	(%ebx), %eax
	jmp	*.L4643(,%eax,4)
	.section	.rodata
	.align 4
	.align 4
.L4643:
	.long	.L4642
	.long	.L4647
	.long	.L4647
	.long	.L4647
	.long	.L4647
	.text
	.align 16
.L4647:
	addl	$8, %ebx
.L4641:
	movzwl	34(%edx), %eax
	addl	$1, %esi
	cmpl	%esi, %eax
	jg	.L4658
.L4634:
	addl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4642:
	.cfi_restore_state
	movl	Processor_Entries, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	addl	$20, %ebx
	pushl	$0
	.cfi_def_cfa_offset 28
	movzbl	2(%eax), %edx
	pushl	%edx
	.cfi_def_cfa_offset 32
	movzbl	1(%eax), %eax
	pushl	%eax
	.cfi_def_cfa_offset 36
	pushl	$0
	.cfi_def_cfa_offset 40
	pushl	$7
	.cfi_def_cfa_offset 44
	pushl	$.LC552
	.cfi_def_cfa_offset 48
	call	printf
	addl	$32, %esp
	.cfi_def_cfa_offset 16
	movl	mpcth, %edx
	jmp	.L4641
	.align 16
.L4671:
	movswl	1038, %ebx
	sall	$4, %ebx
	andl	$1048560, %ebx
	leal	1023(%ebx), %esi
	jmp	.L4638
	.align 16
.L4674:
	addl	$16, %ebx
	cmpl	%esi, %ebx
	jg	.L4673
.L4638:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$.LC551
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	MPCheckHeader
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	jne	.L4674
	jmp	.L4635
	.align 16
.L4673:
	movl	$-4097, %ebx
	jmp	.L4639
	.align 16
.L4676:
	addl	$4, %ebx
	cmpl	$-1, %ebx
	je	.L4675
.L4639:
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	$.LC551
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	MPCheckHeader
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	jne	.L4676
	jmp	.L4635
.L4675:
	pushl	$0
	.cfi_def_cfa_offset 20
	pushl	$-1
	.cfi_def_cfa_offset 24
	pushl	$9
	.cfi_def_cfa_offset 28
	pushl	$.LC553
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	addl	$4, %esp
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE477:
	.size	MPtables_parse, .-MPtables_parse
	.section	.rodata
	.align 4
.LC556:
	.string	"\nSizeof Neuron_t: %x\n%gBuilding Neurons\n"
	.align 4
.LC557:
	.string	"\n%gLinking Neural Networks, Building Random Connections..."
	.align 4
.LC558:
	.string	"\nNeural Network setup complete.\n%gVerification:%g \n"
.LC559:
	.string	"C%x=%x--"
.LC560:
	.string	"P%x=%x--"
	.text
	.align 16
	.globl	NeuralNet_init
	.type	NeuralNet_init, @function
NeuralNet_init:
.LFB478:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$48, %esp
	.cfi_def_cfa_offset 68
	pushl	$9
	.cfi_def_cfa_offset 72
	pushl	$496
	.cfi_def_cfa_offset 76
	pushl	$.LC556
	.cfi_def_cfa_offset 80
	call	printf
/APP
/  15 "Source/NeuralNetwork/Neuron/Neuron.c" 1
	int $50
/  0 "" 2
/  16 "Source/NeuralNetwork/Neuron/Neuron.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	Main_CSI_struct, %edi
	popl	%esi
	.cfi_def_cfa_offset 76
	pushl	12(%edi)
	.cfi_def_cfa_offset 80
	call	StrToInt
	popl	%ebp
	.cfi_def_cfa_offset 76
	pushl	16(%edi)
	.cfi_def_cfa_offset 80
	movl	%eax, %ebx
	call	StrToInt
	popl	%edx
	.cfi_def_cfa_offset 76
	pushl	20(%edi)
	.cfi_def_cfa_offset 80
	movl	%eax, %esi
	call	StrToInt
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	$4, %edx
	testl	%ebx, %ebx
	cmovne	%ebx, %edx
	movl	$15, %ecx
	testl	%eax, %eax
	cmove	%ecx, %eax
	testl	%esi, %esi
	movl	$1, %ecx
	cmove	%ecx, %esi
	cmpl	$1, %edx
	movl	%eax, %ecx
	jle	.L4681
.L4682:
	subl	$1, %edx
	imull	%eax, %ecx
	cmpl	$1, %edx
	jne	.L4682
.L4681:
	movl	%esi, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	imull	%ecx, %eax
	subl	$1, %eax
	movl	%eax, Neurons
	movl	Neurons, %eax
	imull	$496, %eax, %eax
	movl	%eax, NeuralSize
	movl	NeuralSize, %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	*kmalloc
	movl	%eax, NeuralBase
	movl	NeuralBase, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	Neurons, %ecx
	xorl	%edx, %edx
	testl	%ecx, %ecx
	je	.L4687
.L4706:
	xorl	%ecx, %ecx
	movl	%edx, 12(%eax)
	xorl	%ebx, %ebx
	movw	%cx, 10(%eax)
	movl	Neurons, %ecx
	addl	$1, %edx
	movl	$0, (%eax)
	movw	%bx, 8(%eax)
	addl	$496, %eax
	cmpl	%ecx, %edx
	jb	.L4706
.L4687:
	movl	NeuralBase, %eax
	movl	%eax, 20(%esp)
	movl	%eax, %ebx
	call	init_hpet
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	leal	136(%ebx), %edi
	pushl	$12
	.cfi_def_cfa_offset 76
	pushl	$.LC557
	.cfi_def_cfa_offset 80
	call	printf
	movl	Neurons, %eax
	leal	16(%ebx), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	$0, 28(%esp)
	movl	%edx, 16(%esp)
	cmpl	$1, %eax
	je	.L4685
.L4704:
	xorl	%eax, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $112
/  0 "" 2
/  45 "Library/sys.h" 1
	inb $113, %al
/  0 "" 2
/NO_APP
	movzbl	%al, %eax
	addl	$9, %eax
	movl	%eax, seed1
	movl	HPET_main_counter, %eax
	movl	(%eax), %eax
	leal	3(%eax,%eax,2), %eax
	movl	%eax, seed2
	movl	Lapic, %eax
	movl	912(%eax), %eax
	addl	$999, %eax
	movl	%eax, seed3
	movl	16(%esp), %eax
	movl	%eax, 12(%esp)
	leal	-136(%edi), %eax
	movl	%eax, 24(%esp)
	.align 16
.L4690:
	xorl	%esi, %esi
.L4694:
	call	random
	movl	Neurons, %ecx
	xorl	%edx, %edx
	movl	%eax, %ebx
	divl	%ecx
	movl	Neurons, %eax
	cmpl	%eax, %esi
	je	.L4691
	imull	$496, %edx, %edx
	movl	20(%esp), %eax
	leal	(%eax,%edx), %ecx
	movl	16(%esp), %eax
	movzwl	10(%ecx), %ebp
	cmpw	$30, %bp
	je	.L4721
	.align 16
.L4705:
	cmpl	(%eax), %ecx
	je	.L4694
	addl	$4, %eax
	cmpl	%edi, %eax
	jne	.L4705
	movl	12(%esp), %eax
	xorl	%edx, %edx
	movl	%ecx, (%eax)
	movl	Neurons, %esi
	movl	$2, 120(%eax)
	movl	%ebx, %eax
	addw	$1, -128(%edi)
	divl	%esi
	movl	%eax, %ebx
	movl	$67100673, %eax
	mull	%ebx
	movzwl	%bp, %eax
	addl	$1, %ebp
	leal	(%ecx,%eax,4), %eax
	shrl	$7, %edx
	movl	%edx, %esi
	sall	$13, %esi
	addl	%esi, %edx
	movl	24(%esp), %esi
	subl	%edx, %ebx
	addl	$4194304, %ebx
	movl	%ebx, -132(%edi)
	addl	$4, 12(%esp)
	movl	%esi, 256(%eax)
	movl	$2, 376(%eax)
	movl	12(%esp), %eax
	movw	%bp, 10(%ecx)
	cmpl	%edi, %eax
	jne	.L4690
.L4691:
	addl	$1, 28(%esp)
	movl	Neurons, %eax
	addl	$496, %edi
	movl	28(%esp), %ebx
	addl	$496, 16(%esp)
	subl	$1, %eax
	cmpl	%ebx, %eax
	ja	.L4704
.L4685:
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	xorl	%ebx, %ebx
	pushl	$15
	.cfi_def_cfa_offset 72
	pushl	$13
	.cfi_def_cfa_offset 76
	pushl	$.LC558
	.cfi_def_cfa_offset 80
	call	printf
	movl	Neurons, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	20(%esp), %esi
	cmpl	$1, %eax
	jne	.L4703
	jmp	.L4689
	.align 16
.L4696:
	movzwl	10(%esi), %eax
	cmpw	$29, %ax
	jbe	.L4722
.L4697:
	addl	$496, %esi
	movl	12(%esi), %eax
	testl	%eax, %eax
	je	.L4689
.L4723:
	movl	Neurons, %eax
	addl	$1, %ebx
	subl	$1, %eax
	cmpl	%ebx, %eax
	jbe	.L4689
.L4703:
	movzwl	8(%esi), %eax
	cmpw	$29, %ax
	ja	.L4696
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	movzwl	%ax, %eax
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	12(%esi)
	.cfi_def_cfa_offset 76
	pushl	$.LC559
	.cfi_def_cfa_offset 80
	call	printf
	movzwl	10(%esi), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	cmpw	$29, %ax
	ja	.L4697
.L4722:
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	movzwl	%ax, %eax
	addl	$496, %esi
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	-484(%esi)
	.cfi_def_cfa_offset 76
	pushl	$.LC560
	.cfi_def_cfa_offset 80
	call	printf
	movl	12(%esi), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	jne	.L4723
.L4689:
	movl	NeuralBase, %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	movl	%eax, Neuron_New
	movl	NeuralSize, %edx
	movl	NeuralBase, %eax
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	NeuralNet_SaveToFile
/APP
/  103 "Source/NeuralNetwork/Neuron/Neuron.c" 1
	sti
/  0 "" 2
/NO_APP
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4721:
	.cfi_restore_state
	addl	$1, %esi
	jmp	.L4694
	.cfi_endproc
.LFE478:
	.size	NeuralNet_init, .-NeuralNet_init
	.section	.rodata
.LC561:
	.string	"mkdir "
.LC562:
	.string	"mkfl "
.LC563:
	.string	".ann"
	.align 4
.LC564:
	.string	"\n\nSaving the Contents to File...\n"
	.align 4
.LC565:
	.string	"\"%s\" Directory not found, Creating one...\n"
	.align 4
.LC566:
	.string	"File \"%s\" dosent exist, Creating...\n"
.LC567:
	.string	"Size of file: %d\n"
	.align 4
.LC568:
	.string	"Saving the Neural Network to file \"%s\"\n"
.LC569:
	.string	"Saving...\nFile Size: %d\n"
.LC570:
	.string	"Done\n"
	.text
	.align 16
	.globl	NeuralNet_SaveToFile
	.type	NeuralNet_SaveToFile, @function
NeuralNet_SaveToFile:
.LFB479:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subl	$124, %esp
	.cfi_def_cfa_offset 140
	pushl	$.LC564
	.cfi_def_cfa_offset 144
	call	printf
	movl	$annDirName, (%esp)
	call	search_folderOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	testl	%eax, %eax
	leal	62(%esp), %ebx
	je	.L4792
.L4725:
	subl	$12, %esp
	.cfi_def_cfa_offset 140
	pushl	$annCounter
	.cfi_def_cfa_offset 144
	call	file_loadOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	testl	%eax, %eax
	je	.L4793
.L4732:
	subl	$12, %esp
	.cfi_def_cfa_offset 140
	pushl	$annCounter
	.cfi_def_cfa_offset 144
	call	file_size
	popl	%edx
	.cfi_def_cfa_offset 140
	popl	%ecx
	.cfi_def_cfa_offset 136
	pushl	%eax
	.cfi_def_cfa_offset 140
	pushl	$.LC567
	.cfi_def_cfa_offset 144
	movl	%eax, %esi
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	testl	%esi, %esi
	movl	%ebx, %eax
	leal	72(%esp), %edx
	jne	.L4794
	.align 16
.L4770:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L4770
	subl	$4, %esp
	.cfi_def_cfa_offset 132
	pushl	$annCounter
	.cfi_def_cfa_offset 136
	pushl	$10
	.cfi_def_cfa_offset 140
	pushl	%ebx
	.cfi_def_cfa_offset 144
	call	file_writeAppend
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	movl	$1, %eax
.L4741:
	leal	12(%esp), %edi
	movl	$10, %ecx
	movl	%edi, %edx
	call	itoa.part.3
.L4742:
	subl	$12, %esp
	.cfi_def_cfa_offset 140
	pushl	$annCounter
	.cfi_def_cfa_offset 144
	pushl	$10
	.cfi_def_cfa_offset 148
	pushl	%edi
	.cfi_def_cfa_offset 152
	pushl	$10
	.cfi_def_cfa_offset 156
	pushl	$0
	.cfi_def_cfa_offset 160
	call	file_editFM
	addl	$20, %esp
	.cfi_def_cfa_offset 140
	pushl	$annCounter
	.cfi_def_cfa_offset 144
	call	file_closeOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	movzbl	annFileNames, %ecx
	movl	$annFileNames, %edx
	leal	22(%esp), %esi
	movl	%esi, %eax
	.align 16
.L4743:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4743
	movb	$0, (%eax)
	cmpb	$0, 22(%esp)
	je	.L4762
	leal	23(%esp), %edx
	.align 16
.L4745:
	movl	%edx, %eax
	addl	$1, %edx
	cmpb	$0, -1(%edx)
	jne	.L4745
.L4744:
	movzbl	12(%esp), %edx
	.align 16
.L4746:
	addl	$1, %eax
	addl	$1, %edi
	movb	%dl, -1(%eax)
	movzbl	(%edi), %edx
	testb	%dl, %dl
	jne	.L4746
	movb	$0, (%eax)
	cmpb	$0, 22(%esp)
	je	.L4763
	leal	23(%esp), %edx
	.align 16
.L4748:
	movl	%edx, %eax
	addl	$1, %edx
	cmpb	$0, -1(%edx)
	jne	.L4748
.L4747:
	movl	$46, %ecx
	movl	$.LC563, %edx
	.align 16
.L4749:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4749
	subl	$8, %esp
	.cfi_def_cfa_offset 136
	movb	$0, (%eax)
	pushl	%esi
	.cfi_def_cfa_offset 140
	pushl	$.LC568
	.cfi_def_cfa_offset 144
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	movl	$109, %ecx
	movl	$.LC562, %edx
	movl	%ebx, %eax
	.align 16
.L4750:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4750
	movb	$0, (%eax)
	cmpb	$0, 62(%esp)
	je	.L4764
	leal	63(%esp), %edx
	.align 16
.L4752:
	movl	%edx, %eax
	addl	$1, %edx
	cmpb	$0, -1(%edx)
	jne	.L4752
.L4751:
	movzbl	22(%esp), %ecx
	movl	%esi, %edx
	.align 16
.L4753:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4753
	movb	$0, (%eax)
	xorl	%eax, %eax
	cmpb	$0, 62(%esp)
	je	.L4754
	.align 16
.L4755:
	addl	$1, %eax
	cmpb	$0, (%ebx,%eax)
	jne	.L4755
.L4754:
	subl	$8, %esp
	.cfi_def_cfa_offset 136
	pushl	%eax
	.cfi_def_cfa_offset 140
	pushl	%ebx
	.cfi_def_cfa_offset 144
	call	Shell_CFexecute
	movl	%esi, (%esp)
	call	file_loadOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	testl	%eax, %eax
	je	.L4756
	subl	$8, %esp
	.cfi_def_cfa_offset 136
	pushl	140(%esp)
	.cfi_def_cfa_offset 140
	pushl	$.LC569
	.cfi_def_cfa_offset 144
	call	printf
	addl	$12, %esp
	.cfi_def_cfa_offset 132
	pushl	%esi
	.cfi_def_cfa_offset 136
	pushl	140(%esp)
	.cfi_def_cfa_offset 140
	pushl	140(%esp)
	.cfi_def_cfa_offset 144
	call	file_writeAppend
	movl	%esi, (%esp)
	call	file_closeOGP
	addl	$16, %esp
	.cfi_def_cfa_offset 128
.L4756:
	subl	$12, %esp
	.cfi_def_cfa_offset 140
	pushl	$.LC570
	.cfi_def_cfa_offset 144
	call	printf
	subl	$-128, %esp
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4794:
	.cfi_def_cfa_offset 128
	.cfi_offset 3, -16
	.cfi_offset 6, -12
	.cfi_offset 7, -8
	pushl	$annCounter
	.cfi_def_cfa_offset 132
	pushl	%esi
	.cfi_def_cfa_offset 136
	pushl	$0
	.cfi_def_cfa_offset 140
	leal	14(%esp), %esi
	pushl	%esi
	.cfi_def_cfa_offset 144
	call	file_readTM
	movl	%esi, (%esp)
	call	StrToInt
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	addl	$1, %eax
	jne	.L4741
	movb	$48, 12(%esp)
	movb	$0, 13(%esp)
	leal	12(%esp), %edi
	jmp	.L4742
	.align 16
.L4793:
	subl	$8, %esp
	.cfi_def_cfa_offset 136
	pushl	$annCounter
	.cfi_def_cfa_offset 140
	pushl	$.LC566
	.cfi_def_cfa_offset 144
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	movl	$109, %ecx
	movl	$.LC562, %edx
	movl	%ebx, %eax
	.align 16
.L4733:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4733
	movb	$0, (%eax)
	cmpb	$0, 62(%esp)
	je	.L4759
	leal	63(%esp), %edx
	.align 16
.L4735:
	movl	%edx, %eax
	addl	$1, %edx
	cmpb	$0, -1(%edx)
	jne	.L4735
.L4734:
	movzbl	annCounter, %ecx
	movl	$annCounter, %edx
	.align 16
.L4736:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4736
	movb	$0, (%eax)
	xorl	%eax, %eax
	cmpb	$0, 62(%esp)
	je	.L4737
	.align 16
.L4738:
	addl	$1, %eax
	cmpb	$0, (%ebx,%eax)
	jne	.L4738
.L4737:
	subl	$8, %esp
	.cfi_def_cfa_offset 136
	pushl	%eax
	.cfi_def_cfa_offset 140
	pushl	%ebx
	.cfi_def_cfa_offset 144
	call	Shell_CFexecute
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	jmp	.L4732
.L4762:
	movl	%esi, %eax
	jmp	.L4744
.L4763:
	movl	%esi, %eax
	jmp	.L4747
.L4764:
	movl	%ebx, %eax
	jmp	.L4751
.L4792:
	subl	$8, %esp
	.cfi_def_cfa_offset 136
	pushl	$annDirName
	.cfi_def_cfa_offset 140
	pushl	$.LC565
	.cfi_def_cfa_offset 144
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	movl	$109, %ecx
	movl	$.LC561, %edx
	movl	%ebx, %eax
	.align 16
.L4726:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4726
	movb	$0, (%eax)
	cmpb	$0, 62(%esp)
	je	.L4757
	leal	63(%esp), %edx
	.align 16
.L4728:
	movl	%edx, %eax
	addl	$1, %edx
	cmpb	$0, -1(%edx)
	jne	.L4728
.L4727:
	movzbl	annDirName, %ecx
	movl	$annDirName, %edx
	.align 16
.L4729:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L4729
	movb	$0, (%eax)
	xorl	%eax, %eax
	cmpb	$0, 62(%esp)
	je	.L4730
	.align 16
.L4731:
	addl	$1, %eax
	cmpb	$0, (%ebx,%eax)
	jne	.L4731
.L4730:
	subl	$8, %esp
	.cfi_def_cfa_offset 136
	pushl	%eax
	.cfi_def_cfa_offset 140
	pushl	%ebx
	.cfi_def_cfa_offset 144
	call	Shell_CFexecute
	addl	$16, %esp
	.cfi_def_cfa_offset 128
	jmp	.L4725
.L4759:
	movl	%ebx, %eax
	jmp	.L4734
.L4757:
	movl	%ebx, %eax
	jmp	.L4727
	.cfi_endproc
.LFE479:
	.size	NeuralNet_SaveToFile, .-NeuralNet_SaveToFile
	.align 16
	.globl	localapic_read
	.type	localapic_read, @function
localapic_read:
.LFB480:
	.cfi_startproc
	subl	$16, %esp
	.cfi_def_cfa_offset 20
	movl	20(%esp), %edx
	movl	24(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 12(%esp)
	movl	12(%esp), %eax
	movl	(%eax), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE480:
	.size	localapic_read, .-localapic_read
	.align 16
	.globl	localapic_write
	.type	localapic_write, @function
localapic_write:
.LFB481:
	.cfi_startproc
	subl	$16, %esp
	.cfi_def_cfa_offset 20
	movl	20(%esp), %edx
	movl	24(%esp), %eax
	addl	%edx, %eax
	movl	%eax, 12(%esp)
	movl	12(%esp), %eax
	movl	28(%esp), %edx
	movl	%edx, (%eax)
	nop
	addl	$16, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE481:
	.size	localapic_write, .-localapic_write
	.align 16
	.globl	localapic_write_with_mask
	.type	localapic_write_with_mask, @function
localapic_write_with_mask:
.LFB482:
	.cfi_startproc
	movl	APIC_LOCAL_BASE, %eax
	addl	8(%esp), %eax
	movl	12(%esp), %edx
	movl	(%eax), %ecx
	notl	%edx
	andl	%ecx, %edx
	movl	%edx, (%eax)
	movl	(%eax), %edx
	orl	16(%esp), %edx
	movl	%edx, (%eax)
	ret
	.cfi_endproc
.LFE482:
	.size	localapic_write_with_mask, .-localapic_write_with_mask
	.comm	ext2_path,4,4
	.globl	ext2_current_dir_inode
	.section	.bss
	.align 4
	.type	ext2_current_dir_inode, @object
	.size	ext2_current_dir_inode, 4
ext2_current_dir_inode:
	.zero	4
	.comm	ext2_root,4,4
	.comm	ext2_root_name,4,4
	.globl	ext2_g_sblock
	.align 4
	.type	ext2_g_sblock, @object
	.size	ext2_g_sblock, 4
ext2_g_sblock:
	.zero	4
	.globl	ext2_g_gdesc
	.align 4
	.type	ext2_g_gdesc, @object
	.size	ext2_g_gdesc, 4
ext2_g_gdesc:
	.zero	4
	.globl	ext2_g_inode_table
	.align 4
	.type	ext2_g_inode_table, @object
	.size	ext2_g_inode_table, 4
ext2_g_inode_table:
	.zero	4
	.globl	ext2_g_bb
	.align 4
	.type	ext2_g_bb, @object
	.size	ext2_g_bb, 4
ext2_g_bb:
	.zero	4
	.globl	ext2_g_ib
	.align 4
	.type	ext2_g_ib, @object
	.size	ext2_g_ib, 4
ext2_g_ib:
	.zero	4
	.globl	_Rlogged
	.data
	.align 4
	.type	_Rlogged, @object
	.size	_Rlogged, 4
_Rlogged:
	.long	256
	.globl	_Wlogged
	.align 4
	.type	_Wlogged, @object
	.size	_Wlogged, 4
_Wlogged:
	.long	128
	.globl	_Xlogged
	.align 4
	.type	_Xlogged, @object
	.size	_Xlogged, 4
_Xlogged:
	.long	64
	.local	dirent
	.comm	dirent,12,4
	.text
	.align 16
	.globl	shiftData
	.type	shiftData, @function
shiftData:
.LFB483:
	.cfi_startproc
	movl	8(%esp), %edx
	movl	4(%esp), %eax
	movl	12(%esp), %ecx
	testl	%edx, %edx
	js	.L4801
	rep ret
	.align 16
.L4801:
	jmp	shiftData.part.14
	.cfi_endproc
.LFE483:
	.size	shiftData, .-shiftData
	.align 16
	.globl	ext2_read
	.type	ext2_read, @function
ext2_read:
.LFB484:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	12(%esp), %eax
	movl	16(%esp), %edx
	movl	20(%esp), %ecx
	movl	24(%esp), %esi
	movzwl	8(%eax), %ebx
	testl	%ebx, _Rlogged
	je	.L4803
	movl	%esi, 12(%esp)
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	jmp	ext2_read.part.17
	.align 16
.L4803:
	.cfi_restore_state
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE484:
	.size	ext2_read, .-ext2_read
	.section	.rodata
	.align 4
.LC571:
	.string	"Sizeof ext2_superblock_t = %x\n"
	.text
	.align 16
	.globl	ext2_read_meta_data
	.type	ext2_read_meta_data, @function
ext2_read_meta_data:
.LFB485:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	pushl	$204
	.cfi_def_cfa_offset 48
	call	*kmalloc
	movl	curr_port, %esi
	movl	$1, (%esp)
	movl	%eax, %ebx
	pushl	$0
	.cfi_def_cfa_offset 52
	pushl	$2
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%eax
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%esi
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	jne	.L4817
.L4806:
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	$204
	.cfi_def_cfa_offset 44
	pushl	$.LC571
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	cmpw	$-4269, 56(%ebx)
	movl	$1, %eax
	je	.L4818
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4817:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L4806
	.align 16
.L4818:
	movl	4(%ebx), %eax
	xorl	%edx, %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	divl	32(%ebx)
	sall	$5, %eax
	movl	%eax, %esi
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	*kmalloc
	movl	%eax, %edi
	movl	%esi, %eax
	movl	curr_port, %ebp
	shrl	$9, %eax
	shrl	$13, %esi
	addl	$1, %eax
	movzwl	%si, %esi
	movl	%eax, (%esp)
	addl	$1, %esi
	sall	$9, %eax
	pushl	$0
	.cfi_def_cfa_offset 52
	pushl	$4
	.cfi_def_cfa_offset 56
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	pushl	%edi
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%ebp
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	jne	.L4819
.L4808:
	movl	32(%esp), %eax
	movl	%ebx, (%eax)
	movl	36(%esp), %eax
	movl	%edi, (%eax)
	movl	%ebx, ext2_g_sblock
	xorl	%eax, %eax
	movl	%edi, ext2_g_gdesc
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4819:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L4808
	.cfi_endproc
.LFE485:
	.size	ext2_read_meta_data, .-ext2_read_meta_data
	.align 16
	.globl	ext2_format_block_bitmap
	.type	ext2_format_block_bitmap, @function
ext2_format_block_bitmap:
.LFB486:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	ext2_g_bb, %esi
	movl	64(%esp), %edx
	movl	(%edx), %eax
	movl	%eax, 16(%esp)
	movl	20(%edx), %eax
	sall	$10, %eax
	shrl	$9, %eax
	addl	8(%edx), %eax
	testl	%esi, %esi
	movl	%eax, 8(%esp)
	je	.L4859
.L4821:
	movl	68(%esp), %eax
	testl	%eax, %eax
	je	.L4822
	movl	68(%esp), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	sall	$2, %eax
	movl	%eax, 36(%esp)
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	%eax, %edi
	call	*kmalloc
	movl	%eax, 44(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%edi, %edi
	je	.L4823
	movl	%eax, %edx
	addl	%edi, %edx
	.align 16
.L4824:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L4824
.L4823:
	movl	68(%esp), %ebx
	xorl	%ebp, %ebp
	xorl	%eax, %eax
.L4837:
	movzbl	(%esi,%ebp), %ecx
	movl	$128, %edx
	movb	%cl, 15(%esp)
	movzbl	%cl, %edi
	xorl	%ecx, %ecx
	jmp	.L4836
	.align 16
.L4862:
	addl	$1, %eax
	cmpl	%eax, %ebx
	je	.L4860
	addl	$1, %ecx
	shrl	%edx
	cmpl	$8, %ecx
	je	.L4861
.L4836:
	testl	%edi, %edx
	je	.L4862
	addl	$1, %ecx
	xorl	%eax, %eax
	shrl	%edx
	cmpl	$8, %ecx
	jne	.L4836
.L4861:
	addl	$1, %ebp
	cmpl	$1024, %ebp
	jne	.L4837
.L4822:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%edi, %edi
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	movl	%edi, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4860:
	.cfi_restore_state
	movl	68(%esp), %edi
	movl	%edx, 20(%esp)
	movl	24(%esp), %edx
	movzbl	15(%esp), %ebx
	sall	$2, %edi
	subl	%edi, %edx
	movl	28(%esp), %edi
	addl	%edx, %edi
	movl	20(%esp), %edx
	jmp	.L4829
	.align 16
.L4863:
	movzbl	(%esi,%ebp), %ebx
.L4829:
	orl	%edx, %ebx
	addl	%edx, %edx
	movb	%bl, (%esi,%ebp)
	leal	(%ecx,%ebp,8), %ebx
	subl	$1, %ecx
	sall	$10, %ebx
	shrl	$9, %ebx
	addl	8(%esp), %ebx
	cmpl	$128, %edx
	movl	%ebx, -4(%edi,%eax,4)
	jbe	.L4827
	subl	$1, %ebp
	movl	$1, %edx
	movl	$7, %ecx
.L4827:
	subl	$1, %eax
	jne	.L4863
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$3
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	36(%esp)
	.cfi_def_cfa_offset 88
	pushl	$1536
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%esi
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	jne	.L4864
.L4830:
	movl	68(%esp), %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	sall	$10, %ebx
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%ebx, %ebx
	leal	(%eax,%ebx), %ecx
	movl	%eax, %edx
	je	.L4835
	.align 16
.L4847:
	addl	$1, %edx
	movb	$0, -1(%edx)
	cmpl	%ecx, %edx
	jne	.L4847
.L4835:
	movl	%ebx, %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	curr_port, %esi
	shrl	$9, %edx
	shrl	$13, %ebx
	addl	$1, %edx
	pushl	%edx
	.cfi_def_cfa_offset 80
	sall	$9, %edx
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	(%edi)
	.cfi_def_cfa_offset 88
	pushl	%edx
	.cfi_def_cfa_offset 92
	movzwl	%bx, %edx
	addl	$1, %edx
	pushl	%edx
	.cfi_def_cfa_offset 96
	pushl	%eax
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	jne	.L4865
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%edi, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4865:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	%edi, %eax
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4864:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L4830
.L4859:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1024
	.cfi_def_cfa_offset 80
	call	*kmalloc
	movl	curr_port, %ebx
	movl	$3, (%esp)
	movl	%eax, %esi
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	36(%esp)
	.cfi_def_cfa_offset 88
	pushl	$1536
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%eax
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	movl	%eax, ext2_g_bb
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L4821
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L4821
	.cfi_endproc
.LFE486:
	.size	ext2_format_block_bitmap, .-ext2_format_block_bitmap
	.align 16
	.globl	ext2_singly_create
	.type	ext2_singly_create, @function
ext2_singly_create:
.LFB487:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	24(%esp), %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$1024
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	%eax, %ebx
	popl	%eax
	.cfi_def_cfa_offset 28
	popl	%edx
	.cfi_def_cfa_offset 24
	pushl	$1
	.cfi_def_cfa_offset 28
	pushl	40(%esp)
	.cfi_def_cfa_offset 32
	call	ext2_format_block_bitmap
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%eax, %edi
	movl	20(%esp), %edx
	movl	16(%esp), %eax
	cmpl	$256, %esi
	movl	$256, %ecx
	cmovbe	%esi, %ecx
	leal	(%eax,%edx,4), %esi
	xorl	%eax, %eax
	testl	%ecx, %ecx
	je	.L4871
	.align 16
.L4872:
	movl	(%esi,%eax,4), %edx
	movl	%edx, (%ebx,%eax,4)
	addl	$1, %eax
	cmpl	%ecx, %eax
	jne	.L4872
.L4871:
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$3
	.cfi_def_cfa_offset 32
	pushl	$0
	.cfi_def_cfa_offset 36
	pushl	(%edi)
	.cfi_def_cfa_offset 40
	pushl	$1536
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%ebx
	.cfi_def_cfa_offset 52
	pushl	$1
	.cfi_def_cfa_offset 56
	pushl	$48
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	je	.L4869
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC215
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L4869:
	movl	(%edi), %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE487:
	.size	ext2_singly_create, .-ext2_singly_create
	.align 16
	.globl	ext2_doubly_create
	.type	ext2_doubly_create, @function
ext2_doubly_create:
.LFB488:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	leal	1024(%eax), %edx
	movl	%eax, %ebx
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	.align 16
.L4878:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L4878
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	movl	$65536, %esi
	pushl	$1
	.cfi_def_cfa_offset 60
	pushl	72(%esp)
	.cfi_def_cfa_offset 64
	call	ext2_format_block_bitmap
	movl	%eax, 28(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	52(%esp), %eax
	cmpl	$65536, 56(%esp)
	cmovbe	56(%esp), %esi
	leal	-256(%eax), %ebp
	movzbl	56(%esp), %eax
	movl	%esi, %ecx
	movl	%esi, 4(%esp)
	xorl	%esi, %esi
	testl	%ecx, %ecx
	movl	%eax, 8(%esp)
	je	.L4884
	.align 16
.L4886:
	movl	%esi, %eax
	addl	$256, %esi
	shrl	$8, %eax
	cmpl	%esi, 56(%esp)
	leal	(%ebx,%eax,4), %edi
	movl	$256, %eax
	cmovb	8(%esp), %eax
	pushl	60(%esp)
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	leal	0(%ebp,%esi), %eax
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	60(%esp)
	.cfi_def_cfa_offset 64
	call	ext2_singly_create
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, (%edi)
	cmpl	4(%esp), %esi
	jb	.L4886
.L4884:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	curr_port, %esi
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	movl	32(%esp), %eax
	pushl	(%eax)
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ebx
	.cfi_def_cfa_offset 84
	pushl	$1
	.cfi_def_cfa_offset 88
	pushl	$48
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L4881
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC215
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L4881:
	movl	12(%esp), %eax
	movl	(%eax), %eax
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE488:
	.size	ext2_doubly_create, .-ext2_doubly_create
	.align 16
	.globl	ext2_inode_entry_blocks
	.type	ext2_inode_entry_blocks, @function
ext2_inode_entry_blocks:
.LFB489:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	64(%esp), %edi
	movl	72(%esp), %esi
	leal	52(%edi), %eax
	leal	112(%edi), %edx
	.align 16
.L4893:
	movl	$0, (%eax)
	addl	$4, %eax
	cmpl	%edx, %eax
	jne	.L4893
	movl	76(%esp), %eax
	testl	%eax, %eax
	je	.L4892
	cmpl	$12, 76(%esp)
	movl	$12, %ecx
	cmovbe	76(%esp), %ecx
	xorl	%eax, %eax
	.align 16
.L4895:
	movl	(%esi,%eax,4), %edx
	movl	%edx, 52(%edi,%eax,4)
	addl	$1, %eax
	cmpl	%ecx, %eax
	jne	.L4895
	cmpl	$11, 76(%esp)
	ja	.L4943
.L4892:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4943:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1024
	.cfi_def_cfa_offset 80
	call	*kmalloc
	movl	%eax, 24(%esp)
	movl	%eax, %ebp
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	addl	$1024, %eax
	.align 16
.L4896:
	addl	$1, %ebp
	movb	$0, -1(%ebp)
	cmpl	%eax, %ebp
	jne	.L4896
	movl	76(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	subl	$12, %eax
	movl	%eax, 24(%esp)
	pushl	$1
	.cfi_def_cfa_offset 76
	pushl	80(%esp)
	.cfi_def_cfa_offset 80
	call	ext2_format_block_bitmap
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	%eax, %ebx
	movl	$256, %edx
	movl	16(%esp), %eax
	cmpl	$256, %eax
	cmovbe	%eax, %edx
	xorl	%eax, %eax
	testl	%edx, %edx
	je	.L4901
	movl	8(%esp), %ecx
	movl	%ebx, 12(%esp)
	.align 16
.L4924:
	movl	48(%esi,%eax,4), %ebx
	movl	%ebx, (%ecx,%eax,4)
	addl	$1, %eax
	cmpl	%edx, %eax
	jne	.L4924
	movl	12(%esp), %ebx
.L4901:
	movl	curr_port, %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$3
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	(%ebx)
	.cfi_def_cfa_offset 88
	pushl	$1536
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	40(%esp)
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%edx
	.cfi_def_cfa_offset 112
	movl	%edx, 60(%esp)
	call	SATA_Commander
	movl	60(%esp), %edx
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	movl	16(%edx), %eax
	testl	$1073741824, %eax
	jne	.L4944
.L4899:
	movl	(%ebx), %eax
	movl	8(%esp), %ebx
	movl	%eax, 100(%edi)
	movl	76(%esp), %eax
	subl	$268, %eax
	cmpl	$256, 16(%esp)
	movl	%eax, 12(%esp)
	jbe	.L4892
	.align 16
.L4923:
	addl	$1, %ebx
	movb	$0, -1(%ebx)
	cmpl	%ebx, %ebp
	jne	.L4923
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	$1
	.cfi_def_cfa_offset 76
	pushl	80(%esp)
	.cfi_def_cfa_offset 80
	call	ext2_format_block_bitmap
	movl	%eax, 40(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	12(%esp), %ecx
	movl	$65536, %eax
	movl	%edi, 64(%esp)
	movl	68(%esp), %edi
	movl	%ebx, 28(%esp)
	cmpl	$65536, %ecx
	cmovbe	%ecx, %eax
	movzbl	%cl, %ecx
	movl	%eax, 16(%esp)
	movl	%ecx, 20(%esp)
	xorl	%eax, %eax
	.align 16
.L4905:
	movl	8(%esp), %ecx
	movl	%eax, %edx
	leal	256(%eax), %ebp
	shrl	$8, %edx
	cmpl	%ebp, 12(%esp)
	leal	(%ecx,%edx,4), %ebx
	movl	$256, %ecx
	cmovb	20(%esp), %ecx
	addl	$12, %eax
	pushl	%edi
	.cfi_def_cfa_offset 68
	pushl	%ecx
	.cfi_def_cfa_offset 72
	pushl	%eax
	.cfi_def_cfa_offset 76
	pushl	%esi
	.cfi_def_cfa_offset 80
	call	ext2_singly_create
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	%eax, (%ebx)
	movl	%ebp, %eax
	cmpl	16(%esp), %ebp
	jb	.L4905
	movl	28(%esp), %ebx
	movl	64(%esp), %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$3
	.cfi_def_cfa_offset 80
	movl	curr_port, %ebp
	pushl	$0
	.cfi_def_cfa_offset 84
	movl	44(%esp), %eax
	pushl	(%eax)
	.cfi_def_cfa_offset 88
	pushl	$1536
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	40(%esp)
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebp
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	jne	.L4945
.L4907:
	movl	24(%esp), %eax
	cmpl	$65536, 12(%esp)
	movl	(%eax), %eax
	movl	%eax, 104(%edi)
	jbe	.L4892
	movl	76(%esp), %eax
	subl	$16777484, %eax
	movl	%eax, 12(%esp)
	movl	8(%esp), %eax
	.align 16
.L4911:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %ebx
	jne	.L4911
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	$1
	.cfi_def_cfa_offset 76
	pushl	80(%esp)
	.cfi_def_cfa_offset 80
	call	ext2_format_block_bitmap
	movl	%eax, 40(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	12(%esp), %ecx
	movl	$16777216, %eax
	cmpl	$16777216, %ecx
	cmovbe	%ecx, %eax
	movzwl	%cx, %ecx
	movl	%eax, %edx
	movl	%eax, 16(%esp)
	xorl	%eax, %eax
	testl	%edx, %edx
	movl	%ecx, 20(%esp)
	je	.L4917
	movl	%edi, 64(%esp)
	movl	68(%esp), %edi
	.align 16
.L4922:
	movl	8(%esp), %ecx
	movl	%eax, %edx
	leal	65536(%eax), %ebp
	shrl	$8, %edx
	cmpl	%ebp, 12(%esp)
	leal	(%ecx,%edx,4), %ebx
	movl	$65536, %ecx
	cmovb	20(%esp), %ecx
	addl	$268, %eax
	pushl	%edi
	.cfi_def_cfa_offset 68
	pushl	%ecx
	.cfi_def_cfa_offset 72
	pushl	%eax
	.cfi_def_cfa_offset 76
	pushl	%esi
	.cfi_def_cfa_offset 80
	call	ext2_doubly_create
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	%eax, (%ebx)
	movl	%ebp, %eax
	cmpl	%ebp, 16(%esp)
	ja	.L4922
	movl	64(%esp), %edi
.L4917:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	curr_port, %ebx
	pushl	$3
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	movl	44(%esp), %eax
	pushl	(%eax)
	.cfi_def_cfa_offset 88
	pushl	$1536
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	40(%esp)
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	jne	.L4946
.L4914:
	movl	24(%esp), %eax
	movl	(%eax), %eax
	movl	%eax, 108(%edi)
	jmp	.L4892
.L4944:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L4899
.L4945:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L4907
.L4946:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L4914
	.cfi_endproc
.LFE489:
	.size	ext2_inode_entry_blocks, .-ext2_inode_entry_blocks
	.align 16
	.globl	ext2_data_to_inode_table
	.type	ext2_data_to_inode_table, @function
ext2_data_to_inode_table:
.LFB490:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	movl	ext2_g_inode_table, %edi
	movl	32(%esp), %ebx
	movl	36(%esp), %ebp
	testl	%edi, %edi
	je	.L4965
.L4948:
	xorl	%esi, %esi
	cmpw	$0, 42(%edi)
	movl	40(%esp), %eax
	movl	%edi, %ecx
	movl	40(%eax), %edx
	je	.L4950
	testl	%edx, %edx
	je	.L4955
	leal	140(%edi), %eax
	xorl	%esi, %esi
	jmp	.L4952
	.align 16
.L4966:
	addl	$140, %eax
	cmpl	%edx, %esi
	je	.L4955
.L4952:
	addl	$1, %esi
	cmpw	$0, 42(%eax)
	movl	%eax, %ecx
	jne	.L4966
.L4950:
	cmpl	%esi, %edx
	je	.L4955
	xorl	%eax, %eax
	.align 16
.L4953:
	movzbl	(%ebx,%eax), %edx
	movb	%dl, (%ecx,%eax)
	addl	$1, %eax
	cmpl	$140, %eax
	jne	.L4953
	movl	20(%ebp), %eax
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	sall	$10, %eax
	movl	%eax, %edx
	shrl	$13, %eax
	shrl	$9, %edx
	movzwl	%ax, %eax
	addl	$1, %edx
	addl	$1, %eax
	pushl	%edx
	.cfi_def_cfa_offset 48
	sall	$9, %edx
	pushl	$0
	.cfi_def_cfa_offset 52
	pushl	8(%ebp)
	.cfi_def_cfa_offset 56
	pushl	%edx
	.cfi_def_cfa_offset 60
	pushl	%eax
	.cfi_def_cfa_offset 64
	pushl	%edi
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	jne	.L4967
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%esi, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L4967:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%esi, %eax
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L4955:
	.cfi_restore_state
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %esi
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	movl	%esi, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L4965:
	.cfi_restore_state
	movl	20(%ebp), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	sall	$10, %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	*kmalloc
	movl	%eax, %edi
	movl	%eax, ext2_g_inode_table
	movl	20(%ebp), %eax
	movl	curr_port, %esi
	sall	$10, %eax
	movl	%eax, %ecx
	shrl	$13, %eax
	shrl	$9, %ecx
	movzwl	%ax, %eax
	addl	$1, %ecx
	addl	$1, %eax
	movl	%ecx, (%esp)
	sall	$9, %ecx
	pushl	$0
	.cfi_def_cfa_offset 52
	pushl	8(%ebp)
	.cfi_def_cfa_offset 56
	pushl	%ecx
	.cfi_def_cfa_offset 60
	pushl	%eax
	.cfi_def_cfa_offset 64
	pushl	%edi
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%esi
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L4948
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L4948
	.cfi_endproc
.LFE490:
	.size	ext2_data_to_inode_table, .-ext2_data_to_inode_table
	.align 16
	.globl	ext2_inode_from_inode_table
	.type	ext2_inode_from_inode_table, @function
ext2_inode_from_inode_table:
.LFB491:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
	movl	ext2_g_inode_table, %eax
	movl	20(%esp), %ebx
	testl	%eax, %eax
	je	.L4973
.L4969:
	imull	$140, 16(%esp), %ecx
	addl	%eax, %ecx
	xorl	%eax, %eax
	.align 16
.L4970:
	movzbl	(%ecx,%eax), %edx
	movb	%dl, (%ebx,%eax)
	addl	$1, %eax
	cmpl	$140, %eax
	jne	.L4970
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L4973:
	.cfi_restore_state
	movl	24(%esp), %eax
	call	ext2_get_inode_table.part.22
	movl	%eax, ext2_g_inode_table
	jmp	.L4969
	.cfi_endproc
.LFE491:
	.size	ext2_inode_from_inode_table, .-ext2_inode_from_inode_table
	.align 16
	.globl	ext2_file_from_dir
	.type	ext2_file_from_dir, @function
ext2_file_from_dir:
.LFB492:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	ext2_g_sblock, %edx
	movl	48(%esp), %edi
	movl	52(%esp), %esi
	testl	%edx, %edx
	je	.L4975
	movl	ext2_g_gdesc, %eax
	testl	%eax, %eax
	je	.L4975
	movl	%edx, 8(%esp)
	movl	%eax, 12(%esp)
.L4978:
	movl	ext2_g_inode_table, %ebx
	testl	%ebx, %ebx
	je	.L5005
.L4980:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$140
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	12(%edi), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	$2, %eax
	jne	.L4977
	movl	8(%esp), %edx
	movl	(%edx), %ecx
	testl	%ecx, %ecx
	je	.L4977
	xorl	%ebx, %ebx
	jmp	.L4988
	.align 16
.L5006:
	addl	$1, %ebx
	cmpl	%ebx, (%edx)
	jbe	.L4977
.L4982:
	movl	12(%edi), %eax
.L4988:
	cmpl	$2, %eax
	jne	.L5006
	movl	%ebx, %edx
	movl	%edi, %eax
	call	ext2_dirent_from_dir.part.18
	testl	%eax, %eax
	movl	%eax, %ebp
	je	.L5004
	movl	8(%eax), %edx
	movzbl	(%esi), %ecx
	cmpb	(%edx), %cl
	jne	.L5004
	testb	%cl, %cl
	je	.L4984
	movl	%esi, %ecx
	jmp	.L4986
	.align 16
.L4987:
	testb	%al, %al
	je	.L4984
.L4986:
	addl	$1, %ecx
	addl	$1, %edx
	movzbl	(%ecx), %eax
	cmpb	(%edx), %al
	je	.L4987
.L5004:
	movl	8(%esp), %edx
	addl	$1, %ebx
	cmpl	%ebx, (%edx)
	ja	.L4982
.L4977:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L4984:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	0(%ebp)
	.cfi_def_cfa_offset 64
	call	ext2_inode_from_offset
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L4975:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 60
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	jne	.L4977
	movl	12(%esp), %eax
	jmp	.L4978
.L5005:
	call	ext2_get_inode_table.part.22
	jmp	.L4980
	.cfi_endproc
.LFE492:
	.size	ext2_file_from_dir, .-ext2_file_from_dir
	.align 16
	.globl	ext2_dirent_from_dir_data
	.type	ext2_dirent_from_dir_data, @function
ext2_dirent_from_dir_data:
.LFB493:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$4, %esp
	.cfi_def_cfa_offset 16
	movl	16(%esp), %eax
	movl	20(%esp), %ecx
	movl	24(%esp), %esi
	cmpl	$2, 12(%eax)
	jne	.L5015
	testl	%esi, %esi
	je	.L5015
	testl	%ecx, %ecx
	je	.L5016
	movzwl	4(%esi), %eax
	xorl	%edx, %edx
	xorl	%ebx, %ebx
	testw	%ax, %ax
	jne	.L5018
	jmp	.L5015
	.align 16
.L5014:
	movzwl	4(%esi,%ebx), %eax
	testw	%ax, %ax
	je	.L5015
.L5018:
	movzwl	%ax, %eax
	addl	$1, %edx
	addl	%eax, %ebx
	cmpl	%edx, %ecx
	jne	.L5014
	leal	4(%ebx), %eax
.L5009:
	movzwl	(%esi,%eax), %edx
	xorl	%eax, %eax
	testw	%dx, %dx
	je	.L5007
	movl	(%esi,%ebx), %eax
	movw	%dx, dirent2.8022+4
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	%eax, dirent2.8022
	movzbl	6(%esi,%ebx), %eax
	movb	%al, dirent2.8022+6
	movzbl	7(%esi,%ebx), %edx
	movzbl	%al, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 32
	movb	%dl, dirent2.8022+7
	call	*kmalloc
	movzbl	dirent2.8022+6, %edx
	movl	%eax, dirent2.8022+8
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	leal	1(%eax,%edx), %edx
	.align 16
.L5011:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %edx
	jne	.L5011
	movzbl	dirent2.8022+6, %ecx
	leal	8(%esi,%ebx), %esi
	movl	dirent2.8022+8, %ebx
	xorl	%eax, %eax
	addl	$1, %ecx
	.align 16
.L5012:
	movzbl	(%esi,%eax), %edx
	movb	%dl, (%ebx,%eax)
	addl	$1, %eax
	cmpl	%eax, %ecx
	jne	.L5012
	movzbl	dirent2.8022+6, %eax
	movl	dirent2.8022+8, %edx
	movb	$0, (%edx,%eax)
	movl	$dirent2.8022, %eax
.L5007:
	addl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5015:
	.cfi_restore_state
	addl	$4, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
.L5016:
	.cfi_restore_state
	xorl	%ebx, %ebx
	movl	$4, %eax
	jmp	.L5009
	.cfi_endproc
.LFE493:
	.size	ext2_dirent_from_dir_data, .-ext2_dirent_from_dir_data
	.align 16
	.globl	ext2_dirent_from_dir
	.type	ext2_dirent_from_dir, @function
ext2_dirent_from_dir:
.LFB494:
	.cfi_startproc
	movl	4(%esp), %eax
	movl	8(%esp), %edx
	cmpl	$2, 12(%eax)
	jne	.L5025
	jmp	ext2_dirent_from_dir.part.18
	.align 16
.L5025:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE494:
	.size	ext2_dirent_from_dir, .-ext2_dirent_from_dir
	.align 16
	.globl	ext2_block_of_set
	.type	ext2_block_of_set, @function
ext2_block_of_set:
.LFB495:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	52(%esp), %eax
	cmpl	$11, %eax
	ja	.L5027
	movl	48(%esp), %ecx
	leal	(%ecx,%eax,4), %ebx
	movl	52(%ebx), %eax
	testl	%eax, %eax
	jne	.L5028
.L5032:
	xorl	%ebx, %ebx
.L5026:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5027:
	.cfi_restore_state
	leal	-12(%eax), %ebx
	cmpl	$255, %ebx
	jbe	.L5102
	leal	-268(%eax), %ebx
	cmpl	$65535, %ebx
	jbe	.L5103
	leal	-65804(%eax), %ebx
	cmpl	$16777215, %ebx
	ja	.L5032
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	$1024, (%esp)
	movl	%eax, %esi
	call	*kmalloc
	movl	$1024, (%esp)
	movl	%eax, %ebp
	call	*kmalloc
	movl	%eax, %edi
	movl	64(%esp), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	108(%eax), %eax
	testl	%eax, %eax
	je	.L5032
	movl	curr_port, %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%esi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%edx
	.cfi_def_cfa_offset 96
	movl	%edx, 60(%esp)
	call	SATA_Commander
	movl	60(%esp), %edx
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	movl	16(%edx), %eax
	testl	$1073741824, %eax
	jne	.L5104
.L5037:
	movl	%ebx, %eax
	shrl	$16, %eax
	movl	(%esi,%eax,4), %eax
	testl	%eax, %eax
	je	.L5032
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ebp
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5105
.L5038:
	movzbl	%bh, %eax
	movl	0(%ebp,%eax,4), %eax
	testl	%eax, %eax
	je	.L5032
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5106
.L5039:
	movzbl	%bl, %eax
	movl	(%edi,%eax,4), %ebx
	testl	%ebx, %ebx
	je	.L5032
	jmp	.L5100
	.align 16
.L5028:
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	88(%esp)
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L5030
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L5030:
	movl	52(%ebx), %ebx
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5102:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%eax, %esi
	movl	64(%esp), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	100(%eax), %eax
	testl	%eax, %eax
	je	.L5032
	movl	curr_port, %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%esi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%edi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5107
.L5033:
	movl	(%esi,%ebx,4), %ebx
	testl	%ebx, %ebx
	je	.L5032
	jmp	.L5100
	.align 16
.L5103:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	$1024, (%esp)
	movl	%eax, %esi
	call	*kmalloc
	movl	%eax, %edi
	movl	64(%esp), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	104(%eax), %eax
	testl	%eax, %eax
	je	.L5032
	movl	curr_port, %ebp
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebp
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5108
.L5035:
	movl	%ebx, %eax
	shrl	$8, %eax
	movl	(%edi,%eax,4), %eax
	testl	%eax, %eax
	je	.L5032
	movl	curr_port, %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%esi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%edi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5109
.L5036:
	movzbl	%bl, %eax
	movl	(%esi,%eax,4), %ebx
	testl	%ebx, %ebx
	je	.L5032
.L5100:
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%ebx
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	88(%esp)
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L5026
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5026
	.align 16
.L5107:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5033
	.align 16
.L5108:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5035
	.align 16
.L5104:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5037
.L5109:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5036
.L5105:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5038
.L5106:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5039
	.cfi_endproc
.LFE495:
	.size	ext2_block_of_set, .-ext2_block_of_set
	.align 16
	.globl	ext2_write_block_of_set
	.type	ext2_write_block_of_set, @function
ext2_write_block_of_set:
.LFB496:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	movl	20(%esp), %ebx
	movl	8(%esp), %eax
	movl	12(%esp), %edx
	movl	16(%esp), %ecx
	testl	%ebx, %ebx
	je	.L5111
	movl	%ebx, 8(%esp)
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	jmp	ext2_write_block_of_set.part.19
	.align 16
.L5111:
	.cfi_restore_state
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE496:
	.size	ext2_write_block_of_set, .-ext2_write_block_of_set
	.align 16
	.globl	ext2_add_file_to_dir
	.type	ext2_add_file_to_dir, @function
ext2_add_file_to_dir:
.LFB497:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$60, %esp
	.cfi_def_cfa_offset 80
	movl	92(%esp), %edi
	movl	80(%esp), %esi
	movl	84(%esp), %ecx
	movl	88(%esp), %eax
	cmpb	$0, (%edi)
	je	.L5114
	xorl	%edx, %edx
	.align 16
.L5115:
	addl	$1, %edx
	cmpb	$0, (%edi,%edx)
	jne	.L5115
	cmpl	$255, %edx
	movl	$255, %ebx
	movb	%al, 43(%esp)
	cmovbe	%edx, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	movl	%ebx, %edx
	movl	%ebx, 36(%esp)
	movl	%ebx, %ebp
	leal	9(%edx), %ebx
	movl	4(%ecx), %edx
	movl	%ebp, %eax
	addl	$1, %eax
	movw	%bx, 52(%esp)
	movl	%edx, 48(%esp)
	movl	%ebp, %edx
	movb	%dl, 54(%esp)
	pushl	%eax
	.cfi_def_cfa_offset 96
	call	*kmalloc
	movl	%eax, %edx
	movl	%eax, 44(%esp)
	movl	%eax, 60(%esp)
	addl	%ebp, %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	.align 16
.L5116:
	addl	$1, %edi
	movzbl	-1(%edi), %ecx
	addl	$1, %eax
	cmpl	%edx, %eax
	movb	%cl, -1(%eax)
	jne	.L5116
.L5118:
	movl	28(%esp), %eax
	movl	24(%esp), %edx
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	movb	$0, (%eax,%edx)
	pushl	$1024
	.cfi_def_cfa_offset 96
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movzwl	%bx, %ecx
	testl	%eax, %eax
	movl	%eax, %edi
	sete	19(%esp)
	movl	%ecx, %ebp
	movl	$0, 12(%esp)
	.align 16
.L5117:
	movl	%ecx, 8(%esp)
	subl	$4, %esp
	.cfi_def_cfa_offset 84
	pushl	%edi
	.cfi_def_cfa_offset 88
	pushl	20(%esp)
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	ext2_block_of_set
	movl	%eax, 36(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	testl	%eax, %eax
	je	.L5124
	cmpb	$0, 19(%esp)
	movl	8(%esp), %ecx
	jne	.L5124
	xorl	%ebx, %ebx
	.align 16
.L5119:
	movzwl	4(%edi,%ebx), %eax
	testw	%ax, %ax
	je	.L5127
	movzwl	%ax, %eax
	addl	%eax, %ebx
	leal	(%ebx,%ecx), %eax
	cmpl	$1023, %eax
	ja	.L5122
	leal	(%ebx,%ebp), %eax
	cmpl	20(%esi), %eax
	jb	.L5119
	movl	$1024, %edx
	movl	%esi, %eax
	movl	%ecx, 8(%esp)
	call	ext2_expand.part.21
	testl	%eax, %eax
	movl	8(%esp), %ecx
	je	.L5119
.L5124:
	movl	$1, %ebx
.L5113:
	addl	$60, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5122:
	.cfi_restore_state
	cmpw	$0, 4(%edi,%ebx)
	je	.L5127
	movl	20(%esi), %eax
	addl	$1, 12(%esp)
	addl	$1024, %ebp
	movl	12(%esp), %edx
	subl	$1, %eax
	shrl	$10, %eax
	cmpl	%edx, %eax
	jnb	.L5117
	.align 16
.L5127:
	xorl	%eax, %eax
	subl	24(%esp), %ecx
	leal	(%edi,%ebx), %edx
	movl	%ebx, %esi
	je	.L5130
	.align 16
.L5134:
	movzbl	36(%esp,%eax), %ebx
	movb	%bl, (%edx,%eax)
	addl	$1, %eax
	cmpl	%eax, %ecx
	jne	.L5134
	movl	%esi, %ebx
.L5130:
	movl	28(%esp), %edx
	leal	8(%edi,%ebx), %eax
	movzbl	(%edx), %ecx
	.align 16
.L5129:
	addl	$1, %eax
	addl	$1, %edx
	movb	%cl, -1(%eax)
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	jne	.L5129
	movb	$0, (%eax)
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$3
	.cfi_def_cfa_offset 96
	pushl	$0
	.cfi_def_cfa_offset 100
	pushl	40(%esp)
	.cfi_def_cfa_offset 104
	pushl	$1536
	.cfi_def_cfa_offset 108
	pushl	$1
	.cfi_def_cfa_offset 112
	pushl	%edi
	.cfi_def_cfa_offset 116
	pushl	$1
	.cfi_def_cfa_offset 120
	pushl	$48
	.cfi_def_cfa_offset 124
	pushl	%ebx
	.cfi_def_cfa_offset 128
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 80
	xorl	%ebx, %ebx
	testl	$1073741824, %eax
	je	.L5113
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	pushl	$.LC215
	.cfi_def_cfa_offset 96
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	jmp	.L5113
.L5114:
	movl	4(%ecx), %edx
	movb	$0, 42(%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 92
	movb	%al, 55(%esp)
	movl	$9, %ebx
	movl	%edx, 48(%esp)
	movl	$9, %edx
	movw	%dx, 52(%esp)
	pushl	$1
	.cfi_def_cfa_offset 96
	call	*kmalloc
	movl	%eax, 44(%esp)
	movl	%eax, 60(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 80
	movl	$0, 24(%esp)
	jmp	.L5118
	.cfi_endproc
.LFE497:
	.size	ext2_add_file_to_dir, .-ext2_add_file_to_dir
	.align 16
	.globl	ext2_free_blocks
	.type	ext2_free_blocks, @function
ext2_free_blocks:
.LFB498:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	ext2_g_sblock, %eax
	movl	68(%esp), %ebx
	testl	%eax, %eax
	je	.L5145
	movl	ext2_g_gdesc, %edx
	testl	%edx, %edx
	je	.L5145
	movl	%eax, 24(%esp)
	movl	%edx, 28(%esp)
.L5147:
	movl	ext2_g_bb, %eax
	testl	%eax, %eax
	movl	%eax, 8(%esp)
	je	.L5177
.L5149:
	movl	28(%esp), %eax
	movl	$2, %edi
	subl	8(%eax), %edi
	movl	20(%eax), %eax
	sall	$10, %eax
	shrl	$9, %eax
	subl	%eax, %edi
	testl	%ebx, %ebx
	je	.L5150
	movl	64(%esp), %eax
	movl	$0, 12(%esp)
	movl	$0, 4(%esp)
	leal	(%eax,%ebx,4), %esi
	jmp	.L5152
	.align 16
.L5179:
	addl	8(%esp), %edx
	shrl	$10, %ecx
	movl	$128, %ebx
	andl	$7, %ecx
	subl	$1, %ecx
	sarl	%cl, %ebx
	movzbl	(%edx), %ecx
	movzbl	%cl, %ebp
	testl	%ebp, %ebx
	je	.L5151
	xorl	%ebx, %ecx
	movl	$1, 12(%esp)
	movb	%cl, (%edx)
	.align 16
.L5151:
	addl	$4, %eax
	cmpl	%eax, %esi
	je	.L5178
.L5152:
	movl	(%eax), %edx
	testl	%edx, %edx
	je	.L5151
	leal	(%edi,%edx), %ecx
	sall	$9, %ecx
	movl	%ecx, %edx
	shrl	$13, %edx
	cmpl	$1024, %edx
	jbe	.L5179
	addl	$4, %eax
	movl	$1, 4(%esp)
	cmpl	%eax, %esi
	jne	.L5152
.L5178:
	cmpl	$1, 12(%esp)
	je	.L5180
.L5144:
	movl	4(%esp), %eax
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5145:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	leal	36(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 76
	leal	36(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	je	.L5147
.L5150:
	movl	$0, 4(%esp)
	movl	4(%esp), %eax
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5180:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	curr_port, %ebx
	pushl	$3
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	movl	48(%esp), %eax
	pushl	(%eax)
	.cfi_def_cfa_offset 88
	pushl	$1536
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	40(%esp)
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L5144
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5144
.L5177:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1024
	.cfi_def_cfa_offset 80
	call	*kmalloc
	movl	$3, (%esp)
	movl	%eax, 24(%esp)
	movl	%eax, %edi
	pushl	$0
	.cfi_def_cfa_offset 84
	movl	curr_port, %esi
	movl	%eax, ext2_g_bb
	movl	48(%esp), %eax
	pushl	(%eax)
	.cfi_def_cfa_offset 88
	pushl	$1536
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%edi
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L5149
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5149
	.cfi_endproc
.LFE498:
	.size	ext2_free_blocks, .-ext2_free_blocks
	.align 16
	.globl	ext2_update_node_in_inode_table
	.type	ext2_update_node_in_inode_table, @function
ext2_update_node_in_inode_table:
.LFB499:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	ext2_g_sblock, %edx
	movl	48(%esp), %ebx
	testl	%edx, %edx
	je	.L5182
	movl	ext2_g_gdesc, %eax
	testl	%eax, %eax
	je	.L5182
	movl	12(%esp), %eax
.L5183:
	movl	20(%eax), %esi
	movl	ext2_g_inode_table, %edi
	sall	$10, %esi
	testl	%edi, %edi
	je	.L5207
.L5186:
	testl	%esi, %esi
	je	.L5193
	movl	4(%ebx), %ebp
	xorl	%edx, %edx
	cmpl	%ebp, 4(%edi)
	leal	19600(%edi), %eax
	jne	.L5189
	jmp	.L5208
	.align 16
.L5192:
	movl	%eax, %ecx
	addl	$19600, %eax
	cmpl	%ebp, -19596(%eax)
	je	.L5187
.L5189:
	addl	$140, %edx
	cmpl	%edx, %esi
	ja	.L5192
.L5193:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5208:
	.cfi_restore_state
	movl	%edi, %ecx
	.align 16
.L5187:
	xorl	%eax, %eax
	.align 16
.L5190:
	movzbl	(%ebx,%eax), %edx
	movb	%dl, (%ecx,%eax)
	addl	$1, %eax
	cmpl	$140, %eax
	jne	.L5190
	movl	%esi, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	curr_port, %ebx
	shrl	$9, %eax
	shrl	$13, %esi
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	sall	$9, %eax
	movl	32(%esp), %edx
	pushl	8(%edx)
	.cfi_def_cfa_offset 72
	pushl	%eax
	.cfi_def_cfa_offset 76
	movzwl	%si, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$1
	.cfi_def_cfa_offset 88
	pushl	$48
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5209
.L5205:
	xorl	%eax, %eax
.L5210:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5182:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 60
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	jne	.L5205
	movl	ext2_g_sblock, %eax
	movl	%eax, 8(%esp)
	movl	ext2_g_gdesc, %eax
	movl	%eax, 12(%esp)
	jmp	.L5183
.L5209:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC215
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	xorl	%eax, %eax
	jmp	.L5210
.L5207:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%eax, %edi
	movl	%eax, ext2_g_inode_table
	movl	%esi, %eax
	shrl	$9, %eax
	movl	curr_port, %ebp
	addl	$1, %eax
	movl	%eax, (%esp)
	pushl	$0
	.cfi_def_cfa_offset 68
	sall	$9, %eax
	movl	32(%esp), %edx
	pushl	8(%edx)
	.cfi_def_cfa_offset 72
	pushl	%eax
	.cfi_def_cfa_offset 76
	movl	%esi, %eax
	shrl	$13, %eax
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	%edi
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebp
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebp), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L5186
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5186
	.cfi_endproc
.LFE499:
	.size	ext2_update_node_in_inode_table, .-ext2_update_node_in_inode_table
	.align 16
	.globl	ext2_expand
	.type	ext2_expand, @function
ext2_expand:
.LFB500:
	.cfi_startproc
	movl	8(%esp), %edx
	movl	4(%esp), %eax
	testl	%edx, %edx
	je	.L5212
	jmp	ext2_expand.part.21
	.align 16
.L5212:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE500:
	.size	ext2_expand, .-ext2_expand
	.align 16
	.globl	ext2_remove_inode_entry
	.type	ext2_remove_inode_entry, @function
ext2_remove_inode_entry:
.LFB501:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	ext2_g_sblock, %eax
	testl	%eax, %eax
	je	.L5214
	movl	ext2_g_gdesc, %edx
	testl	%edx, %edx
	je	.L5214
	movl	%eax, 24(%esp)
	movl	%edx, 28(%esp)
.L5216:
	movl	ext2_g_inode_table, %ebx
	testl	%ebx, %ebx
	je	.L5253
.L5218:
	movl	ext2_g_ib, %esi
	testl	%esi, %esi
	je	.L5254
.L5219:
	movl	64(%esp), %eax
	xorl	%ebp, %ebp
	movl	%ebx, %edx
	movl	4(%eax), %ecx
	cmpl	%ecx, 4(%ebx)
	movl	24(%esp), %eax
	movl	40(%eax), %edi
	je	.L5221
	testl	%edi, %edi
	je	.L5227
	leal	140(%ebx), %eax
	xorl	%ebp, %ebp
	jmp	.L5222
	.align 16
.L5255:
	addl	$140, %eax
	cmpl	%edi, %ebp
	je	.L5227
.L5222:
	addl	$1, %ebp
	cmpl	%ecx, 4(%eax)
	movl	%eax, %edx
	jne	.L5255
.L5221:
	cmpl	%ebp, %edi
	je	.L5227
	movl	%ebp, %ecx
	movl	$128, %edi
	shrl	$3, %ebp
	andl	$7, %ecx
	subl	$1, %ecx
	sarl	%cl, %edi
	leal	(%esi,%ebp), %ecx
	movzbl	(%ecx), %eax
	movzbl	%al, %ebp
	movb	%al, 15(%esp)
	movl	$1, %eax
	testl	%ebp, %edi
	je	.L5213
	movzbl	15(%esp), %eax
	xorl	%edi, %eax
	movb	%al, (%ecx)
	leal	140(%edx), %eax
	.align 16
.L5223:
	addl	$1, %edx
	movb	$0, -1(%edx)
	cmpl	%edx, %eax
	jne	.L5223
	movl	28(%esp), %ecx
	movl	curr_port, %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	20(%ecx), %eax
	sall	$10, %eax
	movl	%eax, %edx
	shrl	$13, %eax
	shrl	$9, %edx
	movzwl	%ax, %eax
	addl	$1, %edx
	addl	$1, %eax
	pushl	%edx
	.cfi_def_cfa_offset 80
	sall	$9, %edx
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	8(%ecx)
	.cfi_def_cfa_offset 88
	pushl	%edx
	.cfi_def_cfa_offset 92
	pushl	%eax
	.cfi_def_cfa_offset 96
	pushl	%ebx
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%edi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	jne	.L5256
.L5224:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	curr_port, %ebx
	pushl	$3
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	movl	48(%esp), %eax
	pushl	4(%eax)
	.cfi_def_cfa_offset 88
	pushl	$1536
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%esi
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	jne	.L5257
.L5252:
	xorl	%eax, %eax
.L5213:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5227:
	.cfi_restore_state
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5214:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	leal	36(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 76
	leal	36(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	je	.L5216
	jmp	.L5252
.L5257:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5252
.L5256:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5224
.L5253:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	40(%esp), %eax
	movl	20(%eax), %eax
	sall	$10, %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	*kmalloc
	movl	44(%esp), %ecx
	movl	%eax, %ebx
	movl	%eax, ext2_g_inode_table
	movl	curr_port, %esi
	movl	20(%ecx), %eax
	sall	$10, %eax
	movl	%eax, %edx
	shrl	$13, %eax
	shrl	$9, %edx
	movzwl	%ax, %eax
	addl	$1, %edx
	addl	$1, %eax
	movl	%edx, (%esp)
	sall	$9, %edx
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	8(%ecx)
	.cfi_def_cfa_offset 88
	pushl	%edx
	.cfi_def_cfa_offset 92
	pushl	%eax
	.cfi_def_cfa_offset 96
	pushl	%ebx
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%esi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L5218
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5218
.L5254:
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$1024
	.cfi_def_cfa_offset 80
	call	*kmalloc
	movl	$3, (%esp)
	movl	curr_port, %edi
	movl	%eax, %esi
	pushl	$0
	.cfi_def_cfa_offset 84
	movl	%eax, ext2_g_ib
	movl	48(%esp), %eax
	pushl	4(%eax)
	.cfi_def_cfa_offset 88
	pushl	$1536
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%esi
	.cfi_def_cfa_offset 100
	pushl	$0
	.cfi_def_cfa_offset 104
	pushl	$32
	.cfi_def_cfa_offset 108
	pushl	%edi
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L5219
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC214
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5219
	.cfi_endproc
.LFE501:
	.size	ext2_remove_inode_entry, .-ext2_remove_inode_entry
	.align 16
	.globl	ext2_delete
	.type	ext2_delete, @function
ext2_delete:
.LFB502:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$24, %esp
	.cfi_def_cfa_offset 32
	movl	ext2_g_sblock, %eax
	movl	36(%esp), %ebx
	testl	%eax, %eax
	je	.L5259
	movl	ext2_g_gdesc, %edx
	testl	%edx, %edx
	je	.L5259
	movl	%eax, 8(%esp)
	movl	%edx, 12(%esp)
.L5261:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	%ebx
	.cfi_def_cfa_offset 48
	call	ext2_remove_inode_entry
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L5271
.L5263:
	addl	$24, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5271:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ebx
	.cfi_def_cfa_offset 44
	pushl	44(%esp)
	.cfi_def_cfa_offset 48
	call	ext2_free_data_blocks
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	jne	.L5263
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ebx
	.cfi_def_cfa_offset 44
	pushl	44(%esp)
	.cfi_def_cfa_offset 48
	call	ext2_remove_dirent
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	setne	%al
	addl	$24, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movzbl	%al, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5259:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 44
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L5261
	jmp	.L5263
	.cfi_endproc
.LFE502:
	.size	ext2_delete, .-ext2_delete
	.section	.rodata
.LC572:
	.string	"."
	.text
	.align 16
	.globl	ext2_create_dir
	.type	ext2_create_dir, @function
ext2_create_dir:
.LFB503:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	ext2_g_sblock, %edi
	testl	%edi, %edi
	je	.L5273
	movl	ext2_g_gdesc, %esi
	testl	%esi, %esi
	je	.L5273
	movl	%edi, 8(%esp)
	movl	%esi, 12(%esp)
.L5276:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	$1
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	ext2_format_block_bitmap
	movl	$140, (%esp)
	movl	%eax, %ebp
	call	*kmalloc
	movl	%eax, %ebx
	movb	$83, (%eax)
	popl	%eax
	.cfi_def_cfa_offset 60
	popl	%edx
	.cfi_def_cfa_offset 56
	pushl	%esi
	.cfi_def_cfa_offset 60
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	ext2_find_open_inode
	movl	%eax, 4(%ebx)
	xorl	%eax, %eax
	movl	$493, %ecx
	movw	%ax, 16(%ebx)
	xorl	%eax, %eax
	movw	%cx, 8(%ebx)
	movw	%ax, 40(%ebx)
	movl	$2, %eax
	movl	$2, 12(%ebx)
	movw	%ax, 42(%ebx)
	movl	$2, %eax
	movl	$1024, 20(%ebx)
	movw	%ax, 44(%ebx)
	xorl	%eax, %eax
	movl	$65535, 24(%ebx)
	movw	%ax, 46(%ebx)
	movl	$5, %eax
	movl	$65535, 28(%ebx)
	movl	$65535, 32(%ebx)
	movl	$0, 36(%ebx)
	movw	%ax, 48(%ebx)
	pushl	$1
	.cfi_def_cfa_offset 68
	pushl	%ebp
	.cfi_def_cfa_offset 72
	pushl	%esi
	.cfi_def_cfa_offset 76
	xorl	%ebp, %ebp
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	ext2_inode_entry_blocks
	addl	$28, %esp
	.cfi_def_cfa_offset 52
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	movw	%dx, 112(%ebx)
	movw	%cx, 114(%ebx)
	movw	%bp, 116(%ebx)
	movw	%ax, 118(%ebx)
	pushl	%edi
	.cfi_def_cfa_offset 56
	pushl	%esi
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	ext2_data_to_inode_table
	movl	curr_port, %edi
	subw	$1, 14(%esi)
	subw	$1, 12(%esi)
	movl	$1, (%esp)
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	24(%esi)
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%esi
	.cfi_def_cfa_offset 84
	pushl	$1
	.cfi_def_cfa_offset 88
	pushl	$48
	.cfi_def_cfa_offset 92
	pushl	%edi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L5277
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC215
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L5277:
	movl	8(%esp), %eax
	movl	48(%esp), %edx
	movl	84(%eax), %eax
	cmpl	%eax, 4(%edx)
	je	.L5272
	pushl	52(%esp)
	.cfi_def_cfa_offset 52
	pushl	12(%ebx)
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	60(%esp)
	.cfi_def_cfa_offset 64
	call	ext2_add_file_to_dir
	pushl	$.LC572
	.cfi_def_cfa_offset 68
	pushl	$8
	.cfi_def_cfa_offset 72
	pushl	%ebx
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	ext2_add_file_to_dir
	addl	$32, %esp
	.cfi_def_cfa_offset 48
	pushl	$.LC433
	.cfi_def_cfa_offset 52
	pushl	$8
	.cfi_def_cfa_offset 56
	pushl	56(%esp)
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	ext2_add_file_to_dir
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L5272:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5273:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 60
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	jne	.L5278
	movl	12(%esp), %esi
	movl	8(%esp), %edi
	jmp	.L5276
	.align 16
.L5278:
	xorl	%ebx, %ebx
	jmp	.L5272
	.cfi_endproc
.LFE503:
	.size	ext2_create_dir, .-ext2_create_dir
	.align 16
	.globl	ext2_find_open_inode
	.type	ext2_find_open_inode, @function
ext2_find_open_inode:
.LFB505:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$12, %esp
	.cfi_def_cfa_offset 32
	movl	ext2_g_ib, %edi
	testl	%edi, %edi
	je	.L5304
.L5287:
	movzbl	(%edi), %esi
	movl	32(%esp), %eax
	movl	$1, %ebx
	xorl	%ecx, %ecx
	movl	(%eax), %ebp
	movl	$128, %eax
	movl	%esi, %edx
	movzbl	%dl, %edx
	.align 16
.L5288:
	testl	%eax, %edx
	je	.L5290
.L5305:
	cmpl	%ebp, %ebx
	jnb	.L5290
	addl	$1, %ebx
	shrl	%eax
	jne	.L5288
	addl	$1, %ecx
	movl	$128, %eax
	movzbl	(%edi,%ecx), %esi
	movl	%esi, %edx
	movzbl	%dl, %edx
	testl	%eax, %edx
	jne	.L5305
.L5290:
	cmpl	%ebp, %ebx
	jne	.L5306
	movl	$-1, %ebx
.L5286:
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5306:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	orl	%esi, %eax
	movb	%al, (%edi,%ecx)
	pushl	$3
	.cfi_def_cfa_offset 48
	movl	curr_port, %esi
	pushl	$0
	.cfi_def_cfa_offset 52
	movl	56(%esp), %eax
	pushl	4(%eax)
	.cfi_def_cfa_offset 56
	pushl	$1536
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%edi
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%esi
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L5286
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%ebx, %eax
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5304:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$1024
	.cfi_def_cfa_offset 48
	call	*kmalloc
	movl	$3, (%esp)
	movl	curr_port, %ebx
	movl	%eax, %edi
	pushl	$0
	.cfi_def_cfa_offset 52
	movl	%eax, ext2_g_ib
	movl	56(%esp), %eax
	pushl	4(%eax)
	.cfi_def_cfa_offset 56
	pushl	$1536
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%edi
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L5287
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L5287
	.cfi_endproc
.LFE505:
	.size	ext2_find_open_inode, .-ext2_find_open_inode
	.align 16
	.globl	ext2_create_file
	.type	ext2_create_file, @function
ext2_create_file:
.LFB506:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	movl	$0, %eax
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	56(%esp), %edi
	leal	-1(%edi), %esi
	shrl	$10, %esi
	addl	$1, %esi
	testl	%edi, %edi
	cmove	%eax, %esi
	movl	ext2_g_sblock, %eax
	testl	%eax, %eax
	je	.L5309
	movl	ext2_g_gdesc, %ebp
	testl	%ebp, %ebp
	je	.L5309
	movl	12(%esp), %eax
.L5310:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%esi
	.cfi_def_cfa_offset 60
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	ext2_format_block_bitmap
	movl	$140, (%esp)
	movl	%eax, %ebp
	call	*kmalloc
	movl	%eax, %ebx
	movb	$83, (%eax)
	popl	%eax
	.cfi_def_cfa_offset 60
	popl	%edx
	.cfi_def_cfa_offset 56
	pushl	20(%esp)
	.cfi_def_cfa_offset 60
	pushl	20(%esp)
	.cfi_def_cfa_offset 64
	call	ext2_find_open_inode
	movl	%eax, 4(%ebx)
	xorl	%eax, %eax
	movl	$438, %ecx
	movw	%ax, 16(%ebx)
	xorl	%eax, %eax
	movw	%cx, 8(%ebx)
	movw	%ax, 40(%ebx)
	movl	$1, %eax
	movl	%edi, 20(%ebx)
	movw	%ax, 42(%ebx)
	movl	%esi, %eax
	movl	$1, 12(%ebx)
	sall	$10, %eax
	movl	$65535, 24(%ebx)
	movl	$65535, 28(%ebx)
	shrl	$9, %eax
	movl	$65535, 32(%ebx)
	movl	$0, 36(%ebx)
	movw	%ax, 44(%ebx)
	xorl	%eax, %eax
	xorl	%edi, %edi
	movw	%ax, 46(%ebx)
	movl	$5, %eax
	movw	%ax, 48(%ebx)
	pushl	%esi
	.cfi_def_cfa_offset 68
	pushl	%ebp
	.cfi_def_cfa_offset 72
	pushl	36(%esp)
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	ext2_inode_entry_blocks
	addl	$28, %esp
	.cfi_def_cfa_offset 52
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	movw	%dx, 114(%ebx)
	movw	%cx, 116(%ebx)
	movw	%ax, 112(%ebx)
	movw	%di, 118(%ebx)
	pushl	12(%esp)
	.cfi_def_cfa_offset 56
	pushl	20(%esp)
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	ext2_data_to_inode_table
	movl	28(%esp), %eax
	subw	%si, 12(%eax)
	movl	curr_port, %esi
	subw	$1, 14(%eax)
	movl	$1, (%esp)
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	24(%eax)
	.cfi_def_cfa_offset 72
	pushl	$512
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%eax
	.cfi_def_cfa_offset 84
	pushl	$1
	.cfi_def_cfa_offset 88
	pushl	$48
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5320
.L5313:
	pushl	52(%esp)
	.cfi_def_cfa_offset 52
	pushl	12(%ebx)
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	60(%esp)
	.cfi_def_cfa_offset 64
	call	ext2_add_file_to_dir
	addl	$44, %esp
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5320:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -20
	.cfi_offset 5, -8
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC215
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5313
	.align 16
.L5309:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	xorl	%ebx, %ebx
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 60
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	jne	.L5313
	movl	ext2_g_sblock, %eax
	movl	%eax, 8(%esp)
	movl	ext2_g_gdesc, %eax
	movl	%eax, 12(%esp)
	jmp	.L5310
	.cfi_endproc
.LFE506:
	.size	ext2_create_file, .-ext2_create_file
	.align 16
	.globl	ext2_write
	.type	ext2_write, @function
ext2_write:
.LFB508:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	12(%esp), %eax
	movl	16(%esp), %edx
	movl	20(%esp), %ecx
	movl	24(%esp), %esi
	movzwl	8(%eax), %ebx
	testl	%ebx, _Wlogged
	je	.L5322
	movl	%esi, 12(%esp)
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	jmp	ext2_write.part.20
	.align 16
.L5322:
	.cfi_restore_state
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE508:
	.size	ext2_write, .-ext2_write
	.align 16
	.globl	ext2_get_singly
	.type	ext2_get_singly, @function
ext2_get_singly:
.LFB509:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$24, %esp
	.cfi_def_cfa_offset 44
	pushl	$1024
	.cfi_def_cfa_offset 48
	call	*kmalloc
	movl	curr_port, %ebx
	movl	$3, (%esp)
	movl	%eax, %esi
	pushl	$0
	.cfi_def_cfa_offset 52
	pushl	52(%esp)
	.cfi_def_cfa_offset 56
	pushl	$1536
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%eax
	.cfi_def_cfa_offset 68
	pushl	$0
	.cfi_def_cfa_offset 72
	pushl	$32
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	jne	.L5342
.L5325:
	movl	(%esi), %ebx
	testl	%ebx, %ebx
	je	.L5326
	xorl	%ebx, %ebx
	jmp	.L5328
	.align 16
.L5344:
	cmpl	$256, %ebx
	je	.L5343
.L5328:
	addl	$1, %ebx
	movl	(%esi,%ebx,4), %edx
	leal	0(,%ebx,4), %eax
	testl	%edx, %edx
	jne	.L5344
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, %ebp
.L5331:
	leal	0(%ebp,%ebx), %edi
	movl	%esi, %ecx
	movl	%ebp, %edx
	.align 16
.L5330:
	addl	$1, %ecx
	movzbl	-1(%ecx), %eax
	addl	$1, %edx
	cmpl	%edi, %edx
	movb	%al, -1(%edx)
	jne	.L5330
.L5332:
	movl	36(%esp), %edx
	movl	%ebp, %eax
	movl	%ebx, (%edx)
	addl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5342:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC214
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	jmp	.L5325
.L5343:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$1024
	.cfi_def_cfa_offset 48
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, %ebp
	jmp	.L5331
.L5326:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$0
	.cfi_def_cfa_offset 48
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, %ebp
	jmp	.L5332
	.cfi_endproc
.LFE509:
	.size	ext2_get_singly, .-ext2_get_singly
	.align 16
	.globl	ext2_get_doubly
	.type	ext2_get_doubly, @function
ext2_get_doubly:
.LFB510:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	curr_port, %ebx
	movl	$3, (%esp)
	movl	%eax, %ebp
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	68(%esp)
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%eax
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5376
.L5346:
	movl	0(%ebp), %eax
	testl	%eax, %eax
	movl	%eax, (%esp)
	je	.L5347
	xorl	%ebx, %ebx
	jmp	.L5349
	.align 16
.L5378:
	cmpl	$256, %ebx
	je	.L5377
.L5349:
	addl	$1, %ebx
	movl	0(%ebp,%ebx,4), %eax
	testl	%eax, %eax
	jne	.L5378
	movl	%ebx, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	sall	$10, %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
.L5374:
	call	*kmalloc
	movl	$4, (%esp)
	movl	%eax, 28(%esp)
	xorl	%esi, %esi
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, %edi
	xorl	%ecx, %ecx
	movl	12(%esp), %eax
	movl	%ebp, 4(%esp)
	movl	%ecx, %ebp
	movl	%ebx, 8(%esp)
	movl	%eax, %ebx
	.align 16
.L5353:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%edi
	.cfi_def_cfa_offset 60
	movl	16(%esp), %eax
	pushl	(%eax,%esi,4)
	.cfi_def_cfa_offset 64
	call	ext2_get_singly
	movl	(%edi), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%ecx, %ecx
	je	.L5351
	addl	%ebx, %ecx
	movl	%ebx, %edx
	movl	%ebx, (%esp)
	.align 16
.L5352:
	addl	$1, %eax
	movzbl	-1(%eax), %ebx
	addl	$1, %edx
	cmpl	%ecx, %edx
	movb	%bl, -1(%edx)
	jne	.L5352
	movl	(%esp), %ebx
	movl	(%edi), %ecx
.L5351:
	addl	%ecx, %ebp
	leal	1(%esi), %eax
	addl	$1024, %ebx
	cmpl	8(%esp), %eax
	jnb	.L5379
	movl	%eax, %esi
	jmp	.L5353
.L5379:
	movl	%ebp, (%esp)
	sall	$8, %esi
.L5358:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	addl	%esi, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 64
	call	*kmalloc
	addl	(%edi), %esi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	12(%esp), %ecx
	movl	%eax, %edx
	movl	%esi, %ebx
	leal	(%eax,%esi), %esi
	testl	%ebx, %ebx
	je	.L5356
	.align 16
.L5364:
	addl	$1, %ecx
	movzbl	-1(%ecx), %ebx
	addl	$1, %edx
	cmpl	%esi, %edx
	movb	%bl, -1(%edx)
	jne	.L5364
.L5356:
	movl	(%esp), %edi
	movl	52(%esp), %edx
	movl	%edi, (%edx)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5376:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5346
.L5377:
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 60
	pushl	$262144
	.cfi_def_cfa_offset 64
	jmp	.L5374
.L5347:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	$-256, %esi
	pushl	$0
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	$4, (%esp)
	movl	%eax, 28(%esp)
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, %edi
	movl	(%eax), %ecx
	jmp	.L5358
	.cfi_endproc
.LFE510:
	.size	ext2_get_doubly, .-ext2_get_doubly
	.align 16
	.globl	ext2_get_triply
	.type	ext2_get_triply, @function
ext2_get_triply:
.LFB511:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	curr_port, %ebx
	movl	$3, (%esp)
	movl	%eax, %ebp
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	68(%esp)
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%eax
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5411
.L5381:
	movl	0(%ebp), %eax
	testl	%eax, %eax
	movl	%eax, (%esp)
	je	.L5382
	xorl	%ebx, %ebx
	jmp	.L5384
	.align 16
.L5413:
	cmpl	$256, %ebx
	je	.L5412
.L5384:
	addl	$1, %ebx
	movl	0(%ebp,%ebx,4), %eax
	testl	%eax, %eax
	jne	.L5413
	movl	%ebx, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	sall	$18, %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
.L5409:
	call	*kmalloc
	movl	$4, (%esp)
	movl	%eax, 28(%esp)
	xorl	%esi, %esi
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, %edi
	xorl	%ecx, %ecx
	movl	12(%esp), %eax
	movl	%ebp, 4(%esp)
	movl	%ecx, %ebp
	movl	%ebx, 8(%esp)
	movl	%eax, %ebx
	.align 16
.L5388:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%edi
	.cfi_def_cfa_offset 60
	movl	16(%esp), %eax
	pushl	(%eax,%esi,4)
	.cfi_def_cfa_offset 64
	call	ext2_get_doubly
	movl	(%edi), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%ecx, %ecx
	je	.L5386
	addl	%ebx, %ecx
	movl	%ebx, %edx
	movl	%ebx, (%esp)
	.align 16
.L5387:
	addl	$1, %eax
	movzbl	-1(%eax), %ebx
	addl	$1, %edx
	cmpl	%ecx, %edx
	movb	%bl, -1(%edx)
	jne	.L5387
	movl	(%esp), %ebx
	movl	(%edi), %ecx
.L5386:
	addl	%ecx, %ebp
	leal	1(%esi), %eax
	addl	$262144, %ebx
	cmpl	8(%esp), %eax
	jnb	.L5414
	movl	%eax, %esi
	jmp	.L5388
.L5414:
	movl	%ebp, (%esp)
	sall	$16, %esi
.L5393:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	addl	%esi, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 64
	call	*kmalloc
	addl	(%edi), %esi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	12(%esp), %ecx
	movl	%eax, %edx
	movl	%esi, %ebx
	leal	(%eax,%esi), %esi
	testl	%ebx, %ebx
	je	.L5391
	.align 16
.L5399:
	addl	$1, %ecx
	movzbl	-1(%ecx), %ebx
	addl	$1, %edx
	cmpl	%esi, %edx
	movb	%bl, -1(%edx)
	jne	.L5399
.L5391:
	movl	(%esp), %edi
	movl	52(%esp), %edx
	movl	%edi, (%edx)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5411:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5381
.L5412:
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 60
	pushl	$67108864
	.cfi_def_cfa_offset 64
	jmp	.L5409
.L5382:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	$-65536, %esi
	pushl	$0
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	$4, (%esp)
	movl	%eax, 28(%esp)
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, %edi
	movl	(%eax), %ecx
	jmp	.L5393
	.cfi_endproc
.LFE511:
	.size	ext2_get_triply, .-ext2_get_triply
	.align 16
	.globl	ext2_block_locs
	.type	ext2_block_locs, @function
ext2_block_locs:
.LFB512:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	48(%esp), %esi
	testl	%esi, %esi
	je	.L5436
	movl	20(%esi), %eax
	xorl	%ebx, %ebx
	testl	%eax, %eax
	je	.L5415
	subl	$1, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	shrl	$10, %eax
	leal	4(,%eax,4), %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	100(%esi), %ebp
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, %ebx
	testl	%ebp, %ebp
	jne	.L5470
.L5417:
	movl	52(%esi), %edx
	leal	52(%esi), %ecx
	testl	%edx, %edx
	je	.L5415
	xorl	%eax, %eax
	.align 16
.L5421:
	movl	%edx, (%ebx,%eax,4)
	addl	$1, %eax
	movl	(%ecx,%eax,4), %edx
	testl	%edx, %edx
	jne	.L5421
.L5415:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5470:
	.cfi_restore_state
	movl	104(%esi), %eax
	testl	%eax, %eax
	jne	.L5418
	movl	108(%esi), %edi
	testl	%edi, %edi
	jne	.L5417
	leal	52(%esi), %ecx
	.align 16
.L5422:
	movl	(%ecx,%eax,4), %edx
	movl	%edx, (%ebx,%eax,4)
	addl	$1, %eax
	cmpl	$12, %eax
	jne	.L5422
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$4
	.cfi_def_cfa_offset 64
	call	*kmalloc
	popl	%edx
	.cfi_def_cfa_offset 60
	popl	%ecx
	.cfi_def_cfa_offset 56
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	100(%esi)
	.cfi_def_cfa_offset 64
	movl	%eax, %edi
	call	ext2_get_singly
	movl	(%edi), %esi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%esi, %esi
	je	.L5415
	xorl	%edx, %edx
	.align 16
.L5423:
	movzbl	(%eax,%edx), %ecx
	movb	%cl, 48(%ebx,%edx)
	addl	$1, %edx
	cmpl	%edx, %esi
	jne	.L5423
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5418:
	.cfi_restore_state
	movl	108(%esi), %eax
	testl	%eax, %eax
	je	.L5420
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$4
	.cfi_def_cfa_offset 64
	call	*kmalloc
	leal	52(%esi), %ecx
	movl	%eax, %edi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	xorl	%eax, %eax
	.align 16
.L5428:
	movl	(%ecx,%eax,4), %edx
	movl	%edx, (%ebx,%eax,4)
	addl	$1, %eax
	cmpl	$12, %eax
	jne	.L5428
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	curr_port, %edx
	movl	$3, (%esp)
	movl	%eax, %ebp
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	100(%esi)
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%eax
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%edx
	.cfi_def_cfa_offset 96
	movl	%edx, 60(%esp)
	call	SATA_Commander
	movl	60(%esp), %edx
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	movl	16(%edx), %eax
	testl	$1073741824, %eax
	jne	.L5471
.L5429:
	xorl	%eax, %eax
	.align 16
.L5430:
	movl	0(%ebp,%eax,4), %edx
	movl	%edx, 48(%ebx,%eax,4)
	addl	$1, %eax
	cmpl	$256, %eax
	jne	.L5430
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%edi
	.cfi_def_cfa_offset 60
	pushl	104(%esi)
	.cfi_def_cfa_offset 64
	call	ext2_get_doubly
	movl	(%edi), %ebp
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	xorl	%edx, %edx
	testl	%ebp, %ebp
	je	.L5434
	.align 16
.L5451:
	movzbl	(%eax,%edx), %ecx
	movb	%cl, 1072(%ebx,%edx)
	addl	$1, %edx
	cmpl	%edx, %ebp
	jne	.L5451
.L5434:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%edi
	.cfi_def_cfa_offset 60
	pushl	108(%esi)
	.cfi_def_cfa_offset 64
	call	ext2_get_triply
	movl	(%edi), %esi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	xorl	%edx, %edx
	testl	%esi, %esi
	je	.L5415
	.align 16
.L5450:
	movzbl	(%eax,%edx), %ecx
	movb	%cl, 263216(%ebx,%edx)
	addl	$1, %edx
	cmpl	%esi, %edx
	jne	.L5450
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5420:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$4
	.cfi_def_cfa_offset 64
	call	*kmalloc
	leal	52(%esi), %ecx
	movl	%eax, %edi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	xorl	%eax, %eax
	.align 16
.L5424:
	movl	(%ecx,%eax,4), %edx
	movl	%edx, (%ebx,%eax,4)
	addl	$1, %eax
	cmpl	$12, %eax
	jne	.L5424
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	curr_port, %edx
	movl	$3, (%esp)
	movl	%eax, %ebp
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	100(%esi)
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%eax
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%edx
	.cfi_def_cfa_offset 96
	movl	%edx, 60(%esp)
	call	SATA_Commander
	movl	60(%esp), %edx
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	movl	16(%edx), %eax
	testl	$1073741824, %eax
	jne	.L5472
.L5425:
	xorl	%eax, %eax
	.align 16
.L5426:
	movl	0(%ebp,%eax,4), %edx
	movl	%edx, 48(%ebx,%eax,4)
	addl	$1, %eax
	cmpl	$256, %eax
	jne	.L5426
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	%edi
	.cfi_def_cfa_offset 60
	pushl	104(%esi)
	.cfi_def_cfa_offset 64
	call	ext2_get_doubly
	movl	(%edi), %esi
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%esi, %esi
	je	.L5415
	xorl	%edx, %edx
	.align 16
.L5427:
	movzbl	(%eax,%edx), %ecx
	movb	%cl, 1072(%ebx,%edx)
	addl	$1, %edx
	cmpl	%esi, %edx
	jne	.L5427
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5472:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5425
.L5471:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5429
.L5436:
	xorl	%ebx, %ebx
	jmp	.L5415
	.cfi_endproc
.LFE512:
	.size	ext2_block_locs, .-ext2_block_locs
	.align 16
	.globl	ext2_chunk_data
	.type	ext2_chunk_data, @function
ext2_chunk_data:
.LFB513:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	52(%esp), %ebp
	movl	60(%esp), %eax
	movl	56(%esp), %ecx
	testl	%ebp, %ebp
	movl	$0, (%eax)
	je	.L5496
	cmpl	%ebp, %ecx
	jnb	.L5496
	cmpl	$1, %ebp
	je	.L5526
	xorl	%eax, %eax
	testl	%ecx, %ecx
	movl	48(%esp), %edx
	sete	%al
	movl	$1024, 8(%esp)
	movl	%eax, %edi
	movl	48(%esp), %eax
	movl	4(%edx), %esi
	movl	%edi, (%esp)
	xorl	%edx, %edx
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	%eax, 12(%esp)
	movl	$1, %eax
	jmp	.L5487
	.align 16
.L5480:
	leal	1(%eax), %ebx
	cmpl	%ebx, %ebp
	je	.L5490
	movl	48(%esp), %edi
	movl	%esi, 12(%esp)
	movl	(%edi,%eax,4), %edi
	movl	%edi, 4(%esp)
	movl	48(%esp), %edi
	movl	4(%edi,%eax,4), %esi
.L5492:
	cmpl	%ecx, %edx
	jne	.L5485
	movl	(%esp), %edi
	testl	%edi, %edi
	jne	.L5485
	movl	$1, (%esp)
	movl	$1024, 8(%esp)
.L5485:
	cmpl	%eax, %ebp
	jbe	.L5496
	movl	%ebx, %eax
.L5487:
	movl	$-2, %ebx
	subl	4(%esp), %ebx
	addl	%esi, %ebx
	cmpl	$1, %ebx
	jbe	.L5477
	testl	%ecx, %ecx
	je	.L5527
	movl	(%esp), %ebx
	addl	$1, %edx
	testl	%ebx, %ebx
	je	.L5480
	addl	$1, %eax
	cmpl	%eax, %ebp
	jne	.L5528
.L5490:
	cmpl	%edx, %ecx
	jne	.L5496
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	curr_port, %edi
	movl	$3, (%esp)
	movl	%eax, %ebx
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%esi
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%eax
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%edi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%edi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L5482
.L5519:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5482
	.align 16
.L5477:
	leal	1(%eax), %ebx
	addl	$1024, 8(%esp)
	cmpl	%ebx, %ebp
	je	.L5529
	movl	48(%esp), %edi
	movl	(%edi,%eax,4), %edi
	movl	%edi, 4(%esp)
	movl	48(%esp), %edi
	movl	4(%edi,%eax,4), %esi
	jmp	.L5492
	.align 16
.L5496:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%ebx, %ebx
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5527:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	20(%esp), %edi
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%eax, %ebx
	movl	%edi, %eax
	movl	curr_port, %esi
	shrl	$9, %eax
	addl	$1, %eax
	movl	%eax, (%esp)
	pushl	$0
	.cfi_def_cfa_offset 68
	movl	68(%esp), %ecx
	pushl	(%ecx)
	.cfi_def_cfa_offset 72
.L5522:
	sall	$9, %eax
	pushl	%eax
	.cfi_def_cfa_offset 76
	movl	%edi, %eax
	shrl	$13, %eax
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	%ebx
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5530
.L5484:
	movl	60(%esp), %eax
	movl	8(%esp), %ecx
	movl	%ecx, (%eax)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5528:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	20(%esp), %edi
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%eax, %ebx
	movl	%edi, %eax
	movl	curr_port, %esi
	shrl	$9, %eax
	addl	$1, %eax
	movl	%eax, (%esp)
	sall	$9, %eax
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	32(%esp)
	.cfi_def_cfa_offset 72
	pushl	%eax
	.cfi_def_cfa_offset 76
	movl	%edi, %eax
	shrl	$13, %eax
	movzwl	%ax, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	pushl	%ebx
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5531
.L5489:
	movl	60(%esp), %eax
	movl	8(%esp), %edx
	movl	%edx, (%eax)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5530:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5484
.L5531:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L5489
.L5526:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	$3, (%esp)
	movl	curr_port, %esi
	movl	%eax, %ebx
	pushl	$0
	.cfi_def_cfa_offset 68
	movl	68(%esp), %eax
	pushl	(%eax)
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ebx
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L5519
.L5482:
	movl	60(%esp), %eax
	movl	$1024, (%eax)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5529:
	.cfi_restore_state
	cmpl	%edx, %ecx
	jne	.L5496
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	20(%esp), %edi
	pushl	%edi
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%eax, %ebx
	movl	%edi, %eax
	movl	curr_port, %esi
	shrl	$9, %eax
	addl	$1, %eax
	movl	%eax, (%esp)
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	32(%esp)
	.cfi_def_cfa_offset 72
	jmp	.L5522
	.cfi_endproc
.LFE513:
	.size	ext2_chunk_data, .-ext2_chunk_data
	.align 16
	.globl	ext2_sblock_set_data
	.type	ext2_sblock_set_data, @function
ext2_sblock_set_data:
.LFB514:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	12(%esp), %ecx
	movl	32(%esp), %esi
	leal	204(%ecx), %edx
	movl	%ecx, %eax
	.align 16
.L5533:
	addl	$4, %eax
	movl	$0, -4(%eax)
	cmpl	%edx, %eax
	jne	.L5533
	movl	16(%esp), %eax
	movl	$20, %edx
	movl	$-4269, %ebx
	movw	%dx, 54(%ecx)
	xorl	%edx, %edx
	movl	$0, (%ecx)
	movl	$0, 4(%ecx)
	movl	$1024, 24(%ecx)
	movl	%eax, 8(%ecx)
	movl	20(%esp), %eax
	movl	$0, 32(%ecx)
	movl	$0, 44(%ecx)
	movl	$0, 48(%ecx)
	movw	%bx, 56(%ecx)
	movl	%eax, 20(%ecx)
	xorl	%eax, %eax
	movl	$0, 64(%ecx)
	movw	%ax, 52(%ecx)
	xorl	%eax, %eax
	movl	$3600, 68(%ecx)
	movw	%ax, 58(%ecx)
	movl	24(%esp), %eax
	movl	$5, 72(%ecx)
	movl	$0, 76(%ecx)
	movl	$0, 84(%ecx)
	movl	$0, 92(%ecx)
	movw	%ax, 60(%ecx)
	xorl	%eax, %eax
	movl	$0, 96(%ecx)
	movw	%ax, 62(%ecx)
	movl	$140, %eax
	movl	$0, 100(%ecx)
	movw	%ax, 88(%ecx)
	movl	28(%esp), %eax
	movw	%dx, 110(%ecx)
	movw	%ax, 90(%ecx)
	xorl	%eax, %eax
	movw	%ax, 104(%ecx)
	xorl	%eax, %eax
	movw	%ax, 106(%ecx)
	xorl	%eax, %eax
	movw	%ax, 108(%ecx)
	cmpb	$0, (%esi)
	je	.L5534
	xorl	%eax, %eax
	.align 16
.L5535:
	addl	$1, %eax
	cmpb	$0, (%esi,%eax)
	jne	.L5535
	cmpl	$15, %eax
	jbe	.L5555
	leal	120(%ecx), %eax
	movb	$0, 135(%ecx)
.L5537:
	movzbl	(%esi), %ebx
	movl	%esi, %edx
	.align 16
.L5539:
	addl	$1, %eax
	addl	$1, %edx
	movb	%bl, -1(%eax)
	movzbl	(%edx), %ebx
	testb	%bl, %bl
	jne	.L5539
	movb	$0, (%eax)
	leal	200(%ecx), %edx
	leal	136(%ecx), %eax
	.align 16
.L5540:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%edx, %eax
	jne	.L5540
	movl	$0, 200(%ecx)
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
.L5555:
	.cfi_restore_state
	leal	120(%ecx), %eax
	xorl	%edx, %edx
	.align 16
.L5538:
	addl	$1, %edx
	cmpb	$0, (%esi,%edx)
	jne	.L5538
.L5541:
	movb	$0, (%eax,%edx)
	jmp	.L5537
.L5534:
	leal	120(%ecx), %eax
	xorl	%edx, %edx
	jmp	.L5541
	.cfi_endproc
.LFE514:
	.size	ext2_sblock_set_data, .-ext2_sblock_set_data
	.align 16
	.globl	ext2_set_gdesc_table
	.type	ext2_set_gdesc_table, @function
ext2_set_gdesc_table:
.LFB515:
	.cfi_startproc
	movl	4(%esp), %eax
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	movw	%dx, 12(%eax)
	movw	%cx, 14(%eax)
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	movl	$0, (%eax)
	movl	$0, 4(%eax)
	movl	$0, 8(%eax)
	movw	%dx, 16(%eax)
	movw	%cx, 18(%eax)
	movl	$0, 28(%eax)
	ret
	.cfi_endproc
.LFE515:
	.size	ext2_set_gdesc_table, .-ext2_set_gdesc_table
	.section	.rodata
.LC573:
	.string	"Primary"
	.text
	.align 16
	.globl	ext2_set_block_group
	.type	ext2_set_block_group, @function
ext2_set_block_group:
.LFB516:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	movl	$8388608, %ecx
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	$25760, %edx
	subl	$152, %esp
	.cfi_def_cfa_offset 172
	movl	172(%esp), %esi
	cmpl	$8388608, %esi
	cmovbe	%esi, %ecx
	movl	%ecx, %eax
	shrl	$10, %eax
	movl	%eax, 64(%esp)
	xorl	%eax, %eax
	cmpl	$8388608, %ecx
	sete	%al
	leal	25(,%eax,4), %eax
	movl	%eax, 28(%esp)
	movl	$29960, %eax
	cmovne	%edx, %eax
	movl	$184, %edx
	movl	%eax, 60(%esp)
	movl	$214, %eax
	cmovne	%edx, %eax
	xorl	%edx, %edx
	movl	%eax, 48(%esp)
	movl	%esi, %eax
	divl	%ecx
	movl	%eax, 40(%esp)
	pushl	$204
	.cfi_def_cfa_offset 176
	call	*kmalloc
	movl	ext2_g_sblock, %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	movl	%eax, %edi
	testl	%edx, %edx
	je	.L5610
.L5559:
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	$32
	.cfi_def_cfa_offset 176
	call	*kmalloc
	movl	%eax, %ebx
	movl	44(%esp), %eax
	sall	$10, %eax
	movl	%eax, 40(%esp)
	movl	%eax, (%esp)
	call	*kmalloc
	movl	%eax, 60(%esp)
	movl	ext2_g_gdesc, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	testl	%eax, %eax
	je	.L5611
.L5560:
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	$2048
	.cfi_def_cfa_offset 176
	call	*kmalloc
	leal	2048(%eax), %edx
	movl	%eax, %ebp
	addl	$16, %esp
	.cfi_def_cfa_offset 160
.L5561:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %edx
	jne	.L5561
	movl	ext2_g_bb, %eax
	testl	%eax, %eax
	je	.L5612
.L5562:
	movl	ext2_g_ib, %eax
	testl	%eax, %eax
	je	.L5613
.L5563:
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	60(%esp)
	.cfi_def_cfa_offset 176
	call	*kmalloc
	movl	64(%esp), %edx
	movl	%eax, 80(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	addl	%eax, %edx
.L5564:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %edx
	jne	.L5564
	movl	ext2_g_inode_table, %eax
	testl	%eax, %eax
	je	.L5614
.L5565:
	movl	28(%esp), %ecx
	testl	%ecx, %ecx
	movl	%ecx, %eax
	je	.L5606
	shrl	$10, %esi
	sall	$15, %eax
	movl	36(%esp), %edx
	movl	%esi, 100(%esp)
	movl	24(%esp), %esi
	movl	%eax, 96(%esp)
	movl	$0, 60(%esp)
	imull	%ecx, %edx
	movl	$0, 56(%esp)
	movl	$2, 32(%esp)
	movl	%esi, %eax
	movl	$0, 40(%esp)
	addl	$3072, %eax
	movl	%eax, %ecx
	movl	%esi, %eax
	movl	%edx, 72(%esp)
	shrl	$9, %ecx
	addl	$4096, %eax
	leal	2048(%esi), %edx
	movl	%ecx, 4(%esp)
	movzwl	52(%esp), %ecx
	movl	%eax, %esi
	shrl	$9, %edx
	shrl	$9, %esi
	leal	-3(%ecx), %eax
	subw	28(%esp), %ax
	movzwl	36(%esp), %ecx
	subw	16(%esp), %ax
	movw	%cx, 104(%esp)
	movl	24(%esp), %ecx
	movw	%ax, 106(%esp)
	movl	48(%esp), %eax
	shrl	$9, %eax
	addl	$1, %eax
	movl	%eax, 92(%esp)
	sall	$9, %eax
	movl	%eax, 88(%esp)
	movl	%ecx, %eax
	shrl	$9, %eax
	addl	$1, %eax
	movl	%eax, 84(%esp)
	sall	$9, %eax
	movl	%eax, 80(%esp)
	movl	%ecx, %eax
	shrl	$13, %eax
	movzwl	%ax, %eax
	leal	1(%eax), %ecx
	leal	-2(%edx), %eax
	movl	%ecx, 76(%esp)
	movl	%eax, 108(%esp)
	imull	$19600, 36(%esp), %ecx
	movl	4(%esp), %eax
	subl	$2, %eax
	movl	%eax, 112(%esp)
	leal	-2(%esi), %eax
	movl	%ecx, 68(%esp)
	movl	44(%esp), %ecx
	addl	24(%esp), %ecx
	movl	%eax, 116(%esp)
	movl	%ecx, 12(%esp)
.L5581:
	subl	$8, %esp
	.cfi_def_cfa_offset 168
	pushl	$.LC573
	.cfi_def_cfa_offset 172
	pushl	$0
	.cfi_def_cfa_offset 176
	pushl	$1
	.cfi_def_cfa_offset 180
	pushl	52(%esp)
	.cfi_def_cfa_offset 184
	pushl	$0
	.cfi_def_cfa_offset 188
	pushl	%edi
	.cfi_def_cfa_offset 192
	call	ext2_sblock_set_data
	movl	104(%esp), %eax
	movl	%eax, (%edi)
	movl	132(%esp), %eax
	movl	%eax, 4(%edi)
	movl	84(%esp), %eax
	movl	%eax, 32(%edi)
	movl	68(%esp), %eax
	movl	%eax, 40(%edi)
	movl	72(%esp), %eax
	addl	$32, %esp
	.cfi_def_cfa_offset 160
	imull	$41616, %eax, %edx
	addl	ext2_g_sblock, %edx
	movw	%ax, 90(%edi)
	xorl	%eax, %eax
	.align 16
.L5567:
	movzbl	(%edi,%eax), %ecx
	movb	%cl, (%edx,%eax)
	addl	$1, %eax
	cmpl	$204, %eax
	jne	.L5567
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	$1
	.cfi_def_cfa_offset 176
	pushl	$0
	.cfi_def_cfa_offset 180
	pushl	52(%esp)
	.cfi_def_cfa_offset 184
	pushl	$512
	.cfi_def_cfa_offset 188
	pushl	$1
	.cfi_def_cfa_offset 192
	pushl	%edi
	.cfi_def_cfa_offset 196
	pushl	$1
	.cfi_def_cfa_offset 200
	pushl	$48
	.cfi_def_cfa_offset 204
	pushl	%esi
	.cfi_def_cfa_offset 208
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 160
	testl	$1073741824, %eax
	jne	.L5615
.L5568:
	movl	32(%esp), %esi
	movl	108(%esp), %eax
	movl	40(%esp), %ecx
	movl	ext2_g_bb, %edx
	addl	%esi, %eax
	movl	%eax, 8(%esp)
	movl	112(%esp), %eax
	sall	$10, %ecx
	addl	%ecx, %edx
	addl	%esi, %eax
	movl	%ecx, %esi
	movl	%eax, 20(%esp)
	xorl	%eax, %eax
	.align 16
.L5569:
	movzbl	0(%ebp,%eax), %ecx
	movb	%cl, (%edx,%eax)
	addl	$1, %eax
	cmpl	$1024, %eax
	jne	.L5569
	movl	ext2_g_ib, %ecx
	xorl	%eax, %eax
	addl	%esi, %ecx
	movl	%ecx, %edx
	.align 16
.L5570:
	movzbl	0(%ebp,%eax), %ecx
	movb	%cl, (%edx,%eax)
	addl	$1, %eax
	cmpl	$1024, %eax
	jne	.L5570
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	$5
	.cfi_def_cfa_offset 176
	pushl	$0
	.cfi_def_cfa_offset 180
	pushl	28(%esp)
	.cfi_def_cfa_offset 184
	pushl	$2560
	.cfi_def_cfa_offset 188
	pushl	$1
	.cfi_def_cfa_offset 192
	pushl	%ebp
	.cfi_def_cfa_offset 196
	pushl	$1
	.cfi_def_cfa_offset 200
	pushl	$48
	.cfi_def_cfa_offset 204
	pushl	%esi
	.cfi_def_cfa_offset 208
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 160
	testl	$1073741824, %eax
	jne	.L5616
.L5571:
	movl	60(%esp), %eax
	addl	ext2_g_inode_table, %eax
	movl	116(%esp), %esi
	movl	48(%esp), %ecx
	addl	32(%esp), %esi
	movl	64(%esp), %edx
	movl	%ebx, 4(%esp)
	addl	%eax, %ecx
	.align 16
.L5572:
	addl	$1, %edx
	movzbl	-1(%edx), %ebx
	addl	$1, %eax
	cmpl	%eax, %ecx
	movb	%bl, -1(%eax)
	jne	.L5572
	movl	curr_port, %edx
	movl	4(%esp), %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	104(%esp)
	.cfi_def_cfa_offset 176
	pushl	$0
	.cfi_def_cfa_offset 180
	pushl	%esi
	.cfi_def_cfa_offset 184
	pushl	112(%esp)
	.cfi_def_cfa_offset 188
	pushl	$4
	.cfi_def_cfa_offset 192
	pushl	96(%esp)
	.cfi_def_cfa_offset 196
	pushl	$1
	.cfi_def_cfa_offset 200
	pushl	$48
	.cfi_def_cfa_offset 204
	pushl	%edx
	.cfi_def_cfa_offset 208
	movl	%edx, 52(%esp)
	call	SATA_Commander
	movl	52(%esp), %edx
	addl	$48, %esp
	.cfi_def_cfa_offset 160
	movl	16(%edx), %eax
	testl	$1073741824, %eax
	jne	.L5617
.L5573:
	movl	32(%esp), %eax
	movl	%edi, 120(%esp)
	movl	%ebp, 124(%esp)
	movzwl	104(%esp), %edi
	movzwl	106(%esp), %ebp
	movl	44(%esp), %edx
	leal	2(%eax), %ecx
	.align 16
.L5576:
	xorl	%eax, %eax
	movl	$0, 28(%ebx)
	movl	%esi, 8(%ebx)
	movw	%ax, 16(%ebx)
	xorl	%eax, %eax
	movw	%bp, 12(%ebx)
	movw	%ax, 18(%ebx)
	movl	8(%esp), %eax
	movw	%di, 14(%ebx)
	movl	%ecx, 24(%ebx)
	movl	%ecx, 4(%esp)
	movl	%eax, (%ebx)
	movl	20(%esp), %eax
	movl	%eax, 4(%ebx)
	movl	16(%esp), %eax
	movl	%eax, 20(%ebx)
	xorl	%eax, %eax
	.align 16
.L5574:
	movzbl	(%ebx,%eax), %ecx
	movb	%cl, (%edx,%eax)
	addl	$1, %eax
	cmpl	$32, %eax
	jne	.L5574
	movl	4(%esp), %ecx
	leal	32(%edx), %eax
	addl	$1024, %edx
	.align 16
.L5575:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %edx
	jne	.L5575
	cmpl	12(%esp), %edx
	jne	.L5576
	movl	24(%esp), %esi
	movl	56(%esp), %edx
	addl	ext2_g_gdesc, %edx
	movl	120(%esp), %edi
	movl	124(%esp), %ebp
	movl	44(%esp), %eax
	testl	%esi, %esi
	je	.L5580
	movl	12(%esp), %esi
	movl	%ecx, 4(%esp)
	.align 16
.L5591:
	addl	$1, %eax
	movzbl	-1(%eax), %ecx
	addl	$1, %edx
	cmpl	%esi, %eax
	movb	%cl, -1(%edx)
	jne	.L5591
	movl	4(%esp), %ecx
.L5580:
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	96(%esp)
	.cfi_def_cfa_offset 176
	pushl	$0
	.cfi_def_cfa_offset 180
	pushl	%ecx
	.cfi_def_cfa_offset 184
	pushl	104(%esp)
	.cfi_def_cfa_offset 188
	pushl	104(%esp)
	.cfi_def_cfa_offset 192
	pushl	76(%esp)
	.cfi_def_cfa_offset 196
	pushl	$1
	.cfi_def_cfa_offset 200
	pushl	$48
	.cfi_def_cfa_offset 204
	pushl	%esi
	.cfi_def_cfa_offset 208
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 160
	testl	$1073741824, %eax
	jne	.L5618
.L5579:
	addl	$1, 40(%esp)
	movl	52(%esp), %ecx
	movl	40(%esp), %eax
	addl	%ecx, 32(%esp)
	movl	96(%esp), %ecx
	addl	%ecx, 56(%esp)
	movl	68(%esp), %ecx
	addl	%ecx, 60(%esp)
	cmpl	%eax, 28(%esp)
	jne	.L5581
.L5606:
	addl	$140, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5618:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	$.LC215
	.cfi_def_cfa_offset 176
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	jmp	.L5579
.L5617:
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	$.LC215
	.cfi_def_cfa_offset 176
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	jmp	.L5573
.L5616:
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	$.LC215
	.cfi_def_cfa_offset 176
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	jmp	.L5571
.L5615:
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	$.LC215
	.cfi_def_cfa_offset 176
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	jmp	.L5568
.L5614:
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	imull	$140, 40(%esp), %eax
	imull	48(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 176
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	movl	%eax, ext2_g_inode_table
	jmp	.L5565
.L5613:
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	36(%esp)
	.cfi_def_cfa_offset 176
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	movl	%eax, ext2_g_ib
	jmp	.L5563
.L5612:
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	pushl	36(%esp)
	.cfi_def_cfa_offset 176
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	movl	%eax, ext2_g_bb
	jmp	.L5562
.L5611:
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	movl	40(%esp), %ecx
	movl	%ecx, %eax
	imull	%ecx, %eax
	sall	$10, %eax
	pushl	%eax
	.cfi_def_cfa_offset 176
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	movl	%eax, ext2_g_gdesc
	jmp	.L5560
.L5610:
	subl	$12, %esp
	.cfi_def_cfa_offset 172
	imull	$204, 40(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 176
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 160
	movl	%eax, ext2_g_sblock
	jmp	.L5559
	.cfi_endproc
.LFE516:
	.size	ext2_set_block_group, .-ext2_set_block_group
	.align 16
	.globl	ext2_inode_from_offset
	.type	ext2_inode_from_offset, @function
ext2_inode_from_offset:
.LFB517:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$24, %esp
	.cfi_def_cfa_offset 32
	movl	ext2_g_inode_table, %ebx
	testl	%ebx, %ebx
	je	.L5633
.L5620:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$140
	.cfi_def_cfa_offset 48
	call	*kmalloc
	imull	$140, 48(%esp), %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	addl	%edx, %ebx
	xorl	%edx, %edx
	.align 16
.L5627:
	movzbl	(%ebx,%edx), %ecx
	movb	%cl, (%eax,%edx)
	addl	$1, %edx
	cmpl	$140, %edx
	jne	.L5627
.L5619:
	addl	$24, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L5633:
	.cfi_restore_state
	movl	ext2_g_sblock, %edx
	testl	%edx, %edx
	je	.L5621
	movl	ext2_g_gdesc, %eax
	testl	%eax, %eax
	je	.L5621
	movl	%edx, 8(%esp)
	movl	%eax, 12(%esp)
.L5625:
	call	ext2_get_inode_table.part.22
	movl	%eax, %ebx
.L5626:
	movl	%ebx, ext2_g_inode_table
	jmp	.L5620
.L5621:
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 44
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	jne	.L5634
	movl	ext2_g_inode_table, %ebx
	testl	%ebx, %ebx
	jne	.L5626
	movl	12(%esp), %eax
	jmp	.L5625
	.align 16
.L5634:
	xorl	%eax, %eax
	jmp	.L5619
	.cfi_endproc
.LFE517:
	.size	ext2_inode_from_offset, .-ext2_inode_from_offset
	.align 16
	.globl	ext2_get_inode_table
	.type	ext2_get_inode_table, @function
ext2_get_inode_table:
.LFB518:
	.cfi_startproc
	movl	ext2_g_inode_table, %eax
	movl	4(%esp), %edx
	testl	%eax, %eax
	je	.L5637
	rep ret
	.align 16
.L5637:
	movl	%edx, %eax
	jmp	ext2_get_inode_table.part.22
	.cfi_endproc
.LFE518:
	.size	ext2_get_inode_table, .-ext2_get_inode_table
	.align 16
	.globl	ext2_get_name_of_dir
	.type	ext2_get_name_of_dir, @function
ext2_get_name_of_dir:
.LFB520:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	ext2_g_sblock, %eax
	movl	64(%esp), %edi
	testl	%eax, %eax
	je	.L5639
	movl	ext2_g_gdesc, %edx
	testl	%edx, %edx
	je	.L5639
	movl	%eax, 24(%esp)
	movl	%edx, 28(%esp)
.L5641:
	movl	ext2_g_inode_table, %eax
	testl	%eax, %eax
	je	.L5663
.L5643:
	movl	ext2_root, %eax
	movl	4(%eax), %eax
	cmpl	%eax, 4(%edi)
	je	.L5664
	cmpl	$2, 12(%edi)
	je	.L5665
	movl	0, %eax
	ud2
.L5665:
	movl	$1, %edx
	movl	%edi, %eax
	call	ext2_dirent_from_dir.part.18
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	(%eax)
	.cfi_def_cfa_offset 80
	call	ext2_inode_from_offset
	movl	$1024, (%esp)
	movl	%eax, %ebx
	call	*kmalloc
	addl	$12, %esp
	.cfi_def_cfa_offset 68
	movl	%eax, %esi
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$0
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	ext2_block_of_set
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	je	.L5646
	movl	$0, 8(%esp)
	xorl	%ebp, %ebp
	.align 16
.L5647:
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	pushl	%esi
	.cfi_def_cfa_offset 72
	pushl	%ebp
	.cfi_def_cfa_offset 76
	addl	$1, %ebp
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	ext2_dirent_from_dir_data
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	movl	%eax, %edx
	je	.L5666
.L5648:
	cmpb	$2, 7(%edx)
	jne	.L5647
	movl	4(%edi), %eax
	cmpl	%eax, (%edx)
	jne	.L5647
	movl	8(%edx), %eax
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5666:
	.cfi_restore_state
	addl	$1, 8(%esp)
	movl	%eax, 12(%esp)
	subl	$4, %esp
	.cfi_def_cfa_offset 68
	movl	12(%esp), %eax
	pushl	%esi
	.cfi_def_cfa_offset 72
	pushl	%eax
	.cfi_def_cfa_offset 76
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	ext2_block_of_set
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	movl	12(%esp), %edx
	jne	.L5648
.L5646:
	xorl	%eax, %eax
.L5667:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5639:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	leal	36(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 76
	leal	36(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	je	.L5641
	xorl	%eax, %eax
	jmp	.L5667
.L5664:
	movl	ext2_root_name, %eax
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5663:
	.cfi_restore_state
	movl	28(%esp), %eax
	call	ext2_get_inode_table.part.22
	movl	%eax, ext2_g_inode_table
	jmp	.L5643
	.cfi_endproc
.LFE520:
	.size	ext2_get_name_of_dir, .-ext2_get_name_of_dir
	.align 16
	.globl	ext2_free_data_blocks
	.type	ext2_free_data_blocks, @function
ext2_free_data_blocks:
.LFB521:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	ext2_g_sblock, %eax
	movl	68(%esp), %ebx
	testl	%eax, %eax
	je	.L5669
	movl	ext2_g_gdesc, %ecx
	testl	%ecx, %ecx
	je	.L5669
	movl	%eax, 24(%esp)
	movl	%ecx, 28(%esp)
.L5671:
	movl	20(%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	leal	-1(%eax), %edi
	shrl	$10, %edi
	leal	1(%edi), %eax
	addl	$4, %edi
	leal	0(,%edi,4), %ebp
	movl	%eax, 24(%esp)
	pushl	%ebx
	.cfi_def_cfa_offset 80
	call	ext2_block_locs
	movl	%ebp, (%esp)
	movl	%eax, %esi
	subl	$12, %ebp
	call	*kmalloc
	movl	100(%ebx), %ecx
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	%ecx, (%eax)
	movl	104(%ebx), %ecx
	movl	%ecx, 4(%eax)
	movl	108(%ebx), %edx
	movl	%edx, 8(%eax)
	xorl	%edx, %edx
	.align 16
.L5673:
	movzbl	(%esi,%edx), %ecx
	movb	%cl, 12(%eax,%edx)
	addl	$1, %edx
	cmpl	%edx, %ebp
	jne	.L5673
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edi
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	ext2_free_blocks
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	movl	%eax, %esi
	je	.L5686
.L5674:
	movl	$1, %esi
.L5668:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%esi, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5686:
	.cfi_restore_state
	movl	28(%esp), %eax
	movzwl	12(%esp), %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	addw	%bx, 12(%eax)
	movl	curr_port, %ebx
	addw	$1, 14(%eax)
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	$0
	.cfi_def_cfa_offset 84
	pushl	24(%eax)
	.cfi_def_cfa_offset 88
	pushl	$512
	.cfi_def_cfa_offset 92
	pushl	$1
	.cfi_def_cfa_offset 96
	pushl	%eax
	.cfi_def_cfa_offset 100
	pushl	$1
	.cfi_def_cfa_offset 104
	pushl	$48
	.cfi_def_cfa_offset 108
	pushl	%ebx
	.cfi_def_cfa_offset 112
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 64
	testl	$1073741824, %eax
	je	.L5668
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$.LC215
	.cfi_def_cfa_offset 80
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	movl	%esi, %eax
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5669:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	leal	36(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 76
	leal	36(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	je	.L5671
	jmp	.L5674
	.cfi_endproc
.LFE521:
	.size	ext2_free_data_blocks, .-ext2_free_data_blocks
	.align 16
	.globl	ext2_remove_dirent
	.type	ext2_remove_dirent, @function
ext2_remove_dirent:
.LFB522:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	48(%esp), %eax
	cmpl	$2, 12(%eax)
	je	.L5721
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5721:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$12
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%eax, 28(%esp)
	popl	%eax
	.cfi_def_cfa_offset 60
	movl	60(%esp), %eax
	pushl	20(%eax)
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%eax, %ebx
	movl	64(%esp), %eax
	movl	20(%eax), %eax
	movl	%eax, 24(%esp)
	shrl	$10, %eax
	movl	%eax, (%esp)
	call	*kmalloc
	movl	64(%esp), %edi
	movl	%eax, %ebp
	movl	64(%esp), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movzwl	8(%edi), %edx
	movl	20(%eax), %eax
	testl	%edx, _Rlogged
	jne	.L5722
.L5689:
	movl	%eax, %esi
	shrl	$10, %esi
	testl	%esi, %esi
	je	.L5690
	movl	48(%esp), %edi
	xorl	%esi, %esi
	.align 16
.L5692:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	%eax, 0(%ebp,%esi,4)
	xorl	%edx, %edx
	.align 16
.L5691:
	movzbl	(%ebx,%edx), %ecx
	movb	%cl, (%eax,%edx)
	addl	$1, %edx
	cmpl	$1024, %edx
	jne	.L5691
	movl	20(%edi), %eax
	addl	$1, %esi
	addl	$1024, %ebx
	movl	%eax, %edx
	shrl	$10, %edx
	cmpl	%esi, %edx
	ja	.L5692
.L5690:
	movl	0(%ebp), %edi
	testl	%edi, %edi
	je	.L5704
	movl	52(%esp), %ecx
	subl	$1, %eax
	xorl	%ebx, %ebx
	shrl	$10, %eax
	movl	%eax, 8(%esp)
	movl	4(%ecx), %edx
	movl	(%edi), %ecx
.L5694:
	leal	(%esi,%ecx), %eax
	cmpl	%edx, (%eax)
	je	.L5723
.L5695:
	movzwl	4(%eax), %eax
	testw	%ax, %ax
	je	.L5698
	movzwl	%ax, %eax
	addl	%eax, %esi
	leal	(%esi,%ecx), %eax
	cmpl	%edx, (%eax)
	jne	.L5695
.L5723:
	movl	12(%esp), %ecx
	movl	%edx, (%ecx)
	movl	(%edi), %ebp
	addl	%esi, %ebp
	movzwl	4(%ebp), %eax
	movw	%ax, 4(%ecx)
	movzwl	%ax, %eax
	testl	%eax, %eax
	je	.L5696
	movl	$1024, %ecx
	movl	%eax, %edx
	subl	%esi, %ecx
	negl	%edx
	subl	%eax, %ecx
	addl	%ebp, %eax
	call	shiftData.part.14
.L5696:
	movl	48(%esp), %eax
	movzwl	8(%eax), %eax
	testl	%eax, _Wlogged
	jne	.L5724
	xorl	%eax, %eax
.L5725:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L5722:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	%eax, %ecx
	xorl	%edx, %edx
	pushl	%ebx
	.cfi_def_cfa_offset 64
	movl	64(%esp), %eax
	call	ext2_read.part.17
	movl	64(%esp), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	20(%eax), %eax
	jmp	.L5689
.L5704:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5698:
	.cfi_restore_state
	addl	$1, %ebx
	cmpl	8(%esp), %ebx
	ja	.L5704
	movl	0(%ebp,%ebx,4), %edi
	testl	%edi, %edi
	je	.L5704
	movl	(%edi), %ecx
	xorl	%esi, %esi
	jmp	.L5694
.L5724:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	sall	$10, %ebx
	movl	$1024, %ecx
	pushl	%edi
	.cfi_def_cfa_offset 64
	movl	64(%esp), %eax
	movl	%ebx, %edx
	call	ext2_write.part.20
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	xorl	%eax, %eax
	jmp	.L5725
	.cfi_endproc
.LFE522:
	.size	ext2_remove_dirent, .-ext2_remove_dirent
	.align 16
	.globl	ext2_initialize
	.type	ext2_initialize, @function
ext2_initialize:
.LFB524:
	.cfi_startproc
	movl	4(%esp), %eax
	cmpl	$4194304, %eax
	jbe	.L5728
	movl	$1, %eax
	ret
	.align 16
.L5728:
	jmp	ext2_initialize.part.23
	.cfi_endproc
.LFE524:
	.size	ext2_initialize, .-ext2_initialize
	.align 16
	.globl	Randomizer
	.type	Randomizer, @function
Randomizer:
.LFB525:
	.cfi_startproc
	xorl	%eax, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $112
/  0 "" 2
/  45 "Library/sys.h" 1
	inb $113, %al
/  0 "" 2
/NO_APP
	movzbl	%al, %eax
	addl	$9, %eax
	movl	%eax, seed1
	movl	HPET_main_counter, %eax
	movl	(%eax), %eax
	leal	3(%eax,%eax,2), %eax
	movl	%eax, seed2
	movl	Lapic, %eax
	movl	912(%eax), %eax
	addl	$999, %eax
	movl	%eax, seed3
	ret
	.cfi_endproc
.LFE525:
	.size	Randomizer, .-Randomizer
	.align 16
	.globl	random
	.type	random, @function
random:
.LFB526:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$36, %esp
	.cfi_def_cfa_offset 56
	movl	seed3, %ebx
	movl	seed1, %ecx
	movl	$0, 4(%esp)
	fnstcw	22(%esp)
	imull	$4845, %ebx, %edx
	movl	%ecx, %ebp
	imull	$99231, %ecx, %eax
	addl	%edx, %eax
	addl	seed2, %eax
	xorl	%edx, %edx
	imull	%ebx, %ebp
	movl	%eax, (%esp)
	leal	9(%ebx,%ebx,2), %eax
	fildq	(%esp)
	movl	$0, 4(%esp)
	movl	%eax, (%esp)
	movzwl	22(%esp), %eax
	fildq	(%esp)
	fdivrp	%st, %st(1)
	movb	$12, %ah
	movw	%ax, 20(%esp)
	movl	HPET_main_counter, %eax
	fldcw	20(%esp)
	fistpq	8(%esp)
	fldcw	22(%esp)
	movl	(%eax), %eax
	movl	8(%esp), %esi
	addl	$1, %eax
	imull	$178481, %eax, %eax
	divl	%ebp
	leal	99(%ecx), %eax
	movl	%edx, %ebp
	sall	$10, %ebp
	subl	%edx, %ebp
	movl	%eax, %edx
	movl	%esi, %eax
	movl	%edx, (%esp)
	xorl	%edx, %edx
	addl	seed2, %ebp
	divl	(%esp)
	movl	%ebp, seed2
	addl	%edx, %ecx
	movl	$33818641, %edx
	movl	%ecx, %eax
	mull	%edx
	movl	%ecx, %eax
	subl	%edx, %eax
	shrl	%eax
	addl	%eax, %edx
	shrl	$6, %edx
	movl	%edx, %eax
	sall	$7, %eax
	subl	%edx, %eax
	xorl	%edx, %edx
	subl	%eax, %ecx
	leal	(%esi,%ebp), %eax
	addl	$3, %ecx
	movl	%ecx, seed1
	leal	1023(%ebx), %ecx
	divl	%ecx
	movl	%esi, %eax
	addl	%ebx, %edx
	movzwl	%dx, %edx
	addl	$127, %edx
	movl	%edx, seed3
	addl	$36, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE526:
	.size	random, .-random
	.section	.rodata
.LC574:
	.string	"\nCoPrimes: %d, CoFactors: %d"
.LC576:
	.string	"\nCalculated: %f"
.LC578:
	.string	"\nError: %f"
.LC581:
	.string	"\nError Percentage: %f"
	.text
	.align 16
	.globl	RandomnessCalculator
	.type	RandomnessCalculator, @function
RandomnessCalculator:
.LFB527:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	52(%esp), %eax
	call	*%eax
	movl	56(%esp), %eax
	testl	%eax, %eax
	je	.L5743
	xorl	%ebp, %ebp
	movl	$0, (%esp)
	movl	$0, 8(%esp)
	.align 16
.L5738:
	call	*48(%esp)
	xorl	%edx, %edx
	divl	60(%esp)
	movl	%edx, %ebx
	call	*48(%esp)
	xorl	%edx, %edx
	divl	60(%esp)
	cmpl	%edx, %ebx
	movl	%edx, %edi
	movl	%edx, %esi
	cmovle	%ebx, %edi
	cmpl	$2, %edi
	jle	.L5734
	movl	$2, %ecx
	.align 16
.L5737:
	movl	%ebx, %eax
	cltd
	idivl	%ecx
	testl	%edx, %edx
	jne	.L5735
	movl	%esi, %eax
	cltd
	idivl	%ecx
	testl	%edx, %edx
	je	.L5736
.L5735:
	addl	$1, %ecx
	cmpl	%edi, %ecx
	jne	.L5737
.L5734:
	addl	$1, (%esp)
.L5742:
	addl	$1, %ebp
	cmpl	56(%esp), %ebp
	jne	.L5738
.L5733:
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	pushl	12(%esp)
	.cfi_def_cfa_offset 56
	pushl	8(%esp)
	.cfi_def_cfa_offset 60
	pushl	$.LC574
	.cfi_def_cfa_offset 64
	call	printf
	fildl	16(%esp)
	fildl	24(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	fadd	%st(1), %st
	fmuls	.LC575
	fdivp	%st, %st(1)
	fld	%st(0)
	fsqrt
	fldz
	fucomip	%st(2), %st
	jbe	.L5749
	fstpl	(%esp)
	subl	$16, %esp
	.cfi_def_cfa_offset 64
	fstpl	(%esp)
	call	sqrt
	fstp	%st(0)
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	fldl	(%esp)
	jmp	.L5739
.L5749:
	fstp	%st(1)
.L5739:
	fstps	12(%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	flds	24(%esp)
	fstl	(%esp)
	fstpl	12(%esp)
	pushl	$.LC576
	.cfi_def_cfa_offset 64
	call	printf
	fldl	16(%esp)
	fsubrl	.LC577
	fstps	28(%esp)
	flds	28(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	fldz
	fucomip	%st(1), %st
	jbe	.L5740
	fchs
.L5740:
	fld	%st(0)
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	fstps	20(%esp)
	fstl	(%esp)
	fstpl	12(%esp)
	pushl	$.LC578
	.cfi_def_cfa_offset 64
	call	printf
	fldl	16(%esp)
	movl	$.LC581, (%esp)
	fdivl	.LC579
	fstps	28(%esp)
	flds	28(%esp)
	fmuls	.LC580
	fstpl	4(%esp)
	call	printf
	flds	24(%esp)
	addl	$44, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5736:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -20
	.cfi_offset 5, -8
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	addl	$1, 8(%esp)
	jmp	.L5742
.L5743:
	movl	$0, (%esp)
	movl	$0, 8(%esp)
	jmp	.L5733
	.cfi_endproc
.LFE527:
	.size	RandomnessCalculator, .-RandomnessCalculator
	.align 16
	.globl	CheckNeuralSCE
	.type	CheckNeuralSCE, @function
CheckNeuralSCE:
.LFB528:
	.cfi_startproc
	subl	$20, %esp
	.cfi_def_cfa_offset 24
	xorl	%edx, %edx
	movl	24(%esp), %eax
	movl	%edx, 4(%esp)
	xorl	%edx, %edx
	movl	%eax, (%esp)
	movl	28(%esp), %eax
	fildq	(%esp)
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	fildq	(%esp)
	fsubrp	%st, %st(1)
	fadd	%st(0), %st
	fldz
	fucomip	%st(1), %st
	fld	%st(0)
	ja	.L5757
	fadds	.LC543
.L5753:
	fdivr	%st(1), %st
	fadds	.LC543
	fmulp	%st, %st(1)
	fdivs	.LC582
	addl	$20, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5757:
	.cfi_restore_state
	fsubrs	.LC543
	jmp	.L5753
	.cfi_endproc
.LFE528:
	.size	CheckNeuralSCE, .-CheckNeuralSCE
	.align 16
	.globl	NeuralForwardUpdater
	.type	NeuralForwardUpdater, @function
NeuralForwardUpdater:
.LFB529:
	.cfi_startproc
	movl	NeuralBase, %eax
	movl	NeuralBase, %ecx
	movl	Neurons, %eax
	testl	%eax, %eax
	je	.L5787
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	fldz
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	addl	$136, %ecx
	xorl	%ebp, %ebp
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	.align 16
.L5763:
	movl	-136(%ecx), %eax
	xorl	%edx, %edx
	movl	%edx, 4(%esp)
	xorl	%edx, %edx
	movl	%eax, (%esp)
	movl	-132(%ecx), %eax
	fildq	(%esp)
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	fildq	(%esp)
	fsubrp	%st, %st(1)
	fadd	%st(0), %st
	fxch	%st(1)
	fucomi	%st(1), %st
	ja	.L5791
	fld1
	fadd	%st(2), %st
.L5771:
	fdivr	%st(2), %st
	fld1
	fadd	%st, %st(1)
	fxch	%st(3)
	fmulp	%st, %st(1)
	flds	.LC582
	fdivrp	%st, %st(1)
	fldz
	fxch	%st(1)
	fucomi	%st(1), %st
	jbe	.L5794
	movzwl	-128(%ecx), %eax
	leal	-120(%ecx), %edi
	testl	%eax, %eax
	movl	%eax, 8(%esp)
	je	.L5795
	fmuls	.LC583
	fnstcw	18(%esp)
	movzwl	18(%esp), %eax
	xorl	%edx, %edx
	movb	$12, %ah
	movw	%ax, 16(%esp)
	faddp	%st, %st(3)
	jmp	.L5768
	.align 16
.L5793:
	addl	$1, %edx
	cmpl	%edx, 8(%esp)
	je	.L5796
	.align 16
.L5768:
	fildl	(%ecx,%edx,4)
	movl	(%edi,%edx,4), %ebx
	xorl	%esi, %esi
	movl	$0, 4(%esp)
	fmul	%st(3), %st
	fldcw	16(%esp)
	fistpl	12(%esp)
	fldcw	18(%esp)
	movl	12(%esp), %eax
	addl	(%ebx), %eax
	movl	%eax, (%ebx)
	movl	4(%ebx), %ebx
	movl	%eax, (%esp)
	fildq	(%esp)
	movl	%esi, 4(%esp)
	movl	%ebx, (%esp)
	fildq	(%esp)
	fsubrp	%st, %st(1)
	fadd	%st(0), %st
	fxch	%st(1)
	fucomi	%st(1), %st
	ja	.L5792
	fld1
	fadd	%st(2), %st
.L5770:
	fdivr	%st(2), %st
	fadds	.LC543
	fmulp	%st, %st(2)
	fxch	%st(1)
	fdivs	.LC582
	fldz
	fxch	%st(1)
	fucomip	%st(1), %st
	fstp	%st(0)
	jp	.L5767
	je	.L5793
.L5767:
	movl	(%ecx,%edx,4), %eax
	leal	(%eax,%eax,4), %ebx
	movl	%ebx, %eax
	shrl	$31, %eax
	addl	%ebx, %eax
	sarl	%eax
	movl	%eax, (%ecx,%edx,4)
	addl	$1, %edx
	cmpl	%edx, 8(%esp)
	jne	.L5768
	fstp	%st(0)
	fstp	%st(1)
	jmp	.L5764
	.align 16
.L5794:
	fstp	%st(0)
	fstp	%st(0)
	fstp	%st(1)
	jmp	.L5764
	.align 16
.L5795:
	fstp	%st(0)
	fstp	%st(0)
	fstp	%st(1)
	jmp	.L5764
.L5796:
	fstp	%st(0)
	fstp	%st(1)
	.align 16
.L5764:
	movl	Neurons, %eax
	addl	$1, %ebp
	addl	$496, %ecx
	cmpl	%eax, %ebp
	jb	.L5763
	fstp	%st(0)
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
.L5787:
	rep ret
	.align 16
.L5792:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -20
	.cfi_offset 5, -8
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	fld	%st(1)
	fsubrs	.LC543
	jmp	.L5770
	.align 16
.L5791:
	fld	%st(1)
	fsubrs	.LC543
	jmp	.L5771
	.cfi_endproc
.LFE529:
	.size	NeuralForwardUpdater, .-NeuralForwardUpdater
	.align 16
	.globl	NeuralBackwardUpdater
	.type	NeuralBackwardUpdater, @function
NeuralBackwardUpdater:
.LFB530:
	.cfi_startproc
	.align 16
.L5799:
	movl	NeuralBase, %eax
	jmp	.L5799
	.cfi_endproc
.LFE530:
	.size	NeuralBackwardUpdater, .-NeuralBackwardUpdater
	.section	.rodata
.LC584:
	.string	"IPCprocessorP"
	.text
	.align 16
	.globl	IPC_init
	.type	IPC_init, @function
IPC_init:
.LFB531:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	movl	MotherSpace, %eax
	movl	%eax, 12(%esp)
	movl	$0, 8(%esp)
	jmp	.L5801
.L5802:
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$24
	.cfi_def_cfa_offset 48
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, 4(%esp)
	movl	12(%esp), %eax
	movl	4(%esp), %edx
	movl	%edx, 72(%eax)
	movl	4(%esp), %eax
	movl	$0, (%eax)
	movl	4(%esp), %eax
	movl	$0, 20(%eax)
	movl	4(%esp), %eax
	movl	20(%eax), %edx
	movl	4(%esp), %eax
	movl	%edx, 16(%eax)
	movl	$IPCprocessorP_end_t, %edx
	movl	$IPCprocessorP_t, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, (%esp)
	movl	kmalloc, %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	12(%esp)
	.cfi_def_cfa_offset 48
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, %edx
	movl	4(%esp), %eax
	movl	%edx, 8(%eax)
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	pushl	4(%esp)
	.cfi_def_cfa_offset 40
	pushl	$IPCprocessorP_t
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	memcpy
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	4(%esp), %eax
	movl	8(%eax), %edx
	movl	4(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ecx
	.cfi_def_cfa_offset 44
	pushl	%edx
	.cfi_def_cfa_offset 48
	pushl	$4
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$4
	.cfi_def_cfa_offset 60
	pushl	$1115988625
	.cfi_def_cfa_offset 64
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	4(%esp), %eax
	movl	8(%eax), %edx
	movl	12(%esp), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ecx
	.cfi_def_cfa_offset 44
	pushl	%edx
	.cfi_def_cfa_offset 48
	pushl	$4
	.cfi_def_cfa_offset 52
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$4
	.cfi_def_cfa_offset 60
	pushl	$1115988626
	.cfi_def_cfa_offset 64
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	$Priority_promoter, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ecx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	$4
	.cfi_def_cfa_offset 52
	pushl	%edx
	.cfi_def_cfa_offset 56
	pushl	$4
	.cfi_def_cfa_offset 60
	pushl	$1115988627
	.cfi_def_cfa_offset 64
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	$Activate_task_direct_SP, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ecx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	$4
	.cfi_def_cfa_offset 52
	pushl	%edx
	.cfi_def_cfa_offset 56
	pushl	$4
	.cfi_def_cfa_offset 60
	pushl	$1115988628
	.cfi_def_cfa_offset 64
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	$printf, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ecx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	$4
	.cfi_def_cfa_offset 52
	pushl	%edx
	.cfi_def_cfa_offset 56
	pushl	$4
	.cfi_def_cfa_offset 60
	pushl	$1115988624
	.cfi_def_cfa_offset 64
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	$Task_sleep, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ecx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	$4
	.cfi_def_cfa_offset 52
	pushl	%edx
	.cfi_def_cfa_offset 56
	pushl	$4
	.cfi_def_cfa_offset 60
	pushl	$1115988629
	.cfi_def_cfa_offset 64
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	$Task_wakeup, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ecx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	$4
	.cfi_def_cfa_offset 52
	pushl	%edx
	.cfi_def_cfa_offset 56
	pushl	$4
	.cfi_def_cfa_offset 60
	pushl	$1115988630
	.cfi_def_cfa_offset 64
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	$Priority_changer, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ecx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	$4
	.cfi_def_cfa_offset 52
	pushl	%edx
	.cfi_def_cfa_offset 56
	pushl	$4
	.cfi_def_cfa_offset 60
	pushl	$1115988631
	.cfi_def_cfa_offset 64
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	$Task_Refresh, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ecx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	$4
	.cfi_def_cfa_offset 52
	pushl	%edx
	.cfi_def_cfa_offset 56
	pushl	$4
	.cfi_def_cfa_offset 60
	pushl	$1115988632
	.cfi_def_cfa_offset 64
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	$_kill, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ecx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	$4
	.cfi_def_cfa_offset 52
	pushl	%edx
	.cfi_def_cfa_offset 56
	pushl	$4
	.cfi_def_cfa_offset 60
	pushl	$1115988633
	.cfi_def_cfa_offset 64
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	addl	%edx, %eax
	movl	%eax, %ecx
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	movl	$Task_Swap, %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	%ecx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	$4
	.cfi_def_cfa_offset 52
	pushl	%edx
	.cfi_def_cfa_offset 56
	pushl	$4
	.cfi_def_cfa_offset 60
	pushl	$1115988640
	.cfi_def_cfa_offset 64
	call	ByteSequence_Replace
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	kernel_proc, %edx
	movl	4(%esp), %eax
	movl	8(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	%edx
	.cfi_def_cfa_offset 48
	pushl	$514
	.cfi_def_cfa_offset 52
	pushl	$20
	.cfi_def_cfa_offset 56
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	$.LC584
	.cfi_def_cfa_offset 64
	call	create_task
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, %edx
	movl	4(%esp), %eax
	movl	%edx, 12(%eax)
	movl	4(%esp), %eax
	movl	12(%eax), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	20(%esp)
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	Activate_task_direct_SP
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	addl	$76, 12(%esp)
	addl	$1, 8(%esp)
.L5801:
	movl	total_CPU_Cores, %eax
	leal	-1(%eax), %edx
	movl	8(%esp), %eax
	cmpl	%eax, %edx
	ja	.L5802
	nop
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE531:
	.size	IPC_init, .-IPC_init
	.align 16
	.globl	IPCdispatch
	.type	IPCdispatch, @function
IPCdispatch:
.LFB532:
	.cfi_startproc
	subl	$16, %esp
	.cfi_def_cfa_offset 20
	movl	20(%esp), %eax
	movl	8(%eax), %eax
	movl	72(%eax), %eax
	movl	%eax, 12(%esp)
	nop
.L5804:
	movl	12(%esp), %eax
	movl	4(%eax), %eax
	testl	%eax, %eax
	jne	.L5804
	movl	12(%esp), %eax
	movl	$1, 4(%eax)
	movl	12(%esp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	.L5805
	movl	12(%esp), %eax
	movl	20(%esp), %edx
	movl	%edx, 16(%eax)
	jmp	.L5806
.L5805:
	movl	12(%esp), %eax
	movl	20(%eax), %eax
	movl	20(%esp), %edx
	movl	%edx, 40(%eax)
.L5806:
	movl	12(%esp), %eax
	movl	20(%esp), %edx
	movl	%edx, 20(%eax)
	movl	20(%esp), %eax
	movl	$0, 40(%eax)
	movl	12(%esp), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	12(%esp), %eax
	movl	%edx, (%eax)
	movl	12(%esp), %eax
	movl	$0, 4(%eax)
	nop
	addl	$16, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE532:
	.size	IPCdispatch, .-IPCdispatch
	.align 16
	.globl	IPCprocessorP_t
	.type	IPCprocessorP_t, @function
IPCprocessorP_t:
.LFB533:
	.cfi_startproc
	subl	$60, %esp
	.cfi_def_cfa_offset 64
	movl	$1115988625, 44(%esp)
	movl	$1115988626, 40(%esp)
	movl	$1115988627, 36(%esp)
	movl	$1115988628, 32(%esp)
	movl	$1115988624, 28(%esp)
	movl	$1115988629, 24(%esp)
	movl	$1115988630, 20(%esp)
	movl	$1115988631, 16(%esp)
	movl	$1115988632, 12(%esp)
	movl	$1115988633, 8(%esp)
	movl	$1115988640, 4(%esp)
.L5822:
/APP
/  84 "Source/multitasking/IPCInterface/IPCInterface.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	44(%esp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	.L5808
	movl	44(%esp), %eax
	movl	16(%eax), %eax
	movl	%eax, (%esp)
	jmp	.L5809
.L5810:
/APP
/  93 "Source/multitasking/IPCInterface/IPCInterface.c" 1
	int $50
/  0 "" 2
/NO_APP
.L5809:
	movl	44(%esp), %eax
	movl	4(%eax), %eax
	testl	%eax, %eax
	jne	.L5810
	movl	44(%esp), %eax
	movl	$1, 4(%eax)
	movl	(%esp), %eax
	movl	(%eax), %eax
	cmpl	$8, %eax
	ja	.L5811
	movl	.L5813(,%eax,4), %eax
	jmp	*%eax
	.section	.rodata
	.align 4
	.align 4
.L5813:
	.long	.L5811
	.long	.L5812
	.long	.L5814
	.long	.L5815
	.long	.L5816
	.long	.L5817
	.long	.L5818
	.long	.L5819
	.long	.L5820
	.text
.L5812:
	movl	(%esp), %eax
	movl	12(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	52(%esp), %eax
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5811
.L5814:
	movl	(%esp), %eax
	movl	12(%eax), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	48(%esp)
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	48(%esp), %eax
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5811
.L5815:
	movl	(%esp), %eax
	movl	12(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	40(%esp), %eax
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5811
.L5816:
	movl	(%esp), %eax
	movl	12(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	36(%esp), %eax
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5811
.L5817:
	movl	(%esp), %eax
	movl	20(%eax), %edx
	movl	(%esp), %eax
	movl	12(%eax), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	32(%esp), %eax
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5811
.L5818:
	movl	(%esp), %eax
	movl	20(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	movl	12(%eax), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	28(%esp), %eax
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5811
.L5819:
	movl	(%esp), %eax
	movl	12(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	24(%esp), %eax
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	jmp	.L5811
.L5820:
	movl	(%esp), %eax
	movl	20(%eax), %eax
	movl	%eax, %edx
	movl	(%esp), %eax
	movl	12(%eax), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	pushl	%edx
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	movl	20(%esp), %eax
	call	*%eax
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	nop
.L5811:
	movl	(%esp), %eax
	movl	40(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, 16(%eax)
	movl	44(%esp), %eax
	movl	(%eax), %eax
	leal	-1(%eax), %edx
	movl	44(%esp), %eax
	movl	%edx, (%eax)
	movl	(%esp), %eax
	movl	$1, 36(%eax)
	jmp	.L5821
.L5808:
	movl	multitasking_ON, %eax
.L5821:
	movl	44(%esp), %eax
	movl	$0, 4(%eax)
/APP
/  150 "Source/multitasking/IPCInterface/IPCInterface.c" 1
	int $50
/  0 "" 2
/NO_APP
	jmp	.L5822
	.cfi_endproc
.LFE533:
	.size	IPCprocessorP_t, .-IPCprocessorP_t
	.align 16
	.globl	IPCprocessorP_end_t
	.type	IPCprocessorP_end_t, @function
IPCprocessorP_end_t:
.LFB570:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE570:
	.size	IPCprocessorP_end_t, .-IPCprocessorP_end_t
	.comm	initial_esp,4,4
	.comm	initial_ebp,4,4
	.globl	calle
	.section	.bss
	.type	calle, @object
	.size	calle, 1
calle:
	.zero	1
	.section	.rodata
	.align 4
.LC585:
	.string	"\n\tLocation of var 1: %x, var 2: %x var 3: %x var 4: %x var 5: %x \n"
	.align 4
.LC586:
	.string	"\tPutting Magic Numbers into first two addresses\n"
	.align 4
.LC587:
	.string	"\tDone, Now Reading what we just wrote, 4284 on first few memory, 100 on others\n"
.LC588:
	.string	" %d "
.LC589:
	.string	" %x "
.LC590:
	.string	" %x %x "
	.align 4
.LC591:
	.string	"If you just saw few 4284's and 100's and nothing else, no extra space; everything worked fine!\n"
.LC592:
	.string	"Now Freeing the memory!\n"
	.align 4
.LC593:
	.string	" If you didnt saw any numbers above, It worked!!!\n"
	.align 4
.LC594:
	.string	"\tLocation of var 1: %x, var 2: %x %x\n"
	.text
	.align 16
	.globl	dbug
	.type	dbug, @function
dbug:
.LFB535:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	pushl	$810
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%eax, %ebp
	movl	$4284, (%eax)
	movl	$1024, (%esp)
	call	*kmalloc
	movl	$1048576, (%esp)
	movl	%eax, 20(%esp)
	movl	%eax, %esi
	call	*kmalloc
	movl	$432, (%esp)
	movl	%eax, 24(%esp)
	movl	%eax, %ebx
	call	*kmalloc
	movl	$128, (%esp)
	movl	%eax, 28(%esp)
	movl	%eax, %edi
	call	*kmalloc
	popl	%edx
	.cfi_def_cfa_offset 60
	popl	%ecx
	.cfi_def_cfa_offset 56
	pushl	%eax
	.cfi_def_cfa_offset 60
	pushl	%edi
	.cfi_def_cfa_offset 64
	pushl	%ebx
	.cfi_def_cfa_offset 68
	pushl	%esi
	.cfi_def_cfa_offset 72
	movl	%ebp, %esi
	pushl	%ebp
	.cfi_def_cfa_offset 76
	pushl	$.LC585
	.cfi_def_cfa_offset 80
	call	printf
	addl	$20, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC586
	.cfi_def_cfa_offset 64
	call	printf
	leal	32(%ebp), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	.align 16
.L5825:
	movl	$4284, (%esi)
	addl	$4, %esi
	cmpl	%eax, %esi
	jne	.L5825
	movl	4(%esp), %edi
	movl	%edi, %eax
	addl	$32, %eax
	.align 16
.L5826:
	movl	$100, (%edi)
	addl	$4, %edi
	cmpl	%edi, %eax
	jne	.L5826
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	%ebp, %ebx
	pushl	$.LC587
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	.align 16
.L5827:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	(%ebx)
	.cfi_def_cfa_offset 60
	addl	$4, %ebx
	pushl	$.LC588
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	%ebx, %esi
	jne	.L5827
	movl	4(%esp), %esi
	.align 16
.L5828:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	(%esi)
	.cfi_def_cfa_offset 60
	addl	$4, %esi
	pushl	$.LC589
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	%esi, %edi
	jne	.L5828
	movl	8(%esp), %eax
	movl	12(%esp), %edx
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	movl	$12345678, (%eax)
	movl	$87654321, (%edx)
	pushl	$87654321
	.cfi_def_cfa_offset 56
	pushl	(%eax)
	.cfi_def_cfa_offset 60
	pushl	$.LC590
	.cfi_def_cfa_offset 64
	call	printf
	movl	$.LC591, (%esp)
	call	printf
	movl	$.LC592, (%esp)
	call	printf
	popl	%eax
	.cfi_def_cfa_offset 60
	pushl	16(%esp)
	.cfi_def_cfa_offset 64
	call	*kfree
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	.align 16
.L5829:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	0(%ebp)
	.cfi_def_cfa_offset 60
	addl	$4, %ebp
	pushl	$.LC589
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	%ebp, %ebx
	jne	.L5829
	movl	4(%esp), %ebx
	.align 16
.L5830:
	subl	$8, %esp
	.cfi_def_cfa_offset 56
	pushl	(%ebx)
	.cfi_def_cfa_offset 60
	addl	$4, %ebx
	pushl	$.LC589
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	cmpl	%ebx, %esi
	jne	.L5830
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC593
	.cfi_def_cfa_offset 64
	call	printf
	movl	$32, (%esp)
	call	*kmalloc
	movl	$123, (%esp)
	movl	%eax, %ebx
	call	*kmalloc
	movl	%eax, %esi
	movl	$12, (%esp)
	call	*kmalloc
	pushl	%eax
	.cfi_def_cfa_offset 68
	pushl	%esi
	.cfi_def_cfa_offset 72
	pushl	%ebx
	.cfi_def_cfa_offset 76
	pushl	$.LC594
	.cfi_def_cfa_offset 80
	call	printf
	addl	$60, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE535:
	.size	dbug, .-dbug
	.comm	initial_eip,4,4
	.comm	TSS,4,4
	.section	.rodata
	.align 4
.LC595:
	.string	"\nDESCRIPTOR TABLES INITIALIZED \n"
.LC596:
	.string	"\nEnabling ACPI!\n"
.LC597:
	.string	"\nMouse Drivers initialized\n"
	.align 4
.LC598:
	.string	"\nKeyboard Drivers Initialized\n"
.LC599:
	.string	"\nAvailable memory: "
	.align 4
.LC600:
	.string	"\nInitializing Memory Manager!\n"
	.align 4
.LC601:
	.string	"\n Paging Has been Enabled Successfully!"
.LC602:
	.string	"\n Available Memory: %x KB\n"
	.align 4
.LC603:
	.string	"\n\nEnumerating all devices on PCI BUS:\n"
.LC604:
	.string	"\nEnabling Hard Disk\n"
.LC605:
	.string	"\nLOADING MAIN KERNEL...\n"
	.align 4
.LC606:
	.string	"\n\n\tType shutdown to do ACPI shutdown (wont work on certain systems)"
.LC607:
	.string	"\nsize of HPET_Table_t: %x"
.LC608:
	.string	"\nCPU has MSR"
	.align 4
.LC609:
	.string	"\n\nInitializing MultiThreading System"
	.text
	.align 16
	.globl	kernel_early
	.type	kernel_early, @function
kernel_early:
.LFB536:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$4, %esp
	.cfi_def_cfa_offset 16
	movl	16(%esp), %ebx
	call	console_init
	lock orl	$0, (%esp)
	movl	$0, printlockLocked
	call	init_gdt
	call	init_idt
	call	detect_cpu
	call	BasicCPU_Init
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC595
	.cfi_def_cfa_offset 32
	call	printf
	movl	$.LC596, (%esp)
	call	printf
	movl	$.LC597, (%esp)
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	.align 16
.L5839:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L5839
	movl	$-19, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
	.align 16
/NO_APP
.L5840:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$2, %al
	jne	.L5840
	movl	$5, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $96
/  0 "" 2
/NO_APP
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	movl	$Main_key_codes, scancodes
	pushl	$.LC598
	.cfi_def_cfa_offset 32
	call	printf
	movl	$.LC599, (%esp)
	call	printf
	popl	%ecx
	.cfi_def_cfa_offset 28
	pushl	8(%ebx)
	.cfi_def_cfa_offset 32
	call	printint
	movl	$.LC460, (%esp)
	call	printf
	movl	8(%ebx), %eax
	movl	48(%ebx), %esi
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	xorl	%ebx, %ebx
	movl	%eax, memAvailable
	movl	%eax, maxmem
	movl	%esi, %eax
	movl	%esi, mmap_info
	.align 16
.L5844:
	movl	(%eax), %edx
	testl	%edx, %edx
	je	.L5845
	movl	20(%eax), %edx
	subl	$8, %esp
	.cfi_def_cfa_offset 24
	pushl	strMemoryTypes-4(,%edx,4)
	.cfi_def_cfa_offset 28
	pushl	%edx
	.cfi_def_cfa_offset 32
	pushl	12(%eax)
	.cfi_def_cfa_offset 36
	pushl	4(%eax)
	.cfi_def_cfa_offset 40
	pushl	%ebx
	.cfi_def_cfa_offset 44
	pushl	$.LC461
	.cfi_def_cfa_offset 48
	addl	$1, %ebx
	call	printf
	movl	mmap_info, %eax
	addl	$32, %esp
	.cfi_def_cfa_offset 16
	movl	$1048130, 16(%eax)
	addl	$24, %eax
	cmpl	$20, %ebx
	movl	%eax, mmap_info
	jne	.L5844
.L5845:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC600
	.cfi_def_cfa_offset 32
	call	printf
	movl	maxmem, %eax
	movl	%esi, mmap_info
	sall	$10, %eax
	movl	%eax, max_mem
	call	Setup_Paging
	movl	$.LC601, (%esp)
	call	printf
	popl	%eax
	.cfi_def_cfa_offset 28
	popl	%edx
	.cfi_def_cfa_offset 24
	pushl	maxmem
	.cfi_def_cfa_offset 28
	pushl	$.LC602
	.cfi_def_cfa_offset 32
	call	printf
	movl	$.LC603, (%esp)
	call	printf
	call	checkAllBuses
	movl	$.LC604, (%esp)
	call	printf
	call	checkAHCI
	movl	$.LC605, (%esp)
	call	printf
	movl	$.LC606, (%esp)
	movl	$dbug, mdbug
	movl	$setVesa, vesa
	call	printf
	call	Init_fs
	popl	%ecx
	.cfi_def_cfa_offset 28
	popl	%ebx
	.cfi_def_cfa_offset 24
	pushl	$1016
	.cfi_def_cfa_offset 28
	pushl	$.LC607
	.cfi_def_cfa_offset 32
	call	printf
	movl	$1, %eax
/APP
/  19 "Source/arch/x86/cpu/cpu.c" 1
	cpuid
/  0 "" 2
/NO_APP
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	andl	$32, %edx
	jne	.L5851
.L5843:
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 28
	pushl	$4194304
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	%eax, console_dbuffer_original
	movl	console_dbuffer_original, %eax
	movl	$.LC609, (%esp)
	movl	%eax, console_dbuffer
	movl	console_dbuffer_original, %eax
	addl	$4194304, %eax
	movl	%eax, console_dbuffer_limit
	call	printf
/APP
/  58 "Source/Drivers/timer/timer.c" 1
	cli
/  0 "" 2
/NO_APP
	movl	$54, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $67
/  0 "" 2
/NO_APP
	movl	$-87, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $64
/  0 "" 2
/NO_APP
	movl	$4, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $64
/  0 "" 2
/  45 "Library/sys.h" 1
	inb $33, %al
/  0 "" 2
/NO_APP
	andl	$-2, %eax
/APP
/  37 "Library/sys.h" 1
	outb %al, $33
/  0 "" 2
/  216 "Source/kernel.c" 1
	sti
/  0 "" 2
/NO_APP
	call	init_multitasking
.L5851:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC608
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L5843
	.cfi_endproc
.LFE536:
	.size	kernel_early, .-kernel_early
	.align 16
	.globl	kernel_start
	.type	kernel_start, @function
kernel_start:
.LFB537:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE537:
	.size	kernel_start, .-kernel_start
	.align 16
	.globl	kernel_main
	.type	kernel_main, @function
kernel_main:
.LFB538:
	.cfi_startproc
	.align 16
.L5854:
	jmp	.L5854
	.cfi_endproc
.LFE538:
	.size	kernel_main, .-kernel_main
	.local	p.1398
	.comm	p.1398,4,4
	.section	.rodata
	.align 8
	.type	c8.7720, @object
	.size	c8.7720, 8
c8.7720:
	.long	-600767046
	.long	1052147681
	.align 8
	.type	c7overc8.7719, @object
	.size	c7overc8.7719, 8
c7overc8.7719:
	.long	-1121416287
	.long	1076303678
	.align 8
	.type	c6overc8.7718, @object
	.size	c6overc8.7718, 8
c6overc8.7718:
	.long	-1455927703
	.long	1079817750
	.align 8
	.type	c5overc8.7717, @object
	.size	c5overc8.7717, 8
c5overc8.7717:
	.long	-146903665
	.long	1083117799
	.align 8
	.type	c4overc8.7716, @object
	.size	c4overc8.7716, 8
c4overc8.7716:
	.long	60408694
	.long	1086063603
	.align 8
	.type	c3.7715, @object
	.size	c3.7715, 8
c3.7715:
	.long	1961972739
	.long	1068264202
	.align 8
	.type	c2.7714, @object
	.size	c2.7714, 8
c2.7714:
	.long	-49345798
	.long	1070514109
	.align 8
	.type	c1.7713, @object
	.size	c1.7713, 8
c1.7713:
	.long	-49728530
	.long	1072049730
	.align 8
	.type	c0.7712, @object
	.size	c0.7712, 8
c0.7712:
	.long	125482
	.long	1072693248
	.align 8
	.type	recip_ln2.7682, @object
	.size	recip_ln2.7682, 8
recip_ln2.7682:
	.long	1697350398
	.long	1073157447
	.local	dirent2.8022
	.comm	dirent2.8022,12,4
	.local	dirent2.8032
	.comm	dirent2.8032,12,4
	.text
	.align 16
	.type	strncmp.part.0, @function
strncmp.part.0:
.LFB539:
	.cfi_startproc
	testl	%ecx, %ecx
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	je	.L5856
	leal	-1(%ecx), %ebx
	movzbl	(%eax), %ecx
	cmpb	(%edx), %cl
	jne	.L5856
	testb	%cl, %cl
	je	.L5862
	testl	%ebx, %ebx
	jne	.L5859
	jmp	.L5862
	.align 16
.L5860:
	testl	%ebx, %ebx
	je	.L5862
	testb	%cl, %cl
	je	.L5862
.L5859:
	addl	$1, %eax
	addl	$1, %edx
	movzbl	(%eax), %ecx
	subl	$1, %ebx
	cmpb	(%edx), %cl
	je	.L5860
.L5856:
	movzbl	(%edx), %ebx
	xorl	%ecx, %ecx
	cmpb	%bl, (%eax)
	movl	$-1, %eax
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	seta	%cl
	cmovnb	%ecx, %eax
	ret
	.align 16
.L5862:
	.cfi_restore_state
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE539:
	.size	strncmp.part.0, .-strncmp.part.0
	.align 16
	.type	mouse_wait.part.1, @function
mouse_wait.part.1:
.LFB540:
	.cfi_startproc
	movl	$100000, %edx
	jmp	.L5872
	.align 16
.L5874:
	subl	$1, %edx
	je	.L5868
.L5872:
/APP
/  45 "Library/sys.h" 1
	inb $100, %al
/  0 "" 2
/NO_APP
	testb	$1, %al
	je	.L5874
.L5868:
	rep ret
	.cfi_endproc
.LFE540:
	.size	mouse_wait.part.1, .-mouse_wait.part.1
	.align 16
	.type	itoa.part.3, @function
itoa.part.3:
.LFB542:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	xorl	%esi, %esi
	testl	%eax, %eax
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	%edx, %edi
	je	.L5876
	.align 16
.L5877:
	xorl	%edx, %edx
	divl	%ecx
	movzbl	bchars(%edx), %ebx
	movl	%esi, %edx
	addl	$1, %esi
	movb	%bl, tbuf-1(%esi)
	testl	%eax, %eax
	jne	.L5877
	movl	%edi, %ecx
	leal	tbuf-1(%edx), %eax
	jmp	.L5878
	.align 16
.L5884:
	movzbl	(%eax), %ebx
	subl	$1, %eax
.L5878:
	movb	%bl, (%ecx)
	addl	$1, %ecx
	cmpl	$tbuf-1, %eax
	jne	.L5884
.L5876:
	movb	$0, (%edi,%esi)
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE542:
	.size	itoa.part.3, .-itoa.part.3
	.section	.rodata
.LC610:
	.string	"Mon"
.LC611:
	.string	"Tue"
.LC612:
	.string	"Wed"
.LC613:
	.string	"Thu"
.LC614:
	.string	"Fri"
.LC615:
	.string	"Sat"
	.align 4
	.type	CSWTCH.1398, @object
	.size	CSWTCH.1398, 24
CSWTCH.1398:
	.long	.LC610
	.long	.LC611
	.long	.LC612
	.long	.LC613
	.long	.LC614
	.long	.LC615
.LC616:
	.string	"Monday"
.LC617:
	.string	"Tuesday"
.LC618:
	.string	"Wednesday"
.LC619:
	.string	"Thursday"
.LC620:
	.string	"Friday"
.LC621:
	.string	"Saturday"
	.align 4
	.type	CSWTCH.1400, @object
	.size	CSWTCH.1400, 24
CSWTCH.1400:
	.long	.LC616
	.long	.LC617
	.long	.LC618
	.long	.LC619
	.long	.LC620
	.long	.LC621
.LC622:
	.string	"Feb"
.LC623:
	.string	"Mar"
.LC624:
	.string	"Apr"
.LC625:
	.string	"May"
.LC626:
	.string	"Jun"
.LC627:
	.string	"Jul"
.LC628:
	.string	"Aug"
.LC629:
	.string	"Sep"
.LC630:
	.string	"Oct"
.LC631:
	.string	"Nov"
.LC632:
	.string	"Dec"
	.align 32
	.type	CSWTCH.1402, @object
	.size	CSWTCH.1402, 44
CSWTCH.1402:
	.long	.LC622
	.long	.LC623
	.long	.LC624
	.long	.LC625
	.long	.LC626
	.long	.LC627
	.long	.LC628
	.long	.LC629
	.long	.LC630
	.long	.LC631
	.long	.LC632
.LC633:
	.string	"February"
.LC634:
	.string	"March"
.LC635:
	.string	"April"
.LC636:
	.string	"June"
.LC637:
	.string	"July"
.LC638:
	.string	"August"
.LC639:
	.string	"September"
.LC640:
	.string	"October"
.LC641:
	.string	"November"
.LC642:
	.string	"December"
	.align 32
	.type	CSWTCH.1404, @object
	.size	CSWTCH.1404, 44
CSWTCH.1404:
	.long	.LC633
	.long	.LC634
	.long	.LC635
	.long	.LC625
	.long	.LC636
	.long	.LC637
	.long	.LC638
	.long	.LC639
	.long	.LC640
	.long	.LC641
	.long	.LC642
.LC643:
	.string	"acpi poweroff failed.\n"
	.text
	.align 16
	.type	acpiPowerOff.part.8, @function
acpiPowerOff.part.8:
.LFB547:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	acpiEnable
	movzwl	SLP_EN, %ecx
	movl	PM1a_CNT, %edx
	movl	%ecx, %eax
	orw	SLP_TYPa, %ax
/APP
/  53 "Library/sys.h" 1
	outw %ax, %dx
/  0 "" 2
/NO_APP
	movl	PM1b_CNT, %edx
	testl	%edx, %edx
	je	.L5886
	movl	%ecx, %eax
	orw	SLP_TYPb, %ax
/APP
/  53 "Library/sys.h" 1
	outw %ax, %dx
/  0 "" 2
/NO_APP
.L5886:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC643
	.cfi_def_cfa_offset 32
	call	printf
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE547:
	.size	acpiPowerOff.part.8, .-acpiPowerOff.part.8
	.align 16
	.type	get_special_dirs.part.9, @function
get_special_dirs.part.9:
.LFB548:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$16, %esp
	.cfi_def_cfa_offset 28
	movl	root_dir, %ebx
	movl	curr_port, %esi
	movl	30(%ebx), %edx
	movl	26(%ebx), %eax
	pushl	$1
	.cfi_def_cfa_offset 32
	shrdl	$9, %edx, %eax
	movl	%edx, %ecx
	shrl	$9, %ecx
	pushl	%ecx
	.cfi_def_cfa_offset 36
	pushl	%eax
	.cfi_def_cfa_offset 40
	pushl	$512
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%ebx
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$32
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	je	.L5891
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC214
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L5891:
	addl	$4, %esp
	.cfi_def_cfa_offset 12
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE548:
	.size	get_special_dirs.part.9, .-get_special_dirs.part.9
	.align 16
	.type	flush_dir.part.10, @function
flush_dir.part.10:
.LFB549:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$16, %esp
	.cfi_def_cfa_offset 28
	movl	30(%eax), %ebx
	movl	26(%eax), %ecx
	movl	curr_port, %esi
	pushl	$1
	.cfi_def_cfa_offset 32
	shrdl	$9, %ebx, %ecx
	movl	%ebx, %edx
	shrl	$9, %edx
	pushl	%edx
	.cfi_def_cfa_offset 36
	pushl	%ecx
	.cfi_def_cfa_offset 40
	pushl	$512
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%eax
	.cfi_def_cfa_offset 52
	pushl	$1
	.cfi_def_cfa_offset 56
	pushl	$48
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	je	.L5898
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC215
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L5898:
	addl	$4, %esp
	.cfi_def_cfa_offset 12
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE549:
	.size	flush_dir.part.10, .-flush_dir.part.10
	.align 16
	.type	flush_file.part.11, @function
flush_file.part.11:
.LFB550:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$16, %esp
	.cfi_def_cfa_offset 28
	movl	30(%eax), %ebx
	movl	26(%eax), %ecx
	movl	curr_port, %esi
	pushl	$1
	.cfi_def_cfa_offset 32
	shrdl	$9, %ebx, %ecx
	movl	%ebx, %edx
	shrl	$9, %edx
	pushl	%edx
	.cfi_def_cfa_offset 36
	pushl	%ecx
	.cfi_def_cfa_offset 40
	pushl	$512
	.cfi_def_cfa_offset 44
	pushl	$1
	.cfi_def_cfa_offset 48
	pushl	%eax
	.cfi_def_cfa_offset 52
	pushl	$1
	.cfi_def_cfa_offset 56
	pushl	$48
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	je	.L5904
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC215
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
.L5904:
	addl	$4, %esp
	.cfi_def_cfa_offset 12
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE550:
	.size	flush_file.part.11, .-flush_file.part.11
	.align 16
	.type	powf.part.13, @function
powf.part.13:
.LFB552:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	subl	$36, %esp
	.cfi_def_cfa_offset 48
	movl	48(%esp), %edx
	movl	52(%esp), %ebx
	leal	-1(%edx), %ecx
	movl	%edx, %eax
	andl	$2147483647, %ebx
	andl	$2147483647, %eax
	cmpl	$2139095038, %ecx
	ja	.L5910
	leal	-1(%ebx), %ecx
	cmpl	$1258291198, %ecx
	jbe	.L5911
.L5910:
	testl	%ebx, %ebx
	fld1
	je	.L6016
	flds	48(%esp)
	fucomip	%st(0), %st
	jp	.L6028
	flds	52(%esp)
	fucomip	%st(0), %st
	jp	.L6029
	movl	%ebx, %edx
	shrl	$23, %edx
	movzbl	gMaskShift(%edx), %ecx
	movl	$1073741823, %edx
	shrl	%cl, %edx
	movl	%edx, %esi
	movl	$1073741824, %edx
	shrl	%cl, %edx
	andl	%ebx, %esi
	andl	%ebx, %edx
	testl	%eax, %eax
	jne	.L5917
	testl	%esi, %esi
	jne	.L6030
	testl	%edx, %edx
	je	.L6031
	fldz
	flds	52(%esp)
	fxch	%st(1)
	fucomip	%st(1), %st
	fstp	%st(0)
	ja	.L6021
	fstp	%st(0)
	flds	48(%esp)
	jmp	.L6016
.L6036:
	fstp	%st(0)
.L6016:
	addl	$36, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L5911:
	.cfi_restore_state
	cmpl	$1056964608, %ebx
	je	.L5929
	shrl	$23, %eax
	andl	$8388607, %edx
	subl	$127, %eax
	orl	$1065353216, %edx
	cmpl	$-127, %eax
	movl	%edx, 8(%esp)
	je	.L6022
.L5934:
	movl	%eax, 16(%esp)
	fildl	16(%esp)
	fld1
	flds	8(%esp)
	fucomip	%st(1), %st
	fstp	%st(0)
	jp	.L5977
	je	.L5935
.L5977:
	movl	8(%esp), %eax
	flds	8(%esp)
	shrl	$12, %eax
	andl	$2032, %eax
	fmull	powf_log_table(%eax)
	fxch	%st(1)
	faddl	powf_log_table+8(%eax)
	fxch	%st(1)
	fsubs	.LC543
	fld	%st(0)
	fmul	%st(1), %st
	fldl	.LC648
	fmul	%st(2), %st
	fsubs	.LC583
	fldl	.LC649
	fmul	%st(3), %st
	fsubs	.LC650
	fxch	%st(1)
	fmul	%st(2), %st
	faddp	%st, %st(3)
	fxch	%st(1)
	fmul	%st(0), %st
	fmulp	%st, %st(1)
	faddp	%st, %st(1)
	fmull	.LC651
	faddp	%st, %st(1)
.L5935:
	fmuls	52(%esp)
	flds	.LC652
	fxch	%st(1)
	fucomi	%st(1), %st
	fstp	%st(1)
	jnb	.L6023
	flds	.LC654
	fucomip	%st(1), %st
	jnb	.L6024
	fnstcw	26(%esp)
	movzwl	26(%esp), %eax
	movb	$12, %ah
	movw	%ax, 24(%esp)
	fldcw	24(%esp)
	fistl	20(%esp)
	fldcw	26(%esp)
	movl	20(%esp), %eax
	movl	%eax, 8(%esp)
	fildl	8(%esp)
	fsubrp	%st, %st(1)
	fldz
	fxch	%st(1)
	fucomi	%st(1), %st
	fstp	%st(1)
	jp	.L5980
	je	.L6012
.L5980:
	fld	%st(0)
	fmul	%st(1), %st
	fldl	.LC656
	fmul	%st(2), %st
	fldl	.LC657
	fmul	%st(3), %st
	fldl	.LC658
	fmul	%st(4), %st
	fxch	%st(4)
	fmull	.LC659
	fld	%st(3)
	fmul	%st(4), %st
	fxch	%st(3)
	faddl	.LC660
	fxch	%st(2)
	faddl	.LC661
	fxch	%st(5)
	faddl	.LC662
	fxch	%st(2)
	fmul	%st(4), %st
	fxch	%st(4)
	fmulp	%st, %st(5)
	fxch	%st(1)
	fadd	%st(2), %st
	faddp	%st, %st(3)
	faddl	.LC663
	faddp	%st, %st(3)
	fmull	.LC664
	fmulp	%st, %st(1)
	faddp	%st, %st(1)
.L5941:
	addl	$1023, %eax
	movl	%eax, %edx
	movl	$0, %eax
	sall	$20, %edx
	movl	%eax, 8(%esp)
	movl	%edx, 12(%esp)
	fmull	8(%esp)
	fstps	28(%esp)
	flds	28(%esp)
	jmp	.L6016
	.align 16
.L5917:
	cmpl	$2139095040, %ebx
	je	.L6025
	fstp	%st(0)
	flds	.LC646
	flds	48(%esp)
	fucomi	%st(1), %st
	fstp	%st(1)
	jbe	.L6006
	fldz
	flds	52(%esp)
	fxch	%st(1)
	fucomi	%st(1), %st
	fstp	%st(1)
	fcmovbe	%st(1), %st
	fstp	%st(1)
	jmp	.L6016
	.align 16
.L5929:
	fldz
	flds	48(%esp)
	fxch	%st(1)
	fucomi	%st(1), %st
	fstp	%st(1)
	ja	.L6032
	flds	52(%esp)
	fucomip	%st(1), %st
	jbe	.L6013
	fstp	%st(0)
	jmp	.L5943
.L6032:
	fstp	%st(0)
	.align 16
.L5943:
	flds	48(%esp)
	fsqrt
	fldz
	flds	48(%esp)
	fxch	%st(1)
	fucomip	%st(1), %st
	fstp	%st(0)
	jbe	.L6016
	fstps	8(%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	60(%esp)
	.cfi_def_cfa_offset 64
	call	sqrtf
	fstp	%st(0)
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	flds	8(%esp)
	jmp	.L6016
.L6028:
	fstp	%st(0)
	jmp	.L5968
.L6029:
	fstp	%st(0)
.L5968:
	flds	48(%esp)
	fadds	52(%esp)
	addl	$36, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L6023:
	.cfi_restore_state
	fmull	.LC653
	fstps	28(%esp)
	flds	28(%esp)
	jmp	.L6016
	.align 16
.L6030:
	fstp	%st(0)
	jmp	.L5918
	.align 16
.L6031:
	fstp	%st(0)
.L5918:
	fldz
	flds	52(%esp)
	fucomip	%st(1), %st
	ja	.L6016
	fstp	%st(0)
	flds	48(%esp)
	fabs
	fdivrs	.LC543
	jmp	.L6016
	.align 16
.L6022:
	flds	8(%esp)
	fsubs	.LC543
	fstps	8(%esp)
	movl	8(%esp), %edx
	movl	%edx, %eax
	andl	$-2139095041, %edx
	shrl	$23, %eax
	orl	$1065353216, %edx
	movzbl	%al, %eax
	movl	%edx, 8(%esp)
	subl	$253, %eax
	jmp	.L5934
	.align 16
.L6006:
	fstp	%st(0)
	flds	.LC647
	flds	48(%esp)
	fucomip	%st(1), %st
	fstp	%st(0)
	jb	.L6007
	testl	%esi, %esi
	jne	.L5929
.L5930:
	flds	.LC665
	flds	52(%esp)
	fucomi	%st(1), %st
	fstp	%st(1)
	jbe	.L6014
	flds	.LC666
	fucomip	%st(1), %st
	ja	.L6026
	fstp	%st(0)
	flds	48(%esp)
	movl	$2147483520, %eax
.L6018:
	fld1
	jmp	.L5953
	.align 16
.L6034:
	fxch	%st(1)
.L5953:
	sarl	%eax
	je	.L6033
	fxch	%st(1)
.L6027:
	testb	$1, %al
	fmul	%st(0), %st
	je	.L6034
	sarl	%eax
	fmul	%st, %st(1)
	jne	.L6027
	fstp	%st(0)
	jmp	.L5954
.L6033:
	fstp	%st(1)
.L5954:
	fstps	28(%esp)
	flds	28(%esp)
	jmp	.L6016
	.align 16
.L6012:
	fstp	%st(0)
	fld1
	jmp	.L5941
	.align 16
.L6007:
	fldz
	testl	%esi, %esi
	flds	52(%esp)
	jne	.L5931
	testl	%edx, %edx
	je	.L5931
	fucomip	%st(1), %st
	fstp	%st(0)
	fldz
	fchs
	flds	48(%esp)
	fcmovbe	%st(1), %st
	fstp	%st(1)
	jmp	.L6016
	.align 16
.L6024:
	fmull	.LC655
	fstps	28(%esp)
	flds	28(%esp)
	jmp	.L6016
	.align 16
.L6025:
	fld1
	fchs
	flds	48(%esp)
	fucomip	%st(1), %st
	fstp	%st(0)
	jp	.L6035
	je	.L6016
	fstp	%st(0)
	jmp	.L5970
.L6035:
	fstp	%st(0)
	.align 16
.L5970:
	cmpl	$1065353216, %eax
	jbe	.L5922
	fldz
	flds	52(%esp)
	fucomi	%st(1), %st
	fcmovbe	%st(1), %st
	fstp	%st(1)
	jmp	.L6016
	.align 16
.L6021:
	fdivs	48(%esp)
	jmp	.L6016
	.align 16
.L5931:
	fucomip	%st(1), %st
	jbe	.L6009
	fstp	%st(0)
	flds	48(%esp)
	fchs
	jmp	.L6016
	.align 16
.L5922:
	fldz
	flds	52(%esp)
	fucomi	%st(1), %st
	ja	.L6036
	fstp	%st(1)
	fabs
	jmp	.L6016
.L6013:
	fld1
	fdivs	48(%esp)
	fld	%st(0)
	fsqrt
	fxch	%st(2)
	fucomip	%st(1), %st
	jbe	.L6037
	fxch	%st(1)
	fstpl	8(%esp)
	subl	$16, %esp
	.cfi_def_cfa_offset 64
	fstpl	(%esp)
	call	sqrt
	fstp	%st(0)
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	fldl	8(%esp)
	jmp	.L5946
.L6037:
	fstp	%st(0)
.L5946:
	fstps	28(%esp)
	flds	28(%esp)
	jmp	.L6016
.L6014:
	fstp	%st(0)
	flds	48(%esp)
	movl	$-2147483520, %eax
.L5956:
	fdivrs	.LC543
	negl	%eax
.L5951:
	testb	$1, %al
	je	.L6018
	fld	%st(0)
	jmp	.L5953
.L6009:
	jmp	.L6016
.L6026:
	fnstcw	26(%esp)
	movzwl	26(%esp), %eax
	orb	$12, %ah
	movw	%ax, 24(%esp)
	fldcw	24(%esp)
	fistpl	20(%esp)
	fldcw	26(%esp)
	movl	20(%esp), %eax
	testl	%eax, %eax
	flds	48(%esp)
	jns	.L5951
	jmp	.L5956
	.cfi_endproc
.LFE552:
	.size	powf.part.13, .-powf.part.13
	.align 16
	.type	shiftData.part.14, @function
shiftData.part.14:
.LFB553:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	leal	(%eax,%edx), %ebx
	subl	$4, %esp
	.cfi_def_cfa_offset 24
	testl	%ecx, %ecx
	je	.L6039
	leal	(%eax,%ecx), %ebp
	movl	%eax, %esi
	movl	%edx, (%esp)
	.align 16
.L6040:
	movzbl	(%esi), %edx
	movl	%esi, %edi
	addl	$1, %esi
	subl	%eax, %edi
	cmpl	%ebp, %esi
	movb	%dl, (%edi,%ebx)
	jne	.L6040
	movl	(%esp), %edx
.L6039:
	addl	%ebx, %ecx
	movl	%ecx, %eax
	subl	%edx, %eax
	testl	%edx, %edx
	je	.L6050
	.align 16
.L6046:
	addl	$1, %ecx
	movb	$0, -1(%ecx)
	cmpl	%eax, %ecx
	jne	.L6046
.L6050:
	addl	$4, %esp
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE553:
	.size	shiftData.part.14, .-shiftData.part.14
	.align 16
	.type	ext2_read.part.17, @function
ext2_read.part.17:
.LFB556:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	testl	%ecx, %ecx
	je	.L6055
	movl	20(%eax), %ebx
	cmpl	%edx, %ebx
	jb	.L6055
	movl	%eax, %esi
	leal	(%ecx,%edx), %eax
	movl	%ebx, %ebp
	subl	%edx, %ebp
	cmpl	%eax, %ebx
	cmovb	%ebp, %ecx
	movl	48(%esp), %ebp
	xorl	%ebx, %ebx
	leal	-1(%ecx), %edi
	movl	%ecx, 12(%esp)
	shrl	$10, %edi
	addl	$1, %edi
	jmp	.L6057
	.align 16
.L6064:
	addl	$1, %ebx
	addl	$1024, %ebp
	cmpl	%ebx, %edi
	je	.L6052
.L6057:
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	pushl	%ebp
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	ext2_block_of_set
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	jne	.L6064
.L6055:
	movl	$0, 12(%esp)
.L6052:
	movl	12(%esp), %eax
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE556:
	.size	ext2_read.part.17, .-ext2_read.part.17
	.align 16
	.type	ext2_dirent_from_dir.part.18, @function
ext2_dirent_from_dir.part.18:
.LFB557:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	%eax, %esi
	subl	$40, %esp
	.cfi_def_cfa_offset 60
	movl	%edx, 20(%esp)
	movl	%eax, 24(%esp)
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	addl	$12, %esp
	.cfi_def_cfa_offset 52
	movl	%eax, %edi
	pushl	%eax
	.cfi_def_cfa_offset 56
	pushl	$0
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	ext2_block_of_set
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%edi, %edi
	je	.L6066
	movl	8(%esp), %edx
	testl	%edx, %edx
	je	.L6075
	xorl	%ebp, %ebp
	xorl	%ebx, %ebx
	xorl	%esi, %esi
	jmp	.L6069
	.align 16
.L6086:
	movzwl	%ax, %eax
	addl	$1, %esi
	addl	%eax, %ebx
	cmpl	%esi, %edx
	leal	4(%ebx), %eax
	je	.L6067
.L6069:
	movzwl	4(%edi,%ebx), %eax
	testw	%ax, %ax
	jne	.L6086
	movl	%edx, 8(%esp)
	addl	$1, %ebp
	subl	$4, %esp
	.cfi_def_cfa_offset 52
	pushl	%edi
	.cfi_def_cfa_offset 56
	pushl	%ebp
	.cfi_def_cfa_offset 60
	xorl	%ebx, %ebx
	pushl	24(%esp)
	.cfi_def_cfa_offset 64
	call	ext2_block_of_set
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	8(%esp), %edx
	movl	$4, %eax
	cmpl	%esi, %edx
	jne	.L6069
.L6067:
	movzwl	(%edi,%eax), %eax
	testw	%ax, %ax
	je	.L6066
	movl	(%edi,%ebx), %edx
	movw	%ax, dirent2.8032+4
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	%edx, dirent2.8032
	movzbl	6(%edi,%ebx), %eax
	movb	%al, dirent2.8032+6
	movzbl	7(%edi,%ebx), %edx
	movzbl	%al, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	movb	%dl, dirent2.8032+7
	call	*kmalloc
	movzbl	dirent2.8032+6, %edx
	movl	%eax, dirent2.8032+8
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	leal	1(%eax,%edx), %edx
	.align 16
.L6070:
	addl	$1, %eax
	movb	$0, -1(%eax)
	cmpl	%eax, %edx
	jne	.L6070
	movzbl	dirent2.8032+6, %ecx
	leal	8(%edi,%ebx), %esi
	movl	dirent2.8032+8, %ebx
	xorl	%eax, %eax
	addl	$1, %ecx
	.align 16
.L6071:
	movzbl	(%esi,%eax), %edx
	movb	%dl, (%ebx,%eax)
	addl	$1, %eax
	cmpl	%eax, %ecx
	jne	.L6071
	movzbl	dirent2.8032+6, %eax
	movl	dirent2.8032+8, %edx
	movb	$0, (%edx,%eax)
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$dirent2.8032, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L6066:
	.cfi_restore_state
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
.L6075:
	.cfi_restore_state
	xorl	%ebx, %ebx
	movl	$4, %eax
	jmp	.L6067
	.cfi_endproc
.LFE557:
	.size	ext2_dirent_from_dir.part.18, .-ext2_dirent_from_dir.part.18
	.align 16
	.type	ext2_write_block_of_set.part.19, @function
ext2_write_block_of_set.part.19:
.LFB558:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	movl	%ecx, %ebp
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	%eax, %ebx
	movl	$1024, %esi
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	%esi, %edi
	movl	48(%esp), %eax
	cmpl	$1024, %eax
	cmovbe	%eax, %edi
	cmpl	$11, %edx
	ja	.L6088
	leal	(%ebx,%edx,4), %ebx
	movl	52(%ebx), %edx
	testl	%edx, %edx
	jne	.L6089
.L6093:
	xorl	%ebx, %ebx
.L6087:
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L6088:
	.cfi_restore_state
	leal	-12(%edx), %esi
	cmpl	$255, %esi
	jbe	.L6163
	leal	-268(%edx), %eax
	cmpl	$65535, %eax
	jbe	.L6164
	subl	$65804, %edx
	cmpl	$16777215, %edx
	ja	.L6093
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	$1024, (%esp)
	movl	%eax, 20(%esp)
	call	*kmalloc
	movl	$1024, (%esp)
	movl	%eax, 24(%esp)
	call	*kmalloc
	movl	%eax, 28(%esp)
	movl	108(%ebx), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	je	.L6093
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	36(%esp)
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L6165
.L6098:
	movl	4(%esp), %ecx
	movl	%esi, %eax
	shrl	$16, %eax
	movl	(%ecx,%eax,4), %eax
	testl	%eax, %eax
	je	.L6093
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	40(%esp)
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L6166
.L6099:
	movl	8(%esp), %ecx
	movl	%esi, %eax
	movzbl	%ah, %eax
	movl	(%ecx,%eax,4), %eax
	testl	%eax, %eax
	je	.L6093
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	44(%esp)
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L6167
.L6100:
	movl	%esi, %eax
	movzbl	%al, %esi
	movl	12(%esp), %eax
	movl	(%eax,%esi,4), %ebx
	testl	%ebx, %ebx
	je	.L6093
	jmp	.L6161
	.align 16
.L6089:
	movl	%edi, %eax
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	shrl	$9, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	sall	$9, %eax
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%edx
	.cfi_def_cfa_offset 72
	pushl	%eax
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ecx
	.cfi_def_cfa_offset 84
	pushl	$1
	.cfi_def_cfa_offset 88
	pushl	$48
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L6091
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC215
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
.L6091:
	movl	52(%ebx), %ebx
	addl	$28, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L6163:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	%eax, %edx
	movl	100(%ebx), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	je	.L6093
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	curr_port, %ebx
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edx
	.cfi_def_cfa_offset 84
	movl	%edx, 40(%esp)
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	movl	4(%esp), %edx
	testl	$1073741824, %eax
	jne	.L6168
.L6094:
	movl	(%edx,%esi,4), %ebx
	testl	%ebx, %ebx
	je	.L6093
	jmp	.L6161
	.align 16
.L6164:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$1024
	.cfi_def_cfa_offset 64
	call	*kmalloc
	movl	$1024, (%esp)
	movl	%eax, 20(%esp)
	call	*kmalloc
	movl	%eax, %edx
	movl	104(%ebx), %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	je	.L6093
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	curr_port, %ebx
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%edx
	.cfi_def_cfa_offset 84
	movl	%edx, 44(%esp)
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	movl	8(%esp), %edx
	testl	$1073741824, %eax
	jne	.L6169
.L6096:
	movl	%esi, %eax
	shrl	$8, %eax
	movl	(%edx,%eax,4), %eax
	testl	%eax, %eax
	je	.L6093
	movl	curr_port, %ebx
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$3
	.cfi_def_cfa_offset 64
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%eax
	.cfi_def_cfa_offset 72
	pushl	$1536
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	36(%esp)
	.cfi_def_cfa_offset 84
	pushl	$0
	.cfi_def_cfa_offset 88
	pushl	$32
	.cfi_def_cfa_offset 92
	pushl	%ebx
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%ebx), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	jne	.L6170
.L6097:
	movl	%esi, %eax
	movzbl	%al, %esi
	movl	4(%esp), %eax
	movl	(%eax,%esi,4), %ebx
	testl	%ebx, %ebx
	je	.L6093
.L6161:
	movl	%edi, %eax
	movl	curr_port, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	shrl	$9, %eax
	addl	$1, %eax
	pushl	%eax
	.cfi_def_cfa_offset 64
	sall	$9, %eax
	pushl	$0
	.cfi_def_cfa_offset 68
	pushl	%ebx
	.cfi_def_cfa_offset 72
	pushl	%eax
	.cfi_def_cfa_offset 76
	pushl	$1
	.cfi_def_cfa_offset 80
	pushl	%ebp
	.cfi_def_cfa_offset 84
	pushl	$1
	.cfi_def_cfa_offset 88
	pushl	$48
	.cfi_def_cfa_offset 92
	pushl	%esi
	.cfi_def_cfa_offset 96
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 48
	testl	$1073741824, %eax
	je	.L6087
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC215
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L6087
	.align 16
.L6168:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	4(%esp), %edx
	jmp	.L6094
	.align 16
.L6169:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	movl	8(%esp), %edx
	jmp	.L6096
	.align 16
.L6165:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L6098
.L6170:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L6097
.L6166:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L6099
.L6167:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	pushl	$.LC214
	.cfi_def_cfa_offset 64
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	jmp	.L6100
	.cfi_endproc
.LFE558:
	.size	ext2_write_block_of_set.part.19, .-ext2_write_block_of_set.part.19
	.align 16
	.type	ext2_write.part.20, @function
ext2_write.part.20:
.LFB559:
	.cfi_startproc
	testl	%ecx, %ecx
	je	.L6188
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	movl	%eax, %ebp
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	subl	$28, %esp
	.cfi_def_cfa_offset 48
	movl	20(%eax), %eax
	cmpl	%edx, %eax
	jb	.L6174
	movl	%ecx, %edi
	leal	(%ecx,%edx), %ecx
	movl	%eax, %esi
	subl	%edx, %esi
	cmpl	%ecx, %eax
	cmovb	%esi, %edi
	movl	48(%esp), %esi
	xorl	%ebx, %ebx
	leal	-1(%edi), %eax
	shrl	$10, %eax
	addl	$1, %eax
	movl	%eax, 12(%esp)
	jmp	.L6176
	.align 16
.L6190:
	subl	$12, %esp
	.cfi_def_cfa_offset 60
	movl	%esi, %ecx
	movl	%ebx, %edx
	pushl	%edi
	.cfi_def_cfa_offset 64
	movl	%ebp, %eax
	call	ext2_write_block_of_set.part.19
	addl	$16, %esp
	.cfi_def_cfa_offset 48
	testl	%eax, %eax
	je	.L6174
	addl	$1, %ebx
	addl	$1024, %esi
	cmpl	%ebx, 12(%esp)
	je	.L6189
.L6176:
	testl	%edi, %edi
	jne	.L6190
.L6174:
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L6188:
	movl	$1, %eax
	ret
	.align 16
.L6189:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -20
	.cfi_offset 5, -8
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	addl	$28, %esp
	.cfi_def_cfa_offset 20
	xorl	%eax, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE559:
	.size	ext2_write.part.20, .-ext2_write.part.20
	.align 16
	.type	ext2_expand.part.21, @function
ext2_expand.part.21:
.LFB560:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_def_cfa_offset 12
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_def_cfa_offset 20
	.cfi_offset 3, -20
	movl	%eax, %esi
	movl	%edx, %ebx
	subl	$44, %esp
	.cfi_def_cfa_offset 64
	movl	ext2_g_sblock, %eax
	testl	%eax, %eax
	je	.L6192
	movl	ext2_g_gdesc, %edx
	testl	%edx, %edx
	je	.L6192
	movl	%eax, 24(%esp)
	movl	%edx, 28(%esp)
.L6194:
	movl	20(%esi), %eax
	testl	%eax, %eax
	je	.L6196
	leal	-1(%eax), %ebp
	addl	%ebx, %eax
	leal	-1(%eax), %edx
	shrl	$10, %ebp
	shrl	$10, %edx
	addl	$1, %ebp
	addl	$1, %edx
	cmpl	%edx, %ebp
	jne	.L6197
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	movl	%eax, 20(%esi)
	pushl	%esi
	.cfi_def_cfa_offset 80
.L6213:
	call	ext2_update_node_in_inode_table
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	setne	%al
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	movzbl	%al, %eax
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L6196:
	.cfi_restore_state
	leal	-1(%ebx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	shrl	$10, %eax
	addl	$1, %eax
	leal	0(,%eax,4), %edi
	movl	%eax, 16(%esp)
	movl	%eax, %ebp
	pushl	%edi
	.cfi_def_cfa_offset 80
	call	*kmalloc
	movl	%esi, (%esp)
	movl	%eax, 16(%esp)
	call	ext2_block_locs
	popl	%eax
	.cfi_def_cfa_offset 76
	popl	%edx
	.cfi_def_cfa_offset 72
	pushl	%ebp
	.cfi_def_cfa_offset 76
	pushl	40(%esp)
	.cfi_def_cfa_offset 80
	xorl	%ebp, %ebp
	call	ext2_format_block_bitmap
	addl	$16, %esp
	.cfi_def_cfa_offset 64
.L6199:
	movl	(%esp), %ecx
	addl	%eax, %edi
	leal	(%ecx,%ebp), %edx
	.align 16
.L6201:
	addl	$1, %eax
	movzbl	-1(%eax), %ecx
	addl	$1, %edx
	cmpl	%edi, %eax
	movb	%cl, -1(%edx)
	jne	.L6201
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	pushl	$12
	.cfi_def_cfa_offset 80
	call	*kmalloc
	movl	100(%esi), %edx
	movl	%edx, (%eax)
	movl	104(%esi), %edx
	movl	%edx, 4(%eax)
	movl	108(%esi), %edx
	movl	%edx, 8(%eax)
	popl	%ecx
	.cfi_def_cfa_offset 76
	popl	%edi
	.cfi_def_cfa_offset 72
	pushl	$3
	.cfi_def_cfa_offset 76
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	ext2_free_blocks
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	je	.L6214
.L6202:
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 20
	movl	$1, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 12
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 8
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L6214:
	.cfi_restore_state
	pushl	4(%esp)
	.cfi_remember_state
	.cfi_def_cfa_offset 68
	pushl	4(%esp)
	.cfi_def_cfa_offset 72
	pushl	36(%esp)
	.cfi_def_cfa_offset 76
	pushl	%esi
	.cfi_def_cfa_offset 80
	call	ext2_inode_entry_blocks
	addl	%ebx, 20(%esi)
	movl	%esi, (%esp)
	jmp	.L6213
	.align 16
.L6192:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 72
	leal	36(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 76
	leal	36(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%eax, %eax
	je	.L6194
	jmp	.L6202
	.align 16
.L6197:
	leal	-1(%ebx), %edi
	subl	$12, %esp
	.cfi_def_cfa_offset 76
	shrl	$10, %edi
	leal	1(%edi), %eax
	movl	%eax, 20(%esp)
	movl	%eax, %edi
	addl	%ebp, %eax
	movl	%eax, 16(%esp)
	sall	$2, %eax
	sall	$2, %ebp
	pushl	%eax
	.cfi_def_cfa_offset 80
	call	*kmalloc
	movl	%esi, (%esp)
	movl	%eax, 16(%esp)
	call	ext2_block_locs
	movl	%eax, 28(%esp)
	popl	%eax
	.cfi_def_cfa_offset 76
	popl	%edx
	.cfi_def_cfa_offset 72
	pushl	%edi
	.cfi_def_cfa_offset 76
	pushl	40(%esp)
	.cfi_def_cfa_offset 80
	call	ext2_format_block_bitmap
	addl	$16, %esp
	.cfi_def_cfa_offset 64
	testl	%ebp, %ebp
	movl	12(%esp), %edx
	je	.L6212
	movl	(%esp), %ecx
	leal	(%edx,%ebp), %edi
	movl	%eax, 12(%esp)
	.align 16
.L6200:
	addl	$1, %edx
	movzbl	-1(%edx), %eax
	addl	$1, %ecx
	cmpl	%edi, %edx
	movb	%al, -1(%ecx)
	jne	.L6200
	movl	12(%esp), %eax
.L6212:
	movl	8(%esp), %edi
	sall	$2, %edi
	jmp	.L6199
	.cfi_endproc
.LFE560:
	.size	ext2_expand.part.21, .-ext2_expand.part.21
	.align 16
	.type	ext2_get_inode_table.part.22, @function
ext2_get_inode_table.part.22:
.LFB561:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	movl	%eax, %edi
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	20(%eax), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	sall	$10, %eax
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	*kmalloc
	movl	%eax, %ebx
	movl	20(%edi), %eax
	movl	curr_port, %esi
	sall	$10, %eax
	movl	%eax, %edx
	shrl	$13, %eax
	shrl	$9, %edx
	movzwl	%ax, %eax
	addl	$1, %edx
	addl	$1, %eax
	movl	%edx, (%esp)
	sall	$9, %edx
	pushl	$0
	.cfi_def_cfa_offset 36
	pushl	8(%edi)
	.cfi_def_cfa_offset 40
	pushl	%edx
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	pushl	%ebx
	.cfi_def_cfa_offset 52
	pushl	$0
	.cfi_def_cfa_offset 56
	pushl	$32
	.cfi_def_cfa_offset 60
	pushl	%esi
	.cfi_def_cfa_offset 64
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 16
	testl	$1073741824, %eax
	jne	.L6221
	movl	%ebx, %eax
	movl	%ebx, ext2_g_inode_table
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L6221:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	$.LC214
	.cfi_def_cfa_offset 32
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	%ebx, %eax
	movl	%ebx, ext2_g_inode_table
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE561:
	.size	ext2_get_inode_table.part.22, .-ext2_get_inode_table.part.22
	.section	.rodata
	.align 4
.LC667:
	.string	"The disk does not have preexisting data on it\n"
	.align 4
.LC668:
	.string	"\n\tFormating required ext2 structures, this may take some time\n"
	.align 4
.LC669:
	.string	"\tCreating the superblock and group descriptor table..."
.LC670:
	.string	"%cgdone%cw\n"
	.align 4
.LC671:
	.string	"\tCreating the root directory..."
	.align 4
.LC672:
	.string	"\tCreating root \".\" hardlink..."
	.align 4
.LC673:
	.string	"\tCreating root \"..\" hardlink..."
	.align 4
.LC674:
	.string	"The floppy disk has preexisting data on it!\n"
	.align 4
.LC675:
	.string	"\n creator_os: %x \n Block_size: %x\n"
	.align 4
.LC676:
	.string	"\tAcquiring preexisting root directory..."
	.text
	.align 16
	.type	ext2_initialize.part.23, @function
ext2_initialize.part.23:
.LFB562:
	.cfi_startproc
	pushl	%esi
	.cfi_def_cfa_offset 8
	.cfi_offset 6, -8
	pushl	%ebx
	.cfi_def_cfa_offset 12
	.cfi_offset 3, -12
	movl	%eax, %esi
	subl	$28, %esp
	.cfi_def_cfa_offset 40
	leal	12(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 44
	leal	12(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L6223
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC667
	.cfi_def_cfa_offset 48
	call	printf
	movl	$.LC668, (%esp)
	call	printf
	movl	$.LC669, (%esp)
	call	printf
	movl	%esi, (%esp)
	call	ext2_set_block_group
	movl	$.LC670, (%esp)
	call	printf
	movl	$.LC671, (%esp)
	call	printf
	movl	ext2_g_sblock, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L6224
	movl	ext2_g_gdesc, %edx
	testl	%edx, %edx
	je	.L6224
	movl	%eax, 8(%esp)
	movl	%edx, 12(%esp)
.L6227:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	movl	$5, %esi
	pushl	$140
	.cfi_def_cfa_offset 48
	call	*kmalloc
	movl	%eax, %ebx
	movb	$83, (%eax)
	movl	$0, 4(%eax)
	movl	$493, %eax
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	movw	%ax, 8(%ebx)
	xorl	%eax, %eax
	movw	%dx, 44(%ebx)
	movw	%ax, 16(%ebx)
	xorl	%eax, %eax
	leal	112(%ebx), %edx
	movw	%ax, 40(%ebx)
	movl	$2, %eax
	movl	$2, 12(%ebx)
	movw	%ax, 42(%ebx)
	leal	52(%ebx), %eax
	movl	$0, 20(%ebx)
	movl	$65535, 24(%ebx)
	movl	$65535, 28(%ebx)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	$65535, 32(%ebx)
	movl	$0, 36(%ebx)
	movw	%cx, 46(%ebx)
	movw	%si, 48(%ebx)
	.align 16
.L6228:
	movl	$0, (%eax)
	addl	$4, %eax
	cmpl	%edx, %eax
	jne	.L6228
	xorl	%eax, %eax
	subl	$4, %esp
	.cfi_def_cfa_offset 36
	xorl	%ecx, %ecx
	movw	%ax, 116(%ebx)
	xorl	%esi, %esi
	xorl	%eax, %eax
	movw	%cx, 112(%ebx)
	movw	%si, 114(%ebx)
	movw	%ax, 118(%ebx)
	pushl	12(%esp)
	.cfi_def_cfa_offset 40
	pushl	20(%esp)
	.cfi_def_cfa_offset 44
	pushl	%ebx
	.cfi_def_cfa_offset 48
	call	ext2_data_to_inode_table
	movl	28(%esp), %eax
	movl	curr_port, %esi
	subw	$1, 14(%eax)
	movl	$1, (%esp)
	pushl	$0
	.cfi_def_cfa_offset 52
	pushl	24(%eax)
	.cfi_def_cfa_offset 56
	pushl	$512
	.cfi_def_cfa_offset 60
	pushl	$1
	.cfi_def_cfa_offset 64
	pushl	%eax
	.cfi_def_cfa_offset 68
	pushl	$1
	.cfi_def_cfa_offset 72
	pushl	$48
	.cfi_def_cfa_offset 76
	pushl	%esi
	.cfi_def_cfa_offset 80
	call	SATA_Commander
	movl	16(%esi), %eax
	addl	$48, %esp
	.cfi_def_cfa_offset 32
	testl	$1073741824, %eax
	je	.L6226
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC215
	.cfi_def_cfa_offset 48
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
.L6226:
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	$.LC372
	.cfi_def_cfa_offset 44
	pushl	%ebx
	.cfi_def_cfa_offset 48
	call	ext2_create_dir
	movl	%eax, %ebx
	movl	$.LC670, (%esp)
	call	printf
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%ebx, %ebx
	movl	%ebx, ext2_root
	je	.L6231
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$2
	.cfi_def_cfa_offset 48
	call	*kmalloc
	movl	%eax, ext2_path
	movb	$47, (%eax)
	movl	ext2_path, %eax
	movb	$0, 1(%eax)
	movl	$.LC672, (%esp)
	call	printf
	pushl	$.LC572
	.cfi_def_cfa_offset 52
	pushl	$8
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	ext2_add_file_to_dir
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L6244
.L6231:
	movl	$1, %ebx
	addl	$20, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L6223:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	movl	%eax, %ebx
	pushl	$.LC674
	.cfi_def_cfa_offset 48
	call	printf
	movl	16(%esp), %eax
	addl	$12, %esp
	.cfi_def_cfa_offset 36
	pushl	24(%eax)
	.cfi_def_cfa_offset 40
	pushl	72(%eax)
	.cfi_def_cfa_offset 44
	pushl	$.LC675
	.cfi_def_cfa_offset 48
	call	printf
	movl	ext2_g_inode_table, %edx
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%edx, %edx
	je	.L6245
.L6232:
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC676
	.cfi_def_cfa_offset 48
	call	printf
	movl	$1, (%esp)
	call	ext2_inode_from_offset
	movl	$.LC670, (%esp)
	movl	%eax, ext2_root
	call	printf
	movl	ext2_root, %eax
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L6231
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$2
	.cfi_def_cfa_offset 48
.L6242:
	call	*kmalloc
	movl	%eax, ext2_root_name
	movb	$47, (%eax)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	ext2_root_name, %eax
	movb	$0, 1(%eax)
	addl	$20, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 12
	movl	%ebx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 4
	ret
	.align 16
.L6244:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 44
	pushl	$.LC670
	.cfi_def_cfa_offset 48
	call	printf
	movl	$.LC673, (%esp)
	call	printf
	pushl	$.LC433
	.cfi_def_cfa_offset 52
	pushl	$8
	.cfi_def_cfa_offset 56
	pushl	%ebx
	.cfi_def_cfa_offset 60
	pushl	%ebx
	.cfi_def_cfa_offset 64
	call	ext2_add_file_to_dir
	addl	$32, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	movl	%eax, %ebx
	jne	.L6231
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	$.LC670
	.cfi_def_cfa_offset 48
	call	printf
	movl	$2, (%esp)
	jmp	.L6242
	.align 16
.L6224:
	.cfi_restore_state
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 44
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	ext2_read_meta_data
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L6227
	xorl	%ebx, %ebx
	jmp	.L6226
.L6245:
	movl	4(%esp), %eax
	call	ext2_get_inode_table.part.22
	movl	%eax, ext2_g_inode_table
	jmp	.L6232
	.cfi_endproc
.LFE562:
	.size	ext2_initialize.part.23, .-ext2_initialize.part.23
	.align 16
	.type	_putchar.constprop.24, @function
_putchar.constprop.24:
.LFB583:
	.cfi_startproc
	movl	consolerow, %eax
	movl	$0, consolecolumn
	addl	$1, %eax
	cmpl	$24, %eax
	movl	%eax, consolerow
	jbe	.L6247
	movl	console_rows_skipped, %eax
	addl	$1, %eax
	movl	%eax, console_rows_skipped
	movl	console_skip, %eax
	addl	$1, %eax
	movl	%eax, console_skip
.L6247:
	movl	$10, %eax
	ret
	.cfi_endproc
.LFE583:
	.size	_putchar.constprop.24, .-_putchar.constprop.24
	.section	.rodata
	.align 4
.LC1:
	.long	1259902592
	.align 4
.LC217:
	.long	1144537088
	.align 4
.LC218:
	.long	1197482240
	.align 4
.LC543:
	.long	1065353216
	.align 8
.LC544:
	.long	-1717986918
	.long	1073322393
	.align 8
.LC545:
	.long	1717986918
	.long	1072850534
	.align 8
.LC546:
	.long	171798692
	.long	1072735191
	.align 8
.LC547:
	.long	-858993459
	.long	1072483532
	.align 4
.LC548:
	.long	925353388
	.align 8
.LC549:
	.long	354811962
	.long	1071369083
	.align 4
.LC575:
	.long	1086324736
	.align 8
.LC577:
	.long	1413754602
	.long	1074340347
	.align 8
.LC579:
	.long	1293080650
	.long	1074340347
	.align 4
.LC580:
	.long	1120403456
	.align 4
.LC582:
	.long	1077936128
	.align 4
.LC583:
	.long	1056964608
	.align 4
.LC646:
	.long	2139095039
	.align 4
.LC647:
	.long	-8388609
	.align 8
.LC648:
	.long	1431655765
	.long	1070945621
	.align 8
.LC649:
	.long	-1717986918
	.long	1070176665
	.align 4
.LC650:
	.long	1048576000
	.align 8
.LC651:
	.long	1697350398
	.long	1073157447
	.align 4
.LC652:
	.long	1124073472
	.align 8
.LC653:
	.long	0
	.long	1206910976
	.align 4
.LC654:
	.long	-1021968384
	.align 8
.LC655:
	.long	0
	.long	1048576
	.align 8
.LC656:
	.long	-1121416287
	.long	1076303678
	.align 8
.LC657:
	.long	1961972739
	.long	1068264202
	.align 8
.LC658:
	.long	-146903665
	.long	1083117799
	.align 8
.LC659:
	.long	-49728530
	.long	1072049730
	.align 8
.LC660:
	.long	-1455927703
	.long	1079817750
	.align 8
.LC661:
	.long	-49345798
	.long	1070514109
	.align 8
.LC662:
	.long	60408694
	.long	1086063603
	.align 8
.LC663:
	.long	125482
	.long	1072693248
	.align 8
.LC664:
	.long	-600767046
	.long	1052147681
	.align 4
.LC665:
	.long	-822083585
	.align 4
.LC666:
	.long	1325400063
	.ident	"GCC: (GNU) 6.1.0"
