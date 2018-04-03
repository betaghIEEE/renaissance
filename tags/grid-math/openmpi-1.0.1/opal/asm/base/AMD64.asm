START_FILE
	TEXT

START_FUNC(opal_atomic_mb)
	pushq	%rbp
	movq	%rsp, %rbp
	leave
	ret
END_FUNC(opal_atomic_mb)


START_FUNC(opal_atomic_rmb)
	pushq	%rbp
	movq	%rsp, %rbp
	leave
	ret
END_FUNC(opal_atomic_rmb)


START_FUNC(opal_atomic_wmb)
	pushq	%rbp
	movq	%rsp, %rbp
	leave
	ret
END_FUNC(opal_atomic_wmb)


START_FUNC(opal_atomic_cmpset_32)
	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movl	-16(%rbp), %ecx
	movq	-8(%rbp), %rdx
	movl	-12(%rbp), %eax
	cmpxchgl %ecx,(%rdx)
	movq	%rax, -24(%rbp)
	movl	-24(%rbp), %eax
	movl	%eax, -28(%rbp)
	movl	-28(%rbp), %eax
	cmpl	-12(%rbp), %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -28(%rbp)
	movl	-28(%rbp), %eax
	leave
	ret
END_FUNC(opal_atomic_cmpset_32)


START_FUNC(opal_atomic_cmpset_64)
	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-24(%rbp), %rcx
	movq	-8(%rbp), %rdx
	movq	-16(%rbp), %rax
	cmpxchgq %rcx,(%rdx)   
	
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	cmpq	-16(%rbp), %rax
	sete	%al
	movzbl	%al, %eax
	leave
	ret
END_FUNC(opal_atomic_cmpset_64)


START_FUNC(opal_sys_timer_get_cycles)
        rdtsc
        salq    $32, %rdx
        mov     %eax, %eax
        orq     %rdx, %rax
        ret
END_FUNC(opal_sys_timer_get_cycles)