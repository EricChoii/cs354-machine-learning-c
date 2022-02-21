	.file	"find_max.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"The length of the array is %d\n"
	.text
	.globl	Find_Max
	.type	Find_Max, @function
Find_Max:
// prologue - write code for the prologue here
	.cfi_startproc
	endbr64							# security
	pushq	%rbp					# store %rbp for caller
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp				# move %rsp to %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp				# reserve space on stack
	movl	%edi, -20(%rbp)			# store n
	movq	%rsi, -32(%rbp)			# store a*

// printf("The length of the array is %d\n", n);
// update this code if necessary to print the length of the array
// this block prints the value of register %edi
// initially the parameter n
	movl	%edi, %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

// reserve space for local variables
/* make a table here the variables from 
 * your c code and the addresses used in assembly 
 * n - address of n		%rdi
 * a - address of a		%rsi
 * i - address of i		-4(%rbp)
 * max_index -			-8(%rbp)
*/ 
	movl	$0, -8(%rbp)		# initiate max_index to 0
	movl	$1, -4(%rbp)		# initiate i to 0
	jmp	.CONDITIONAL			# check if condition
.TOP_OF_LOOP:
	movl	-4(%rbp), %eax		# store i to %eax
	cltq
	leaq	0(,%eax,4), %rdx	# store (i * 4) to %rdx
	movq	-32(%rbp), %rax		# get -32(%rbp)
	addq	%rdx, %rax			# get address of ith element of a*

// this code calls Print_One_Number to 
// print the number found in register %eax - use this code to debug 
// replace $999 with a register or memory address you wish to print
	movl    (%rax), %eax		# store ith element of a*
	movl	%eax, %edi
	call	Print_One_Number@PLT

// write your code to find the index of the maximum value here
	movl	-8(%rbp), %eax		# store max_index
	cltq
	leaq	0(,%rax,4), %rdx	# store (max_index * 4) to %rdx
	movq	-32(%rbp), %rax		# get -32(%rbp)
	addq	%rdx, %rax			# get address of a[max_index]
	movl	(%rax), %edx		# store a[max_index]
	movl	-4(%rbp), %eax		# store i to %eax
	cltq
	leaq	0(,%rax,4), %rcx	# store (i * 4) to %rdx
	movq	-32(%rbp), %rax		# get -32(%rbp)
	addq	%rcx, %rax			# get address of a[i]
	movl	(%rax), %eax		# store a[i]
	cmpl	%eax, %edx			# compare a[max_index] with a[i]
	jge	.BOTTOM_OF_LOOP			# TRUE_BLOCK - a[max_index] >= a[i], FALSE_BLCOK -a[max_index] < a[i]
	movl	-4(%rbp), %eax		# store i to %eax
	movl	%eax, -8(%rbp)		# update max_index with i
.BOTTOM_OF_LOOP:
	addl	$1, -4(%rbp)		# increment i by 1
.CONDITIONAL:	
	movl	-4(%rbp), %eax		# store i to %eax
	cmpl	-20(%rbp), %eax		# compare i with n
	jl	.TOP_OF_LOOP			# TRUE_BLOCK - i<n, FALSE_BLOCK - i>=n
// prepare the return value
// the template returns -1, you will need to update this
	movl	-8(%rbp), %eax		# return max_index
// epilogue - complete the epilogue below
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	Find_Max, .-Find_Max
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:



