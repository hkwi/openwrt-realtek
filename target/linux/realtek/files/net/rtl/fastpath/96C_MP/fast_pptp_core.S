	.file	1 "fast_pptp_core.c"
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
	.globl	fast_pptp_fw
	.data
	.align	2
	.type	fast_pptp_fw, @object
	.size	fast_pptp_fw, 4
fast_pptp_fw:
	.word	0
	.globl	pptp_tcp_finished
	.align	2
	.type	pptp_tcp_finished, @object
	.size	pptp_tcp_finished, 4
pptp_tcp_finished:
	.word	0
	.align	2
	.type	res_check_pptp, @object
	.size	res_check_pptp, 4
res_check_pptp:
	.word	0
	.align	2
	.type	res1, @object
	.size	res1, 4
res1:
	.word	0
	.align	2
	.type	pptp_conn_check, @object
	.size	pptp_conn_check, 4
pptp_conn_check:
	.word	0
	.section	.text.is_pptp_device,"ax",@progbits
	.align	2
	.globl	is_pptp_device
	.ent	is_pptp_device
	.type	is_pptp_device, @function
is_pptp_device:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lui	$2,%hi(pptpAccInfo+56)
	lw	$2,%lo(pptpAccInfo+56)($2)
	#nop
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
	nop
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

	.end	is_pptp_device
	.section	.text.set_pptp_device,"ax",@progbits
	.align	2
	.globl	set_pptp_device
	.ent	set_pptp_device
	.type	set_pptp_device, @function
set_pptp_device:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,16($sp)
	lui	$2,%hi(fast_pptp_fw)
	lw	$2,%lo(fast_pptp_fw)($2)
	move	$5,$4
	lui	$4,%hi(init_net)
	beq	$2,$0,$L4
	addiu	$4,$4,%lo(init_net)

	jal	__dev_get_by_name
	nop

	lui	$3,%hi(pptpAccInfo)
	addiu	$3,$3,%lo(pptpAccInfo)
	sw	$2,56($3)
$L4:
	lw	$31,16($sp)
	nop
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	set_pptp_device
	.section	.text.Check_GRE_rx_net_device,"ax",@progbits
	.align	2
	.globl	Check_GRE_rx_net_device
	.ent	Check_GRE_rx_net_device
	.type	Check_GRE_rx_net_device, @function
Check_GRE_rx_net_device:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lw	$3,152($4)
	lui	$2,%hi(pptpAccInfo)
	addiu	$7,$2,%lo(pptpAccInfo)
	lbu	$2,0($3)
	lw	$6,8($7)
	lw	$3,172($4)
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$3,$3,$2
	beq	$6,$0,$L6
	move	$5,$0

	lw	$2,24($4)
	nop
	bne	$6,$2,$L10
	li	$5,1			# 0x1

	lhu	$3,6($3)
	lhu	$2,30($7)
	nop
	beq	$3,$2,$L6
	nop

$L10:
	move	$5,$0
$L6:
	j	$31
	move	$2,$5

	.set	macro
	.set	reorder
	.end	Check_GRE_rx_net_device
	.section	.text.fast_pptp_filter,"ax",@progbits
	.align	2
	.globl	fast_pptp_filter
	.ent	fast_pptp_filter
	.type	fast_pptp_filter, @function
fast_pptp_filter:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$18,24($sp)
	sw	$31,28($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lw	$2,84($4)
	lw	$8,152($4)
	sltu	$2,$2,56
	bne	$2,$0,$L13
	move	$18,$4

	lbu	$2,0($8)
	lbu	$3,9($8)
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$4,$8,$2
	li	$2,6			# 0x6
	bne	$3,$2,$L13
	li	$2,1723			# 0x6bb

	lhu	$3,0($4)
	nop
	bne	$3,$2,$L13
	nop

	lw	$2,12($4)
	nop
	srl	$2,$2,28
	sll	$2,$2,2
	addu	$4,$4,$2
	lhu	$7,2($4)
	li	$2,1			# 0x1
	addiu	$9,$4,8
	andi	$5,$7,0xffff
	bne	$5,$2,$L18
	addiu	$6,$4,12

	lhu	$3,8($4)
	li	$2,8			# 0x8
	bne	$3,$2,$L18
	nop

	lbu	$2,4($6)
	nop
	bne	$2,$5,$L18
	lui	$16,%hi(pptpAccInfo)

	addiu	$5,$16,%lo(pptpAccInfo)
	lw	$2,36($5)
	nop
	beq	$2,$0,$L19
	nop

	lw	$2,20($5)
	nop
	beq	$2,$0,$L19
	nop

	lw	$2,32($5)
	nop
	beq	$2,$0,$L19
	nop

	lhu	$3,12($4)
	lhu	$2,18($5)
	nop
	bne	$2,$3,$L13
	nop

	lhu	$3,30($5)
	lhu	$2,2($6)
	nop
	bne	$3,$2,$L13
	nop

$L19:
	li	$17,1			# 0x1
	lui	$2,%hi(pptp_tcp_finished)
	sw	$17,%lo(pptp_tcp_finished)($2)
	lhu	$2,0($6)
	addiu	$16,$16,%lo(pptpAccInfo)
	lui	$4,%hi(pptpAccInfo+24)
	sh	$2,18($16)
	lhu	$2,2($6)
	addiu	$4,$4,%lo(pptpAccInfo+24)
	li	$6,6			# 0x6
	sh	$2,30($16)
	lw	$2,16($8)
	nop
	sw	$2,20($16)
	lw	$2,12($8)
	sw	$17,36($16)
	sw	$2,32($16)
	lw	$2,24($18)
	nop
	sw	$2,8($16)
	lw	$5,172($18)
	jal	memcpy
	addiu	$5,$5,-8

	lw	$5,172($18)
	lui	$4,%hi(pptpAccInfo+12)
	addiu	$4,$4,%lo(pptpAccInfo+12)
	addiu	$5,$5,-14
	jal	memcpy
	li	$6,6			# 0x6

	j	$L13
	sw	$17,4($16)

$L18:
	andi	$3,$7,0xffff
	li	$2,1			# 0x1
	bne	$3,$2,$L13
	li	$2,13			# 0xd

	lhu	$3,0($9)
	nop
	bne	$3,$2,$L13
	lui	$4,%hi(pptpAccInfo)

	addiu	$3,$4,%lo(pptpAccInfo)
	lw	$2,36($3)
	nop
	beq	$2,$0,$L33
	lui	$2,%hi(pptp_tcp_finished)

	lw	$2,20($3)
	nop
	beq	$2,$0,$L33
	lui	$2,%hi(pptp_tcp_finished)

	lw	$2,32($3)
	nop
	beq	$2,$0,$L33
	lui	$2,%hi(pptp_tcp_finished)

	lhu	$3,18($3)
	lhu	$2,4($9)
	nop
	bne	$3,$2,$L13
	addiu	$3,$4,%lo(pptpAccInfo)

	lui	$2,%hi(pptp_tcp_finished)
$L33:
	sw	$0,%lo(pptp_tcp_finished)($2)
	sw	$0,4($3)
$L13:
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	fast_pptp_filter
	.section	.text.fast_pptp_to_lan,"ax",@progbits
	.align	2
	.globl	fast_pptp_to_lan
	.ent	fast_pptp_to_lan
	.type	fast_pptp_to_lan, @function
fast_pptp_to_lan:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$31,36($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lw	$16,0($4)
	lui	$9,%hi(pptpAccInfo)
	move	$20,$4
	addiu	$4,$9,%lo(pptpAccInfo)
	lw	$3,24($16)
	lw	$2,8($4)
	nop
	bne	$3,$2,$L41
	move	$19,$0

	lw	$7,152($16)
	li	$2,47			# 0x2f
	lbu	$3,9($7)
	nop
	bne	$3,$2,$L34
	move	$8,$0

	lw	$2,84($16)
	nop
	sltu	$2,$2,20
	bne	$2,$0,$L34
	nop

	lbu	$2,0($7)
	lw	$3,172($16)
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$5,$3,$2
	lbu	$2,1($5)
	li	$3,1			# 0x1
	andi	$2,$2,0x7
	bne	$2,$3,$L34
	li	$2,34827			# 0x880b

	lhu	$3,2($5)
	nop
	bne	$3,$2,$L34
	li	$17,8			# 0x8

	lhu	$3,6($5)
	lhu	$2,30($4)
	nop
	bne	$3,$2,$L34
	move	$6,$0

	lbu	$2,0($5)
	nop
	andi	$2,$2,0x10
	beq	$2,$0,$L43
	nop

	lw	$2,8($5)
	li	$17,12			# 0xc
	sw	$2,40($4)
$L43:
	lb	$2,1($5)
	lhu	$4,4($5)
	addiu	$3,$17,4
	slt	$2,$2,0
	movn	$17,$3,$2		#RLX4181/RLX4281:conditional move  
	beq	$4,$0,$L41
	addu	$5,$5,$17

	lbu	$3,0($5)
	nop
	bne	$3,$0,$L46
	li	$2,255			# 0xff

	addiu	$5,$5,1
	j	$L47
	li	$6,1			# 0x1

$L46:
	bne	$3,$2,$L47
	li	$2,3			# 0x3

	lbu	$3,1($5)
	nop
	bne	$3,$2,$L47
	nop

	addiu	$5,$5,2
	lbu	$2,0($5)
	nop
	bne	$2,$0,$L47
	li	$6,2			# 0x2

	addiu	$5,$5,1
	li	$6,3			# 0x3
$L47:
	lbu	$3,0($5)
	li	$2,33			# 0x21
	beq	$3,$2,$L51
	li	$2,253			# 0xfd

	bne	$3,$2,$L34
	move	$8,$0

$L51:
	addiu	$6,$6,1
	beq	$6,$0,$L41
	move	$18,$3

	lbu	$2,0($7)
	nop
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$2,$2,$17
	addu	$17,$2,$6
	li	$2,33			# 0x21
	beq	$3,$2,$L54
	move	$5,$17

	addiu	$19,$9,%lo(pptpAccInfo)
	lw	$2,56($19)
	nop
	beq	$2,$0,$L34
	move	$8,$0

	lw	$2,180($2)
	nop
	beq	$2,$0,$L34
	move	$4,$16

	jal	skb_pull
	addiu	$5,$17,-2

	lw	$2,172($16)
	move	$5,$16
	sb	$0,0($2)
	lw	$2,172($16)
	nop
	sb	$18,1($2)
	lw	$2,56($19)
	nop
	lw	$4,180($2)
	jal	ppp_receive_nonmp_frame
	li	$6,1			# 0x1

	move	$16,$2
	beq	$2,$0,$L34
	li	$8,-2			# 0xfffffffffffffffe

	li	$19,1			# 0x1
	j	$L58
	sw	$2,0($20)

$L54:
	jal	skb_pull
	move	$4,$16

$L58:
	lw	$4,172($16)
	nop
	andi	$17,$4,0x3
	beq	$17,$0,$L59
	nop

	lw	$2,168($16)
	nop
	subu	$2,$4,$2
	sltu	$2,$2,$17
	bne	$2,$0,$L59
	move	$5,$4

	lw	$6,84($16)
	jal	memmove
	subu	$4,$4,$17

	lw	$2,172($16)
	lw	$3,160($16)
	subu	$2,$2,$17
	subu	$3,$3,$17
	sw	$2,172($16)
	sw	$3,160($16)
$L59:
	lw	$2,172($16)
	lui	$5,%hi(pptpAccInfo)
	addiu	$3,$5,%lo(pptpAccInfo)
	sw	$2,148($16)
	sw	$2,152($16)
	lw	$2,56($3)
	nop
	beq	$2,$0,$L34
	move	$8,$0

	bne	$19,$0,$L63
	nop

	lw	$4,180($2)
	nop
	beq	$4,$0,$L68
	addiu	$2,$5,%lo(pptpAccInfo)

	lw	$2,156($4)
	lw	$3,164($4)
	addiu	$2,$2,1
	sw	$2,156($4)
	lw	$2,84($16)
	nop
	addu	$3,$3,$2
	addiu	$3,$3,-2
	sw	$3,164($4)
$L63:
	addiu	$2,$5,%lo(pptpAccInfo)
$L68:
	lw	$2,56($2)
	li	$8,1			# 0x1
	j	$L34
	sw	$2,24($16)

$L41:
	move	$8,$0
$L34:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	move	$2,$8
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	fast_pptp_to_lan
	.section	.text.get_fastpptp_lastxmit,"ax",@progbits
	.align	2
	.globl	get_fastpptp_lastxmit
	.ent	get_fastpptp_lastxmit
	.type	get_fastpptp_lastxmit, @function
get_fastpptp_lastxmit:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(fast_pptp_fw)
	lw	$2,%lo(fast_pptp_fw)($2)
	lui	$5,%hi(pptpAccInfo)
	addiu	$3,$5,%lo(pptpAccInfo)
	beq	$2,$0,$L69
	move	$4,$0

	lw	$3,4($3)
	li	$2,1			# 0x1
	bne	$3,$2,$L69
	nop

	lw	$4,%lo(pptpAccInfo)($5)
$L69:
	j	$31
	move	$2,$4

	.set	macro
	.set	reorder
	.end	get_fastpptp_lastxmit
	.rdata
	.align	2
	.type	__func__.0, @object
	.size	__func__.0, 17
__func__.0:
	.ascii	"fast_pptp_to_wan\000"
	.align	2
$LC0:
	.ascii	"ppp0\000"
	.align	2
$LC2:
	.ascii	"RTL\000"
	.section	.text.fast_pptp_to_wan,"ax",@progbits
	.align	2
	.globl	fast_pptp_to_wan
	.ent	fast_pptp_to_wan
	.type	fast_pptp_to_wan, @function
fast_pptp_to_wan:
	.set	nomips16
	.frame	$sp,80,$31		# vars= 40, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-80
	sw	$17,60($sp)
	sw	$31,76($sp)
	sw	$20,72($sp)
	sw	$19,68($sp)
	sw	$18,64($sp)
	sw	$16,56($sp)
	lw	$7,152($4)
	li	$2,16711680			# 0xff0000
	move	$17,$4
	lw	$3,12($7)
	li	$4,-16777216			# 0xffffffffff000000
	and	$5,$3,$2
	and	$4,$3,$4
	li	$2,167772160			# 0xa000000
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L78
	andi	$3,$3,0xff00
	.set	macro
	.set	reorder

	li	$2,4194304			# 0x400000
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L78
	li	$2,16384			# 0x4000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L112
	lui	$19,%hi(pptpAccInfo)
	.set	macro
	.set	reorder

	lui	$2,%hi(pptp_conn_check)
	lw	$3,%lo(pptp_conn_check)($2)
	li	$2,3			# 0x3
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L71
	move	$4,$0
	.set	macro
	.set	reorder

$L78:
	lui	$19,%hi(pptpAccInfo)
$L112:
	addiu	$6,$19,%lo(pptpAccInfo)
	lw	$5,56($6)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L71
	move	$4,$0
	.set	macro
	.set	reorder

	lui	$3,%hi($LC0)
	addiu	$2,$3,%lo($LC0)
	move	$4,$5
#APP
	.set	noreorder
	.set	noat
	lbu	$3,($4)
1:	lbu	$1,($2)
	addiu	$4,1
	bne	$1,$3,2f
	addiu	$2,1
	bnez	$3,1b
	lbu	$3,($4)
	nop
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L71
	move	$4,$0
	.set	macro
	.set	reorder

	lw	$2,4($6)
	#nop
	beq	$2,$0,$L71
	lw	$16,180($5)
	#nop
	beq	$16,$0,$L71
	lw	$2,92($16)
	#nop
	beq	$2,$0,$L83
	lw	$2,80($16)
	#nop
	andi	$2,$2,0x1000
	beq	$2,$0,$L71
	lw	$2,128($16)
	#nop
	beq	$2,$0,$L82
$L83:
	lw	$2,120($16)
	lbu	$20,1($7)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L71
	move	$4,$0
	.set	macro
	.set	reorder

	lui	$5,%hi($LC2)
	addiu	$5,$5,%lo($LC2)
	addiu	$4,$17,32
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,3			# 0x3
	.set	macro
	.set	reorder

	addiu	$2,$19,%lo(pptpAccInfo)
	lw	$5,56($2)
	.set	noreorder
	.set	nomacro
	jal	ppp_start_xmit
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$18,120($16)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L71
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

	sw	$0,120($16)
	lw	$2,172($18)
	lw	$3,168($18)
	#nop
	subu	$2,$2,$3
	sltu	$2,$2,50
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L92
	move	$17,$18
	.set	macro
	.set	reorder

	lw	$2,104($18)
	li	$3,1073741824			# 0x40000000
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L111
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	lw	$2,164($18)
	#nop
	lw	$2,0($2)
	#nop
	andi	$2,$2,0xffff
	bne	$2,$3,$L92
$L111:
	lw	$2,180($17)
	#nop
	xori	$2,$2,0x1
	beq	$2,$0,$L91
$L92:
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	skb_realloc_headroom
	li	$5,50			# 0x32
	.set	macro
	.set	reorder

	move	$16,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L71
	move	$4,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	consume_skb
	move	$4,$18
	.set	macro
	.set	reorder

	move	$17,$16
$L91:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	skb_push
	li	$5,50			# 0x32
	.set	macro
	.set	reorder

	lui	$5,%hi(pptpAccInfo+24)
	move	$4,$2
	addiu	$5,$5,%lo(pptpAccInfo+24)
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$16,$2
	.set	macro
	.set	reorder

	lui	$5,%hi(pptpAccInfo+12)
	addiu	$5,$5,%lo(pptpAccInfo+12)
	addiu	$4,$16,6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	li	$2,8			# 0x8
	sb	$2,12($16)
	sb	$0,13($16)
	lw	$3,16($sp)
	li	$2,268369920			# 0xfff0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	li	$2,1073741824			# 0x40000000
	or	$3,$3,$2
	li	$2,-251723776			# 0xfffffffff0ff0000
	ori	$2,$2,0xffff
	addiu	$7,$19,%lo(pptpAccInfo)
	and	$3,$3,$2
	li	$2,83886080			# 0x5000000
	lw	$5,32($7)
	lw	$4,20($7)
	or	$3,$3,$2
	li	$2,16384
	sh	$2,22($sp)
	li	$2,47
	sb	$2,25($sp)
	li	$2,64
	sw	$3,16($sp)
	sw	$4,28($sp)
	sb	$2,24($sp)
	sb	$20,17($sp)
	sw	$5,32($sp)
	lw	$3,104($17)
	li	$2,-805371904			# 0xffffffffcfff0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	sw	$3,104($17)
	lw	$2,84($17)
	lhu	$5,44($7)
	sh	$0,26($sp)
	addiu	$2,$2,-14
	addiu	$5,$5,1
	sh	$5,20($sp)
	sh	$2,18($sp)
	lw	$2,16($sp)
	lw	$3,20($sp)
	lw	$4,24($sp)
	lw	$6,28($sp)
	addu	$8,$2,$3
	sltu	$3,$8,$3
	addu	$8,$8,$3
	addu	$8,$8,$4
	sltu	$3,$8,$4
	addu	$8,$8,$3
	addu	$8,$8,$6
	srl	$2,$2,22
	sltu	$3,$8,$6
	andi	$2,$2,0x3c
	addiu	$6,$sp,16
	sh	$5,44($7)
	addu	$8,$8,$3
	addu	$5,$6,$2
	addiu	$4,$sp,32
$L100:
	lw	$2,0($4)
	addiu	$4,$4,4
	addu	$8,$8,$2
	sltu	$3,$8,$2
	.set	noreorder
	.set	nomacro
	bne	$4,$5,$L100
	addu	$8,$8,$3
	.set	macro
	.set	reorder

	lhu	$2,4($6)
	addiu	$16,$19,%lo(pptpAccInfo)
#APP
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $8, 16	
	addu	$8, $1		
	sltu	$1, $8, $1	
	srl	$8, $8, 16	
	addu	$8, $1		
	xori	$8, 0xffff	
	.set	pop
#NO_APP
	sh	$2,46($16)
	sh	$8,10($6)
	lw	$4,172($17)
	addiu	$5,$sp,16
	li	$6,20			# 0x14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,14
	.set	macro
	.set	reorder

	li	$2,48
	sb	$2,40($sp)
	li	$2,-127
	sb	$2,41($sp)
	li	$2,-30709
	sh	$2,42($sp)
	lw	$2,36($16)
	lw	$3,84($17)
	lhu	$5,18($16)
	lw	$6,40($16)
	addiu	$3,$3,-50
	move	$4,$2
	addiu	$2,$2,1
	sh	$3,44($sp)
	sh	$5,46($sp)
	sw	$2,36($16)
	sw	$6,52($sp)
	sw	$4,48($sp)
	lw	$4,172($17)
	addiu	$5,$sp,40
	li	$6,16			# 0x10
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,34
	.set	macro
	.set	reorder

	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	lw	$3,8($16)
	move	$4,$17
	sw	$2,%lo(pptpAccInfo)($19)
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sw	$3,24($17)
	.set	macro
	.set	reorder

	li	$4,1			# 0x1
$L82:
$L71:
	lw	$31,76($sp)
	lw	$20,72($sp)
	lw	$19,68($sp)
	lw	$18,64($sp)
	lw	$17,60($sp)
	lw	$16,56($sp)
	move	$2,$4
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,80
	.set	macro
	.set	reorder

	.end	fast_pptp_to_wan
	.section	.text.fast_pptp_sync_rx_seq,"ax",@progbits
	.align	2
	.globl	fast_pptp_sync_rx_seq
	.ent	fast_pptp_sync_rx_seq
	.type	fast_pptp_sync_rx_seq, @function
fast_pptp_sync_rx_seq:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lw	$5,152($4)
	li	$3,1			# 0x1
	addiu	$4,$5,20
	lbu	$2,1($4)
	nop
	andi	$2,$2,0x7
	bne	$2,$3,$L119
	li	$2,34827			# 0x880b

	lhu	$3,2($4)
	nop
	bne	$3,$2,$L119
	lui	$2,%hi(pptpAccInfo)

	addiu	$6,$2,%lo(pptpAccInfo)
	lbu	$2,20($5)
	nop
	andi	$2,$2,0x10
	beq	$2,$0,$L117
	nop

	lw	$2,52($6)
	nop
	move	$3,$2
	addiu	$2,$2,1
	sw	$2,52($6)
	sw	$3,8($4)
$L117:
	lb	$2,1($4)
	nop
	bgez	$2,$L119
	lui	$2,%hi(pptpAccInfo+48)

	lw	$2,%lo(pptpAccInfo+48)($2)
	nop
	sw	$2,12($4)
$L119:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	fast_pptp_sync_rx_seq
	.section	.text.sync_tx_pptp_gre_seqno,"ax",@progbits
	.align	2
	.globl	sync_tx_pptp_gre_seqno
	.ent	sync_tx_pptp_gre_seqno
	.type	sync_tx_pptp_gre_seqno, @function
sync_tx_pptp_gre_seqno:
	.set	nomips16
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lui	$2,%hi(fast_pptp_fw)
	lw	$5,172($4)
	lw	$2,%lo(fast_pptp_fw)($2)
	addiu	$sp,$sp,-8
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L120
	addiu	$8,$5,14
	.set	macro
	.set	reorder

#APP
	raw_local_irq_save	$10
#NO_APP
	lbu	$3,9($8)
	li	$2,47			# 0x2f
	bne	$3,$2,$L124
	lw	$2,84($4)
	#nop
	sltu	$2,$2,35
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L124
	move	$12,$0
	.set	macro
	.set	reorder

	lbu	$2,14($5)
	lw	$3,172($4)
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$3,$3,$2
	addiu	$7,$3,16
	addiu	$6,$3,14
	move	$9,$sp
	move	$5,$0
$L128:
	addu	$2,$7,$5
	lbu	$4,0($2)
	addu	$3,$9,$5
	addiu	$5,$5,1
	slt	$2,$5,2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L128
	sb	$4,0($3)
	.set	macro
	.set	reorder

	lbu	$2,1($6)
	li	$3,1			# 0x1
	andi	$2,$2,0x7
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L124
	li	$2,34827			# 0x880b
	.set	macro
	.set	reorder

	lhu	$3,0($sp)
	#nop
	bne	$3,$2,$L124
	lbu	$2,0($6)
	#nop
	andi	$2,$2,0x10
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L131
	lui	$11,%hi(pptpAccInfo)
	.set	macro
	.set	reorder

	addiu	$4,$11,%lo(pptpAccInfo)
	lhu	$3,4($8)
	lhu	$2,46($4)
	#nop
	beq	$3,$2,$L132
	lhu	$3,6($6)
	lhu	$2,18($4)
	#nop
	bne	$3,$2,$L132
	lw	$3,32($4)
	lw	$2,16($8)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L132
	lui	$2,%hi(pptpAccInfo+48)
	.set	macro
	.set	reorder

	lw	$3,8($6)
	addiu	$7,$2,%lo(pptpAccInfo+48)
	sw	$3,4($sp)
	addiu	$9,$sp,4
	move	$5,$0
$L139:
	addu	$2,$9,$5
	lbu	$4,0($2)
	addu	$3,$7,$5
	addiu	$5,$5,1
	slt	$2,$5,4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L139
	sb	$4,0($3)
	.set	macro
	.set	reorder

	addiu	$2,$11,%lo(pptpAccInfo)
	lw	$3,36($2)
	addiu	$7,$6,8
	move	$5,$0
	move	$4,$3
	addiu	$3,$3,1
	sw	$3,36($2)
	sw	$4,4($sp)
$L144:
	addu	$2,$9,$5
	lbu	$4,0($2)
	addu	$3,$7,$5
	addiu	$5,$5,1
	slt	$2,$5,4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L144
	sb	$4,0($3)
	.set	macro
	.set	reorder

	j	$L131
$L132:
	li	$12,1			# 0x1
$L131:
	lb	$2,1($6)
	#nop
	.set	noreorder
	.set	nomacro
	bgez	$2,$L147
	lui	$2,%hi(pptpAccInfo)
	.set	macro
	.set	reorder

	addiu	$4,$2,%lo(pptpAccInfo)
	lhu	$3,6($6)
	lhu	$2,18($4)
	#nop
	bne	$3,$2,$L124
	lw	$3,32($4)
	lw	$2,16($8)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L124
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$2,40($4)
	addiu	$6,$6,12
	addiu	$7,$sp,4
	sw	$2,4($sp)
$L154:
	addu	$2,$7,$5
	lbu	$4,0($2)
	addu	$3,$6,$5
	addiu	$5,$5,1
	slt	$2,$5,4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L154
	sb	$4,0($3)
	.set	macro
	.set	reorder

$L147:
	bne	$12,$0,$L124
	lui	$3,%hi(pptpAccInfo)
	addiu	$3,$3,%lo(pptpAccInfo)
	lhu	$2,44($3)
	addiu	$7,$8,16
	addiu	$2,$2,1
	sh	$2,44($3)
	sh	$2,4($8)
	lw	$3,4($8)
	lw	$2,0($8)
	sh	$0,10($8)
	lw	$4,8($8)
	addu	$5,$2,$3
	sltu	$6,$5,$3
	addu	$5,$5,$6
	lw	$3,12($8)
	addu	$5,$5,$4
	sltu	$6,$5,$4
	addu	$5,$5,$6
	addu	$5,$5,$3
	srl	$2,$2,22
	sltu	$6,$5,$3
	andi	$2,$2,0x3c
	addu	$5,$5,$6
	addu	$3,$8,$2
$L157:
	lw	$2,0($7)
	addiu	$7,$7,4
	addu	$5,$5,$2
	sltu	$6,$5,$2
	.set	noreorder
	.set	nomacro
	bne	$7,$3,$L157
	addu	$5,$5,$6
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
$L124:
#APP
	raw_local_irq_restore	$10
#NO_APP
$L120:
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,8
	.set	macro
	.set	reorder

	.end	sync_tx_pptp_gre_seqno
	.rdata
	.align	2
$LC7:
	.ascii	"%d\n\000"
	.section	.text.ppfw_read_proc,"ax",@progbits
	.align	2
	.globl	ppfw_read_proc
	.ent	ppfw_read_proc
	.type	ppfw_read_proc, @function
ppfw_read_proc:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	sw	$31,32($sp)
	lui	$2,%hi(fast_pptp_fw)
	move	$17,$6
	lw	$6,%lo(fast_pptp_fw)($2)
	move	$19,$5
	lui	$5,%hi($LC7)
	addiu	$5,$5,%lo($LC7)
	move	$16,$7
	jal	sprintf
	move	$18,$4

	subu	$4,$2,$17
	addu	$3,$17,$16
	slt	$3,$3,$2
	slt	$2,$16,$4
	movn	$4,$16,$2		#RLX4181/RLX4281:conditional move
	addu	$18,$18,$17
	bne	$3,$0,$L178
	slt	$5,$4,0

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L178:
	sw	$18,0($19)
	lw	$31,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	movz	$2,$4,$5		#RLX4181/RLX4281:conditional move  
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	ppfw_read_proc
	.section	.text.atoi_dec,"ax",@progbits
	.align	2
	.ent	atoi_dec
	.type	atoi_dec, @function
atoi_dec:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lbu	$5,0($4)
	move	$6,$4
	addiu	$2,$5,-48
	andi	$2,$2,0x00ff
	sltu	$2,$2,10
	beq	$2,$0,$L186
	move	$7,$0

$L184:
	addiu	$6,$6,1
	sll	$2,$5,24
	lbu	$5,0($6)
	sll	$3,$7,2
	addu	$3,$3,$7
	addiu	$4,$5,-48
	sra	$2,$2,24
	sll	$3,$3,1
	andi	$4,$4,0x00ff
	addu	$3,$3,$2
	sltu	$4,$4,10
	bne	$4,$0,$L184
	addiu	$7,$3,-48

$L186:
	j	$31
	move	$2,$7

	.set	macro
	.set	reorder
	.end	atoi_dec
	.section	.text.pptpconn_read_proc,"ax",@progbits
	.align	2
	.globl	pptpconn_read_proc
	.ent	pptpconn_read_proc
	.type	pptpconn_read_proc, @function
pptpconn_read_proc:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	sw	$31,32($sp)
	lui	$2,%hi(pptp_conn_check)
	move	$17,$6
	lw	$6,%lo(pptp_conn_check)($2)
	move	$19,$5
	lui	$5,%hi($LC7)
	addiu	$5,$5,%lo($LC7)
	move	$16,$7
	jal	sprintf
	move	$18,$4

	subu	$4,$2,$17
	addu	$3,$17,$16
	slt	$3,$3,$2
	slt	$2,$16,$4
	movn	$4,$16,$2		#RLX4181/RLX4281:conditional move
	addu	$18,$18,$17
	bne	$3,$0,$L188
	slt	$5,$4,0

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L188:
	sw	$18,0($19)
	lw	$31,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	movz	$2,$4,$5		#RLX4181/RLX4281:conditional move  
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	pptpconn_read_proc
	.section	.text.pptpconn_write_proc,"ax",@progbits
	.align	2
	.globl	pptpconn_write_proc
	.ent	pptpconn_write_proc
	.type	pptpconn_write_proc, @function
pptpconn_write_proc:
	.set	nomips16
	.frame	$sp,224,$31		# vars= 200, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-224
	sltu	$2,$6,2
	sw	$31,216($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L191
	move	$3,$6
	.set	macro
	.set	reorder

	addu	$2,$5,$6
	or	$2,$5,$2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L191
	or	$7,$2,$6
	.set	macro
	.set	reorder

	lw	$2,24($28)
	addiu	$13,$sp,16
	and	$2,$2,$7
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L194
	move	$4,$13
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
	move	$3,$6
$L194:
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L191
	move	$4,$13
	.set	macro
	.set	reorder

	jal	atoi_dec
	lui	$3,%hi(pptp_conn_check)
	sw	$2,%lo(pptp_conn_check)($3)
$L191:
	lw	$31,216($sp)
	li	$2,-14			# 0xfffffffffffffff2
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,224
	.set	macro
	.set	reorder

	.end	pptpconn_write_proc
	.section	.text.ppfw_write_proc,"ax",@progbits
	.align	2
	.globl	ppfw_write_proc
	.ent	ppfw_write_proc
	.type	ppfw_write_proc, @function
ppfw_write_proc:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 8, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-40
	sltu	$2,$6,2
	sw	$18,32($sp)
	sw	$16,24($sp)
	sw	$31,36($sp)
	sw	$17,28($sp)
	move	$18,$6
	move	$16,$5
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L201
	li	$3,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

$L207:
$L248:
	addiu	$4,$6,32
	.set	noreorder
	.set	nomacro
	jal	__kmalloc
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	move	$17,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L201
	li	$3,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L252
	addu	$2,$16,$18
	.set	macro
	.set	reorder

	lw	$3,24($28)
	or	$2,$16,$2
	or	$2,$2,$18
	and	$3,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L253
	move	$6,$18
	.set	macro
	.set	reorder

	move	$5,$16
	move	$4,$17
#APP
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
#NO_APP
$L253:
	bne	$6,$0,$L252
	.set	noreorder
	.set	nomacro
	jal	atoi_dec
	move	$4,$17
	.set	macro
	.set	reorder

	lui	$4,%hi(pptpAccInfo)
	lui	$16,%hi(fast_pptp_fw)
	addiu	$4,$4,%lo(pptpAccInfo)
	move	$5,$0
	li	$6,64			# 0x40
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$2,%lo(fast_pptp_fw)($16)
	.set	macro
	.set	reorder

	lw	$2,%lo(fast_pptp_fw)($16)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L260
	lui	$3,%hi(sync_tx_pptp_gre_seqno_hook)
	.set	macro
	.set	reorder

	lui	$2,%hi(sync_tx_pptp_gre_seqno)
	addiu	$2,$2,%lo(sync_tx_pptp_gre_seqno)
	.set	noreorder
	.set	nomacro
	j	$L261
	sw	$2,%lo(sync_tx_pptp_gre_seqno_hook)($3)
	.set	macro
	.set	reorder

$L260:
	sw	$0,%lo(sync_tx_pptp_gre_seqno_hook)($3)
$L261:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L201
	move	$3,$18
	.set	macro
	.set	reorder

$L252:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	li	$3,-14			# 0xfffffffffffffff2
$L201:
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	move	$2,$3
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,40
	.set	macro
	.set	reorder

	.end	ppfw_write_proc
	.rdata
	.align	2
$LC8:
	.ascii	"fast_pptp\000"
	.align	2
$LC9:
	.ascii	"pptp_conn_ck\000"
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	fast_pptp_init
	.ent	fast_pptp_init
	.type	fast_pptp_init, @function
fast_pptp_init:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$4,%hi($LC8)
	addiu	$sp,$sp,-24
	addiu	$4,$4,%lo($LC8)
	move	$5,$0
	sw	$31,16($sp)
	jal	create_proc_entry
	move	$6,$0

	lui	$4,%hi($LC9)
	lui	$7,%hi(res1)
	move	$3,$2
	addiu	$4,$4,%lo($LC9)
	move	$5,$0
	move	$6,$0
	beq	$2,$0,$L263
	sw	$2,%lo(res1)($7)

	lui	$2,%hi(ppfw_read_proc)
	addiu	$2,$2,%lo(ppfw_read_proc)
	sw	$2,64($3)
	lw	$3,%lo(res1)($7)
	lui	$2,%hi(ppfw_write_proc)
	addiu	$2,$2,%lo(ppfw_write_proc)
	sw	$2,68($3)
$L263:
	jal	create_proc_entry
	nop

	lui	$4,%hi(pptpAccInfo)
	lui	$7,%hi(res_check_pptp)
	move	$3,$2
	addiu	$4,$4,%lo(pptpAccInfo)
	move	$5,$0
	li	$6,64			# 0x40
	beq	$2,$0,$L264
	sw	$2,%lo(res_check_pptp)($7)

	lui	$2,%hi(pptpconn_read_proc)
	addiu	$2,$2,%lo(pptpconn_read_proc)
	sw	$2,64($3)
	lw	$3,%lo(res_check_pptp)($7)
	lui	$2,%hi(pptpconn_write_proc)
	addiu	$2,$2,%lo(pptpconn_write_proc)
	sw	$2,68($3)
$L264:
	jal	memset
	nop

	lw	$31,16($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fast_pptp_init
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	fast_pptp_exit
	.ent	fast_pptp_exit
	.type	fast_pptp_exit, @function
fast_pptp_exit:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$16,16($sp)
	lui	$16,%hi(res1)
	lw	$2,%lo(res1)($16)
	lui	$4,%hi($LC8)
	sw	$17,20($sp)
	sw	$31,24($sp)
	lui	$17,%hi(res_check_pptp)
	addiu	$4,$4,%lo($LC8)
	beq	$2,$0,$L266
	move	$5,$2

	jal	remove_proc_entry
	nop

	sw	$0,%lo(res1)($16)
$L266:
	lw	$2,%lo(res_check_pptp)($17)
	lui	$4,%hi($LC9)
	addiu	$4,$4,%lo($LC9)
	beq	$2,$0,$L265
	move	$5,$2

	jal	remove_proc_entry
	nop

	sw	$0,%lo(res_check_pptp)($17)
$L265:
	lw	$31,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	fast_pptp_exit
	.local	pptpAccInfo
	.comm	pptpAccInfo,64,4
	.ident	"GCC: (GNU) 3.4.6-1.3.6"
