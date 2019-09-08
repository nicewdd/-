.data
.text
	move $fp, $sp
	j main
main:
	subi $sp, $sp, 76
	li $v0, 5
	syscall
	move $s3, $v0
	li $t0, 0
	li $t1, 1
	mul $t3, $t0, 4
	add $t4, $t3, 16
	sub $t5, $fp, $t4
	sw  $t1, 0($t5)
	li $t0, 1
	li $t1, 2
	mul $t3, $t0, 4
	add $t4, $t3, 16
	sub $t5, $fp, $t4
	sw  $t1, 0($t5)
	li $t0, 2
	li $t1, 3
	mul $t3, $t0, 4
	add $t4, $t3, 16
	sub $t5, $fp, $t4
	sw  $t1, 0($t5)
	li $t0, 3
	li $t1, 4
	mul $t3, $t0, 4
	add $t4, $t3, 16
	sub $t5, $fp, $t4
	sw  $t1, 0($t5)
	li $t1, 2
	mul $t0, $t1, 4
	addi $t1, $t0, 16
	sub $t0, $fp, $t1
	lw $t1, ($t0)
	move $s5, $t1
	move $s0, $s5
	li $t1, 3
	mul $t0, $t1, 4
	addi $t1, $t0, 16
	sub $t0, $fp, $t1
	lw $t1, ($t0)
	move $s6, $t1
	move $s1, $s6
	li $t1, 1
	mul $t0, $t1, 4
	addi $t1, $t0, 16
	sub $t0, $fp, $t1
	lw $t1, ($t0)
	move $s7, $t1
	move $s2, $s7
	sw $s0, -48($fp)
	sw $s1, -52($fp)
	lw $t1,-48($fp)
	lw $t2,-52($fp)
	addu $t0, $t1, $t2
	sw $t0, -56($fp)
	sw $s2, -60($fp)
	lw $t1,-56($fp)
	lw $t2,-60($fp)
	addu $t0, $t1, $t2
	sw $t0, -64($fp)
	sw $s3, -68($fp)
	lw $t1,-64($fp)
	lw $t2,-68($fp)
	addu $t0, $t1, $t2
	sw $t0, -72($fp)
	lw $a0, -72($fp)
	li $v0, 1
	syscall
