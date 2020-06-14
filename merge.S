#include <sys/regdef.h>
#include <sys/syscall.h>
#include <asm/mman.h>

.abicalls
.text
.align 	2
.globl	merge
.ent 	merge

#define MERGE_STACK_SIZE 	40
#define MERGE_RA_OFFSET		32
#define MERGE_FP_OFFSET 	28
#define MERGE_GP_OFFSET 	24

merge:
	.frame		fp,MERGE_STACK_SIZE,ra
	subu		sp,sp,MERGE_STACK_SIZE
	.cprestore 	MERGE_GP_OFFSET
	sw		ra,MERGE_RA_OFFSET(sp)	
	sw		fp,MERGE_FP_OFFSET(sp)
	move		fp,sp

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
	li	v0,0		#si llegue acá es porque no hubo errores.
	lw	ra,MERGE_RA_OFFSET(sp)
	lw	fp,MERGE_FP_OFFSET(sp)
	addu	sp,sp,MERGE_STACK_SIZE
	
	jr	ra
	.end merge	
