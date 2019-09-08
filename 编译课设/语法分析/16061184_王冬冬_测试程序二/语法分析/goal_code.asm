.data
	flag: .word 0
	num1: .word 0
	num2: .word 0
	array: .space 40
	n: .word 0
	string: .space 12
	str1: .asciiz "compute the factorial of "
	str2: .asciiz "the result is:"
	str3: .asciiz "the sum is:"
	str4: .asciiz "too big!"
	str5: .asciiz "must be a positive number!"
	str6: .asciiz "too big!"
	str7: .asciiz "too small!"
	str8: .asciiz "too big!"
	str9: .asciiz "too small!"
	str10: .asciiz "invalid flag!"
	str11: .asciiz "invalid flag!"
	str12: .asciiz "flag is not equal to 0."
	str13: .asciiz "flag is equal to 0."
.text
	move $fp, $sp
	j main
sum:
	lw $s0, -0($fp)
	lw $s1, -4($fp)
	subi $sp, $sp, 12
	add $s2, $s0, $s1
	move $v0, $s2
	jr $ra
init:
	subi $sp, $sp, 56
	li $s1, 0
	sw $s1, flag
	li $s2, 0
	sw $s2, num1
	li $s3, 0
	sw $s3, num2
	li $s4, 0
	sw $s4, n
	li $s5, 0
	move $s0, $s5
label1:
	li $t1, 10
	bge $s0, $t1, label2
	li $s6, 0
	mul $t0, $s0, 4
	sw $s6, array($t0)
	li $s7, 1
	add $t0, $s0, $s7
	sw $t0, -32($fp)
	lw $t0,-32($fp)
	move $s0, $t0
	j label1
label2:
	li $t0, 2
	sw $t0, -36($fp)
	lw $t0,-36($fp)
	move $s0, $t0
label3:
	li $t0, 48
	sw $t0, -40($fp)
	lw $t1, -40($fp)
	mul $t0, $s0, 4
	sw $t1, string($t0)
	li $t0, 1
	sw $t0, -44($fp)
	lw $t2,-44($fp)
	sub $s0, $s0, $t2
	li $t0, 0
	sw $t0, -48($fp)
	lw $t1, -48($fp)
	blt $s0, $t1, label4
	j label3
label4:
	li $t0, 0
	sw $t0, -52($fp)
	lw $v0, -52($fp)
	jr $ra
print:
	lw $s0, -0($fp)
	subi $sp, $sp, 12
	li $s1, 1
	lw $t0, flag
	bne $t0, $s1, label5
	la $a0, str1
	li $v0, 4
	syscall
	lw $a0, n
	li $v0, 1
	syscall
	la $a0, str2
	li $v0, 4
	syscall
	move $a0, $s0
	li $v0, 1
	syscall
	jr $ra
	j label6
label5:
label6:
	li $s2, 2
	lw $t0, flag
	bne $t0, $s2, label7
	la $a0, str3
	li $v0, 4
	syscall
	move $a0, $s0
	li $v0, 1
	syscall
	jr $ra
	j label8
label7:
label8:
	jr $ra
	jr $ra
factorial:
	lw $s0, -0($fp)
	subi $sp, $sp, 24
	li $s1, 1
	bne $s0, $s1, label9
	move $v0, $s0
	jr $ra
	j label10
label9:
	li $s2, 1
	sub $s3, $s0, $s2
	sw $fp, 0($sp)
	sw $ra, -4($sp)
	subi $sp, $sp, 8
	sw $s3, -0($sp)
	sw $s0, -0($fp)
	sw $s1, -4($fp)
	sw $s2, -8($fp)
	sw $s3, -12($fp)
	sw $s4, -16($fp)
	sw $s5, -20($fp)
	move $fp, $sp
	jal factorial
	lw $ra, 4($fp)
	move $sp, $fp
	lw $fp, 8($sp)
	add $sp, $sp, 8
	lw $s0, -0($fp)
	lw $s1, -4($fp)
	lw $s2, -8($fp)
	lw $s3, -12($fp)
	lw $s4, -16($fp)
	lw $s5, -20($fp)
	move $s4, $v0
	mul $s5, $s0, $s4
	move $v0, $s5
	jr $ra
label10:
printc:
	subi $sp, $sp, 36
	li $t0, 97
	move $a0, $t0
	li $v0, 11
	syscall
	li $s1, 43
	move $a0, $s1
	li $v0, 11
	syscall
	li $s2, 45
	move $a0, $s2
	li $v0, 11
	syscall
	li $s3, 42
	move $a0, $s3
	li $v0, 11
	syscall
	li $s4, 47
	move $a0, $s4
	li $v0, 11
	syscall
	li $s5, 0
	move $s0, $s5
label11:
	mul $t0, $s0, 4
	lw $t1, string($t0)
	move $s6, $t1
	move $a0, $s6
	li $v0, 11
	syscall
	li $s7, 1
	add $s0, $s0, $s7
	li $t0, 3
	sw $t0, -32($fp)
	lw $t1, -32($fp)
	beq $s0, $t1, label12
	j label11
label12:
	jr $ra
fun:
	lw $s0, -0($fp)
	lw $s1, -4($fp)
	subi $sp, $sp, 88
	add $s4, $s0, $s1
	li $s5, 0
	mul $t0, $s5, 4
	lw $t1, array($t0)
	move $s6, $t1
	mul $s7, $s4, $s6
	li $t0, 10
	sw $t0, -32($fp)
	lw $t2,-32($fp)
	mul $t0, $s7, $t2
	sw $t0, -36($fp)
	li $t0, 65
	sw $t0, -40($fp)
	lw $t1,-36($fp)
	lw $t2,-40($fp)
	mul $t0, $t1, $t2
	sw $t0, -44($fp)
	li $t0, 1
	sw $t0, -48($fp)
	sw $fp, 0($sp)
	sw $ra, -4($sp)
	subi $sp, $sp, 8
	sw $s0, -0($sp)
	lw $t0, -48($fp)
	sw $t0, -4($sp)
	sw $s0, -0($fp)
	sw $s1, -4($fp)
	sw $s2, -8($fp)
	sw $s3, -12($fp)
	sw $s4, -16($fp)
	sw $s5, -20($fp)
	sw $s6, -24($fp)
	sw $s7, -28($fp)
	move $fp, $sp
	jal sum
	lw $ra, 4($fp)
	move $sp, $fp
	lw $fp, 8($sp)
	add $sp, $sp, 8
	lw $s0, -0($fp)
	lw $s1, -4($fp)
	lw $s2, -8($fp)
	lw $s3, -12($fp)
	lw $s4, -16($fp)
	lw $s5, -20($fp)
	lw $s6, -24($fp)
	lw $s7, -28($fp)
	sw $v0, -52($fp)
	lw $t1,-44($fp)
	lw $t2,-52($fp)
	div $t0, $t1, $t2
	sw $t0, -56($fp)
	lw $t0,-56($fp)
	move $s3, $t0
	li $t1, 0
	sub $t0, $t1, $s3
	sw $t0, -60($fp)
	li $t0, 97
	sw $t0, -64($fp)
	lw $t1,-60($fp)
	lw $t2,-64($fp)
	add $t0, $t1, $t2
	sw $t0, -68($fp)
	lw $t0,-68($fp)
	move $s3, $t0
	li $t1, 0
	add $t0, $t1, $s3
	sw $t0, -72($fp)
	li $t0, 97
	sw $t0, -76($fp)
	lw $t1,-72($fp)
	lw $t2,-76($fp)
	sub $t0, $t1, $t2
	sw $t0, -80($fp)
	lw $t0,-80($fp)
	move $s3, $t0
	li $t0, 97
	sw $t0, -84($fp)
	lw $t0,-84($fp)
	move $s2, $t0
	move $v0, $s2
	jr $ra
main:
	subi $sp, $sp, 52
	sw $fp, 0($sp)
	sw $ra, -4($sp)
	subi $sp, $sp, 8
	sw $s0, -0($fp)
	sw $s1, -4($fp)
	sw $s2, -8($fp)
	sw $s3, -12($fp)
	sw $s4, -16($fp)
	sw $s5, -20($fp)
	sw $s6, -24($fp)
	sw $s7, -28($fp)
	move $fp, $sp
	jal init
	lw $ra, 4($fp)
	move $sp, $fp
	lw $fp, 8($sp)
	add $sp, $sp, 8
	lw $s0, -0($fp)
	lw $s1, -4($fp)
	lw $s2, -8($fp)
	lw $s3, -12($fp)
	lw $s4, -16($fp)
	lw $s5, -20($fp)
	lw $s6, -24($fp)
	lw $s7, -28($fp)
	move $s1, $v0
	li $v0, 5
	syscall
	sw $v0, flag
	li $s2, 1
	lw $t0, flag
	bne $t0, $s2, label13
	li $v0, 5
	syscall
	sw $v0, n
	lw $t0, n
	li $t1, 10
	ble $t0, $t1, label14
	la $a0, str4
	li $v0, 4
	syscall
	li $v0, 10
	syscall
	j label15
label14:
label15:
	li $s3, 0
	lw $t0, n
	bgt $t0, $s3, label16
	la $a0, str5
	li $v0, 4
	syscall
	li $v0, 10
	syscall
	j label17
label16:
label17:
	sw $fp, 0($sp)
	sw $ra, -4($sp)
	subi $sp, $sp, 8
	lw $t0, n
	sw $t0, -0($sp)
	sw $s0, -0($fp)
	sw $s1, -4($fp)
	sw $s2, -8($fp)
	sw $s3, -12($fp)
	sw $s4, -16($fp)
	sw $s5, -20($fp)
	sw $s6, -24($fp)
	sw $s7, -28($fp)
	move $fp, $sp
	jal factorial
	lw $ra, 4($fp)
	move $sp, $fp
	lw $fp, 8($sp)
	add $sp, $sp, 8
	lw $s0, -0($fp)
	lw $s1, -4($fp)
	lw $s2, -8($fp)
	lw $s3, -12($fp)
	lw $s4, -16($fp)
	lw $s5, -20($fp)
	lw $s6, -24($fp)
	lw $s7, -28($fp)
	move $s4, $v0
	move $s0, $s4
	j label18
label13:
label18:
	li $s5, 2
	lw $t0, flag
	bne $t0, $s5, label19
	li $v0, 5
	syscall
	sw $v0, num1
	li $v0, 5
	syscall
	sw $v0, num2
	lw $t0, num1
	li $t1, 10
	ble $t0, $t1, label20
	la $a0, str6
	li $v0, 4
	syscall
	li $v0, 10
	syscall
	j label21
label20:
label21:
	lw $t0, num1
	li $t1, -100
	bge $t0, $t1, label22
	la $a0, str7
	li $v0, 4
	syscall
	li $v0, 10
	syscall
	j label23
label22:
label23:
	lw $t0, num2
	li $t1, 10
	ble $t0, $t1, label24
	la $a0, str8
	li $v0, 4
	syscall
	li $v0, 10
	syscall
	j label25
label24:
label25:
	lw $t0, num2
	li $t1, -100
	bge $t0, $t1, label26
	la $a0, str9
	li $v0, 4
	syscall
	li $v0, 10
	syscall
	j label27
label26:
label27:
	sw $fp, 0($sp)
	sw $ra, -4($sp)
	subi $sp, $sp, 8
	lw $t0, num1
	sw $t0, -0($sp)
	lw $t0, num2
	sw $t0, -4($sp)
	sw $s0, -0($fp)
	sw $s1, -4($fp)
	sw $s2, -8($fp)
	sw $s3, -12($fp)
	sw $s4, -16($fp)
	sw $s5, -20($fp)
	sw $s6, -24($fp)
	sw $s7, -28($fp)
	move $fp, $sp
	jal sum
	lw $ra, 4($fp)
	move $sp, $fp
	lw $fp, 8($sp)
	add $sp, $sp, 8
	lw $s0, -0($fp)
	lw $s1, -4($fp)
	lw $s2, -8($fp)
	lw $s3, -12($fp)
	lw $s4, -16($fp)
	lw $s5, -20($fp)
	lw $s6, -24($fp)
	lw $s7, -28($fp)
	move $s6, $v0
	move $s0, $s6
	j label28
label19:
label28:
	li $s7, 3
	lw $t0, flag
	bne $t0, $s7, label29
	sw $fp, 0($sp)
	sw $ra, -4($sp)
	subi $sp, $sp, 8
	sw $s0, -0($fp)
	sw $s1, -4($fp)
	sw $s2, -8($fp)
	sw $s3, -12($fp)
	sw $s4, -16($fp)
	sw $s5, -20($fp)
	sw $s6, -24($fp)
	sw $s7, -28($fp)
	move $fp, $sp
	jal printc
	lw $ra, 4($fp)
	move $sp, $fp
	lw $fp, 8($sp)
	add $sp, $sp, 8
	lw $s0, -0($fp)
	lw $s1, -4($fp)
	lw $s2, -8($fp)
	lw $s3, -12($fp)
	lw $s4, -16($fp)
	lw $s5, -20($fp)
	lw $s6, -24($fp)
	lw $s7, -28($fp)
	li $v0, 10
	syscall
	j label30
label29:
label30:
	li $t0, 3
	sw $t0, -32($fp)
	lw $t0, flag
	lw $t1, -32($fp)
	ble $t0, $t1, label31
	la $a0, str10
	li $v0, 4
	syscall
	li $v0, 10
	syscall
	j label32
label31:
label32:
	li $t0, 0
	sw $t0, -36($fp)
	lw $t0, flag
	lw $t1, -36($fp)
	bge $t0, $t1, label33
	la $a0, str11
	li $v0, 4
	syscall
	li $v0, 10
	syscall
	j label34
label33:
label34:
	sw $fp, 0($sp)
	sw $ra, -4($sp)
	subi $sp, $sp, 8
	sw $s0, -0($sp)
	sw $s0, -0($fp)
	sw $s1, -4($fp)
	sw $s2, -8($fp)
	sw $s3, -12($fp)
	sw $s4, -16($fp)
	sw $s5, -20($fp)
	sw $s6, -24($fp)
	sw $s7, -28($fp)
	move $fp, $sp
	jal print
	lw $ra, 4($fp)
	move $sp, $fp
	lw $fp, 8($sp)
	add $sp, $sp, 8
	lw $s0, -0($fp)
	lw $s1, -4($fp)
	lw $s2, -8($fp)
	lw $s3, -12($fp)
	lw $s4, -16($fp)
	lw $s5, -20($fp)
	lw $s6, -24($fp)
	lw $s7, -28($fp)
	li $t0, 0
	sw $t0, -40($fp)
	li $t0, 0
	sw $t0, -44($fp)
	sw $fp, 0($sp)
	sw $ra, -4($sp)
	subi $sp, $sp, 8
	lw $t0, -40($fp)
	sw $t0, -0($sp)
	lw $t0, -44($fp)
	sw $t0, -4($sp)
	sw $s0, -0($fp)
	sw $s1, -4($fp)
	sw $s2, -8($fp)
	sw $s3, -12($fp)
	sw $s4, -16($fp)
	sw $s5, -20($fp)
	sw $s6, -24($fp)
	sw $s7, -28($fp)
	move $fp, $sp
	jal fun
	lw $ra, 4($fp)
	move $sp, $fp
	lw $fp, 8($sp)
	add $sp, $sp, 8
	lw $s0, -0($fp)
	lw $s1, -4($fp)
	lw $s2, -8($fp)
	lw $s3, -12($fp)
	lw $s4, -16($fp)
	lw $s5, -20($fp)
	lw $s6, -24($fp)
	lw $s7, -28($fp)
	sw $v0, -48($fp)
	lw $a0, -48($fp)
	li $v0, 11
	syscall
	lw $t0, flag
	beqz $t0, label35
	la $a0, str12
	li $v0, 4
	syscall
	j label36
label35:
	la $a0, str13
	li $v0, 4
	syscall
label36:
	li $v0, 10
	syscall
