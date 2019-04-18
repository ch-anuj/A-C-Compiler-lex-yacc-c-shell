
.data

a.0:	.word	0

t.0:	.word	0


.text


#start of a basic block

.globl main

main:
sub $sp,$sp,4
sw $ra,($sp)
li $t0,4

li $s0,1
add $t1,$t0,$s0

move $t0,$t1

li $v0,1
move $a0,$t0
syscall

sw $t0,a.0

sw $t1,t.0

li $v0,10
syscall


#putting dirty variables back into memory

