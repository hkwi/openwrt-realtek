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
	.rdata
	.align	2
	.type	rtl_ap_EDCA, @object
	.size	rtl_ap_EDCA, 120
rtl_ap_EDCA:
	.word	0
	.word	7
	.word	4
	.word	10
	.word	0
	.word	0
	.word	3
	.word	4
	.word	6
	.word	0
	.word	0
	.word	1
	.word	3
	.word	4
	.word	188
	.word	0
	.word	1
	.word	2
	.word	3
	.word	102
	.word	0
	.word	1
	.word	3
	.word	4
	.word	94
	.word	0
	.word	1
	.word	2
	.word	3
	.word	47
	.align	2
	.type	rtl_sta_EDCA, @object
	.size	rtl_sta_EDCA, 120
rtl_sta_EDCA:
	.word	0
	.word	7
	.word	4
	.word	10
	.word	0
	.word	0
	.word	3
	.word	4
	.word	10
	.word	0
	.word	0
	.word	2
	.word	3
	.word	4
	.word	188
	.word	0
	.word	2
	.word	2
	.word	3
	.word	102
	.word	0
	.word	2
	.word	3
	.word	4
	.word	94
	.word	0
	.word	2
	.word	2
	.word	3
	.word	47
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
	.type	__func__.3, @object
	.size	__func__.3, 14
__func__.3:
	.ascii	"wapiDeauthSta\000"
	.section	.text.wapiDeauthSta,"ax",@progbits
	.align	2
	.ent	wapiDeauthSta
	.type	wapiDeauthSta, @function
wapiDeauthSta:
	.set	nomips16
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-48
	sw	$31,40($sp)
	sw	$4,48($sp)
	sw	$5,52($sp)
	sw	$6,56($sp)
	lw	$2,52($sp)
	addiu	$2,$2,6070
	lw	$4,48($sp)
	move	$5,$2
	lw	$6,56($sp)
	jal	issue_deauth
#APP
	raw_local_irq_save	$2
#NO_APP
	sw	$2,16($sp)
	lw	$2,52($sp)
	addiu	$2,$2,8
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$3,0($2)
	lw	$2,24($sp)
	xor	$2,$3,$2
	sltu	$2,$2,1
	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L7
	lw	$2,52($sp)
	addiu	$2,$2,8
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,4($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$3,32($sp)
	lw	$2,28($sp)
	sw	$2,4($3)
	lw	$3,28($sp)
	lw	$2,32($sp)
	sw	$2,0($3)
	lw	$2,24($sp)
	sw	$2,32($sp)
	lw	$3,32($sp)
	lw	$2,32($sp)
	sw	$2,0($3)
	lw	$3,32($sp)
	lw	$2,32($sp)
	sw	$2,4($3)
	lw	$2,52($sp)
	lw	$2,6248($2)
	beq	$2,$0,$L7
	lw	$4,48($sp)
	lw	$5,52($sp)
	move	$6,$0
	lui	$2,%hi(__func__.3)
	addiu	$7,$2,%lo(__func__.3)
	jal	cnt_assoc_num
	lw	$4,48($sp)
	lw	$5,52($sp)
	move	$6,$0
	jal	check_sta_characteristic
$L7:
	lw	$2,16($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sw	$2,28($sp)
	lw	$2,28($sp)
	beq	$2,$0,$L16
	lw	$2,16($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
#APP
	raw_local_irq_restore	$2
#NO_APP
	sw	$2,24($sp)
	j	$L13
$L16:
	lw	$2,16($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
#APP
	raw_local_irq_restore	$2
#NO_APP
	sw	$2,24($sp)
$L13:
	lw	$4,48($sp)
	lw	$5,52($sp)
	jal	free_stainfo
	lw	$31,40($sp)
	addiu	$sp,$sp,48
	j	$31
	.end	wapiDeauthSta
	.section	.text.wapiFreeAllSta,"ax",@progbits
	.align	2
	.ent	wapiFreeAllSta
	.type	wapiFreeAllSta, @function
wapiFreeAllSta:
	.set	nomips16
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$31,40($sp)
	sw	$4,48($sp)
	sw	$5,52($sp)
	sw	$0,32($sp)
	sw	$0,28($sp)
$L24:
	lw	$2,28($sp)
	sltu	$2,$2,31
	beq	$2,$0,$L25
	nop

	lw	$2,28($sp)
	sll	$3,$2,3
	lw	$2,48($sp)
	addu	$3,$3,$2
	li	$2,65536			# 0x10000
	ori	$2,$2,0x2a74
	addu	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
$L27:
	lw	$3,16($sp)
	lw	$2,20($sp)
	beq	$3,$2,$L26
	nop

	lw	$2,16($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,24($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,24($sp)
	lw	$2,6676($2)
	beq	$2,$0,$L27
	nop

	lw	$2,24($sp)
	lw	$2,6676($2)
	lw	$2,364($2)
	bne	$2,$0,$L29
	nop

	j	$L27
	nop

$L29:
	lw	$3,52($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L32
	nop

	lw	$2,24($sp)
	lw	$2,6676($2)
	lw	$2,352($2)
	andi	$2,$2,0x400
	bne	$2,$0,$L31
	nop

$L32:
	lw	$4,48($sp)
	lw	$5,24($sp)
	li	$6,25			# 0x19
	jal	wapiDeauthSta
	nop

	j	$L27
	nop

$L31:
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
	j	$L27
	nop

$L26:
	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
	j	$L24
	nop

$L25:
	lw	$2,32($sp)
	lw	$31,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiFreeAllSta
	.section	.text.wapiMCastUpdateKeyTimeout,"ax",@progbits
	.align	2
	.ent	wapiMCastUpdateKeyTimeout
	.type	wapiMCastUpdateKeyTimeout, @function
wapiMCastUpdateKeyTimeout:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,32($sp)
	sw	$4,40($sp)
	lw	$2,40($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,1232($2)
	andi	$2,$2,0x2000
	beq	$2,$0,$L35
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1308
	move	$4,$2
	jal	del_timer
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1308
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	nop

	lw	$3,16($sp)
	lw	$2,16($sp)
	sw	$2,1324($3)
	lw	$3,16($sp)
	lui	$2,%hi(wapiMCastUpdateKeyTimeout)
	addiu	$2,$2,%lo(wapiMCastUpdateKeyTimeout)
	sw	$2,1320($3)
	lw	$4,16($sp)
	lw	$3,1232($4)
	li	$2,-8193			# 0xffffdfff
	and	$2,$3,$2
	sw	$2,1232($4)
	lw	$3,16($sp)
	li	$2,65536			# 0x10000
	addu	$2,$2,$3
	lw	$2,15804($2)
	sw	$2,24($sp)
	lw	$2,16($sp)
	lw	$2,1232($2)
	andi	$2,$2,0x4000
	bne	$2,$0,$L36
	nop

	lw	$4,16($sp)
	move	$5,$0
	jal	wapiFreeAllSta
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L36
	nop

	lw	$4,16($sp)
	lw	$3,1232($4)
	li	$2,-16385			# 0xffffbfff
	and	$2,$3,$2
	sw	$2,1232($4)
	lw	$3,16($sp)
	lw	$4,1232($3)
	li	$2,-65536			# 0xffff0000
	ori	$2,$2,0x7fff
	and	$2,$4,$2
	sw	$2,1232($3)
	lw	$4,16($sp)
	lw	$3,1232($4)
	li	$2,-8193			# 0xffffdfff
	and	$2,$3,$2
	sw	$2,1232($4)
	lw	$3,16($sp)
	lw	$2,1232($3)
	ori	$2,$2,0x1000
	sw	$2,1232($3)
$L36:
	lw	$2,24($sp)
	beq	$2,$0,$L34
	nop

	lw	$2,16($sp)
	lw	$2,8($2)
	lw	$3,5252($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L39
	nop

	lw	$2,16($sp)
	lw	$2,8($2)
	lw	$3,5252($2)
	li	$2,4			# 0x4
	beq	$3,$2,$L39
	nop

	j	$L34
	nop

$L39:
	lw	$2,16($sp)
	lw	$2,8($2)
	lw	$3,5256($2)
	move	$2,$3
	sll	$2,$2,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$2,$2,2
	bgez	$2,$L40
	nop

	lw	$2,16($sp)
	addiu	$4,$2,1308
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	li	$2,2147418112			# 0x7fff0000
	ori	$2,$2,0xffff
	addu	$2,$3,$2
	move	$5,$2
	jal	mod_timer
	nop

	j	$L34
	nop

$L40:
	lw	$2,16($sp)
	addiu	$4,$2,1308
	lw	$2,16($sp)
	lw	$2,8($2)
	lw	$3,5256($2)
	move	$2,$3
	sll	$2,$2,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$3,$2,2
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$2,$3,$2
	move	$5,$2
	jal	mod_timer
	nop

	j	$L34
	nop

$L35:
	lw	$3,16($sp)
	li	$2,65536			# 0x10000
	addu	$2,$2,$3
	lw	$2,15776($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	beq	$2,$0,$L43
	nop

	lw	$2,20($sp)
	lw	$2,6676($2)
	beq	$2,$0,$L43
	nop

	lw	$2,20($sp)
	lw	$2,6676($2)
	lw	$2,352($2)
	andi	$2,$2,0x1000
	beq	$2,$0,$L43
	nop

	lw	$2,16($sp)
	addiu	$3,$2,1308
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,100
	move	$4,$3
	move	$5,$2
	jal	mod_timer
	nop

	j	$L34
	nop

$L43:
	lw	$4,16($sp)
	move	$5,$0
	jal	wapiUpdateMSK
	nop

$L34:
	lw	$31,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiMCastUpdateKeyTimeout
	.section	.text.wapiUCastUpdateKeyTimeout,"ax",@progbits
	.align	2
	.ent	wapiUCastUpdateKeyTimeout
	.type	wapiUCastUpdateKeyTimeout, @function
wapiUCastUpdateKeyTimeout:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,32($sp)
	sw	$4,40($sp)
	lw	$2,40($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,6676($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,652($2)
	sw	$2,20($sp)
	lw	$2,24($sp)
	lw	$2,352($2)
	andi	$2,$2,0x1000
	beq	$2,$0,$L46
	nop

	lw	$4,20($sp)
	lw	$5,16($sp)
	li	$6,25			# 0x19
	jal	wapiDeauthSta
	nop

	j	$L45
	nop

$L46:
	lw	$2,20($sp)
	lw	$2,1232($2)
	andi	$2,$2,0x2000
	beq	$2,$0,$L48
	nop

	lw	$2,24($sp)
	addiu	$3,$2,624
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,100
	move	$4,$3
	move	$5,$2
	jal	mod_timer
	nop

	j	$L45
	nop

$L48:
	lw	$4,20($sp)
	lw	$5,16($sp)
	jal	wapiUpdateUSK
	nop

$L45:
	lw	$31,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiUCastUpdateKeyTimeout
	.section	.text.wapiResendTimeout,"ax",@progbits
	.align	2
	.ent	wapiResendTimeout
	.type	wapiResendTimeout, @function
wapiResendTimeout:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,32($sp)
	sw	$4,40($sp)
	lw	$2,40($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,6676($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,652($2)
	sw	$2,20($sp)
	lw	$5,24($sp)
	lw	$2,24($sp)
	lw	$2,352($2)
	srl	$2,$2,2
	andi	$2,$2,0x3f
	addiu	$2,$2,1
	andi	$2,$2,0x00ff
	andi	$2,$2,0x3f
	sll	$4,$2,2
	lw	$3,352($5)
	li	$2,-253			# 0xffffff03
	and	$2,$3,$2
	or	$2,$2,$4
	sw	$2,352($5)
	lw	$2,24($sp)
	lw	$2,352($2)
	srl	$2,$2,2
	andi	$2,$2,0x3f
	andi	$2,$2,0x00ff
	sltu	$2,$2,7
	bne	$2,$0,$L51
	nop

	lw	$4,20($sp)
	lw	$5,16($sp)
	li	$6,25			# 0x19
	jal	wapiDeauthSta
	nop

	j	$L50
	nop

$L51:
	lw	$2,24($sp)
	sw	$2,28($sp)
	lw	$3,28($sp)
	lw	$2,364($3)
	sltu	$2,$2,14
	beq	$2,$0,$L50
	nop

	lw	$3,28($sp)
	lw	$2,364($3)
	sll	$3,$2,2
	lui	$2,%hi($L76)
	addiu	$2,$2,%lo($L76)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
$L76:
	.word	$L53
	.word	$L50
	.word	$L55
	.word	$L57
	.word	$L50
	.word	$L50
	.word	$L59
	.word	$L71
	.word	$L50
	.word	$L50
	.word	$L62
	.word	$L50
	.word	$L50
	.word	$L63
	.section	.text.wapiResendTimeout
$L53:
	lw	$2,24($sp)
	lw	$3,360($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L50
	nop

	lw	$4,16($sp)
	jal	wapiReqActiveCA
	nop

	j	$L50
	nop

$L55:
	lw	$2,24($sp)
	lw	$3,360($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L50
	nop

	lw	$2,24($sp)
	lw	$3,24($sp)
	lw	$4,20($sp)
	lw	$5,16($sp)
	lw	$6,592($2)
	lw	$7,656($3)
	jal	WapiSendActivateAuthenticationPacket
	nop

	j	$L50
	nop

$L57:
	lw	$2,24($sp)
	lw	$3,360($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L50
	nop

	lw	$2,24($sp)
	addiu	$3,$2,600
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,1000
	move	$4,$3
	move	$5,$2
	jal	mod_timer
	nop

	lw	$3,24($sp)
	lw	$2,24($sp)
	lw	$2,652($2)
	lw	$6,24($sp)
	lw	$7,24($sp)
	lw	$4,652($3)
	lw	$5,36($2)
	lw	$6,656($6)
	lw	$7,592($7)
	jal	DOT11_EnQueue
	nop

	lw	$2,24($sp)
	lw	$4,652($2)
	jal	wapi_event_indicate
	nop

	j	$L50
	nop

$L59:
	lw	$2,24($sp)
	lw	$3,360($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L60
	nop

	lw	$2,24($sp)
	lw	$3,24($sp)
	lw	$4,20($sp)
	lw	$5,16($sp)
	lw	$6,592($2)
	lw	$7,656($3)
	jal	WapiSendAuthenticationRspPacket
	nop

$L60:
	lw	$4,20($sp)
	lw	$5,16($sp)
	jal	wapiSendUnicastKeyAgrementRequeset
	nop

	j	$L50
	nop

$L62:
	lw	$4,20($sp)
	lw	$5,16($sp)
	jal	wapiSendUnicastKeyAgrementConfirm
	nop

	lw	$4,20($sp)
	lw	$5,16($sp)
	jal	wapiSendMulticastKeyNotification
	nop

	j	$L50
	nop

$L63:
	lw	$2,20($sp)
	lw	$2,1232($2)
	andi	$2,$2,0x2000
	beq	$2,$0,$L64
	nop

	lw	$4,20($sp)
	lw	$5,16($sp)
	jal	wapiSendMulticastKeyNotification
	nop

	j	$L50
	nop

$L64:
	lw	$2,24($sp)
	lw	$2,352($2)
	andi	$2,$2,0x1000
	beq	$2,$0,$L66
	nop

	lw	$4,20($sp)
	lw	$5,16($sp)
	jal	wapiSendUnicastKeyAgrementRequeset
	nop

	j	$L50
	nop

$L66:
	lw	$2,20($sp)
	lw	$2,8($2)
	lw	$2,892($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L68
	nop

	lw	$4,20($sp)
	lw	$5,16($sp)
	jal	WapiSendMulticastKeyResponse
	nop

	j	$L50
	nop

$L68:
	lw	$2,20($sp)
	lw	$2,8($2)
	lw	$2,892($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L50
	nop

	lw	$2,24($sp)
	lw	$4,652($2)
	lw	$5,16($sp)
	jal	wapiSendUnicastKeyAgrementConfirm
	nop

	lw	$2,24($sp)
	addiu	$2,$2,600
	move	$4,$2
	jal	del_timer
	nop

	lw	$2,24($sp)
	addiu	$2,$2,600
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	nop

	lw	$3,24($sp)
	lw	$2,16($sp)
	sw	$2,616($3)
	lw	$3,24($sp)
	lui	$2,%hi(wapiResendTimeout)
	addiu	$2,$2,%lo(wapiResendTimeout)
	sw	$2,612($3)
	j	$L50
	nop

$L71:
	lw	$4,20($sp)
	lw	$5,16($sp)
	jal	WapiSendUnicastKeyAggrementResponse
	nop

$L50:
	lw	$31,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiResendTimeout
	.section	.text.wapiInit,"ax",@progbits
	.align	2
	.globl	wapiInit
	.ent	wapiInit
	.type	wapiInit, @function
wapiInit:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,24($sp)
	sw	$4,32($sp)
	lw	$4,32($sp)
	jal	wapiInstallMCastKey
	nop

	li	$2,16			# 0x10
	sw	$2,16($sp)
$L78:
	lw	$2,16($sp)
	blez	$2,$L79
	nop

	lw	$3,32($sp)
	lw	$2,16($sp)
	addu	$3,$3,$2
	li	$2,92
	sb	$2,927($3)
	lw	$3,32($sp)
	lw	$2,16($sp)
	addu	$3,$3,$2
	li	$2,54
	sb	$2,926($3)
	lw	$2,16($sp)
	addiu	$2,$2,-2
	sw	$2,16($sp)
	j	$L78
	nop

$L79:
	lw	$2,32($sp)
	addiu	$3,$2,944
	lw	$2,32($sp)
	addiu	$2,$2,928
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,32($sp)
	addiu	$2,$2,1308
	move	$4,$2
	jal	del_timer
	nop

	lw	$2,32($sp)
	addiu	$2,$2,1308
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	nop

	lw	$3,32($sp)
	lw	$2,32($sp)
	sw	$2,1324($3)
	lw	$3,32($sp)
	lui	$2,%hi(wapiMCastUpdateKeyTimeout)
	addiu	$2,$2,%lo(wapiMCastUpdateKeyTimeout)
	sw	$2,1320($3)
	lw	$3,32($sp)
	lw	$2,1232($3)
	ori	$2,$2,0x1000
	sw	$2,1232($3)
	lw	$3,32($sp)
	lw	$2,32($sp)
	lw	$2,8($2)
	lw	$2,5260($2)
	sw	$2,1304($3)
	jal	init_SMS4_CK_Sbox
	nop

	lw	$31,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiInit
	.section	.text.wapiExit,"ax",@progbits
	.align	2
	.globl	wapiExit
	.ent	wapiExit
	.type	wapiExit, @function
wapiExit:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,16($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	addiu	$2,$2,1308
	move	$4,$2
	jal	del_timer
	nop

	lw	$31,16($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

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
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,32($sp)
	sw	$4,40($sp)
	lw	$2,40($sp)
	lw	$2,6676($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,600
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	nop

	lw	$3,16($sp)
	lw	$2,40($sp)
	sw	$2,616($3)
	lw	$3,16($sp)
	lui	$2,%hi(wapiResendTimeout)
	addiu	$2,$2,%lo(wapiResendTimeout)
	sw	$2,612($3)
	lw	$2,16($sp)
	addiu	$2,$2,624
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	nop

	lw	$3,16($sp)
	lw	$2,40($sp)
	sw	$2,640($3)
	lw	$3,16($sp)
	lui	$2,%hi(wapiUCastUpdateKeyTimeout)
	addiu	$2,$2,%lo(wapiUCastUpdateKeyTimeout)
	sw	$2,636($3)
	lw	$2,16($sp)
	sh	$0,358($2)
	lw	$3,16($sp)
	lw	$2,16($sp)
	lw	$2,652($2)
	lhu	$2,1236($2)
	sh	$2,356($3)
	lw	$2,16($sp)
	lw	$3,652($2)
	lw	$2,16($sp)
	lw	$2,652($2)
	lhu	$2,1236($2)
	addiu	$2,$2,8
	sh	$2,1236($3)
	li	$2,16			# 0x10
	sw	$2,20($sp)
$L83:
	lw	$2,20($sp)
	blez	$2,$L84
	nop

	lw	$3,16($sp)
	lw	$2,20($sp)
	addu	$3,$3,$2
	li	$2,92
	sb	$2,367($3)
	lw	$3,16($sp)
	lw	$2,20($sp)
	addu	$3,$3,$2
	li	$2,54
	sb	$2,366($3)
	lw	$2,20($sp)
	addiu	$2,$2,-2
	sw	$2,20($sp)
	j	$L83
	nop

$L84:
	li	$2,1			# 0x1
	sw	$2,24($sp)
$L86:
	lw	$2,24($sp)
	slt	$2,$2,4
	beq	$2,$0,$L87
	nop

	lw	$2,24($sp)
	sll	$3,$2,4
	lw	$2,16($sp)
	addu	$2,$3,$2
	addiu	$3,$2,368
	lw	$2,16($sp)
	addiu	$2,$2,368
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
	j	$L86
	nop

$L87:
	lw	$2,16($sp)
	addiu	$3,$2,432
	lw	$2,16($sp)
	addiu	$2,$2,368
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,16($sp)
	lw	$2,652($2)
	addiu	$3,$2,928
	lw	$2,16($sp)
	addiu	$2,$2,368
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,16($sp)
	lw	$2,652($2)
	addiu	$3,$2,944
	lw	$2,16($sp)
	addiu	$2,$2,368
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,16($sp)
	lw	$2,652($2)
	addiu	$3,$2,960
	lw	$2,16($sp)
	addiu	$2,$2,368
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	sw	$0,24($sp)
$L89:
	lw	$2,24($sp)
	slt	$2,$2,4
	beq	$2,$0,$L90
	nop

	lw	$3,16($sp)
	lw	$2,24($sp)
	sll	$2,$2,4
	addu	$3,$2,$3
	lw	$2,20($sp)
	addu	$3,$3,$2
	li	$2,55
	sb	$2,368($3)
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
	j	$L89
	nop

$L90:
	lw	$3,16($sp)
	lw	$2,20($sp)
	addu	$3,$3,$2
	li	$2,55
	sb	$2,432($3)
	lw	$3,16($sp)
	lw	$4,352($3)
	li	$2,-65536			# 0xffff0000
	ori	$2,$2,0x7fff
	and	$2,$4,$2
	sw	$2,352($3)
	lw	$4,16($sp)
	lw	$3,352($4)
	li	$2,-4097			# 0xffffefff
	and	$2,$3,$2
	sw	$2,352($4)
	lw	$4,16($sp)
	lw	$3,352($4)
	li	$2,-16385			# 0xffffbfff
	and	$2,$3,$2
	sw	$2,352($4)
	lw	$4,16($sp)
	lw	$3,352($4)
	li	$2,-8193			# 0xffffdfff
	and	$2,$3,$2
	sw	$2,352($4)
	lw	$4,16($sp)
	lw	$3,352($4)
	li	$2,-2049			# 0xfffff7ff
	and	$2,$3,$2
	sw	$2,352($4)
	lw	$2,16($sp)
	sb	$0,353($2)
	lw	$2,16($sp)
	sw	$0,648($2)
	lw	$2,16($sp)
	sw	$0,364($2)
	lw	$2,16($sp)
	sw	$0,656($2)
	lw	$2,16($sp)
	sw	$0,592($2)
	lw	$31,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

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
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,32($sp)
	sw	$4,40($sp)
	li	$2,256
	sh	$2,16($sp)
	li	$2,256
	sh	$2,18($sp)
	li	$2,256
	sh	$2,20($sp)
	sh	$0,22($sp)
	lw	$2,40($sp)
	sb	$0,1232($2)
	lw	$2,40($sp)
	lw	$2,908($2)
	sw	$2,24($sp)
	lw	$4,24($sp)
	addiu	$5,$sp,16
	li	$6,2			# 0x2
	jal	memcpy
	nop

	lw	$2,24($sp)
	addiu	$2,$2,2
	sw	$2,24($sp)
	lw	$3,40($sp)
	lw	$2,40($sp)
	lbu	$2,1232($2)
	addiu	$2,$2,2
	sb	$2,1232($3)
	addiu	$2,$sp,18
	lw	$4,24($sp)
	move	$5,$2
	li	$6,2			# 0x2
	jal	memcpy
	nop

	lw	$2,24($sp)
	addiu	$2,$2,2
	sw	$2,24($sp)
	lw	$3,40($sp)
	lw	$2,40($sp)
	lbu	$2,1232($2)
	addiu	$2,$2,2
	sb	$2,1232($3)
	lw	$4,24($sp)
	lui	$2,%hi(WAPI_KM_OUI)
	addiu	$5,$2,%lo(WAPI_KM_OUI)
	li	$6,3			# 0x3
	jal	memcpy
	nop

	lw	$3,24($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	lw	$2,5192($2)
	sb	$2,3($3)
	lw	$2,24($sp)
	addiu	$2,$2,4
	sw	$2,24($sp)
	lw	$3,40($sp)
	lw	$2,40($sp)
	lbu	$2,1232($2)
	addiu	$2,$2,4
	sb	$2,1232($3)
	addiu	$2,$sp,20
	lw	$4,24($sp)
	move	$5,$2
	li	$6,2			# 0x2
	jal	memcpy
	nop

	lw	$2,24($sp)
	addiu	$2,$2,2
	move	$4,$2
	lui	$2,%hi(WAPI_KM_OUI)
	addiu	$5,$2,%lo(WAPI_KM_OUI)
	li	$6,3			# 0x3
	jal	memcpy
	nop

	lw	$3,24($sp)
	li	$2,1
	sb	$2,5($3)
	lw	$2,24($sp)
	addiu	$2,$2,6
	sw	$2,24($sp)
	lw	$3,40($sp)
	lw	$2,40($sp)
	lbu	$2,1232($2)
	addiu	$2,$2,6
	sb	$2,1232($3)
	lw	$4,24($sp)
	lui	$2,%hi(WAPI_KM_OUI)
	addiu	$5,$2,%lo(WAPI_KM_OUI)
	li	$6,3			# 0x3
	jal	memcpy
	nop

	lw	$3,24($sp)
	li	$2,1
	sb	$2,3($3)
	lw	$2,24($sp)
	addiu	$2,$2,4
	sw	$2,24($sp)
	lw	$3,40($sp)
	lw	$2,40($sp)
	lbu	$2,1232($2)
	addiu	$2,$2,4
	sb	$2,1232($3)
	addiu	$2,$sp,22
	lw	$4,24($sp)
	move	$5,$2
	li	$6,2			# 0x2
	jal	memcpy
	nop

	lw	$2,24($sp)
	addiu	$2,$2,2
	sw	$2,24($sp)
	lw	$3,40($sp)
	lw	$2,40($sp)
	lbu	$2,1232($2)
	addiu	$2,$2,2
	sb	$2,1232($3)
	lw	$31,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiSetIE
	.rdata
	.align	2
$LC0:
	.ascii	"multicast or station key expansion for station unicast a"
	.ascii	"nd multicast and broadcast\000"
	.align	2
$LC1:
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.section	.text.wapiInstallMCastKey,"ax",@progbits
	.align	2
	.ent	wapiInstallMCastKey
	.type	wapiInstallMCastKey, @function
wapiInstallMCastKey:
	.set	nomips16
	.frame	$sp,88,$31		# vars= 56, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-88
	sw	$31,80($sp)
	sw	$4,88($sp)
	lui	$2,%hi($LC0)
	addiu	$2,$2,%lo($LC0)
	sw	$2,24($sp)
	lui	$3,%hi($LC1)
	lbu	$2,%lo($LC1)($3)
	sll	$4,$2,24
	addiu	$2,$3,%lo($LC1)
	lbu	$2,1($2)
	sll	$2,$2,16
	or	$4,$2,$4
	addiu	$2,$3,%lo($LC1)
	lbu	$2,2($2)
	sll	$2,$2,8
	or	$4,$2,$4
	addiu	$2,$3,%lo($LC1)
	lbu	$2,3($2)
	or	$2,$2,$4
	sw	$2,72($sp)
	addiu	$3,$3,%lo($LC1)
	lbu	$2,4($3)
	sb	$2,76($sp)
	lbu	$2,5($3)
	sb	$2,77($sp)
	lw	$4,24($sp)
	jal	strlen
	nop

	move	$7,$2
	lw	$2,88($sp)
	addiu	$3,$2,912
	addiu	$2,$sp,32
	sw	$2,16($sp)
	li	$2,32			# 0x20
	sw	$2,20($sp)
	move	$4,$3
	li	$5,16			# 0x10
	lw	$6,24($sp)
	jal	KD_hmac_sha256
	nop

	lw	$2,88($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sll	$3,$2,5
	lw	$2,88($sp)
	addu	$2,$3,$2
	addiu	$2,$2,1238
	addiu	$3,$sp,32
	move	$4,$2
	move	$5,$3
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,88($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sll	$3,$2,5
	lw	$2,88($sp)
	addu	$2,$3,$2
	addiu	$3,$2,1254
	addiu	$2,$sp,32
	addiu	$2,$2,16
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,88($sp)
	lw	$2,8($2)
	lw	$2,172($2)
	bne	$2,$0,$L93
	nop

	addiu	$5,$sp,72
	lw	$2,88($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sll	$6,$2,1
	sw	$0,16($sp)
	lw	$2,88($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sll	$3,$2,5
	lw	$2,88($sp)
	addu	$2,$3,$2
	addiu	$2,$2,1238
	sw	$2,20($sp)
	lw	$4,88($sp)
	li	$7,24			# 0x18
	jal	CamAddOneEntry
	nop

	sw	$2,64($sp)
	lw	$2,64($sp)
	beq	$2,$0,$L95
	nop

	addiu	$5,$sp,72
	lw	$2,88($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sll	$2,$2,1
	addiu	$6,$2,1
	li	$2,1			# 0x1
	sw	$2,16($sp)
	lw	$2,88($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sll	$3,$2,5
	lw	$2,88($sp)
	addu	$2,$3,$2
	addiu	$2,$2,1254
	sw	$2,20($sp)
	lw	$4,88($sp)
	li	$7,24			# 0x18
	jal	CamAddOneEntry
	nop

	sw	$2,64($sp)
	lw	$2,64($sp)
	beq	$2,$0,$L96
	nop

	lw	$2,88($sp)
	lw	$3,8($2)
	li	$2,1			# 0x1
	sw	$2,668($3)
	j	$L93
	nop

$L96:
	lw	$2,88($sp)
	lw	$2,8($2)
	sw	$0,668($2)
	j	$L93
	nop

$L95:
	lw	$2,88($sp)
	lw	$2,8($2)
	sw	$0,668($2)
$L93:
	lw	$31,80($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiInstallMCastKey
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
	.frame	$sp,216,$31		# vars= 184, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-216
	sw	$31,208($sp)
	sw	$4,216($sp)
	sw	$5,220($sp)
	lui	$2,%hi($LC2)
	addiu	$2,$2,%lo($LC2)
	sw	$2,196($sp)
	lw	$2,216($sp)
	lw	$2,6676($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,652($2)
	sw	$2,200($sp)
	sb	$0,192($sp)
	lw	$2,200($sp)
	lw	$2,8($2)
	lw	$2,892($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L100
	nop

	addiu	$3,$sp,32
	lbu	$2,192($sp)
	addu	$2,$3,$2
	addiu	$3,$2,6
	lw	$2,200($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	addiu	$3,$sp,32
	lbu	$2,192($sp)
	addu	$3,$3,$2
	lw	$2,216($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	j	$L101
	nop

$L100:
	lw	$2,200($sp)
	lw	$2,8($2)
	lw	$2,892($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L101
	nop

	addiu	$3,$sp,32
	lbu	$2,192($sp)
	addu	$3,$3,$2
	lw	$2,200($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	addiu	$3,$sp,32
	lbu	$2,192($sp)
	addu	$2,$3,$2
	addiu	$3,$2,6
	lw	$2,216($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

$L101:
	li	$2,12
	sb	$2,192($sp)
	addiu	$3,$sp,32
	lbu	$2,192($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	addiu	$2,$2,288
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcpy
	nop

	lbu	$2,192($sp)
	addiu	$2,$2,32
	sb	$2,192($sp)
	addiu	$3,$sp,32
	lbu	$2,192($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	addiu	$2,$2,256
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcpy
	nop

	lbu	$2,192($sp)
	addiu	$2,$2,32
	sb	$2,192($sp)
	lw	$4,196($sp)
	jal	strlen
	nop

	move	$6,$2
	addiu	$3,$sp,32
	lbu	$2,192($sp)
	addu	$2,$3,$2
	move	$4,$2
	lw	$5,196($sp)
	jal	memcpy
	nop

	lw	$4,196($sp)
	jal	strlen
	nop

	lbu	$3,192($sp)
	addu	$2,$3,$2
	sb	$2,192($sp)
	lw	$2,24($sp)
	addiu	$3,$2,456
	addiu	$6,$sp,32
	lbu	$7,192($sp)
	lw	$2,220($sp)
	sw	$2,16($sp)
	li	$2,96			# 0x60
	sw	$2,20($sp)
	move	$4,$3
	li	$5,16			# 0x10
	jal	KD_hmac_sha256
	nop

	lw	$31,208($sp)
	addiu	$sp,$sp,216
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiDerivedUCastKey
	.section	.text.wapiInstallUCastKey,"ax",@progbits
	.align	2
	.ent	wapiInstallUCastKey
	.type	wapiInstallUCastKey, @function
wapiInstallUCastKey:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 24, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	sw	$31,48($sp)
	sw	$4,56($sp)
	move	$2,$5
	sw	$6,64($sp)
	sb	$2,60($sp)
	lui	$3,%hi($LC1)
	lbu	$2,%lo($LC1)($3)
	sll	$4,$2,24
	addiu	$2,$3,%lo($LC1)
	lbu	$2,1($2)
	sll	$2,$2,16
	or	$4,$2,$4
	addiu	$2,$3,%lo($LC1)
	lbu	$2,2($2)
	sll	$2,$2,8
	or	$4,$2,$4
	addiu	$2,$3,%lo($LC1)
	lbu	$2,3($2)
	or	$2,$2,$4
	sw	$2,40($sp)
	addiu	$3,$3,%lo($LC1)
	lbu	$2,4($3)
	sb	$2,44($sp)
	lbu	$2,5($3)
	sb	$2,45($sp)
	lw	$2,56($sp)
	lw	$2,6676($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,652($2)
	sw	$2,32($sp)
	lw	$2,24($sp)
	addiu	$3,$2,568
	lw	$2,64($sp)
	addiu	$2,$2,32
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lbu	$2,60($sp)
	sll	$3,$2,5
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$2,$2,488
	move	$4,$2
	lw	$5,64($sp)
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lbu	$2,60($sp)
	sll	$3,$2,5
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$3,$2,504
	lw	$2,64($sp)
	addiu	$2,$2,16
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,24($sp)
	addiu	$3,$2,552
	lw	$2,64($sp)
	addiu	$2,$2,48
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,64($sp)
	addiu	$3,$2,64
	lw	$2,24($sp)
	addiu	$2,$2,288
	move	$4,$3
	li	$5,32			# 0x20
	move	$6,$2
	move	$7,$0
	jal	sha2
	nop

	lw	$2,32($sp)
	lw	$2,8($2)
	lw	$2,172($2)
	bne	$2,$0,$L103
	nop

	lw	$2,56($sp)
	addiu	$5,$2,6070
	lbu	$6,60($sp)
	sw	$0,16($sp)
	lbu	$2,60($sp)
	sll	$3,$2,5
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$2,$2,488
	sw	$2,20($sp)
	lw	$4,32($sp)
	li	$7,24			# 0x18
	jal	CamAddOneEntry
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	beq	$2,$0,$L105
	nop

	lw	$2,32($sp)
	lw	$3,60($2)
	lw	$2,32($sp)
	lw	$2,60($2)
	lw	$2,7420($2)
	addiu	$2,$2,1
	sw	$2,7420($3)
	lw	$2,56($sp)
	addiu	$5,$2,6070
	lbu	$6,60($sp)
	li	$2,1			# 0x1
	sw	$2,16($sp)
	lbu	$2,60($sp)
	sll	$3,$2,5
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$2,$2,504
	sw	$2,20($sp)
	lw	$4,32($sp)
	li	$7,24			# 0x18
	jal	CamAddOneEntry
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	beq	$2,$0,$L106
	nop

	lw	$3,56($sp)
	li	$2,1			# 0x1
	sw	$2,6124($3)
	lw	$2,32($sp)
	lw	$3,60($2)
	lw	$2,32($sp)
	lw	$2,60($2)
	lw	$2,7420($2)
	addiu	$2,$2,1
	sw	$2,7420($3)
	j	$L108
	nop

$L106:
	lw	$2,56($sp)
	sw	$0,6124($2)
	j	$L108
	nop

$L105:
	lw	$2,56($sp)
	sw	$0,6124($2)
$L108:
	lbu	$2,60($sp)
	xori	$2,$2,0x0
	sltu	$2,$2,1
	sb	$2,28($sp)
	lw	$2,56($sp)
	addiu	$2,$2,6070
	lbu	$3,28($sp)
	lw	$4,32($sp)
	move	$5,$2
	move	$6,$3
	move	$7,$0
	jal	CamDeleteOneEntry
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	beq	$2,$0,$L109
	nop

	lw	$2,32($sp)
	lw	$3,60($2)
	lw	$2,32($sp)
	lw	$2,60($2)
	lw	$2,7420($2)
	addiu	$2,$2,-1
	sw	$2,7420($3)
	lw	$2,56($sp)
	addiu	$2,$2,6070
	lbu	$3,28($sp)
	lw	$4,32($sp)
	move	$5,$2
	move	$6,$3
	move	$7,$0
	jal	CamDeleteOneEntry
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	beq	$2,$0,$L109
	nop

	lw	$2,32($sp)
	lw	$3,60($2)
	lw	$2,32($sp)
	lw	$2,60($2)
	lw	$2,7420($2)
	addiu	$2,$2,-1
	sw	$2,7420($3)
$L109:
	addiu	$5,$sp,40
	lw	$2,32($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sll	$6,$2,1
	sw	$0,16($sp)
	lw	$2,32($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sll	$3,$2,5
	lw	$2,32($sp)
	addu	$2,$3,$2
	addiu	$2,$2,1238
	sw	$2,20($sp)
	lw	$4,32($sp)
	li	$7,24			# 0x18
	jal	CamAddOneEntry
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	beq	$2,$0,$L111
	nop

	addiu	$5,$sp,40
	lw	$2,32($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sll	$2,$2,1
	addiu	$6,$2,1
	li	$2,1			# 0x1
	sw	$2,16($sp)
	lw	$2,32($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sll	$3,$2,5
	lw	$2,32($sp)
	addu	$2,$3,$2
	addiu	$2,$2,1254
	sw	$2,20($sp)
	lw	$4,32($sp)
	li	$7,24			# 0x18
	jal	CamAddOneEntry
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	beq	$2,$0,$L112
	nop

	lw	$2,32($sp)
	lw	$3,8($2)
	li	$2,1			# 0x1
	sw	$2,668($3)
	j	$L103
	nop

$L112:
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$0,668($2)
	j	$L103
	nop

$L111:
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$0,668($2)
$L103:
	lw	$31,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

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
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$31,40($sp)
	sw	$4,48($sp)
	lw	$2,48($sp)
	lw	$2,648($2)
	bne	$2,$0,$L116
	nop

	j	$L115
	nop

$L116:
	lw	$2,48($sp)
	lw	$2,648($2)
	sw	$2,20($sp)
$L117:
	lw	$2,20($sp)
	sw	$2,16($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$3,16($sp)
	lw	$2,16($sp)
	sw	$0,4($2)
	sw	$0,0($3)
	lw	$2,48($sp)
	lw	$2,652($2)
	sw	$2,24($sp)
	lw	$2,16($sp)
	sw	$2,28($sp)
	li	$2,2			# 0x2
	sw	$2,32($sp)
	lw	$4,28($sp)
	jal	dev_kfree_skb_any
	nop

	lw	$2,48($sp)
	lw	$3,648($2)
	lw	$2,20($sp)
	beq	$3,$2,$L118
	nop

	j	$L117
	nop

$L118:
	lw	$2,48($sp)
	sw	$0,648($2)
	lw	$2,48($sp)
	sb	$0,353($2)
	lw	$2,48($sp)
	sw	$0,584($2)
$L115:
	lw	$31,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiReleaseFragementQueue
	.section	.text.wapiDefragement,"ax",@progbits
	.align	2
	.ent	wapiDefragement
	.type	wapiDefragement, @function
wapiDefragement:
	.set	nomips16
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	sw	$31,64($sp)
	sw	$4,72($sp)
	sw	$5,76($sp)
	sw	$6,80($sp)
	lw	$2,72($sp)
	lw	$3,176($2)
	lw	$2,80($sp)
	addu	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,76($sp)
	lw	$2,6676($2)
	sw	$2,16($sp)
	lw	$2,20($sp)
	lw	$3,16($sp)
	lbu	$4,10($2)
	lbu	$2,353($3)
	beq	$4,$2,$L122
	nop

	lw	$4,16($sp)
	jal	wapiReleaseFragementQueue
	nop

	sw	$0,56($sp)
	j	$L121
	nop

$L122:
	lw	$3,16($sp)
	lw	$2,16($sp)
	lbu	$2,353($2)
	addiu	$2,$2,1
	sb	$2,353($3)
	lw	$2,20($sp)
	lbu	$2,11($2)
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	beq	$2,$0,$L124
	nop

	lw	$2,16($sp)
	lw	$2,652($2)
	sw	$2,32($sp)
	lw	$2,16($sp)
	lw	$2,584($2)
	sw	$2,36($sp)
	li	$2,2			# 0x2
	sw	$2,40($sp)
	li	$2,1			# 0x1
	sw	$2,44($sp)
	sw	$0,52($sp)
	lw	$4,32($sp)
	jal	alloc_skb_from_queue
	nop

	sw	$2,52($sp)
	lw	$2,52($sp)
	bne	$2,$0,$L125
	nop

	lw	$2,44($sp)
	beq	$2,$0,$L125
	nop

	lw	$4,36($sp)
	jal	dev_alloc_skb
	nop

	sw	$2,52($sp)
$L125:
	lw	$2,52($sp)
	sw	$2,48($sp)
	lw	$2,48($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L127
	nop

	lw	$4,16($sp)
	jal	wapiReleaseFragementQueue
	nop

	lw	$2,16($sp)
	lw	$2,652($2)
	sw	$2,52($sp)
	lw	$2,72($sp)
	sw	$2,48($sp)
	li	$2,2			# 0x2
	sw	$2,44($sp)
	lw	$4,48($sp)
	jal	dev_kfree_skb_any
	nop

	sw	$0,56($sp)
	j	$L121
	nop

$L127:
	lw	$2,16($sp)
	lw	$2,648($2)
	sw	$2,28($sp)
$L129:
	lw	$2,24($sp)
	lw	$3,24($sp)
	lw	$4,176($2)
	lw	$2,84($3)
	addu	$2,$4,$2
	lw	$3,28($sp)
	lw	$6,28($sp)
	move	$4,$2
	lw	$5,176($3)
	lw	$6,84($6)
	jal	memcpy
	nop

	lw	$2,28($sp)
	lw	$4,24($sp)
	lw	$5,84($2)
	jal	skb_put
	nop

	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,16($sp)
	lw	$3,648($2)
	lw	$2,28($sp)
	beq	$3,$2,$L130
	nop

	j	$L129
	nop

$L130:
	lw	$2,24($sp)
	lw	$3,24($sp)
	lw	$4,176($2)
	lw	$2,84($3)
	addu	$4,$4,$2
	lw	$2,72($sp)
	lw	$3,176($2)
	lw	$2,80($sp)
	addu	$2,$3,$2
	addiu	$5,$2,12
	lw	$2,72($sp)
	lw	$3,84($2)
	lw	$2,80($sp)
	subu	$2,$3,$2
	addiu	$2,$2,-12
	move	$6,$2
	jal	memcpy
	nop

	lw	$2,72($sp)
	lw	$3,84($2)
	lw	$2,80($sp)
	subu	$2,$3,$2
	addiu	$2,$2,-12
	lw	$4,24($sp)
	move	$5,$2
	jal	skb_put
	nop

	lw	$3,24($sp)
	lw	$2,72($sp)
	lw	$2,24($2)
	sw	$2,24($3)
	lw	$4,16($sp)
	jal	wapiReleaseFragementQueue
	nop

	lw	$2,16($sp)
	lw	$2,652($2)
	sw	$2,52($sp)
	lw	$2,72($sp)
	sw	$2,48($sp)
	li	$2,2			# 0x2
	sw	$2,44($sp)
	lw	$4,48($sp)
	jal	dev_kfree_skb_any
	nop

	lw	$2,24($sp)
	sw	$2,56($sp)
	j	$L121
	nop

$L124:
	lw	$2,16($sp)
	lw	$2,648($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	beq	$2,$0,$L134
	nop

	lw	$2,28($sp)
	lw	$3,0($2)
	lw	$2,72($sp)
	sw	$2,4($3)
	lw	$3,72($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,0($3)
	lw	$3,28($sp)
	lw	$2,72($sp)
	sw	$2,0($3)
	lw	$3,72($sp)
	lw	$2,28($sp)
	sw	$2,4($3)
	lw	$4,72($sp)
	lw	$2,72($sp)
	lw	$3,176($2)
	lw	$2,80($sp)
	addu	$2,$3,$2
	addiu	$2,$2,12
	sw	$2,176($4)
	lw	$4,72($sp)
	lw	$2,72($sp)
	lw	$3,84($2)
	lw	$2,80($sp)
	subu	$2,$3,$2
	addiu	$2,$2,-12
	sw	$2,84($4)
	lw	$5,16($sp)
	lw	$2,16($sp)
	lw	$3,72($sp)
	lw	$4,584($2)
	lw	$2,84($3)
	addu	$2,$4,$2
	sw	$2,584($5)
	j	$L135
	nop

$L134:
	lw	$3,16($sp)
	lw	$2,72($sp)
	sw	$2,648($3)
	lw	$4,72($sp)
	lw	$3,72($sp)
	lw	$2,72($sp)
	sw	$2,4($3)
	sw	$2,0($4)
	lw	$3,16($sp)
	lw	$2,72($sp)
	lw	$2,84($2)
	sw	$2,584($3)
$L135:
	sw	$0,56($sp)
$L121:
	lw	$2,56($sp)
	lw	$31,64($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiDefragement
	.section	.text.wapiFragementSend,"ax",@progbits
	.align	2
	.ent	wapiFragementSend
	.type	wapiFragementSend, @function
wapiFragementSend:
	.set	nomips16
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-96
	sw	$31,88($sp)
	sw	$4,96($sp)
	sw	$5,100($sp)
	lw	$2,100($sp)
	lw	$4,4($2)
	lw	$2,100($sp)
	lw	$2,4($2)
	lw	$2,208($2)
	andi	$3,$2,0x7
	lw	$2,208($4)
	subu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,-34
	sw	$2,20($sp)
	lw	$2,96($sp)
	lw	$2,84($2)
	addiu	$3,$2,-26
	lw	$2,20($sp)
	addiu	$2,$2,-26
	divu	$0,$3,$2
	mflo	$2
	sw	$2,24($sp)
	lw	$2,96($sp)
	lw	$2,84($2)
	addiu	$3,$2,-26
	lw	$2,20($sp)
	addiu	$2,$2,-26
	divu	$0,$3,$2
	mfhi	$2
	beq	$2,$0,$L137
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L137:
	lw	$2,96($sp)
	lw	$2,176($2)
	addiu	$2,$2,26
	sw	$2,44($sp)
	lw	$2,96($sp)
	lw	$2,84($2)
	addiu	$2,$2,-26
	sw	$2,32($sp)
	sw	$0,28($sp)
$L138:
	lw	$2,28($sp)
	lw	$3,24($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L139
	nop

	lw	$2,100($sp)
	sw	$2,52($sp)
	li	$2,2400			# 0x960
	sw	$2,56($sp)
	li	$2,1			# 0x1
	sw	$2,60($sp)
	li	$2,1			# 0x1
	sw	$2,64($sp)
	sw	$0,72($sp)
	lw	$4,52($sp)
	jal	alloc_skb_from_queue
	nop

	sw	$2,72($sp)
	lw	$2,72($sp)
	bne	$2,$0,$L141
	nop

	lw	$2,64($sp)
	beq	$2,$0,$L141
	nop

	lw	$4,56($sp)
	jal	dev_alloc_skb
	nop

	sw	$2,72($sp)
$L141:
	lw	$2,72($sp)
	sw	$2,68($sp)
	lw	$2,68($sp)
	sw	$2,40($sp)
	lw	$2,40($sp)
	bne	$2,$0,$L143
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,76($sp)
	j	$L136
	nop

$L143:
	lw	$2,40($sp)
	sw	$2,72($sp)
	li	$2,34			# 0x22
	sw	$2,68($sp)
	lw	$4,72($sp)
	lw	$2,72($sp)
	lw	$3,176($2)
	lw	$2,68($sp)
	addu	$2,$3,$2
	sw	$2,176($4)
	lw	$4,72($sp)
	lw	$2,72($sp)
	lw	$3,164($2)
	lw	$2,68($sp)
	addu	$2,$3,$2
	sw	$2,164($4)
	lw	$2,20($sp)
	addiu	$3,$2,-26
	lw	$2,32($sp)
	sltu	$2,$3,$2
	beq	$2,$0,$L145
	nop

	lw	$2,20($sp)
	sw	$2,80($sp)
	j	$L146
	nop

$L145:
	lw	$2,32($sp)
	addiu	$2,$2,26
	sw	$2,80($sp)
$L146:
	lw	$4,40($sp)
	lw	$5,80($sp)
	jal	skb_put
	nop

	lw	$2,40($sp)
	lw	$2,84($2)
	addiu	$2,$2,-26
	sw	$2,36($sp)
	lw	$2,40($sp)
	lw	$2,176($2)
	addiu	$2,$2,14
	sw	$2,48($sp)
	lw	$2,40($sp)
	lw	$3,96($sp)
	lw	$4,176($2)
	lw	$5,176($3)
	li	$6,26			# 0x1a
	jal	memcpy
	nop

	lw	$2,40($sp)
	lw	$2,176($2)
	addiu	$2,$2,26
	move	$4,$2
	lw	$5,44($sp)
	lw	$6,36($sp)
	jal	memcpy
	nop

	lw	$3,44($sp)
	lw	$2,36($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
	lw	$3,32($sp)
	lw	$2,36($sp)
	subu	$2,$3,$2
	sw	$2,32($sp)
	lw	$3,40($sp)
	lw	$2,96($sp)
	lw	$2,24($2)
	sw	$2,24($3)
	lw	$3,40($sp)
	li	$2,-30540
	sh	$2,106($3)
	lw	$3,48($sp)
	lw	$2,28($sp)
	sb	$2,10($3)
	lw	$3,48($sp)
	lw	$2,32($sp)
	sltu	$2,$0,$2
	sb	$2,11($3)
	lw	$3,48($sp)
	lw	$2,36($sp)
	addiu	$2,$2,12
	sh	$2,6($3)
	lw	$2,100($sp)
	lw	$4,40($sp)
	lw	$5,4($2)
	jal	rtl8192cd_start_xmit
	nop

	beq	$2,$0,$L140
	nop

	lw	$2,100($sp)
	sw	$2,72($sp)
	lw	$2,40($sp)
	sw	$2,68($sp)
	li	$2,1			# 0x1
	sw	$2,64($sp)
	lw	$4,68($sp)
	jal	dev_kfree_skb_any
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,76($sp)
	j	$L136
	nop

$L140:
	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
	j	$L138
	nop

$L139:
	lw	$2,100($sp)
	sw	$2,72($sp)
	lw	$2,96($sp)
	sw	$2,68($sp)
	li	$2,1			# 0x1
	sw	$2,64($sp)
	lw	$4,68($sp)
	jal	dev_kfree_skb_any
	nop

	li	$2,1			# 0x1
	sw	$2,76($sp)
$L136:
	lw	$2,76($sp)
	lw	$31,88($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

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
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	sw	$5,12($sp)
	lw	$2,12($sp)
	lbu	$3,15($2)
	lw	$2,8($sp)
	lbu	$2,15($2)
	subu	$2,$3,$2
	andi	$2,$2,0x80
	beq	$2,$0,$L151
	nop

	sw	$0,4($sp)
	j	$L150
	nop

$L151:
	li	$2,16
	sb	$2,0($sp)
$L152:
	lb	$2,0($sp)
	blez	$2,$L153
	nop

	lb	$3,0($sp)
	lw	$2,8($sp)
	addu	$4,$3,$2
	lb	$3,0($sp)
	lw	$2,12($sp)
	addu	$2,$3,$2
	lbu	$3,-1($4)
	lbu	$2,-1($2)
	bne	$3,$2,$L155
	nop

	j	$L154
	nop

$L155:
	lb	$3,0($sp)
	lw	$2,8($sp)
	addu	$4,$3,$2
	lb	$3,0($sp)
	lw	$2,12($sp)
	addu	$2,$3,$2
	lbu	$3,-1($4)
	lbu	$2,-1($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L157
	nop

	sw	$0,4($sp)
	j	$L150
	nop

$L157:
	li	$2,-1			# 0xffffffff
	sw	$2,4($sp)
	j	$L150
	nop

$L154:
	lbu	$2,0($sp)
	addiu	$2,$2,-1
	sb	$2,0($sp)
	j	$L152
	nop

$L153:
	li	$2,-1			# 0xffffffff
	sw	$2,4($sp)
$L150:
	lw	$2,4($sp)
	addiu	$sp,$sp,8
	j	$31
	nop

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
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	move	$2,$5
	sb	$2,12($sp)
	sb	$0,0($sp)
$L160:
	lbu	$2,0($sp)
	sltu	$2,$2,16
	beq	$2,$0,$L161
	nop

	lbu	$3,0($sp)
	lw	$2,8($sp)
	addu	$2,$3,$2
	lbu	$3,0($2)
	lbu	$2,12($sp)
	addu	$2,$3,$2
	slt	$2,$2,256
	beq	$2,$0,$L163
	nop

	lbu	$3,0($sp)
	lw	$2,8($sp)
	addu	$4,$3,$2
	lbu	$3,0($sp)
	lw	$2,8($sp)
	addu	$2,$3,$2
	lbu	$3,0($2)
	lbu	$2,12($sp)
	addu	$2,$3,$2
	sb	$2,0($4)
	li	$2,-1			# 0xffffffff
	sw	$2,4($sp)
	j	$L159
	nop

$L163:
	lbu	$3,0($sp)
	lw	$2,8($sp)
	addu	$4,$3,$2
	lbu	$3,0($sp)
	lw	$2,8($sp)
	addu	$2,$3,$2
	lbu	$3,0($2)
	lbu	$2,12($sp)
	addu	$2,$3,$2
	sb	$2,0($4)
	li	$2,1
	sb	$2,12($sp)
	lbu	$2,0($sp)
	addiu	$2,$2,1
	sb	$2,0($sp)
	j	$L160
	nop

$L161:
	sw	$0,4($sp)
$L159:
	lw	$2,4($sp)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	WapiIncreasePN
	.section	.text.DOT11_Process_WAPI_Info,"ax",@progbits
	.align	2
	.globl	DOT11_Process_WAPI_Info
	.ent	DOT11_Process_WAPI_Info
	.type	DOT11_Process_WAPI_Info, @function
DOT11_Process_WAPI_Info:
	.set	nomips16
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,56($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	sw	$6,72($sp)
	lw	$2,68($sp)
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,4($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,6676($2)
	bne	$2,$0,$L166
	li	$2,-1			# 0xffffffff
	sw	$2,48($sp)
	j	$L165
$L166:
	lw	$2,16($sp)
	lw	$2,6676($2)
	sw	$2,28($sp)
	lw	$2,20($sp)
	addiu	$2,$2,8
	move	$4,$2
	jal	strlen
	move	$6,$2
	lw	$3,64($sp)
	lw	$2,20($sp)
	addiu	$2,$2,8
	lw	$4,4($3)
	move	$5,$2
	jal	memcmp
	beq	$2,$0,$L167
	li	$3,-1			# 0xffffffff
	sw	$3,48($sp)
	j	$L165
$L167:
	lw	$2,20($sp)
	lhu	$2,2($2)
	sw	$2,52($sp)
	li	$2,1			# 0x1
	lw	$3,52($sp)
	beq	$3,$2,$L171
	lw	$3,52($sp)
	slt	$2,$3,2
	beq	$2,$0,$L193
	lw	$2,52($sp)
	beq	$2,$0,$L169
	j	$L168
$L193:
	li	$2,2			# 0x2
	lw	$3,52($sp)
	beq	$3,$2,$L188
	j	$L168
$L169:
	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L168
	lw	$2,28($sp)
	addiu	$2,$2,624
	move	$4,$2
	jal	del_timer
	lw	$2,28($sp)
	addiu	$2,$2,624
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	lw	$3,28($sp)
	lw	$2,16($sp)
	sw	$2,640($3)
	lw	$3,28($sp)
	lui	$2,%hi(wapiUCastUpdateKeyTimeout)
	addiu	$2,$2,%lo(wapiUCastUpdateKeyTimeout)
	sw	$2,636($3)
	lw	$4,28($sp)
	lw	$3,352($4)
	li	$2,-253			# 0xffffff03
	and	$2,$3,$2
	sw	$2,352($4)
	lw	$2,72($sp)
	addiu	$3,$2,-24
	lw	$2,20($sp)
	addiu	$2,$2,24
	lw	$4,64($sp)
	lw	$5,16($sp)
	move	$6,$3
	move	$7,$2
	jal	WapiSendActivateAuthenticationPacket
	j	$L168
$L171:
	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L168
#APP
	raw_local_irq_save	$2
#NO_APP
	sw	$2,32($sp)
	lw	$3,64($sp)
	lw	$2,20($sp)
	addiu	$2,$2,24
	sw	$2,908($3)
	lw	$4,16($sp)
	jal	wapiSetBK
	lw	$2,64($sp)
	sw	$0,908($2)
	lw	$2,32($sp)
	sw	$2,40($sp)
	lw	$2,40($sp)
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sw	$2,36($sp)
	lw	$2,36($sp)
	beq	$2,$0,$L181
	lw	$2,32($sp)
	sw	$2,40($sp)
	lw	$2,40($sp)
#APP
	raw_local_irq_restore	$2
#NO_APP
	sw	$2,44($sp)
	j	$L168
$L181:
	lw	$2,32($sp)
	sw	$2,44($sp)
	lw	$2,44($sp)
#APP
	raw_local_irq_restore	$2
#NO_APP
	sw	$2,40($sp)
	j	$L168
$L188:
	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L168
	lw	$2,20($sp)
	addiu	$2,$2,24
	sw	$2,24($sp)
	lw	$2,72($sp)
	addiu	$3,$2,-24
	lw	$2,20($sp)
	addiu	$2,$2,24
	lw	$4,64($sp)
	lw	$5,16($sp)
	move	$6,$3
	move	$7,$2
	jal	WapiSendAuthenticationRspPacket
	lw	$2,24($sp)
	lbu	$2,65($2)
	bne	$2,$0,$L190
	lw	$4,64($sp)
	lw	$5,16($sp)
	jal	wapiSendUnicastKeyAgrementRequeset
	j	$L168
$L190:
	lw	$4,64($sp)
	lw	$5,16($sp)
	li	$6,24			# 0x18
	jal	wapiDeauthSta
$L168:
	sw	$0,48($sp)
$L165:
	lw	$2,48($sp)
	lw	$31,56($sp)
	addiu	$sp,$sp,64
	j	$31
	.end	DOT11_Process_WAPI_Info
	.section	.text.WapiRecvUnicastKeyAggrementRequest,"ax",@progbits
	.align	2
	.ent	WapiRecvUnicastKeyAggrementRequest
	.type	WapiRecvUnicastKeyAggrementRequest, @function
WapiRecvUnicastKeyAggrementRequest:
	.set	nomips16
	.frame	$sp,160,$31		# vars= 136, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-160
	sw	$31,152($sp)
	sw	$4,160($sp)
	sw	$5,164($sp)
	sw	$6,168($sp)
	lw	$2,160($sp)
	lbu	$3,3($2)
	li	$2,8			# 0x8
	beq	$3,$2,$L195
	li	$2,-1			# 0xffffffff
	sw	$2,144($sp)
	j	$L194
$L195:
	lw	$2,164($sp)
	lw	$2,6676($2)
	sw	$2,20($sp)
	lw	$2,160($sp)
	addiu	$2,$2,12
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$3,$2,1
	lw	$2,20($sp)
	addiu	$2,$2,472
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcmp
	beq	$2,$0,$L196
	li	$2,-1			# 0xffffffff
	sw	$2,144($sp)
	j	$L194
$L196:
	lw	$2,16($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x10
	andi	$2,$2,0x00ff
	beq	$2,$0,$L197
	lw	$3,16($sp)
	lw	$2,20($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	lbu	$3,17($3)
	andi	$2,$2,0x00ff
	bne	$3,$2,$L198
	li	$2,-1			# 0xffffffff
	sw	$2,144($sp)
	j	$L194
$L198:
	lw	$2,16($sp)
	addiu	$3,$2,30
	lw	$2,20($sp)
	addiu	$2,$2,288
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcmp
	beq	$2,$0,$L200
	li	$2,-1			# 0xffffffff
	sw	$2,144($sp)
	j	$L194
$L197:
	lw	$2,20($sp)
	addiu	$3,$2,288
	lw	$2,16($sp)
	addiu	$2,$2,30
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcpy
$L200:
	lw	$2,20($sp)
	lw	$2,352($2)
	andi	$2,$2,0x1000
	beq	$2,$0,$L201
	lw	$2,20($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sb	$2,120($sp)
	j	$L202
$L201:
	lw	$2,20($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	sb	$2,120($sp)
$L202:
	lw	$2,20($sp)
	addiu	$2,$2,256
	move	$4,$2
	li	$5,32			# 0x20
	jal	GenerateRandomData
	addiu	$2,$sp,24
	lw	$4,164($sp)
	move	$5,$2
	jal	wapiDerivedUCastKey
	lbu	$2,120($sp)
	addiu	$3,$sp,24
	lw	$4,164($sp)
	move	$5,$2
	move	$6,$3
	jal	wapiInstallUCastKey
	lw	$3,20($sp)
	lw	$2,352($3)
	ori	$2,$2,0x4000
	sw	$2,352($3)
	lw	$2,20($sp)
	lw	$2,652($2)
	sw	$2,128($sp)
#APP
	raw_local_irq_save	$2
#NO_APP
	sw	$2,124($sp)
	lw	$2,20($sp)
	lw	$3,652($2)
	lw	$2,164($sp)
	lw	$2,6676($2)
	addiu	$2,$2,2
	sw	$2,908($3)
	lw	$2,20($sp)
	lw	$4,652($2)
	jal	wapiSetIE
	lw	$3,20($sp)
	li	$2,68
	sb	$2,0($3)
	lw	$3,20($sp)
	lw	$2,20($sp)
	lw	$2,652($2)
	lbu	$2,1232($2)
	sb	$2,1($3)
	lw	$3,20($sp)
	lw	$2,20($sp)
	lw	$2,652($2)
	lbu	$2,1232($2)
	addiu	$2,$2,2
	sb	$2,352($3)
	lw	$2,124($sp)
	sw	$2,136($sp)
	lw	$2,136($sp)
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sw	$2,132($sp)
	lw	$2,132($sp)
	beq	$2,$0,$L211
	lw	$2,124($sp)
	sw	$2,136($sp)
	lw	$2,136($sp)
#APP
	raw_local_irq_restore	$2
#NO_APP
	sw	$2,140($sp)
	j	$L208
$L211:
	lw	$2,124($sp)
	sw	$2,140($sp)
	lw	$2,140($sp)
#APP
	raw_local_irq_restore	$2
#NO_APP
	sw	$2,136($sp)
$L208:
	sw	$0,144($sp)
$L194:
	lw	$2,144($sp)
	lw	$31,152($sp)
	addiu	$sp,$sp,160
	j	$31
	.end	WapiRecvUnicastKeyAggrementRequest
	.section	.text.WapiRecvUnicastKeyAggrementConfirm,"ax",@progbits
	.align	2
	.ent	WapiRecvUnicastKeyAggrementConfirm
	.type	WapiRecvUnicastKeyAggrementConfirm, @function
WapiRecvUnicastKeyAggrementConfirm:
	.set	nomips16
	.frame	$sp,72,$31		# vars= 40, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	sw	$31,64($sp)
	sw	$4,72($sp)
	sw	$5,76($sp)
	sw	$6,80($sp)
	lw	$2,72($sp)
	lbu	$3,3($2)
	li	$2,10			# 0xa
	beq	$3,$2,$L219
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,56($sp)
	j	$L218
	nop

$L219:
	lw	$2,76($sp)
	lw	$2,6676($2)
	sw	$2,28($sp)
	lw	$2,72($sp)
	addiu	$2,$2,12
	sw	$2,24($sp)
	lw	$2,24($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x10
	andi	$2,$2,0x00ff
	beq	$2,$0,$L220
	nop

	lw	$3,24($sp)
	lw	$2,28($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	lbu	$3,17($3)
	andi	$2,$2,0x00ff
	bne	$3,$2,$L220
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,56($sp)
	j	$L218
	nop

$L220:
	lw	$2,24($sp)
	addiu	$3,$2,1
	lw	$2,28($sp)
	addiu	$2,$2,472
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcmp
	nop

	bne	$2,$0,$L223
	nop

	lw	$2,24($sp)
	addiu	$3,$2,30
	lw	$2,28($sp)
	addiu	$2,$2,256
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcmp
	nop

	bne	$2,$0,$L223
	nop

	lw	$2,24($sp)
	addiu	$3,$2,18
	lw	$2,76($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcmp
	nop

	bne	$2,$0,$L223
	nop

	lw	$2,24($sp)
	addiu	$3,$2,24
	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcmp
	nop

	bne	$2,$0,$L223
	nop

	j	$L222
	nop

$L223:
	li	$2,-1			# 0xffffffff
	sw	$2,56($sp)
	j	$L218
	nop

$L222:
	lw	$2,28($sp)
	addiu	$3,$2,568
	lw	$2,72($sp)
	addiu	$6,$2,12
	lw	$2,72($sp)
	lhu	$2,6($2)
	addiu	$7,$2,-32
	addiu	$2,$sp,32
	sw	$2,16($sp)
	li	$2,20			# 0x14
	sw	$2,20($sp)
	move	$4,$3
	li	$5,16			# 0x10
	jal	sha256_hmac
	nop

	addiu	$4,$sp,32
	lw	$2,72($sp)
	lhu	$3,6($2)
	lw	$2,72($sp)
	addu	$2,$3,$2
	addiu	$2,$2,-20
	move	$5,$2
	li	$6,20			# 0x14
	jal	memcmp
	nop

	beq	$2,$0,$L224
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,56($sp)
	j	$L218
	nop

$L224:
	lw	$2,24($sp)
	addiu	$4,$2,62
	lw	$2,28($sp)
	lw	$2,652($2)
	addiu	$3,$2,976
	lw	$2,28($sp)
	lw	$2,652($2)
	lbu	$2,1233($2)
	move	$5,$3
	move	$6,$2
	jal	memcmp
	nop

	beq	$2,$0,$L225
	nop

	lw	$3,80($sp)
	li	$2,27			# 0x1b
	sw	$2,0($3)
	li	$2,-2			# 0xfffffffe
	sw	$2,56($sp)
	j	$L218
	nop

$L225:
	lw	$2,28($sp)
	lw	$2,656($2)
	beq	$2,$0,$L226
	nop

	lw	$2,28($sp)
	lw	$4,656($2)
	jal	kfree
	nop

	lw	$2,28($sp)
	sw	$0,656($2)
	lw	$2,28($sp)
	sw	$0,592($2)
$L226:
	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,7			# 0x7
	bne	$3,$2,$L227
	nop

	lw	$3,28($sp)
	li	$2,9			# 0x9
	sw	$2,364($3)
	lw	$3,28($sp)
	lw	$2,352($3)
	ori	$2,$2,0x4000
	sw	$2,352($3)
	lw	$3,28($sp)
	lw	$2,352($3)
	ori	$2,$2,0x2000
	sw	$2,352($3)
$L227:
	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,8($2)
	lw	$3,5240($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L229
	nop

	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,8($2)
	lw	$3,5240($2)
	li	$2,4			# 0x4
	beq	$3,$2,$L229
	nop

	j	$L228
	nop

$L229:
	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,8($2)
	lw	$3,5244($2)
	move	$2,$3
	sll	$2,$2,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$2,$2,2
	bgez	$2,$L230
	nop

	lw	$2,28($sp)
	addiu	$4,$2,624
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	li	$2,2147418112			# 0x7fff0000
	ori	$2,$2,0xffff
	addu	$2,$3,$2
	move	$5,$2
	jal	mod_timer
	nop

	j	$L232
	nop

$L230:
	lw	$2,28($sp)
	addiu	$4,$2,624
	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,8($2)
	lw	$3,5244($2)
	move	$2,$3
	sll	$2,$2,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$3,$2,2
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$2,$3,$2
	move	$5,$2
	jal	mod_timer
	nop

	j	$L232
	nop

$L228:
	lw	$2,28($sp)
	addiu	$2,$2,624
	move	$4,$2
	jal	del_timer
	nop

	lw	$2,28($sp)
	addiu	$2,$2,624
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	nop

	lw	$3,28($sp)
	lw	$2,76($sp)
	sw	$2,640($3)
	lw	$3,28($sp)
	lui	$2,%hi(wapiUCastUpdateKeyTimeout)
	addiu	$2,$2,%lo(wapiUCastUpdateKeyTimeout)
	sw	$2,636($3)
$L232:
	lw	$4,28($sp)
	lw	$3,352($4)
	li	$2,-253			# 0xffffff03
	and	$2,$3,$2
	sw	$2,352($4)
	lw	$3,28($sp)
	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,8($2)
	lw	$2,5248($2)
	sw	$2,596($3)
	sw	$0,56($sp)
$L218:
	lw	$2,56($sp)
	lw	$31,64($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	WapiRecvUnicastKeyAggrementConfirm
	.section	.text.WapiRecvMulticastKeyNotification,"ax",@progbits
	.align	2
	.ent	WapiRecvMulticastKeyNotification
	.type	WapiRecvMulticastKeyNotification, @function
WapiRecvMulticastKeyNotification:
	.set	nomips16
	.frame	$sp,88,$31		# vars= 48, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-88
	sw	$31,80($sp)
	sw	$4,88($sp)
	sw	$5,92($sp)
	sw	$6,96($sp)
	lw	$2,88($sp)
	lbu	$3,3($2)
	li	$2,11			# 0xb
	beq	$3,$2,$L234
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,72($sp)
	j	$L233
	nop

$L234:
	lw	$2,92($sp)
	lw	$2,6676($2)
	sw	$2,36($sp)
	lw	$2,88($sp)
	addiu	$2,$2,12
	sw	$2,32($sp)
	lw	$2,36($sp)
	lw	$2,652($2)
	sw	$2,64($sp)
	lw	$2,32($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x20
	andi	$2,$2,0x00ff
	beq	$2,$0,$L235
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,72($sp)
	j	$L233
	nop

$L235:
	lw	$2,36($sp)
	addiu	$3,$2,568
	lw	$2,88($sp)
	lhu	$2,6($2)
	addiu	$7,$2,-32
	addiu	$2,$sp,40
	sw	$2,16($sp)
	li	$2,20			# 0x14
	sw	$2,20($sp)
	move	$4,$3
	li	$5,16			# 0x10
	lw	$6,32($sp)
	jal	sha256_hmac
	nop

	addiu	$4,$sp,40
	lw	$2,88($sp)
	lhu	$3,6($2)
	lw	$2,88($sp)
	addu	$2,$3,$2
	addiu	$2,$2,-20
	move	$5,$2
	li	$6,20			# 0x14
	jal	memcmp
	nop

	beq	$2,$0,$L236
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,72($sp)
	j	$L233
	nop

$L236:
	lw	$2,32($sp)
	addiu	$3,$2,31
	lw	$2,64($sp)
	addiu	$2,$2,960
	move	$4,$3
	move	$5,$2
	jal	WapiComparePN
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffff
	bne	$3,$2,$L237
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,72($sp)
	j	$L233
	nop

$L237:
	lw	$2,64($sp)
	addiu	$3,$2,960
	lw	$2,32($sp)
	addiu	$2,$2,31
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$3,$2,552
	lw	$2,64($sp)
	addiu	$5,$2,960
	lw	$2,32($sp)
	addiu	$6,$2,48
	lw	$2,32($sp)
	lbu	$7,47($2)
	lw	$2,64($sp)
	addiu	$2,$2,912
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$3
	jal	WapiSMS4ForMNKEncrypt
	nop

	lw	$4,64($sp)
	jal	wapiInstallMCastKey
	nop

	lw	$2,36($sp)
	lw	$3,364($2)
	li	$2,9			# 0x9
	bne	$3,$2,$L238
	nop

	lw	$3,36($sp)
	li	$2,11			# 0xb
	sw	$2,364($3)
	lw	$3,36($sp)
	lw	$2,352($3)
	ori	$2,$2,0x800
	sw	$2,352($3)
$L238:
	lw	$2,36($sp)
	lw	$2,652($2)
	lw	$2,1304($2)
	bne	$2,$0,$L239
	nop

	lw	$2,36($sp)
	lw	$3,652($2)
	lw	$2,36($sp)
	lw	$2,652($2)
	lw	$2,8($2)
	lw	$2,5260($2)
	sw	$2,1304($3)
$L239:
	lw	$4,36($sp)
	lw	$3,352($4)
	li	$2,-253			# 0xffffff03
	and	$2,$3,$2
	sw	$2,352($4)
	sw	$0,72($sp)
$L233:
	lw	$2,72($sp)
	lw	$31,80($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	WapiRecvMulticastKeyNotification
	.section	.text.WapiRecvAccessAuthenticateRequest,"ax",@progbits
	.align	2
	.ent	WapiRecvAccessAuthenticateRequest
	.type	WapiRecvAccessAuthenticateRequest, @function
WapiRecvAccessAuthenticateRequest:
	.set	nomips16
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-96
	sw	$31,88($sp)
	sw	$4,96($sp)
	sw	$5,100($sp)
	sw	$6,104($sp)
	lw	$2,100($sp)
	lw	$2,6676($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$3,364($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L241
	nop

	lw	$2,96($sp)
	lbu	$3,3($2)
	li	$2,4			# 0x4
	beq	$3,$2,$L242
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,84($sp)
	j	$L240
	nop

$L242:
	lw	$2,96($sp)
	addiu	$2,$2,12
	sw	$2,16($sp)
	lw	$2,16($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L244
	nop

	lw	$2,16($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x4
	andi	$2,$2,0x00ff
	beq	$2,$0,$L244
	nop

	lw	$2,16($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L244
	nop

	j	$L243
	nop

$L244:
	li	$2,-1			# 0xffffffff
	sw	$2,84($sp)
	j	$L240
	nop

$L243:
	lw	$2,16($sp)
	lbu	$3,65($2)
	lw	$2,16($sp)
	addu	$2,$3,$2
	addiu	$2,$2,66
	sw	$2,24($sp)
	lw	$2,24($sp)
	lhu	$3,2($2)
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,24($sp)
	lw	$2,24($sp)
	lhu	$3,2($2)
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,32($sp)
	lw	$2,20($sp)
	lw	$2,656($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	lhu	$3,2($2)
	lw	$2,28($sp)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,28($sp)
	lw	$2,28($sp)
	lhu	$3,2($2)
	lw	$2,28($sp)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,36($sp)
	lw	$3,32($sp)
	lbu	$2,1($3)
	sll	$4,$2,8
	lbu	$2,2($3)
	or	$5,$2,$4
	lw	$3,36($sp)
	lbu	$2,1($3)
	sll	$4,$2,8
	lbu	$2,2($3)
	or	$2,$2,$4
	andi	$3,$5,0xffff
	andi	$2,$2,0xffff
	bne	$3,$2,$L246
	nop

	lw	$3,32($sp)
	lbu	$2,1($3)
	sll	$4,$2,8
	lbu	$2,2($3)
	or	$2,$2,$4
	andi	$2,$2,0xffff
	addiu	$2,$2,3
	lw	$4,32($sp)
	lw	$5,36($sp)
	move	$6,$2
	jal	memcmp
	nop

	bne	$2,$0,$L246
	nop

	j	$L245
	nop

$L246:
	li	$2,-1			# 0xffffffff
	sw	$2,84($sp)
	j	$L240
	nop

$L245:
	lw	$3,20($sp)
	li	$2,3			# 0x3
	sw	$2,364($3)
	lw	$4,20($sp)
	lw	$3,352($4)
	li	$2,-253			# 0xffffff03
	and	$2,$3,$2
	sw	$2,352($4)
	lw	$2,20($sp)
	lw	$2,656($2)
	sw	$2,40($sp)
	lw	$3,40($sp)
	li	$2,4
	sh	$2,2($3)
	lw	$3,40($sp)
	lw	$2,100($sp)
	sw	$2,4($3)
	lw	$2,40($sp)
	addiu	$2,$2,8
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	jal	memset
	nop

	lw	$2,20($sp)
	lw	$2,652($2)
	lw	$4,4($2)
	jal	strlen
	nop

	move	$6,$2
	lw	$2,40($sp)
	addiu	$3,$2,8
	lw	$2,20($sp)
	lw	$2,652($2)
	move	$4,$3
	lw	$5,4($2)
	jal	memcpy
	nop

	lw	$3,20($sp)
	lw	$2,96($sp)
	lhu	$2,6($2)
	addiu	$2,$2,-12
	sw	$2,592($3)
	lw	$2,40($sp)
	addiu	$4,$2,24
	lw	$2,96($sp)
	addiu	$2,$2,12
	lw	$3,20($sp)
	move	$5,$2
	lw	$6,592($3)
	jal	memcpy
	nop

	addiu	$2,$sp,48
	move	$4,$2
	li	$5,32			# 0x20
	jal	GenerateRandomData
	nop

	lw	$2,40($sp)
	addiu	$2,$2,24
	sw	$2,80($sp)
	lw	$2,20($sp)
	lw	$3,80($sp)
	lw	$2,592($2)
	addu	$2,$3,$2
	sw	$2,80($sp)
	addiu	$2,$sp,48
	lw	$4,80($sp)
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcpy
	nop

	lw	$3,20($sp)
	lw	$2,20($sp)
	lw	$2,592($2)
	addiu	$2,$2,24
	sw	$2,592($3)
	lw	$3,20($sp)
	lw	$2,20($sp)
	lw	$2,592($2)
	addiu	$2,$2,32
	sw	$2,592($3)
$L241:
	lw	$2,20($sp)
	addiu	$3,$2,600
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,1000
	move	$4,$3
	move	$5,$2
	jal	mod_timer
	nop

	lw	$3,20($sp)
	lw	$2,20($sp)
	lw	$2,652($2)
	lw	$6,20($sp)
	lw	$7,20($sp)
	lw	$4,652($3)
	lw	$5,36($2)
	lw	$6,656($6)
	lw	$7,592($7)
	jal	DOT11_EnQueue
	nop

	lw	$2,20($sp)
	lw	$4,652($2)
	jal	wapi_event_indicate
	nop

	sw	$0,84($sp)
$L240:
	lw	$2,84($sp)
	lw	$31,88($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	WapiRecvAccessAuthenticateRequest
	.section	.text.wapiRecvUnicastKeyAgreementResponse,"ax",@progbits
	.align	2
	.ent	wapiRecvUnicastKeyAgreementResponse
	.type	wapiRecvUnicastKeyAgreementResponse, @function
wapiRecvUnicastKeyAgreementResponse:
	.set	nomips16
	.frame	$sp,168,$31		# vars= 136, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-168
	sw	$31,160($sp)
	sw	$4,168($sp)
	sw	$5,172($sp)
	sw	$6,176($sp)
	lw	$2,168($sp)
	lbu	$3,3($2)
	li	$2,9			# 0x9
	beq	$3,$2,$L248
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,156($sp)
	j	$L247
	nop

$L248:
	lw	$2,172($sp)
	lw	$2,6676($2)
	sw	$2,28($sp)
	lw	$2,168($sp)
	addiu	$2,$2,12
	sw	$2,24($sp)
	lw	$2,24($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x10
	andi	$2,$2,0x00ff
	beq	$2,$0,$L249
	nop

	lw	$3,24($sp)
	lw	$2,28($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	lbu	$3,17($3)
	andi	$2,$2,0x00ff
	bne	$3,$2,$L249
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,156($sp)
	j	$L247
	nop

$L249:
	lw	$2,28($sp)
	lw	$2,352($2)
	andi	$2,$2,0x1000
	beq	$2,$0,$L251
	nop

	lw	$2,28($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sb	$2,152($sp)
	j	$L252
	nop

$L251:
	lw	$2,28($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	sb	$2,152($sp)
$L252:
	lw	$2,24($sp)
	addiu	$3,$2,1
	lw	$2,28($sp)
	addiu	$2,$2,472
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcmp
	nop

	bne	$2,$0,$L254
	nop

	lw	$2,24($sp)
	lbu	$3,17($2)
	lbu	$2,152($sp)
	beq	$3,$2,$L255
	nop

	lw	$2,28($sp)
	lw	$2,352($2)
	andi	$2,$2,0x1000
	bne	$2,$0,$L255
	nop

	j	$L254
	nop

$L255:
	lw	$2,24($sp)
	addiu	$3,$2,62
	lw	$2,28($sp)
	addiu	$2,$2,288
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcmp
	nop

	bne	$2,$0,$L254
	nop

	lw	$2,24($sp)
	addiu	$3,$2,18
	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcmp
	nop

	bne	$2,$0,$L254
	nop

	lw	$2,24($sp)
	addiu	$3,$2,24
	lw	$2,172($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcmp
	nop

	bne	$2,$0,$L254
	nop

	j	$L253
	nop

$L254:
	li	$2,-1			# 0xffffffff
	sw	$2,156($sp)
	j	$L247
	nop

$L253:
	lw	$2,28($sp)
	addiu	$3,$2,256
	lw	$2,24($sp)
	addiu	$2,$2,30
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcpy
	nop

	addiu	$2,$sp,32
	lw	$4,172($sp)
	move	$5,$2
	jal	wapiDerivedUCastKey
	nop

	addiu	$2,$sp,32
	addiu	$3,$2,32
	lw	$2,168($sp)
	addiu	$6,$2,12
	lw	$2,168($sp)
	lhu	$2,6($2)
	addiu	$7,$2,-32
	addiu	$2,$sp,128
	sw	$2,16($sp)
	li	$2,20			# 0x14
	sw	$2,20($sp)
	move	$4,$3
	li	$5,16			# 0x10
	jal	sha256_hmac
	nop

	addiu	$4,$sp,128
	lw	$2,168($sp)
	lhu	$3,6($2)
	lw	$2,168($sp)
	addu	$2,$3,$2
	addiu	$2,$2,-20
	move	$5,$2
	li	$6,20			# 0x14
	jal	memcmp
	nop

	beq	$2,$0,$L256
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,156($sp)
	j	$L247
	nop

$L256:
	lw	$2,24($sp)
	addiu	$3,$2,94
	lw	$2,28($sp)
	lbu	$2,352($2)
	move	$4,$3
	lw	$5,28($sp)
	move	$6,$2
	jal	memcmp
	nop

	beq	$2,$0,$L257
	nop

	lw	$3,176($sp)
	li	$2,27			# 0x1b
	sw	$2,0($3)
	li	$2,-2			# 0xfffffffe
	sw	$2,156($sp)
	j	$L247
	nop

$L257:
	lbu	$2,152($sp)
	addiu	$3,$sp,32
	lw	$4,172($sp)
	move	$5,$2
	move	$6,$3
	jal	wapiInstallUCastKey
	nop

	lw	$2,28($sp)
	lw	$2,656($2)
	beq	$2,$0,$L258
	nop

	lw	$2,28($sp)
	lw	$4,656($2)
	jal	kfree
	nop

	lw	$2,28($sp)
	sw	$0,656($2)
	lw	$2,28($sp)
	sw	$0,592($2)
$L258:
	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,6			# 0x6
	bne	$3,$2,$L259
	nop

	lw	$3,28($sp)
	li	$2,8			# 0x8
	sw	$2,364($3)
	lw	$3,28($sp)
	lw	$2,352($3)
	ori	$2,$2,0x4000
	sw	$2,352($3)
	lw	$3,28($sp)
	lw	$2,352($3)
	ori	$2,$2,0x2000
	sw	$2,352($3)
$L259:
	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,8($2)
	lw	$3,5240($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L261
	nop

	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,8($2)
	lw	$3,5240($2)
	li	$2,4			# 0x4
	beq	$3,$2,$L261
	nop

	j	$L260
	nop

$L261:
	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,8($2)
	lw	$3,5244($2)
	move	$2,$3
	sll	$2,$2,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$2,$2,2
	bgez	$2,$L262
	nop

	lw	$2,28($sp)
	addiu	$4,$2,624
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	li	$2,2147418112			# 0x7fff0000
	ori	$2,$2,0xffff
	addu	$2,$3,$2
	move	$5,$2
	jal	mod_timer
	nop

	j	$L264
	nop

$L262:
	lw	$2,28($sp)
	addiu	$4,$2,624
	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,8($2)
	lw	$3,5244($2)
	move	$2,$3
	sll	$2,$2,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$3,$2,2
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$2,$3,$2
	move	$5,$2
	jal	mod_timer
	nop

	j	$L264
	nop

$L260:
	lw	$2,28($sp)
	addiu	$2,$2,624
	move	$4,$2
	jal	del_timer
	nop

	lw	$2,28($sp)
	addiu	$2,$2,624
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	nop

	lw	$3,28($sp)
	lw	$2,172($sp)
	sw	$2,640($3)
	lw	$3,28($sp)
	lui	$2,%hi(wapiUCastUpdateKeyTimeout)
	addiu	$2,$2,%lo(wapiUCastUpdateKeyTimeout)
	sw	$2,636($3)
$L264:
	lw	$4,28($sp)
	lw	$3,352($4)
	li	$2,-253			# 0xffffff03
	and	$2,$3,$2
	sw	$2,352($4)
	lw	$3,28($sp)
	lw	$2,28($sp)
	lw	$2,652($2)
	lw	$2,8($2)
	lw	$2,5248($2)
	sw	$2,596($3)
	sw	$0,156($sp)
$L247:
	lw	$2,156($sp)
	lw	$31,160($sp)
	addiu	$sp,$sp,168
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiRecvUnicastKeyAgreementResponse
	.section	.text.wapiRecvMulticastKeyResponse,"ax",@progbits
	.align	2
	.ent	wapiRecvMulticastKeyResponse
	.type	wapiRecvMulticastKeyResponse, @function
wapiRecvMulticastKeyResponse:
	.set	nomips16
	.frame	$sp,80,$31		# vars= 48, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-80
	sw	$31,72($sp)
	sw	$4,80($sp)
	sw	$5,84($sp)
	sw	$6,88($sp)
	lw	$2,80($sp)
	lbu	$3,3($2)
	li	$2,12			# 0xc
	beq	$3,$2,$L266
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,64($sp)
	j	$L265
	nop

$L266:
	lw	$2,84($sp)
	lw	$2,6676($2)
	sw	$2,28($sp)
	lw	$2,80($sp)
	addiu	$2,$2,12
	sw	$2,24($sp)
	lw	$2,28($sp)
	lw	$2,652($2)
	sw	$2,32($sp)
	lw	$3,24($sp)
	lw	$2,28($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	lbu	$3,2($3)
	andi	$2,$2,0x00ff
	bne	$3,$2,$L268
	nop

	lw	$3,24($sp)
	lw	$2,32($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	lbu	$3,1($3)
	andi	$2,$2,0x00ff
	beq	$3,$2,$L269
	nop

	lw	$2,32($sp)
	lw	$2,1232($2)
	andi	$2,$2,0x2000
	bne	$2,$0,$L269
	nop

	j	$L268
	nop

$L269:
	lw	$2,24($sp)
	addiu	$3,$2,15
	lw	$2,32($sp)
	addiu	$2,$2,960
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcmp
	nop

	bne	$2,$0,$L268
	nop

	lw	$2,24($sp)
	addiu	$3,$2,3
	lw	$2,32($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcmp
	nop

	bne	$2,$0,$L268
	nop

	lw	$2,24($sp)
	addiu	$3,$2,9
	lw	$2,84($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcmp
	nop

	bne	$2,$0,$L268
	nop

	j	$L267
	nop

$L268:
	li	$2,-1			# 0xffffffff
	sw	$2,64($sp)
	j	$L265
	nop

$L267:
	lw	$2,28($sp)
	addiu	$3,$2,568
	addiu	$2,$sp,40
	sw	$2,16($sp)
	li	$2,20			# 0x14
	sw	$2,20($sp)
	move	$4,$3
	li	$5,16			# 0x10
	lw	$6,24($sp)
	li	$7,31			# 0x1f
	jal	sha256_hmac
	nop

	addiu	$3,$sp,40
	lw	$2,24($sp)
	addiu	$2,$2,31
	move	$4,$3
	move	$5,$2
	li	$6,20			# 0x14
	jal	memcmp
	nop

	beq	$2,$0,$L270
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,64($sp)
	j	$L265
	nop

$L270:
	lw	$3,28($sp)
	li	$2,13			# 0xd
	sw	$2,364($3)
	lw	$2,28($sp)
	addiu	$2,$2,600
	move	$4,$2
	jal	del_timer
	nop

	lw	$2,28($sp)
	addiu	$2,$2,600
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	nop

	lw	$3,28($sp)
	lw	$2,84($sp)
	sw	$2,616($3)
	lw	$3,28($sp)
	lui	$2,%hi(wapiResendTimeout)
	addiu	$2,$2,%lo(wapiResendTimeout)
	sw	$2,612($3)
	lw	$4,28($sp)
	lw	$3,352($4)
	li	$2,-253			# 0xffffff03
	and	$2,$3,$2
	sw	$2,352($4)
	lw	$3,28($sp)
	lw	$2,352($3)
	ori	$2,$2,0x800
	sw	$2,352($3)
	lw	$2,32($sp)
	lw	$2,1304($2)
	bne	$2,$0,$L271
	nop

	lw	$3,32($sp)
	lw	$2,32($sp)
	lw	$2,8($2)
	lw	$2,5260($2)
	sw	$2,1304($3)
$L271:
	lw	$2,32($sp)
	lw	$2,1232($2)
	andi	$2,$2,0x1000
	beq	$2,$0,$L272
	nop

	lw	$2,32($sp)
	lw	$2,8($2)
	lw	$3,5252($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L273
	nop

	lw	$2,32($sp)
	lw	$2,8($2)
	lw	$3,5252($2)
	li	$2,4			# 0x4
	beq	$3,$2,$L273
	nop

	j	$L272
	nop

$L273:
	lw	$2,32($sp)
	lw	$2,8($2)
	lw	$3,5256($2)
	move	$2,$3
	sll	$2,$2,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$2,$2,2
	bgez	$2,$L274
	nop

	lw	$2,32($sp)
	addiu	$4,$2,1308
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	li	$2,2147418112			# 0x7fff0000
	ori	$2,$2,0xffff
	addu	$2,$3,$2
	move	$5,$2
	jal	mod_timer
	nop

	j	$L275
	nop

$L274:
	lw	$2,32($sp)
	addiu	$4,$2,1308
	lw	$2,32($sp)
	lw	$2,8($2)
	lw	$3,5256($2)
	move	$2,$3
	sll	$2,$2,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$3,$2,2
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$2,$3,$2
	move	$5,$2
	jal	mod_timer
	nop

$L275:
	lw	$4,32($sp)
	lw	$3,1232($4)
	li	$2,-4097			# 0xffffefff
	and	$2,$3,$2
	sw	$2,1232($4)
$L272:
	sw	$0,64($sp)
$L265:
	lw	$2,64($sp)
	lw	$31,72($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiRecvMulticastKeyResponse
	.section	.text.wapiRecvKeyUpdateResponse,"ax",@progbits
	.align	2
	.ent	wapiRecvKeyUpdateResponse
	.type	wapiRecvKeyUpdateResponse, @function
wapiRecvKeyUpdateResponse:
	.set	nomips16
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	sw	$31,64($sp)
	sw	$4,72($sp)
	sw	$5,76($sp)
	sw	$6,80($sp)
	li	$2,-1			# 0xffffffff
	sw	$2,20($sp)
	lw	$2,76($sp)
	lw	$2,6676($2)
	sw	$2,16($sp)
	lw	$2,72($sp)
	lbu	$3,3($2)
	li	$2,12			# 0xc
	bne	$3,$2,$L277
	nop

	lw	$2,16($sp)
	lw	$2,652($2)
	lw	$2,1232($2)
	andi	$2,$2,0x2000
	beq	$2,$0,$L289
	nop

	lw	$4,72($sp)
	lw	$5,76($sp)
	lw	$6,80($sp)
	jal	wapiRecvMulticastKeyResponse
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L289
	nop

	lw	$3,16($sp)
	lw	$2,352($3)
	ori	$2,$2,0x400
	sw	$2,352($3)
	li	$2,1			# 0x1
	sw	$2,32($sp)
	sw	$0,48($sp)
$L280:
	lw	$2,48($sp)
	sltu	$2,$2,31
	beq	$2,$0,$L281
	nop

	lw	$4,16($sp)
	lw	$2,48($sp)
	sll	$3,$2,3
	lw	$2,652($4)
	addu	$3,$3,$2
	li	$2,65536			# 0x10000
	ori	$2,$2,0x2a74
	addu	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
$L283:
	lw	$3,36($sp)
	lw	$2,40($sp)
	beq	$3,$2,$L284
	nop

	lw	$2,36($sp)
	sw	$2,56($sp)
	lw	$2,56($sp)
	sw	$2,44($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,44($sp)
	lw	$2,6676($2)
	lw	$2,364($2)
	sltu	$2,$2,13
	beq	$2,$0,$L285
	nop

	lw	$2,16($sp)
	lw	$4,652($2)
	lw	$5,44($sp)
	li	$6,25			# 0x19
	jal	wapiDeauthSta
	nop

	j	$L283
	nop

$L285:
	lw	$2,44($sp)
	lw	$2,6676($2)
	lw	$2,352($2)
	andi	$2,$2,0x400
	bne	$2,$0,$L283
	nop

	sw	$0,32($sp)
$L284:
	lw	$2,32($sp)
	bne	$2,$0,$L282
	nop

	j	$L281
	nop

$L282:
	lw	$2,48($sp)
	addiu	$2,$2,1
	sw	$2,48($sp)
	j	$L280
	nop

$L281:
	lw	$3,32($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L289
	nop

	lw	$2,16($sp)
	lw	$5,652($2)
	lw	$2,16($sp)
	lw	$2,652($2)
	lw	$2,1232($2)
	srl	$2,$2,15
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	sll	$4,$2,15
	lw	$3,1232($5)
	li	$2,-65536			# 0xffff0000
	ori	$2,$2,0x7fff
	and	$2,$3,$2
	or	$2,$2,$4
	sw	$2,1232($5)
	lw	$2,16($sp)
	lw	$3,652($2)
	lw	$2,1232($3)
	ori	$2,$2,0x4000
	sw	$2,1232($3)
	lw	$2,16($sp)
	lw	$4,652($2)
	jal	wapiInit
	nop

	lw	$2,16($sp)
	lw	$2,652($2)
	addiu	$3,$2,1308
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,100
	move	$4,$3
	move	$5,$2
	jal	mod_timer
	nop

	j	$L289
	nop

$L277:
	lw	$2,72($sp)
	lbu	$3,3($2)
	li	$2,9			# 0x9
	bne	$3,$2,$L289
	nop

	lw	$2,16($sp)
	lw	$2,352($2)
	andi	$2,$2,0x1000
	beq	$2,$0,$L291
	nop

	lw	$4,72($sp)
	lw	$5,76($sp)
	lw	$6,80($sp)
	jal	wapiRecvUnicastKeyAgreementResponse
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	beq	$2,$0,$L292
	nop

	lw	$2,20($sp)
	sw	$2,60($sp)
	j	$L276
	nop

$L292:
	lw	$2,16($sp)
	lw	$4,652($2)
	lw	$5,76($sp)
	jal	wapiSendUnicastKeyAgrementConfirm
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L289
	nop

	lw	$2,16($sp)
	addiu	$2,$2,600
	move	$4,$2
	jal	del_timer
	nop

	lw	$2,16($sp)
	addiu	$2,$2,600
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	nop

	lw	$3,16($sp)
	lw	$2,76($sp)
	sw	$2,616($3)
	lw	$3,16($sp)
	lui	$2,%hi(wapiResendTimeout)
	addiu	$2,$2,%lo(wapiResendTimeout)
	sw	$2,612($3)
	lw	$5,16($sp)
	lw	$2,16($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	sll	$4,$2,15
	lw	$3,352($5)
	li	$2,-65536			# 0xffff0000
	ori	$2,$2,0x7fff
	and	$2,$3,$2
	or	$2,$2,$4
	sw	$2,352($5)
	li	$2,16			# 0x10
	sw	$2,24($sp)
$L294:
	lw	$2,24($sp)
	blez	$2,$L295
	nop

	lw	$3,16($sp)
	lw	$2,24($sp)
	addu	$3,$3,$2
	li	$2,92
	sb	$2,367($3)
	lw	$3,16($sp)
	lw	$2,24($sp)
	addu	$3,$3,$2
	li	$2,54
	sb	$2,366($3)
	lw	$2,24($sp)
	addiu	$2,$2,-2
	sw	$2,24($sp)
	j	$L294
	nop

$L295:
	li	$2,1			# 0x1
	sw	$2,28($sp)
$L297:
	lw	$2,28($sp)
	slt	$2,$2,4
	beq	$2,$0,$L298
	nop

	lw	$2,28($sp)
	sll	$3,$2,4
	lw	$2,16($sp)
	addu	$2,$3,$2
	addiu	$3,$2,368
	lw	$2,16($sp)
	addiu	$2,$2,368
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
	j	$L297
	nop

$L298:
	lw	$2,16($sp)
	addiu	$3,$2,432
	lw	$2,16($sp)
	addiu	$2,$2,368
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	sw	$0,28($sp)
$L300:
	lw	$2,28($sp)
	slt	$2,$2,4
	beq	$2,$0,$L301
	nop

	lw	$3,16($sp)
	lw	$2,28($sp)
	sll	$2,$2,4
	addu	$3,$2,$3
	lw	$2,24($sp)
	addu	$3,$3,$2
	li	$2,55
	sb	$2,368($3)
	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
	j	$L300
	nop

$L301:
	lw	$3,16($sp)
	lw	$2,24($sp)
	addu	$3,$3,$2
	li	$2,55
	sb	$2,432($3)
	lw	$2,16($sp)
	addiu	$2,$2,448
	move	$4,$2
	move	$5,$0
	li	$6,8			# 0x8
	jal	memset
	nop

	lw	$4,16($sp)
	lw	$3,352($4)
	li	$2,-4097			# 0xffffefff
	and	$2,$3,$2
	sw	$2,352($4)
	j	$L289
	nop

$L291:
	lw	$2,72($sp)
	addiu	$2,$2,12
	sw	$2,52($sp)
	lw	$2,52($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x10
	andi	$2,$2,0x00ff
	beq	$2,$0,$L289
	nop

	lw	$3,52($sp)
	lw	$2,16($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	lbu	$3,17($3)
	andi	$2,$2,0x00ff
	beq	$3,$2,$L289
	nop

	lw	$2,16($sp)
	lw	$4,652($2)
	lw	$5,76($sp)
	jal	wapiUpdateUSK
	nop

$L289:
	lw	$2,20($sp)
	sw	$2,60($sp)
$L276:
	lw	$2,60($sp)
	lw	$31,64($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiRecvKeyUpdateResponse
	.section	.text.SecIsWAIPacket,"ax",@progbits
	.align	2
	.ent	SecIsWAIPacket
	.type	SecIsWAIPacket, @function
SecIsWAIPacket:
	.set	nomips16
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	sw	$5,20($sp)
	lw	$2,16($sp)
	lw	$2,176($2)
	lhu	$2,0($2)
	sh	$2,4($sp)
	lhu	$2,4($sp)
	andi	$2,$2,0xff
	sll	$3,$2,8
	lhu	$2,4($sp)
	andi	$2,$2,0xff00
	sra	$2,$2,8
	or	$2,$3,$2
	andi	$2,$2,0xffff
	sw	$2,8($sp)
	lw	$2,8($sp)
	andi	$2,$2,0xffff
	andi	$3,$2,0x88
	li	$2,136			# 0x88
	beq	$3,$2,$L308
	nop

	j	$L306
	nop

$L308:
	li	$2,32			# 0x20
	sw	$2,0($sp)
	j	$L310
	nop

$L306:
	li	$2,30			# 0x1e
	sw	$2,0($sp)
$L310:
	lw	$4,16($sp)
	lw	$2,0($sp)
	addiu	$3,$2,2
	lw	$2,84($4)
	sltu	$2,$2,$3
	beq	$2,$0,$L311
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,12($sp)
	j	$L305
	nop

$L311:
	lw	$2,16($sp)
	lw	$3,176($2)
	lw	$2,0($sp)
	addu	$2,$3,$2
	lhu	$3,0($2)
	li	$2,34996			# 0x88b4
	beq	$3,$2,$L312
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,12($sp)
	j	$L305
	nop

$L312:
	lw	$2,20($sp)
	lw	$3,0($sp)
	addiu	$3,$3,2
	sw	$3,0($2)
	li	$2,1			# 0x1
	sw	$2,12($sp)
$L305:
	lw	$2,12($sp)
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	SecIsWAIPacket
	.section	.text.wapiHandleRecvPacket,"ax",@progbits
	.align	2
	.globl	wapiHandleRecvPacket
	.ent	wapiHandleRecvPacket
	.type	wapiHandleRecvPacket, @function
wapiHandleRecvPacket:
	.set	nomips16
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-80
	sw	$31,72($sp)
	sw	$4,80($sp)
	sw	$5,84($sp)
	lw	$2,84($sp)
	lw	$2,6676($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L314
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,52($sp)
	j	$L313
	nop

$L314:
	lw	$2,20($sp)
	lw	$2,652($2)
	sw	$2,28($sp)
	lw	$2,20($sp)
	lw	$2,360($2)
	bne	$2,$0,$L315
	nop

	li	$3,-1			# 0xffffffff
	sw	$3,52($sp)
	j	$L313
	nop

$L315:
	lw	$2,80($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	addiu	$2,$sp,36
	lw	$4,16($sp)
	move	$5,$2
	jal	SecIsWAIPacket
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffff
	bne	$3,$2,$L316
	nop

	lw	$2,20($sp)
	lw	$3,364($2)
	li	$2,13			# 0xd
	bne	$3,$2,$L319
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,52($sp)
	j	$L313
	nop

$L316:
	lw	$2,16($sp)
	lw	$3,176($2)
	lw	$2,36($sp)
	addu	$2,$3,$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$3,20($sp)
	lhu	$4,8($2)
	lhu	$2,358($3)
	sltu	$2,$2,$4
	bne	$2,$0,$L320
	nop

	lw	$4,20($sp)
	jal	wapiReleaseFragementQueue
	nop

	j	$L319
	nop

$L320:
	lw	$2,24($sp)
	lhu	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L319
	nop

	lw	$2,24($sp)
	lbu	$3,2($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L319
	nop

	lw	$2,24($sp)
	lhu	$3,6($2)
	lw	$2,16($sp)
	lw	$2,84($2)
	sltu	$2,$2,$3
	bne	$2,$0,$L319
	nop

	lw	$2,24($sp)
	lbu	$2,11($2)
	andi	$2,$2,0x1
	bne	$2,$0,$L324
	nop

	lw	$2,24($sp)
	lbu	$2,10($2)
	bne	$2,$0,$L324
	nop

	j	$L323
	nop

$L324:
	lw	$4,16($sp)
	lw	$5,84($sp)
	lw	$6,36($sp)
	jal	wapiDefragement
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L325
	nop

	li	$3,1			# 0x1
	sw	$3,52($sp)
	j	$L313
	nop

$L325:
	lw	$2,16($sp)
	lw	$3,176($2)
	lw	$2,36($sp)
	addu	$2,$3,$2
	sw	$2,24($sp)
$L323:
	lw	$3,20($sp)
	lw	$2,24($sp)
	lhu	$2,8($2)
	sh	$2,358($3)
	lw	$2,28($sp)
	lw	$2,8($2)
	lw	$2,892($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L327
	nop

	lw	$2,20($sp)
	lw	$2,364($2)
	sw	$2,60($sp)
	li	$2,6			# 0x6
	lw	$3,60($sp)
	beq	$3,$2,$L332
	nop

	lw	$3,60($sp)
	sltu	$2,$3,7
	beq	$2,$0,$L350
	nop

	li	$2,2			# 0x2
	lw	$3,60($sp)
	beq	$3,$2,$L329
	nop

	j	$L319
	nop

$L350:
	li	$2,10			# 0xa
	lw	$3,60($sp)
	beq	$3,$2,$L337
	nop

	li	$2,13			# 0xd
	lw	$3,60($sp)
	beq	$3,$2,$L340
	nop

	j	$L319
	nop

$L329:
	addiu	$2,$sp,32
	lw	$4,24($sp)
	lw	$5,84($sp)
	move	$6,$2
	jal	WapiRecvAccessAuthenticateRequest
	nop

	j	$L319
	nop

$L332:
	addiu	$2,$sp,32
	lw	$4,24($sp)
	lw	$5,84($sp)
	move	$6,$2
	jal	wapiRecvUnicastKeyAgreementResponse
	nop

	sw	$2,56($sp)
	li	$2,-2			# 0xfffffffe
	lw	$3,56($sp)
	beq	$3,$2,$L336
	nop

	li	$2,-1			# 0xffffffff
	lw	$3,56($sp)
	beq	$3,$2,$L319
	nop

	lw	$4,28($sp)
	lw	$5,84($sp)
	jal	wapiSendUnicastKeyAgrementConfirm
	nop

	lw	$4,28($sp)
	lw	$5,84($sp)
	jal	wapiSendMulticastKeyNotification
	nop

	j	$L319
	nop

$L337:
	addiu	$2,$sp,32
	lw	$4,24($sp)
	lw	$5,84($sp)
	move	$6,$2
	jal	wapiRecvMulticastKeyResponse
	nop

	j	$L319
	nop

$L340:
	addiu	$2,$sp,32
	lw	$4,24($sp)
	lw	$5,84($sp)
	move	$6,$2
	jal	wapiRecvKeyUpdateResponse
	nop

	j	$L319
	nop

$L327:
	lw	$2,28($sp)
	lw	$2,8($2)
	lw	$2,892($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L336
	nop

	lw	$2,20($sp)
	lw	$2,364($2)
	sw	$2,64($sp)
	li	$2,7			# 0x7
	lw	$3,64($sp)
	beq	$3,$2,$L357
	nop

	lw	$3,64($sp)
	sltu	$2,$3,8
	beq	$2,$0,$L369
	nop

	lw	$2,64($sp)
	beq	$2,$0,$L354
	nop

	j	$L364
	nop

$L369:
	li	$2,9			# 0x9
	lw	$3,64($sp)
	beq	$3,$2,$L361
	nop

	j	$L364
	nop

$L354:
	addiu	$2,$sp,32
	lw	$4,24($sp)
	lw	$5,84($sp)
	move	$6,$2
	jal	WapiRecvUnicastKeyAggrementRequest
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffff
	beq	$3,$2,$L319
	nop

	lw	$4,28($sp)
	lw	$5,84($sp)
	jal	WapiSendUnicastKeyAggrementResponse
	nop

	j	$L319
	nop

$L357:
	addiu	$2,$sp,32
	lw	$4,24($sp)
	lw	$5,84($sp)
	move	$6,$2
	jal	WapiRecvUnicastKeyAggrementConfirm
	nop

	move	$3,$2
	li	$2,-2			# 0xfffffffe
	beq	$3,$2,$L336
	nop

	j	$L319
	nop

$L361:
	addiu	$2,$sp,32
	lw	$4,24($sp)
	lw	$5,84($sp)
	move	$6,$2
	jal	WapiRecvMulticastKeyNotification
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffff
	beq	$3,$2,$L319
	nop

	lw	$4,28($sp)
	lw	$5,84($sp)
	jal	WapiSendMulticastKeyResponse
	nop

	j	$L319
	nop

$L364:
	lw	$2,24($sp)
	lbu	$3,3($2)
	li	$2,11			# 0xb
	beq	$3,$2,$L366
	nop

	j	$L319
	nop

$L366:
	addiu	$2,$sp,32
	lw	$4,24($sp)
	lw	$5,84($sp)
	move	$6,$2
	jal	WapiRecvMulticastKeyNotification
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffff
	beq	$3,$2,$L319
	nop

	lw	$4,28($sp)
	lw	$5,84($sp)
	jal	WapiSendMulticastKeyResponse
	nop

	j	$L319
	nop

$L336:
	lw	$4,28($sp)
	lw	$5,84($sp)
	lw	$6,32($sp)
	jal	wapiDeauthSta
	nop

$L319:
	lw	$2,28($sp)
	sw	$2,40($sp)
	lw	$2,16($sp)
	sw	$2,44($sp)
	li	$2,2			# 0x2
	sw	$2,48($sp)
	lw	$4,44($sp)
	jal	dev_kfree_skb_any
	nop

	li	$2,1			# 0x1
	sw	$2,52($sp)
$L313:
	lw	$2,52($sp)
	lw	$31,72($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiHandleRecvPacket
	.section	.text.wapiIEInfoInstall,"ax",@progbits
	.align	2
	.globl	wapiIEInfoInstall
	.ent	wapiIEInfoInstall
	.type	wapiIEInfoInstall, @function
wapiIEInfoInstall:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,48($sp)
	sw	$4,56($sp)
	sw	$5,60($sp)
	lw	$2,60($sp)
	lw	$2,6676($2)
	addiu	$2,$2,4
	sw	$2,20($sp)
	lw	$2,20($sp)
	lhu	$2,0($2)
	sh	$2,28($sp)
	lhu	$2,28($sp)
	andi	$2,$2,0xff
	sll	$3,$2,8
	lhu	$2,28($sp)
	andi	$2,$2,0xff00
	sra	$2,$2,8
	or	$2,$3,$2
	andi	$2,$2,0xffff
	sw	$2,32($sp)
	lw	$2,32($sp)
	andi	$2,$2,0xffff
	sw	$2,16($sp)
	lw	$2,20($sp)
	addiu	$2,$2,2
	sw	$2,20($sp)
	lw	$4,20($sp)
	lui	$2,%hi(WAPI_KM_OUI)
	addiu	$5,$2,%lo(WAPI_KM_OUI)
	li	$6,3			# 0x3
	jal	memcmp
	bne	$2,$0,$L375
	lw	$2,20($sp)
	lbu	$3,3($2)
	lw	$2,56($sp)
	lw	$2,8($2)
	lw	$2,5192($2)
	and	$2,$3,$2
	beq	$2,$0,$L375
	lw	$2,60($sp)
	lw	$3,6676($2)
	lw	$2,20($sp)
	lbu	$2,3($2)
	sw	$2,360($3)
	j	$L376
$L375:
	li	$2,31			# 0x1f
	sw	$2,44($sp)
	j	$L371
$L376:
	lw	$2,20($sp)
	addiu	$2,$2,4
	sw	$2,20($sp)
	lw	$2,20($sp)
	lhu	$2,0($2)
	sh	$2,28($sp)
	lhu	$2,28($sp)
	andi	$2,$2,0xff
	sll	$3,$2,8
	lhu	$2,28($sp)
	andi	$2,$2,0xff00
	sra	$2,$2,8
	or	$2,$3,$2
	andi	$2,$2,0xffff
	sw	$2,32($sp)
	lw	$2,32($sp)
	andi	$2,$2,0xffff
	sw	$2,16($sp)
	lw	$2,20($sp)
	addiu	$2,$2,2
	sw	$2,20($sp)
	lw	$4,20($sp)
	lui	$2,%hi(WAPI_KM_OUI)
	addiu	$5,$2,%lo(WAPI_KM_OUI)
	li	$6,3			# 0x3
	jal	memcmp
	bne	$2,$0,$L380
	lw	$2,20($sp)
	lbu	$3,3($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L380
	j	$L381
$L380:
	li	$2,28			# 0x1c
	sw	$2,44($sp)
	j	$L371
$L381:
	lw	$2,20($sp)
	addiu	$2,$2,4
	sw	$2,20($sp)
	lw	$4,20($sp)
	lui	$2,%hi(WAPI_KM_OUI)
	addiu	$5,$2,%lo(WAPI_KM_OUI)
	li	$6,3			# 0x3
	jal	memcmp
	bne	$2,$0,$L382
	lw	$2,20($sp)
	lbu	$3,3($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L382
	j	$L383
$L382:
	li	$2,29			# 0x1d
	sw	$2,44($sp)
	j	$L371
$L383:
#APP
	raw_local_irq_save	$2
#NO_APP
	sw	$2,24($sp)
	lw	$3,56($sp)
	lw	$2,56($sp)
	addiu	$2,$2,978
	sw	$2,908($3)
	lw	$4,56($sp)
	jal	wapiSetIE
	lw	$3,56($sp)
	li	$2,68
	sb	$2,976($3)
	lw	$3,56($sp)
	lw	$2,56($sp)
	lbu	$2,1232($2)
	sb	$2,977($3)
	lw	$3,56($sp)
	lw	$2,56($sp)
	lbu	$2,1232($2)
	addiu	$2,$2,2
	sb	$2,1233($3)
	lw	$2,24($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sw	$2,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L392
	lw	$2,24($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
#APP
	raw_local_irq_restore	$2
#NO_APP
	sw	$2,40($sp)
	j	$L389
$L392:
	lw	$2,24($sp)
	sw	$2,40($sp)
	lw	$2,40($sp)
#APP
	raw_local_irq_restore	$2
#NO_APP
	sw	$2,36($sp)
$L389:
	sw	$0,44($sp)
$L371:
	lw	$2,44($sp)
	lw	$31,48($sp)
	addiu	$sp,$sp,56
	j	$31
	.end	wapiIEInfoInstall
	.local	data.4
	.comm	data.4,64,4
	.section	.text.wapiReqActiveCA,"ax",@progbits
	.align	2
	.globl	wapiReqActiveCA
	.ent	wapiReqActiveCA
	.type	wapiReqActiveCA, @function
wapiReqActiveCA:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,32($sp)
	sw	$4,40($sp)
	lw	$2,40($sp)
	lw	$2,6676($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,652($2)
	sw	$2,24($sp)
	lui	$2,%hi(data.4)
	addiu	$2,$2,%lo(data.4)
	sw	$2,16($sp)
	lui	$2,%hi(data.4)
	addiu	$4,$2,%lo(data.4)
	move	$5,$0
	li	$6,64			# 0x40
	jal	memset
	nop

	lw	$3,16($sp)
	li	$2,3
	sh	$2,2($3)
	lw	$3,16($sp)
	lw	$2,40($sp)
	sw	$2,4($3)
	lw	$2,16($sp)
	addiu	$2,$2,8
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	jal	memset
	nop

	lw	$2,24($sp)
	lw	$4,4($2)
	jal	strlen
	nop

	move	$6,$2
	lw	$2,16($sp)
	addiu	$2,$2,8
	lw	$3,24($sp)
	move	$4,$2
	lw	$5,4($3)
	jal	memcpy
	nop

	lw	$2,16($sp)
	addiu	$3,$2,24
	lw	$2,24($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,16($sp)
	addiu	$3,$2,30
	lw	$2,40($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$3,20($sp)
	lw	$2,20($sp)
	lw	$2,652($2)
	lw	$4,652($3)
	lw	$5,36($2)
	lw	$6,16($sp)
	li	$7,36			# 0x24
	jal	DOT11_EnQueue
	nop

	lw	$2,20($sp)
	lw	$4,652($2)
	jal	wapi_event_indicate
	nop

	lw	$2,20($sp)
	lw	$2,364($2)
	bne	$2,$0,$L400
	nop

	lw	$3,20($sp)
	li	$2,1			# 0x1
	sw	$2,364($3)
$L400:
	lw	$2,20($sp)
	addiu	$3,$2,600
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,1000
	move	$4,$3
	move	$5,$2
	jal	mod_timer
	nop

	lw	$31,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiReqActiveCA
	.rdata
	.align	2
$LC3:
	.ascii	"preshared key expansion for authentication and key negot"
	.ascii	"iation\000"
	.align	2
$LC4:
	.ascii	"base key expansion for key and additional nonce\000"
	.section	.text.wapiSetBK,"ax",@progbits
	.align	2
	.globl	wapiSetBK
	.ent	wapiSetBK
	.type	wapiSetBK, @function
wapiSetBK:
	.set	nomips16
	.frame	$sp,200,$31		# vars= 168, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-200
	sw	$31,192($sp)
	sw	$4,200($sp)
	lui	$2,%hi($LC3)
	addiu	$2,$2,%lo($LC3)
	sw	$2,48($sp)
	lui	$2,%hi($LC4)
	addiu	$2,$2,%lo($LC4)
	sw	$2,52($sp)
	lw	$2,200($sp)
	lw	$2,6676($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,652($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$3,360($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L402
	nop

	lw	$4,48($sp)
	jal	strlen
	nop

	move	$7,$2
	lw	$2,28($sp)
	lw	$2,8($2)
	addiu	$3,$2,5204
	lw	$2,28($sp)
	lw	$5,8($2)
	lw	$2,24($sp)
	addiu	$2,$2,456
	sw	$2,16($sp)
	li	$2,16			# 0x10
	sw	$2,20($sp)
	move	$4,$3
	lw	$5,5236($5)
	lw	$6,48($sp)
	jal	KD_hmac_sha256
	nop

	j	$L403
	nop

$L402:
	lw	$2,24($sp)
	lw	$3,360($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L403
	nop

	li	$2,64
	sb	$2,184($sp)
	addiu	$4,$sp,56
	lw	$2,28($sp)
	lw	$2,908($2)
	addiu	$2,$2,24
	lbu	$3,184($sp)
	move	$5,$2
	move	$6,$3
	jal	memcpy
	nop

	lw	$4,52($sp)
	jal	strlen
	nop

	move	$6,$2
	addiu	$3,$sp,56
	lbu	$2,184($sp)
	addu	$2,$3,$2
	move	$4,$2
	lw	$5,52($sp)
	jal	memcpy
	nop

	lw	$4,52($sp)
	jal	strlen
	nop

	lbu	$3,184($sp)
	addu	$2,$3,$2
	sb	$2,184($sp)
	lw	$3,28($sp)
	addiu	$6,$sp,56
	lbu	$7,184($sp)
	lw	$2,24($sp)
	addiu	$2,$2,456
	sw	$2,16($sp)
	li	$2,16			# 0x10
	sw	$2,20($sp)
	lw	$4,908($3)
	li	$5,24			# 0x18
	jal	KD_hmac_sha256
	nop

$L403:
	lw	$2,28($sp)
	lw	$2,8($2)
	lw	$2,892($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L405
	nop

	addiu	$2,$sp,32
	addiu	$3,$2,6
	lw	$2,28($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	addiu	$3,$sp,32
	lw	$2,200($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	j	$L406
	nop

$L405:
	lw	$2,28($sp)
	lw	$2,8($2)
	lw	$2,892($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L406
	nop

	addiu	$3,$sp,32
	lw	$2,28($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	addiu	$2,$sp,32
	addiu	$3,$2,6
	lw	$2,200($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,24($sp)
	addiu	$2,$2,288
	move	$4,$2
	li	$5,32			# 0x20
	jal	GenerateRandomData
	nop

$L406:
	lw	$2,24($sp)
	addiu	$3,$2,456
	addiu	$6,$sp,32
	lw	$2,24($sp)
	addiu	$2,$2,472
	sw	$2,16($sp)
	li	$2,16			# 0x10
	sw	$2,20($sp)
	move	$4,$3
	li	$5,16			# 0x10
	li	$7,12			# 0xc
	jal	KD_hmac_sha256
	nop

	lw	$3,24($sp)
	li	$2,5			# 0x5
	sw	$2,364($3)
	lw	$31,192($sp)
	addiu	$sp,$sp,200
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiSetBK
	.section	.text.wapiSetBKByCA,"ax",@progbits
	.align	2
	.globl	wapiSetBKByCA
	.ent	wapiSetBKByCA
	.type	wapiSetBKByCA, @function
wapiSetBKByCA:
	.set	nomips16
	.frame	$sp,64,$31		# vars= 32, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	sw	$31,56($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	lui	$2,%hi($LC3)
	addiu	$2,$2,%lo($LC3)
	sw	$2,48($sp)
	lw	$2,64($sp)
	lw	$2,6676($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,652($2)
	sw	$2,28($sp)
	lw	$4,48($sp)
	jal	strlen
	nop

	move	$7,$2
	lw	$2,28($sp)
	lw	$2,8($2)
	addiu	$3,$2,5204
	lw	$2,28($sp)
	lw	$5,8($2)
	lw	$2,24($sp)
	addiu	$2,$2,456
	sw	$2,16($sp)
	li	$2,16			# 0x10
	sw	$2,20($sp)
	move	$4,$3
	lw	$5,5236($5)
	lw	$6,48($sp)
	jal	KD_hmac_sha256
	nop

	addiu	$3,$sp,32
	lw	$2,28($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	addiu	$2,$sp,32
	addiu	$3,$2,6
	lw	$2,64($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,24($sp)
	addiu	$3,$2,456
	addiu	$6,$sp,32
	lw	$2,24($sp)
	addiu	$2,$2,472
	sw	$2,16($sp)
	li	$2,16			# 0x10
	sw	$2,20($sp)
	move	$4,$3
	li	$5,16			# 0x10
	li	$7,12			# 0xc
	jal	KD_hmac_sha256
	nop

	lw	$2,24($sp)
	addiu	$2,$2,288
	move	$4,$2
	li	$5,32			# 0x20
	jal	GenerateRandomData
	nop

	lw	$2,64($sp)
	lw	$3,6676($2)
	li	$2,5			# 0x5
	sw	$2,364($3)
	lw	$31,56($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiSetBKByCA
	.section	.text.WapiSendUnicastKeyAggrementResponse,"ax",@progbits
	.align	2
	.ent	WapiSendUnicastKeyAggrementResponse
	.type	WapiSendUnicastKeyAggrementResponse, @function
WapiSendUnicastKeyAggrementResponse:
	.set	nomips16
	.frame	$sp,80,$31		# vars= 48, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-80
	sw	$31,72($sp)
	sw	$4,80($sp)
	sw	$5,84($sp)
	lw	$2,84($sp)
	lw	$2,6676($2)
	sw	$2,28($sp)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,100
	sw	$2,40($sp)
	lw	$2,80($sp)
	sw	$2,44($sp)
	li	$2,1560			# 0x618
	sw	$2,48($sp)
	li	$2,1			# 0x1
	sw	$2,52($sp)
	li	$2,1			# 0x1
	sw	$2,56($sp)
	sw	$0,64($sp)
	lw	$4,44($sp)
	jal	alloc_skb_from_queue
	nop

	sw	$2,64($sp)
	lw	$2,64($sp)
	bne	$2,$0,$L410
	nop

	lw	$2,56($sp)
	beq	$2,$0,$L410
	nop

	lw	$4,48($sp)
	jal	dev_alloc_skb
	nop

	sw	$2,64($sp)
$L410:
	lw	$2,64($sp)
	sw	$2,60($sp)
	lw	$2,60($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L412
	nop

	j	$L413
	nop

$L412:
	lw	$3,24($sp)
	li	$2,-30540
	sh	$2,106($3)
	lw	$3,24($sp)
	lw	$2,84($sp)
	addiu	$2,$2,6070
	lw	$4,176($3)
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,24($sp)
	lw	$2,176($2)
	addiu	$3,$2,6
	lw	$2,80($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,24($sp)
	lw	$3,176($2)
	li	$2,-30540
	sh	$2,12($3)
	lw	$3,24($sp)
	lw	$2,80($sp)
	lw	$2,4($2)
	sw	$2,24($3)
	lw	$4,24($sp)
	li	$5,14			# 0xe
	jal	skb_put
	nop

	lw	$2,24($sp)
	lw	$3,24($sp)
	lw	$4,176($2)
	lw	$2,84($3)
	addu	$2,$4,$2
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,64($sp)
	li	$2,9
	sb	$2,68($sp)
	lw	$3,64($sp)
	li	$2,1
	sh	$2,0($3)
	lw	$3,64($sp)
	li	$2,1
	sb	$2,2($3)
	lw	$3,64($sp)
	lbu	$2,68($sp)
	sb	$2,3($3)
	lw	$2,64($sp)
	sh	$0,4($2)
	lw	$3,64($sp)
	li	$2,12
	sh	$2,6($3)
	lw	$2,64($sp)
	sb	$0,10($2)
	lw	$2,64($sp)
	sb	$0,11($2)
	lw	$4,32($sp)
	lw	$3,28($sp)
	lw	$2,28($sp)
	lhu	$2,356($2)
	addiu	$2,$2,1
	sh	$2,356($3)
	sh	$2,8($4)
	lw	$2,32($sp)
	lhu	$3,6($2)
	lw	$2,32($sp)
	addu	$2,$3,$2
	sw	$2,36($sp)
	lw	$5,36($sp)
	lw	$4,36($sp)
	lbu	$3,0($4)
	li	$2,-16			# 0xfffffff0
	and	$2,$3,$2
	sb	$2,0($4)
	lbu	$2,0($5)
	andi	$2,$2,0x1f
	sb	$2,0($5)
	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,5			# 0x5
	beq	$3,$2,$L416
	nop

	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,7			# 0x7
	beq	$3,$2,$L416
	nop

	j	$L415
	nop

$L416:
	lw	$4,36($sp)
	lbu	$3,0($4)
	li	$2,-17			# 0xffffffef
	and	$2,$3,$2
	sb	$2,0($4)
	lw	$3,36($sp)
	lw	$2,28($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	sb	$2,17($3)
	lw	$3,28($sp)
	lw	$4,352($3)
	li	$2,-65536			# 0xffff0000
	ori	$2,$2,0x7fff
	and	$2,$4,$2
	sw	$2,352($3)
	lw	$4,28($sp)
	lw	$3,352($4)
	li	$2,-4097			# 0xffffefff
	and	$2,$3,$2
	sw	$2,352($4)
	j	$L417
	nop

$L415:
	lw	$3,36($sp)
	lbu	$2,0($3)
	ori	$2,$2,0x10
	sb	$2,0($3)
	lw	$3,36($sp)
	lw	$2,28($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sb	$2,17($3)
$L417:
	lw	$2,36($sp)
	addiu	$3,$2,1
	lw	$2,28($sp)
	addiu	$2,$2,472
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$3,$2,24
	lw	$2,80($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$3,$2,18
	lw	$2,84($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$3,$2,30
	lw	$2,28($sp)
	addiu	$2,$2,256
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$3,$2,62
	lw	$2,28($sp)
	addiu	$2,$2,288
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$3,$2,94
	lw	$2,28($sp)
	lbu	$2,352($2)
	move	$4,$3
	lw	$5,28($sp)
	move	$6,$2
	jal	memcpy
	nop

	lw	$2,28($sp)
	addiu	$4,$2,568
	lw	$2,28($sp)
	lbu	$2,352($2)
	addiu	$7,$2,94
	lw	$2,28($sp)
	lbu	$3,352($2)
	lw	$2,36($sp)
	addu	$2,$3,$2
	addiu	$2,$2,94
	sw	$2,16($sp)
	li	$2,20			# 0x14
	sw	$2,20($sp)
	li	$5,16			# 0x10
	lw	$6,36($sp)
	jal	sha256_hmac
	nop

	lw	$5,32($sp)
	lw	$4,32($sp)
	lw	$2,28($sp)
	lbu	$3,352($2)
	lhu	$2,6($4)
	addu	$2,$3,$2
	addiu	$2,$2,114
	sh	$2,6($5)
	lw	$2,32($sp)
	lhu	$2,6($2)
	lw	$4,24($sp)
	move	$5,$2
	jal	skb_put
	nop

	lw	$2,24($sp)
	lw	$2,84($2)
	addiu	$3,$2,-14
	lw	$2,80($sp)
	lw	$2,4($2)
	lw	$2,208($2)
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L418
	nop

	lw	$4,24($sp)
	lw	$5,80($sp)
	jal	wapiFragementSend
	nop

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L421
	nop

	lw	$2,80($sp)
	sw	$2,64($sp)
	lw	$2,24($sp)
	sw	$2,60($sp)
	li	$2,1			# 0x1
	sw	$2,56($sp)
	lw	$4,60($sp)
	jal	dev_kfree_skb_any
	nop

	j	$L421
	nop

$L418:
	lw	$2,80($sp)
	lw	$4,24($sp)
	lw	$5,4($2)
	jal	rtl8192cd_start_xmit
	nop

	beq	$2,$0,$L421
	nop

	lw	$2,80($sp)
	sw	$2,64($sp)
	lw	$2,24($sp)
	sw	$2,60($sp)
	li	$2,1			# 0x1
	sw	$2,56($sp)
	lw	$4,60($sp)
	jal	dev_kfree_skb_any
	nop

$L421:
	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L413
	nop

	lw	$3,28($sp)
	li	$2,7			# 0x7
	sw	$2,364($3)
$L413:
	lw	$2,28($sp)
	addiu	$2,$2,600
	move	$4,$2
	lw	$5,40($sp)
	jal	mod_timer
	nop

	move	$2,$0
	lw	$31,72($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	WapiSendUnicastKeyAggrementResponse
	.section	.text.WapiSendMulticastKeyResponse,"ax",@progbits
	.align	2
	.ent	WapiSendMulticastKeyResponse
	.type	WapiSendMulticastKeyResponse, @function
WapiSendMulticastKeyResponse:
	.set	nomips16
	.frame	$sp,88,$31		# vars= 56, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-88
	sw	$31,80($sp)
	sw	$4,88($sp)
	sw	$5,92($sp)
	lw	$2,92($sp)
	lw	$2,6676($2)
	sw	$2,28($sp)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,100
	sw	$2,40($sp)
	lw	$2,88($sp)
	sw	$2,44($sp)
	li	$2,1560			# 0x618
	sw	$2,48($sp)
	li	$2,1			# 0x1
	sw	$2,52($sp)
	li	$2,1			# 0x1
	sw	$2,56($sp)
	sw	$0,64($sp)
	lw	$4,44($sp)
	jal	alloc_skb_from_queue
	nop

	sw	$2,64($sp)
	lw	$2,64($sp)
	bne	$2,$0,$L426
	nop

	lw	$2,56($sp)
	beq	$2,$0,$L426
	nop

	lw	$4,48($sp)
	jal	dev_alloc_skb
	nop

	sw	$2,64($sp)
$L426:
	lw	$2,64($sp)
	sw	$2,60($sp)
	lw	$2,60($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L428
	nop

	j	$L429
	nop

$L428:
	lw	$3,24($sp)
	li	$2,-30540
	sh	$2,106($3)
	lw	$3,24($sp)
	lw	$2,92($sp)
	addiu	$2,$2,6070
	lw	$4,176($3)
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,24($sp)
	lw	$2,176($2)
	addiu	$3,$2,6
	lw	$2,88($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,24($sp)
	lw	$3,176($2)
	li	$2,-30540
	sh	$2,12($3)
	lw	$4,24($sp)
	li	$5,14			# 0xe
	jal	skb_put
	nop

	lw	$3,24($sp)
	lw	$2,88($sp)
	lw	$2,4($2)
	sw	$2,24($3)
	lw	$2,24($sp)
	lw	$3,24($sp)
	lw	$4,176($2)
	lw	$2,84($3)
	addu	$2,$4,$2
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,64($sp)
	li	$2,12
	sb	$2,68($sp)
	lw	$3,64($sp)
	li	$2,1
	sh	$2,0($3)
	lw	$3,64($sp)
	li	$2,1
	sb	$2,2($3)
	lw	$3,64($sp)
	lbu	$2,68($sp)
	sb	$2,3($3)
	lw	$2,64($sp)
	sh	$0,4($2)
	lw	$3,64($sp)
	li	$2,12
	sh	$2,6($3)
	lw	$2,64($sp)
	sb	$0,10($2)
	lw	$2,64($sp)
	sb	$0,11($2)
	lw	$4,32($sp)
	lw	$3,28($sp)
	lw	$2,28($sp)
	lhu	$2,356($2)
	addiu	$2,$2,1
	sh	$2,356($3)
	sh	$2,8($4)
	lw	$2,32($sp)
	lhu	$3,6($2)
	lw	$2,32($sp)
	addu	$2,$3,$2
	sw	$2,36($sp)
	lw	$5,36($sp)
	lw	$4,36($sp)
	lbu	$3,0($4)
	li	$2,-32			# 0xffffffe0
	and	$2,$3,$2
	sb	$2,0($4)
	lbu	$2,0($5)
	andi	$2,$2,0x7f
	sb	$2,0($5)
	lw	$5,36($sp)
	lw	$4,36($sp)
	lbu	$3,0($4)
	li	$2,-33			# 0xffffffdf
	and	$2,$3,$2
	sb	$2,0($4)
	lbu	$3,0($5)
	li	$2,-65			# 0xffffffbf
	and	$2,$3,$2
	sb	$2,0($5)
	lw	$3,36($sp)
	lw	$2,88($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	sb	$2,1($3)
	lw	$3,36($sp)
	lw	$2,28($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	sb	$2,2($3)
	lw	$2,36($sp)
	addiu	$3,$2,9
	lw	$2,88($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$3,$2,3
	lw	$2,92($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$3,$2,15
	lw	$2,88($sp)
	addiu	$2,$2,960
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,28($sp)
	addiu	$3,$2,568
	lw	$2,36($sp)
	addiu	$2,$2,31
	sw	$2,16($sp)
	li	$2,20			# 0x14
	sw	$2,20($sp)
	move	$4,$3
	li	$5,16			# 0x10
	lw	$6,36($sp)
	li	$7,31			# 0x1f
	jal	sha256_hmac
	nop

	lw	$3,32($sp)
	lw	$2,32($sp)
	lhu	$2,6($2)
	addiu	$2,$2,51
	sh	$2,6($3)
	lw	$2,32($sp)
	lhu	$2,6($2)
	lw	$4,24($sp)
	move	$5,$2
	jal	skb_put
	nop

	lw	$2,24($sp)
	lw	$2,84($2)
	addiu	$3,$2,-14
	lw	$2,88($sp)
	lw	$2,4($2)
	lw	$2,208($2)
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L431
	nop

	lw	$4,24($sp)
	lw	$5,88($sp)
	jal	wapiFragementSend
	nop

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L434
	nop

	lw	$2,88($sp)
	sw	$2,64($sp)
	lw	$2,24($sp)
	sw	$2,60($sp)
	li	$2,1			# 0x1
	sw	$2,56($sp)
	lw	$4,60($sp)
	jal	dev_kfree_skb_any
	nop

	j	$L434
	nop

$L431:
	lw	$2,88($sp)
	lw	$4,24($sp)
	lw	$5,4($2)
	jal	rtl8192cd_start_xmit
	nop

	beq	$2,$0,$L434
	nop

	lw	$2,88($sp)
	sw	$2,64($sp)
	lw	$2,24($sp)
	sw	$2,60($sp)
	li	$2,1			# 0x1
	sw	$2,56($sp)
	lw	$4,60($sp)
	jal	dev_kfree_skb_any
	nop

$L434:
	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,11			# 0xb
	bne	$3,$2,$L437
	nop

	lw	$3,28($sp)
	li	$2,13			# 0xd
	sw	$2,364($3)
$L437:
	lw	$2,28($sp)
	addiu	$2,$2,600
	move	$4,$2
	jal	del_timer
	nop

	lw	$2,28($sp)
	addiu	$2,$2,600
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	nop

	lw	$3,28($sp)
	lw	$2,92($sp)
	sw	$2,616($3)
	lw	$3,28($sp)
	lui	$2,%hi(wapiResendTimeout)
	addiu	$2,$2,%lo(wapiResendTimeout)
	sw	$2,612($3)
	lw	$4,28($sp)
	lw	$3,352($4)
	li	$2,-253			# 0xffffff03
	and	$2,$3,$2
	sw	$2,352($4)
	sw	$0,72($sp)
	j	$L425
	nop

$L429:
	lw	$2,28($sp)
	addiu	$2,$2,600
	move	$4,$2
	lw	$5,40($sp)
	jal	mod_timer
	nop

	sw	$0,72($sp)
$L425:
	lw	$2,72($sp)
	lw	$31,80($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	WapiSendMulticastKeyResponse
	.section	.text.WapiSendActivateAuthenticationPacket,"ax",@progbits
	.align	2
	.ent	WapiSendActivateAuthenticationPacket
	.type	WapiSendActivateAuthenticationPacket, @function
WapiSendActivateAuthenticationPacket:
	.set	nomips16
	.frame	$sp,120,$31		# vars= 96, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-120
	sw	$31,116($sp)
	sw	$16,112($sp)
	sw	$4,120($sp)
	sw	$5,124($sp)
	sw	$6,128($sp)
	sw	$7,132($sp)
	lw	$2,124($sp)
	lw	$2,6676($2)
	sw	$2,20($sp)
	lw	$2,132($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	lhu	$2,2($2)
	addiu	$2,$2,4
	sw	$2,44($sp)
	lw	$2,36($sp)
	lhu	$3,2($2)
	lw	$2,36($sp)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,36($sp)
	lw	$2,36($sp)
	lhu	$3,2($2)
	lw	$2,44($sp)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,44($sp)
	lw	$2,36($sp)
	lhu	$3,2($2)
	lw	$2,36($sp)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,40($sp)
	lw	$3,40($sp)
	lbu	$2,1($3)
	sll	$4,$2,8
	lbu	$2,2($3)
	or	$2,$2,$4
	andi	$3,$2,0xffff
	lw	$2,44($sp)
	addu	$2,$3,$2
	addiu	$2,$2,3
	sw	$2,44($sp)
	lw	$3,44($sp)
	lw	$2,128($sp)
	beq	$3,$2,$L439
	nop

	li	$2,-1			# 0xffffffff
	sw	$2,108($sp)
	j	$L438
	nop

$L439:
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,100
	sw	$2,32($sp)
	lw	$2,120($sp)
	sw	$2,48($sp)
	li	$2,1560			# 0x618
	sw	$2,52($sp)
	li	$2,1			# 0x1
	sw	$2,56($sp)
	li	$2,1			# 0x1
	sw	$2,60($sp)
	sw	$0,68($sp)
	lw	$4,48($sp)
	jal	alloc_skb_from_queue
	nop

	sw	$2,68($sp)
	lw	$2,68($sp)
	bne	$2,$0,$L440
	nop

	lw	$2,60($sp)
	beq	$2,$0,$L440
	nop

	lw	$4,52($sp)
	jal	dev_alloc_skb
	nop

	sw	$2,68($sp)
$L440:
	lw	$2,68($sp)
	sw	$2,64($sp)
	lw	$2,64($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L442
	nop

	j	$L443
	nop

$L442:
	lw	$2,20($sp)
	lw	$2,656($2)
	bne	$2,$0,$L444
	nop

	lw	$16,20($sp)
	li	$2,2688			# 0xa80
	sw	$2,68($sp)
	li	$2,32			# 0x20
	sw	$2,64($sp)
$L449:
$L490:
	lw	$4,68($sp)
	lw	$5,64($sp)
	jal	__kmalloc
	nop

	sw	$2,60($sp)
	lw	$2,60($sp)
	sw	$2,656($16)
	lw	$2,20($sp)
	lw	$2,656($2)
	bne	$2,$0,$L444
	nop

	lw	$2,120($sp)
	sw	$2,100($sp)
	lw	$2,16($sp)
	sw	$2,96($sp)
	li	$2,1			# 0x1
	sw	$2,92($sp)
	lw	$4,96($sp)
	jal	dev_kfree_skb_any
	nop

	j	$L443
	nop

$L444:
	lw	$3,16($sp)
	li	$2,-30540
	sh	$2,106($3)
	lw	$3,16($sp)
	lw	$2,124($sp)
	addiu	$2,$2,6070
	lw	$4,176($3)
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,16($sp)
	lw	$2,176($2)
	addiu	$3,$2,6
	lw	$2,120($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,16($sp)
	lw	$3,176($2)
	li	$2,-30540
	sh	$2,12($3)
	lw	$3,16($sp)
	lw	$2,120($sp)
	lw	$2,4($2)
	sw	$2,24($3)
	lw	$4,16($sp)
	li	$5,14			# 0xe
	jal	skb_put
	nop

	lw	$2,16($sp)
	lw	$3,16($sp)
	lw	$4,176($2)
	lw	$2,84($3)
	addu	$2,$4,$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,100($sp)
	li	$2,3
	sb	$2,104($sp)
	lw	$3,100($sp)
	li	$2,1
	sh	$2,0($3)
	lw	$3,100($sp)
	li	$2,1
	sb	$2,2($3)
	lw	$3,100($sp)
	lbu	$2,104($sp)
	sb	$2,3($3)
	lw	$2,100($sp)
	sh	$0,4($2)
	lw	$3,100($sp)
	li	$2,12
	sh	$2,6($3)
	lw	$2,100($sp)
	sb	$0,10($2)
	lw	$2,100($sp)
	sb	$0,11($2)
	lw	$4,24($sp)
	lw	$3,20($sp)
	lw	$2,20($sp)
	lhu	$2,356($2)
	addiu	$2,$2,1
	sh	$2,356($3)
	sh	$2,8($4)
	lw	$2,24($sp)
	lhu	$3,6($2)
	lw	$2,24($sp)
	addu	$2,$3,$2
	sw	$2,28($sp)
	lw	$3,28($sp)
	lbu	$2,0($3)
	andi	$2,$2,0x3
	sb	$2,0($3)
	lw	$4,28($sp)
	lbu	$3,0($4)
	li	$2,-3			# 0xfffffffd
	and	$2,$3,$2
	sb	$2,0($4)
	lw	$2,20($sp)
	lw	$3,364($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L497
	nop

	lw	$2,20($sp)
	lw	$3,364($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L497
	nop

	j	$L496
	nop

$L497:
	lw	$4,28($sp)
	lbu	$3,0($4)
	li	$2,-2			# 0xfffffffe
	and	$2,$3,$2
	sb	$2,0($4)
	lw	$2,20($sp)
	addiu	$2,$2,320
	move	$4,$2
	li	$5,32			# 0x20
	jal	GenerateRandomData
	nop

	j	$L498
	nop

$L496:
	lw	$2,28($sp)
	lbu	$3,0($2)
	ori	$3,$3,0x1
	sb	$3,0($2)
$L498:
	lw	$2,28($sp)
	addiu	$3,$2,1
	lw	$2,20($sp)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcpy
	nop

	lw	$2,28($sp)
	addiu	$2,$2,33
	move	$4,$2
	lw	$5,132($sp)
	lw	$6,128($sp)
	jal	memcpy
	nop

	lw	$5,24($sp)
	lw	$2,24($sp)
	lw	$3,128($sp)
	lhu	$4,6($2)
	move	$2,$3
	addu	$2,$4,$2
	addiu	$2,$2,33
	sh	$2,6($5)
	lw	$2,24($sp)
	lhu	$2,6($2)
	lw	$4,16($sp)
	move	$5,$2
	jal	skb_put
	nop

	lw	$2,16($sp)
	lw	$2,84($2)
	addiu	$3,$2,-14
	lw	$2,120($sp)
	lw	$2,4($2)
	lw	$2,208($2)
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L499
	nop

	lw	$4,16($sp)
	lw	$5,120($sp)
	jal	wapiFragementSend
	nop

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L502
	nop

	lw	$2,120($sp)
	sw	$2,100($sp)
	lw	$2,16($sp)
	sw	$2,96($sp)
	li	$2,1			# 0x1
	sw	$2,92($sp)
	lw	$4,96($sp)
	jal	dev_kfree_skb_any
	nop

	j	$L502
	nop

$L499:
	lw	$2,120($sp)
	lw	$4,16($sp)
	lw	$5,4($2)
	jal	rtl8192cd_start_xmit
	nop

	beq	$2,$0,$L502
	nop

	lw	$2,120($sp)
	sw	$2,100($sp)
	lw	$2,16($sp)
	sw	$2,96($sp)
	li	$2,1			# 0x1
	sw	$2,92($sp)
	lw	$4,96($sp)
	jal	dev_kfree_skb_any
	nop

$L502:
	lw	$2,20($sp)
	lw	$3,364($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L443
	nop

	lw	$2,20($sp)
	lw	$4,656($2)
	lw	$5,132($sp)
	lw	$6,128($sp)
	jal	memcpy
	nop

	lw	$3,20($sp)
	lw	$2,128($sp)
	sw	$2,592($3)
	lw	$3,20($sp)
	li	$2,2			# 0x2
	sw	$2,364($3)
$L443:
	lw	$2,20($sp)
	addiu	$2,$2,600
	move	$4,$2
	lw	$5,32($sp)
	jal	mod_timer
	nop

	sw	$0,108($sp)
$L438:
	lw	$2,108($sp)
	lw	$31,116($sp)
	lw	$16,112($sp)
	addiu	$sp,$sp,120
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	WapiSendActivateAuthenticationPacket
	.section	.text.WapiSendAuthenticationRspPacket,"ax",@progbits
	.align	2
	.ent	WapiSendAuthenticationRspPacket
	.type	WapiSendAuthenticationRspPacket, @function
WapiSendAuthenticationRspPacket:
	.set	nomips16
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	sw	$31,64($sp)
	sw	$4,72($sp)
	sw	$5,76($sp)
	sw	$6,80($sp)
	sw	$7,84($sp)
	lw	$2,76($sp)
	lw	$2,6676($2)
	sw	$2,20($sp)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,100
	sw	$2,32($sp)
	lw	$2,72($sp)
	sw	$2,36($sp)
	li	$2,2688			# 0xa80
	sw	$2,40($sp)
	li	$2,1			# 0x1
	sw	$2,44($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	sw	$0,56($sp)
	lw	$4,36($sp)
	jal	alloc_skb_from_queue
	nop

	sw	$2,56($sp)
	lw	$2,56($sp)
	bne	$2,$0,$L507
	nop

	lw	$2,48($sp)
	beq	$2,$0,$L507
	nop

	lw	$4,40($sp)
	jal	dev_alloc_skb
	nop

	sw	$2,56($sp)
$L507:
	lw	$2,56($sp)
	sw	$2,52($sp)
	lw	$2,52($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L509
	nop

	j	$L510
	nop

$L509:
	lw	$2,20($sp)
	lw	$2,656($2)
	bne	$2,$0,$L511
	nop

	lw	$2,72($sp)
	sw	$2,56($sp)
	lw	$2,16($sp)
	sw	$2,52($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	lw	$4,52($sp)
	jal	dev_kfree_skb_any
	nop

	j	$L510
	nop

$L511:
	lw	$3,16($sp)
	li	$2,-30540
	sh	$2,106($3)
	lw	$3,16($sp)
	lw	$2,76($sp)
	addiu	$2,$2,6070
	lw	$4,176($3)
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,16($sp)
	lw	$2,176($2)
	addiu	$3,$2,6
	lw	$2,72($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,16($sp)
	lw	$3,176($2)
	li	$2,-30540
	sh	$2,12($3)
	lw	$3,16($sp)
	lw	$2,72($sp)
	lw	$2,4($2)
	sw	$2,24($3)
	lw	$4,16($sp)
	li	$5,14			# 0xe
	jal	skb_put
	nop

	lw	$2,16($sp)
	lw	$3,16($sp)
	lw	$4,176($2)
	lw	$2,84($3)
	addu	$2,$4,$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,56($sp)
	li	$2,5
	sb	$2,60($sp)
	lw	$3,56($sp)
	li	$2,1
	sh	$2,0($3)
	lw	$3,56($sp)
	li	$2,1
	sb	$2,2($3)
	lw	$3,56($sp)
	lbu	$2,60($sp)
	sb	$2,3($3)
	lw	$2,56($sp)
	sh	$0,4($2)
	lw	$3,56($sp)
	li	$2,12
	sh	$2,6($3)
	lw	$2,56($sp)
	sb	$0,10($2)
	lw	$2,56($sp)
	sb	$0,11($2)
	lw	$4,24($sp)
	lw	$3,20($sp)
	lw	$2,20($sp)
	lhu	$2,356($2)
	addiu	$2,$2,1
	sh	$2,356($3)
	sh	$2,8($4)
	lw	$2,24($sp)
	lhu	$3,6($2)
	lw	$2,24($sp)
	addu	$2,$3,$2
	sw	$2,28($sp)
	lw	$4,28($sp)
	lw	$5,84($sp)
	lw	$6,80($sp)
	jal	memcpy
	nop

	lw	$5,24($sp)
	lw	$2,24($sp)
	lw	$3,80($sp)
	lhu	$4,6($2)
	move	$2,$3
	addu	$2,$4,$2
	sh	$2,6($5)
	lw	$2,24($sp)
	lhu	$2,6($2)
	lw	$4,16($sp)
	move	$5,$2
	jal	skb_put
	nop

	lw	$2,16($sp)
	lw	$2,84($2)
	addiu	$3,$2,-14
	lw	$2,72($sp)
	lw	$2,4($2)
	lw	$2,208($2)
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L514
	nop

	lw	$4,16($sp)
	lw	$5,72($sp)
	jal	wapiFragementSend
	nop

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L517
	nop

	lw	$2,72($sp)
	sw	$2,56($sp)
	lw	$2,16($sp)
	sw	$2,52($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	lw	$4,52($sp)
	jal	dev_kfree_skb_any
	nop

	j	$L517
	nop

$L514:
	lw	$2,72($sp)
	lw	$4,16($sp)
	lw	$5,4($2)
	jal	rtl8192cd_start_xmit
	nop

	beq	$2,$0,$L517
	nop

	lw	$2,72($sp)
	sw	$2,56($sp)
	lw	$2,16($sp)
	sw	$2,52($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	lw	$4,52($sp)
	jal	dev_kfree_skb_any
	nop

$L517:
	lw	$2,20($sp)
	lw	$3,364($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L510
	nop

	lw	$2,20($sp)
	lw	$4,656($2)
	lw	$5,84($sp)
	lw	$6,80($sp)
	jal	memcpy
	nop

	lw	$3,20($sp)
	lw	$2,80($sp)
	sw	$2,592($3)
$L510:
	lw	$2,20($sp)
	addiu	$2,$2,600
	move	$4,$2
	lw	$5,32($sp)
	jal	mod_timer
	nop

	move	$2,$0
	lw	$31,64($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	WapiSendAuthenticationRspPacket
	.section	.text.wapiSendUnicastKeyAgrementRequeset,"ax",@progbits
	.align	2
	.globl	wapiSendUnicastKeyAgrementRequeset
	.ent	wapiSendUnicastKeyAgrementRequeset
	.type	wapiSendUnicastKeyAgrementRequeset, @function
wapiSendUnicastKeyAgrementRequeset:
	.set	nomips16
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	sw	$31,64($sp)
	sw	$4,72($sp)
	sw	$5,76($sp)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,100
	sw	$2,32($sp)
	lw	$2,76($sp)
	lw	$2,6676($2)
	sw	$2,20($sp)
	lw	$2,72($sp)
	sw	$2,36($sp)
	li	$2,1560			# 0x618
	sw	$2,40($sp)
	li	$2,1			# 0x1
	sw	$2,44($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	sw	$0,56($sp)
	lw	$4,36($sp)
	jal	alloc_skb_from_queue
	nop

	sw	$2,56($sp)
	lw	$2,56($sp)
	bne	$2,$0,$L522
	nop

	lw	$2,48($sp)
	beq	$2,$0,$L522
	nop

	lw	$4,40($sp)
	jal	dev_alloc_skb
	nop

	sw	$2,56($sp)
$L522:
	lw	$2,56($sp)
	sw	$2,52($sp)
	lw	$2,52($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L524
	nop

	j	$L525
	nop

$L524:
	lw	$3,16($sp)
	li	$2,-30540
	sh	$2,106($3)
	lw	$3,16($sp)
	lw	$2,76($sp)
	addiu	$2,$2,6070
	lw	$4,176($3)
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,16($sp)
	lw	$2,176($2)
	addiu	$3,$2,6
	lw	$2,72($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,16($sp)
	lw	$3,176($2)
	li	$2,-30540
	sh	$2,12($3)
	lw	$4,16($sp)
	li	$5,14			# 0xe
	jal	skb_put
	nop

	lw	$3,16($sp)
	lw	$2,72($sp)
	lw	$2,4($2)
	sw	$2,24($3)
	lw	$2,16($sp)
	lw	$3,16($sp)
	lw	$4,176($2)
	lw	$2,84($3)
	addu	$2,$4,$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,56($sp)
	li	$2,8
	sb	$2,60($sp)
	lw	$3,56($sp)
	li	$2,1
	sh	$2,0($3)
	lw	$3,56($sp)
	li	$2,1
	sb	$2,2($3)
	lw	$3,56($sp)
	lbu	$2,60($sp)
	sb	$2,3($3)
	lw	$2,56($sp)
	sh	$0,4($2)
	lw	$3,56($sp)
	li	$2,12
	sh	$2,6($3)
	lw	$2,56($sp)
	sb	$0,10($2)
	lw	$2,56($sp)
	sb	$0,11($2)
	lw	$4,24($sp)
	lw	$3,20($sp)
	lw	$2,20($sp)
	lhu	$2,356($2)
	addiu	$2,$2,1
	sh	$2,356($3)
	sh	$2,8($4)
	lw	$2,24($sp)
	lhu	$3,6($2)
	lw	$2,24($sp)
	addu	$2,$3,$2
	sw	$2,28($sp)
	lw	$5,28($sp)
	lw	$4,28($sp)
	lbu	$3,0($4)
	li	$2,-16			# 0xfffffff0
	and	$2,$3,$2
	sb	$2,0($4)
	lbu	$2,0($5)
	andi	$2,$2,0x1f
	sb	$2,0($5)
	lw	$2,20($sp)
	lw	$3,364($2)
	li	$2,5			# 0x5
	beq	$3,$2,$L528
	nop

	lw	$2,20($sp)
	lw	$3,364($2)
	li	$2,6			# 0x6
	beq	$3,$2,$L528
	nop

	j	$L527
	nop

$L528:
	lw	$4,28($sp)
	lbu	$3,0($4)
	li	$2,-17			# 0xffffffef
	and	$2,$3,$2
	sb	$2,0($4)
	lw	$2,28($sp)
	sb	$0,17($2)
	lw	$3,20($sp)
	lw	$4,352($3)
	li	$2,-65536			# 0xffff0000
	ori	$2,$2,0x7fff
	and	$2,$4,$2
	sw	$2,352($3)
	lw	$4,20($sp)
	lw	$3,352($4)
	li	$2,-4097			# 0xffffefff
	and	$2,$3,$2
	sw	$2,352($4)
	j	$L529
	nop

$L527:
	lw	$3,28($sp)
	lbu	$2,0($3)
	ori	$2,$2,0x10
	sb	$2,0($3)
	lw	$3,28($sp)
	lw	$2,20($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sb	$2,17($3)
$L529:
	lw	$2,28($sp)
	addiu	$3,$2,1
	lw	$2,20($sp)
	addiu	$2,$2,472
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,28($sp)
	addiu	$3,$2,18
	lw	$2,72($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,28($sp)
	addiu	$3,$2,24
	lw	$2,76($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,28($sp)
	addiu	$3,$2,30
	lw	$2,20($sp)
	addiu	$2,$2,288
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcpy
	nop

	lw	$3,24($sp)
	lw	$2,24($sp)
	lhu	$2,6($2)
	addiu	$2,$2,62
	sh	$2,6($3)
	lw	$2,24($sp)
	lhu	$2,6($2)
	lw	$4,16($sp)
	move	$5,$2
	jal	skb_put
	nop

	lw	$2,16($sp)
	lw	$2,84($2)
	addiu	$3,$2,-14
	lw	$2,72($sp)
	lw	$2,4($2)
	lw	$2,208($2)
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L530
	nop

	lw	$4,16($sp)
	lw	$5,72($sp)
	jal	wapiFragementSend
	nop

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L533
	nop

	lw	$2,72($sp)
	sw	$2,56($sp)
	lw	$2,16($sp)
	sw	$2,52($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	lw	$4,52($sp)
	jal	dev_kfree_skb_any
	nop

	j	$L533
	nop

$L530:
	lw	$2,72($sp)
	lw	$4,16($sp)
	lw	$5,4($2)
	jal	rtl8192cd_start_xmit
	nop

	beq	$2,$0,$L533
	nop

	lw	$2,72($sp)
	sw	$2,56($sp)
	lw	$2,16($sp)
	sw	$2,52($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	lw	$4,52($sp)
	jal	dev_kfree_skb_any
	nop

$L533:
	lw	$2,20($sp)
	lw	$3,364($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L525
	nop

	lw	$3,20($sp)
	li	$2,6			# 0x6
	sw	$2,364($3)
$L525:
	lw	$2,20($sp)
	addiu	$2,$2,600
	move	$4,$2
	lw	$5,32($sp)
	jal	mod_timer
	nop

	move	$2,$0
	lw	$31,64($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiSendUnicastKeyAgrementRequeset
	.section	.text.wapiSendUnicastKeyAgrementConfirm,"ax",@progbits
	.align	2
	.globl	wapiSendUnicastKeyAgrementConfirm
	.ent	wapiSendUnicastKeyAgrementConfirm
	.type	wapiSendUnicastKeyAgrementConfirm, @function
wapiSendUnicastKeyAgrementConfirm:
	.set	nomips16
	.frame	$sp,80,$31		# vars= 48, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-80
	sw	$31,72($sp)
	sw	$4,80($sp)
	sw	$5,84($sp)
	lw	$2,80($sp)
	sw	$2,40($sp)
	li	$2,1560			# 0x618
	sw	$2,44($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	li	$2,1			# 0x1
	sw	$2,52($sp)
	sw	$0,60($sp)
	lw	$4,40($sp)
	jal	alloc_skb_from_queue
	nop

	sw	$2,60($sp)
	lw	$2,60($sp)
	bne	$2,$0,$L538
	nop

	lw	$2,52($sp)
	beq	$2,$0,$L538
	nop

	lw	$4,44($sp)
	jal	dev_alloc_skb
	nop

	sw	$2,60($sp)
$L538:
	lw	$2,60($sp)
	sw	$2,56($sp)
	lw	$2,56($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L540
	nop

	j	$L541
	nop

$L540:
	lw	$3,24($sp)
	li	$2,-30540
	sh	$2,106($3)
	lw	$3,24($sp)
	lw	$2,84($sp)
	addiu	$2,$2,6070
	lw	$4,176($3)
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,24($sp)
	lw	$2,176($2)
	addiu	$3,$2,6
	lw	$2,80($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,24($sp)
	lw	$3,176($2)
	li	$2,-30540
	sh	$2,12($3)
	lw	$4,24($sp)
	li	$5,14			# 0xe
	jal	skb_put
	nop

	lw	$3,24($sp)
	lw	$2,80($sp)
	lw	$2,4($2)
	sw	$2,24($3)
	lw	$2,84($sp)
	lw	$2,6676($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$3,24($sp)
	lw	$4,176($2)
	lw	$2,84($3)
	addu	$2,$4,$2
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,60($sp)
	li	$2,10
	sb	$2,64($sp)
	lw	$3,60($sp)
	li	$2,1
	sh	$2,0($3)
	lw	$3,60($sp)
	li	$2,1
	sb	$2,2($3)
	lw	$3,60($sp)
	lbu	$2,64($sp)
	sb	$2,3($3)
	lw	$2,60($sp)
	sh	$0,4($2)
	lw	$3,60($sp)
	li	$2,12
	sh	$2,6($3)
	lw	$2,60($sp)
	sb	$0,10($2)
	lw	$2,60($sp)
	sb	$0,11($2)
	lw	$4,32($sp)
	lw	$3,28($sp)
	lw	$2,28($sp)
	lhu	$2,356($2)
	addiu	$2,$2,1
	sh	$2,356($3)
	sh	$2,8($4)
	lw	$2,32($sp)
	lhu	$3,6($2)
	lw	$2,32($sp)
	addu	$2,$3,$2
	sw	$2,36($sp)
	lw	$5,36($sp)
	lw	$4,36($sp)
	lbu	$3,0($4)
	li	$2,-16			# 0xfffffff0
	and	$2,$3,$2
	sb	$2,0($4)
	lbu	$2,0($5)
	andi	$2,$2,0x1f
	sb	$2,0($5)
	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,8			# 0x8
	beq	$3,$2,$L544
	nop

	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,9			# 0x9
	beq	$3,$2,$L544
	nop

	j	$L543
	nop

$L544:
	lw	$4,36($sp)
	lbu	$3,0($4)
	li	$2,-17			# 0xffffffef
	and	$2,$3,$2
	sb	$2,0($4)
	lw	$2,36($sp)
	sb	$0,17($2)
	j	$L545
	nop

$L543:
	lw	$3,36($sp)
	lbu	$2,0($3)
	ori	$2,$2,0x10
	sb	$2,0($3)
	lw	$3,36($sp)
	lw	$2,28($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sb	$2,17($3)
$L545:
	lw	$2,36($sp)
	addiu	$3,$2,1
	lw	$2,28($sp)
	addiu	$2,$2,472
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$3,$2,18
	lw	$2,80($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$3,$2,24
	lw	$2,84($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$3,$2,30
	lw	$2,28($sp)
	addiu	$2,$2,256
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	jal	memcpy
	nop

	lw	$2,36($sp)
	addiu	$4,$2,62
	lw	$2,28($sp)
	lw	$2,652($2)
	addiu	$3,$2,976
	lw	$2,80($sp)
	lbu	$2,1233($2)
	move	$5,$3
	move	$6,$2
	jal	memcpy
	nop

	lw	$2,28($sp)
	addiu	$4,$2,568
	lw	$2,80($sp)
	lbu	$2,1233($2)
	addiu	$7,$2,62
	lw	$2,80($sp)
	lbu	$3,1233($2)
	lw	$2,36($sp)
	addu	$2,$3,$2
	addiu	$2,$2,62
	sw	$2,16($sp)
	li	$2,20			# 0x14
	sw	$2,20($sp)
	li	$5,16			# 0x10
	lw	$6,36($sp)
	jal	sha256_hmac
	nop

	lw	$5,32($sp)
	lw	$4,32($sp)
	lw	$2,80($sp)
	lbu	$3,1233($2)
	lhu	$2,6($4)
	addu	$2,$3,$2
	addiu	$2,$2,82
	sh	$2,6($5)
	lw	$2,32($sp)
	lhu	$2,6($2)
	lw	$4,24($sp)
	move	$5,$2
	jal	skb_put
	nop

	lw	$2,24($sp)
	lw	$2,84($2)
	addiu	$3,$2,-14
	lw	$2,80($sp)
	lw	$2,4($2)
	lw	$2,208($2)
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L546
	nop

	lw	$4,24($sp)
	lw	$5,80($sp)
	jal	wapiFragementSend
	nop

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L549
	nop

	lw	$2,80($sp)
	sw	$2,60($sp)
	lw	$2,24($sp)
	sw	$2,56($sp)
	li	$2,1			# 0x1
	sw	$2,52($sp)
	lw	$4,56($sp)
	jal	dev_kfree_skb_any
	nop

	j	$L549
	nop

$L546:
	lw	$2,80($sp)
	lw	$4,24($sp)
	lw	$5,4($2)
	jal	rtl8192cd_start_xmit
	nop

	beq	$2,$0,$L549
	nop

	lw	$2,80($sp)
	sw	$2,60($sp)
	lw	$2,24($sp)
	sw	$2,56($sp)
	li	$2,1			# 0x1
	sw	$2,52($sp)
	lw	$4,56($sp)
	jal	dev_kfree_skb_any
	nop

$L549:
	lw	$2,28($sp)
	lw	$3,364($2)
	li	$2,8			# 0x8
	bne	$3,$2,$L541
	nop

	lw	$3,28($sp)
	li	$2,9			# 0x9
	sw	$2,364($3)
$L541:
	move	$2,$0
	lw	$31,72($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiSendUnicastKeyAgrementConfirm
	.section	.text.wapiSendMulticastKeyNotification,"ax",@progbits
	.align	2
	.globl	wapiSendMulticastKeyNotification
	.ent	wapiSendMulticastKeyNotification
	.type	wapiSendMulticastKeyNotification, @function
wapiSendMulticastKeyNotification:
	.set	nomips16
	.frame	$sp,96,$31		# vars= 56, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-96
	sw	$31,88($sp)
	sw	$4,96($sp)
	sw	$5,100($sp)
	lw	$2,100($sp)
	lw	$2,6676($2)
	sw	$2,36($sp)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,100
	sw	$2,48($sp)
	lw	$2,96($sp)
	addiu	$2,$2,960
	move	$4,$2
	li	$5,1			# 0x1
	jal	WapiIncreasePN
	nop

	bne	$2,$0,$L554
	nop

	lw	$4,96($sp)
	li	$5,1			# 0x1
	jal	wapiFreeAllSta
	nop

	sw	$0,80($sp)
	j	$L553
	nop

$L554:
	lw	$2,96($sp)
	sw	$2,52($sp)
	li	$2,1560			# 0x618
	sw	$2,56($sp)
	li	$2,1			# 0x1
	sw	$2,60($sp)
	li	$2,1			# 0x1
	sw	$2,64($sp)
	sw	$0,72($sp)
	lw	$4,52($sp)
	jal	alloc_skb_from_queue
	nop

	sw	$2,72($sp)
	lw	$2,72($sp)
	bne	$2,$0,$L555
	nop

	lw	$2,64($sp)
	beq	$2,$0,$L555
	nop

	lw	$4,56($sp)
	jal	dev_alloc_skb
	nop

	sw	$2,72($sp)
$L555:
	lw	$2,72($sp)
	sw	$2,68($sp)
	lw	$2,68($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L557
	nop

	j	$L558
	nop

$L557:
	lw	$3,32($sp)
	li	$2,-30540
	sh	$2,106($3)
	lw	$3,32($sp)
	lw	$2,100($sp)
	addiu	$2,$2,6070
	lw	$4,176($3)
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,32($sp)
	lw	$2,176($2)
	addiu	$3,$2,6
	lw	$2,96($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,32($sp)
	lw	$3,176($2)
	li	$2,-30540
	sh	$2,12($3)
	lw	$4,32($sp)
	li	$5,14			# 0xe
	jal	skb_put
	nop

	lw	$3,32($sp)
	lw	$2,96($sp)
	lw	$2,4($2)
	sw	$2,24($3)
	lw	$2,32($sp)
	lw	$3,32($sp)
	lw	$4,176($2)
	lw	$2,84($3)
	addu	$2,$4,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	sw	$2,72($sp)
	li	$2,11
	sb	$2,76($sp)
	lw	$3,72($sp)
	li	$2,1
	sh	$2,0($3)
	lw	$3,72($sp)
	li	$2,1
	sb	$2,2($3)
	lw	$3,72($sp)
	lbu	$2,76($sp)
	sb	$2,3($3)
	lw	$2,72($sp)
	sh	$0,4($2)
	lw	$3,72($sp)
	li	$2,12
	sh	$2,6($3)
	lw	$2,72($sp)
	sb	$0,10($2)
	lw	$2,72($sp)
	sb	$0,11($2)
	lw	$4,40($sp)
	lw	$3,36($sp)
	lw	$2,36($sp)
	lhu	$2,356($2)
	addiu	$2,$2,1
	sh	$2,356($3)
	sh	$2,8($4)
	lw	$2,40($sp)
	lhu	$3,6($2)
	lw	$2,40($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
	lw	$5,44($sp)
	lw	$4,44($sp)
	lbu	$3,0($4)
	li	$2,-32			# 0xffffffe0
	and	$2,$3,$2
	sb	$2,0($4)
	lbu	$2,0($5)
	andi	$2,$2,0x7f
	sb	$2,0($5)
	lw	$5,44($sp)
	lw	$4,44($sp)
	lbu	$3,0($4)
	li	$2,-33			# 0xffffffdf
	and	$2,$3,$2
	sb	$2,0($4)
	lbu	$3,0($5)
	li	$2,-65			# 0xffffffbf
	and	$2,$3,$2
	sb	$2,0($5)
	lw	$2,36($sp)
	lw	$3,364($2)
	li	$2,9			# 0x9
	beq	$3,$2,$L561
	nop

	lw	$2,36($sp)
	lw	$3,364($2)
	li	$2,10			# 0xa
	beq	$3,$2,$L561
	nop

	j	$L560
	nop

$L561:
	lw	$3,44($sp)
	lw	$2,96($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	sb	$2,1($3)
	j	$L562
	nop

$L560:
	lw	$3,44($sp)
	lw	$2,96($sp)
	lw	$2,1232($2)
	srl	$2,$2,15
	xori	$2,$2,0x1
	andi	$2,$2,0x1
	sb	$2,1($3)
$L562:
	lw	$3,44($sp)
	lw	$2,36($sp)
	lw	$2,352($2)
	srl	$2,$2,15
	andi	$2,$2,0x1
	sb	$2,2($3)
	lw	$2,44($sp)
	addiu	$3,$2,3
	lw	$2,96($sp)
	lw	$2,4($2)
	addiu	$2,$2,320
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,44($sp)
	addiu	$3,$2,9
	lw	$2,100($sp)
	addiu	$2,$2,6070
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	jal	memcpy
	nop

	lw	$2,44($sp)
	addiu	$3,$2,15
	lw	$2,96($sp)
	addiu	$2,$2,928
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$2,44($sp)
	addiu	$3,$2,31
	lw	$2,96($sp)
	addiu	$2,$2,960
	move	$4,$3
	move	$5,$2
	li	$6,16			# 0x10
	jal	memcpy
	nop

	lw	$3,40($sp)
	lw	$2,40($sp)
	lhu	$2,6($2)
	addiu	$2,$2,47
	sh	$2,6($3)
	lw	$2,36($sp)
	addiu	$4,$2,552
	lw	$2,96($sp)
	addiu	$5,$2,960
	lw	$2,96($sp)
	addiu	$3,$2,912
	lw	$2,44($sp)
	addiu	$2,$2,48
	sw	$2,16($sp)
	lw	$2,44($sp)
	addiu	$2,$2,47
	sw	$2,20($sp)
	sw	$0,24($sp)
	move	$6,$3
	li	$7,16			# 0x10
	jal	WapiSMS4ForMNKEncrypt
	nop

	lw	$5,40($sp)
	lw	$4,40($sp)
	lw	$2,44($sp)
	lbu	$3,47($2)
	lhu	$2,6($4)
	addu	$2,$3,$2
	addiu	$2,$2,1
	sh	$2,6($5)
	lw	$2,36($sp)
	addiu	$4,$2,568
	lw	$2,40($sp)
	lhu	$2,6($2)
	addiu	$7,$2,-12
	lw	$2,40($sp)
	lhu	$3,6($2)
	lw	$2,44($sp)
	addu	$2,$3,$2
	addiu	$2,$2,-12
	sw	$2,16($sp)
	li	$2,20			# 0x14
	sw	$2,20($sp)
	li	$5,16			# 0x10
	lw	$6,44($sp)
	jal	sha256_hmac
	nop

	lw	$3,40($sp)
	lw	$2,40($sp)
	lhu	$2,6($2)
	addiu	$2,$2,20
	sh	$2,6($3)
	lw	$2,40($sp)
	lhu	$2,6($2)
	lw	$4,32($sp)
	move	$5,$2
	jal	skb_put
	nop

	lw	$2,32($sp)
	lw	$2,84($2)
	addiu	$3,$2,-14
	lw	$2,96($sp)
	lw	$2,4($2)
	lw	$2,208($2)
	addiu	$2,$2,-26
	sltu	$2,$2,$3
	beq	$2,$0,$L563
	nop

	lw	$4,32($sp)
	lw	$5,96($sp)
	jal	wapiFragementSend
	nop

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L566
	nop

	lw	$2,96($sp)
	sw	$2,72($sp)
	lw	$2,32($sp)
	sw	$2,68($sp)
	li	$2,1			# 0x1
	sw	$2,64($sp)
	lw	$4,68($sp)
	jal	dev_kfree_skb_any
	nop

	j	$L566
	nop

$L563:
	lw	$2,96($sp)
	lw	$4,32($sp)
	lw	$5,4($2)
	jal	rtl8192cd_start_xmit
	nop

	beq	$2,$0,$L566
	nop

	lw	$2,96($sp)
	sw	$2,72($sp)
	lw	$2,32($sp)
	sw	$2,68($sp)
	li	$2,1			# 0x1
	sw	$2,64($sp)
	lw	$4,68($sp)
	jal	dev_kfree_skb_any
	nop

$L566:
	lw	$2,36($sp)
	lw	$3,364($2)
	li	$2,9			# 0x9
	bne	$3,$2,$L558
	nop

	lw	$3,36($sp)
	li	$2,10			# 0xa
	sw	$2,364($3)
$L558:
	lw	$2,36($sp)
	addiu	$2,$2,600
	move	$4,$2
	lw	$5,48($sp)
	jal	mod_timer
	nop

	sw	$0,80($sp)
$L553:
	lw	$2,80($sp)
	lw	$31,88($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiSendMulticastKeyNotification
	.section	.text.wapiUpdateUSK,"ax",@progbits
	.align	2
	.globl	wapiUpdateUSK
	.ent	wapiUpdateUSK
	.type	wapiUpdateUSK, @function
wapiUpdateUSK:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,24($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	lw	$2,36($sp)
	lw	$2,6676($2)
	sw	$2,16($sp)
	lw	$3,16($sp)
	lw	$2,352($3)
	ori	$2,$2,0x1000
	sw	$2,352($3)
	lw	$2,16($sp)
	sh	$0,358($2)
	lw	$3,16($sp)
	lw	$2,16($sp)
	lw	$2,652($2)
	lhu	$2,1236($2)
	sh	$2,356($3)
	lw	$2,16($sp)
	lw	$3,652($2)
	lw	$2,16($sp)
	lw	$2,652($2)
	lhu	$2,1236($2)
	addiu	$2,$2,8
	sh	$2,1236($3)
	lw	$4,16($sp)
	jal	wapiReleaseFragementQueue
	nop

	lw	$2,16($sp)
	addiu	$3,$2,624
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,600
	move	$4,$3
	move	$5,$2
	jal	mod_timer
	nop

	lw	$4,32($sp)
	lw	$5,36($sp)
	jal	wapiSendUnicastKeyAgrementRequeset
	nop

	lw	$31,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiUpdateUSK
	.section	.text.wapiUpdateMSK,"ax",@progbits
	.align	2
	.globl	wapiUpdateMSK
	.ent	wapiUpdateMSK
	.type	wapiUpdateMSK, @function
wapiUpdateMSK:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	sw	$31,48($sp)
	sw	$4,56($sp)
	sw	$5,60($sp)
	lw	$2,56($sp)
	lw	$2,8($2)
	lw	$3,5256($2)
	move	$2,$3
	sll	$2,$2,1
	addu	$2,$2,$3
	sll	$2,$2,3
	addu	$2,$2,$3
	sll	$2,$2,2
	sw	$2,32($sp)
	lw	$3,56($sp)
	li	$2,65536			# 0x10000
	ori	$2,$2,0x2b6c
	addu	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$3,0($2)
	lw	$2,36($sp)
	xor	$2,$3,$2
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$2,40($sp)
	beq	$2,$0,$L572
	nop

	j	$L574
	nop

$L572:
	lw	$2,56($sp)
	addiu	$2,$2,912
	move	$4,$2
	li	$5,16			# 0x10
	jal	GenerateRandomData
	nop

	lw	$4,56($sp)
	lw	$3,1232($4)
	li	$2,-16385			# 0xffffbfff
	and	$2,$3,$2
	sw	$2,1232($4)
	lw	$4,56($sp)
	lw	$3,1232($4)
	li	$2,-8193			# 0xffffdfff
	and	$2,$3,$2
	sw	$2,1232($4)
	sw	$0,28($sp)
$L575:
	lw	$2,28($sp)
	sltu	$2,$2,31
	beq	$2,$0,$L576
	nop

	lw	$2,28($sp)
	sll	$3,$2,3
	lw	$2,56($sp)
	addu	$3,$3,$2
	li	$2,65536			# 0x10000
	ori	$2,$2,0x2a74
	addu	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
$L578:
	lw	$3,16($sp)
	lw	$2,20($sp)
	beq	$3,$2,$L577
	nop

	lw	$2,16($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,24($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,24($sp)
	lw	$2,6676($2)
	lw	$2,364($2)
	sltu	$2,$2,13
	beq	$2,$0,$L580
	nop

	lw	$4,56($sp)
	lw	$5,24($sp)
	li	$6,25			# 0x19
	jal	wapiDeauthSta
	nop

	j	$L578
	nop

$L580:
	lw	$3,56($sp)
	lw	$2,1232($3)
	ori	$2,$2,0x2000
	sw	$2,1232($3)
	lw	$2,24($sp)
	lw	$4,6676($2)
	lw	$3,352($4)
	li	$2,-1025			# 0xfffffbff
	and	$2,$3,$2
	sw	$2,352($4)
	lw	$2,24($sp)
	lw	$2,6676($2)
	sh	$0,358($2)
	lw	$2,24($sp)
	lw	$3,6676($2)
	lw	$2,56($sp)
	lhu	$2,1236($2)
	sh	$2,356($3)
	lw	$3,56($sp)
	lw	$2,56($sp)
	lhu	$2,1236($2)
	addiu	$2,$2,8
	sh	$2,1236($3)
	lw	$2,24($sp)
	lw	$4,6676($2)
	jal	wapiReleaseFragementQueue
	nop

	lw	$4,56($sp)
	lw	$5,24($sp)
	jal	wapiSendMulticastKeyNotification
	nop

	j	$L578
	nop

$L577:
	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
	j	$L575
	nop

$L576:
	lw	$2,56($sp)
	lw	$2,1232($2)
	andi	$2,$2,0x2000
	beq	$2,$0,$L574
	nop

	li	$2,600			# 0x258
	sw	$2,32($sp)
$L574:
	lw	$2,56($sp)
	addiu	$2,$2,1308
	move	$4,$2
	jal	del_timer
	nop

	lw	$2,56($sp)
	addiu	$2,$2,1308
	move	$4,$2
	move	$5,$0
	move	$6,$0
	jal	init_timer_key
	nop

	lw	$3,56($sp)
	lw	$2,56($sp)
	sw	$2,1324($3)
	lw	$3,56($sp)
	lui	$2,%hi(wapiMCastUpdateKeyTimeout)
	addiu	$2,$2,%lo(wapiMCastUpdateKeyTimeout)
	sw	$2,1320($3)
	lw	$2,56($sp)
	lw	$2,8($2)
	lw	$3,5252($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L583
	nop

	lw	$2,56($sp)
	lw	$2,8($2)
	lw	$3,5252($2)
	li	$2,4			# 0x4
	beq	$3,$2,$L583
	nop

	j	$L582
	nop

$L583:
	lw	$2,32($sp)
	bgez	$2,$L584
	nop

	lw	$2,56($sp)
	addiu	$4,$2,1308
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	li	$2,2147418112			# 0x7fff0000
	ori	$2,$2,0xffff
	addu	$2,$3,$2
	move	$5,$2
	jal	mod_timer
	nop

	j	$L582
	nop

$L584:
	lw	$2,56($sp)
	addiu	$4,$2,1308
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,32($sp)
	addu	$2,$3,$2
	move	$5,$2
	jal	mod_timer
	nop

$L582:
	li	$2,1			# 0x1
	lw	$31,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	wapiUpdateMSK
	.ident	"GCC: (GNU) 3.4.6-1.3.6"
