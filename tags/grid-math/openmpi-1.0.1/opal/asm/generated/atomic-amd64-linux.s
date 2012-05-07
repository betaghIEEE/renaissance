	.text

	.globl opal_atomic_mb
	.type opal_atomic_mb, @function
opal_atomic_mb:
	pushq	%rbp
	movq	%rsp, %rbp
	leave
	ret
	.size opal_atomic_mb, .-opal_atomic_mb


	.globl opal_atomic_rmb
	.type opal_atomic_rmb, @function
opal_atomic_rmb:
	pushq	%rbp
	movq	%rsp, %rbp
	leave
	ret
	.size opal_atomic_rmb, .-opal_atomic_rmb


	.globl opal_atomic_wmb
	.type opal_atomic_wmb, @function
opal_atomic_wmb:
	pushq	%rbp
	movq	%rsp, %rbp
	leave
	ret
	.size opal_atomic_wmb, .-opal_atomic_wmb


	.globl opal_atomic_cmpset_32
	.type opal_atomic_cmpset_32, @function
opal_atomic_cmpset_32:
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
	.size opal_atomic_cmpset_32, .-opal_atomic_cmpset_32


	.globl opal_atomic_cmpset_64
	.type opal_atomic_cmpset_64, @function
opal_atomic_cmpset_64:
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
	.size opal_atomic_cmpset_64, .-opal_atomic_cmpset_64


	.globl opal_sys_timer_get_cycles
	.type opal_sys_timer_get_cycles, @function
opal_sys_timer_get_cycles:
        rdtsc
        salq    $32, %rdx
        mov     %eax, %eax
        orq     %rdx, %rax
        ret
	.size opal_sys_timer_get_cycles, .-opal_sys_timer_get_cycles
