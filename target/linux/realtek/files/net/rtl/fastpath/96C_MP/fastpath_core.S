	.file	1 "fastpath_core.c"
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
	.section	.text.arp_req_get_ha,"ax",@progbits
	.align	2
	.globl	arp_req_get_ha
	.ent	arp_req_get_ha
	.type	arp_req_get_ha, @function
arp_req_get_ha:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 8, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-40
	sw	$17,28($sp)
	sw	$4,16($sp)
	move	$17,$5
	lui	$4,%hi(arp_tbl)
	sw	$18,32($sp)
	addiu	$4,$4,%lo(arp_tbl)
	move	$18,$6
	addiu	$5,$sp,16
	sw	$16,24($sp)
	sw	$31,36($sp)
	.set	noreorder
	.set	nomacro
	jal	neigh_lookup
	move	$6,$17
	.set	macro
	.set	reorder

	move	$16,$2
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L2
	li	$2,-6			# 0xfffffffffffffffa
	.set	macro
	.set	reorder

	jal	local_bh_disable
	lbu	$6,256($17)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$16,36
	.set	macro
	.set	reorder

	jal	local_bh_enable
#APP
	raw_local_irq_save	$3
#NO_APP
	lw	$2,72($16)
	#nop
	addiu	$2,$2,-1
	sw	$2,72($16)
#APP
	raw_local_irq_restore	$3
#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L11
	move	$4,$16
	.set	macro
	.set	reorder

	jal	neigh_destroy
$L11:
	move	$2,$0
$L2:
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,40
	.set	macro
	.set	reorder

	.end	arp_req_get_ha
	.section	.text.cache_timeout,"ax",@progbits
	.align	2
	.ent	cache_timeout
	.type	cache_timeout, @function
cache_timeout:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lbu	$2,0($4)
	nop
	beq	$2,$0,$L20
	move	$7,$4

	lw	$2,8($4)
	lw	$3,4($4)
	lhu	$4,2($4)
	lbu	$5,1($7)
	xor	$3,$3,$2
	srl	$2,$3,16
	xor	$2,$2,$4
	xor	$3,$3,$2
	lui	$2,%hi(max_udp_frag_entry)
	lw	$4,%lo(max_udp_frag_entry)($2)
	srl	$2,$3,8
	xor	$2,$2,$5
	xor	$3,$3,$2
	addiu	$4,$4,-1
	and	$4,$3,$4
	lw	$6,40($7)
	sll	$2,$4,1
	addu	$2,$2,$4
	sb	$0,0($7)
	lw	$3,44($7)
	beq	$6,$0,$L16
	sll	$5,$2,2

	j	$L17
	sw	$3,44($6)

$L16:
	lui	$2,%hi(udp_cache_table)
	lw	$2,%lo(udp_cache_table)($2)
	nop
	lw	$2,0($2)
	nop
	addu	$2,$5,$2
	sw	$3,4($2)
$L17:
	lw	$2,40($7)
	lw	$5,44($7)
	sll	$3,$4,1
	addu	$3,$3,$4
	sw	$2,0($5)
	lui	$2,%hi(udp_cache_table)
	lw	$2,%lo(udp_cache_table)($2)
	sll	$3,$3,2
	lui	$4,%hi(udp_cache_list_free)
	lw	$2,0($2)
	addiu	$4,$4,%lo(udp_cache_list_free)
	addiu	$6,$7,48
	addu	$3,$3,$2
	lw	$2,8($3)
	nop
	addiu	$2,$2,-1
	sw	$2,8($3)
	sw	$0,48($7)
	lw	$2,4($4)
	nop
	sw	$2,52($7)
	lw	$2,4($4)
	nop
	sw	$7,0($2)
	lw	$2,8($4)
	sw	$6,4($4)
	addiu	$2,$2,1
	sw	$2,8($4)
$L20:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	cache_timeout
	.section	.text.udp_fragCache_init,"ax",@progbits
	.align	2
	.globl	udp_fragCache_init
	.ent	udp_fragCache_init
	.type	udp_fragCache_init, @function
udp_fragCache_init:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 16, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L26:
$L67:
	addiu	$sp,$sp,-56
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	sw	$31,48($sp)
	sw	$19,44($sp)
	lui	$2,%hi(malloc_sizes+4)
	move	$18,$4
	lw	$4,%lo(malloc_sizes+4)($2)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	lui	$16,%hi(udp_cache_table)

	move	$17,$2
	sw	$2,%lo(udp_cache_table)($16)
	beq	$2,$0,$L21
	move	$3,$0

$L76:
$L117:
	lui	$19,%hi(udp_cache_list_free)
	sll	$4,$18,1
	addiu	$3,$19,%lo(udp_cache_list_free)
	addu	$4,$4,$18
	sw	$3,4($3)
	lui	$2,%hi(max_udp_frag_entry)
	sw	$0,8($3)
	sll	$4,$4,2
	li	$5,32			# 0x20
	sw	$18,%lo(max_udp_frag_entry)($2)
	jal	__kmalloc
	sw	$0,%lo(udp_cache_list_free)($19)

	sw	$2,0($17)
	lw	$2,%lo(udp_cache_table)($16)
	nop
	lw	$2,0($2)
	nop
	beq	$2,$0,$L21
	li	$3,-1			# 0xffffffffffffffff

	j	$L120
	slt	$5,$0,$18

$L184:
	j	$L21
	move	$3,$0

$L120:
	beq	$5,$0,$L183
	move	$17,$0

	move	$4,$0
$L125:
	lw	$2,%lo(udp_cache_table)($16)
	addiu	$17,$17,1
	slt	$3,$17,$18
	lw	$2,0($2)
	nop
	addu	$2,$4,$2
	sw	$0,8($2)
	lw	$2,%lo(udp_cache_table)($16)
	nop
	lw	$2,0($2)
	nop
	addu	$2,$4,$2
	sw	$0,0($2)
	lw	$2,%lo(udp_cache_table)($16)
	nop
	lw	$2,0($2)
	nop
	addu	$2,$4,$2
	sw	$2,4($2)
	bne	$3,$0,$L125
	addiu	$4,$4,12

	move	$17,$0
$L185:
	beq	$5,$0,$L183
	lui	$2,%hi(malloc_sizes+12)

$L133:
$L174:
	lw	$4,%lo(malloc_sizes+12)($2)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	move	$16,$2
	addiu	$4,$2,16
	move	$5,$0
	beq	$2,$0,$L184
	move	$6,$0

	jal	init_timer_key
	nop

	lui	$2,%hi(cache_timeout)
	addiu	$2,$2,%lo(cache_timeout)
	sw	$2,28($16)
	sw	$16,32($16)
	sw	$0,48($16)
	addiu	$3,$19,%lo(udp_cache_list_free)
	lw	$2,4($3)
	addiu	$4,$16,48
	slt	$5,$17,$18
	sw	$2,52($16)
	lw	$2,4($3)
	nop
	sw	$16,0($2)
	lw	$2,8($3)
	sw	$4,4($3)
	addiu	$2,$2,1
	j	$L185
	sw	$2,8($3)

$L183:
	li	$3,1			# 0x1
$L21:
	lw	$31,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	udp_fragCache_init
	.section	.text.FastPath_Hash_NAPT_Entry,"ax",@progbits
	.align	2
	.ent	FastPath_Hash_NAPT_Entry
	.type	FastPath_Hash_NAPT_Entry, @function
FastPath_Hash_NAPT_Entry:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	srl	$3,$4,16
	srl	$2,$6,16
	xor	$2,$2,$6
	xor	$3,$3,$4
	xor	$3,$3,$2
	lhu	$4,18($sp)
	xor	$3,$3,$5
	lui	$2,%hi(napt_table_list_max)
	xor	$3,$3,$7
	lw	$2,%lo(napt_table_list_max)($2)
	xor	$3,$3,$4
	srl	$4,$3,12
	xor	$3,$3,$4
	addiu	$2,$2,-1
	j	$31
	and	$2,$2,$3

	.set	macro
	.set	reorder
	.end	FastPath_Hash_NAPT_Entry
	.section	.text.rtk_addArp,"ax",@progbits
	.align	2
	.globl	rtk_addArp
	.ent	rtk_addArp
	.type	rtk_addArp, @function
rtk_addArp:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addArp
	.section	.text.rtk_modifyArp,"ax",@progbits
	.align	2
	.globl	rtk_modifyArp
	.ent	rtk_modifyArp
	.type	rtk_modifyArp, @function
rtk_modifyArp:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_modifyArp
	.section	.text.rtk_delArp,"ax",@progbits
	.align	2
	.globl	rtk_delArp
	.ent	rtk_delArp
	.type	rtk_delArp, @function
rtk_delArp:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	li	$2,-4			# 0xfffffffffffffffc

	.set	macro
	.set	reorder
	.end	rtk_delArp
	.section	.text.rtk_addRoute,"ax",@progbits
	.align	2
	.globl	rtk_addRoute
	.ent	rtk_addRoute
	.type	rtk_addRoute, @function
rtk_addRoute:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addRoute
	.section	.text.rtk_modifyRoute,"ax",@progbits
	.align	2
	.globl	rtk_modifyRoute
	.ent	rtk_modifyRoute
	.type	rtk_modifyRoute, @function
rtk_modifyRoute:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_modifyRoute
	.section	.text.rtk_delRoute,"ax",@progbits
	.align	2
	.globl	rtk_delRoute
	.ent	rtk_delRoute
	.type	rtk_delRoute, @function
rtk_delRoute:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	li	$2,-4			# 0xfffffffffffffffc

	.set	macro
	.set	reorder
	.end	rtk_delRoute
	.section	.text.rtk_addSession,"ax",@progbits
	.align	2
	.globl	rtk_addSession
	.ent	rtk_addSession
	.type	rtk_addSession, @function
rtk_addSession:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addSession
	.section	.text.rtk_delSession,"ax",@progbits
	.align	2
	.globl	rtk_delSession
	.ent	rtk_delSession
	.type	rtk_delSession, @function
rtk_delSession:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_delSession
	.section	.text.FastPath_Route,"ax",@progbits
	.align	2
	.globl	FastPath_Route
	.ent	FastPath_Route
	.type	FastPath_Route, @function
FastPath_Route:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(route_list_inuse)
	lw	$3,%lo(route_list_inuse)($2)
	move	$8,$0
	move	$7,$0
$L203:
	beq	$3,$0,$L202
	nop

	lw	$5,8($3)
	nop
	sltu	$2,$5,$7
	bne	$2,$0,$L198
	and	$6,$4,$5

	lw	$2,4($3)
	nop
	bne	$6,$2,$L198
	nop

	move	$7,$5
	addiu	$8,$3,16
$L198:
	lw	$3,44($3)
	j	$L203
	nop

$L202:
	j	$31
	move	$2,$8

	.set	macro
	.set	reorder
	.end	FastPath_Route
	.section	.text.rtk_addNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_addNaptConnection
	.ent	rtk_addNaptConnection
	.type	rtk_addNaptConnection, @function
rtk_addNaptConnection:
	.set	nomips16
	.frame	$sp,152,$31		# vars= 88, regs= 10/0, args= 24, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-152
	sw	$23,140($sp)
	sw	$18,120($sp)
	sw	$17,116($sp)
	sw	$31,148($sp)
	sw	$fp,144($sp)
	sw	$22,136($sp)
	sw	$21,132($sp)
	sw	$20,128($sp)
	sw	$19,124($sp)
	sw	$16,112($sp)
	move	$18,$5
	move	$17,$4
	sw	$6,160($sp)
	move	$23,$7
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L204
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	lw	$3,0($4)
	lw	$4,20($4)
	li	$2,17			# 0x11
	xori	$3,$3,0x2
	li	$5,6			# 0x6
	movn	$5,$2,$3		#RLX4181/RLX4281:conditional move  
	.set	noreorder
	.set	nomacro
	jal	filter_addconnect
	sw	$5,72($sp)
	.set	macro
	.set	reorder

	lw	$22,0($17)
	lw	$21,4($17)
	lw	$20,8($17)
	lw	$19,20($17)
	lw	$16,24($17)
	andi	$2,$22,0xffff
	move	$4,$21
	sw	$2,16($sp)
	move	$5,$20
	move	$6,$19
	.set	noreorder
	.set	nomacro
	jal	FastPath_Hash_NAPT_Entry
	move	$7,$16
	.set	macro
	.set	reorder

	lui	$8,%hi(table_napt)
	lw	$3,%lo(table_napt)($8)
	sll	$9,$2,1
	move	$fp,$2
	lw	$3,0($3)
	addu	$2,$9,$2
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$4,0($2)
	#nop
$L341:
	beq	$4,$0,$L339
	lw	$2,4($4)
	#nop
	bne	$2,$22,$L210
	lw	$2,8($4)
	#nop
	bne	$2,$21,$L210
	lw	$2,12($4)
	#nop
	bne	$2,$20,$L210
	lw	$3,16($4)
	lw	$2,12($17)
	#nop
	bne	$3,$2,$L210
	lw	$3,20($4)
	lw	$2,16($17)
	#nop
	bne	$3,$2,$L210
	lw	$2,24($4)
	#nop
	bne	$2,$19,$L210
	lw	$2,28($4)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$16,$L329
	move	$2,$0
	.set	macro
	.set	reorder

$L210:
	lw	$4,36($4)
	j	$L341
$L339:
#APP
	raw_local_irq_save	$2
#NO_APP
	lui	$4,%hi(napt_list_free)
	lw	$7,%lo(napt_list_free)($4)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L215
	sw	$2,76($sp)
	.set	macro
	.set	reorder

	lw	$2,0($17)
	lw	$6,44($7)
	lw	$5,48($7)
	sw	$2,4($7)
	lw	$2,4($17)
	#nop
	sw	$2,8($7)
	lw	$2,8($17)
	#nop
	sw	$2,12($7)
	lw	$2,12($17)
	#nop
	sw	$2,16($7)
	lw	$2,16($17)
	#nop
	sw	$2,20($7)
	lw	$2,20($17)
	#nop
	sw	$2,24($7)
	lw	$3,24($17)
	li	$2,-1
	sw	$23,32($7)
	sb	$2,0($7)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L217
	sw	$3,28($7)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L218
	sw	$5,48($6)
	.set	macro
	.set	reorder

$L217:
	addiu	$2,$4,%lo(napt_list_free)
	sw	$5,4($2)
$L218:
	lw	$3,48($7)
	lw	$2,44($7)
	addiu	$4,$4,%lo(napt_list_free)
	addiu	$5,$7,44
	sw	$2,0($3)
	lw	$2,8($4)
	lui	$3,%hi(napt_list_inuse)
	addiu	$3,$3,%lo(napt_list_inuse)
	addiu	$2,$2,-1
	sw	$2,8($4)
	sw	$0,44($7)
	lw	$2,4($3)
	addu	$4,$9,$fp
	sll	$4,$4,2
	sw	$2,48($7)
	lw	$2,4($3)
	addiu	$6,$7,36
	sw	$7,0($2)
	lw	$2,8($3)
	sw	$5,4($3)
	addiu	$2,$2,1
	sw	$2,8($3)
	sw	$0,36($7)
	lw	$2,%lo(table_napt)($8)
	#nop
	lw	$2,0($2)
	#nop
	addu	$2,$4,$2
	lw	$2,4($2)
	#nop
	sw	$2,40($7)
	lw	$2,%lo(table_napt)($8)
	#nop
	lw	$2,0($2)
	#nop
	addu	$2,$4,$2
	lw	$2,4($2)
	#nop
	sw	$7,0($2)
	lw	$2,%lo(table_napt)($8)
	#nop
	lw	$2,0($2)
	#nop
	addu	$2,$4,$2
	sw	$6,4($2)
	lw	$2,%lo(table_napt)($8)
	#nop
	lw	$2,0($2)
	#nop
	addu	$4,$4,$2
	lw	$2,8($4)
	#nop
	addiu	$2,$2,1
	sw	$2,8($4)
	li	$2,33
	sb	$2,57($sp)
	li	$2,8
	sb	$2,64($sp)
	sb	$0,56($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl865x_getLanDev
	sb	$0,65($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl865x_getWanDev
	move	$21,$2
	.set	macro
	.set	reorder

	lw	$20,152($18)
	move	$22,$2
	lw	$5,156($18)
	lbu	$2,0($20)
	lhu	$3,106($18)
	addiu	$4,$sp,24
	andi	$2,$2,0xf
	addiu	$5,$5,6
	li	$6,6			# 0x6
	sll	$2,$2,2
	addu	$23,$20,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,100($sp)
	.set	macro
	.set	reorder

	lw	$5,156($18)
	addiu	$4,$sp,32
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$5,12($20)
	#nop
	sw	$5,80($sp)
	lhu	$2,0($23)
	#nop
	sw	$2,88($sp)
	lw	$3,16($20)
	#nop
	sw	$3,84($sp)
	lhu	$5,2($23)
	#nop
	sw	$5,92($sp)
	lw	$2,28($18)
	#nop
	sw	$2,96($sp)
	sw	$0,28($18)
	lw	$5,4($17)
	lw	$6,20($17)
	lw	$4,8($17)
	srl	$2,$5,8
	srl	$3,$6,16
	xor	$2,$2,$5
	xor	$3,$3,$6
	lw	$5,24($17)
	xor	$2,$2,$3
	srl	$4,$4,4
	xor	$2,$2,$4
	xor	$2,$2,$5
	lw	$5,72($sp)
	lui	$3,%hi(path_table_list_max)
	lw	$4,%lo(path_table_list_max)($3)
	lui	$3,%hi(path_list_free)
	xor	$2,$2,$5
	lw	$16,%lo(path_list_free)($3)
	lw	$5,140($18)
	srl	$3,$2,12
	xor	$2,$2,$3
	addiu	$4,$4,-1
	and	$fp,$4,$2
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L228
	sw	$5,104($sp)
	.set	macro
	.set	reorder

	lw	$2,0($17)
	#nop
	sw	$2,4($16)
	lw	$2,4($17)
	#nop
	sw	$2,16($16)
	lw	$2,8($17)
	#nop
	sw	$2,12($16)
	lw	$2,20($17)
	#nop
	sw	$2,20($16)
	lw	$2,24($17)
	#nop
	sw	$2,8($16)
	lw	$2,12($17)
	#nop
	sw	$2,24($16)
	lw	$2,16($17)
	#nop
	sw	$2,28($16)
	lw	$4,20($17)
	#nop
	sw	$4,32($16)
	lw	$2,24($17)
	.set	noreorder
	.set	nomacro
	jal	FastPath_Route
	sw	$2,36($16)
	.set	macro
	.set	reorder

	lw	$4,16($16)
	lw	$3,24($16)
	sw	$2,44($16)
	li	$2,1
	sb	$2,2($16)
	li	$2,-1
	sb	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$4,$3,$L229
	sb	$0,1($16)
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	sb	$2,1($16)
$L229:
	lw	$3,12($16)
	lw	$2,28($16)
	#nop
	beq	$3,$2,$L230
	lbu	$2,1($16)
	#nop
	ori	$2,$2,0x2
	sb	$2,1($16)
$L230:
	sw	$0,48($16)
	sw	$0,52($16)
	lw	$2,4($17)
	addiu	$19,$sp,40
	move	$4,$19
	sw	$2,12($20)
	lw	$2,8($17)
	move	$5,$0
	li	$6,14			# 0xe
	sh	$2,0($23)
	lw	$2,20($17)
	#nop
	sw	$2,16($20)
	lw	$2,24($17)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($23)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L231
	move	$5,$21
	.set	macro
	.set	reorder

	lw	$4,4($17)
	.set	noreorder
	.set	nomacro
	jal	arp_req_get_ha
	move	$6,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L231
	addiu	$5,$sp,56
	.set	macro
	.set	reorder

	lw	$4,172($18)
	li	$6,2			# 0x2
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$4,$4,-2
	.set	macro
	.set	reorder

	bne	$2,$0,$L232
	lw	$3,156($18)
	#nop
	beq	$3,$0,$L232
	lbu	$2,12($3)
	lbu	$3,13($3)
	sll	$2,$2,8
	or	$3,$3,$2
	li	$2,2048			# 0x800
	beq	$3,$2,$L232
	lw	$2,172($18)
	#nop
	addiu	$2,$2,-22
	sw	$2,156($18)
$L232:
	lw	$4,156($18)
	move	$5,$19
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,6
	.set	macro
	.set	reorder

	lw	$4,156($18)
	addiu	$5,$21,320
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

$L231:
	sw	$0,140($18)
	lw	$3,100($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L241
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L241
	lui	$2,%hi(init_net+204)
	.set	macro
	.set	reorder

	lw	$2,%lo(init_net+204)($2)
	move	$4,$18
	move	$5,$0
	move	$6,$21
	move	$7,$22
	.set	noreorder
	.set	nomacro
	jal	ipt_do_table
	sw	$2,16($sp)
	.set	macro
	.set	reorder

$L241:
	lw	$2,140($18)
	addiu	$19,$sp,40
	move	$4,$19
	sw	$2,48($16)
	lw	$2,12($17)
	move	$5,$0
	li	$6,14			# 0xe
	sw	$2,12($20)
	lw	$2,16($17)
	#nop
	sh	$2,0($23)
	lw	$2,20($17)
	#nop
	sw	$2,16($20)
	lw	$2,24($17)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($23)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$22,$0,$L246
	move	$5,$22
	.set	macro
	.set	reorder

	lw	$4,20($17)
	.set	noreorder
	.set	nomacro
	jal	arp_req_get_ha
	move	$6,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L246
	addiu	$5,$sp,56
	.set	macro
	.set	reorder

	lw	$4,172($18)
	li	$6,2			# 0x2
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$4,$4,-2
	.set	macro
	.set	reorder

	bne	$2,$0,$L247
	lw	$3,156($18)
	#nop
	beq	$3,$0,$L247
	lbu	$2,12($3)
	lbu	$3,13($3)
	sll	$2,$2,8
	or	$3,$3,$2
	li	$2,2048			# 0x800
	beq	$3,$2,$L247
	lw	$2,172($18)
	#nop
	addiu	$2,$2,-22
	sw	$2,156($18)
$L247:
	lw	$4,156($18)
	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$4,156($18)
	addiu	$5,$22,320
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,6
	.set	macro
	.set	reorder

$L246:
	sw	$0,140($18)
	lw	$5,100($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L256
	lui	$2,%hi(nf_hooks+160)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L256
	lui	$2,%hi(init_net+204)
	.set	macro
	.set	reorder

	lw	$2,%lo(init_net+204)($2)
	move	$4,$18
	li	$5,4			# 0x4
	move	$6,$21
	move	$7,$22
	.set	noreorder
	.set	nomacro
	jal	ipt_do_table
	sw	$2,16($sp)
	.set	macro
	.set	reorder

$L256:
	lw	$2,140($18)
	#nop
	sw	$2,52($16)
	lw	$3,72($sp)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L261
	lui	$2,%hi(nf_ct_udp_timeout_stream)
	.set	macro
	.set	reorder

	lw	$5,160($sp)
	#nop
	lbu	$4,176($5)
	jal	tcp_get_timeouts_by_state
	.set	noreorder
	.set	nomacro
	j	$L346
	sw	$2,60($16)
	.set	macro
	.set	reorder

$L261:
	lw	$2,%lo(nf_ct_udp_timeout_stream)($2)
	#nop
	sw	$2,60($16)
$L346:
	lw	$2,160($sp)
	lw	$3,72($16)
	lw	$4,76($16)
	sw	$2,56($16)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L264
	sw	$2,40($16)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L265
	sw	$4,76($3)
	.set	macro
	.set	reorder

$L264:
	lui	$3,%hi(path_list_free)
	addiu	$2,$3,%lo(path_list_free)
	sw	$4,4($2)
$L265:
	lw	$4,76($16)
	lw	$2,72($16)
	lui	$3,%hi(path_list_free)
	addiu	$3,$3,%lo(path_list_free)
	sw	$2,0($4)
	lw	$2,8($3)
	lui	$4,%hi(path_list_inuse)
	addiu	$4,$4,%lo(path_list_inuse)
	addiu	$2,$2,-1
	sw	$2,8($3)
	sw	$0,72($16)
	lw	$2,4($4)
	addiu	$6,$16,72
	lui	$5,%hi(table_path)
	sw	$2,76($16)
	lw	$2,4($4)
	sll	$3,$fp,1
	addu	$3,$3,$fp
	sw	$16,0($2)
	lw	$2,8($4)
	sw	$6,4($4)
	sll	$3,$3,2
	addiu	$2,$2,1
	sw	$2,8($4)
	sw	$0,64($16)
	lw	$2,%lo(table_path)($5)
	addiu	$4,$16,64
	lw	$2,0($2)
	#nop
	addu	$2,$3,$2
	lw	$2,4($2)
	#nop
	sw	$2,68($16)
	lw	$2,%lo(table_path)($5)
	#nop
	lw	$2,0($2)
	#nop
	addu	$2,$3,$2
	lw	$2,4($2)
	#nop
	sw	$16,0($2)
	lw	$2,%lo(table_path)($5)
	#nop
	lw	$2,0($2)
	#nop
	addu	$2,$3,$2
	sw	$4,4($2)
	lw	$2,%lo(table_path)($5)
	#nop
	lw	$2,0($2)
	#nop
	addu	$3,$3,$2
	lw	$2,8($3)
	#nop
	addiu	$2,$2,1
	sw	$2,8($3)
$L228:
	lw	$5,20($17)
	lw	$6,12($17)
	lw	$4,24($17)
	srl	$2,$5,8
	srl	$3,$6,16
	xor	$2,$2,$5
	xor	$3,$3,$6
	lw	$5,16($17)
	xor	$2,$2,$3
	srl	$4,$4,4
	xor	$2,$2,$4
	lw	$3,72($sp)
	xor	$2,$2,$5
	lui	$5,%hi(path_table_list_max)
	lw	$4,%lo(path_table_list_max)($5)
	lui	$5,%hi(path_list_free)
	xor	$2,$2,$3
	lw	$16,%lo(path_list_free)($5)
	srl	$3,$2,12
	xor	$2,$2,$3
	addiu	$4,$4,-1
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L269
	and	$fp,$4,$2
	.set	macro
	.set	reorder

	lw	$2,0($17)
	#nop
	sw	$2,4($16)
	lw	$2,20($17)
	#nop
	sw	$2,16($16)
	lw	$2,24($17)
	#nop
	sw	$2,12($16)
	lw	$2,12($17)
	#nop
	sw	$2,20($16)
	lw	$2,16($17)
	#nop
	sw	$2,8($16)
	lw	$2,20($17)
	#nop
	sw	$2,24($16)
	lw	$2,24($17)
	#nop
	sw	$2,28($16)
	lw	$4,4($17)
	#nop
	sw	$4,32($16)
	lw	$2,8($17)
	.set	noreorder
	.set	nomacro
	jal	FastPath_Route
	sw	$2,36($16)
	.set	macro
	.set	reorder

	lw	$4,20($16)
	lw	$3,32($16)
	sw	$2,44($16)
	li	$2,2
	sb	$2,2($16)
	li	$2,-1
	sb	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$4,$3,$L270
	sb	$0,1($16)
	.set	macro
	.set	reorder

	li	$2,4			# 0x4
	sb	$2,1($16)
$L270:
	lw	$3,8($16)
	lw	$2,36($16)
	#nop
	beq	$3,$2,$L271
	lbu	$2,1($16)
	#nop
	ori	$2,$2,0x8
	sb	$2,1($16)
$L271:
	sw	$0,48($16)
	sw	$0,52($16)
	lw	$2,20($17)
	addiu	$19,$sp,40
	move	$4,$19
	sw	$2,12($20)
	lw	$2,24($17)
	move	$5,$0
	li	$6,14			# 0xe
	sh	$2,0($23)
	lw	$2,12($17)
	#nop
	sw	$2,16($20)
	lw	$2,16($17)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($23)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$22,$0,$L272
	move	$5,$22
	.set	macro
	.set	reorder

	lw	$4,20($17)
	.set	noreorder
	.set	nomacro
	jal	arp_req_get_ha
	move	$6,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L272
	addiu	$5,$sp,56
	.set	macro
	.set	reorder

	lw	$4,172($18)
	li	$6,2			# 0x2
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$4,$4,-2
	.set	macro
	.set	reorder

	bne	$2,$0,$L273
	lw	$3,156($18)
	#nop
	beq	$3,$0,$L273
	lbu	$2,12($3)
	lbu	$3,13($3)
	sll	$2,$2,8
	or	$3,$3,$2
	li	$2,2048			# 0x800
	beq	$3,$2,$L273
	lw	$2,172($18)
	#nop
	addiu	$2,$2,-22
	sw	$2,156($18)
$L273:
	lw	$4,156($18)
	move	$5,$19
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,6
	.set	macro
	.set	reorder

	lw	$4,156($18)
	addiu	$5,$22,320
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

$L272:
	sw	$0,140($18)
	lw	$3,100($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L282
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L282
	lui	$2,%hi(init_net+204)
	.set	macro
	.set	reorder

	lw	$2,%lo(init_net+204)($2)
	move	$4,$18
	move	$5,$0
	move	$6,$22
	move	$7,$21
	.set	noreorder
	.set	nomacro
	jal	ipt_do_table
	sw	$2,16($sp)
	.set	macro
	.set	reorder

$L282:
	lw	$2,140($18)
	addiu	$19,$sp,40
	move	$4,$19
	sw	$2,48($16)
	lw	$2,20($17)
	move	$5,$0
	li	$6,14			# 0xe
	sw	$2,12($20)
	lw	$2,24($17)
	#nop
	sh	$2,0($23)
	lw	$2,4($17)
	#nop
	sw	$2,16($20)
	lw	$2,8($17)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($23)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L287
	move	$5,$21
	.set	macro
	.set	reorder

	lw	$4,4($17)
	.set	noreorder
	.set	nomacro
	jal	arp_req_get_ha
	move	$6,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L287
	addiu	$5,$sp,56
	.set	macro
	.set	reorder

	lw	$4,172($18)
	li	$6,2			# 0x2
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$4,$4,-2
	.set	macro
	.set	reorder

	bne	$2,$0,$L288
	lw	$3,156($18)
	#nop
	beq	$3,$0,$L288
	lbu	$2,12($3)
	lbu	$3,13($3)
	sll	$2,$2,8
	or	$3,$3,$2
	li	$2,2048			# 0x800
	beq	$3,$2,$L288
	lw	$2,172($18)
	#nop
	addiu	$2,$2,-22
	sw	$2,156($18)
$L288:
	lw	$4,156($18)
	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$4,156($18)
	addiu	$5,$21,320
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,6
	.set	macro
	.set	reorder

$L287:
	sw	$0,140($18)
	lw	$5,100($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L297
	lui	$2,%hi(nf_hooks+160)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L297
	lui	$2,%hi(init_net+204)
	.set	macro
	.set	reorder

	lw	$2,%lo(init_net+204)($2)
	move	$4,$18
	li	$5,4			# 0x4
	move	$6,$22
	move	$7,$21
	.set	noreorder
	.set	nomacro
	jal	ipt_do_table
	sw	$2,16($sp)
	.set	macro
	.set	reorder

$L297:
	lw	$2,140($18)
	#nop
	sw	$2,52($16)
	lw	$3,72($sp)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L302
	lui	$2,%hi(nf_ct_udp_timeout_stream)
	.set	macro
	.set	reorder

	lw	$5,160($sp)
	#nop
	lbu	$4,176($5)
	jal	tcp_get_timeouts_by_state
	.set	noreorder
	.set	nomacro
	j	$L347
	sw	$2,60($16)
	.set	macro
	.set	reorder

$L302:
	lw	$2,%lo(nf_ct_udp_timeout_stream)($2)
	#nop
	sw	$2,60($16)
$L347:
	lw	$2,160($sp)
	lw	$3,72($16)
	lw	$4,76($16)
	sw	$2,56($16)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L305
	sw	$2,40($16)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L306
	sw	$4,76($3)
	.set	macro
	.set	reorder

$L305:
	lui	$3,%hi(path_list_free)
	addiu	$2,$3,%lo(path_list_free)
	sw	$4,4($2)
$L306:
	lw	$4,76($16)
	lw	$2,72($16)
	lui	$3,%hi(path_list_free)
	addiu	$3,$3,%lo(path_list_free)
	sw	$2,0($4)
	lw	$2,8($3)
	lui	$4,%hi(path_list_inuse)
	addiu	$4,$4,%lo(path_list_inuse)
	addiu	$2,$2,-1
	sw	$2,8($3)
	sw	$0,72($16)
	lw	$2,4($4)
	addiu	$6,$16,72
	lui	$5,%hi(table_path)
	sw	$2,76($16)
	lw	$2,4($4)
	sll	$3,$fp,1
	addu	$3,$3,$fp
	sw	$16,0($2)
	lw	$2,8($4)
	sw	$6,4($4)
	sll	$3,$3,2
	addiu	$2,$2,1
	sw	$2,8($4)
	sw	$0,64($16)
	lw	$2,%lo(table_path)($5)
	addiu	$4,$16,64
	lw	$2,0($2)
	#nop
	addu	$2,$3,$2
	lw	$2,4($2)
	#nop
	sw	$2,68($16)
	lw	$2,%lo(table_path)($5)
	#nop
	lw	$2,0($2)
	#nop
	addu	$2,$3,$2
	lw	$2,4($2)
	#nop
	sw	$16,0($2)
	lw	$2,%lo(table_path)($5)
	#nop
	lw	$2,0($2)
	#nop
	addu	$2,$3,$2
	sw	$4,4($2)
	lw	$2,%lo(table_path)($5)
	#nop
	lw	$2,0($2)
	#nop
	addu	$3,$3,$2
	lw	$2,8($3)
	#nop
	addiu	$2,$2,1
	sw	$2,8($3)
$L269:
	lw	$4,156($18)
	addiu	$5,$sp,24
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,6
	.set	macro
	.set	reorder

	lw	$4,156($18)
	addiu	$5,$sp,32
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$5,104($sp)
	#nop
	sw	$5,140($18)
	lw	$2,80($sp)
	#nop
	sw	$2,12($20)
	lw	$3,88($sp)
	#nop
	sh	$3,0($23)
	lw	$5,84($sp)
	#nop
	sw	$5,16($20)
	lw	$2,92($sp)
	#nop
	sh	$2,2($23)
	lw	$3,96($sp)
	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L313
	sw	$3,28($18)
	.set	macro
	.set	reorder

#APP
	raw_local_irq_save	$3
#NO_APP
	lw	$2,452($21)
	#nop
	addiu	$2,$2,-1
	sw	$2,452($21)
#APP
	raw_local_irq_restore	$3
#NO_APP
$L313:
	beq	$22,$0,$L317
#APP
	raw_local_irq_save	$3
#NO_APP
	lw	$2,452($22)
	#nop
	addiu	$2,$2,-1
	sw	$2,452($22)
#APP
	raw_local_irq_restore	$3
#NO_APP
$L317:
	lw	$5,76($sp)
	#nop
	xori	$2,$5,0x1
	andi	$2,$2,0x1
	beq	$2,$0,$L327
	.set	noreorder
	.set	nomacro
	j	$L345
	move	$2,$5
	.set	macro
	.set	reorder

$L327:
	lw	$2,76($sp)
	#nop
$L345:
#APP
	raw_local_irq_restore	$2
#NO_APP
	.set	noreorder
	.set	nomacro
	j	$L204
	move	$2,$0
	.set	macro
	.set	reorder

$L215:
	lw	$3,76($sp)
	#nop
	xori	$2,$3,0x1
	andi	$2,$2,0x1
	beq	$2,$0,$L336
	.set	noreorder
	.set	nomacro
	j	$L344
	move	$2,$3
	.set	macro
	.set	reorder

$L336:
	lw	$2,76($sp)
	#nop
$L344:
#APP
	raw_local_irq_restore	$2
#NO_APP
	li	$2,-1			# 0xffffffffffffffff
$L329:
$L204:
	lw	$31,148($sp)
	lw	$fp,144($sp)
	lw	$23,140($sp)
	lw	$22,136($sp)
	lw	$21,132($sp)
	lw	$20,128($sp)
	lw	$19,124($sp)
	lw	$18,120($sp)
	lw	$17,116($sp)
	lw	$16,112($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,152
	.set	macro
	.set	reorder

	.end	rtk_addNaptConnection
	.section	.text.rtk_delNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_delNaptConnection
	.ent	rtk_delNaptConnection
	.type	rtk_delNaptConnection, @function
rtk_delNaptConnection:
	.set	nomips16
	.frame	$sp,64,$31		# vars= 0, regs= 9/0, args= 24, gp= 0
	.mask	0x80ff0000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$23,52($sp)
	sw	$16,24($sp)
	sw	$31,56($sp)
	sw	$22,48($sp)
	sw	$21,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	lw	$2,0($4)
	li	$3,17			# 0x11
	li	$23,6			# 0x6
	xori	$2,$2,0x2
	move	$16,$4
	movn	$23,$3,$2		#RLX4181/RLX4281:conditional move  
#APP
	raw_local_irq_save	$17
#NO_APP
	lw	$4,20($4)
	jal	filter_delconnect
	lw	$22,0($16)
	lw	$21,4($16)
	lw	$20,8($16)
	lw	$19,20($16)
	lw	$18,24($16)
	andi	$2,$22,0xffff
	move	$5,$20
	move	$6,$19
	move	$7,$18
	sw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	jal	FastPath_Hash_NAPT_Entry
	move	$4,$21
	.set	macro
	.set	reorder

	lui	$10,%hi(table_napt)
	lw	$3,%lo(table_napt)($10)
	sll	$5,$2,1
	move	$7,$2
	lw	$3,0($3)
	addu	$2,$5,$2
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$6,0($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L408
	lui	$2,%hi(napt_list_inuse)
	.set	macro
	.set	reorder

	addiu	$9,$2,%lo(napt_list_inuse)
	lui	$2,%hi(napt_list_free)
	addiu	$8,$2,%lo(napt_list_free)
	lui	$24,%hi(path_list_inuse)
$L398:
	lw	$2,4($6)
	#nop
	bne	$2,$22,$L355
	lw	$2,8($6)
	#nop
	bne	$2,$21,$L355
	lw	$2,12($6)
	#nop
	bne	$2,$20,$L355
	lw	$3,16($6)
	lw	$2,12($16)
	#nop
	bne	$3,$2,$L355
	lw	$3,20($6)
	lw	$2,16($16)
	#nop
	bne	$3,$2,$L355
	lw	$2,24($6)
	#nop
	bne	$2,$19,$L355
	lw	$2,28($6)
	#nop
	bne	$2,$18,$L355
	lw	$2,36($6)
	sb	$0,0($6)
	lw	$4,40($6)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L358
	addu	$3,$5,$7
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L359
	sw	$4,40($2)
	.set	macro
	.set	reorder

$L358:
	lw	$2,%lo(table_napt)($10)
	sll	$3,$3,2
	lw	$2,0($2)
	#nop
	addu	$3,$3,$2
	sw	$4,4($3)
$L359:
	lw	$2,36($6)
	lw	$4,40($6)
	sll	$3,$7,1
	addu	$3,$3,$7
	sw	$2,0($4)
	lui	$2,%hi(table_napt)
	lw	$2,%lo(table_napt)($2)
	sll	$3,$3,2
	lw	$2,0($2)
	#nop
	addu	$3,$3,$2
	lw	$2,8($3)
	#nop
	addiu	$2,$2,-1
	sw	$2,8($3)
	lw	$2,44($6)
	lw	$3,48($6)
	beq	$2,$0,$L361
	.set	noreorder
	.set	nomacro
	j	$L362
	sw	$3,48($2)
	.set	macro
	.set	reorder

$L361:
	sw	$3,4($9)
$L362:
	lw	$3,48($6)
	lw	$2,44($6)
	addiu	$4,$6,44
	lui	$18,%hi(path_table_list_max)
	sw	$2,0($3)
	lw	$2,8($9)
	lui	$13,%hi(table_path)
	addiu	$2,$2,-1
	sw	$2,8($9)
	sw	$0,44($6)
	lw	$2,4($8)
	#nop
	sw	$2,48($6)
	lw	$2,4($8)
	#nop
	sw	$6,0($2)
	lw	$2,8($8)
	sw	$4,4($8)
	lw	$5,%lo(path_table_list_max)($18)
	addiu	$2,$2,1
	sw	$2,8($8)
	lw	$9,4($16)
	lw	$8,20($16)
	lw	$12,8($16)
	srl	$2,$9,8
	srl	$3,$8,16
	xor	$3,$3,$8
	xor	$2,$2,$9
	lw	$11,24($16)
	xor	$2,$2,$3
	srl	$3,$12,4
	xor	$2,$2,$3
	xor	$2,$2,$11
	xor	$2,$2,$23
	srl	$3,$2,12
	lw	$4,%lo(table_path)($13)
	xor	$2,$2,$3
	addiu	$5,$5,-1
	and	$7,$5,$2
	lw	$3,0($4)
	sll	$2,$7,1
	addu	$2,$2,$7
	sll	$2,$2,2
	addu	$3,$2,$3
	lw	$6,0($3)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L366
	move	$5,$2
	.set	macro
	.set	reorder

	lui	$2,%hi(path_list_free)
	addiu	$7,$2,%lo(path_list_free)
	move	$4,$5
	addiu	$10,$24,%lo(path_list_inuse)
	li	$14,1			# 0x1
	move	$15,$13
$L376:
	lw	$3,4($6)
	lw	$2,0($16)
	#nop
	bne	$3,$2,$L367
	lbu	$2,2($6)
	#nop
	bne	$2,$14,$L367
	lw	$2,16($6)
	#nop
	bne	$2,$9,$L367
	lw	$2,12($6)
	#nop
	bne	$2,$12,$L367
	lw	$3,24($6)
	lw	$2,12($16)
	#nop
	bne	$3,$2,$L367
	lw	$3,28($6)
	lw	$2,16($16)
	#nop
	bne	$3,$2,$L367
	lw	$2,20($6)
	#nop
	bne	$2,$8,$L367
	lw	$2,8($6)
	#nop
	bne	$2,$11,$L367
	lw	$2,64($6)
	sb	$0,0($6)
	lw	$3,68($6)
	beq	$2,$0,$L370
	.set	noreorder
	.set	nomacro
	j	$L371
	sw	$3,68($2)
	.set	macro
	.set	reorder

$L370:
	lw	$2,%lo(table_path)($13)
	#nop
	lw	$2,0($2)
	#nop
	addu	$2,$5,$2
	sw	$3,4($2)
$L371:
	lw	$3,68($6)
	lw	$2,64($6)
	#nop
	sw	$2,0($3)
	lw	$2,%lo(table_path)($15)
	#nop
	lw	$3,0($2)
	#nop
	addu	$3,$4,$3
	lw	$2,8($3)
	#nop
	addiu	$2,$2,-1
	sw	$2,8($3)
	lw	$3,72($6)
	lw	$2,76($6)
	beq	$3,$0,$L373
	.set	noreorder
	.set	nomacro
	j	$L374
	sw	$2,76($3)
	.set	macro
	.set	reorder

$L373:
	sw	$2,4($10)
$L374:
	lw	$2,72($6)
	lw	$3,76($6)
	addiu	$4,$6,72
	sw	$2,0($3)
	lw	$2,8($10)
	#nop
	addiu	$2,$2,-1
	sw	$2,8($10)
	sw	$0,72($6)
	lw	$2,4($7)
	#nop
	sw	$2,76($6)
	lw	$2,4($7)
	#nop
	sw	$6,0($2)
	lw	$2,8($7)
	sw	$4,4($7)
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	$L366
	sw	$2,8($7)
	.set	macro
	.set	reorder

$L367:
	lw	$6,64($6)
	#nop
	bne	$6,$0,$L376
$L366:
	lw	$9,20($16)
	lw	$8,12($16)
	lw	$13,24($16)
	srl	$2,$9,8
	srl	$3,$8,16
	xor	$3,$3,$8
	xor	$2,$2,$9
	lw	$12,16($16)
	xor	$2,$2,$3
	srl	$3,$13,4
	xor	$2,$2,$3
	xor	$2,$2,$12
	lw	$3,%lo(path_table_list_max)($18)
	xor	$2,$2,$23
	lui	$10,%hi(table_path)
	srl	$4,$2,12
	lw	$5,%lo(table_path)($10)
	xor	$2,$2,$4
	addiu	$3,$3,-1
	and	$7,$3,$2
	sll	$2,$7,1
	lw	$3,0($5)
	addu	$2,$2,$7
	sll	$2,$2,2
	addu	$3,$2,$3
	lw	$6,0($3)
	#nop
	beq	$6,$0,$L379
	move	$5,$2
	lui	$2,%hi(path_list_free)
	addiu	$11,$24,%lo(path_list_inuse)
	addiu	$7,$2,%lo(path_list_free)
	move	$4,$5
	li	$14,2			# 0x2
	move	$15,$10
$L389:
	lw	$3,4($6)
	lw	$2,0($16)
	#nop
	bne	$3,$2,$L380
	lbu	$2,2($6)
	#nop
	bne	$2,$14,$L380
	lw	$2,20($6)
	#nop
	bne	$2,$8,$L380
	lw	$2,8($6)
	#nop
	bne	$2,$12,$L380
	lw	$2,24($6)
	#nop
	bne	$2,$9,$L380
	lw	$2,28($6)
	#nop
	bne	$2,$13,$L380
	lw	$3,32($6)
	lw	$2,4($16)
	#nop
	bne	$3,$2,$L380
	lw	$3,36($6)
	lw	$2,8($16)
	#nop
	bne	$3,$2,$L380
	lw	$2,64($6)
	sb	$0,0($6)
	lw	$3,68($6)
	beq	$2,$0,$L383
	.set	noreorder
	.set	nomacro
	j	$L384
	sw	$3,68($2)
	.set	macro
	.set	reorder

$L383:
	lw	$2,%lo(table_path)($10)
	#nop
	lw	$2,0($2)
	#nop
	addu	$2,$5,$2
	sw	$3,4($2)
$L384:
	lw	$3,68($6)
	lw	$2,64($6)
	#nop
	sw	$2,0($3)
	lw	$2,%lo(table_path)($15)
	#nop
	lw	$3,0($2)
	#nop
	addu	$3,$4,$3
	lw	$2,8($3)
	#nop
	addiu	$2,$2,-1
	sw	$2,8($3)
	lw	$3,72($6)
	lw	$2,76($6)
	beq	$3,$0,$L386
	.set	noreorder
	.set	nomacro
	j	$L387
	sw	$2,76($3)
	.set	macro
	.set	reorder

$L386:
	sw	$2,4($11)
$L387:
	lw	$2,72($6)
	lw	$3,76($6)
	addiu	$4,$6,72
	sw	$2,0($3)
	lw	$2,8($11)
	#nop
	addiu	$2,$2,-1
	sw	$2,8($11)
	sw	$0,72($6)
	lw	$2,4($7)
	#nop
	sw	$2,76($6)
	lw	$2,4($7)
	#nop
	sw	$6,0($2)
	lw	$2,8($7)
	sw	$4,4($7)
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	$L379
	sw	$2,8($7)
	.set	macro
	.set	reorder

$L380:
	lw	$6,64($6)
	#nop
	bne	$6,$0,$L389
$L379:
#APP
	raw_local_irq_restore	$17
#NO_APP
	.set	noreorder
	.set	nomacro
	j	$L348
	move	$2,$0
	.set	macro
	.set	reorder

$L355:
	lw	$6,36($6)
	#nop
	bne	$6,$0,$L398
$L408:
#APP
	raw_local_irq_restore	$17
#NO_APP
	li	$2,-4			# 0xfffffffffffffffc
$L348:
	lw	$31,56($sp)
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

	.end	rtk_delNaptConnection
	.section	.text.rtk_idleNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_idleNaptConnection
	.ent	rtk_idleNaptConnection
	.type	rtk_idleNaptConnection, @function
rtk_idleNaptConnection:
	.set	nomips16
	.frame	$sp,8,$31		# vars= 0, regs= 1/0, args= 0, gp= 0
	.mask	0x00010000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-8
	sw	$16,0($sp)
	lw	$9,4($4)
	lw	$8,20($4)
	lw	$15,8($4)
	lw	$12,0($4)
	srl	$2,$9,8
	srl	$3,$8,16
	xor	$3,$3,$8
	lw	$11,24($4)
	xor	$2,$2,$9
	xor	$2,$2,$3
	srl	$3,$15,4
	xor	$2,$2,$3
	move	$7,$4
	li	$3,17			# 0x11
	xori	$4,$12,0x2
	li	$10,6			# 0x6
	lui	$16,%hi(path_table_list_max)
	movn	$10,$3,$4		#RLX4181/RLX4281:conditional move  
	xor	$2,$2,$11
	lw	$3,%lo(path_table_list_max)($16)
	xor	$2,$2,$10
	lui	$25,%hi(table_path)
	srl	$4,$2,12
	lw	$6,%lo(table_path)($25)
	xor	$2,$2,$4
	addiu	$3,$3,-1
	and	$4,$3,$2
	sll	$2,$4,1
	lw	$3,0($6)
	addu	$2,$2,$4
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$4,0($2)
	lui	$2,%hi(jiffies)
	move	$13,$5
	lw	$14,%lo(jiffies)($2)
	beq	$4,$0,$L416
	li	$2,-16777216			# 0xffffffffff000000

	ori	$24,$2,0x100
	ori	$5,$2,0xff00
	subu	$6,$14,$13
$L420:
	lw	$2,4($4)
	nop
	bne	$2,$12,$L417
	nop

	lw	$2,0($4)
	nop
	and	$2,$2,$5
	bne	$2,$24,$L417
	nop

	lw	$2,16($4)
	nop
	bne	$2,$9,$L417
	nop

	lw	$2,12($4)
	nop
	bne	$2,$15,$L417
	nop

	lw	$3,24($4)
	lw	$2,12($7)
	nop
	bne	$3,$2,$L417
	nop

	lw	$3,28($4)
	lw	$2,16($7)
	nop
	bne	$3,$2,$L417
	nop

	lw	$2,20($4)
	nop
	bne	$2,$8,$L417
	nop

	lw	$2,8($4)
	nop
	bne	$2,$11,$L417
	nop

	lw	$5,40($4)
	nop
	subu	$2,$6,$5
	bgez	$2,$L416
	nop

	j	$L430
	nop

$L417:
	lw	$4,64($4)
	nop
	bne	$4,$0,$L420
	nop

$L416:
	lw	$8,20($7)
	lw	$6,12($7)
	lw	$12,24($7)
	srl	$2,$8,8
	srl	$3,$6,16
	xor	$3,$3,$6
	xor	$2,$2,$8
	lw	$11,16($7)
	xor	$2,$2,$3
	srl	$3,$12,4
	xor	$2,$2,$3
	xor	$2,$2,$11
	lw	$4,%lo(path_table_list_max)($16)
	xor	$2,$2,$10
	srl	$3,$2,12
	lw	$5,%lo(table_path)($25)
	xor	$2,$2,$3
	addiu	$4,$4,-1
	and	$4,$4,$2
	sll	$2,$4,1
	lw	$3,0($5)
	addu	$2,$2,$4
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$4,0($2)
	nop
	beq	$4,$0,$L423
	li	$2,-16777216			# 0xffffffffff000000

	ori	$10,$2,0x200
	subu	$9,$14,$13
	ori	$5,$2,0xff00
$L427:
	lw	$3,4($4)
	lw	$2,0($7)
	nop
	bne	$3,$2,$L424
	nop

	lw	$2,0($4)
	nop
	and	$2,$2,$5
	bne	$2,$10,$L424
	nop

	lw	$2,20($4)
	nop
	bne	$2,$6,$L424
	nop

	lw	$2,8($4)
	nop
	bne	$2,$11,$L424
	nop

	lw	$2,24($4)
	nop
	bne	$2,$8,$L424
	nop

	lw	$2,28($4)
	nop
	bne	$2,$12,$L424
	nop

	lw	$3,32($4)
	lw	$2,4($7)
	nop
	bne	$3,$2,$L424
	nop

	lw	$3,36($4)
	lw	$2,8($7)
	nop
	bne	$3,$2,$L424
	nop

	lw	$5,40($4)
	nop
	subu	$2,$9,$5
	bgez	$2,$L423
	nop

$L430:
	lw	$3,56($4)
	addu	$2,$5,$13
	li	$4,-1			# 0xffffffffffffffff
	j	$L411
	sw	$2,116($3)

$L424:
	lw	$4,64($4)
	nop
	bne	$4,$0,$L427
	nop

$L423:
	move	$4,$0
$L411:
	lw	$16,0($sp)
	move	$2,$4
	j	$31
	addiu	$sp,$sp,8

	.set	macro
	.set	reorder
	.end	rtk_idleNaptConnection
	.section	.text.rtl_fpTimer_update,"ax",@progbits
	.align	2
	.globl	rtl_fpTimer_update
	.ent	rtl_fpTimer_update
	.type	rtl_fpTimer_update, @function
rtl_fpTimer_update:
	.set	nomips16
	.frame	$sp,64,$31		# vars= 32, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	sw	$16,48($sp)
	sw	$31,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	lbu	$3,50($4)
	lui	$2,%hi(jiffies)
	lw	$18,%lo(jiffies)($2)
	li	$2,17			# 0x11
	bne	$3,$2,$L432
	move	$16,$4

	lw	$2,100($4)
	nop
	andi	$2,$2,0x2
	beq	$2,$0,$L433
	li	$11,1			# 0x1

	lui	$2,%hi(nf_ct_udp_timeout_stream)
	lw	$17,%lo(nf_ct_udp_timeout_stream)($2)
	j	$L435
	nop

$L433:
	lui	$2,%hi(nf_ct_udp_timeout)
	lw	$17,%lo(nf_ct_udp_timeout)($2)
	j	$L435
	nop

$L432:
	li	$2,6			# 0x6
	bne	$3,$2,$L431
	li	$2,-1			# 0xffffffffffffffff

	lbu	$4,176($4)
	nop
	sltu	$2,$4,6
	beq	$2,$0,$L431
	li	$2,-1			# 0xffffffffffffffff

	jal	tcp_get_timeouts_by_state
	nop

	move	$17,$2
	li	$11,2			# 0x2
$L435:
	lw	$10,12($16)
	lw	$8,80($16)
	lw	$9,60($16)
	lw	$7,32($16)
	bne	$10,$8,$L438
	addiu	$4,$sp,16

	lhu	$2,76($16)
	lhu	$3,48($16)
	lhu	$6,28($16)
	move	$5,$17
	sw	$11,16($sp)
	sw	$9,20($sp)
	sw	$2,24($sp)
	sw	$7,28($sp)
	sw	$3,32($sp)
	j	$L446
	sw	$8,36($sp)

$L438:
	bne	$7,$9,$L431
	li	$2,-1			# 0xffffffffffffffff

	lhu	$2,28($16)
	lhu	$3,96($16)
	lhu	$6,76($16)
	addiu	$4,$sp,16
	move	$5,$17
	sw	$11,16($sp)
	sw	$10,20($sp)
	sw	$2,24($sp)
	sw	$8,28($sp)
	sw	$3,32($sp)
	sw	$7,36($sp)
$L446:
	jal	rtk_idleNaptConnection
	sw	$6,40($sp)

	beq	$2,$0,$L443
	addu	$5,$18,$17

	jal	rtl_check_for_acc
	move	$4,$16

	j	$L431
	move	$2,$0

$L443:
	li	$2,-1			# 0xffffffffffffffff
$L431:
	lw	$31,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,64

	.set	macro
	.set	reorder
	.end	rtl_fpTimer_update
	.section	.text.rtl_br_fdb_time_update,"ax",@progbits
	.align	2
	.globl	rtl_br_fdb_time_update
	.ent	rtl_br_fdb_time_update
	.type	rtl_br_fdb_time_update, @function
rtl_br_fdb_time_update:
	.set	nomips16
	.frame	$sp,40,$31		# vars= 8, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$16,24($sp)
	move	$4,$0
	move	$16,$5
	move	$5,$6
	li	$6,2			# 0x2
	sw	$31,32($sp)
	jal	rtl_get_hw_fdb_age
	sw	$17,28($sp)

	move	$3,$2
	li	$2,300			# 0x12c
	beq	$3,$2,$L450
	lui	$2,%hi(jiffies)

	sltu	$2,$3,301
	beq	$2,$0,$L453
	li	$2,450			# 0x1c2

	li	$2,150			# 0x96
	beq	$3,$2,$L451
	lui	$2,%hi(jiffies)

	j	$L448
	nop

$L453:
	bne	$3,$2,$L448
	lui	$2,%hi(jiffies)

	lw	$2,%lo(jiffies)($2)
	j	$L448
	sw	$2,24($16)

$L450:
	lw	$2,%lo(jiffies)($2)
	j	$L458
	addiu	$2,$2,-15000

$L451:
	lw	$2,%lo(jiffies)($2)
	nop
	addiu	$2,$2,-30000
$L458:
	sw	$2,24($16)
$L448:
	lbu	$2,1112($17)
	nop
	beq	$2,$0,$L455
	nop

	lw	$5,1080($17)
	j	$L457
	nop

$L455:
	lw	$5,1088($17)
$L457:
	lw	$2,24($16)
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	addu	$2,$2,$5
	lw	$31,32($sp)
	subu	$3,$3,$2
	lw	$17,28($sp)
	lw	$16,24($sp)
	slt	$3,$3,0
	li	$2,-1			# 0xffffffffffffffff
	movn	$2,$0,$3		#RLX4181/RLX4281:conditional move
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	rtl_br_fdb_time_update
	.rdata
	.align	2
$LC0:
	.ascii	"lo\000"
	.section	.text.enter_fast_path,"ax",@progbits
	.align	2
	.globl	enter_fast_path
	.ent	enter_fast_path
	.type	enter_fast_path, @function
enter_fast_path:
	.set	nomips16
	.frame	$sp,88,$31		# vars= 24, regs= 10/0, args= 24, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-88
	sw	$23,76($sp)
	sw	$21,68($sp)
	sw	$31,84($sp)
	sw	$fp,80($sp)
	sw	$22,72($sp)
	sw	$20,64($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	lw	$18,152($4)
	li	$2,1			# 0x1
	sw	$2,32($sp)
	lbu	$20,9($18)
	li	$2,6			# 0x6
	move	$21,$4
	sw	$0,36($sp)
	sw	$0,40($sp)
	.set	noreorder
	.set	nomacro
	beq	$20,$2,$L462
	move	$23,$0
	.set	macro
	.set	reorder

	li	$2,17			# 0x11
	.set	noreorder
	.set	nomacro
	bne	$20,$2,$L459
	move	$4,$0
	.set	macro
	.set	reorder

$L462:
	lbu	$2,0($18)
	move	$4,$18
	move	$6,$21
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$22,$18,$2
	.set	noreorder
	.set	nomacro
	jal	fast_path_pre_process_check
	move	$5,$22
	.set	macro
	.set	reorder

	move	$5,$2
	move	$4,$2
	li	$2,6			# 0x6
	bne	$5,$2,$L459
	lhu	$3,0($22)
	lhu	$16,6($18)
	lw	$19,12($18)
	lw	$fp,16($18)
	sw	$3,24($sp)
	lhu	$4,2($22)
	#nop
	sw	$4,28($sp)
	lw	$3,24($21)
	.set	noreorder
	.set	nomacro
	bne	$20,$5,$L464
	lui	$4,%hi($LC0)
	.set	macro
	.set	reorder

	addiu	$2,$4,%lo($LC0)
	move	$5,$3
#APP
	.set	noreorder
	.set	noat
	lbu	$3,($5)
1:	lbu	$1,($2)
	addiu	$5,1
	bne	$1,$3,2f
	addiu	$2,1
	bnez	$3,1b
	lbu	$3,($5)
	nop
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L459
	move	$4,$0
	.set	macro
	.set	reorder

	andi	$2,$16,0x3fff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L470
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	xori	$2,$16,0x2000
	.set	noreorder
	.set	nomacro
	j	$L470
	sltu	$3,$2,1
	.set	macro
	.set	reorder

$L640:
	lw	$5,24($4)
	lhu	$7,30($4)
	j	$L475
$L470:
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L471
	li	$2,65536			# 0x10000
	.set	macro
	.set	reorder

	lw	$3,12($22)
	#nop
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L472
	li	$2,393216			# 0x60000
	.set	macro
	.set	reorder

	lw	$4,24($sp)
	lw	$5,28($sp)
	srl	$2,$19,8
	srl	$3,$fp,16
	andi	$9,$4,0xffff
	xor	$3,$3,$fp
	xor	$2,$2,$19
	xor	$2,$2,$3
	srl	$3,$9,4
	xor	$2,$2,$3
	andi	$8,$5,0xffff
	lui	$10,%hi(path_table_list_max)
	andi	$6,$20,0xffff
	xor	$2,$2,$8
	lw	$3,%lo(path_table_list_max)($10)
	xor	$2,$2,$6
	lui	$11,%hi(table_path)
	srl	$4,$2,12
	lw	$5,%lo(table_path)($11)
	xor	$2,$2,$4
	addiu	$3,$3,-1
	and	$4,$3,$2
	sll	$2,$4,1
	lw	$3,0($5)
	addu	$2,$2,$4
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$4,0($2)
	#nop
	beq	$4,$0,$L482
	li	$3,255			# 0xff
$L480:
	lw	$2,12($4)
	#nop
	bne	$2,$9,$L476
	lw	$2,8($4)
	#nop
	bne	$2,$8,$L476
	lw	$2,16($4)
	#nop
	bne	$2,$19,$L476
	lw	$2,20($4)
	#nop
	bne	$2,$fp,$L476
	lbu	$2,0($4)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L476
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	lbu	$3,2($4)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L640
	sb	$0,0($4)
	.set	macro
	.set	reorder

	lw	$5,32($4)
	lhu	$7,38($4)
	j	$L475
$L476:
	lw	$4,64($4)
	#nop
	bne	$4,$0,$L480
	.set	noreorder
	.set	nomacro
	j	$L459
	move	$4,$0
	.set	macro
	.set	reorder

$L475:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L482
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	lbu	$3,2($4)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L483
	srl	$3,$19,16
	.set	macro
	.set	reorder

	srl	$2,$fp,8
	srl	$3,$5,16
	xor	$3,$3,$5
	xor	$2,$2,$fp
	xor	$2,$2,$3
	srl	$3,$8,4
	xor	$2,$2,$3
	.set	noreorder
	.set	nomacro
	j	$L646
	xor	$2,$2,$7
	.set	macro
	.set	reorder

$L483:
	srl	$2,$5,8
	xor	$3,$3,$19
	xor	$2,$2,$5
	xor	$2,$2,$3
	srl	$3,$7,4
	xor	$2,$2,$3
	xor	$2,$2,$9
$L646:
	lw	$4,%lo(path_table_list_max)($10)
	xor	$2,$2,$6
	srl	$3,$2,12
	xor	$2,$2,$3
	addiu	$4,$4,-1
	and	$4,$4,$2
	lw	$2,%lo(table_path)($11)
	sll	$3,$4,1
	addu	$3,$3,$4
	lw	$2,0($2)
	sll	$3,$3,2
	addu	$3,$3,$2
	lw	$4,0($3)
	#nop
	beq	$4,$0,$L482
	li	$3,255			# 0xff
$L491:
	lw	$2,24($4)
	#nop
	bne	$2,$fp,$L489
	lw	$2,28($4)
	#nop
	bne	$2,$8,$L489
	lw	$2,32($4)
	#nop
	bne	$2,$19,$L489
	lw	$2,36($4)
	#nop
	bne	$2,$9,$L489
	lbu	$2,0($4)
	#nop
	bne	$2,$3,$L489
	sb	$0,0($4)
$L489:
	lw	$4,64($4)
	#nop
	bne	$4,$0,$L491
$L482:
	.set	noreorder
	.set	nomacro
	j	$L459
	move	$4,$0
	.set	macro
	.set	reorder

$L472:
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L459
	move	$4,$0
	.set	macro
	.set	reorder

$L471:
	lw	$2,12($22)
	li	$3,1048576			# 0x100000
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L494
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$3,2($18)
	#nop
	bne	$3,$2,$L494
	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

	j	$L494
$L464:
	addiu	$2,$4,%lo($LC0)
	move	$5,$3
#APP
	.set	noreorder
	.set	noat
	lbu	$3,($5)
1:	lbu	$1,($2)
	addiu	$5,1
	bne	$1,$3,2f
	addiu	$2,1
	bnez	$3,1b
	lbu	$3,($5)
	nop
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L459
	move	$4,$0
	.set	macro
	.set	reorder

$L494:
	lhu	$2,6($18)
	#nop
	andi	$2,$2,0x3fff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L497
	li	$2,8192			# 0x2000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$2,$L498
	andi	$2,$16,0x2000
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L499
	sw	$2,36($sp)
	.set	macro
	.set	reorder

$L498:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L499
	andi	$3,$16,0x1fff
	.set	macro
	.set	reorder

	lw	$4,40($sp)
	li	$2,1			# 0x1
	movn	$4,$2,$3		#RLX4181/RLX4281:conditional move
	sw	$4,40($sp)
$L499:
	lw	$9,12($18)
	lw	$8,16($18)
	lhu	$7,4($18)
	andi	$6,$20,0x00ff
	xor	$3,$9,$8
	srl	$2,$3,16
	xor	$2,$2,$7
	xor	$3,$3,$2
	lui	$2,%hi(max_udp_frag_entry)
	lw	$5,%lo(max_udp_frag_entry)($2)
	srl	$4,$3,8
	lui	$2,%hi(udp_cache_table)
	xor	$4,$4,$6
	lw	$2,%lo(udp_cache_table)($2)
	xor	$3,$3,$4
	addiu	$5,$5,-1
	and	$3,$3,$5
	lw	$4,0($2)
	sll	$2,$3,1
	addu	$2,$2,$3
	sll	$2,$2,2
	addu	$2,$2,$4
	lw	$4,0($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L508
	move	$23,$0
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
$L507:
	lhu	$2,2($4)
	#nop
	bne	$2,$7,$L504
	lw	$2,4($4)
	#nop
	bne	$2,$9,$L504
	lw	$2,8($4)
	#nop
	bne	$2,$8,$L504
	lbu	$2,1($4)
	#nop
	bne	$2,$6,$L504
	lbu	$2,0($4)
	#nop
	beq	$2,$3,$L641
$L504:
	lw	$4,40($4)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L507
	move	$23,$0
	.set	macro
	.set	reorder

	j	$L508
$L641:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L508
	move	$23,$4
	.set	macro
	.set	reorder

	lhu	$5,12($4)
	lw	$3,36($sp)
	sw	$5,24($sp)
	lhu	$2,14($4)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L497
	sw	$2,28($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L497
	sw	$0,32($sp)
	.set	macro
	.set	reorder

$L508:
	lw	$5,36($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L459
	move	$4,$0
	.set	macro
	.set	reorder

$L497:
	lw	$4,24($sp)
	srl	$3,$fp,16
	srl	$2,$19,8
	sw	$3,44($sp)
	lw	$5,28($sp)
	xor	$3,$3,$fp
	xor	$2,$2,$19
	xor	$2,$2,$3
	srl	$3,$4,4
	xor	$2,$2,$3
	lui	$3,%hi(path_table_list_max)
	xor	$2,$2,$5
	lw	$4,%lo(path_table_list_max)($3)
	xor	$2,$2,$20
	lui	$3,%hi(table_path)
	lw	$5,%lo(table_path)($3)
	srl	$3,$2,12
	xor	$2,$2,$3
	addiu	$4,$4,-1
	and	$4,$4,$2
	lw	$3,0($5)
	sll	$2,$4,1
	addu	$2,$2,$4
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$16,0($2)
	#nop
$L647:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L639
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$2,12($16)
	lw	$3,24($sp)
	#nop
	bne	$2,$3,$L515
	lw	$2,8($16)
	lw	$4,28($sp)
	#nop
	bne	$2,$4,$L515
	lw	$2,16($16)
	#nop
	bne	$2,$19,$L515
	lw	$2,20($16)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$fp,$L515
	li	$2,255			# 0xff
	.set	macro
	.set	reorder

	lbu	$3,0($16)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L515
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

	lw	$3,4($16)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L657
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$20,$2,$L517
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L657:
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L515
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L515
$L517:
	lw	$5,32($16)
	lbu	$7,1($18)
	lw	$2,24($21)
	move	$4,$21
	move	$6,$19
	.set	noreorder
	.set	nomacro
	jal	ip_route_input
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L459
	move	$4,$0
	.set	macro
	.set	reorder

	lw	$3,28($21)
	#nop
	lw	$2,48($3)
	#nop
	bne	$2,$0,$L522
	lw	$2,44($3)
	#nop
	beq	$2,$0,$L654
$L522:
	lw	$2,72($3)
	lw	$3,84($21)
	#nop
	sltu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L654
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$20,$2,$L526
	addiu	$17,$22,16
	.set	macro
	.set	reorder

	lhu	$3,6($22)
	addiu	$2,$22,6
	move	$17,$0
	movn	$17,$2,$3		#RLX4181/RLX4281:conditional move  
$L526:
	lw	$5,36($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L529
	xor	$3,$19,$fp
	.set	macro
	.set	reorder

	lhu	$6,4($18)
	srl	$2,$3,16
	xor	$2,$2,$6
	xor	$3,$3,$2
	lui	$2,%hi(max_udp_frag_entry)
	lw	$4,%lo(max_udp_frag_entry)($2)
	andi	$5,$20,0x00ff
	srl	$2,$3,8
	xor	$2,$2,$5
	xor	$3,$3,$2
	addiu	$4,$4,-1
	lui	$8,%hi(udp_cache_list_free)
	and	$9,$3,$4
	lw	$2,24($sp)
	lw	$4,28($sp)
	lw	$7,%lo(udp_cache_list_free)($8)
	andi	$3,$2,0xffff
	.set	noreorder
	.set	nomacro
	bne	$7,$0,$L532
	andi	$2,$4,0xffff
	.set	macro
	.set	reorder

$L654:
	lw	$4,28($21)
	jal	dst_release
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L459
	sw	$0,28($21)
	.set	macro
	.set	reorder

$L532:
	lw	$4,48($7)
	sh	$2,14($7)
	li	$2,1
	sh	$3,12($7)
	sh	$6,2($7)
	sb	$5,1($7)
	sb	$2,0($7)
	sw	$19,4($7)
	sw	$fp,8($7)
	lw	$3,52($7)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L535
	addiu	$2,$8,%lo(udp_cache_list_free)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L536
	sw	$3,52($4)
	.set	macro
	.set	reorder

$L535:
	sw	$3,4($2)
$L536:
	lw	$3,52($7)
	lw	$2,48($7)
	addiu	$4,$8,%lo(udp_cache_list_free)
	lui	$5,%hi(udp_cache_table)
	sw	$2,0($3)
	lw	$2,8($4)
	sll	$3,$9,1
	addu	$3,$3,$9
	addiu	$2,$2,-1
	sw	$2,8($4)
	sw	$0,40($7)
	lw	$2,%lo(udp_cache_table)($5)
	sll	$3,$3,2
	addiu	$6,$7,40
	lw	$2,0($2)
	addiu	$4,$7,16
	addu	$2,$3,$2
	lw	$2,4($2)
	#nop
	sw	$2,44($7)
	lw	$2,%lo(udp_cache_table)($5)
	#nop
	lw	$2,0($2)
	#nop
	addu	$2,$3,$2
	lw	$2,4($2)
	#nop
	sw	$7,0($2)
	lw	$2,%lo(udp_cache_table)($5)
	#nop
	lw	$2,0($2)
	#nop
	addu	$2,$3,$2
	sw	$6,4($2)
	lw	$2,%lo(udp_cache_table)($5)
	#nop
	lw	$2,0($2)
	#nop
	addu	$3,$3,$2
	lw	$2,8($3)
	#nop
	addiu	$2,$2,1
	sw	$2,8($3)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	#nop
	addiu	$2,$2,1000
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$2,24($7)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L656
	move	$4,$21
	.set	macro
	.set	reorder

$L529:
	lw	$5,40($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L656
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	del_timer
	addiu	$4,$23,16
	.set	macro
	.set	reorder

	lw	$2,8($23)
	lw	$3,4($23)
	lhu	$4,2($23)
	lbu	$5,1($23)
	xor	$3,$3,$2
	srl	$2,$3,16
	xor	$2,$2,$4
	xor	$3,$3,$2
	lui	$2,%hi(max_udp_frag_entry)
	lw	$4,%lo(max_udp_frag_entry)($2)
	lw	$6,40($23)
	srl	$2,$3,8
	xor	$2,$2,$5
	addiu	$4,$4,-1
	xor	$3,$3,$2
	and	$5,$3,$4
	sb	$0,0($23)
	lw	$4,44($23)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L542
	lui	$2,%hi(udp_cache_table)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L543
	sw	$4,44($6)
	.set	macro
	.set	reorder

$L542:
	lw	$3,%lo(udp_cache_table)($2)
	sll	$2,$5,1
	addu	$2,$2,$5
	lw	$3,0($3)
	sll	$2,$2,2
	addu	$2,$2,$3
	sw	$4,4($2)
$L543:
	lw	$4,44($23)
	lw	$2,40($23)
	sll	$3,$5,1
	addu	$3,$3,$5
	sw	$2,0($4)
	lui	$2,%hi(udp_cache_table)
	lw	$2,%lo(udp_cache_table)($2)
	sll	$3,$3,2
	lui	$4,%hi(udp_cache_list_free)
	lw	$2,0($2)
	addiu	$4,$4,%lo(udp_cache_list_free)
	addiu	$5,$23,48
	addu	$3,$3,$2
	lw	$2,8($3)
	#nop
	addiu	$2,$2,-1
	sw	$2,8($3)
	sw	$0,48($23)
	lw	$2,4($4)
	#nop
	sw	$2,52($23)
	lw	$2,4($4)
	#nop
	sw	$23,0($2)
	lw	$2,8($4)
	sw	$5,4($4)
	addiu	$2,$2,1
	sw	$2,8($4)
	move	$4,$21
$L656:
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	fast_path_before_nat_check
	move	$6,$20
	.set	macro
	.set	reorder

	move	$5,$2
	move	$4,$2
	li	$2,6			# 0x6
	bne	$5,$2,$L459
	lbu	$3,1($16)
	#nop
	sltu	$2,$3,13
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L608
	sll	$2,$3,2
	.set	macro
	.set	reorder

	lui	$3,%hi($L631)
	addiu	$3,$3,%lo($L631)
	addu	$2,$2,$3
	lw	$2,0($2)
	#nop
	j	$2
	.rdata
	.align	2
$L631:
	.word	$L548
	.word	$L550
	.word	$L565
	.word	$L565
	.word	$L579
	.word	$L608
	.word	$L608
	.word	$L608
	.word	$L594
	.word	$L608
	.word	$L608
	.word	$L608
	.word	$L594
	.section	.text.enter_fast_path
$L550:
	lw	$2,24($16)
	srl	$3,$19,16
	andi	$6,$19,0xffff
	addu	$6,$6,$3
	lhu	$4,10($18)
	andi	$3,$2,0xffff
	subu	$6,$6,$3
	srl	$2,$2,16
	subu	$6,$6,$2
	addu	$6,$6,$4
	.set	noreorder
	.set	nomacro
	bgez	$6,$L552
	andi	$2,$6,0xffff
	.set	macro
	.set	reorder

	subu	$6,$0,$6
	sra	$2,$6,16
	andi	$3,$6,0xffff
	addu	$6,$2,$3
	sra	$2,$6,16
	addu	$6,$6,$2
	nor	$2,$0,$6
	.set	noreorder
	.set	nomacro
	j	$L551
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L552:
	sra	$3,$6,16
	addu	$6,$3,$2
	sra	$2,$6,16
	addu	$6,$6,$2
	sh	$6,10($18)
$L551:
	lw	$2,32($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L569
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L555
	beq	$17,$0,$L569
	lhu	$6,0($17)
	#nop
	beq	$6,$0,$L569
	lw	$2,12($18)
	lw	$3,24($16)
	srl	$4,$2,16
	andi	$5,$2,0xffff
	addu	$5,$5,$4
	andi	$2,$3,0xffff
	subu	$5,$5,$2
	srl	$3,$3,16
	subu	$5,$5,$3
	addu	$5,$5,$6
	.set	noreorder
	.set	nomacro
	bgez	$5,$L558
	sra	$3,$5,16
	.set	macro
	.set	reorder

	subu	$5,$0,$5
	sra	$2,$5,16
	andi	$3,$5,0xffff
	addu	$5,$2,$3
	sra	$2,$5,16
	addu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	j	$L648
	nor	$2,$0,$5
	.set	macro
	.set	reorder

$L558:
	andi	$2,$5,0xffff
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	j	$L569
	sh	$5,0($17)
	.set	macro
	.set	reorder

$L555:
	lw	$4,12($18)
	lw	$3,24($16)
	lhu	$5,0($17)
	srl	$2,$4,16
	andi	$4,$4,0xffff
	addu	$4,$4,$2
	andi	$2,$3,0xffff
	subu	$4,$4,$2
	srl	$3,$3,16
	subu	$4,$4,$3
	addu	$4,$4,$5
	.set	noreorder
	.set	nomacro
	bgez	$4,$L562
	andi	$2,$4,0xffff
	.set	macro
	.set	reorder

	subu	$4,$0,$4
	sra	$2,$4,16
	andi	$3,$4,0xffff
	addu	$4,$2,$3
	sra	$2,$4,16
	addu	$4,$4,$2
	nor	$2,$0,$4
$L648:
	.set	noreorder
	.set	nomacro
	j	$L569
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L562:
	sra	$3,$4,16
	addu	$4,$3,$2
	sra	$2,$4,16
	addu	$4,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L569
	sh	$4,0($17)
	.set	macro
	.set	reorder

$L565:
	lw	$2,24($16)
	srl	$3,$19,16
	andi	$6,$19,0xffff
	addu	$6,$6,$3
	lhu	$4,10($18)
	andi	$3,$2,0xffff
	subu	$6,$6,$3
	srl	$2,$2,16
	subu	$6,$6,$2
	addu	$6,$6,$4
	.set	noreorder
	.set	nomacro
	bgez	$6,$L567
	andi	$2,$6,0xffff
	.set	macro
	.set	reorder

	subu	$6,$0,$6
	sra	$2,$6,16
	andi	$3,$6,0xffff
	addu	$6,$2,$3
	sra	$2,$6,16
	addu	$6,$6,$2
	nor	$2,$0,$6
	.set	noreorder
	.set	nomacro
	j	$L566
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L567:
	sra	$3,$6,16
	addu	$6,$3,$2
	sra	$2,$6,16
	addu	$6,$6,$2
	sh	$6,10($18)
$L566:
	lw	$3,32($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L569
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L570
	beq	$17,$0,$L575
	lhu	$6,0($17)
	#nop
	beq	$6,$0,$L575
	lw	$4,12($18)
	lw	$3,24($16)
	lhu	$5,0($22)
	srl	$2,$4,16
	andi	$4,$4,0xffff
	addu	$4,$4,$2
	andi	$2,$3,0xffff
	subu	$4,$4,$2
	srl	$3,$3,16
	lw	$2,28($16)
	subu	$4,$4,$3
	addu	$4,$4,$5
	subu	$4,$4,$2
	addu	$4,$4,$6
	.set	noreorder
	.set	nomacro
	bgez	$4,$L573
	sra	$3,$4,16
	.set	macro
	.set	reorder

	subu	$4,$0,$4
	sra	$2,$4,16
	andi	$3,$4,0xffff
	addu	$4,$2,$3
	sra	$2,$4,16
	addu	$4,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L649
	nor	$2,$0,$4
	.set	macro
	.set	reorder

$L573:
	andi	$2,$4,0xffff
	addu	$4,$3,$2
	sra	$2,$4,16
	addu	$4,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L575
	sh	$4,0($17)
	.set	macro
	.set	reorder

$L570:
	lw	$4,12($18)
	lw	$3,24($16)
	lhu	$5,0($22)
	srl	$2,$4,16
	andi	$6,$4,0xffff
	addu	$6,$6,$2
	andi	$2,$3,0xffff
	subu	$6,$6,$2
	srl	$3,$3,16
	lw	$4,28($16)
	subu	$6,$6,$3
	lhu	$2,0($17)
	addu	$6,$6,$5
	subu	$6,$6,$4
	addu	$6,$6,$2
	.set	noreorder
	.set	nomacro
	bgez	$6,$L577
	andi	$2,$6,0xffff
	.set	macro
	.set	reorder

	subu	$6,$0,$6
	sra	$2,$6,16
	andi	$3,$6,0xffff
	addu	$6,$2,$3
	sra	$2,$6,16
	addu	$6,$6,$2
	nor	$2,$0,$6
$L649:
	.set	noreorder
	.set	nomacro
	j	$L575
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L577:
	sra	$3,$6,16
	addu	$6,$3,$2
	sra	$2,$6,16
	addu	$6,$6,$2
	sh	$6,0($17)
$L575:
	lw	$2,28($16)
	#nop
	sh	$2,0($22)
$L569:
	lw	$2,24($16)
	.set	noreorder
	.set	nomacro
	j	$L548
	sw	$2,12($18)
	.set	macro
	.set	reorder

$L579:
	lw	$2,32($16)
	lw	$4,44($sp)
	andi	$6,$fp,0xffff
	andi	$3,$2,0xffff
	addu	$5,$6,$4
	lhu	$4,10($18)
	subu	$5,$5,$3
	srl	$2,$2,16
	subu	$5,$5,$2
	addu	$5,$5,$4
	.set	noreorder
	.set	nomacro
	bgez	$5,$L581
	andi	$2,$5,0xffff
	.set	macro
	.set	reorder

	subu	$5,$0,$5
	sra	$2,$5,16
	andi	$3,$5,0xffff
	addu	$5,$2,$3
	sra	$2,$5,16
	addu	$5,$5,$2
	nor	$2,$0,$5
	.set	noreorder
	.set	nomacro
	j	$L580
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L581:
	sra	$3,$5,16
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	sh	$5,10($18)
$L580:
	lw	$5,32($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L653
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L584
	beq	$17,$0,$L653
	lhu	$5,0($17)
	#nop
	beq	$5,$0,$L653
	lw	$2,32($16)
	lw	$3,44($sp)
	#nop
	addu	$4,$6,$3
	andi	$3,$2,0xffff
	subu	$4,$4,$3
	srl	$2,$2,16
	subu	$4,$4,$2
	addu	$4,$4,$5
	.set	noreorder
	.set	nomacro
	bgez	$4,$L587
	sra	$3,$4,16
	.set	macro
	.set	reorder

	subu	$4,$0,$4
	sra	$2,$4,16
	andi	$3,$4,0xffff
	addu	$4,$2,$3
	sra	$2,$4,16
	addu	$4,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L650
	nor	$2,$0,$4
	.set	macro
	.set	reorder

$L587:
	andi	$2,$4,0xffff
	addu	$4,$3,$2
	sra	$2,$4,16
	addu	$4,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L653
	sh	$4,0($17)
	.set	macro
	.set	reorder

$L584:
	lw	$2,32($16)
	lw	$4,44($sp)
	andi	$3,$2,0xffff
	addu	$5,$6,$4
	lhu	$4,0($17)
	subu	$5,$5,$3
	srl	$2,$2,16
	subu	$5,$5,$2
	addu	$5,$5,$4
	.set	noreorder
	.set	nomacro
	bgez	$5,$L591
	andi	$2,$5,0xffff
	.set	macro
	.set	reorder

	subu	$5,$0,$5
	sra	$2,$5,16
	andi	$3,$5,0xffff
	addu	$5,$2,$3
	sra	$2,$5,16
	addu	$5,$5,$2
	nor	$2,$0,$5
$L650:
	.set	noreorder
	.set	nomacro
	j	$L653
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L591:
	sra	$3,$5,16
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	j	$L653
	sh	$5,0($17)
	.set	macro
	.set	reorder

$L594:
	lw	$2,32($16)
	lw	$3,44($sp)
	andi	$6,$fp,0xffff
	lhu	$4,10($18)
	addu	$5,$6,$3
	andi	$3,$2,0xffff
	subu	$5,$5,$3
	srl	$2,$2,16
	subu	$5,$5,$2
	addu	$5,$5,$4
	.set	noreorder
	.set	nomacro
	bgez	$5,$L596
	andi	$2,$5,0xffff
	.set	macro
	.set	reorder

	subu	$5,$0,$5
	sra	$2,$5,16
	andi	$3,$5,0xffff
	addu	$5,$2,$3
	sra	$2,$5,16
	addu	$5,$5,$2
	nor	$2,$0,$5
	.set	noreorder
	.set	nomacro
	j	$L595
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L596:
	sra	$3,$5,16
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	sh	$5,10($18)
$L595:
	lw	$4,32($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L653
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L599
	beq	$17,$0,$L604
	lhu	$7,0($17)
	#nop
	beq	$7,$0,$L604
	lw	$3,32($16)
	lw	$2,44($sp)
	lhu	$4,2($22)
	addu	$5,$6,$2
	andi	$2,$3,0xffff
	subu	$5,$5,$2
	srl	$3,$3,16
	lw	$2,36($16)
	subu	$5,$5,$3
	addu	$5,$5,$4
	subu	$5,$5,$2
	addu	$5,$5,$7
	.set	noreorder
	.set	nomacro
	bgez	$5,$L602
	sra	$3,$5,16
	.set	macro
	.set	reorder

	subu	$5,$0,$5
	sra	$2,$5,16
	andi	$3,$5,0xffff
	addu	$5,$2,$3
	sra	$2,$5,16
	addu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	j	$L651
	nor	$2,$0,$5
	.set	macro
	.set	reorder

$L602:
	andi	$2,$5,0xffff
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	j	$L604
	sh	$5,0($17)
	.set	macro
	.set	reorder

$L599:
	lw	$3,32($16)
	lw	$4,44($sp)
	lhu	$5,2($22)
	andi	$2,$3,0xffff
	addu	$6,$6,$4
	subu	$6,$6,$2
	srl	$3,$3,16
	lw	$4,36($16)
	subu	$6,$6,$3
	lhu	$2,0($17)
	addu	$6,$6,$5
	subu	$6,$6,$4
	addu	$6,$6,$2
	.set	noreorder
	.set	nomacro
	bgez	$6,$L606
	andi	$2,$6,0xffff
	.set	macro
	.set	reorder

	subu	$6,$0,$6
	sra	$2,$6,16
	andi	$3,$6,0xffff
	addu	$6,$2,$3
	sra	$2,$6,16
	addu	$6,$6,$2
	nor	$2,$0,$6
$L651:
	.set	noreorder
	.set	nomacro
	j	$L604
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L606:
	sra	$3,$6,16
	addu	$6,$3,$2
	sra	$2,$6,16
	addu	$6,$6,$2
	sh	$6,0($17)
$L604:
	lw	$2,36($16)
	.set	noreorder
	.set	nomacro
	j	$L653
	sh	$2,2($22)
	.set	macro
	.set	reorder

$L608:
	lw	$2,24($16)
	srl	$7,$19,16
	andi	$6,$19,0xffff
	andi	$3,$2,0xffff
	addu	$5,$6,$7
	lhu	$4,10($18)
	subu	$5,$5,$3
	srl	$2,$2,16
	subu	$5,$5,$2
	addu	$5,$5,$4
	.set	noreorder
	.set	nomacro
	bgez	$5,$L610
	andi	$2,$5,0xffff
	.set	macro
	.set	reorder

	subu	$5,$0,$5
	sra	$2,$5,16
	andi	$3,$5,0xffff
	addu	$5,$2,$3
	sra	$2,$5,16
	addu	$5,$5,$2
	nor	$2,$0,$5
	.set	noreorder
	.set	nomacro
	j	$L609
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L610:
	sra	$3,$5,16
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	sh	$5,10($18)
$L609:
	lw	$2,32($16)
	lw	$3,44($sp)
	andi	$8,$fp,0xffff
	lhu	$4,10($18)
	addu	$5,$8,$3
	andi	$3,$2,0xffff
	subu	$5,$5,$3
	srl	$2,$2,16
	subu	$5,$5,$2
	addu	$5,$5,$4
	.set	noreorder
	.set	nomacro
	bgez	$5,$L613
	andi	$2,$5,0xffff
	.set	macro
	.set	reorder

	subu	$5,$0,$5
	sra	$2,$5,16
	andi	$3,$5,0xffff
	addu	$5,$2,$3
	sra	$2,$5,16
	addu	$5,$5,$2
	nor	$2,$0,$5
	.set	noreorder
	.set	nomacro
	j	$L612
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L613:
	sra	$3,$5,16
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	sh	$5,10($18)
$L612:
	lw	$4,32($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L615
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L616
	beq	$17,$0,$L624
	lhu	$5,0($17)
	#nop
	beq	$5,$0,$L624
	lw	$3,24($16)
	addu	$6,$6,$7
	lhu	$4,0($22)
	andi	$2,$3,0xffff
	subu	$6,$6,$2
	srl	$3,$3,16
	lw	$2,28($16)
	subu	$6,$6,$3
	addu	$6,$6,$4
	subu	$6,$6,$2
	addu	$6,$6,$5
	.set	noreorder
	.set	nomacro
	bgez	$6,$L619
	andi	$2,$6,0xffff
	.set	macro
	.set	reorder

	subu	$6,$0,$6
	sra	$2,$6,16
	andi	$3,$6,0xffff
	addu	$6,$2,$3
	sra	$2,$6,16
	addu	$6,$6,$2
	nor	$2,$0,$6
	.set	noreorder
	.set	nomacro
	j	$L618
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L619:
	sra	$3,$6,16
	addu	$6,$3,$2
	sra	$2,$6,16
	addu	$6,$6,$2
	sh	$6,0($17)
$L618:
	lw	$5,44($sp)
	lw	$3,32($16)
	.set	noreorder
	.set	nomacro
	j	$L655
	addu	$6,$8,$5
	.set	macro
	.set	reorder

$L616:
	lw	$3,24($16)
	addu	$6,$6,$7
	lhu	$5,0($22)
	andi	$2,$3,0xffff
	subu	$6,$6,$2
	srl	$3,$3,16
	lw	$4,28($16)
	subu	$6,$6,$3
	lhu	$2,0($17)
	addu	$6,$6,$5
	subu	$6,$6,$4
	addu	$6,$6,$2
	.set	noreorder
	.set	nomacro
	bgez	$6,$L626
	andi	$2,$6,0xffff
	.set	macro
	.set	reorder

	subu	$6,$0,$6
	sra	$2,$6,16
	andi	$3,$6,0xffff
	addu	$6,$2,$3
	sra	$2,$6,16
	addu	$6,$6,$2
	nor	$2,$0,$6
	.set	noreorder
	.set	nomacro
	j	$L625
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L626:
	sra	$3,$6,16
	addu	$6,$3,$2
	sra	$2,$6,16
	addu	$6,$6,$2
	sh	$6,0($17)
$L625:
	lw	$2,44($sp)
	lw	$3,32($16)
	addu	$6,$8,$2
$L655:
	andi	$2,$3,0xffff
	lhu	$5,2($22)
	subu	$6,$6,$2
	srl	$3,$3,16
	lw	$4,36($16)
	subu	$6,$6,$3
	lhu	$2,0($17)
	addu	$6,$6,$5
	subu	$6,$6,$4
	addu	$6,$6,$2
	.set	noreorder
	.set	nomacro
	bgez	$6,$L629
	andi	$2,$6,0xffff
	.set	macro
	.set	reorder

	subu	$6,$0,$6
	sra	$2,$6,16
	andi	$3,$6,0xffff
	addu	$6,$2,$3
	sra	$2,$6,16
	addu	$6,$6,$2
	nor	$2,$0,$6
	.set	noreorder
	.set	nomacro
	j	$L624
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L629:
	sra	$3,$6,16
	addu	$6,$3,$2
	sra	$2,$6,16
	addu	$6,$6,$2
	sh	$6,0($17)
$L624:
	lw	$2,28($16)
	#nop
	sh	$2,0($22)
	lw	$2,36($16)
	#nop
	sh	$2,2($22)
$L615:
	lw	$2,24($16)
	#nop
	sw	$2,12($18)
$L653:
	lw	$2,32($16)
	#nop
	sw	$2,16($18)
$L548:
	lw	$3,104($21)
	li	$2,-805371904			# 0xffffffffcfff0000
	lw	$4,28($21)
	ori	$2,$2,0xffff
	and	$3,$3,$2
	sw	$3,104($21)
	lw	$2,12($4)
	#nop
	sw	$2,24($21)
	lw	$5,48($16)
	lw	$6,52($16)
	.set	noreorder
	.set	nomacro
	jal	fastpath_set_qos_mark
	move	$4,$21
	.set	macro
	.set	reorder

	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	move	$4,$18
	move	$5,$22
	sw	$2,40($16)
	.set	noreorder
	.set	nomacro
	jal	fast_path_post_process_xmit_check
	move	$6,$21
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L459
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	ip_finish_output3
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L459
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

$L515:
	lw	$16,64($16)
	j	$L647
$L639:
	lw	$2,24($21)
	move	$5,$22
	sw	$2,136($21)
	move	$6,$21
	lw	$31,84($sp)
	lw	$fp,80($sp)
	lw	$23,76($sp)
	lw	$22,72($sp)
	lw	$21,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	.set	noreorder
	.set	nomacro
	j	fast_path_post_process_return_check
	addiu	$sp,$sp,88
	.set	macro
	.set	reorder

$L459:
	lw	$31,84($sp)
	lw	$fp,80($sp)
	lw	$23,76($sp)
	lw	$22,72($sp)
	lw	$21,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	move	$2,$4
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,88
	.set	macro
	.set	reorder

	.end	enter_fast_path
	.rdata
	.align	2
$LC1:
	.ascii	"napt num: %d, path num: %d.\n\000"
	.section	.text.fastpath_dump_napt_entry_num,"ax",@progbits
	.align	2
	.globl	fastpath_dump_napt_entry_num
	.ent	fastpath_dump_napt_entry_num
	.type	fastpath_dump_napt_entry_num, @function
fastpath_dump_napt_entry_num:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$16,16($sp)
	sw	$31,20($sp)
	lui	$2,%hi(path_list_inuse)
	lw	$2,%lo(path_list_inuse)($2)
	move	$16,$5
	move	$7,$0
$L671:
	beq	$2,$0,$L668
	move	$6,$0

	lw	$2,72($2)
	j	$L671
	addiu	$7,$7,1

$L668:
	lui	$2,%hi(napt_list_inuse)
	lw	$2,%lo(napt_list_inuse)($2)
	nop
$L672:
	beq	$2,$0,$L670
	lui	$5,%hi($LC1)

	lw	$2,44($2)
	j	$L672
	addiu	$6,$6,1

$L670:
	addu	$4,$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC1)

	addu	$2,$16,$2
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fastpath_dump_napt_entry_num
	.section	.text.init_table_napt,"ax",@progbits
	.align	2
	.globl	init_table_napt
	.ent	init_table_napt
	.type	init_table_napt, @function
init_table_napt:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 16, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L678:
$L719:
	addiu	$sp,$sp,-56
	sw	$20,48($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	sw	$31,52($sp)
	sw	$19,44($sp)
	lui	$2,%hi(malloc_sizes+4)
	move	$17,$4
	lw	$4,%lo(malloc_sizes+4)($2)
	move	$20,$5
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	move	$16,$2
	lui	$2,%hi(table_napt)
	sw	$16,%lo(table_napt)($2)
	move	$18,$2
	beq	$16,$0,$L673
	li	$3,-1			# 0xffffffffffffffff

$L729:
$L770:
	lui	$2,%hi(napt_list_inuse)
	lui	$19,%hi(napt_list_free)
	sll	$4,$17,1
	addiu	$3,$2,%lo(napt_list_inuse)
	addiu	$6,$19,%lo(napt_list_free)
	addu	$4,$4,$17
	sw	$0,%lo(napt_list_inuse)($2)
	sw	$3,4($3)
	lui	$2,%hi(napt_table_list_max)
	sw	$0,8($3)
	sll	$4,$4,2
	li	$5,32			# 0x20
	sw	$6,4($6)
	sw	$0,8($6)
	sw	$17,%lo(napt_table_list_max)($2)
	jal	__kmalloc
	sw	$0,%lo(napt_list_free)($19)

	sw	$2,0($16)
	lw	$2,%lo(table_napt)($18)
	nop
	lw	$2,0($2)
	nop
	beq	$2,$0,$L673
	li	$3,-1			# 0xffffffffffffffff

	j	$L773
	move	$16,$0

$L837:
	j	$L673
	li	$3,-2			# 0xfffffffffffffffe

$L773:
	blez	$17,$L834
	nop

	move	$4,$0
$L778:
	lw	$2,%lo(table_napt)($18)
	addiu	$16,$16,1
	slt	$3,$16,$17
	lw	$2,0($2)
	nop
	addu	$2,$4,$2
	sw	$0,8($2)
	lw	$2,%lo(table_napt)($18)
	nop
	lw	$2,0($2)
	nop
	addu	$2,$4,$2
	sw	$0,0($2)
	lw	$2,%lo(table_napt)($18)
	nop
	lw	$2,0($2)
	nop
	addu	$2,$4,$2
	sw	$2,4($2)
	bne	$3,$0,$L778
	addiu	$4,$4,12

$L834:
	blez	$20,$L836
	move	$16,$0

$L832:
$L786:
$L827:
	lui	$2,%hi(malloc_sizes+12)
	lw	$4,%lo(malloc_sizes+12)($2)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$16,$16,1

	move	$3,$2
	addiu	$4,$19,%lo(napt_list_free)
	addiu	$5,$2,44
	beq	$2,$0,$L837
	slt	$6,$16,$20

	sw	$0,44($2)
	lw	$2,4($4)
	nop
	sw	$2,48($3)
	lw	$2,4($4)
	nop
	sw	$3,0($2)
	lw	$2,8($4)
	sw	$5,4($4)
	addiu	$2,$2,1
	bne	$6,$0,$L832
	sw	$2,8($4)

$L836:
	move	$3,$0
$L673:
	lw	$31,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	init_table_napt
	.section	.text.init_table_path,"ax",@progbits
	.align	2
	.globl	init_table_path
	.ent	init_table_path
	.type	init_table_path, @function
init_table_path:
	.set	nomips16
	.frame	$sp,56,$31		# vars= 16, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L843:
$L884:
	addiu	$sp,$sp,-56
	sw	$20,48($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	sw	$31,52($sp)
	sw	$19,44($sp)
	lui	$2,%hi(malloc_sizes+4)
	move	$17,$4
	lw	$4,%lo(malloc_sizes+4)($2)
	move	$20,$5
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	move	$16,$2
	lui	$2,%hi(table_path)
	sw	$16,%lo(table_path)($2)
	move	$18,$2
	beq	$16,$0,$L838
	li	$3,-1			# 0xffffffffffffffff

$L894:
$L935:
	lui	$2,%hi(path_list_inuse)
	lui	$19,%hi(path_list_free)
	sll	$4,$17,1
	addiu	$3,$2,%lo(path_list_inuse)
	addiu	$6,$19,%lo(path_list_free)
	addu	$4,$4,$17
	sw	$0,%lo(path_list_inuse)($2)
	sw	$3,4($3)
	lui	$2,%hi(path_table_list_max)
	sw	$0,8($3)
	sll	$4,$4,2
	li	$5,32			# 0x20
	sw	$6,4($6)
	sw	$0,8($6)
	sw	$17,%lo(path_table_list_max)($2)
	jal	__kmalloc
	sw	$0,%lo(path_list_free)($19)

	sw	$2,0($16)
	lw	$2,%lo(table_path)($18)
	nop
	lw	$2,0($2)
	nop
	beq	$2,$0,$L838
	li	$3,-1			# 0xffffffffffffffff

	j	$L938
	move	$16,$0

$L1002:
	j	$L838
	li	$3,-2			# 0xfffffffffffffffe

$L938:
	blez	$17,$L999
	nop

	move	$4,$0
$L943:
	lw	$2,%lo(table_path)($18)
	addiu	$16,$16,1
	slt	$3,$16,$17
	lw	$2,0($2)
	nop
	addu	$2,$4,$2
	sw	$0,8($2)
	lw	$2,%lo(table_path)($18)
	nop
	lw	$2,0($2)
	nop
	addu	$2,$4,$2
	sw	$0,0($2)
	lw	$2,%lo(table_path)($18)
	nop
	lw	$2,0($2)
	nop
	addu	$2,$4,$2
	sw	$2,4($2)
	bne	$3,$0,$L943
	addiu	$4,$4,12

$L999:
	blez	$20,$L1001
	move	$16,$0

$L997:
$L951:
$L992:
	lui	$2,%hi(malloc_sizes+20)
	lw	$4,%lo(malloc_sizes+20)($2)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$16,$16,1

	move	$3,$2
	addiu	$4,$19,%lo(path_list_free)
	addiu	$5,$2,72
	beq	$2,$0,$L1002
	slt	$6,$16,$20

	sw	$0,72($2)
	lw	$2,4($4)
	nop
	sw	$2,76($3)
	lw	$2,4($4)
	nop
	sw	$3,0($2)
	lw	$2,8($4)
	sw	$5,4($4)
	addiu	$2,$2,1
	bne	$6,$0,$L997
	sw	$2,8($4)

$L1001:
	move	$3,$0
$L838:
	lw	$31,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	init_table_path
	.globl	route_list_inuse
	.section	.bss
	.align	2
	.type	route_list_inuse, @object
	.size	route_list_inuse, 12
route_list_inuse:
	.space	12
	.globl	route_list_free
	.align	2
	.type	route_list_free, @object
	.size	route_list_free, 12
route_list_free:
	.space	12
	.globl	napt_list_inuse
	.align	2
	.type	napt_list_inuse, @object
	.size	napt_list_inuse, 12
napt_list_inuse:
	.space	12
	.globl	napt_list_free
	.align	2
	.type	napt_list_free, @object
	.size	napt_list_free, 12
napt_list_free:
	.space	12
	.globl	table_napt
	.align	2
	.type	table_napt, @object
	.size	table_napt, 4
table_napt:
	.space	4
	.local	napt_table_list_max
	.comm	napt_table_list_max,4,4
	.globl	path_list_inuse
	.align	2
	.type	path_list_inuse, @object
	.size	path_list_inuse, 12
path_list_inuse:
	.space	12
	.globl	path_list_free
	.align	2
	.type	path_list_free, @object
	.size	path_list_free, 12
path_list_free:
	.space	12
	.globl	table_path
	.align	2
	.type	table_path, @object
	.size	table_path, 4
table_path:
	.space	4
	.local	path_table_list_max
	.comm	path_table_list_max,4,4
	.globl	udp_cache_list_free
	.align	2
	.type	udp_cache_list_free, @object
	.size	udp_cache_list_free, 12
udp_cache_list_free:
	.space	12
	.globl	udp_cache_table
	.align	2
	.type	udp_cache_table, @object
	.size	udp_cache_table, 4
udp_cache_table:
	.space	4
	.local	max_udp_frag_entry
	.comm	max_udp_frag_entry,4,4
	.ident	"GCC: (GNU) 3.4.6-1.3.6"
