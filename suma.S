#include <sys/regdef.h>
#include <sys/syscall.h>

.text
.abicalls
.align 	2

.globl 	mystrlen
.ent 	mystrlen
mystrlen:
	.frame 	fp,24,ra
	subu	sp,sp,24
	sw	fp,20(sp)
	move	fp,sp
	
	sw	a0,24(fp)
	sw	a1,28(fp)
	
suma:
	lw	v1,24(fp)
	lw	v0,28(fp)
	addu	v0,v1,v0
	sw	v0,24(fp)
return:
	lw	v0,24(fp)
	move	sp,fp
	lw	fp,20(sp)
	addiu	sp,sp,24
	jr	ra
	
	.end mystrlen







