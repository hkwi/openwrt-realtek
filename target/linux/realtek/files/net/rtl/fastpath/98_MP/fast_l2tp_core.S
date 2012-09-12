	.file	1 "fast_l2tp_core.c"
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
	.data
	.align	2
	.type	res1, @object
	.size	res1, 4
res1:
	.word	0
	.align	2
	.type	l2tpInfo, @object
	.size	l2tpInfo, 40
l2tpInfo:
	.word	0
	.space	36
	.section	.text.is_l2tp_device,"ax",@progbits
	.align	2
	.globl	is_l2tp_device
	.ent	is_l2tp_device
	.type	is_l2tp_device, @function
is_l2tp_device:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lui	$2,%hi(l2tpInfo+4)
	lw	$2,%lo(l2tpInfo+4)($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2
	move	$5,$2
	.set	macro
	.set	reorder

	move	$3,$4
#APP
	.set	noreorder
	.set	noat
	lbu	$2,($5)
1:	lbu	$1,($3)
	addiu	$5,1
	bne	$1,$2,2f
	addiu	$3,1
	bnez	$2,1b
	lbu	$2,($5)
	move	$2,$1
2:	subu	$2,$1
3:	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

$L2:
	move	$3,$0
$L1:
	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$3
	.set	macro
	.set	reorder

	.end	is_l2tp_device
	.section	.text.set_l2tp_device,"ax",@progbits
	.align	2
	.globl	set_l2tp_device
	.ent	set_l2tp_device
	.type	set_l2tp_device, @function
set_l2tp_device:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	lui	$2,%hi(fast_l2tp_fw)
	sw	$31,16($sp)
	move	$5,$4
	lw	$2,%lo(fast_l2tp_fw)($2)
	lui	$4,%hi(init_net)
	beq	$2,$0,$L4
	addiu	$4,$4,%lo(init_net)

	jal	__dev_get_by_name
	nop

	lui	$3,%hi(l2tpInfo)
	addiu	$3,$3,%lo(l2tpInfo)
	sw	$2,4($3)
$L4:
	lw	$31,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	set_l2tp_device
	.section	.text.event_ppp_dev_down,"ax",@progbits
	.align	2
	.globl	event_ppp_dev_down
	.ent	event_ppp_dev_down
	.type	event_ppp_dev_down, @function
event_ppp_dev_down:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	lui	$2,%hi(l2tpInfo)
	addiu	$sp,$sp,-24
	sw	$31,16($sp)
	addiu	$5,$2,%lo(l2tpInfo)
	lhu	$2,38($5)
	beq	$2,$0,$L6
	lw	$2,4($5)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L6
	move	$6,$2
	.set	macro
	.set	reorder

	move	$3,$4
#APP
	.set	noreorder
	.set	noat
	lbu	$2,($6)
1:	lbu	$1,($3)
	addiu	$6,1
	bne	$1,$2,2f
	addiu	$3,1
	bnez	$2,1b
	lbu	$2,($6)
	move	$2,$1
2:	subu	$2,$1
3:	.set	at
	.set	reorder
#NO_APP
	li	$6,40			# 0x28
	move	$4,$5
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L6
	move	$5,$0
	.set	macro
	.set	reorder

	jal	memset
	lui	$2,%hi(state)
	sw	$0,%lo(state)($2)
$L6:
	lw	$31,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	event_ppp_dev_down
	.rdata
	.align	2
$LC0:
	.ascii	"ppp0\000"
	.align	2
$LC1:
	.ascii	"eth1\000"
	.section	.text.l2tp_tx_id,"ax",@progbits
	.align	2
	.globl	l2tp_tx_id
	.ent	l2tp_tx_id
	.type	l2tp_tx_id, @function
l2tp_tx_id:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$19,28($sp)
	move	$19,$4
	sw	$31,36($sp)
	sw	$20,32($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lw	$18,152($4)
	lbu	$2,0($18)
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$4,$18,$2
	li	$2,1701			# 0x6a5
	lhu	$6,0($4)
	andi	$3,$6,0xffff
	bne	$3,$2,$L28
	addiu	$5,$4,8

	lhu	$2,2($4)
	bne	$2,$3,$L28
	li	$2,1701			# 0x6a5

	lhu	$3,8($4)
	sll	$2,$3,16
	sra	$2,$2,16
	bgez	$2,$L13
	addiu	$2,$4,12

	andi	$3,$3,0x4000
	addiu	$17,$4,10
	movn	$17,$2,$3		#conditional move  
	li	$2,4			# 0x4
	lhu	$16,14($17)
	bne	$16,$2,$L29
	li	$2,11			# 0xb

	lui	$2,%hi(l2tpInfo)
	addiu	$4,$2,%lo(l2tpInfo)
	lhu	$3,20($4)
	lhu	$2,0($17)
	bne	$3,$2,$L29
	li	$2,11			# 0xb

	move	$5,$0
	jal	memset
	li	$6,40			# 0x28

	lui	$4,%hi($LC0)
	addiu	$4,$4,%lo($LC0)
	jal	rtl865x_setNetifType
	li	$5,2			# 0x2

	lui	$2,%hi(state)
	sw	$0,%lo(state)($2)
	li	$2,11			# 0xb
$L29:
	bne	$16,$2,$L30
	li	$2,12			# 0xc

	lui	$20,%hi(state)
	lw	$2,%lo(state)($20)
	bne	$2,$0,$L30
	li	$2,12			# 0xc

	lui	$5,%hi($LC1)
	li	$6,4			# 0x4
	lw	$4,24($19)
	jal	memcmp
	addiu	$5,$5,%lo($LC1)

	bne	$2,$0,$L30
	li	$2,12			# 0xc

	lw	$3,24($19)
	lui	$2,%hi(l2tpInfo)
	lui	$4,%hi(l2tpInfo+24)
	li	$6,6			# 0x6
	sw	$3,%lo(l2tpInfo)($2)
	addiu	$4,$4,%lo(l2tpInfo+24)
	lw	$5,172($19)
	jal	memcpy
	addiu	$5,$5,-8

	lui	$4,%hi(l2tpInfo+30)
	lw	$5,172($19)
	addiu	$4,$4,%lo(l2tpInfo+30)
	li	$6,6			# 0x6
	jal	memcpy
	addiu	$5,$5,-14

	lui	$4,%hi(l2tpInfo+36)
	lw	$5,172($19)
	addiu	$4,$4,%lo(l2tpInfo+36)
	li	$6,2			# 0x2
	jal	memcpy
	addiu	$5,$5,-2

	li	$2,1			# 0x1
	sw	$2,%lo(state)($20)
	li	$2,12			# 0xc
$L30:
	bne	$16,$2,$L10
	nop

	lhu	$2,2($17)
	beq	$2,$0,$L10
	lui	$19,%hi(state)

	lw	$2,%lo(state)($19)
	sltu	$2,$2,2
	beq	$2,$0,$L10
	lui	$16,%hi(l2tpInfo)

	lhu	$2,0($17)
	addiu	$16,$16,%lo(l2tpInfo)
	sh	$2,20($16)
	lhu	$2,2($17)
	sh	$2,22($16)
	lw	$2,12($18)
	sw	$2,16($16)
	lw	$2,16($18)
	move	$4,$2
	jal	filter_addconnect
	sw	$2,12($16)

	li	$2,1
	lui	$4,%hi($LC0)
	li	$5,4			# 0x4
	addiu	$4,$4,%lo($LC0)
	jal	rtl865x_setNetifType
	sh	$2,38($16)

	li	$2,2			# 0x2
	j	$L10
	sw	$2,%lo(state)($19)

$L13:
	andi	$3,$6,0xffff
	li	$2,1701			# 0x6a5
$L28:
	bne	$3,$2,$L10
	nop

	lhu	$2,2($4)
	bne	$2,$3,$L10
	addiu	$3,$5,4

	lhu	$2,0($5)
	addiu	$17,$5,2
	andi	$2,$2,0x4000
	movn	$17,$3,$2		#conditional move  
	li	$2,49185			# 0xc021
	lhu	$3,6($17)
	bne	$3,$2,$L10
	addiu	$4,$17,6

	lbu	$3,2($4)
	li	$2,6			# 0x6
	bne	$3,$2,$L10
	lui	$2,%hi(l2tpInfo)

	addiu	$4,$2,%lo(l2tpInfo)
	lhu	$3,20($4)
	lhu	$2,0($17)
	bne	$3,$2,$L10
	nop

	lhu	$3,2($17)
	lhu	$2,22($4)
	bne	$2,$3,$L10
	move	$5,$0

	jal	memset
	li	$6,40			# 0x28

	lui	$2,%hi(state)
	sw	$0,%lo(state)($2)
$L10:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	l2tp_tx_id
	.section	.text.fast_l2tp_rx,"ax",@progbits
	.align	2
	.globl	fast_l2tp_rx
	.ent	fast_l2tp_rx
	.type	fast_l2tp_rx, @function
fast_l2tp_rx:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$19,28($sp)
	move	$19,$4
	sw	$31,36($sp)
	sw	$20,32($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lw	$18,152($4)
	lw	$3,84($4)
	lbu	$2,0($18)
	sltu	$3,$3,40
	andi	$2,$2,0xf
	sll	$2,$2,2
	bne	$3,$0,$L32
	addu	$5,$18,$2

	lhu	$3,0($5)
	li	$2,1701			# 0x6a5
	bne	$3,$2,$L32
	nop

	lhu	$2,2($5)
	bne	$2,$3,$L32
	lui	$20,%hi(l2tpInfo)

	lw	$2,%lo(l2tpInfo)($20)
	bne	$2,$0,$L37
	nop

	jal	l2tp_tx_id
	nop

$L37:
	lw	$5,172($19)
	addiu	$17,$20,%lo(l2tpInfo)
	lhu	$2,20($17)
	lbu	$3,28($5)
	srl	$4,$3,5
	andi	$3,$3,0x2
	andi	$16,$4,0x2
	beq	$2,$0,$L32
	addu	$16,$16,$3

	lhu	$2,22($17)
	beq	$2,$0,$L32
	addu	$3,$5,$16

	lbu	$2,36($3)
	bne	$2,$0,$L32
	li	$2,33			# 0x21

	lbu	$3,37($3)
	bne	$3,$2,$L32
	nop

	lw	$2,%lo(l2tpInfo)($20)
	beq	$2,$0,$L32
	nop

	lw	$3,16($17)
	lw	$2,16($18)
	bne	$3,$2,$L32
	nop

	lw	$3,12($18)
	lw	$2,12($17)
	bne	$2,$3,$L32
	nop

	lw	$2,4($17)
	bne	$2,$0,$L41
	lui	$4,%hi(init_net)

	lui	$5,%hi($LC0)
	addiu	$4,$4,%lo(init_net)
	jal	__dev_get_by_name
	addiu	$5,$5,%lo($LC0)

	sw	$2,4($17)
$L41:
	lw	$4,4($17)
	beq	$4,$0,$L32
	nop

	lw	$2,172($19)
	addu	$2,$2,$16
	lbu	$3,47($2)
	li	$2,6			# 0x6
	beq	$3,$2,$L44
	li	$2,17			# 0x11

	bne	$3,$2,$L32
	nop

$L44:
	sw	$4,24($19)
	addiu	$5,$16,38
	jal	skb_pull
	move	$4,$19

	lw	$2,172($19)
	sw	$2,148($19)
	sw	$2,152($19)
	lw	$4,4($17)
	beq	$4,$0,$L32
	nop

	lw	$16,180($4)
	beq	$16,$0,$L32
	nop

	jal	get_ppp_stats
	move	$4,$16

	move	$4,$16
	lw	$3,0($2)
	addiu	$3,$3,1
	jal	get_ppp_stats
	sw	$3,0($2)

	lw	$3,8($2)
	lw	$4,84($19)
	addu	$3,$3,$4
	sw	$3,8($2)
$L32:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	fast_l2tp_rx
	.section	.text.get_fast_l2tp_lastxmit,"ax",@progbits
	.align	2
	.globl	get_fast_l2tp_lastxmit
	.ent	get_fast_l2tp_lastxmit
	.type	get_fast_l2tp_lastxmit, @function
get_fast_l2tp_lastxmit:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(l2tpInfo)
	addiu	$5,$2,%lo(l2tpInfo)
	li	$2,1			# 0x1
	lhu	$3,38($5)
	bne	$3,$2,$L46
	move	$4,$0

	lw	$4,8($5)
$L46:
	j	$31
	move	$2,$4

	.set	macro
	.set	reorder
	.end	get_fast_l2tp_lastxmit
	.section	.text.check_for_fast_l2tp_to_wan,"ax",@progbits
	.align	2
	.globl	check_for_fast_l2tp_to_wan
	.ent	check_for_fast_l2tp_to_wan
	.type	check_for_fast_l2tp_to_wan, @function
check_for_fast_l2tp_to_wan:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lw	$4,152($4)
	li	$2,1			# 0x1
	lbu	$3,9($4)
	beq	$3,$2,$L48
	move	$5,$0

	lw	$2,12($4)
	li	$3,-256			# 0xffffff00
	and	$2,$2,$3
	li	$3,171966464			# 0xa400000
	ori	$3,$3,0x4000
	xor	$2,$2,$3
	sltu	$5,$0,$2
$L48:
	j	$31
	move	$2,$5

	.set	macro
	.set	reorder
	.end	check_for_fast_l2tp_to_wan
	.rdata
	.align	2
	.type	__func__.0, @object
	.size	__func__.0, 17
__func__.0:
	.ascii	"fast_l2tp_to_wan\000"
	.section	.text.fast_l2tp_to_wan,"ax",@progbits
	.align	2
	.globl	fast_l2tp_to_wan
	.ent	fast_l2tp_to_wan
	.type	fast_l2tp_to_wan, @function
fast_l2tp_to_wan:
	.set	nomips16
	.frame	$sp,88,$31		# vars= 48, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-88
	lui	$2,%hi(fast_l2tp_fw)
	sw	$17,68($sp)
	sw	$31,80($sp)
	sw	$19,76($sp)
	sw	$18,72($sp)
	sw	$16,64($sp)
	lw	$2,%lo(fast_l2tp_fw)($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L57
	move	$17,$4
	.set	macro
	.set	reorder

	lui	$18,%hi(l2tpInfo)
	addiu	$3,$18,%lo(l2tpInfo)
	lhu	$2,20($3)
	beq	$2,$0,$L57
	lhu	$2,22($3)
	beq	$2,$0,$L57
	lw	$2,%lo(l2tpInfo)($18)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L56
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L57:
	.set	noreorder
	.set	nomacro
	j	$L55
	move	$3,$0
	.set	macro
	.set	reorder

$L56:
	lhu	$4,38($3)
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L55
	move	$3,$0
	.set	macro
	.set	reorder

	lw	$2,172($17)
	lw	$3,168($17)
	lw	$16,152($17)
	subu	$2,$2,$3
	sltu	$2,$2,52
	lhu	$19,2($16)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L62
	li	$3,1073741824			# 0x40000000
	.set	macro
	.set	reorder

	lw	$2,104($17)
	and	$2,$2,$3
	beq	$2,$0,$L78
	lw	$2,164($17)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	.set	noreorder
	.set	nomacro
	bne	$2,$4,$L80
	move	$4,$17
	.set	macro
	.set	reorder

$L78:
	lw	$2,180($17)
	xori	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L81
	move	$4,$17
	.set	macro
	.set	reorder

$L62:
	move	$4,$17
$L80:
	.set	noreorder
	.set	nomacro
	jal	skb_realloc_headroom
	li	$5,52			# 0x34
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L55
	move	$3,$0
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	consume_skb
	move	$17,$2
	.set	macro
	.set	reorder

	move	$4,$17
$L81:
	lbu	$16,1($16)
	.set	noreorder
	.set	nomacro
	jal	skb_push
	li	$5,52			# 0x34
	.set	macro
	.set	reorder

	lui	$5,%hi(l2tpInfo+24)
	move	$4,$2
	addiu	$5,$5,%lo(l2tpInfo+24)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,14			# 0xe
	.set	macro
	.set	reorder

	li	$2,268369920			# 0xfff0000
	lw	$3,16($sp)
	ori	$2,$2,0xffff
	addiu	$4,$18,%lo(l2tpInfo)
	addiu	$8,$sp,16
	and	$3,$3,$2
	li	$2,1073741824			# 0x40000000
	or	$3,$3,$2
	li	$2,-251723776			# 0xf0ff0000
	ori	$2,$2,0xffff
	addiu	$6,$sp,32
	and	$3,$3,$2
	li	$2,83886080			# 0x5000000
	or	$3,$3,$2
	li	$2,16384
	lw	$5,16($4)
	lw	$4,12($4)
	sh	$2,22($sp)
	li	$2,17
	sb	$2,25($sp)
	li	$2,64
	sw	$3,16($sp)
	sw	$5,28($sp)
	sb	$2,24($sp)
	li	$2,-805371904			# 0xcfff0000
	sb	$16,17($sp)
	ori	$2,$2,0xffff
	sw	$4,32($sp)
	lw	$3,104($17)
	lw	$4,84($17)
	and	$3,$3,$2
	addiu	$4,$4,-14
	sw	$3,104($17)
	sh	$4,18($sp)
	lw	$3,16($sp)
	sh	$0,20($sp)
	lw	$4,20($sp)
	srl	$2,$3,22
	andi	$2,$2,0x3c
	addu	$5,$3,$4
	addu	$7,$8,$2
	sh	$0,26($sp)
	sltu	$4,$5,$4
	lw	$2,24($sp)
	addu	$5,$5,$4
	lw	$3,28($sp)
	addu	$5,$5,$2
	sltu	$4,$5,$2
	addu	$5,$5,$4
	addu	$5,$5,$3
	sltu	$4,$5,$3
	addu	$5,$5,$4
$L70:
	lw	$2,0($6)
	addiu	$6,$6,4
	addu	$5,$5,$2
	sltu	$4,$5,$2
	.set	noreorder
	.set	nomacro
	bne	$6,$7,$L70
	addu	$5,$5,$4
	.set	macro
	.set	reorder

#APP
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $5, 16	
	addu	$5, $1		
	sltu	$1, $5, $1	
	srl	$5, $5, 16	
	addu	$5, $1		
	xori	$5, 0xffff	
	.set	pop
#NO_APP
	sh	$5,10($8)
	li	$6,20			# 0x14
	lw	$4,172($17)
	addiu	$5,$sp,16
	addiu	$16,$18,%lo(l2tpInfo)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,14
	.set	macro
	.set	reorder

	addiu	$2,$19,18
	lhu	$4,20($16)
	li	$7,1701
	lhu	$3,22($16)
	addiu	$5,$sp,40
	sh	$2,44($sp)
	li	$2,2
	sh	$2,48($sp)
	li	$2,-253
	sh	$2,54($sp)
	li	$2,33
	sh	$7,42($sp)
	li	$6,18			# 0x12
	sh	$4,50($sp)
	sh	$3,52($sp)
	sh	$2,56($sp)
	sh	$7,40($sp)
	sh	$0,46($sp)
	lw	$4,172($17)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,34
	.set	macro
	.set	reorder

	lw	$2,%lo(l2tpInfo)($18)
	sw	$2,24($17)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	lw	$4,4($16)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L75
	sw	$2,8($16)
	.set	macro
	.set	reorder

	lw	$16,180($4)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L82
	lui	$2,%hi(gQosEnabled)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	get_ppp_stats
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$16
	lw	$3,4($2)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	get_ppp_stats
	sw	$3,4($2)
	.set	macro
	.set	reorder

	lw	$3,12($2)
	lw	$4,84($17)
	addu	$3,$3,$4
	sw	$3,12($2)
$L75:
	lui	$2,%hi(gQosEnabled)
$L82:
	lw	$2,%lo(gQosEnabled)($2)
	beq	$2,$0,$L76
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L55
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

$L76:
	lw	$5,24($17)
	lw	$2,184($5)
	lw	$2,16($2)
	.set	noreorder
	.set	nomacro
	jal	$2
	move	$4,$17
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
$L55:
	lw	$31,80($sp)
	move	$2,$3
	lw	$19,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,88
	.set	macro
	.set	reorder

	.end	fast_l2tp_to_wan
	.rdata
	.align	2
$LC3:
	.ascii	"%c\n\000"
	.section	.text.l2tp_read_proc,"ax",@progbits
	.align	2
	.ent	l2tp_read_proc
	.type	l2tp_read_proc, @function
l2tp_read_proc:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	lui	$2,%hi(fast_l2tp_fw)
	sw	$19,28($sp)
	move	$19,$5
	sw	$18,24($sp)
	lui	$5,%hi($LC3)
	sw	$17,20($sp)
	addiu	$5,$5,%lo($LC3)
	sw	$16,16($sp)
	move	$17,$6
	sw	$31,32($sp)
	move	$16,$7
	lw	$2,%lo(fast_l2tp_fw)($2)
	move	$18,$4
	addu	$18,$18,$17
	jal	sprintf
	addiu	$6,$2,48

	move	$4,$2
	addu	$2,$17,$16
	slt	$2,$2,$4
	subu	$4,$4,$17
	slt	$3,$16,$4
	movn	$4,$16,$3		#conditional move
	bne	$2,$0,$L84
	slt	$5,$4,0

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L84:
	sw	$18,0($19)
	move	$2,$0
	lw	$31,32($sp)
	movz	$2,$4,$5		#conditional move  
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	l2tp_read_proc
	.section	.text.l2tp_write_proc,"ax",@progbits
	.align	2
	.ent	l2tp_write_proc
	.type	l2tp_write_proc, @function
l2tp_write_proc:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sltu	$2,$6,2
	sw	$16,24($sp)
	li	$3,-14			# 0xfffffff2
	sw	$31,28($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L87
	move	$16,$6
	.set	macro
	.set	reorder

	addiu	$2,$5,1
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L89
	or	$3,$5,$2
	.set	macro
	.set	reorder

	lw	$2,24($28)
	addiu	$4,$sp,16
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L89
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

#APP
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L87
	li	$3,-14			# 0xfffffff2
	.set	macro
	.set	reorder

	lb	$2,16($sp)
	lui	$3,%hi(fast_l2tp_fw)
	addiu	$2,$2,-48
	sw	$2,%lo(fast_l2tp_fw)($3)
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L97
	lui	$4,%hi(l2tp_tx_id_hook)
	.set	macro
	.set	reorder

	lui	$2,%hi(l2tp_tx_id)
	addiu	$2,$2,%lo(l2tp_tx_id)
	.set	noreorder
	.set	nomacro
	j	$L98
	sw	$2,%lo(l2tp_tx_id_hook)($4)
	.set	macro
	.set	reorder

$L97:
	lui	$2,%hi(l2tpInfo+20)
	sw	$0,%lo(l2tp_tx_id_hook)($4)
	sh	$0,%lo(l2tpInfo+20)($2)
$L98:
	lui	$4,%hi(l2tpInfo)
	move	$5,$0
	li	$6,40			# 0x28
	.set	noreorder
	.set	nomacro
	jal	memset
	addiu	$4,$4,%lo(l2tpInfo)
	.set	macro
	.set	reorder

	lui	$2,%hi(state)
	move	$3,$16
	.set	noreorder
	.set	nomacro
	j	$L87
	sw	$0,%lo(state)($2)
	.set	macro
	.set	reorder

$L89:
	li	$3,-14			# 0xfffffff2
$L87:
	lw	$31,28($sp)
	move	$2,$3
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	l2tp_write_proc
	.rdata
	.align	2
$LC4:
	.ascii	"fast_l2tp\000"
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	fast_l2tp_init
	.ent	fast_l2tp_init
	.type	fast_l2tp_init, @function
fast_l2tp_init:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$4,%hi($LC4)
	addiu	$sp,$sp,-24
	move	$5,$0
	addiu	$4,$4,%lo($LC4)
	sw	$31,16($sp)
	jal	create_proc_entry
	move	$6,$0

	lui	$4,%hi(l2tpInfo)
	lui	$7,%hi(res1)
	move	$3,$2
	addiu	$4,$4,%lo(l2tpInfo)
	move	$5,$0
	li	$6,40			# 0x28
	beq	$2,$0,$L100
	sw	$2,%lo(res1)($7)

	lui	$2,%hi(l2tp_read_proc)
	addiu	$2,$2,%lo(l2tp_read_proc)
	sw	$2,64($3)
	lui	$2,%hi(l2tp_write_proc)
	lw	$3,%lo(res1)($7)
	addiu	$2,$2,%lo(l2tp_write_proc)
	sw	$2,68($3)
$L100:
	lui	$2,%hi(l2tp_tx_id_hook)
	sw	$0,%lo(l2tp_tx_id_hook)($2)
	lui	$2,%hi(fast_l2tp_fw)
	sw	$0,%lo(fast_l2tp_fw)($2)
	lui	$2,%hi(state)
	jal	memset
	sw	$0,%lo(state)($2)

	move	$2,$0
	lw	$31,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fast_l2tp_init
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	fast_l2tp_exit
	.ent	fast_l2tp_exit
	.type	fast_l2tp_exit, @function
fast_l2tp_exit:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	lui	$4,%hi($LC4)
	sw	$16,16($sp)
	lui	$16,%hi(res1)
	lw	$2,%lo(res1)($16)
	addiu	$4,$4,%lo($LC4)
	sw	$31,20($sp)
	beq	$2,$0,$L101
	move	$5,$2

	jal	remove_proc_entry
	nop

	sw	$0,%lo(res1)($16)
$L101:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fast_l2tp_exit
	.globl	l2tp_tx_id_hook
	.section	.bss
	.align	2
	.type	l2tp_tx_id_hook, @object
	.size	l2tp_tx_id_hook, 4
l2tp_tx_id_hook:
	.space	4
	.globl	fast_l2tp_fw
	.align	2
	.type	fast_l2tp_fw, @object
	.size	fast_l2tp_fw, 4
fast_l2tp_fw:
	.space	4
	.globl	state
	.align	2
	.type	state, @object
	.size	state, 4
state:
	.space	4
	.ident	"GCC: (GNU) 3.4.6-1.3.6"
