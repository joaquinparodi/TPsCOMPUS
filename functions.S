#include <sys/regdef.h>
#include <sys/syscall.h>
#include <asm/mman.h>

.abicalls
.text
.align 2
.globl fill_array
.ent fill_array

fill_array:
	subu	sp,sp,40
	sw	ra,32(sp)
	sw	fp,28(sp)
	move	fp,sp
	
	sw	a0,40(fp)	#:array	original
	sw	a1,44(fp)	#:array a llenar
	sw	a2,48(fp)	#:array size

variables_set:
	li	t0,0
	lw	t1,48(fp)	#size
	li	t2,0
	lw	s0,40(fp)	# array original
	lw	s1,44(fp)	# array a llenar

array_loop:
	bge	t0,t1,fill_array_return
	lw	t2,0(s0)	#t2: elemento de array original
	addiu	s0,s0,4		#Le sumo 4 a la direccion
	sw	t2,0(s1)
	addiu	s1,s1,4
	addiu	t0,t0,4	
	b array_loop

fill_array_return:		#El array se modifica por referencia.
	lw	ra,32(sp)
	lw	fp,28(fp)
	addu	sp,sp,40
	j	ra
	.end fill_array


.globl	merge_sub_arrays
.ent	merge_sub_arrays

#a0: array
#a1: left index 1
#a2: right index 1
#a3: left index 2
#a4: right index 2
#a5: max array size


merge_sub_arrays:
	subu	sp,sp,40
	sw	ra,32(sp)
	sw	fp,28(sp)
	move	fp,sp

	#Parametros
	sw	a0,40(fp)	#array
	sw	a1,44(fp)	#left index 1
	sw	a2,48(fp)	#right index 1 
	sw	a3,52(fp)	#left index 2
	sw	a4,56(fp)	#right index 2
	sw	a5,60(fp)	#max size

create_temporal_array:
	lw	a0,60(fp)	#a0: max size
	jal	mymalloc
	sw	v0,20(fp)	#temporal array

load_variables:
	lw	s0,40(fp)	#array original
	lw	s1,20(fp)	#array temporal
	lw	t0,44(fp)	#left index 1
	lw	t1,48(fp)	#right index 1 
	lw	t2,52(fp)	#left index 2
	lw	t3,56(fp)	#right index 2
	lw	t4,60(fp)	#max size

#iteradores
	lw	t5,44(fp

while_elements_in_both_arrays:	#while(l<= l1 && r<=r2)



return_merge_sub_arrays:
	lw	ra,32(sp) 
	lw	fp,28(sp)
	addiu	sp,sp,40
	j	ra
	.end merge_sub_arrays


.globl	merge_sort
.ent	merge_sort

merge_sort: 	#(a0:array[size], a1:left index, a2:right index)
	subu 	sp,sp,40
	sw	ra,32(sp)
	sw	fp,28(sp)
	move	fp,sp

	sw	a0,40(fp)	#array
	sw	a1,44(fp)	#left index
	sw	a2,48(fp)	#right index
	
	lw	t0,40(fp)
	lw	t1,44(fp)
	lw	t2,48(fp)


	li	t3,0	#medio
	bge	t1,t2,return_merge_sort		#if(l<r)
	addiu	t3,t1,t2	#l+r
	srl	t3,t3,1		#(l+r)/2
	sw	t3,52(fp)	#medio
	
left_recursion:
	lw	a0,40(fp)	#array
	lw	a1,44(fp)	#left
	lw	a2,52(fp)	#medio
	jal	merge_sort

right_recursion:
	lw	a0,40(fp)	#array}
	lw	a1,52(fp)	#medio
	addiu	a1,a1,1		#medio+1
	lw	a2,48(fp)	#right index

merging_sub_arrays:
	#HCER


	.end merge_sort
