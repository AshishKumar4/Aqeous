.section .text
.extern uint32_t old_esp
.extern uint32_t current_esp
.global saveState2
saveState2:
    cli
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esi
    push %edi
    push %ebp
    push %ds
    push %es
    push %fs
    push %gs

.global loadState2
loadState2:
    pop %gs
    pop %fs
    pop %es
    pop %ds
    pop %ebp
    pop %edi
    pop %esi
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    sti
    iret;

    .global switchTask
    switchTask:
        pusha
        pushf
        #mov %cr3, %eax ;#Push CR3
        #push %eax
        mov 44(%esp), %eax ;#The first argument, where to save
        mov %ebx, 4(%eax)
        mov %ecx, 8(%eax)
        mov %edx, 12(%eax)
        mov %esi, 16(%eax)
        mov %edi, 20(%eax)
        mov 36(%esp), %ebx ;#EAX
        mov 40(%esp), %ecx ;#IP
        mov 20(%esp), %edx ;#ESP
        add $4, %edx ;#Remove the return value ;)
        mov 16(%esp), %esi ;#EBP
        mov 4(%esp), %edi ;#EFLAGS
        mov %ebx, (%eax)
        mov %edx, 24(%eax)
        mov %esi, 28(%eax)
        mov %ecx, 32(%eax)
        mov %edi, 36(%eax)
        #pop %ebx ;#CR3
        mov %ebx, 40(%eax)
        push %ebx; #Goodbye again
        mov 48(%esp), %eax; #Now it is the new object
        mov 4(%eax), %ebx; #EBX
        mov 8(%eax), %ecx; #ECX
        mov 12(%eax), %edx; #EDX
        mov 16(%eax), %esi; #ESI
        mov 20(%eax), %edi; #EDI
        mov 28(%eax), %ebp; #EBP
        push %eax
        mov 36(%eax), %eax; #EFLAGS
        push %eax
        popf
        pop %eax
        mov 24(%eax), %esp; #ESP
        push %eax
        mov 44(%eax), %eax; #CR3
        #mov %eax, %cr3
        pop %eax
        push %eax
        mov 32(%eax), %eax ;#EIP
        xchg (%esp), %eax ;#We do not have any more registers to use as tmp storage
        mov (%eax), %eax ;#EAX
        ret ;#This ends all!
