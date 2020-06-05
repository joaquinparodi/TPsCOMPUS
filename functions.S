#include <sys/regdef.h>
#include <sys/syscall.h>
#include <asm/mman.h>

.abicalls
.text
.align 2
.globl	merge_sub_arrays
.ent	merge_sub_arrays

#a0: array
#a1: left index 1
#a2: right index 1
#a3: left index 2
#stack: right index 2
#stack: max array size

#define SUB_ARRAY_STACK_SIZE	48
#define SUB_ARRAY_RA_OFFSET	40
#define SUB_ARRAY_FP_OFFSET	36
#define SUB_ARRAY_GP_OFFSET	32
#define TEMPORAL_OFFSET		16
#define I_OFFSET		20
#define J_OFFSET		24
#define K_OFFSET		28

merge_sub_arrays:
	.frame		fp,SUB_ARRAY_STACK_SIZE,ra
	subu		sp,sp,SUB_ARRAY_STACK_SIZE
	sw		ra,SUB_ARRAY_RA_OFFSET(sp)	
	sw		fp,SUB_ARRAY_FP_OFFSET(sp)
	.cprestore 	SUB_ARRAY_GP_OFFSET
	move		fp,sp

	#Parametros
	sw	a0,48(fp)	#array
	sw	a1,52(fp)	#left index 1
	sw	a2,56(fp)	#right index 1 
	sw	a3,60(fp)	#left index 2
	lw	t3,64(sp)	#right index 2, viene por stack en la 
#posicion 16 del mismo. Luego hago 40 del actual stack para llegar a
#al stack del caller. Esta en la 16 porque en las 16 anteriores estan
#los primeros 4 parametros.
	lw	t4,68(sp)	#max size: Hago lo mismo. 40+20
	
	sw	t3,64(fp)	#guardo right index
	sw	t4,68(fp)	#guardo max size


create_temporal_array:
	lw	a0,68(fp)		#a0: max size
	sll	a0,a0,2			#sizex4
	jal	mymalloc
	sw	v0,TEMPORAL_OFFSET(fp)	#temporal array


load_variables:
	lw	a0,48(fp)		#array original
	lw	a1,TEMPORAL_OFFSET(fp)	#array temporal
	lw	t0,52(fp)		#left index 1
	lw	t1,56(fp)		#right index 1 
	lw	t2,60(fp)		#left index 2
	lw	t3,64(fp)		#right index  2

	sll	t0,t0,2
	sll	t1,t1,2		#multiplico por 4 los indices.
	sll	t2,t2,2	
	sll	t3,t3,2	


#iteradores
	move	t5,t0			#i = l1
	move	t6,t2			#j = l2
	li	t7,0			#k = 0;
	li	t8,0			#auxiliar acceso a array original
	li	t4,0			#auxiliar acceso a array temporal
	sw	t5,I_OFFSET(fp)	
	sw	t6,J_OFFSET(fp)		#guardo en LTA
	sw	t7,K_OFFSET(fp)


while_elements_in_both_arrays:	#while(l<= l1 && r<=r2)
	bgt	t5,t1,while_i_less_than_right_index_1	#condicion 1
	bgt	t6,t3,while_i_less_than_right_index_1	#condicion 2
	
	lw	a0,48(fp)		#a0: dir array
	addu	a0,a0,t5		#s0: dir de array[i]
	lw	t8,0(a0)		#t8: array[i]
	
	lw	a0,48(fp)		#s0: posicion inicial
	addu	a0,a0,t6		#s0: dir de array[j]
	lw	t4,0(a0)		#t9: array[j]
	lw	a0,48(fp)		#s0: posicion inicial
	bge	t8,t4,else_element_is_less_than 	#condicion
	
	if_element_is_less_than:		#if(a[i] < a[j])	
		lw	a1,TEMPORAL_OFFSET(fp)
		addu	a1,a1,t7		#s1: dir de temp[k]
		sw	t8,0(a1)		#temp[k] = array[i]
		addiu	t7,t7,4
		addiu	t5,t5,4
		lw	a1,TEMPORAL_OFFSET(fp)		#s1: posicion inicial
		b while_elements_in_both_arrays

	else_element_is_less_than:
		lw	a1,TEMPORAL_OFFSET(fp)
		addu	a1,a1,t7		#s1:dir temp[k]
		sw	t4,0(a1)		#temp[k] = array[j]
		addiu	t7,t7,4
		addiu	t6,t6,4
		lw	a1,TEMPORAL_OFFSET(fp)	
		b while_elements_in_both_arrays


while_i_less_than_right_index_1:	#while(i<=r1)
	bgt	t5,t1,while_j_less_than_right_index_2
	lw	a1,TEMPORAL_OFFSET(fp)		#cargo temporal
	addu	a1,a1,t7			#s1: dir de temp[k]
	
	lw	a0,48(fp)	#cargo array
	addu	a0,a0,t5	#dir de array[i]
	lw	t8,0(a0)	#t8: array[i]
	sw	t8,0(a1)	#temp[k] = array[i]
	
	#aumento iteradores
	addiu	t7,t7,4		#k++
	addiu	t5,t5,4		#i++
	b while_i_less_than_right_index_1

while_j_less_than_right_index_2:
	bgt	t6,t3,transfer_elements_from_temp_to_array
	lw	a1,TEMPORAL_OFFSET(fp)	#cargo temporal
	addu	a1,a1,t7		#s1: dir de temp[k]
	
	lw	a0,48(fp)		#s0: array
	addu	a0,a0,t6		#s0: dir de array[j]
	lw	t8,0(a0)		#t8: array[j]
	sw	t8,0(a1)		#temp[k] = array[j]

	#aumento iteradores
	addiu	t6,t6,4		#j++
	addiu	t7,t7,4		#k++		
	b while_j_less_than_right_index_2

transfer_elements_from_temp_to_array:
	move	t5,t0		#i=i1
	li	t6,0		#j=0
	while_i_less_or_equal_than_R2:
		bgt	t5,t3,free_temporal
		lw	a0,48(fp)		#cargo array original
		lw	a1,TEMPORAL_OFFSET(fp)	#cargo temporal
		addu	a0,a0,t5		#dir de array[i]
		addu	a1,a1,t6		#dir de temp[j]
		lw	t4,0(a1)		#temp[j]
		sw	t4,0(a0)		#array[i] = temp[j]
		addiu	t5,t5,4
		addiu	t6,t6,4
		b	while_i_less_or_equal_than_R2

free_temporal:
	lw	a0,TEMPORAL_OFFSET(fp)
	jal	myfree
	
return_merge_sub_arrays:
	lw	ra,SUB_ARRAY_RA_OFFSET(sp) 
	lw	fp,SUB_ARRAY_FP_OFFSET(sp)
	addiu	sp,sp,SUB_ARRAY_STACK_SIZE
	j	ra
.end merge_sub_arrays


#define SORT_STACK_SIZE 48
#define SORT_SP_OFFSET 40
#define SORT_FP_OFFSET 36
#define SORT_GP_OFFSET 32
#define MIDDLE_OFFSET 24
.globl	merge_sort
.ent	merge_sort

merge_sort: 	#(a0:array[size], a1:left index, a2:right index, a3:max size)
	.frame		fp,SORT_STACK_SIZE,ra
	subu 		sp,sp,SORT_STACK_SIZE
	sw		ra,SORT_SP_OFFSET(sp)
	sw		fp,SORT_FP_OFFSET(sp)
	.cprestore 	SORT_GP_OFFSET
	move		fp,sp

	sw	a0,48(fp)	#array
	sw	a1,52(fp)	#left index
	sw	a2,56(fp)	#right index
	sw	a3,60(fp)	#max size
	
	lw	t0,48(fp)
	lw	t1,52(fp)
	lw	t2,56(fp)
	lw	t3,60(fp)	

	li	t4,0				#medio
	bge	t1,t2,return_merge_sort		#if(l<r)
	addu	t4,t1,t2			#l+r
	srl	t4,t4,1				#(l+r)/2
	sw	t4,MIDDLE_OFFSET(fp)		#medio
	
left_recursion:
	lw	a0,48(fp)			#array
	lw	a1,52(fp)			#left
	lw	a2,MIDDLE_OFFSET(fp)		#medio
	lw	a3,60(fp)			#size
	jal	merge_sort

right_recursion:
	lw	a0,48(fp)		#array
	lw	a1,MIDDLE_OFFSET(fp)	#medio
	addiu	a1,a1,1			#medio+1
	lw	a2,56(fp)		#right index
	lw	a3,60(fp)		#size
	jal	merge_sort

merging_sub_arrays:
	lw	a0,48(fp)		#array
	lw	a1,52(fp)		#left index
	lw 	a2,MIDDLE_OFFSET(fp)	#medio
	addiu	a3,a2,1			#medio +1

	lw	t0,56(fp)	#j por stack
	sw	t0,16(sp)	#Lo mando por stack
	lw	t1,60(fp)	#t1: max_size	
	sw	t1,20(sp)	#lo mando por stack	
	
	jal 	merge_sub_arrays

return_merge_sort:
	lw	ra,SORT_SP_OFFSET(sp)
	lw	fp,SORT_FP_OFFSET(sp)
	addiu	sp,sp,SORT_STACK_SIZE
	j	ra

	.end merge_sort
