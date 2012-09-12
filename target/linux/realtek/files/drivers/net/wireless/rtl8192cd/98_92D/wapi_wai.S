	.file	1 "wapi_wai.c"
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
	.type	WAPI_KM_OUI, @object
	.size	WAPI_KM_OUI, 3
WAPI_KM_OUI:
	.byte	0
	.byte	20
	.byte	114
	.rdata
	.align	2
	.type	__func__.0, @object
	.size	__func__.0, 14
__func__.0:
	.ascii	"wapiDeauthSta\000"
	.section	.text.wapiDeauthSta,"ax",@progbits
	.align	2
	.ent	wapiDeauthSta
	.type	wapiDeauthSta, @function
wapiDeauthSta:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$16,16($sp)
	move	$16,$5
	addiu	$5,$5,6070
	sw	$18,24($sp)
	move	$18,$4
	sw	$31,28($sp)
	.set	noreorder
	.set	nomacro
	jal	issue_deauth
	sw	$17,20($sp)
	.set	macro
	.set	reorder

#APP
	raw_local_irq_save	$17
#NO_APP
	lw	$6,8($16)
	addiu	$3,$16,8
	.set	noreorder
	.set	nomacro
	beq	$6,$3,$L7
	lui	$7,%hi(__func__.0)
	.set	macro
	.set	reorder

	lw	$2,4($3)
	addiu	$7,$7,%lo(__func__.0)
	move	$4,$18
	sw	$6,0($2)
	move	$5,$16
	sw	$2,4($6)
	move	$6,$0
	sw	$3,4($3)
	lw	$2,6248($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L7
	sw	$3,8($16)
	.set	macro
	.set	reorder

	jal	cnt_assoc_num
	move	$4,$18
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	check_sta_characteristic
	move	$6,$0
	.set	macro
	.set	reorder

$L7:
#APP
	raw_local_irq_restore	$17
#NO_APP
	move	$4,$18
	move	$5,$16
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	free_stainfo
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	wapiDeauthSta
	.section	.text.wapiFreeAllSta,"ax",@progbits
	.align	2
	.ent	wapiFreeAllSta
	.type	wapiFreeAllSta, @function
wapiFreeAllSta:
	.set	nomips16
	.frame	$sp,48,$31		# vars= 0, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$22,40($sp)
	move	$22,$5
	sw	$21,36($sp)
	move	$21,$4
	sw	$20,32($sp)
	move	$20,$0
	sw	$19,28($sp)
	li	$19,30			# 0x1e
	sw	$18,24($sp)
	move	$18,$4
	sw	$31,44($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
$L35:
	li	$2,65536			# 0x10000
	ori	$2,$2,0x2a74
	addu	$17,$18,$2
	lw	$16,0($17)
$L41:
	beq	$16,$17,$L39
	move	$5,$16

	lw	$3,6676($16)
	lw	$16,0($16)
	beq	$3,$0,$L41
	nop

	lw	$2,364($3)
	beq	$2,$0,$L41
	li	$2,1			# 0x1

	move	$4,$21
	beq	$22,$2,$L32
	li	$6,25			# 0x19

	lw	$2,352($3)
	andi	$2,$2,0x400
	bne	$2,$0,$L31
	nop

$L32:
	jal	wapiDeauthSta
	nop

	j	$L41
	nop

$L31:
	j	$L41
	addiu	$20,$20,1

$L39:
	addiu	$19,$19,-1
	bgez	$19,$L35
	addiu	$18,$18,8

	move	$2,$20
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
	.end	wapiFreeAllSta
	.local	data.1
	.comm	data.1,64,4
	.section	.text.wapiReqActiveCA,"ax",@progbits
	.align	2
	.globl	wapiReqActiveCA
	.ent	wapiReqActiveCA
	.type	wapiReqActiveCA, @function
wapiReqActiveCA:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	move	$5,$0
	sw	$31,36($sp)
	li	$6,64			# 0x40
	sw	$19,28($sp)
	sw	$18,24($sp)
	move	$18,$4
	sw	$16,16($sp)
	lui	$16,%hi(data.1)
	sw	$20,32($sp)
	addiu	$16,$16,%lo(data.1)
	sw	$17,20($sp)
	addiu	$19,$16,8
	lw	$20,6676($4)
	lw	$17,652($20)
	jal	memset
	move	$4,$16

	li	$2,3
	move	$5,$0
	li	$6,16			# 0x10
	move	$4,$19
	sh	$2,2($16)
	jal	memset
	sw	$18,4($16)

	lw	$4,4($17)
	jal	strlen
	nop

	move	$4,$19
	lw	$5,4($17)
	jal	memcpy
	move	$6,$2

	addiu	$4,$16,24
	lw	$5,4($17)
	li	$6,6			# 0x6
	jal	memcpy
	addiu	$5,$5,320

	addiu	$5,$18,6070
	addiu	$4,$16,30
	jal	memcpy
	li	$6,6			# 0x6

	move	$6,$16
	lw	$4,652($20)
	lw	$5,36($4)
	jal	DOT11_EnQueue
	li	$7,36			# 0x24

	lw	$4,652($20)
	jal	wapi_event_indicate
	nop

	lw	$2,364($20)
	bne	$2,$0,$L43
	addiu	$4,$20,600

	li	$2,1			# 0x1
	sw	$2,364($20)
$L43:
	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$31,36($sp)
	lw	$20,32($sp)
	addiu	$5,$5,1000
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	mod_timer
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	wapiReqActiveCA
	.section	.text.wapiExit,"ax",@progbits
	.align	2
	.globl	wapiExit
	.ent	wapiExit
	.type	wapiExit, @function
wapiExit:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	del_timer
	addiu	$4,$4,1308

	.set	macro
	.set	reorder
	.end	wapiExit
	.section	.text.wapiStationInit,"ax",@progbits
	.align	2
	.globl	wapiStationInit
	.ent	wapiStationInit
	.type	wapiStationInit, @function
wapiStationInit:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	move	$5,$0
	sw	$31,32($sp)
	move	$6,$0
	sw	$16,16($sp)
	move	$16,$4
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	lw	$17,6676($4)
	jal	init_timer_key
	addiu	$4,$17,600

	lui	$2,%hi(wapiResendTimeout)
	addiu	$2,$2,%lo(wapiResendTimeout)
	addiu	$4,$17,624
	sw	$2,612($17)
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	sw	$16,616($17)

	lui	$2,%hi(wapiUCastUpdateKeyTimeout)
	lw	$3,652($17)
	addiu	$2,$2,%lo(wapiUCastUpdateKeyTimeout)
	sw	$16,640($17)
	li	$4,16			# 0x10
	sw	$2,636($17)
	li	$6,92
	sh	$0,358($17)
	li	$5,54
	lhu	$2,1236($3)
	sh	$2,356($17)
	lhu	$2,1236($3)
	addiu	$2,$2,8
	sh	$2,1236($3)
$L49:
	addu	$2,$4,$17
	addiu	$4,$4,-2
	sb	$5,366($2)
	bgtz	$4,$L49
	sb	$6,367($2)

	addiu	$16,$17,384
	li	$19,2			# 0x2
$L53:
	addiu	$18,$17,368
	addiu	$19,$19,-1
	move	$4,$16
	move	$5,$18
	li	$6,16			# 0x10
	jal	memcpy
	addiu	$16,$16,16

	bgez	$19,$L53
	addiu	$4,$17,432

	move	$5,$18
	li	$6,16			# 0x10
	jal	memcpy
	li	$19,3			# 0x3

	move	$5,$18
	lw	$4,652($17)
	li	$6,16			# 0x10
	jal	memcpy
	addiu	$4,$4,928

	move	$5,$18
	lw	$4,652($17)
	li	$6,16			# 0x10
	jal	memcpy
	addiu	$4,$4,944

	move	$5,$18
	lw	$4,652($17)
	li	$6,16			# 0x10
	jal	memcpy
	addiu	$4,$4,960

	move	$5,$18
	li	$4,55
$L57:
	addiu	$19,$19,-1
	sb	$4,0($5)
	bgez	$19,$L57
	addiu	$5,$5,16

	lw	$3,352($17)
	li	$2,-65536			# 0xffff0000
	ori	$2,$2,0x7fff
	and	$3,$3,$2
	li	$2,-4097			# 0xffffefff
	and	$3,$3,$2
	li	$2,-16385			# 0xffffbfff
	and	$3,$3,$2
	li	$2,-8193			# 0xffffdfff
	and	$3,$3,$2
	li	$2,-2049			# 0xfffff7ff
	and	$3,$3,$2
	sw	$3,352($17)
	sb	$4,432($17)
	sw	$0,592($17)
	sb	$0,353($17)
	sw	$0,648($17)
	sw	$0,364($17)
	sw	$0,656($17)
	lw	$31,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	wapiStationInit
	.section	.text.wapiSetIE,"ax",@progbits
	.align	2
	.globl	wapiSetIE
	.ent	wapiSetIE
	.type	wapiSetIE, @function
wapiSetIE:
	.set	nomips16
	.frame	$sp,48,$31		# vars= 8, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	li	$2,256
	sw	$31,40($sp)
	addiu	$5,$sp,16
	sw	$19,36($sp)
	li	$6,2			# 0x2
	sw	$18,32($sp)
	lui	$18,%hi(WAPI_KM_OUI)
	sw	$17,28($sp)
	move	$17,$4
	sw	$16,24($sp)
	li	$19,1
	lw	$16,908($4)
	sb	$0,1232($4)
	move	$4,$16
	addiu	$16,$16,2
	sh	$2,20($sp)
	sh	$2,16($sp)
	sh	$2,18($sp)
	jal	memcpy
	sh	$0,22($sp)

	move	$4,$16
	lbu	$2,1232($17)
	addiu	$5,$sp,18
	li	$6,2			# 0x2
	addiu	$16,$16,2
	addiu	$2,$2,2
	jal	memcpy
	sb	$2,1232($17)

	move	$4,$16
	lbu	$2,1232($17)
	addiu	$5,$18,%lo(WAPI_KM_OUI)
	li	$6,3			# 0x3
	addiu	$2,$2,2
	jal	memcpy
	sb	$2,1232($17)

	addiu	$5,$sp,20
	lw	$2,8($17)
	li	$6,2			# 0x2
	lw	$2,5192($2)
	sb	$2,3($16)
	addiu	$16,$16,4
	lbu	$2,1232($17)
	move	$4,$16
	addiu	$2,$2,4
	jal	memcpy
	sb	$2,1232($17)

	addiu	$4,$16,2
	addiu	$5,$18,%lo(WAPI_KM_OUI)
	jal	memcpy
	li	$6,3			# 0x3

	addiu	$5,$18,%lo(WAPI_KM_OUI)
	sb	$19,5($16)
	addiu	$16,$16,6
	lbu	$2,1232($17)
	move	$4,$16
	li	$6,3			# 0x3
	addiu	$2,$2,6
	jal	memcpy
	sb	$2,1232($17)

	addiu	$4,$16,4
	sb	$19,3($16)
	addiu	$5,$sp,22
	lbu	$2,1232($17)
	li	$6,2			# 0x2
	addiu	$2,$2,4
	jal	memcpy
	sb	$2,1232($17)

	lbu	$2,1232($17)
	lw	$31,40($sp)
	addiu	$2,$2,2
	sb	$2,1232($17)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	wapiSetIE
	.rdata
	.align	2
$LC1:
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.align	2
$LC0:
	.ascii	"multicast or station key expansion for station unicast a"
	.ascii	"nd multicast and broadcast\000"
	.section	.text.wapiInstallMCastKey,"ax",@progbits
	.align	2
	.ent	wapiInstallMCastKey
	.type	wapiInstallMCastKey, @function
wapiInstallMCastKey:
	.set	nomips16
	.frame	$sp,88,$31		# vars= 40, regs= 5/0, args= 24, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-88
	lui	$2,%hi($LC1)
	sw	$31,80($sp)
	addiu	$6,$2,%lo($LC1)
	sw	$19,76($sp)
	addiu	$19,$sp,56
	sw	$18,72($sp)
	move	$18,$4
	sw	$17,68($sp)
	addiu	$17,$sp,24
	sw	$16,64($sp)
	lui	$16,%hi($LC0)
	lbu	$3,%lo($LC1)($2)
	addiu	$16,$16,%lo($LC0)
	lbu	$2,1($6)
	move	$4,$16
	lbu	$5,2($6)
	sll	$3,$3,24
	sll	$2,$2,16
	or	$2,$2,$3
	sll	$5,$5,8
	lbu	$3,3($6)
	or	$5,$5,$2
	lbu	$7,5($6)
	lbu	$2,4($6)
	or	$3,$3,$5
	sw	$3,56($sp)
	sb	$7,61($sp)
	jal	strlen
	sb	$2,60($sp)

	move	$6,$16
	move	$7,$2
	addiu	$4,$18,912
	li	$2,32			# 0x20
	li	$5,16			# 0x10
	sw	$2,20($sp)
	jal	KD_hmac_sha256
	sw	$17,16($sp)

	move	$5,$17
	lw	$4,1232($18)
	li	$6,16			# 0x10
	srl	$4,$4,10
	andi	$4,$4,0x20
	addu	$4,$18,$4
	jal	memcpy
	addiu	$4,$4,1238

	addiu	$5,$sp,40
	lw	$4,1232($18)
	li	$6,16			# 0x10
	srl	$4,$4,10
	andi	$4,$4,0x20
	addu	$4,$18,$4
	jal	memcpy
	addiu	$4,$4,1254

	move	$4,$18
	lw	$2,8($18)
	move	$5,$19
	lw	$2,172($2)
	bne	$2,$0,$L65
	li	$7,24			# 0x18

	lw	$6,1232($18)
	sw	$0,16($sp)
	srl	$6,$6,15
	andi	$6,$6,0x1
	sll	$2,$6,5
	sll	$6,$6,1
	addu	$2,$18,$2
	addiu	$2,$2,1238
	jal	CamAddOneEntry
	sw	$2,20($sp)

	move	$5,$19
	move	$4,$18
	beq	$2,$0,$L67
	li	$7,24			# 0x18

	lw	$6,1232($18)
	li	$2,1			# 0x1
	sw	$2,16($sp)
	srl	$6,$6,15
	andi	$6,$6,0x1
	sll	$2,$6,5
	sll	$6,$6,1
	addu	$2,$18,$2
	addiu	$6,$6,1
	addiu	$2,$2,1254
	jal	CamAddOneEntry
	sw	$2,20($sp)

	lw	$3,8($18)
	sltu	$2,$0,$2
	j	$L65
	sw	$2,668($3)

$L67:
	lw	$2,8($18)
	sw	$0,668($2)
$L65:
	lw	$31,80($sp)
	lw	$19,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	j	$31
	addiu	$sp,$sp,88

	.set	macro
	.set	reorder
	.end	wapiInstallMCastKey
	.section	.text.wapiInit,"ax",@progbits
	.align	2
	.globl	wapiInit
	.ent	wapiInit
	.type	wapiInit, @function
wapiInit:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$17,20($sp)
	move	$17,$4
	sw	$31,24($sp)
	jal	wapiInstallMCastKey
	sw	$16,16($sp)

	li	$3,16			# 0x10
	li	$5,92
	li	$4,54
$L75:
	addu	$2,$3,$17
	addiu	$3,$3,-2
	sb	$4,926($2)
	bgtz	$3,$L75
	sb	$5,927($2)

	addiu	$5,$17,928
	addiu	$16,$17,1308
	li	$6,16			# 0x10
	jal	memcpy
	addiu	$4,$17,944

	jal	del_timer
	move	$4,$16

	move	$4,$16
	move	$5,$0
	jal	init_timer_key
	move	$6,$0

	lui	$2,%hi(wapiMCastUpdateKeyTimeout)
	lw	$3,1232($17)
	addiu	$2,$2,%lo(wapiMCastUpdateKeyTimeout)
	lw	$4,8($17)
	ori	$3,$3,0x1000
	sw	$2,1320($17)
	sw	$3,1232($17)
	sw	$17,1324($17)
	lw	$2,5260($4)
	lw	$31,24($sp)
	sw	$2,1304($17)
	lw	$16,16($sp)
	lw	$17,20($sp)
	j	init_SMS4_CK_Sbox
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	wapiInit
	.rdata
	.align	2
$LC2:
	.ascii	"pairwise key expansion for unicast and additional keys a"
	.ascii	"nd nonce\000"
	.section	.text.wapiDerivedUCastKey,"ax",@progbits
	.align	2
	.ent	wapiDerivedUCastKey
	.type	wapiDerivedUCastKey, @function
wapiDerivedUCastKey:
	.set	nomips16
	.frame	$sp,208,$31		# vars= 160, regs= 5/0, args= 24, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-208
	lui	$2,%hi($LC2)
	sw	$19,196($sp)
	move	$19,$5
	sw	$18,192($sp)
	addiu	$18,$2,%lo($LC2)
	sw	$17,188($sp)
	li	$6,6			# 0x6
	sw	$31,200($sp)
	move	$17,$4
	sw	$16,184($sp)
	lw	$16,6676($4)
	lw	$3,652($16)
	lw	$2,8($3)
	lw	$2,892($2)
	andi	$5,$2,0x10
	andi	$2,$2,0x8
	beq	$2,$0,$L79
	addiu	$4,$sp,30

	lw	$5,4($3)
	jal	memcpy
	addiu	$5,$5,320

	addiu	$4,$sp,24
	j	$L82
	addiu	$5,$17,6070

$L79:
	addiu	$4,$sp,24
	beq	$5,$0,$L80
	li	$6,6			# 0x6

	lw	$5,4($3)
	jal	memcpy
	addiu	$5,$5,320

	addiu	$5,$17,6070
	addiu	$4,$sp,30
$L82:
	jal	memcpy
	li	$6,6			# 0x6

$L80:
	addiu	$4,$sp,36
	addiu	$5,$16,288
	jal	memcpy
	li	$6,32			# 0x20

	addiu	$5,$16,256
	li	$6,32			# 0x20
	jal	memcpy
	addiu	$4,$sp,68

	jal	strlen
	move	$4,$18

	move	$5,$18
	move	$6,$2
	jal	memcpy
	addiu	$4,$sp,100

	jal	strlen
	move	$4,$18

	addiu	$4,$16,456
	addiu	$2,$2,76
	addiu	$6,$sp,24
	andi	$7,$2,0x00ff
	li	$5,16			# 0x10
	li	$2,96			# 0x60
	sw	$19,16($sp)
	jal	KD_hmac_sha256
	sw	$2,20($sp)

	lw	$31,200($sp)
	lw	$19,196($sp)
	lw	$18,192($sp)
	lw	$17,188($sp)
	lw	$16,184($sp)
	j	$31
	addiu	$sp,$sp,208

	.set	macro
	.set	reorder
	.end	wapiDerivedUCastKey
	.section	.text.wapiInstallUCastKey,"ax",@progbits
	.align	2
	.ent	wapiInstallUCastKey
	.type	wapiInstallUCastKey, @function
wapiInstallUCastKey:
	.set	nomips16
	.frame	$sp,64,$31		# vars= 8, regs= 8/0, args= 24, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	lui	$2,%hi($LC1)
	sw	$31,60($sp)
	addiu	$7,$2,%lo($LC1)
	sw	$22,56($sp)
	sw	$21,52($sp)
	move	$21,$4
	sw	$20,48($sp)
	sw	$19,44($sp)
	andi	$19,$5,0x00ff
	sw	$17,36($sp)
	addiu	$5,$6,32
	sw	$18,40($sp)
	move	$17,$6
	sw	$16,32($sp)
	li	$6,16			# 0x10
	lbu	$3,%lo($LC1)($2)
	lbu	$2,1($7)
	lbu	$4,2($7)
	sll	$3,$3,24
	sll	$2,$2,16
	or	$2,$2,$3
	sll	$4,$4,8
	lbu	$3,3($7)
	or	$4,$4,$2
	lbu	$8,5($7)
	lw	$16,6676($21)
	or	$3,$3,$4
	lbu	$2,4($7)
	sw	$3,24($sp)
	sb	$8,29($sp)
	sb	$2,28($sp)
	lw	$18,652($16)
	jal	memcpy
	addiu	$4,$16,568

	sll	$2,$19,5
	addu	$2,$16,$2
	move	$5,$17
	addiu	$20,$2,488
	addiu	$22,$2,504
	move	$4,$20
	jal	memcpy
	li	$6,16			# 0x10

	move	$4,$22
	addiu	$5,$17,16
	jal	memcpy
	li	$6,16			# 0x10

	addiu	$4,$16,552
	addiu	$5,$17,48
	jal	memcpy
	li	$6,16			# 0x10

	addiu	$4,$17,64
	addiu	$6,$16,288
	li	$5,32			# 0x20
	jal	sha2
	move	$7,$0

	lw	$2,8($18)
	lw	$2,172($2)
	bne	$2,$0,$L83
	addiu	$17,$21,6070

	move	$4,$18
	move	$5,$17
	move	$6,$19
	li	$7,24			# 0x18
	sw	$20,20($sp)
	jal	CamAddOneEntry
	sw	$0,16($sp)

	beq	$2,$0,$L85
	li	$16,1			# 0x1

	lw	$3,60($18)
	move	$4,$18
	move	$5,$17
	lw	$2,9728($3)
	move	$6,$19
	li	$7,24			# 0x18
	addiu	$2,$2,1
	sw	$2,9728($3)
	sw	$22,20($sp)
	jal	CamAddOneEntry
	sw	$16,16($sp)

	beq	$2,$0,$L85
	nop

	sw	$16,6124($21)
	lw	$3,60($18)
	lw	$2,9728($3)
	addiu	$2,$2,1
	j	$L88
	sw	$2,9728($3)

$L85:
	sw	$0,6124($21)
$L88:
	sltu	$16,$19,1
	move	$4,$18
	move	$5,$17
	move	$6,$16
	jal	CamDeleteOneEntry
	move	$7,$0

	beq	$2,$0,$L89
	move	$5,$17

	lw	$3,60($18)
	move	$6,$16
	move	$4,$18
	lw	$2,9728($3)
	move	$7,$0
	addiu	$2,$2,-1
	jal	CamDeleteOneEntry
	sw	$2,9728($3)

	beq	$2,$0,$L89
	nop

	lw	$3,60($18)
	lw	$2,9728($3)
	addiu	$2,$2,-1
	sw	$2,9728($3)
$L89:
	lw	$6,1232($18)
	move	$4,$18
	sw	$0,16($sp)
	addiu	$5,$sp,24
	lw	$2,1232($18)
	srl	$6,$6,14
	andi	$6,$6,0x2
	li	$7,24			# 0x18
	srl	$2,$2,10
	andi	$2,$2,0x20
	addu	$2,$18,$2
	addiu	$2,$2,1238
	jal	CamAddOneEntry
	sw	$2,20($sp)

	beq	$2,$0,$L91
	li	$2,1			# 0x1

	move	$4,$18
	lw	$6,1232($18)
	addiu	$5,$sp,24
	sw	$2,16($sp)
	li	$7,24			# 0x18
	lw	$2,1232($18)
	srl	$6,$6,14
	andi	$6,$6,0x2
	srl	$2,$2,10
	addiu	$6,$6,1
	andi	$2,$2,0x20
	addu	$2,$18,$2
	addiu	$2,$2,1254
	jal	CamAddOneEntry
	sw	$2,20($sp)

	lw	$3,8($18)
	sltu	$2,$0,$2
	j	$L83
	sw	$2,668($3)

$L91:
	lw	$2,8($18)
	sw	$0,668($2)
$L83:
	lw	$31,60($sp)
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,64

	.set	macro
	.set	reorder
	.end	wapiInstallUCastKey
	.section	.text.wapiReleaseFragementQueue,"ax",@progbits
	.align	2
	.globl	wapiReleaseFragementQueue
	.ent	wapiReleaseFragementQueue
	.type	wapiReleaseFragementQueue, @function
wapiReleaseFragementQueue:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$17,20($sp)
	sw	$31,24($sp)
	sw	$16,16($sp)
	lw	$2,648($4)
	beq	$2,$0,$L95
	move	$17,$4

	move	$16,$2
	move	$2,$16
$L101:
	move	$4,$16
	sw	$0,4($16)
	lw	$16,0($16)
	jal	dev_kfree_skb_any
	sw	$0,0($2)

	lw	$2,648($17)
	bne	$16,$2,$L101
	move	$2,$16

	sw	$0,584($17)
	sw	$0,648($17)
	sb	$0,353($17)
$L95:
	lw	$31,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	wapiReleaseFragementQueue
	.section	.text.wapiFragementSend,"ax",@progbits
	.align	2
	.ent	wapiFragementSend
	.type	wapiFragementSend, @function
wapiFragementSend:
	.set	nomips16
	.frame	$sp,64,$31		# vars= 8, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	sw	$fp,56($sp)
	move	$fp,$5
	sw	$23,52($sp)
	sw	$22,48($sp)
	move	$22,$4
	sw	$21,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$31,60($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	lw	$2,4($5)
	lw	$3,84($4)
	lw	$2,208($2)
	addiu	$3,$3,-26
	andi	$4,$2,0x7
	move	$20,$3
	subu	$2,$2,$4
	addiu	$4,$2,-60
	addiu	$2,$2,-34
	divu	$0,$3,$4
	sw	$2,16($sp)
	lw	$2,176($22)
	addiu	$21,$2,26
	mflo	$19
	mfhi	$3
	addiu	$2,$19,1
	movn	$19,$2,$3		#conditional move  
	blez	$19,$L120
	move	$23,$0

$L117:
	jal	alloc_skb_from_queue
	move	$4,$fp

	li	$4,2400			# 0x960
	bne	$2,$0,$L109
	move	$18,$2

	jal	dev_alloc_skb
	nop

	beq	$2,$0,$L121
	move	$18,$2

$L109:
	lw	$4,16($sp)
	addiu	$5,$20,26
	lw	$3,176($18)
	lw	$6,164($18)
	addiu	$2,$4,-26
	addiu	$3,$3,34
	sltu	$2,$2,$20
	addiu	$6,$6,34
	movn	$5,$4,$2		#conditional move
	sw	$3,176($18)
	move	$4,$18
	jal	skb_put
	sw	$6,164($18)

	li	$6,26			# 0x1a
	lw	$16,176($18)
	lw	$5,176($22)
	move	$4,$16
	lw	$17,84($18)
	jal	memcpy
	addiu	$16,$16,14

	move	$5,$21
	lw	$4,176($18)
	addiu	$2,$17,-26
	move	$6,$2
	subu	$20,$20,$2
	addiu	$4,$4,26
	jal	memcpy
	addu	$21,$21,$2

	addiu	$17,$17,-14
	lw	$2,24($22)
	sltu	$3,$0,$20
	move	$4,$18
	sw	$2,24($18)
	li	$2,-30540
	sh	$2,106($18)
	sb	$3,11($16)
	sb	$23,10($16)
	sh	$17,6($16)
	lw	$5,4($fp)
	jal	rtl8192cd_start_xmit
	addiu	$23,$23,1

	beq	$2,$0,$L106
	slt	$3,$23,$19

	jal	dev_kfree_skb_any
	move	$4,$18

$L121:
	j	$L102
	li	$2,-1			# 0xffffffff

$L106:
	bne	$3,$0,$L117
	nop

$L120:
	jal	dev_kfree_skb_any
	move	$4,$22

	li	$2,1			# 0x1
$L102:
	lw	$31,60($sp)
	lw	$fp,56($sp)
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,64

	.set	macro
	.set	reorder
	.end	wapiFragementSend
	.section	.text.WapiComparePN,"ax",@progbits
	.align	2
	.globl	WapiComparePN
	.ent	WapiComparePN
	.type	WapiComparePN, @function
WapiComparePN:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lbu	$2,15($5)
	lbu	$3,15($4)
	subu	$2,$2,$3
	andi	$2,$2,0x80
	bne	$2,$0,$L122
	move	$6,$0

	li	$6,16			# 0x10
$L131:
	addu	$2,$4,$6
	addu	$3,$5,$6
	lbu	$7,-1($2)
	addiu	$2,$6,-1
	lbu	$3,-1($3)
	sll	$6,$2,24
	beq	$7,$3,$L126
	sra	$6,$6,24

	sltu	$2,$3,$7
	beq	$2,$0,$L122
	li	$6,-1			# 0xffffffff

	j	$L122
	move	$6,$0

$L126:
	bgtz	$6,$L131
	nop

	li	$6,-1			# 0xffffffff
$L122:
	j	$31
	move	$2,$6

	.set	macro
	.set	reorder
	.end	WapiComparePN
	.section	.text.WapiIncreasePN,"ax",@progbits
	.align	2
	.globl	WapiIncreasePN
	.ent	WapiIncreasePN
	.type	WapiIncreasePN, @function
WapiIncreasePN:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	andi	$5,$5,0x00ff
	move	$7,$0
$L141:
	addu	$6,$4,$7
	addiu	$2,$7,1
	lbu	$3,0($6)
	andi	$7,$2,0x00ff
	sltu	$8,$7,16
	andi	$2,$3,0x00ff
	addu	$3,$5,$3
	addu	$2,$2,$5
	slt	$2,$2,256
	beq	$2,$0,$L139
	li	$5,1			# 0x1

	li	$2,-1			# 0xffffffff
	j	$31
	sb	$3,0($6)

$L139:
	bne	$8,$0,$L141
	sb	$3,0($6)

	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	WapiIncreasePN
	.rdata
	.align	2
$LC4:
	.ascii	"base key expansion for key and additional nonce\000"
	.align	2
$LC3:
	.ascii	"preshared key expansion for authentication and key negot"
	.ascii	"iation\000"
	.section	.text.wapiSetBK,"ax",@progbits
	.align	2
	.globl	wapiSetBK
	.ent	wapiSetBK
	.type	wapiSetBK, @function
wapiSetBK:
	.set	nomips16
	.frame	$sp,192,$31		# vars= 144, regs= 6/0, args= 24, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-192
	lui	$2,%hi($LC4)
	sw	$20,184($sp)
	move	$20,$4
	sw	$19,180($sp)
	addiu	$19,$2,%lo($LC4)
	sw	$18,176($sp)
	lui	$2,%hi($LC3)
	sw	$31,188($sp)
	addiu	$18,$2,%lo($LC3)
	sw	$17,172($sp)
	li	$2,2			# 0x2
	sw	$16,168($sp)
	lw	$16,6676($4)
	lw	$3,360($16)
	lw	$17,652($16)
	bne	$3,$2,$L145
	move	$4,$18

	jal	strlen
	nop

	move	$6,$18
	lw	$4,8($17)
	move	$7,$2
	addiu	$2,$16,456
	lw	$5,5236($4)
	j	$L151
	addiu	$4,$4,5204

$L145:
	addiu	$18,$sp,40
	li	$2,1			# 0x1
	li	$6,64			# 0x40
	bne	$3,$2,$L146
	move	$4,$18

	lw	$5,908($17)
	jal	memcpy
	addiu	$5,$5,24

	jal	strlen
	move	$4,$19

	move	$5,$19
	move	$6,$2
	jal	memcpy
	addiu	$4,$sp,104

	jal	strlen
	move	$4,$19

	move	$6,$18
	addiu	$2,$2,64
	li	$5,24			# 0x18
	lw	$4,908($17)
	andi	$7,$2,0x00ff
	addiu	$2,$16,456
$L151:
	sw	$2,16($sp)
	li	$2,16			# 0x10
	jal	KD_hmac_sha256
	sw	$2,20($sp)

$L146:
	lw	$2,8($17)
	addiu	$4,$sp,30
	lw	$2,892($2)
	andi	$3,$2,0x10
	andi	$2,$2,0x8
	beq	$2,$0,$L148
	li	$6,6			# 0x6

	lw	$5,4($17)
	jal	memcpy
	addiu	$5,$5,320

	addiu	$5,$20,6070
	addiu	$4,$sp,24
	jal	memcpy
	li	$6,6			# 0x6

	j	$L152
	addiu	$2,$16,472

$L148:
	beq	$3,$0,$L149
	addiu	$4,$sp,24

	lw	$5,4($17)
	jal	memcpy
	addiu	$5,$5,320

	addiu	$5,$20,6070
	addiu	$4,$sp,30
	jal	memcpy
	li	$6,6			# 0x6

	addiu	$4,$16,288
	jal	GenerateRandomData
	li	$5,32			# 0x20

$L149:
	addiu	$2,$16,472
$L152:
	addiu	$4,$16,456
	sw	$2,16($sp)
	addiu	$6,$sp,24
	li	$2,16			# 0x10
	li	$5,16			# 0x10
	li	$7,12			# 0xc
	jal	KD_hmac_sha256
	sw	$2,20($sp)

	li	$2,5			# 0x5
	sw	$2,364($16)
	lw	$31,188($sp)
	lw	$20,184($sp)
	lw	$19,180($sp)
	lw	$18,176($sp)
	lw	$17,172($sp)
	lw	$16,168($sp)
	j	$31
	addiu	$sp,$sp,192

	.set	macro
	.set	reorder
	.end	wapiSetBK
	.section	.text.WapiRecvMulticastKeyNotification,"ax",@progbits
	.align	2
	.ent	WapiRecvMulticastKeyNotification
	.type	WapiRecvMulticastKeyNotification, @function
WapiRecvMulticastKeyNotification:
	.set	nomips16
	.frame	$sp,104,$31		# vars= 32, regs= 9/0, args= 32, gp= 0
	.mask	0x80ff0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-104
	li	$2,11			# 0xb
	sw	$22,88($sp)
	li	$7,-1			# 0xffffffff
	sw	$18,72($sp)
	move	$18,$4
	sw	$31,96($sp)
	addiu	$22,$18,12
	sw	$23,92($sp)
	sw	$21,84($sp)
	sw	$20,80($sp)
	sw	$19,76($sp)
	sw	$17,68($sp)
	sw	$16,64($sp)
	lbu	$23,3($4)
	bne	$23,$2,$L153
	move	$4,$5

	lbu	$2,12($18)
	addiu	$21,$sp,32
	lw	$16,6676($4)
	li	$5,16			# 0x10
	andi	$2,$2,0x20
	move	$6,$22
	lw	$19,652($16)
	bne	$2,$0,$L153
	addiu	$4,$16,568

	lhu	$7,6($18)
	li	$2,20			# 0x14
	sw	$2,20($sp)
	addiu	$17,$19,960
	addiu	$7,$7,-32
	addiu	$20,$18,43
	jal	sha256_hmac
	sw	$21,16($sp)

	move	$4,$21
	lhu	$5,6($18)
	li	$6,20			# 0x14
	addu	$5,$18,$5
	jal	memcmp
	addiu	$5,$5,-20

	move	$4,$20
	move	$5,$17
	bne	$2,$0,$L153
	li	$7,-1			# 0xffffffff

	jal	WapiComparePN
	nop

	li	$3,-1			# 0xffffffff
	move	$5,$20
	move	$4,$17
	li	$6,16			# 0x10
	beq	$2,$3,$L153
	li	$7,-1			# 0xffffffff

	jal	memcpy
	nop

	addiu	$2,$19,912
	lbu	$7,47($22)
	move	$5,$17
	sw	$2,16($sp)
	addiu	$2,$sp,56
	addiu	$6,$18,60
	addiu	$4,$16,552
	sw	$2,20($sp)
	li	$2,1			# 0x1
	jal	WapiSMS4ForMNKEncrypt
	sw	$2,24($sp)

	jal	wapiInstallMCastKey
	move	$4,$19

	li	$2,9			# 0x9
	lw	$3,364($16)
	bne	$3,$2,$L158
	move	$7,$0

	lw	$2,352($16)
	sw	$23,364($16)
	ori	$2,$2,0x800
	sw	$2,352($16)
$L158:
	lw	$3,652($16)
	lw	$2,1304($3)
	bne	$2,$0,$L159
	nop

	lw	$2,8($3)
	lw	$2,5260($2)
	sw	$2,1304($3)
$L159:
	lw	$2,352($16)
	li	$3,-253			# 0xffffff03
	and	$2,$2,$3
	sw	$2,352($16)
$L153:
	lw	$31,96($sp)
	move	$2,$7
	lw	$23,92($sp)
	lw	$22,88($sp)
	lw	$21,84($sp)
	lw	$20,80($sp)
	lw	$19,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	j	$31
	addiu	$sp,$sp,104

	.set	macro
	.set	reorder
	.end	WapiRecvMulticastKeyNotification
	.section	.text.wapiRecvUnicastKeyAgreementResponse,"ax",@progbits
	.align	2
	.ent	wapiRecvUnicastKeyAgreementResponse
	.type	wapiRecvUnicastKeyAgreementResponse, @function
wapiRecvUnicastKeyAgreementResponse:
	.set	nomips16
	.frame	$sp,176,$31		# vars= 120, regs= 8/0, args= 24, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-176
	li	$2,9			# 0x9
	sw	$22,168($sp)
	move	$22,$6
	sw	$21,164($sp)
	move	$21,$5
	sw	$20,160($sp)
	li	$5,-1			# 0xffffffff
	sw	$31,172($sp)
	sw	$19,156($sp)
	sw	$18,152($sp)
	sw	$17,148($sp)
	sw	$16,144($sp)
	lbu	$3,3($4)
	bne	$3,$2,$L160
	move	$20,$4

	lbu	$2,12($4)
	lw	$17,6676($21)
	andi	$2,$2,0x10
	beq	$2,$0,$L162
	addiu	$18,$4,12

	lw	$2,352($17)
	lbu	$3,17($18)
	srl	$2,$2,15
	andi	$2,$2,0x1
	beq	$3,$2,$L160
	nop

$L162:
	lw	$2,352($17)
	srl	$3,$2,15
	andi	$2,$2,0x1000
	beq	$2,$0,$L165
	andi	$19,$3,0x1

	xori	$2,$3,0x1
	andi	$19,$2,0x1
$L165:
	addiu	$4,$18,1
	addiu	$5,$17,472
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L160
	li	$5,-1			# 0xffffffff

	lbu	$2,17($18)
	beq	$2,$19,$L179
	addiu	$4,$18,62

	lw	$2,352($17)
	andi	$2,$2,0x1000
	beq	$2,$0,$L160
	nop

$L179:
	addiu	$5,$17,288
	jal	memcmp
	li	$6,32			# 0x20

	bne	$2,$0,$L167
	addiu	$4,$18,18

	lw	$2,652($17)
	li	$6,6			# 0x6
	lw	$5,4($2)
	jal	memcmp
	addiu	$5,$5,320

	bne	$2,$0,$L160
	li	$5,-1			# 0xffffffff

	addiu	$4,$18,24
	addiu	$5,$21,6070
	jal	memcmp
	li	$6,6			# 0x6

	beq	$2,$0,$L166
	addiu	$4,$17,256

$L167:
	j	$L160
	li	$5,-1			# 0xffffffff

$L166:
	li	$6,32			# 0x20
	addiu	$5,$18,30
	jal	memcpy
	addiu	$16,$sp,120

	move	$4,$21
	jal	wapiDerivedUCastKey
	addiu	$5,$sp,24

	li	$2,20			# 0x14
	lhu	$7,6($20)
	addiu	$4,$sp,56
	li	$5,16			# 0x10
	move	$6,$18
	addiu	$7,$7,-32
	sw	$2,20($sp)
	jal	sha256_hmac
	sw	$16,16($sp)

	move	$4,$16
	lhu	$5,6($20)
	li	$6,20			# 0x14
	addu	$5,$20,$5
	jal	memcmp
	addiu	$5,$5,-20

	bne	$2,$0,$L160
	li	$5,-1			# 0xffffffff

	lbu	$6,352($17)
	addiu	$4,$18,94
	jal	memcmp
	move	$5,$17

	beq	$2,$0,$L170
	move	$4,$21

	li	$2,27			# 0x1b
	li	$5,-2			# 0xfffffffe
	j	$L160
	sw	$2,0($22)

$L170:
	move	$5,$19
	jal	wapiInstallUCastKey
	addiu	$6,$sp,24

	lw	$4,656($17)
	beq	$4,$0,$L171
	nop

	jal	kfree
	nop

	sw	$0,656($17)
	sw	$0,592($17)
$L171:
	lw	$3,364($17)
	li	$2,6			# 0x6
	bne	$3,$2,$L172
	li	$3,8			# 0x8

	lw	$2,352($17)
	sw	$3,364($17)
	ori	$2,$2,0x6000
	sw	$2,352($17)
$L172:
	lw	$2,652($17)
	lw	$4,8($2)
	li	$2,2			# 0x2
	lw	$3,5240($4)
	beq	$3,$2,$L174
	li	$2,4			# 0x4

	bne	$3,$2,$L173
	addiu	$16,$17,624

$L174:
	lw	$3,5244($4)
	lui	$5,%hi(jiffies)
	sll	$2,$3,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$2,$2,2
	bgez	$2,$L175
	addiu	$4,$17,624

	lw	$2,%lo(jiffies)($5)
	li	$5,2147418112			# 0x7fff0000
	ori	$5,$5,0xffff
	j	$L178
	addu	$5,$2,$5

$L175:
	lw	$5,%lo(jiffies)($5)
	addu	$5,$5,$2
$L178:
	jal	mod_timer
	nop

	j	$L177
	nop

$L173:
	jal	del_timer
	move	$4,$16

	move	$4,$16
	move	$5,$0
	jal	init_timer_key
	move	$6,$0

	lui	$2,%hi(wapiUCastUpdateKeyTimeout)
	addiu	$2,$2,%lo(wapiUCastUpdateKeyTimeout)
	sw	$21,640($17)
	sw	$2,636($17)
$L177:
	lw	$3,352($17)
	li	$2,-253			# 0xffffff03
	lw	$4,652($17)
	move	$5,$0
	and	$3,$3,$2
	sw	$3,352($17)
	lw	$2,8($4)
	lw	$2,5248($2)
	sw	$2,596($17)
$L160:
	lw	$31,172($sp)
	move	$2,$5
	lw	$22,168($sp)
	lw	$21,164($sp)
	lw	$20,160($sp)
	lw	$19,156($sp)
	lw	$18,152($sp)
	lw	$17,148($sp)
	lw	$16,144($sp)
	j	$31
	addiu	$sp,$sp,176

	.set	macro
	.set	reorder
	.end	wapiRecvUnicastKeyAgreementResponse
	.section	.text.wapiRecvMulticastKeyResponse,"ax",@progbits
	.align	2
	.ent	wapiRecvMulticastKeyResponse
	.type	wapiRecvMulticastKeyResponse, @function
wapiRecvMulticastKeyResponse:
	.set	nomips16
	.frame	$sp,72,$31		# vars= 24, regs= 6/0, args= 24, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	li	$2,12			# 0xc
	sw	$20,64($sp)
	move	$20,$5
	sw	$19,60($sp)
	move	$19,$4
	sw	$31,68($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	lbu	$3,3($4)
	bne	$3,$2,$L180
	li	$4,-1			# 0xffffffff

	lw	$17,6676($5)
	addiu	$16,$19,12
	lbu	$3,2($16)
	lw	$2,352($17)
	lw	$18,652($17)
	srl	$2,$2,15
	andi	$2,$2,0x1
	bne	$3,$2,$L180
	nop

	lw	$4,1232($18)
	lbu	$3,1($16)
	srl	$2,$4,15
	andi	$2,$2,0x1
	beq	$3,$2,$L184
	andi	$2,$4,0x2000

	beq	$2,$0,$L180
	li	$4,-1			# 0xffffffff

$L184:
	addiu	$4,$19,27
	addiu	$5,$18,960
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L180
	li	$4,-1			# 0xffffffff

	lw	$5,4($18)
	addiu	$4,$19,15
	li	$6,6			# 0x6
	jal	memcmp
	addiu	$5,$5,320

	bne	$2,$0,$L180
	li	$4,-1			# 0xffffffff

	addiu	$4,$19,21
	addiu	$5,$20,6070
	jal	memcmp
	li	$6,6			# 0x6

	beq	$2,$0,$L182
	li	$2,20			# 0x14

	j	$L180
	li	$4,-1			# 0xffffffff

$L182:
	move	$6,$16
	addiu	$16,$sp,24
	addiu	$4,$17,568
	li	$5,16			# 0x10
	li	$7,31			# 0x1f
	sw	$2,20($sp)
	jal	sha256_hmac
	sw	$16,16($sp)

	move	$4,$16
	addiu	$5,$19,43
	jal	memcmp
	li	$6,20			# 0x14

	bne	$2,$0,$L180
	li	$4,-1			# 0xffffffff

	addiu	$16,$17,600
	li	$2,13			# 0xd
	move	$4,$16
	jal	del_timer
	sw	$2,364($17)

	move	$4,$16
	move	$5,$0
	jal	init_timer_key
	move	$6,$0

	li	$3,-253			# 0xffffff03
	lw	$2,352($17)
	sw	$20,616($17)
	and	$2,$2,$3
	ori	$2,$2,0x800
	sw	$2,352($17)
	lui	$2,%hi(wapiResendTimeout)
	addiu	$2,$2,%lo(wapiResendTimeout)
	sw	$2,612($17)
	lw	$2,1304($18)
	bne	$2,$0,$L186
	nop

	lw	$2,8($18)
	lw	$2,5260($2)
	sw	$2,1304($18)
$L186:
	lw	$2,1232($18)
	andi	$2,$2,0x1000
	beq	$2,$0,$L180
	move	$4,$0

	lw	$4,8($18)
	li	$2,2			# 0x2
	lw	$3,5252($4)
	beq	$3,$2,$L188
	li	$2,4			# 0x4

	bne	$3,$2,$L187
	nop

$L188:
	lw	$3,5256($4)
	lui	$5,%hi(jiffies)
	sll	$2,$3,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$2,$2,2
	bgez	$2,$L189
	addiu	$4,$18,1308

	lw	$2,%lo(jiffies)($5)
	li	$5,2147418112			# 0x7fff0000
	ori	$5,$5,0xffff
	j	$L191
	addu	$5,$2,$5

$L189:
	lw	$5,%lo(jiffies)($5)
	addu	$5,$5,$2
$L191:
	jal	mod_timer
	nop

	li	$3,-4097			# 0xffffefff
	lw	$2,1232($18)
	and	$2,$2,$3
	sw	$2,1232($18)
$L187:
	move	$4,$0
$L180:
	lw	$31,68($sp)
	move	$2,$4
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,72

	.set	macro
	.set	reorder
	.end	wapiRecvMulticastKeyResponse
	.section	.text.wapiIEInfoInstall,"ax",@progbits
	.align	2
	.globl	wapiIEInfoInstall
	.ent	wapiIEInfoInstall
	.type	wapiIEInfoInstall, @function
wapiIEInfoInstall:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-40
	li	$6,3			# 0x3
	sw	$20,32($sp)
	lui	$20,%hi(WAPI_KM_OUI)
	sw	$19,28($sp)
	move	$19,$5
	sw	$18,24($sp)
	move	$18,$4
	sw	$16,16($sp)
	sw	$31,36($sp)
	sw	$17,20($sp)
	lw	$17,6676($5)
	addiu	$5,$20,%lo(WAPI_KM_OUI)
	addiu	$16,$17,6
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L192
	li	$2,31			# 0x1f
	.set	macro
	.set	reorder

	lw	$2,8($18)
	lbu	$3,3($16)
	lw	$2,5192($2)
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L192
	li	$2,31			# 0x1f
	.set	macro
	.set	reorder

	lw	$2,6676($19)
	addiu	$16,$17,12
	move	$4,$16
	addiu	$5,$20,%lo(WAPI_KM_OUI)
	sw	$3,360($2)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,3			# 0x3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L192
	li	$2,28			# 0x1c
	.set	macro
	.set	reorder

	lbu	$19,3($16)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$19,$2,$L192
	li	$2,28			# 0x1c
	.set	macro
	.set	reorder

	addiu	$16,$17,16
	addiu	$5,$20,%lo(WAPI_KM_OUI)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,3			# 0x3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L192
	li	$2,29			# 0x1d
	.set	macro
	.set	reorder

	lbu	$2,3($16)
	.set	noreorder
	.set	nomacro
	bne	$2,$19,$L192
	li	$2,29			# 0x1d
	.set	macro
	.set	reorder

#APP
	raw_local_irq_save	$16
#NO_APP
	addiu	$2,$18,978
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	wapiSetIE
	sw	$2,908($18)
	.set	macro
	.set	reorder

	lbu	$4,1232($18)
	addiu	$2,$4,2
	sb	$2,1233($18)
	li	$2,68
	sb	$2,976($18)
	sb	$4,977($18)
#APP
	raw_local_irq_restore	$16
#NO_APP
	move	$2,$0
$L192:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,40
	.set	macro
	.set	reorder

	.end	wapiIEInfoInstall
	.section	.text.wapiSetBKByCA,"ax",@progbits
	.align	2
	.globl	wapiSetBKByCA
	.ent	wapiSetBKByCA
	.type	wapiSetBKByCA, @function
wapiSetBKByCA:
	.set	nomips16
	.frame	$sp,72,$31		# vars= 16, regs= 7/0, args= 24, gp= 0
	.mask	0x803f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	sw	$31,64($sp)
	sw	$21,60($sp)
	li	$21,16			# 0x10
	sw	$20,56($sp)
	sw	$19,52($sp)
	move	$19,$4
	sw	$16,40($sp)
	lui	$16,%hi($LC3)
	sw	$18,48($sp)
	addiu	$16,$16,%lo($LC3)
	sw	$17,44($sp)
	lw	$17,6676($4)
	move	$4,$16
	lw	$18,652($17)
	jal	strlen
	addiu	$20,$17,456

	move	$6,$16
	lw	$4,8($18)
	move	$7,$2
	lw	$5,5236($4)
	addiu	$4,$4,5204
	sw	$20,16($sp)
	jal	KD_hmac_sha256
	sw	$21,20($sp)

	addiu	$4,$sp,24
	lw	$5,4($18)
	li	$6,6			# 0x6
	jal	memcpy
	addiu	$5,$5,320

	addiu	$4,$sp,30
	addiu	$5,$19,6070
	jal	memcpy
	li	$6,6			# 0x6

	addiu	$2,$17,472
	move	$4,$20
	addiu	$6,$sp,24
	li	$5,16			# 0x10
	li	$7,12			# 0xc
	sw	$2,16($sp)
	jal	KD_hmac_sha256
	sw	$21,20($sp)

	addiu	$4,$17,288
	jal	GenerateRandomData
	li	$5,32			# 0x20

	li	$2,5			# 0x5
	lw	$3,6676($19)
	lw	$31,64($sp)
	lw	$21,60($sp)
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,72
	j	$31
	sw	$2,364($3)

	.set	macro
	.set	reorder
	.end	wapiSetBKByCA
	.section	.text.wapiSendMulticastKeyNotification,"ax",@progbits
	.align	2
	.globl	wapiSendMulticastKeyNotification
	.ent	wapiSendMulticastKeyNotification
	.type	wapiSendMulticastKeyNotification, @function
wapiSendMulticastKeyNotification:
	.set	nomips16
	.frame	$sp,72,$31		# vars= 0, regs= 9/0, args= 32, gp= 0
	.mask	0x80ff0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	lui	$2,%hi(jiffies)
	sw	$23,60($sp)
	sw	$21,52($sp)
	addiu	$21,$4,960
	sw	$20,48($sp)
	move	$20,$4
	sw	$16,32($sp)
	move	$4,$21
	sw	$31,64($sp)
	move	$16,$5
	sw	$22,56($sp)
	li	$5,1			# 0x1
	sw	$19,44($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	lw	$2,%lo(jiffies)($2)
	lw	$19,6676($16)
	jal	WapiIncreasePN
	addiu	$23,$2,100

	bne	$2,$0,$L223
	move	$4,$20

	jal	wapiFreeAllSta
	li	$5,1			# 0x1

	j	$L222
	nop

$L223:
	jal	alloc_skb_from_queue
	move	$4,$20

	bne	$2,$0,$L238
	move	$18,$2

	jal	dev_alloc_skb
	li	$4,1560			# 0x618

	beq	$2,$0,$L227
	move	$18,$2

$L238:
	addiu	$22,$16,6070
	li	$16,-30540
	lw	$4,176($18)
	move	$5,$22
	sh	$16,106($18)
	jal	memcpy
	li	$6,6			# 0x6

	li	$6,6			# 0x6
	lw	$4,176($18)
	lw	$5,4($20)
	addiu	$4,$4,6
	jal	memcpy
	addiu	$5,$5,320

	move	$4,$18
	lw	$2,176($18)
	li	$5,14			# 0xe
	jal	skb_put
	sh	$16,12($2)

	lw	$2,84($18)
	lw	$4,176($18)
	lw	$3,4($20)
	addu	$16,$4,$2
	li	$2,12
	sw	$3,24($18)
	sh	$2,6($16)
	li	$2,1			# 0x1
	sh	$2,0($16)
	sb	$2,2($16)
	li	$2,11			# 0xb
	sb	$2,3($16)
	sh	$0,4($16)
	sb	$0,10($16)
	sb	$0,11($16)
	lhu	$2,356($19)
	addiu	$2,$2,1
	sh	$2,356($19)
	lhu	$3,6($16)
	sh	$2,8($16)
	addu	$17,$16,$3
	sb	$0,0($17)
	lw	$2,364($19)
	lw	$3,1232($20)
	addiu	$2,$2,-9
	sltu	$2,$2,2
	bne	$2,$0,$L239
	srl	$2,$3,15

	xori	$2,$2,0x1
$L239:
	andi	$2,$2,0x1
	addiu	$4,$17,3
	sb	$2,1($17)
	li	$6,6			# 0x6
	lw	$2,352($19)
	srl	$2,$2,15
	andi	$2,$2,0x1
	sb	$2,2($17)
	lw	$5,4($20)
	jal	memcpy
	addiu	$5,$5,320

	move	$5,$22
	addiu	$4,$17,9
	jal	memcpy
	li	$6,6			# 0x6

	addiu	$4,$17,15
	addiu	$5,$20,928
	jal	memcpy
	li	$6,16			# 0x10

	addiu	$4,$17,31
	move	$5,$21
	jal	memcpy
	li	$6,16			# 0x10

	addiu	$3,$17,48
	lhu	$2,6($16)
	move	$5,$21
	addiu	$4,$19,552
	addiu	$6,$20,912
	addiu	$2,$2,47
	li	$7,16			# 0x10
	sh	$2,6($16)
	addiu	$2,$17,47
	sw	$3,16($sp)
	sw	$2,20($sp)
	jal	WapiSMS4ForMNKEncrypt
	sw	$0,24($sp)

	move	$6,$17
	lbu	$3,47($17)
	addiu	$4,$19,568
	lhu	$2,6($16)
	li	$5,16			# 0x10
	addu	$2,$2,$3
	addiu	$2,$2,1
	andi	$7,$2,0xffff
	addu	$3,$17,$7
	addiu	$7,$7,-12
	sh	$2,6($16)
	addiu	$3,$3,-12
	li	$2,20			# 0x14
	sw	$3,16($sp)
	jal	sha256_hmac
	sw	$2,20($sp)

	move	$4,$18
	lhu	$5,6($16)
	addiu	$5,$5,20
	sh	$5,6($16)
	jal	skb_put
	andi	$5,$5,0xffff

	lw	$5,4($20)
	lw	$3,84($18)
	lw	$2,208($5)
	addiu	$3,$3,-14
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L231
	nop

	move	$5,$20
	jal	wapiFragementSend
	move	$4,$18

	li	$3,1			# 0x1
	beq	$2,$3,$L234
	nop

	j	$L240
	nop

$L231:
	jal	rtl8192cd_start_xmit
	move	$4,$18

	beq	$2,$0,$L234
	nop

$L240:
	jal	dev_kfree_skb_any
	move	$4,$18

$L234:
	lw	$3,364($19)
	li	$2,9			# 0x9
	bne	$3,$2,$L241
	addiu	$4,$19,600

	li	$2,10			# 0xa
	sw	$2,364($19)
$L227:
	addiu	$4,$19,600
$L241:
	jal	mod_timer
	move	$5,$23

$L222:
	lw	$31,64($sp)
	move	$2,$0
	lw	$23,60($sp)
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,72

	.set	macro
	.set	reorder
	.end	wapiSendMulticastKeyNotification
	.section	.text.wapiUpdateMSK,"ax",@progbits
	.align	2
	.globl	wapiUpdateMSK
	.ent	wapiUpdateMSK
	.type	wapiUpdateMSK, @function
wapiUpdateMSK:
	.set	nomips16
	.frame	$sp,48,$31		# vars= 0, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$21,36($sp)
	sw	$17,20($sp)
	move	$17,$4
	sw	$31,44($sp)
	sw	$22,40($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$16,16($sp)
	lw	$2,8($4)
	li	$4,65536			# 0x10000
	ori	$4,$4,0x2b6c
	lw	$3,5256($2)
	addu	$4,$17,$4
	lw	$5,0($4)
	sll	$2,$3,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	beq	$5,$4,$L245
	sll	$21,$2,2

	addiu	$4,$17,912
	jal	GenerateRandomData
	li	$5,16			# 0x10

	li	$3,-16385			# 0xffffbfff
	lw	$2,1232($17)
	move	$20,$17
	li	$22,30			# 0x1e
	and	$2,$2,$3
	li	$3,-8193			# 0xffffdfff
	and	$2,$2,$3
	sw	$2,1232($17)
$L253:
	li	$2,65536			# 0x10000
	ori	$2,$2,0x2a74
	addu	$19,$20,$2
	lw	$18,0($19)
$L265:
	beq	$18,$19,$L262
	move	$16,$18

	lw	$2,6676($18)
	move	$5,$18
	move	$4,$17
	lw	$2,364($2)
	lw	$18,0($18)
	sltu	$2,$2,13
	beq	$2,$0,$L251
	li	$6,25			# 0x19

	jal	wapiDeauthSta
	nop

	j	$L265
	nop

$L251:
	lw	$2,1232($17)
	li	$3,-1025			# 0xfffffbff
	ori	$2,$2,0x2000
	sw	$2,1232($17)
	lw	$4,6676($16)
	lw	$2,352($4)
	and	$2,$2,$3
	sw	$2,352($4)
	lw	$2,6676($16)
	sh	$0,358($2)
	lhu	$2,1236($17)
	lw	$3,6676($16)
	sh	$2,356($3)
	lhu	$2,1236($17)
	addiu	$2,$2,8
	sh	$2,1236($17)
	lw	$4,6676($16)
	jal	wapiReleaseFragementQueue
	nop

	move	$4,$17
	jal	wapiSendMulticastKeyNotification
	move	$5,$16

	j	$L265
	nop

$L262:
	addiu	$22,$22,-1
	bgez	$22,$L253
	addiu	$20,$20,8

	lw	$2,1232($17)
	li	$3,600			# 0x258
	andi	$2,$2,0x2000
	movn	$21,$3,$2		#conditional move
$L245:
	addiu	$16,$17,1308
	jal	del_timer
	move	$4,$16

	move	$4,$16
	move	$5,$0
	jal	init_timer_key
	move	$6,$0

	lui	$3,%hi(wapiMCastUpdateKeyTimeout)
	lw	$2,8($17)
	addiu	$3,$3,%lo(wapiMCastUpdateKeyTimeout)
	sw	$3,1320($17)
	sw	$17,1324($17)
	lw	$3,5252($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L256
	li	$2,4			# 0x4

	bne	$3,$2,$L255
	nop

$L256:
	bgez	$21,$L257
	lui	$2,%hi(jiffies)

	lw	$2,%lo(jiffies)($2)
	li	$5,2147418112			# 0x7fff0000
	ori	$5,$5,0xffff
	move	$4,$16
	j	$L264
	addu	$5,$2,$5

$L257:
	lw	$5,%lo(jiffies)($2)
	move	$4,$16
	addu	$5,$5,$21
$L264:
	jal	mod_timer
	nop

$L255:
	lw	$31,44($sp)
	li	$2,1			# 0x1
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
	.end	wapiUpdateMSK
	.section	.text.wapiMCastUpdateKeyTimeout,"ax",@progbits
	.align	2
	.ent	wapiMCastUpdateKeyTimeout
	.type	wapiMCastUpdateKeyTimeout, @function
wapiMCastUpdateKeyTimeout:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$16,16($sp)
	sw	$31,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	lw	$2,1232($4)
	andi	$2,$2,0x2000
	beq	$2,$0,$L267
	move	$16,$4

	addiu	$17,$4,1308
	li	$18,-8193			# 0xffffdfff
	jal	del_timer
	move	$4,$17

	move	$4,$17
	move	$5,$0
	jal	init_timer_key
	move	$6,$0

	li	$4,65536			# 0x10000
	lw	$3,1232($16)
	addu	$4,$16,$4
	sw	$16,1324($16)
	and	$2,$3,$18
	andi	$3,$3,0x4000
	sw	$2,1232($16)
	lui	$2,%hi(wapiMCastUpdateKeyTimeout)
	addiu	$2,$2,%lo(wapiMCastUpdateKeyTimeout)
	sw	$2,1320($16)
	lw	$2,15804($4)
	bne	$3,$0,$L268
	move	$4,$16

	jal	wapiFreeAllSta
	move	$5,$0

	bne	$2,$0,$L277
	li	$3,-16385			# 0xffffbfff

	lw	$2,1232($16)
	and	$2,$2,$3
	li	$3,-65536			# 0xffff0000
	ori	$3,$3,0x7fff
	and	$2,$2,$3
	and	$2,$2,$18
	ori	$2,$2,0x1000
	j	$L266
	sw	$2,1232($16)

$L268:
	beq	$2,$0,$L266
	nop

$L277:
	lw	$4,8($16)
	li	$2,2			# 0x2
	lw	$3,5252($4)
	beq	$3,$2,$L271
	li	$2,4			# 0x4

	bne	$3,$2,$L266
	nop

$L271:
	lw	$3,5256($4)
	sll	$2,$3,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$2,$2,2
	bgez	$2,$L272
	lui	$4,%hi(jiffies)

	lw	$2,%lo(jiffies)($4)
	li	$5,2147418112			# 0x7fff0000
	ori	$5,$5,0xffff
	move	$4,$17
	j	$L278
	addu	$5,$2,$5

$L272:
	lw	$5,%lo(jiffies)($4)
	move	$4,$17
	j	$L278
	addu	$5,$5,$2

$L267:
	li	$2,65536			# 0x10000
	addu	$2,$4,$2
	lw	$2,15776($2)
	beq	$2,$0,$L275
	nop

	lw	$2,6676($2)
	beq	$2,$0,$L275
	nop

	lw	$2,352($2)
	andi	$2,$2,0x1000
	beq	$2,$0,$L275
	lui	$2,%hi(jiffies)

	addiu	$4,$4,1308
	lw	$5,%lo(jiffies)($2)
	addiu	$5,$5,100
$L278:
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	mod_timer
	addiu	$sp,$sp,32

$L275:
	move	$4,$16
	move	$5,$0
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	wapiUpdateMSK
	addiu	$sp,$sp,32

$L266:
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	wapiMCastUpdateKeyTimeout
	.section	.text.wapiSendUnicastKeyAgrementConfirm,"ax",@progbits
	.align	2
	.globl	wapiSendUnicastKeyAgrementConfirm
	.ent	wapiSendUnicastKeyAgrementConfirm
	.type	wapiSendUnicastKeyAgrementConfirm, @function
wapiSendUnicastKeyAgrementConfirm:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 0, regs= 7/0, args= 24, gp= 0
	.mask	0x803f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	sw	$20,40($sp)
	move	$20,$4
	sw	$19,36($sp)
	move	$19,$5
	sw	$17,28($sp)
	sw	$31,48($sp)
	sw	$21,44($sp)
	sw	$18,32($sp)
	jal	alloc_skb_from_queue
	sw	$16,24($sp)

	bne	$2,$0,$L294
	move	$17,$2

	jal	dev_alloc_skb
	li	$4,1560			# 0x618

	beq	$2,$0,$L283
	move	$17,$2

$L294:
	li	$16,-30540
	addiu	$21,$19,6070
	lw	$4,176($17)
	move	$5,$21
	sh	$16,106($17)
	jal	memcpy
	li	$6,6			# 0x6

	li	$6,6			# 0x6
	lw	$4,176($17)
	lw	$5,4($20)
	addiu	$4,$4,6
	jal	memcpy
	addiu	$5,$5,320

	move	$4,$17
	lw	$2,176($17)
	li	$5,14			# 0xe
	jal	skb_put
	sh	$16,12($2)

	lw	$3,4($20)
	lw	$2,84($17)
	lw	$4,176($17)
	sw	$3,24($17)
	addu	$18,$4,$2
	li	$2,12
	lw	$19,6676($19)
	sh	$2,6($18)
	li	$2,1			# 0x1
	sh	$2,0($18)
	sb	$2,2($18)
	li	$2,10			# 0xa
	sb	$2,3($18)
	sh	$0,4($18)
	sb	$0,10($18)
	sb	$0,11($18)
	lhu	$2,356($19)
	addiu	$2,$2,1
	sh	$2,356($19)
	lhu	$3,6($18)
	sh	$2,8($18)
	addu	$16,$18,$3
	lbu	$2,0($16)
	andi	$3,$2,0x10
	sb	$3,0($16)
	lw	$2,364($19)
	addiu	$2,$2,-8
	sltu	$2,$2,2
	beq	$2,$0,$L285
	ori	$2,$3,0x10

	sb	$0,0($16)
	j	$L286
	sb	$0,17($16)

$L285:
	sb	$2,0($16)
	lw	$2,352($19)
	srl	$2,$2,15
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sb	$2,17($16)
$L286:
	addiu	$4,$16,1
	addiu	$5,$19,472
	jal	memcpy
	li	$6,16			# 0x10

	addiu	$4,$16,18
	lw	$5,4($20)
	li	$6,6			# 0x6
	jal	memcpy
	addiu	$5,$5,320

	move	$5,$21
	addiu	$4,$16,24
	jal	memcpy
	li	$6,6			# 0x6

	addiu	$4,$16,30
	addiu	$5,$19,256
	jal	memcpy
	li	$6,32			# 0x20

	addiu	$4,$16,62
	lw	$5,652($19)
	lbu	$6,1233($20)
	jal	memcpy
	addiu	$5,$5,976

	move	$6,$16
	lbu	$7,1233($20)
	addiu	$4,$19,568
	li	$5,16			# 0x10
	addu	$2,$16,$7
	addiu	$7,$7,62
	addiu	$2,$2,62
	sw	$2,16($sp)
	li	$2,20			# 0x14
	jal	sha256_hmac
	sw	$2,20($sp)

	move	$4,$17
	lbu	$2,1233($20)
	lhu	$5,6($18)
	addu	$5,$5,$2
	addiu	$5,$5,82
	sh	$5,6($18)
	jal	skb_put
	andi	$5,$5,0xffff

	lw	$5,4($20)
	lw	$3,84($17)
	lw	$2,208($5)
	addiu	$3,$3,-14
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L287
	nop

	move	$5,$20
	jal	wapiFragementSend
	move	$4,$17

	li	$3,1			# 0x1
	beq	$2,$3,$L290
	nop

	j	$L295
	nop

$L287:
	jal	rtl8192cd_start_xmit
	move	$4,$17

	beq	$2,$0,$L290
	nop

$L295:
	jal	dev_kfree_skb_any
	move	$4,$17

$L290:
	lw	$3,364($19)
	li	$2,8			# 0x8
	bne	$3,$2,$L283
	li	$2,9			# 0x9

	sw	$2,364($19)
$L283:
	lw	$31,48($sp)
	move	$2,$0
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	wapiSendUnicastKeyAgrementConfirm
	.section	.text.wapiSendUnicastKeyAgrementRequeset,"ax",@progbits
	.align	2
	.globl	wapiSendUnicastKeyAgrementRequeset
	.ent	wapiSendUnicastKeyAgrementRequeset
	.type	wapiSendUnicastKeyAgrementRequeset, @function
wapiSendUnicastKeyAgrementRequeset:
	.set	nomips16
	.frame	$sp,48,$31		# vars= 0, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	lui	$2,%hi(jiffies)
	sw	$22,40($sp)
	sw	$20,32($sp)
	move	$20,$4
	sw	$17,20($sp)
	sw	$16,16($sp)
	move	$16,$5
	sw	$31,44($sp)
	sw	$21,36($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	lw	$2,%lo(jiffies)($2)
	lw	$19,6676($5)
	jal	alloc_skb_from_queue
	addiu	$22,$2,100

	bne	$2,$0,$L311
	move	$17,$2

	jal	dev_alloc_skb
	li	$4,1560			# 0x618

	beq	$2,$0,$L300
	move	$17,$2

$L311:
	addiu	$21,$16,6070
	li	$16,-30540
	lw	$4,176($17)
	move	$5,$21
	sh	$16,106($17)
	jal	memcpy
	li	$6,6			# 0x6

	li	$6,6			# 0x6
	lw	$4,176($17)
	lw	$5,4($20)
	addiu	$4,$4,6
	jal	memcpy
	addiu	$5,$5,320

	move	$4,$17
	lw	$2,176($17)
	li	$5,14			# 0xe
	jal	skb_put
	sh	$16,12($2)

	lw	$2,84($17)
	lw	$4,176($17)
	lw	$3,4($20)
	addu	$18,$4,$2
	li	$2,12
	sw	$3,24($17)
	sh	$2,6($18)
	li	$2,1			# 0x1
	sh	$2,0($18)
	sb	$2,2($18)
	li	$2,8			# 0x8
	sb	$2,3($18)
	sh	$0,4($18)
	sb	$0,10($18)
	sb	$0,11($18)
	lhu	$2,356($19)
	addiu	$2,$2,1
	sh	$2,356($19)
	lhu	$3,6($18)
	sh	$2,8($18)
	addu	$16,$18,$3
	lbu	$2,0($16)
	andi	$3,$2,0x10
	sb	$3,0($16)
	lw	$2,364($19)
	addiu	$2,$2,-5
	sltu	$2,$2,2
	beq	$2,$0,$L302
	ori	$2,$3,0x10

	sb	$0,0($16)
	li	$3,-65536			# 0xffff0000
	sb	$0,17($16)
	ori	$3,$3,0x7fff
	lw	$2,352($19)
	and	$2,$2,$3
	li	$3,-4097			# 0xffffefff
	and	$2,$2,$3
	j	$L303
	sw	$2,352($19)

$L302:
	sb	$2,0($16)
	lw	$2,352($19)
	srl	$2,$2,15
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sb	$2,17($16)
$L303:
	addiu	$4,$16,1
	addiu	$5,$19,472
	jal	memcpy
	li	$6,16			# 0x10

	addiu	$4,$16,18
	lw	$5,4($20)
	li	$6,6			# 0x6
	jal	memcpy
	addiu	$5,$5,320

	move	$5,$21
	addiu	$4,$16,24
	jal	memcpy
	li	$6,6			# 0x6

	addiu	$4,$16,30
	addiu	$5,$19,288
	jal	memcpy
	li	$6,32			# 0x20

	move	$4,$17
	lhu	$5,6($18)
	addiu	$5,$5,62
	sh	$5,6($18)
	jal	skb_put
	andi	$5,$5,0xffff

	lw	$5,4($20)
	lw	$3,84($17)
	lw	$2,208($5)
	addiu	$3,$3,-14
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L304
	nop

	move	$5,$20
	jal	wapiFragementSend
	move	$4,$17

	li	$3,1			# 0x1
	beq	$2,$3,$L307
	nop

	j	$L312
	nop

$L304:
	jal	rtl8192cd_start_xmit
	move	$4,$17

	beq	$2,$0,$L307
	nop

$L312:
	jal	dev_kfree_skb_any
	move	$4,$17

$L307:
	lw	$3,364($19)
	li	$2,5			# 0x5
	bne	$3,$2,$L313
	addiu	$4,$19,600

	li	$2,6			# 0x6
	sw	$2,364($19)
$L300:
	addiu	$4,$19,600
$L313:
	jal	mod_timer
	move	$5,$22

	move	$2,$0
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
	.end	wapiSendUnicastKeyAgrementRequeset
	.section	.text.wapiUpdateUSK,"ax",@progbits
	.align	2
	.globl	wapiUpdateUSK
	.ent	wapiUpdateUSK
	.type	wapiUpdateUSK, @function
wapiUpdateUSK:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$18,24($sp)
	move	$18,$4
	sw	$17,20($sp)
	move	$17,$5
	sw	$16,16($sp)
	lw	$16,6676($5)
	lw	$2,352($16)
	move	$4,$16
	lw	$3,652($16)
	ori	$2,$2,0x1000
	sw	$2,352($16)
	sh	$0,358($16)
	lhu	$2,1236($3)
	sh	$2,356($16)
	lhu	$2,1236($3)
	addiu	$2,$2,8
	jal	wapiReleaseFragementQueue
	sh	$2,1236($3)

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	addiu	$4,$16,624
	jal	mod_timer
	addiu	$5,$5,600

	move	$4,$18
	move	$5,$17
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	wapiSendUnicastKeyAgrementRequeset
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	wapiUpdateUSK
	.section	.text.wapiUCastUpdateKeyTimeout,"ax",@progbits
	.align	2
	.ent	wapiUCastUpdateKeyTimeout
	.type	wapiUCastUpdateKeyTimeout, @function
wapiUCastUpdateKeyTimeout:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lw	$3,6676($4)
	move	$8,$4
	move	$5,$4
	li	$6,25			# 0x19
	lw	$2,352($3)
	lw	$7,652($3)
	andi	$2,$2,0x1000
	beq	$2,$0,$L316
	move	$4,$7

	j	wapiDeauthSta
	nop

$L316:
	lw	$2,1232($7)
	addiu	$4,$3,624
	andi	$2,$2,0x2000
	beq	$2,$0,$L318
	move	$5,$8

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	j	mod_timer
	addiu	$5,$5,100

$L318:
	j	wapiUpdateUSK
	move	$4,$7

	.set	macro
	.set	reorder
	.end	wapiUCastUpdateKeyTimeout
	.section	.text.WapiSendActivateAuthenticationPacket,"ax",@progbits
	.align	2
	.ent	WapiSendActivateAuthenticationPacket
	.type	WapiSendActivateAuthenticationPacket, @function
WapiSendActivateAuthenticationPacket:
	.set	nomips16
	.frame	$sp,64,$31		# vars= 8, regs= 9/0, args= 16, gp= 0
	.mask	0x80ff0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	sw	$22,48($sp)
	move	$22,$7
	sw	$21,44($sp)
	move	$21,$6
	sw	$20,40($sp)
	li	$6,-1			# 0xffffffff
	sw	$18,32($sp)
	move	$18,$5
	sw	$31,56($sp)
	sw	$23,52($sp)
	sw	$19,36($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	lhu	$5,2($7)
	lw	$19,6676($18)
	addu	$2,$7,$5
	addiu	$2,$2,4
	lhu	$3,2($2)
	addu	$2,$2,$3
	addu	$5,$5,$3
	addiu	$2,$2,4
	lbu	$3,1($2)
	lbu	$2,2($2)
	sll	$3,$3,8
	or	$2,$2,$3
	addu	$5,$5,$2
	addiu	$5,$5,11
	bne	$5,$21,$L320
	move	$20,$4

	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	jal	alloc_skb_from_queue
	addiu	$23,$2,100

	bne	$2,$0,$L387
	move	$17,$2

	jal	dev_alloc_skb
	li	$4,1560			# 0x618

	beq	$2,$0,$L325
	move	$17,$2

$L387:
	lw	$2,656($19)
	bne	$2,$0,$L391
	li	$16,-30540

$L331:
$L372:
	lui	$2,%hi(malloc_sizes+76)
	lw	$4,%lo(malloc_sizes+76)($2)
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	bne	$2,$0,$L326
	sw	$2,656($19)

	jal	dev_kfree_skb_any
	move	$4,$17

	j	$L389
	addiu	$4,$19,600

$L326:
	li	$16,-30540
$L391:
	addiu	$5,$18,6070
	lw	$4,176($17)
	li	$6,6			# 0x6
	jal	memcpy
	sh	$16,106($17)

	li	$6,6			# 0x6
	lw	$4,176($17)
	lw	$5,4($20)
	addiu	$4,$4,6
	jal	memcpy
	addiu	$5,$5,320

	move	$4,$17
	lw	$2,176($17)
	li	$5,14			# 0xe
	sh	$16,12($2)
	lw	$2,4($20)
	jal	skb_put
	sw	$2,24($17)

	li	$4,1
	lw	$3,176($17)
	lw	$2,84($17)
	addu	$16,$3,$2
	li	$2,12
	sh	$2,6($16)
	li	$2,3			# 0x3
	sb	$4,2($16)
	sb	$2,3($16)
	sh	$4,0($16)
	sh	$0,4($16)
	sb	$0,10($16)
	sb	$0,11($16)
	lhu	$2,356($19)
	addiu	$2,$2,1
	sh	$2,356($19)
	lhu	$3,6($16)
	sh	$2,8($16)
	addu	$18,$16,$3
	lbu	$2,0($18)
	andi	$3,$2,0x1
	sb	$3,0($18)
	lw	$2,364($19)
	addiu	$2,$2,-1
	sltu	$2,$2,2
	beq	$2,$0,$L378
	ori	$2,$3,0x1

	sb	$0,0($18)
	addiu	$4,$19,320
	jal	GenerateRandomData
	li	$5,32			# 0x20

	j	$L390
	addiu	$4,$18,1

$L378:
	sb	$2,0($18)
	addiu	$4,$18,1
$L390:
	addiu	$5,$19,320
	jal	memcpy
	li	$6,32			# 0x20

	addiu	$4,$18,33
	move	$5,$22
	jal	memcpy
	move	$6,$21

	move	$4,$17
	lhu	$5,6($16)
	addu	$5,$21,$5
	addiu	$5,$5,33
	sh	$5,6($16)
	jal	skb_put
	andi	$5,$5,0xffff

	lw	$5,4($20)
	lw	$3,84($17)
	lw	$2,208($5)
	addiu	$3,$3,-14
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L380
	nop

	move	$5,$20
	jal	wapiFragementSend
	move	$4,$17

	li	$3,1			# 0x1
	beq	$2,$3,$L383
	nop

	j	$L388
	nop

$L380:
	jal	rtl8192cd_start_xmit
	move	$4,$17

	beq	$2,$0,$L383
	nop

$L388:
	jal	dev_kfree_skb_any
	move	$4,$17

$L383:
	lw	$3,364($19)
	li	$2,1			# 0x1
	bne	$3,$2,$L389
	addiu	$4,$19,600

	lw	$4,656($19)
	move	$5,$22
	jal	memcpy
	move	$6,$21

	li	$2,2			# 0x2
	sw	$21,592($19)
	sw	$2,364($19)
$L325:
	addiu	$4,$19,600
$L389:
	jal	mod_timer
	move	$5,$23

	move	$6,$0
$L320:
	lw	$31,56($sp)
	move	$2,$6
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,64

	.set	macro
	.set	reorder
	.end	WapiSendActivateAuthenticationPacket
	.section	.text.WapiSendAuthenticationRspPacket,"ax",@progbits
	.align	2
	.ent	WapiSendAuthenticationRspPacket
	.type	WapiSendAuthenticationRspPacket, @function
WapiSendAuthenticationRspPacket:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 0, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	lui	$2,%hi(jiffies)
	sw	$fp,48($sp)
	move	$fp,$7
	sw	$23,44($sp)
	li	$23,1			# 0x1
	sw	$22,40($sp)
	sw	$21,36($sp)
	move	$21,$6
	sw	$20,32($sp)
	move	$20,$4
	sw	$19,28($sp)
	move	$19,$5
	sw	$17,20($sp)
	sw	$31,52($sp)
	sw	$18,24($sp)
	sw	$16,16($sp)
	lw	$2,%lo(jiffies)($2)
	lw	$18,6676($5)
	jal	alloc_skb_from_queue
	addiu	$22,$2,100

	bne	$2,$0,$L407
	move	$17,$2

	jal	dev_alloc_skb
	li	$4,2688			# 0xa80

	beq	$2,$0,$L396
	move	$17,$2

$L407:
	lw	$2,656($18)
	bne	$2,$0,$L397
	li	$16,-30540

	jal	dev_kfree_skb_any
	move	$4,$17

	j	$L409
	addiu	$4,$18,600

$L397:
	addiu	$5,$19,6070
	lw	$4,176($17)
	li	$6,6			# 0x6
	jal	memcpy
	sh	$16,106($17)

	li	$6,6			# 0x6
	lw	$4,176($17)
	lw	$5,4($20)
	addiu	$4,$4,6
	jal	memcpy
	addiu	$5,$5,320

	move	$4,$17
	lw	$2,176($17)
	li	$5,14			# 0xe
	sh	$16,12($2)
	lw	$2,4($20)
	jal	skb_put
	sw	$2,24($17)

	move	$5,$fp
	lw	$2,84($17)
	move	$6,$21
	lw	$16,176($17)
	addu	$16,$16,$2
	li	$2,12
	sh	$2,6($16)
	li	$2,1
	sb	$2,2($16)
	li	$2,5			# 0x5
	sb	$2,3($16)
	sh	$23,0($16)
	sh	$0,4($16)
	sb	$0,10($16)
	sb	$0,11($16)
	lhu	$2,356($18)
	addiu	$2,$2,1
	sh	$2,356($18)
	lhu	$4,6($16)
	sh	$2,8($16)
	jal	memcpy
	addu	$4,$16,$4

	move	$4,$17
	lhu	$5,6($16)
	addu	$5,$21,$5
	sh	$5,6($16)
	jal	skb_put
	andi	$5,$5,0xffff

	lw	$5,4($20)
	lw	$3,84($17)
	lw	$2,208($5)
	addiu	$3,$3,-14
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L400
	nop

	move	$5,$20
	jal	wapiFragementSend
	move	$4,$17

	beq	$2,$23,$L403
	nop

	j	$L408
	nop

$L400:
	jal	rtl8192cd_start_xmit
	move	$4,$17

	beq	$2,$0,$L403
	nop

$L408:
	jal	dev_kfree_skb_any
	move	$4,$17

$L403:
	lw	$3,364($18)
	li	$2,5			# 0x5
	bne	$3,$2,$L409
	addiu	$4,$18,600

	lw	$4,656($18)
	move	$5,$fp
	jal	memcpy
	move	$6,$21

	sw	$21,592($18)
$L396:
	addiu	$4,$18,600
$L409:
	jal	mod_timer
	move	$5,$22

	move	$2,$0
	lw	$31,52($sp)
	lw	$fp,48($sp)
	lw	$23,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	WapiSendAuthenticationRspPacket
	.section	.text.DOT11_Process_WAPI_Info,"ax",@progbits
	.align	2
	.globl	DOT11_Process_WAPI_Info
	.ent	DOT11_Process_WAPI_Info
	.type	DOT11_Process_WAPI_Info, @function
DOT11_Process_WAPI_Info:
	.set	nomips16
	.frame	$sp,48,$31		# vars= 0, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-48
	li	$3,-1			# 0xffffffff
	sw	$21,36($sp)
	move	$21,$6
	sw	$20,32($sp)
	move	$20,$5
	sw	$19,28($sp)
	sw	$31,40($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lw	$18,4($5)
	lw	$17,6676($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L410
	move	$19,$4
	.set	macro
	.set	reorder

	addiu	$16,$5,8
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$4,$16
	.set	macro
	.set	reorder

	move	$5,$16
	lw	$4,4($19)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L410
	li	$3,-1			# 0xffffffff
	.set	macro
	.set	reorder

	lhu	$3,2($20)
	li	$4,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$4,$L416
	slt	$2,$3,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L438
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L414
	move	$3,$0
	.set	macro
	.set	reorder

	j	$L410
$L438:
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L433
	move	$3,$0
	.set	macro
	.set	reorder

	j	$L410
$L414:
	lw	$2,364($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$4,$L413
	addiu	$16,$17,624
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	del_timer
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	init_timer_key
	move	$6,$0
	.set	macro
	.set	reorder

	li	$2,-253			# 0xffffff03
	lw	$3,352($17)
	move	$4,$19
	move	$5,$18
	addiu	$6,$21,-24
	and	$3,$3,$2
	lui	$2,%hi(wapiUCastUpdateKeyTimeout)
	addiu	$2,$2,%lo(wapiUCastUpdateKeyTimeout)
	addiu	$7,$20,24
	sw	$3,352($17)
	sw	$2,636($17)
	.set	noreorder
	.set	nomacro
	jal	WapiSendActivateAuthenticationPacket
	sw	$18,640($17)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L410
	move	$3,$0
	.set	macro
	.set	reorder

$L416:
	lw	$3,364($17)
	li	$2,3			# 0x3
	bne	$3,$2,$L413
#APP
	raw_local_irq_save	$16
#NO_APP
	addiu	$2,$20,24
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	wapiSetBK
	sw	$2,908($19)
	.set	macro
	.set	reorder

	sw	$0,908($19)
#APP
	raw_local_irq_restore	$16
#NO_APP
	.set	noreorder
	.set	nomacro
	j	$L410
	move	$3,$0
	.set	macro
	.set	reorder

$L433:
	lw	$3,364($17)
	li	$2,5			# 0x5
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L410
	move	$3,$0
	.set	macro
	.set	reorder

	addiu	$16,$20,24
	addiu	$6,$21,-24
	move	$4,$19
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	WapiSendAuthenticationRspPacket
	move	$7,$16
	.set	macro
	.set	reorder

	lbu	$2,65($16)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L435
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	wapiSendUnicastKeyAgrementRequeset
	move	$5,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L410
	move	$3,$0
	.set	macro
	.set	reorder

$L435:
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	wapiDeauthSta
	li	$6,24			# 0x18
	.set	macro
	.set	reorder

$L413:
	move	$3,$0
$L410:
	lw	$31,40($sp)
	move	$2,$3
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,48
	.set	macro
	.set	reorder

	.end	DOT11_Process_WAPI_Info
	.section	.text.WapiSendUnicastKeyAggrementResponse,"ax",@progbits
	.align	2
	.ent	WapiSendUnicastKeyAggrementResponse
	.type	WapiSendUnicastKeyAggrementResponse, @function
WapiSendUnicastKeyAggrementResponse:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 0, regs= 8/0, args= 24, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	lui	$2,%hi(jiffies)
	sw	$22,48($sp)
	sw	$20,40($sp)
	move	$20,$4
	sw	$17,28($sp)
	sw	$16,24($sp)
	move	$16,$5
	sw	$31,52($sp)
	sw	$21,44($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	lw	$2,%lo(jiffies)($2)
	lw	$18,6676($5)
	jal	alloc_skb_from_queue
	addiu	$22,$2,100

	bne	$2,$0,$L455
	move	$17,$2

	jal	dev_alloc_skb
	li	$4,1560			# 0x618

	beq	$2,$0,$L443
	move	$17,$2

$L455:
	addiu	$21,$16,6070
	li	$16,-30540
	lw	$4,176($17)
	move	$5,$21
	sh	$16,106($17)
	jal	memcpy
	li	$6,6			# 0x6

	li	$6,6			# 0x6
	lw	$4,176($17)
	lw	$5,4($20)
	addiu	$4,$4,6
	jal	memcpy
	addiu	$5,$5,320

	move	$4,$17
	lw	$2,176($17)
	li	$5,14			# 0xe
	sh	$16,12($2)
	lw	$2,4($20)
	jal	skb_put
	sw	$2,24($17)

	lw	$3,176($17)
	lw	$2,84($17)
	addu	$19,$3,$2
	li	$2,12
	sh	$2,6($19)
	li	$2,1			# 0x1
	sh	$2,0($19)
	sb	$2,2($19)
	li	$2,9			# 0x9
	sb	$2,3($19)
	sh	$0,4($19)
	sb	$0,10($19)
	sb	$0,11($19)
	lhu	$2,356($18)
	addiu	$2,$2,1
	sh	$2,356($18)
	lhu	$3,6($19)
	sh	$2,8($19)
	addu	$16,$19,$3
	lbu	$2,0($16)
	andi	$4,$2,0x10
	li	$2,5			# 0x5
	sb	$4,0($16)
	lw	$3,364($18)
	beq	$3,$2,$L446
	li	$2,7			# 0x7

	bne	$3,$2,$L445
	ori	$2,$4,0x10

$L446:
	sb	$0,0($16)
	lw	$2,352($18)
	srl	$2,$2,15
	andi	$2,$2,0x1
	sb	$2,17($16)
	li	$2,-65536			# 0xffff0000
	lw	$3,352($18)
	ori	$2,$2,0x7fff
	and	$3,$3,$2
	li	$2,-4097			# 0xffffefff
	and	$3,$3,$2
	j	$L447
	sw	$3,352($18)

$L445:
	sb	$2,0($16)
	lw	$2,352($18)
	srl	$2,$2,15
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sb	$2,17($16)
$L447:
	addiu	$4,$16,1
	addiu	$5,$18,472
	jal	memcpy
	li	$6,16			# 0x10

	addiu	$4,$16,24
	lw	$5,4($20)
	li	$6,6			# 0x6
	jal	memcpy
	addiu	$5,$5,320

	move	$5,$21
	addiu	$4,$16,18
	jal	memcpy
	li	$6,6			# 0x6

	addiu	$4,$16,30
	addiu	$5,$18,256
	jal	memcpy
	li	$6,32			# 0x20

	addiu	$4,$16,62
	addiu	$5,$18,288
	jal	memcpy
	li	$6,32			# 0x20

	addiu	$4,$16,94
	lbu	$6,352($18)
	jal	memcpy
	move	$5,$18

	move	$6,$16
	lbu	$7,352($18)
	addiu	$4,$18,568
	li	$5,16			# 0x10
	addu	$2,$16,$7
	addiu	$7,$7,94
	addiu	$2,$2,94
	sw	$2,16($sp)
	li	$2,20			# 0x14
	jal	sha256_hmac
	sw	$2,20($sp)

	move	$4,$17
	lbu	$2,352($18)
	lhu	$5,6($19)
	addu	$5,$5,$2
	addiu	$5,$5,114
	sh	$5,6($19)
	jal	skb_put
	andi	$5,$5,0xffff

	lw	$5,4($20)
	lw	$3,84($17)
	lw	$2,208($5)
	addiu	$3,$3,-14
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L448
	nop

	move	$5,$20
	jal	wapiFragementSend
	move	$4,$17

	li	$3,1			# 0x1
	beq	$2,$3,$L451
	nop

	j	$L456
	nop

$L448:
	jal	rtl8192cd_start_xmit
	move	$4,$17

	beq	$2,$0,$L451
	nop

$L456:
	jal	dev_kfree_skb_any
	move	$4,$17

$L451:
	lw	$3,364($18)
	li	$2,5			# 0x5
	bne	$3,$2,$L457
	addiu	$4,$18,600

	li	$2,7			# 0x7
	sw	$2,364($18)
$L443:
	addiu	$4,$18,600
$L457:
	jal	mod_timer
	move	$5,$22

	move	$2,$0
	lw	$31,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	WapiSendUnicastKeyAggrementResponse
	.section	.text.WapiSendMulticastKeyResponse,"ax",@progbits
	.align	2
	.ent	WapiSendMulticastKeyResponse
	.type	WapiSendMulticastKeyResponse, @function
WapiSendMulticastKeyResponse:
	.set	nomips16
	.frame	$sp,64,$31		# vars= 0, regs= 9/0, args= 24, gp= 0
	.mask	0x80ff0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	lui	$2,%hi(jiffies)
	sw	$23,52($sp)
	li	$23,1			# 0x1
	sw	$22,48($sp)
	move	$22,$5
	sw	$21,44($sp)
	move	$21,$4
	sw	$19,36($sp)
	sw	$16,24($sp)
	sw	$31,56($sp)
	sw	$20,40($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	lw	$2,%lo(jiffies)($2)
	lw	$20,6676($5)
	jal	alloc_skb_from_queue
	addiu	$16,$2,100

	bne	$2,$0,$L471
	move	$19,$2

	jal	dev_alloc_skb
	li	$4,1560			# 0x618

	beq	$2,$0,$L462
	move	$19,$2

$L471:
	li	$16,-30540
	addiu	$18,$22,6070
	lw	$4,176($19)
	move	$5,$18
	sh	$16,106($19)
	jal	memcpy
	li	$6,6			# 0x6

	li	$6,6			# 0x6
	lw	$4,176($19)
	lw	$5,4($21)
	addiu	$4,$4,6
	jal	memcpy
	addiu	$5,$5,320

	move	$4,$19
	lw	$2,176($19)
	li	$5,14			# 0xe
	jal	skb_put
	sh	$16,12($2)

	li	$4,12			# 0xc
	lw	$3,84($19)
	li	$6,6			# 0x6
	lw	$2,4($21)
	lw	$17,176($19)
	sw	$2,24($19)
	li	$2,1
	addu	$17,$17,$3
	sh	$4,6($17)
	sb	$4,3($17)
	sb	$2,2($17)
	sh	$23,0($17)
	sh	$0,4($17)
	sb	$0,10($17)
	sb	$0,11($17)
	lhu	$2,356($20)
	addiu	$2,$2,1
	sh	$2,356($20)
	lhu	$16,6($17)
	sh	$2,8($17)
	addu	$16,$17,$16
	sb	$0,0($16)
	addiu	$4,$16,9
	lw	$2,1232($21)
	srl	$2,$2,15
	andi	$2,$2,0x1
	sb	$2,1($16)
	lw	$2,352($20)
	srl	$2,$2,15
	andi	$2,$2,0x1
	sb	$2,2($16)
	lw	$5,4($21)
	jal	memcpy
	addiu	$5,$5,320

	addiu	$4,$16,3
	move	$5,$18
	jal	memcpy
	li	$6,6			# 0x6

	addiu	$4,$16,15
	addiu	$5,$21,960
	jal	memcpy
	li	$6,16			# 0x10

	move	$6,$16
	li	$2,20			# 0x14
	addiu	$4,$20,568
	li	$5,16			# 0x10
	li	$7,31			# 0x1f
	addiu	$16,$16,31
	sw	$2,20($sp)
	jal	sha256_hmac
	sw	$16,16($sp)

	move	$4,$19
	lhu	$5,6($17)
	addiu	$5,$5,51
	sh	$5,6($17)
	jal	skb_put
	andi	$5,$5,0xffff

	lw	$5,4($21)
	lw	$3,84($19)
	lw	$2,208($5)
	addiu	$3,$3,-14
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L464
	nop

	move	$5,$21
	jal	wapiFragementSend
	move	$4,$19

	beq	$2,$23,$L467
	nop

	j	$L472
	nop

$L464:
	jal	rtl8192cd_start_xmit
	move	$4,$19

	beq	$2,$0,$L467
	nop

$L472:
	jal	dev_kfree_skb_any
	move	$4,$19

$L467:
	lw	$3,364($20)
	li	$2,11			# 0xb
	bne	$3,$2,$L473
	addiu	$16,$20,600

	li	$2,13			# 0xd
	sw	$2,364($20)
$L473:
	jal	del_timer
	move	$4,$16

	move	$4,$16
	move	$5,$0
	jal	init_timer_key
	move	$6,$0

	li	$3,-253			# 0xffffff03
	lw	$2,352($20)
	sw	$22,616($20)
	and	$2,$2,$3
	sw	$2,352($20)
	lui	$2,%hi(wapiResendTimeout)
	addiu	$2,$2,%lo(wapiResendTimeout)
	j	$L458
	sw	$2,612($20)

$L462:
	addiu	$4,$20,600
	jal	mod_timer
	move	$5,$16

$L458:
	lw	$31,56($sp)
	move	$2,$0
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,64

	.set	macro
	.set	reorder
	.end	WapiSendMulticastKeyResponse
	.section	.text.wapiHandleRecvPacket,"ax",@progbits
	.align	2
	.globl	wapiHandleRecvPacket
	.ent	wapiHandleRecvPacket
	.type	wapiHandleRecvPacket, @function
wapiHandleRecvPacket:
	.set	nomips16
	.frame	$sp,200,$31		# vars= 136, regs= 9/0, args= 24, gp= 0
	.mask	0x80ff0000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-200
	sw	$22,184($sp)
	move	$22,$5
	sw	$31,192($sp)
	sw	$23,188($sp)
	sw	$21,180($sp)
	sw	$20,176($sp)
	sw	$19,172($sp)
	sw	$18,168($sp)
	sw	$17,164($sp)
	sw	$16,160($sp)
	lw	$19,6676($5)
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L474
	li	$5,-1			# 0xffffffff
	.set	macro
	.set	reorder

	lw	$2,360($19)
	lw	$23,652($19)
	beq	$2,$0,$L474
	lw	$21,0($4)
	li	$5,32			# 0x20
	li	$4,30			# 0x1e
	lw	$6,176($21)
	lw	$2,84($21)
	lhu	$3,0($6)
	srl	$3,$3,8
	andi	$3,$3,0x88
	xori	$3,$3,0x88
	movn	$5,$4,$3		#conditional move  
	addiu	$4,$5,2
	sltu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L484
	li	$7,-1			# 0xffffffff
	.set	macro
	.set	reorder

	addu	$2,$6,$5
	lhu	$3,0($2)
	li	$2,34996			# 0x88b4
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L648
	li	$2,-1			# 0xffffffff
	.set	macro
	.set	reorder

	sw	$4,152($sp)
	li	$7,1			# 0x1
$L484:
	li	$2,-1			# 0xffffffff
$L648:
	.set	noreorder
	.set	nomacro
	bne	$7,$2,$L477
	li	$2,13			# 0xd
	.set	macro
	.set	reorder

	lw	$3,364($19)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L474
	li	$5,-1			# 0xffffffff
	.set	macro
	.set	reorder

	j	$L488
$L477:
	lw	$2,176($21)
	lw	$20,152($sp)
	lhu	$3,358($19)
	addu	$18,$2,$20
	lhu	$2,8($18)
	sltu	$3,$3,$2
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L644
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	lhu	$3,0($18)
	bne	$3,$2,$L488
	lbu	$2,2($18)
	bne	$2,$3,$L488
	lhu	$3,6($18)
	lw	$2,84($21)
	sltu	$2,$2,$3
	bne	$2,$0,$L488
	lbu	$2,11($18)
	andi	$2,$2,0x1
	bne	$2,$0,$L493
	lbu	$2,10($18)
	beq	$2,$0,$L492
$L493:
	lbu	$4,353($19)
	lbu	$3,10($18)
	andi	$2,$4,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L495
	addiu	$2,$4,1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	wapiReleaseFragementQueue
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L474
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

$L495:
	sb	$2,353($19)
	lbu	$2,11($18)
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	beq	$2,$0,$L498
	lw	$4,652($19)
	lw	$16,584($19)
	jal	alloc_skb_from_queue
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L501
	move	$17,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	dev_alloc_skb
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L501
	move	$17,$2
	.set	macro
	.set	reorder

$L644:
	.set	noreorder
	.set	nomacro
	jal	wapiReleaseFragementQueue
	move	$4,$19
	.set	macro
	.set	reorder

	j	$L488
$L501:
	lw	$16,648($19)
$L503:
	lw	$2,176($17)
	lw	$4,84($17)
	lw	$5,176($16)
	lw	$6,84($16)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$4,$2,$4
	.set	macro
	.set	reorder

	lw	$5,84($16)
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$16,0($16)
	lw	$2,648($19)
	bne	$16,$2,$L503
	j	$L635
$L498:
	lw	$16,648($19)
	beq	$16,$0,$L508
	lw	$2,176($21)
	lw	$3,84($21)
	lw	$4,0($16)
	addu	$2,$2,$20
	subu	$3,$3,$20
	addiu	$2,$2,12
	addiu	$3,$3,-12
	sw	$4,0($21)
	sw	$21,4($4)
	sw	$2,176($21)
	sw	$3,84($21)
	sw	$16,4($21)
	sw	$21,0($16)
	lw	$2,584($19)
	.set	noreorder
	.set	nomacro
	j	$L639
	addu	$2,$2,$3
	.set	macro
	.set	reorder

$L508:
	sw	$21,648($19)
	lw	$2,84($21)
	sw	$21,4($21)
	sw	$21,0($21)
$L639:
	.set	noreorder
	.set	nomacro
	j	$L642
	sw	$2,584($19)
	.set	macro
	.set	reorder

$L635:
	lw	$5,176($21)
	lw	$6,84($21)
	lw	$2,176($17)
	addu	$5,$5,$20
	lw	$4,84($17)
	subu	$6,$6,$20
	addiu	$6,$6,-12
	addiu	$5,$5,12
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$4,$2,$4
	.set	macro
	.set	reorder

	move	$4,$17
	lw	$5,84($21)
	subu	$5,$5,$20
	.set	noreorder
	.set	nomacro
	jal	skb_put
	addiu	$5,$5,-12
	.set	macro
	.set	reorder

	move	$4,$19
	lw	$2,24($21)
	.set	noreorder
	.set	nomacro
	jal	wapiReleaseFragementQueue
	sw	$2,24($17)
	.set	macro
	.set	reorder

	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	dev_kfree_skb_any
	move	$21,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L474
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	lw	$3,176($17)
	lw	$2,152($sp)
	addu	$18,$3,$2
$L492:
	lhu	$2,8($18)
	sh	$2,358($19)
	lw	$2,8($23)
	lw	$3,892($2)
	andi	$2,$3,0x10
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L511
	andi	$2,$3,0x8
	.set	macro
	.set	reorder

	lw	$4,364($19)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L522
	sltu	$2,$4,7
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L574
	li	$2,10			# 0xa
	.set	macro
	.set	reorder

	li	$2,2			# 0x2
	beq	$4,$2,$L513
	j	$L488
$L574:
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L527
	li	$2,13			# 0xd
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L530
	li	$2,12			# 0xc
	.set	macro
	.set	reorder

	j	$L488
$L513:
	lw	$19,6676($22)
	lw	$2,364($19)
	.set	noreorder
	.set	nomacro
	bne	$2,$4,$L649
	lui	$2,%hi(jiffies)
	.set	macro
	.set	reorder

	lbu	$3,3($18)
	li	$2,4			# 0x4
	bne	$3,$2,$L488
	lbu	$2,12($18)
	andi	$23,$2,0x7
	.set	noreorder
	.set	nomacro
	bne	$23,$3,$L488
	addiu	$20,$18,12
	.set	macro
	.set	reorder

	lbu	$2,65($20)
	lw	$5,656($19)
	addu	$2,$20,$2
	addiu	$4,$2,66
	lhu	$2,2($4)
	lhu	$3,2($5)
	addu	$2,$4,$2
	addu	$3,$5,$3
	addiu	$4,$2,4
	addiu	$5,$3,4
	lhu	$2,2($4)
	lhu	$3,2($5)
	addu	$2,$4,$2
	addu	$3,$5,$3
	addiu	$7,$2,4
	addiu	$8,$3,4
	lbu	$4,1($8)
	lbu	$3,1($7)
	lbu	$5,2($7)
	sll	$4,$4,8
	lbu	$2,2($8)
	sll	$3,$3,8
	or	$6,$5,$3
	or	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$6,$2,$L488
	move	$4,$7
	.set	macro
	.set	reorder

	move	$5,$8
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$6,$6,3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L488
	li	$2,-253			# 0xffffff03
	.set	macro
	.set	reorder

	lw	$3,656($19)
	lw	$7,352($19)
	move	$5,$0
	addiu	$16,$3,8
	li	$6,16			# 0x10
	and	$7,$7,$2
	li	$2,3			# 0x3
	sw	$7,352($19)
	move	$4,$16
	sw	$2,364($19)
	addiu	$17,$3,24
	sh	$23,2($3)
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$22,4($3)
	.set	macro
	.set	reorder

	lw	$2,652($19)
	lw	$4,4($2)
	jal	strlen
	move	$4,$16
	lw	$3,652($19)
	lw	$5,4($3)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$6,$2
	.set	macro
	.set	reorder

	move	$5,$20
	lhu	$2,6($18)
	move	$4,$17
	addiu	$2,$2,-12
	move	$6,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,592($19)
	.set	macro
	.set	reorder

	addiu	$4,$sp,24
	.set	noreorder
	.set	nomacro
	jal	GenerateRandomData
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	addiu	$5,$sp,24
	lw	$4,592($19)
	li	$6,32			# 0x20
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$4,$17,$4
	.set	macro
	.set	reorder

	lw	$2,592($19)
	addiu	$2,$2,56
	sw	$2,592($19)
	lui	$2,%hi(jiffies)
$L649:
	addiu	$4,$19,600
	lw	$5,%lo(jiffies)($2)
	.set	noreorder
	.set	nomacro
	jal	mod_timer
	addiu	$5,$5,1000
	.set	macro
	.set	reorder

	lw	$4,652($19)
	lw	$6,656($19)
	lw	$5,36($4)
	lw	$7,592($19)
	jal	DOT11_EnQueue
	lw	$4,652($19)
	jal	wapi_event_indicate
	j	$L488
$L522:
	move	$4,$18
	move	$5,$22
	.set	noreorder
	.set	nomacro
	jal	wapiRecvUnicastKeyAgreementResponse
	addiu	$6,$sp,156
	.set	macro
	.set	reorder

	move	$3,$2
	li	$2,-2			# 0xfffffffe
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L526
	li	$2,-1			# 0xffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L488
	move	$4,$23
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	wapiSendUnicastKeyAgrementConfirm
	move	$5,$22
	.set	macro
	.set	reorder

	move	$4,$23
	.set	noreorder
	.set	nomacro
	jal	wapiSendMulticastKeyNotification
	move	$5,$22
	.set	macro
	.set	reorder

	j	$L488
$L527:
	move	$4,$18
	move	$5,$22
	.set	noreorder
	.set	nomacro
	jal	wapiRecvMulticastKeyResponse
	addiu	$6,$sp,156
	.set	macro
	.set	reorder

	j	$L488
$L530:
	lbu	$3,3($18)
	lw	$17,6676($22)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L533
	addiu	$6,$sp,156
	.set	macro
	.set	reorder

	lw	$2,652($17)
	lw	$2,1232($2)
	andi	$2,$2,0x2000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L488
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	wapiRecvMulticastKeyResponse
	move	$5,$22
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L488
	li	$20,1			# 0x1
	.set	macro
	.set	reorder

	lw	$2,352($17)
	move	$19,$0
	ori	$2,$2,0x400
	.set	noreorder
	.set	nomacro
	j	$L536
	sw	$2,352($17)
	.set	macro
	.set	reorder

$L545:
	lw	$3,652($17)
	addu	$3,$3,$2
	li	$2,65536			# 0x10000
	ori	$2,$2,0x2a74
	addu	$18,$3,$2
	lw	$16,0($18)
	.set	noreorder
	.set	nomacro
	j	$L646
	move	$2,$16
	.set	macro
	.set	reorder

$L543:
	lw	$3,6676($2)
	lw	$16,0($16)
	lw	$2,364($3)
	sltu	$2,$2,13
	beq	$2,$0,$L541
	lw	$4,652($17)
	jal	wapiDeauthSta
	.set	noreorder
	.set	nomacro
	j	$L646
	move	$2,$16
	.set	macro
	.set	reorder

$L541:
	lw	$2,352($3)
	andi	$2,$2,0x400
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L645
	move	$2,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L537
	move	$20,$0
	.set	macro
	.set	reorder

$L645:
$L646:
	li	$6,25			# 0x19
	.set	noreorder
	.set	nomacro
	bne	$16,$18,$L543
	move	$5,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L537
	addiu	$19,$19,1
	.set	macro
	.set	reorder

$L536:
	sltu	$2,$19,31
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L545
	sll	$2,$19,3
	.set	macro
	.set	reorder

$L537:
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$20,$2,$L488
	li	$2,-65536			# 0xffff0000
	.set	macro
	.set	reorder

	lw	$5,652($17)
	ori	$2,$2,0x7fff
	lw	$4,1232($5)
	srl	$3,$4,15
	and	$4,$4,$2
	xori	$3,$3,0x1
	andi	$3,$3,0x1
	sll	$3,$3,15
	or	$4,$4,$3
	sw	$4,1232($5)
	lw	$3,652($17)
	lw	$2,1232($3)
	ori	$2,$2,0x4000
	sw	$2,1232($3)
	lw	$4,652($17)
	jal	wapiInit
	lui	$2,%hi(jiffies)
	lw	$4,652($17)
	lw	$5,%lo(jiffies)($2)
	addiu	$4,$4,1308
	.set	noreorder
	.set	nomacro
	jal	mod_timer
	addiu	$5,$5,100
	.set	macro
	.set	reorder

	j	$L488
$L533:
	li	$2,9			# 0x9
	bne	$3,$2,$L488
	lw	$4,352($17)
	andi	$2,$4,0x1000
	beq	$2,$0,$L549
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	wapiRecvUnicastKeyAgreementResponse
	move	$5,$22
	.set	macro
	.set	reorder

	bne	$2,$0,$L488
	lw	$4,652($17)
	.set	noreorder
	.set	nomacro
	jal	wapiSendUnicastKeyAgrementConfirm
	move	$5,$22
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L488
	addiu	$16,$17,600
	.set	macro
	.set	reorder

	li	$18,16			# 0x10
	.set	noreorder
	.set	nomacro
	jal	del_timer
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	init_timer_key
	move	$6,$0
	.set	macro
	.set	reorder

	li	$2,-65536			# 0xffff0000
	lw	$4,352($17)
	ori	$2,$2,0x7fff
	sw	$22,616($17)
	srl	$3,$4,15
	and	$4,$4,$2
	xori	$3,$3,0x1
	lui	$2,%hi(wapiResendTimeout)
	andi	$3,$3,0x1
	addiu	$2,$2,%lo(wapiResendTimeout)
	sll	$3,$3,15
	or	$4,$4,$3
	sw	$2,612($17)
	.set	noreorder
	.set	nomacro
	j	$L553
	sw	$4,352($17)
	.set	macro
	.set	reorder

$L556:
	li	$2,54
	addiu	$18,$18,-2
	sb	$2,366($3)
	li	$2,92
	sb	$2,367($3)
$L553:
	.set	noreorder
	.set	nomacro
	bgtz	$18,$L556
	addu	$3,$18,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L557
	li	$16,1			# 0x1
	.set	macro
	.set	reorder

$L560:
	jal	memcpy
$L557:
	sll	$4,$16,4
	slt	$2,$16,4
	addu	$4,$17,$4
	addiu	$3,$17,368
	li	$6,16			# 0x10
	addiu	$4,$4,368
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L560
	move	$5,$3
	.set	macro
	.set	reorder

	addiu	$4,$17,432
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$16,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L647
	sll	$2,$16,4
	.set	macro
	.set	reorder

$L564:
	li	$2,55
	sb	$2,368($4)
	sll	$2,$16,4
$L647:
	slt	$3,$16,4
	addu	$2,$2,$17
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L564
	addu	$4,$2,$18
	.set	macro
	.set	reorder

	addu	$3,$18,$17
	li	$2,55
	sb	$2,432($3)
	addiu	$4,$17,448
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,8			# 0x8
	.set	macro
	.set	reorder

	li	$3,-4097			# 0xffffefff
	lw	$2,352($17)
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	j	$L488
	sw	$2,352($17)
	.set	macro
	.set	reorder

$L549:
	lbu	$2,12($18)
	andi	$2,$2,0x10
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L488
	addiu	$3,$18,12
	.set	macro
	.set	reorder

	srl	$2,$4,15
	lbu	$3,17($3)
	andi	$2,$2,0x1
	beq	$3,$2,$L488
	lw	$4,652($17)
	.set	noreorder
	.set	nomacro
	jal	wapiUpdateUSK
	move	$5,$22
	.set	macro
	.set	reorder

	j	$L488
$L511:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L526
	li	$2,7			# 0x7
	.set	macro
	.set	reorder

	lw	$4,364($19)
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L605
	sltu	$2,$4,8
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L632
	li	$2,9			# 0x9
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L578
	li	$2,8			# 0x8
	.set	macro
	.set	reorder

	j	$L627
$L632:
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L650
	move	$4,$18
	.set	macro
	.set	reorder

	j	$L627
$L578:
	lbu	$3,3($18)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L582
	li	$4,-1			# 0xffffffff
	.set	macro
	.set	reorder

	lw	$17,6676($22)
	addiu	$4,$18,13
	li	$6,16			# 0x10
	addiu	$16,$18,12
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$17,472
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L582
	li	$4,-1			# 0xffffffff
	.set	macro
	.set	reorder

	lbu	$2,12($18)
	andi	$2,$2,0x10
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L584
	addiu	$5,$18,42
	.set	macro
	.set	reorder

	lw	$2,352($17)
	lbu	$3,17($16)
	srl	$2,$2,15
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L582
	addiu	$5,$17,288
	.set	macro
	.set	reorder

	addiu	$4,$18,42
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L582
	li	$4,-1			# 0xffffffff
	.set	macro
	.set	reorder

	j	$L587
$L584:
	addiu	$4,$17,288
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,32			# 0x20
	.set	macro
	.set	reorder

$L587:
	lw	$2,352($17)
	srl	$3,$2,15
	andi	$2,$2,0x1000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L589
	andi	$18,$3,0x1
	.set	macro
	.set	reorder

	xori	$2,$3,0x1
	andi	$18,$2,0x1
$L589:
	addiu	$4,$17,256
	addiu	$16,$sp,56
	.set	noreorder
	.set	nomacro
	jal	GenerateRandomData
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	move	$4,$22
	.set	noreorder
	.set	nomacro
	jal	wapiDerivedUCastKey
	move	$5,$16
	.set	macro
	.set	reorder

	move	$4,$22
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	wapiInstallUCastKey
	move	$6,$16
	.set	macro
	.set	reorder

	lw	$2,352($17)
	ori	$2,$2,0x4000
	sw	$2,352($17)
#APP
	raw_local_irq_save	$16
#NO_APP
	lw	$2,6676($22)
	lw	$3,652($17)
	addiu	$2,$2,2
	sw	$2,908($3)
	lw	$4,652($17)
	jal	wapiSetIE
	li	$2,68
	lw	$4,652($17)
	sb	$2,0($17)
	lbu	$2,1232($4)
	sb	$2,1($17)
	lbu	$2,1232($4)
	addiu	$2,$2,2
	sb	$2,352($17)
#APP
	raw_local_irq_restore	$16
#NO_APP
	move	$4,$0
$L582:
	li	$2,-1			# 0xffffffff
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L488
	move	$4,$23
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	WapiSendUnicastKeyAggrementResponse
	move	$5,$22
	.set	macro
	.set	reorder

	j	$L488
$L605:
	lbu	$3,3($18)
	li	$2,10			# 0xa
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L610
	li	$5,-1			# 0xffffffff
	.set	macro
	.set	reorder

	lbu	$2,12($18)
	lw	$17,6676($22)
	andi	$2,$2,0x10
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L611
	addiu	$19,$18,12
	.set	macro
	.set	reorder

	lw	$2,352($17)
	lbu	$3,17($19)
	srl	$2,$2,15
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L651
	li	$2,-2			# 0xfffffffe
	.set	macro
	.set	reorder

$L611:
	addiu	$4,$19,1
	addiu	$5,$17,472
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L610
	li	$5,-1			# 0xffffffff
	.set	macro
	.set	reorder

	addiu	$4,$19,30
	addiu	$5,$17,256
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L610
	li	$5,-1			# 0xffffffff
	.set	macro
	.set	reorder

	addiu	$4,$19,18
	addiu	$5,$22,6070
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L614
	addiu	$4,$19,24
	.set	macro
	.set	reorder

	lw	$2,652($17)
	li	$6,6			# 0x6
	lw	$5,4($2)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$5,320
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L613
	li	$2,20			# 0x14
	.set	macro
	.set	reorder

$L614:
	.set	noreorder
	.set	nomacro
	j	$L610
	li	$5,-1			# 0xffffffff
	.set	macro
	.set	reorder

$L613:
	lhu	$7,6($18)
	addiu	$16,$sp,24
	addiu	$4,$17,568
	li	$5,16			# 0x10
	addiu	$7,$7,-32
	move	$6,$19
	sw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	jal	sha256_hmac
	sw	$16,16($sp)
	.set	macro
	.set	reorder

	move	$4,$16
	lhu	$5,6($18)
	li	$6,20			# 0x14
	addu	$5,$18,$5
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$5,-20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L610
	li	$5,-1			# 0xffffffff
	.set	macro
	.set	reorder

	lw	$5,652($17)
	addiu	$4,$19,62
	lbu	$6,1233($5)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$5,976
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L616
	li	$2,27			# 0x1b
	.set	macro
	.set	reorder

	li	$5,-2			# 0xfffffffe
	.set	noreorder
	.set	nomacro
	j	$L610
	sw	$2,156($sp)
	.set	macro
	.set	reorder

$L616:
	lw	$4,656($17)
	beq	$4,$0,$L617
	jal	kfree
	sw	$0,656($17)
	sw	$0,592($17)
$L617:
	lw	$3,364($17)
	li	$2,7			# 0x7
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L618
	li	$3,9			# 0x9
	.set	macro
	.set	reorder

	lw	$2,352($17)
	sw	$3,364($17)
	ori	$2,$2,0x6000
	sw	$2,352($17)
$L618:
	lw	$2,652($17)
	lw	$4,8($2)
	li	$2,2			# 0x2
	lw	$3,5240($4)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L620
	li	$2,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L619
	addiu	$16,$17,624
	.set	macro
	.set	reorder

$L620:
	lw	$3,5244($4)
	lui	$5,%hi(jiffies)
	sll	$2,$3,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$2,$2,2
	.set	noreorder
	.set	nomacro
	bgez	$2,$L621
	addiu	$4,$17,624
	.set	macro
	.set	reorder

	lw	$2,%lo(jiffies)($5)
	li	$5,2147418112			# 0x7fff0000
	ori	$5,$5,0xffff
	.set	noreorder
	.set	nomacro
	j	$L640
	addu	$5,$2,$5
	.set	macro
	.set	reorder

$L621:
	lw	$5,%lo(jiffies)($5)
	addu	$5,$5,$2
$L640:
	jal	mod_timer
	j	$L623
$L619:
	.set	noreorder
	.set	nomacro
	jal	del_timer
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	init_timer_key
	move	$6,$0
	.set	macro
	.set	reorder

	lui	$2,%hi(wapiUCastUpdateKeyTimeout)
	addiu	$2,$2,%lo(wapiUCastUpdateKeyTimeout)
	sw	$2,636($17)
	sw	$22,640($17)
$L623:
	lw	$3,352($17)
	li	$2,-253			# 0xffffff03
	lw	$4,652($17)
	move	$5,$0
	and	$3,$3,$2
	sw	$3,352($17)
	lw	$2,8($4)
	lw	$2,5248($2)
	sw	$2,596($17)
$L610:
	li	$2,-2			# 0xfffffffe
$L651:
	beq	$5,$2,$L526
	j	$L488
$L627:
	lbu	$3,3($18)
	li	$2,11			# 0xb
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L488
	move	$4,$18
	.set	macro
	.set	reorder

$L650:
	move	$5,$22
	.set	noreorder
	.set	nomacro
	jal	WapiRecvMulticastKeyNotification
	addiu	$6,$sp,156
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L488
	move	$4,$23
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	WapiSendMulticastKeyResponse
	move	$5,$22
	.set	macro
	.set	reorder

	j	$L488
$L526:
	lw	$6,156($sp)
	move	$4,$23
	.set	noreorder
	.set	nomacro
	jal	wapiDeauthSta
	move	$5,$22
	.set	macro
	.set	reorder

$L488:
	.set	noreorder
	.set	nomacro
	jal	dev_kfree_skb_any
	move	$4,$21
	.set	macro
	.set	reorder

$L642:
	li	$5,1			# 0x1
$L474:
	lw	$31,192($sp)
	move	$2,$5
	lw	$23,188($sp)
	lw	$22,184($sp)
	lw	$21,180($sp)
	lw	$20,176($sp)
	lw	$19,172($sp)
	lw	$18,168($sp)
	lw	$17,164($sp)
	lw	$16,160($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,200
	.set	macro
	.set	reorder

	.end	wapiHandleRecvPacket
	.section	.text.wapiResendTimeout,"ax",@progbits
	.align	2
	.ent	wapiResendTimeout
	.type	wapiResendTimeout, @function
wapiResendTimeout:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	li	$3,-253			# 0xffffff03
	sw	$18,24($sp)
	move	$18,$4
	sw	$31,28($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lw	$17,6676($4)
	lw	$4,352($17)
	lw	$16,652($17)
	srl	$2,$4,2
	and	$4,$4,$3
	andi	$2,$2,0x3f
	addiu	$2,$2,1
	andi	$2,$2,0x3f
	sll	$2,$2,2
	or	$4,$4,$2
	srl	$2,$4,2
	andi	$2,$2,0x3f
	sltu	$2,$2,7
	bne	$2,$0,$L653
	sw	$4,352($17)

	move	$4,$16
	move	$5,$18
	lw	$31,28($sp)
	li	$6,25			# 0x19
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	wapiDeauthSta
	addiu	$sp,$sp,32

$L653:
	lw	$3,364($17)
	sltu	$2,$3,14
	beq	$2,$0,$L652
	sll	$2,$3,2

	lui	$3,%hi($L678)
	addiu	$3,$3,%lo($L678)
	addu	$2,$2,$3
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
$L678:
	.word	$L655
	.word	$L652
	.word	$L657
	.word	$L659
	.word	$L652
	.word	$L652
	.word	$L661
	.word	$L673
	.word	$L652
	.word	$L652
	.word	$L664
	.word	$L652
	.word	$L652
	.word	$L665
	.section	.text.wapiResendTimeout
$L655:
	lw	$3,360($17)
	li	$2,1			# 0x1
	bne	$3,$2,$L652
	move	$4,$18

	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	wapiReqActiveCA
	addiu	$sp,$sp,32

$L657:
	lw	$3,360($17)
	li	$2,1			# 0x1
	bne	$3,$2,$L652
	move	$4,$16

	lw	$7,656($17)
	lw	$6,592($17)
	move	$5,$18
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	WapiSendActivateAuthenticationPacket
	addiu	$sp,$sp,32

$L659:
	lw	$3,360($17)
	li	$2,1			# 0x1
	bne	$3,$2,$L652
	lui	$2,%hi(jiffies)

	addiu	$4,$17,600
	lw	$5,%lo(jiffies)($2)
	jal	mod_timer
	addiu	$5,$5,1000

	lw	$4,652($17)
	lw	$6,656($17)
	lw	$5,36($4)
	lw	$7,592($17)
	jal	DOT11_EnQueue
	nop

	lw	$4,652($17)
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	wapi_event_indicate
	addiu	$sp,$sp,32

$L661:
	lw	$3,360($17)
	li	$2,1			# 0x1
	bne	$3,$2,$L680
	move	$4,$16

	lw	$7,656($17)
	lw	$6,592($17)
	jal	WapiSendAuthenticationRspPacket
	move	$5,$18

	j	$L681
	move	$4,$16

$L664:
	move	$4,$16
	jal	wapiSendUnicastKeyAgrementConfirm
	move	$5,$18

	j	$L682
	move	$4,$16

$L665:
	lw	$2,1232($16)
	andi	$2,$2,0x2000
	beq	$2,$0,$L666
	move	$4,$16

$L682:
	move	$5,$18
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	wapiSendMulticastKeyNotification
	addiu	$sp,$sp,32

$L666:
	lw	$2,352($17)
	andi	$2,$2,0x1000
	beq	$2,$0,$L668
	nop

$L680:
	move	$4,$16
$L681:
	move	$5,$18
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	wapiSendUnicastKeyAgrementRequeset
	addiu	$sp,$sp,32

$L668:
	lw	$2,8($16)
	lw	$3,892($2)
	andi	$2,$3,0x8
	beq	$2,$0,$L670
	andi	$2,$3,0x10

	move	$4,$16
	move	$5,$18
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	WapiSendMulticastKeyResponse
	addiu	$sp,$sp,32

$L670:
	beq	$2,$0,$L652
	move	$4,$16

	move	$5,$18
	jal	wapiSendUnicastKeyAgrementConfirm
	addiu	$16,$17,600

	jal	del_timer
	move	$4,$16

	move	$4,$16
	move	$5,$0
	jal	init_timer_key
	move	$6,$0

	lui	$2,%hi(wapiResendTimeout)
	addiu	$2,$2,%lo(wapiResendTimeout)
	sw	$2,612($17)
	j	$L652
	sw	$18,616($17)

$L673:
	move	$4,$16
	move	$5,$18
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	WapiSendUnicastKeyAggrementResponse
	addiu	$sp,$sp,32

$L652:
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	wapiResendTimeout
	.ident	"GCC: (GNU) 3.4.6-1.3.6"
