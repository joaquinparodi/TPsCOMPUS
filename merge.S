#include <sys/regdef.h>
#include <sys/syscall.h>
#include <asm/mman.h>

.abicalls
.text
.align 	2
.globl	merge
.ent 	merge

merge:
	.frame	fp,40,ra
	subu	sp,sp,40
	.cprestore 24
	sw	ra,32(sp)	
	sw	fp,28(sp)
	move	fp,sp

	sw	a0,40(fp)	#direccion del array original
	sw	a1,44(fp)	#size del array original

call_merge:
	lw	a0,40(fp)	#a0: array dir
	li	a1,0		#a1: left_index =  0
	lw	a2,44(fp)	#a2: right_index
	lw	a3,44(fp)	#a3: max size
	addiu	a2,a2,-1	#a2: right index-1
	jal merge_sort

fin:
	lw	ra,32(sp)
	lw	fp,28(fp)
	addu	sp,sp,40
	
	jr	ra
	.end merge	
