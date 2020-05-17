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
	.cprestore 24
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
#stack: right index 2
#stack: max array size


merge_sub_arrays:
	.frame	fp,40,ra
	subu	sp,sp,40
	sw	ra,32(sp)	
	sw	fp,28(sp)
	.cprestore 24
	move	fp,sp

	#Parametros
	sw	a0,40(fp)	#array
	sw	a1,44(fp)	#left index 1
	sw	a2,48(fp)	#right index 1 
	sw	a3,52(fp)	#left index 2
	lw	t3,56(sp)	#right index 2, viene por stack en la 
#posicion 16 del mismo. Luego hago 40 del actual stack para llegar a
#al stack del caller. Esta en la 16 porque en las 16 anteriores estan
#los primeros 4 parametros.
	lw	t4,60(sp)	#max size: Hago lo mismo. 40+20
	
	sw	t3,56(fp)	#guardo right index
	sw	t4,60(fp)	#guardo max size


create_temporal_array:
	lw	a0,60(fp)	#a0: max size
	sll	a0,a0,2		#sizex4
	jal	mymalloc
	sw	v0,20(fp)	#temporal array


load_variables:
	lw	s0,40(fp)	#array original
	lw	s1,20(fp)	#array temporal
	lw	t0,44(fp)	#left index 1
	lw	t1,48(fp)	#right index 1 
	lw	t2,52(fp)	#left index 2
	lw	t3,56(fp)	#right index  2
	lw	t4,60(fp)	#max size

	sll	t0,t0,2
	sll	t1,t1,2		#multiplico por 4 los indices.
	sll	t2,t2,2	
	sll	t3,t3,2		#incluso el size
	sll	t4,t4,2		


#iteradores
	move	t5,t0		#i = l1
	move	t6,t2		#j = l2
	li	t7,0		#k = 0;
	li	t8,0		#auxiliar acceso a array original
	li	t9,0		#auxiliar acceso a array temporal

while_elements_in_both_arrays:	#while(l<= l1 && r<=r2)
	bgt	t5,t1,while_i_less_than_right_index_1	#condicion 1
	bgt	t6,t3,while_i_less_than_right_index_1	#condicion 2
	
	lw	s0,40(fp)
	addu	s0,s0,t5		#s0: dir de array[i]
	lw	t8,0(s0)		#t8: array[i]
	
	lw	s0,40(fp)		#s0: posicion inicial
	addu	s0,s0,t6		#s0: dir de array[j]
	lw	t9,0(s0)		#t9: array[j]
	lw	s0,40(fp)		#s0: posicion inicial
	bge	t8,t9,else_element_is_less_than 	#condicion
	
	if_element_is_less_than:		#if(a[i] < a[j])	
		lw	s1,20(fp)
		addu	s1,s1,t7		#s1: dir de temp[k]
		sw	t8,0(s1)		#temp[k] = array[i]
		addiu	t7,t7,4
		addiu	t5,t5,4
		lw	s1,20(fp)		#s1: posicion inicial
		b while_elements_in_both_arrays

	else_element_is_less_than:
		lw	s1,20(fp)
		addu	s1,s1,t7	#s1:dir temp[k]
		sw	t9,0(s1)	#temp[k] = array[j]
		addiu	t7,t7,4
		addiu	t6,t6,4
		lw	s1,20(fp)	
		b while_elements_in_both_arrays


while_i_less_than_right_index_1:	#while(i<=r1)
	bgt	t5,t1,while_j_less_than_right_index_2
	lw	s1,20(fp)	#cargo temporal
	addu	s1,s1,t7	#s1: dir de temp[k]
	
	lw	s0,40(fp)	#cargo array
	addu	s0,s0,t5	#dir de array[i]
	lw	t8,0(s0)	#t8: array[i]
	sw	t8,0(s1)	#temp[k] = array[i]
	
	#aumento iteradores
	addiu	t7,t7,4		#k++
	addiu	t5,t5,4		#i++
	b while_i_less_than_right_index_1

while_j_less_than_right_index_2:
	bgt	t6,t3,transfer_elements_from_temp_to_array
	lw	s1,20(fp)	#cargo temporal
	addu	s1,s1,t7	#s1: dir de temp[k]
	
	lw	s0,40(fp)	#s0: array
	addu	s0,s0,t6	#s0: dir de array[j]
	lw	t8,0(s0)	#t8: array[j]
	sw	t8,0(s1)	#temp[k] = array[j]

	#aumento iteradores
	addiu	t6,t6,4		#j++
	addiu	t7,t7,4		#k++		
	b while_j_less_than_right_index_2

transfer_elements_from_temp_to_array:
	move	t5,t0		#i=i1
	li	t6,0		#j=0
	while_i_less_or_equal_than_R2:
		bgt	t5,t3,return_merge_sub_arrays
		lw	s0,40(fp)	#cargo array original
		lw	s1,20(fp)	#cargo temporal
		addu	s0,s0,t5	#dir de array[i]
		addu	s1,s1,t6	#dir de temp[j]
		lw	t9,0(s1)	#temp[j]
		sw	t9,0(s0)	#array[i] = temp[j]
		addiu	t5,t5,4
		addiu	t6,t6,4
		b	while_i_less_or_equal_than_R2
	
return_merge_sub_arrays:
	lw	v0,20(fp)
	lw	ra,32(sp) 
	lw	fp,28(sp)
	addiu	sp,sp,40
	j	ra
.end merge_sub_arrays


.globl	merge_sort
.ent	merge_sort

merge_sort: 	#(a0:array[size], a1:left index, a2:right index)
	.frame	fp,40,ra
	subu 	sp,sp,40
	sw	ra,32(sp)
	sw	fp,28(sp)
	.cprestore 24
	move	fp,sp

	sw	a0,40(fp)	#array
	sw	a1,44(fp)	#left index
	sw	a2,48(fp)	#right index
	sw	a3,52(fp)	#max size
	
	lw	t0,40(fp)
	lw	t1,44(fp)
	lw	t2,48(fp)
	lw	t3,52(fp)	

	li	t4,0	#medio
	bge	t1,t2,return_merge_sort		#if(l<r)
	addu	t4,t1,t2	#l+r
	srl	t4,t4,1		#(l+r)/2
	sw	t4,56(fp)	#medio
	
left_recursion:
	lw	a0,40(fp)	#array
	lw	a1,44(fp)	#left
	lw	a2,56(fp)	#medio
	jal	merge_sort

right_recursion:
	lw	a0,40(fp)	#array
	lw	a1,56(fp)	#medio
	addiu	a1,a1,1		#medio+1
	lw	a2,48(fp)	#right index
	jal	merge_sort

merging_sub_arrays:
	lw	a0,40(fp)	#array
	lw	a1,44(fp)	#left index
	lw 	a2,56(fp)	#medio
	addiu	a3,a2,1		#medio +1

	lw	t0,48(fp)	#j por stack
	sw	t0,16(sp)	#Lo mando por stack
	lw	t1,52(fp)	#t1: max_size
	sw	t1,20(sp)	#lo mando por stack	
	
	jal 	merge_sub_arrays

return_merge_sort:
	lw	ra,32(sp)
	lw	fp,28(sp)
	addiu	sp,sp,40
	j	ra

	.end merge_sort
