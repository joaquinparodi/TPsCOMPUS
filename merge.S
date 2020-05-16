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
	sll	a1,a1,2		#sizex4
	sw	a1,48(fp)

ask_memory:
	lw	a0,44(fp)	#envio como parametro el array original
	jal	mymalloc
	sw	v0,20(fp)	#20(fp): direcci�n del array dinamico

#cargo en el array local
merge_1:
	lw	a0,40(fp)	#array source
	lw	a1,20(fp)	#array destino
	lw	a2,48(fp)	#sizex4
	jal	fill_array



#modifico el array original
merge_2:
	lw	a0,20(fp)	#source
	lw	a1,40(fp)	#destino
	lw	a2,48(fp)	#size
	jal 	fill_array

fin:
	lw	ra,32(sp)
	lw	fp,28(fp)
	addu	sp,sp,40
	
	jr	ra
	.end merge	
