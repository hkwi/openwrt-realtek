	.file	1 "rtl865x_asicBasic.c"
	.section .mdebug.abi32
	.previous
#APP
	.macro _ssnop; sll $0, $0, 1; .endm
	.macro _ehb; sll $0, $0, 3; .endm
	.macro mtc0_tlbw_hazard; nop; nop; .endm
	.macro tlbw_use_hazard; nop; nop; nop; .endm
	.macro tlb_probe_hazard; nop; nop; nop; .endm
	.macro irq_enable_hazard; _ssnop; _ssnop; _ssnop;; .endm
	.macro irq_disable_hazard; nop; nop; nop; .endm
	.macro back_to_back_c0_hazard; _ssnop; _ssnop; _ssnop;; .endm
		.macro	raw_local_irq_enable				
	.set	push						
	.set	reorder						
	.set	noat						
	mfc0	$1,$12						
	ori	$1,0x1f						
	xori	$1,0x1e						
	mtc0	$1,$12						
	irq_enable_hazard					
	.set	pop						
	.endm
		.macro	raw_local_irq_disable
	.set	push						
	.set	noat						
	mfc0	$1,$12						
	ori	$1,0x1f						
	xori	$1,0x1f						
	.set	noreorder					
	mtc0	$1,$12						
	irq_disable_hazard					
	.set	pop						
	.endm							

		.macro	raw_local_save_flags flags			
	.set	push						
	.set	reorder						
	mfc0	\flags, $12					
	.set	pop						
	.endm							

		.macro	raw_local_irq_save result			
	.set	push						
	.set	reorder						
	.set	noat						
	mfc0	\result, $12					
	ori	$1, \result, 0x1f				
	xori	$1, 0x1f					
	.set	noreorder					
	mtc0	$1, $12						
	irq_disable_hazard					
	.set	pop						
	.endm							

		.macro	raw_local_irq_restore flags			
	.set	push						
	.set	noreorder					
	.set	noat						
	mfc0	$1, $12						
	andi	\flags, 1					
	ori	$1, 0x1f					
	xori	$1, 0x1f					
	or	\flags, $1					
	mtc0	\flags, $12					
	irq_disable_hazard					
	.set	pop						
	.endm							

#NO_APP
	.section	.text.prom_putchar,"ax",@progbits
	.align	2
	.ent	prom_putchar
	.type	prom_putchar, @function
prom_putchar:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	sll	$4,$4,24
	sra	$4,$4,24
	move	$3,$0
	li	$2,-1207959552			# 0xffffffffb8000000
	ori	$5,$2,0x2014
	move	$2,$3
$L6:
	sltu	$2,$2,30000
	bne	$2,$0,$L4
	addiu	$3,$3,1

	li	$3,-60
	li	$2,-1207959552			# 0xffffffffb8000000
	ori	$2,$2,0x2008
	sb	$3,0($2)
	j	$31
	nop

$L4:
	lbu	$2,0($5)
	nop
	andi	$2,$2,0x20
	beq	$2,$0,$L6
	move	$2,$3

	li	$2,-1207959552			# 0xffffffffb8000000
	ori	$2,$2,0x2000
	sb	$4,0($2)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	prom_putchar
	.section	.text.early_console_write,"ax",@progbits
	.align	2
	.ent	early_console_write
	.type	early_console_write, @function
early_console_write:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	move	$16,$4
	beq	$5,$0,$L9
	addiu	$17,$5,-1

	lb	$2,0($4)
	nop
	beq	$2,$0,$L9
	li	$19,10			# 0xa

	li	$18,-1			# 0xffffffffffffffff
	lb	$2,0($16)
	nop
$L13:
	bne	$2,$19,$L10
	nop

	jal	prom_putchar
	li	$4,13			# 0xd

$L10:
	lb	$4,0($16)
	jal	prom_putchar
	addiu	$16,$16,1

	addiu	$17,$17,-1
	beq	$17,$18,$L9
	nop

	lb	$2,0($16)
	nop
	bne	$2,$0,$L13
	nop

$L9:
	lw	$31,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	early_console_write
	.data
	.align	2
	.type	_rtl8651_asicTableSize, @object
	.size	_rtl8651_asicTableSize, 64
_rtl8651_asicTableSize:
	.word	2
	.word	1
	.word	2
	.word	3
	.word	5
	.word	3
	.word	3
	.word	3
	.word	4
	.word	3
	.word	3
	.word	1
	.word	8
	.word	1
	.word	3
	.word	1
	.section	.dram-fwd,"aw",@progbits
	.align	2
	.type	fun_enable, @object
	.size	fun_enable, 4
fun_enable:
	.word	0
	.section	.text.rtl865x_initAsicFun,"ax",@progbits
	.align	2
	.globl	rtl865x_initAsicFun
	.ent	rtl865x_initAsicFun
	.type	rtl865x_initAsicFun, @function
rtl865x_initAsicFun:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	move	$6,$4
	sw	$0,0($4)
	li	$2,-1207959552			# 0xffffffffb8000000
	lw	$4,0($2)
	li	$3,-65536			# 0xffffffffffff0000
	and	$4,$4,$3
	ori	$2,$2,0xc
	lw	$3,0($2)
	li	$2,-1073741824			# 0xffffffffc0000000
	bne	$4,$2,$L15
	andi	$5,$3,0xf

	li	$2,9			# 0x9
	beq	$5,$2,$L17
	nop

	andi	$2,$3,0x8
	bne	$2,$0,$L16
	nop

$L17:
	lw	$2,0($6)
	nop
	ori	$2,$2,0x2e
	j	$L23
	sw	$2,0($6)

$L16:
	li	$2,8			# 0x8
	bne	$5,$2,$L19
	nop

	lw	$2,0($6)
	nop
	ori	$2,$2,0xe
	j	$L23
	sw	$2,0($6)

$L19:
	addiu	$2,$5,-10
	sltu	$2,$2,2
	beq	$2,$0,$L21
	nop

	lw	$2,0($6)
	nop
	ori	$2,$2,0xa
	j	$L23
	sw	$2,0($6)

$L21:
	lw	$2,0($6)
	nop
	ori	$2,$2,0xe
	j	$L23
	sw	$2,0($6)

$L15:
	li	$2,-2147483648			# 0xffffffff80000000
	bne	$4,$2,$L23
	nop

	li	$2,7			# 0x7
	bne	$5,$2,$L25
	nop

	lw	$2,0($6)
	nop
	ori	$2,$2,0x2e
	j	$L23
	sw	$2,0($6)

$L25:
	li	$2,15			# 0xf
	bne	$5,$2,$L27
	nop

	lw	$2,0($6)
	nop
	ori	$2,$2,0x3
	j	$L23
	sw	$2,0($6)

$L27:
	li	$2,3			# 0x3
	bne	$5,$2,$L29
	nop

	lw	$2,0($6)
	nop
	ori	$2,$2,0x3
	j	$L23
	sw	$2,0($6)

$L29:
	lw	$2,0($6)
	nop
	ori	$2,$2,0x3
	sw	$2,0($6)
$L23:
	lw	$3,0($6)
	li	$2,-1			# 0xffffffffffffffff
	j	$31
	movn	$2,$0,$3		#RLX4181/RLX4281:conditional move

	.set	macro
	.set	reorder
	.end	rtl865x_initAsicFun
	.section	.text.rtl865x_getAsicFun,"ax",@progbits
	.align	2
	.globl	rtl865x_getAsicFun
	.ent	rtl865x_getAsicFun
	.type	rtl865x_getAsicFun, @function
rtl865x_getAsicFun:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	move	$17,$4
	lui	$2,%hi(fun_enable)
	lw	$3,%lo(fun_enable)($2)
	nop
	sw	$3,0($4)
	lw	$2,%lo(fun_enable)($2)
	nop
	bne	$2,$0,$L33
	move	$3,$0

	lui	$16,%hi(fun_enable)
	jal	rtl865x_initAsicFun
	addiu	$4,$16,%lo(fun_enable)

	lw	$2,%lo(fun_enable)($16)
	nop
	sw	$2,0($17)
	move	$3,$0
$L33:
	move	$2,$3
	lw	$31,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	rtl865x_getAsicFun
	.rdata
	.align	2
$LC0:
	.ascii	"init switch core failed!!!\n\000"
	.section	.text.bsp_swcore_init,"ax",@progbits
	.align	2
	.globl	bsp_swcore_init
	.ent	bsp_swcore_init
	.type	bsp_swcore_init, @function
bsp_swcore_init:
	.set	nomips16
	.frame	$sp,48,$31		# vars= 0, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$22,40($sp)
	sw	$21,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	move	$22,$4
	move	$20,$0
	li	$16,-1207959552			# 0xffffffffb8000000
	lw	$2,0($16)
	li	$17,-65536			# 0xffffffffffff0000
	and	$18,$2,$17
	ori	$21,$16,0xc
	lw	$2,0($21)
	nop
	andi	$19,$2,0xf
	lui	$4,%hi(fun_enable)
	jal	rtl865x_initAsicFun
	addiu	$4,$4,%lo(fun_enable)

	li	$3,-1			# 0xffffffffffffffff
	beq	$2,$3,$L36
	li	$4,-1			# 0xffffffffffffffff

	lw	$2,0($16)
	nop
	and	$2,$2,$17
	li	$3,-1073741824			# 0xffffffffc0000000
	bne	$2,$3,$L52
	li	$2,-1073741824			# 0xffffffffc0000000

	lw	$2,0($21)
	nop
	andi	$2,$2,0x8
	bne	$2,$0,$L52
	li	$2,-1073741824			# 0xffffffffc0000000

	addiu	$2,$22,-8
	sltu	$2,$2,2
	bne	$2,$0,$L36
	move	$4,$0

	li	$2,-1073741824			# 0xffffffffc0000000
$L52:
	bne	$18,$2,$L41
	li	$2,-2147483648			# 0xffffffff80000000

	li	$2,9			# 0x9
	bne	$22,$2,$L44
	move	$20,$0

	beq	$19,$22,$L44
	lui	$4,%hi($LC0)

	addiu	$4,$4,%lo($LC0)
	jal	early_console_write
	li	$5,27			# 0x1b

	j	$L44
	li	$20,-1			# 0xffffffffffffffff

$L41:
	bne	$18,$2,$L36
	move	$4,$20

	li	$2,7			# 0x7
	bne	$22,$2,$L53
	li	$2,15			# 0xf

	beq	$19,$22,$L53
	lui	$4,%hi($LC0)

	addiu	$4,$4,%lo($LC0)
	jal	early_console_write
	li	$5,27			# 0x1b

	j	$L44
	li	$20,-1			# 0xffffffffffffffff

$L53:
	bne	$22,$2,$L54
	li	$2,3			# 0x3

	beq	$19,$22,$L48
	li	$2,7			# 0x7

	beq	$19,$2,$L48
	li	$2,3			# 0x3

	beq	$19,$2,$L48
	li	$2,11			# 0xb

	beq	$19,$2,$L48
	lui	$4,%hi($LC0)

	addiu	$4,$4,%lo($LC0)
	jal	early_console_write
	li	$5,27			# 0x1b

	j	$L44
	li	$20,-1			# 0xffffffffffffffff

$L48:
	li	$2,3			# 0x3
$L54:
	bne	$22,$2,$L44
	move	$20,$0

	beq	$19,$22,$L44
	li	$2,7			# 0x7

	beq	$19,$2,$L55
	lui	$4,%hi($LC0)

	addiu	$4,$4,%lo($LC0)
	jal	early_console_write
	li	$5,27			# 0x1b

	li	$20,-1			# 0xffffffffffffffff
$L44:
$L55:
	move	$4,$20
$L38:
$L36:
	move	$2,$4
	lw	$31,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	bsp_swcore_init
	.section	.text.rtl865x_accessAsicTable,"ax",@progbits
	.align	2
	.globl	rtl865x_accessAsicTable
	.ent	rtl865x_accessAsicTable
	.type	rtl865x_accessAsicTable, @function
rtl865x_accessAsicTable:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 8, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	move	$16,$4
	move	$17,$5
	jal	rtl865x_getAsicFun
	addiu	$4,$sp,16

	sltu	$2,$16,15
	beq	$2,$0,$L72
	li	$2,1			# 0x1

	li	$3,1			# 0x1
	sll	$4,$3,$16
	andi	$2,$4,0xe22
	bne	$2,$0,$L65
	andi	$2,$4,0x8

	bne	$2,$0,$L68
	andi	$2,$4,0x4000

	beq	$2,$0,$L71
	li	$2,1			# 0x1

	lw	$2,16($sp)
	nop
	andi	$2,$2,0x20
	movz	$3,$0,$2		#RLX4181/RLX4281:conditional move
	j	$L57
	sw	$3,0($17)

$L65:
	lw	$2,16($sp)
	nop
	andi	$2,$2,0x4
	beq	$2,$0,$L66
	li	$2,1			# 0x1

	j	$L57
	sw	$2,0($17)

$L66:
	j	$L57
	sw	$0,0($17)

$L68:
	lw	$2,16($sp)
	nop
	andi	$2,$2,0x2
	beq	$2,$0,$L69
	li	$2,1			# 0x1

	j	$L57
	sw	$2,0($17)

$L69:
	j	$L57
	sw	$0,0($17)

$L71:
$L72:
	sw	$2,0($17)
$L57:
	move	$2,$0
	lw	$31,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	rtl865x_accessAsicTable
	.section	.text._rtl8651_asicTableAccessForward,"ax",@progbits
	.align	2
	.ent	_rtl8651_asicTableAccessForward
	.type	_rtl8651_asicTableAccessForward, @function
_rtl8651_asicTableAccessForward:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	bne	$6,$0,$L74
	li	$2,-1149239296			# 0xffffffffbb800000

$L76:
	j	$L76
	nop

$L74:
	ori	$3,$2,0x4d00
$L78:
	lw	$2,0($3)
	nop
	andi	$2,$2,0x1
	bne	$2,$0,$L78
	sll	$2,$4,2

	move	$8,$2
	lui	$3,%hi(_rtl8651_asicTableSize)
	addiu	$3,$3,%lo(_rtl8651_asicTableSize)
	addu	$2,$2,$3
	lw	$2,0($2)
	nop
	beq	$2,$0,$L85
	move	$7,$0

	li	$9,-1149239296			# 0xffffffffbb800000
	addu	$8,$8,$3
	sll	$2,$7,2
$L86:
	addu	$3,$2,$9
	addu	$2,$2,$6
	lw	$2,0($2)
	nop
	sw	$2,19744($3)
	addiu	$7,$7,1
	lw	$2,0($8)
	nop
	sltu	$2,$7,$2
	bne	$2,$0,$L86
	sll	$2,$7,2

$L85:
	sll	$3,$4,16
	sll	$2,$5,5
	addu	$3,$3,$2
	li	$2,-1157627904			# 0xffffffffbb000000
	addu	$3,$3,$2
	li	$2,-1149239296			# 0xffffffffbb800000
	ori	$2,$2,0x4d08
	sw	$3,0($2)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	_rtl8651_asicTableAccessForward
	.section	.text._rtl8651_addAsicEntry,"ax",@progbits
	.align	2
	.globl	_rtl8651_addAsicEntry
	.ent	_rtl8651_addAsicEntry
	.type	_rtl8651_addAsicEntry, @function
_rtl8651_addAsicEntry:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 8, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	move	$16,$4
	move	$17,$5
	move	$18,$6
	jal	rtl865x_accessAsicTable
	addiu	$5,$sp,16

	lw	$2,16($sp)
	nop
	beq	$2,$0,$L87
	li	$3,-1			# 0xffffffffffffffff

	move	$4,$16
	move	$5,$17
	jal	_rtl8651_asicTableAccessForward
	move	$6,$18

	lui	$2,%hi(RtkHomeGatewayChipNameID)
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L103
	lui	$2,%hi(RtkHomeGatewayChipNameID)

	lui	$2,%hi(RtkHomeGatewayChipRevisionID)
	lw	$2,%lo(RtkHomeGatewayChipRevisionID)($2)
	nop
	bgtz	$2,$L104
	li	$3,-1149239296			# 0xffffffffbb800000

	lui	$2,%hi(RtkHomeGatewayChipNameID)
$L103:
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,3			# 0x3
	beq	$3,$2,$L90
	li	$2,4			# 0x4

	bne	$3,$2,$L105
	li	$2,3			# 0x3

$L90:
	li	$3,-1149239296			# 0xffffffffbb800000
$L104:
	ori	$3,$3,0x4418
	lw	$2,0($3)
	li	$4,262144			# 0x40000
	or	$2,$2,$4
	sw	$2,0($3)
	move	$4,$3
	li	$3,524288			# 0x80000
$L92:
	lw	$2,0($4)
	nop
	and	$2,$2,$3
	beq	$2,$0,$L92
	li	$2,3			# 0x3

$L105:
	li	$3,-1149239296			# 0xffffffffbb800000
	ori	$3,$3,0x4d00
	sw	$2,0($3)
$L94:
	lw	$2,0($3)
	nop
	andi	$2,$2,0x1
	bne	$2,$0,$L94
	li	$2,-1149239296			# 0xffffffffbb800000

	ori	$2,$2,0x4d04
	lw	$2,0($2)
	nop
	andi	$2,$2,0x1
	beq	$2,$0,$L96
	lui	$2,%hi(RtkHomeGatewayChipNameID)

	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L106
	lui	$2,%hi(RtkHomeGatewayChipNameID)

	lui	$2,%hi(RtkHomeGatewayChipRevisionID)
	lw	$2,%lo(RtkHomeGatewayChipRevisionID)($2)
	nop
	bgtz	$2,$L98
	lui	$2,%hi(RtkHomeGatewayChipNameID)

$L106:
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,3			# 0x3
	beq	$3,$2,$L98
	li	$2,4			# 0x4

	bne	$3,$2,$L97
	nop

$L98:
	li	$4,-1149239296			# 0xffffffffbb800000
	ori	$4,$4,0x4418
	lw	$3,0($4)
	li	$2,-327680			# 0xfffffffffffb0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	sw	$3,0($4)
$L97:
	j	$L87
	li	$3,-1			# 0xffffffffffffffff

$L96:
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L107
	lui	$2,%hi(RtkHomeGatewayChipNameID)

	lui	$2,%hi(RtkHomeGatewayChipRevisionID)
	lw	$2,%lo(RtkHomeGatewayChipRevisionID)($2)
	nop
	bgtz	$2,$L108
	li	$4,-1149239296			# 0xffffffffbb800000

	lui	$2,%hi(RtkHomeGatewayChipNameID)
$L107:
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,3			# 0x3
	beq	$3,$2,$L101
	li	$2,4			# 0x4

	bne	$3,$2,$L87
	move	$3,$0

$L101:
	li	$4,-1149239296			# 0xffffffffbb800000
$L108:
	ori	$4,$4,0x4418
	lw	$3,0($4)
	li	$2,-327680			# 0xfffffffffffb0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	sw	$3,0($4)
	move	$3,$0
$L87:
	move	$2,$3
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	_rtl8651_addAsicEntry
	.data
	.align	2
	.type	mcastForceAddOpCnt, @object
	.size	mcastForceAddOpCnt, 4
mcastForceAddOpCnt:
	.word	0
	.section	.text._rtl865x_getForceAddMcastOpCnt,"ax",@progbits
	.align	2
	.globl	_rtl865x_getForceAddMcastOpCnt
	.ent	_rtl865x_getForceAddMcastOpCnt
	.type	_rtl865x_getForceAddMcastOpCnt, @function
_rtl865x_getForceAddMcastOpCnt:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(mcastForceAddOpCnt)
	lw	$2,%lo(mcastForceAddOpCnt)($2)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	_rtl865x_getForceAddMcastOpCnt
	.section	.text._rtl8651_forceAddAsicEntry,"ax",@progbits
	.align	2
	.globl	_rtl8651_forceAddAsicEntry
	.ent	_rtl8651_forceAddAsicEntry
	.type	_rtl8651_forceAddAsicEntry, @function
_rtl8651_forceAddAsicEntry:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 8, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	move	$16,$4
	move	$17,$5
	move	$18,$6
	jal	rtl865x_accessAsicTable
	addiu	$5,$sp,16

	lw	$2,16($sp)
	nop
	beq	$2,$0,$L110
	li	$3,-1			# 0xffffffffffffffff

	li	$2,3			# 0x3
	bne	$16,$2,$L123
	lui	$2,%hi(RtkHomeGatewayChipNameID)

	lui	$3,%hi(mcastForceAddOpCnt)
	lw	$2,%lo(mcastForceAddOpCnt)($3)
	nop
	addiu	$2,$2,1
	sw	$2,%lo(mcastForceAddOpCnt)($3)
	lui	$2,%hi(RtkHomeGatewayChipNameID)
$L123:
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L124
	lui	$2,%hi(RtkHomeGatewayChipNameID)

	lui	$2,%hi(RtkHomeGatewayChipRevisionID)
	lw	$2,%lo(RtkHomeGatewayChipRevisionID)($2)
	nop
	bgtz	$2,$L125
	li	$3,-1149239296			# 0xffffffffbb800000

	lui	$2,%hi(RtkHomeGatewayChipNameID)
$L124:
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,3			# 0x3
	beq	$3,$2,$L114
	li	$2,4			# 0x4

	bne	$3,$2,$L126
	move	$4,$16

$L114:
	li	$3,-1149239296			# 0xffffffffbb800000
$L125:
	ori	$3,$3,0x4418
	lw	$2,0($3)
	li	$4,262144			# 0x40000
	or	$2,$2,$4
	sw	$2,0($3)
	move	$4,$3
	li	$3,524288			# 0x80000
$L116:
	lw	$2,0($4)
	nop
	and	$2,$2,$3
	beq	$2,$0,$L116
	nop

	move	$4,$16
$L126:
	move	$5,$17
	jal	_rtl8651_asicTableAccessForward
	move	$6,$18

	li	$2,9			# 0x9
	li	$3,-1149239296			# 0xffffffffbb800000
	ori	$3,$3,0x4d00
	sw	$2,0($3)
$L118:
	lw	$2,0($3)
	nop
	andi	$2,$2,0x1
	bne	$2,$0,$L118
	lui	$2,%hi(RtkHomeGatewayChipNameID)

	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L127
	lui	$2,%hi(RtkHomeGatewayChipNameID)

	lui	$2,%hi(RtkHomeGatewayChipRevisionID)
	lw	$2,%lo(RtkHomeGatewayChipRevisionID)($2)
	nop
	bgtz	$2,$L128
	li	$4,-1149239296			# 0xffffffffbb800000

	lui	$2,%hi(RtkHomeGatewayChipNameID)
$L127:
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,3			# 0x3
	beq	$3,$2,$L121
	li	$2,4			# 0x4

	bne	$3,$2,$L110
	move	$3,$0

$L121:
	li	$4,-1149239296			# 0xffffffffbb800000
$L128:
	ori	$4,$4,0x4418
	lw	$3,0($4)
	li	$2,-327680			# 0xfffffffffffb0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	sw	$3,0($4)
	move	$3,$0
$L110:
	move	$2,$3
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	_rtl8651_forceAddAsicEntry
	.section	.text._rtl8651_readAsicEntry,"ax",@progbits
	.align	2
	.globl	_rtl8651_readAsicEntry
	.ent	_rtl8651_readAsicEntry
	.type	_rtl8651_readAsicEntry, @function
_rtl8651_readAsicEntry:
	.set	nomips16
	.frame	$sp,112,$31		# vars= 72, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$20,104($sp)
	sw	$19,100($sp)
	sw	$18,96($sp)
	sw	$17,92($sp)
	sw	$16,88($sp)
	move	$17,$4
	move	$18,$5
	move	$19,$6
	li	$16,2			# 0x2
	li	$20,10			# 0xa
	jal	rtl865x_accessAsicTable
	addiu	$5,$sp,80

	lw	$2,80($sp)
	nop
	beq	$2,$0,$L129
	li	$3,-1			# 0xffffffffffffffff

	bne	$19,$0,$L131
	sll	$10,$17,16

$L133:
	j	$L133
	nop

$L131:
	sll	$2,$18,5
	addu	$10,$10,$2
	li	$2,-1157627904			# 0xffffffffbb000000
	addu	$10,$10,$2
	li	$2,-1149239296			# 0xffffffffbb800000
	ori	$3,$2,0x4d00
$L135:
	lw	$2,0($3)
	nop
	andi	$2,$2,0x1
	bne	$2,$0,$L135
	addiu	$12,$sp,16

$L137:
	move	$9,$0
$L144:
	beq	$16,$9,$L142
	sll	$3,$9,5

	addu	$3,$3,$12
	lw	$2,0($10)
	nop
	sw	$2,0($3)
	lw	$2,4($10)
	nop
	sw	$2,4($3)
	lw	$2,8($10)
	nop
	sw	$2,8($3)
	lw	$2,12($10)
	nop
	sw	$2,12($3)
	lw	$2,16($10)
	nop
	sw	$2,16($3)
	lw	$2,20($10)
	nop
	sw	$2,20($3)
	lw	$2,24($10)
	nop
	sw	$2,24($3)
	lw	$2,28($10)
	nop
	sw	$2,28($3)
	move	$16,$9
$L142:
	addiu	$9,$9,1
	sltu	$2,$9,2
	bne	$2,$0,$L144
	move	$11,$0

	move	$9,$0
	addiu	$13,$sp,16
	sltu	$2,$9,2
$L173:
	beq	$2,$0,$L167
	move	$8,$9

	sll	$7,$9,3
	move	$5,$13
$L157:
	move	$4,$0
	sll	$6,$8,3
	addu	$3,$7,$4
$L172:
	sll	$3,$3,2
	addu	$3,$3,$5
	addu	$2,$6,$4
	sll	$2,$2,2
	addu	$2,$2,$5
	lw	$3,0($3)
	lw	$2,0($2)
	nop
	beq	$3,$2,$L153
	addiu	$4,$4,1

	addiu	$4,$4,-1
	j	$L155
	li	$11,1			# 0x1

$L153:
	slt	$2,$4,8
	bne	$2,$0,$L172
	addu	$3,$7,$4

	addiu	$8,$8,1
	sltu	$2,$8,2
	bne	$2,$0,$L157
	nop

$L167:
	addiu	$9,$9,1
	sltu	$2,$9,2
	bne	$2,$0,$L173
	nop

$L155:
	bne	$11,$0,$L138
	addiu	$20,$20,-1

	bne	$20,$0,$L137
	nop

$L138:
	sll	$3,$16,5
	addiu	$2,$sp,16
	addu	$10,$2,$3
	sll	$3,$17,2
	lui	$2,%hi(_rtl8651_asicTableSize)
	addiu	$2,$2,%lo(_rtl8651_asicTableSize)
	addu	$3,$3,$2
	lw	$2,0($3)
	nop
	beq	$2,$0,$L171
	move	$5,$0

	move	$4,$3
	sll	$2,$5,2
$L174:
	addu	$3,$2,$19
	addu	$2,$2,$10
	lw	$2,0($2)
	nop
	sw	$2,0($3)
	addiu	$5,$5,1
	lw	$2,0($4)
	nop
	sltu	$2,$5,$2
	bne	$2,$0,$L174
	sll	$2,$5,2

$L171:
	move	$3,$0
$L129:
	move	$2,$3
	lw	$31,108($sp)
	lw	$20,104($sp)
	lw	$19,100($sp)
	lw	$18,96($sp)
	lw	$17,92($sp)
	lw	$16,88($sp)
	j	$31
	addiu	$sp,$sp,112

	.set	macro
	.set	reorder
	.end	_rtl8651_readAsicEntry
	.section	.text._rtl8651_readAsicEntryStopTLU,"ax",@progbits
	.align	2
	.globl	_rtl8651_readAsicEntryStopTLU
	.ent	_rtl8651_readAsicEntryStopTLU
	.type	_rtl8651_readAsicEntryStopTLU, @function
_rtl8651_readAsicEntryStopTLU:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 8, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	move	$16,$4
	move	$17,$5
	move	$18,$6
	jal	rtl865x_accessAsicTable
	addiu	$5,$sp,16

	lw	$2,16($sp)
	nop
	beq	$2,$0,$L175
	li	$3,-1			# 0xffffffffffffffff

	bne	$18,$0,$L177
	sll	$6,$16,16

$L179:
	j	$L179
	nop

$L177:
	sll	$2,$17,5
	addu	$6,$6,$2
	li	$2,-1157627904			# 0xffffffffbb000000
	addu	$6,$6,$2
	lui	$2,%hi(RtkHomeGatewayChipNameID)
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L193
	lui	$2,%hi(RtkHomeGatewayChipNameID)

	lui	$2,%hi(RtkHomeGatewayChipRevisionID)
	lw	$2,%lo(RtkHomeGatewayChipRevisionID)($2)
	nop
	bgtz	$2,$L194
	li	$2,-1149239296			# 0xffffffffbb800000

	lui	$2,%hi(RtkHomeGatewayChipNameID)
$L193:
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,3			# 0x3
	beq	$3,$2,$L182
	li	$2,4			# 0x4

	bne	$3,$2,$L195
	sll	$2,$16,2

$L182:
	li	$2,-1149239296			# 0xffffffffbb800000
$L194:
	ori	$2,$2,0x4418
	lw	$3,0($2)
	li	$4,262144			# 0x40000
	or	$3,$3,$4
	sw	$3,0($2)
	sll	$2,$16,2
$L195:
	move	$4,$2
	lui	$3,%hi(_rtl8651_asicTableSize)
	addiu	$3,$3,%lo(_rtl8651_asicTableSize)
	addu	$2,$2,$3
	lw	$2,0($2)
	nop
	beq	$2,$0,$L192
	move	$5,$0

	addu	$4,$4,$3
	sll	$2,$5,2
$L196:
	addu	$3,$2,$18
	addu	$2,$2,$6
	lw	$2,0($2)
	nop
	sw	$2,0($3)
	addiu	$5,$5,1
	lw	$2,0($4)
	nop
	sltu	$2,$5,$2
	bne	$2,$0,$L196
	sll	$2,$5,2

$L192:
	lui	$2,%hi(RtkHomeGatewayChipNameID)
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L197
	lui	$2,%hi(RtkHomeGatewayChipNameID)

	lui	$2,%hi(RtkHomeGatewayChipRevisionID)
	lw	$2,%lo(RtkHomeGatewayChipRevisionID)($2)
	nop
	bgtz	$2,$L189
	lui	$2,%hi(RtkHomeGatewayChipNameID)

$L197:
	lw	$3,%lo(RtkHomeGatewayChipNameID)($2)
	li	$2,3			# 0x3
	beq	$3,$2,$L189
	li	$2,4			# 0x4

	bne	$3,$2,$L175
	move	$3,$0

$L189:
	li	$4,-1149239296			# 0xffffffffbb800000
	ori	$4,$4,0x4418
	lw	$3,0($4)
	li	$2,-327680			# 0xfffffffffffb0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	sw	$3,0($4)
	move	$3,$0
$L175:
	move	$2,$3
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	_rtl8651_readAsicEntryStopTLU
	.section	.text._rtl8651_delAsicEntry,"ax",@progbits
	.align	2
	.globl	_rtl8651_delAsicEntry
	.ent	_rtl8651_delAsicEntry
	.type	_rtl8651_delAsicEntry, @function
_rtl8651_delAsicEntry:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	li	$2,-1149239296			# 0xffffffffbb800000
	ori	$3,$2,0x4d00
$L199:
	lw	$2,0($3)
	nop
	andi	$2,$2,0x1
	bne	$2,$0,$L199
	sll	$2,$4,2

	move	$7,$2
	lui	$3,%hi(_rtl8651_asicTableSize)
	addiu	$3,$3,%lo(_rtl8651_asicTableSize)
	addu	$2,$2,$3
	lw	$2,0($2)
	nop
	beq	$2,$0,$L212
	move	$3,$0

	li	$8,-1149239296			# 0xffffffffbb800000
	lui	$2,%hi(_rtl8651_asicTableSize)
	addiu	$2,$2,%lo(_rtl8651_asicTableSize)
	addu	$7,$7,$2
	sll	$2,$3,2
$L215:
	addu	$2,$2,$8
	sw	$0,19744($2)
	addiu	$3,$3,1
	lw	$2,0($7)
	nop
	sltu	$2,$3,$2
	bne	$2,$0,$L215
	sll	$2,$3,2

$L212:
	sltu	$2,$6,$5
	bne	$2,$0,$L214
	nop

	li	$2,-1149239296			# 0xffffffffbb800000
	ori	$10,$2,0x4d08
	sll	$4,$4,16
	li	$9,-1157627904			# 0xffffffffbb000000
	ori	$3,$2,0x4d00
	li	$8,9			# 0x9
	ori	$7,$2,0x4d04
$L210:
	sll	$2,$5,5
	addu	$2,$4,$2
	addu	$2,$2,$9
	sw	$2,0($10)
	sw	$8,0($3)
$L207:
	lw	$2,0($3)
	nop
	andi	$2,$2,0x1
	bne	$2,$0,$L207
	nop

	lw	$2,0($7)
	nop
	andi	$2,$2,0x1
	beq	$2,$0,$L209
	addiu	$5,$5,1

	addiu	$5,$5,-1
	j	$31
	li	$2,-1			# 0xffffffffffffffff

$L209:
	sltu	$2,$6,$5
	beq	$2,$0,$L210
	nop

$L214:
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	_rtl8651_delAsicEntry
	.globl	RtkHomeGatewayChipName
	.section	.bss
	.align	2
	.type	RtkHomeGatewayChipName, @object
	.size	RtkHomeGatewayChipName, 16
RtkHomeGatewayChipName:
	.space	16
	.globl	RtkHomeGatewayChipNameID
	.align	2
	.type	RtkHomeGatewayChipNameID, @object
	.size	RtkHomeGatewayChipNameID, 4
RtkHomeGatewayChipNameID:
	.space	4
	.globl	RtkHomeGatewayChipRevisionID
	.align	2
	.type	RtkHomeGatewayChipRevisionID, @object
	.size	RtkHomeGatewayChipRevisionID, 4
RtkHomeGatewayChipRevisionID:
	.space	4
	.ident	"GCC: (GNU) 3.4.6-1.3.6"
