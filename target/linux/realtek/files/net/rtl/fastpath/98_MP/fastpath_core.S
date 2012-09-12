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
	.frame	$sp,48,$31		# vars= 8, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-48
	sw	$18,32($sp)
	move	$18,$5
	sw	$4,16($sp)
	lui	$4,%hi(arp_tbl)
	sw	$19,36($sp)
	addiu	$4,$4,%lo(arp_tbl)
	move	$19,$6
	addiu	$5,$sp,16
	move	$6,$18
	sw	$17,28($sp)
	sw	$16,24($sp)
	sw	$31,40($sp)
	.set	noreorder
	.set	nomacro
	jal	neigh_lookup
	li	$17,-6			# 0xfffffffa
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2
	move	$16,$2
	.set	macro
	.set	reorder

	jal	local_bh_disable
	move	$4,$19
	lbu	$6,256($18)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$16,36
	.set	macro
	.set	reorder

	jal	local_bh_enable
#APP
	   

	1:	ll	$3, 72($16)		# atomic_sub_return	
       nop                                             
	subu	$2, $3, 1				
	sc	$2, 72($16)					
	beqz	$2, 2f					
	subu	$2, $3, 1				
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
	move	$17,$0
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2
	move	$4,$16
	.set	macro
	.set	reorder

	jal	neigh_destroy
$L2:
	move	$2,$17
	lw	$31,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,48
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
	beq	$2,$0,$L19
	move	$7,$4

	lw	$2,8($4)
	lw	$3,4($4)
	lhu	$4,2($4)
	xor	$3,$3,$2
	srl	$2,$3,16
	xor	$2,$2,$4
	xor	$3,$3,$2
	lui	$2,%hi(max_udp_frag_entry)
	lbu	$5,1($7)
	srl	$4,$3,8
	lw	$2,%lo(max_udp_frag_entry)($2)
	xor	$4,$4,$5
	xor	$3,$3,$4
	addiu	$2,$2,-1
	and	$4,$3,$2
	sll	$2,$4,1
	lw	$6,40($7)
	addu	$2,$2,$4
	sb	$0,0($7)
	lw	$3,44($7)
	beq	$6,$0,$L15
	sll	$5,$2,2

	j	$L16
	sw	$3,44($6)

$L15:
	lui	$2,%hi(udp_cache_table)
	lw	$2,%lo(udp_cache_table)($2)
	lw	$2,0($2)
	addu	$2,$5,$2
	sw	$3,4($2)
$L16:
	lw	$2,40($7)
	sll	$3,$4,1
	lw	$5,44($7)
	addu	$3,$3,$4
	sll	$3,$3,2
	lui	$4,%hi(udp_cache_list_free)
	sw	$2,0($5)
	lui	$2,%hi(udp_cache_table)
	lw	$2,%lo(udp_cache_table)($2)
	addiu	$4,$4,%lo(udp_cache_list_free)
	addiu	$6,$7,48
	lw	$2,0($2)
	addu	$3,$3,$2
	lw	$2,8($3)
	addiu	$2,$2,-1
	sw	$2,8($3)
	sw	$0,48($7)
	lw	$2,4($4)
	sw	$2,52($7)
	lw	$2,4($4)
	sw	$7,0($2)
	lw	$2,8($4)
	sw	$6,4($4)
	addiu	$2,$2,1
	sw	$2,8($4)
$L19:
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
	.frame	$sp,56,$31		# vars= 16, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L25:
$L66:
	addiu	$sp,$sp,-56
	lui	$2,%hi(malloc_sizes+4)
	sw	$19,44($sp)
	move	$19,$4
	sw	$18,40($sp)
	li	$5,32			# 0x20
	sw	$17,36($sp)
	sw	$16,32($sp)
	lui	$16,%hi(udp_cache_table)
	sw	$31,52($sp)
	sw	$20,48($sp)
	lw	$4,%lo(malloc_sizes+4)($2)
	jal	kmem_cache_alloc
	move	$18,$16

	move	$3,$0
	move	$17,$2
	beq	$2,$0,$L20
	sw	$2,%lo(udp_cache_table)($16)

$L75:
$L116:
	lui	$20,%hi(udp_cache_list_free)
	sll	$4,$19,1
	addiu	$3,$20,%lo(udp_cache_list_free)
	addu	$4,$4,$19
	sw	$3,4($3)
	lui	$2,%hi(max_udp_frag_entry)
	sw	$0,8($3)
	sll	$4,$4,2
	li	$5,32			# 0x20
	sw	$19,%lo(max_udp_frag_entry)($2)
	jal	__kmalloc
	sw	$0,%lo(udp_cache_list_free)($20)

	sw	$2,0($17)
	lw	$2,%lo(udp_cache_table)($16)
	lw	$2,0($2)
	beq	$2,$0,$L20
	li	$3,-1			# 0xffffffff

	j	$L119
	slt	$5,$0,$19

$L183:
	j	$L20
	move	$3,$0

$L119:
	beq	$5,$0,$L182
	move	$17,$0

	move	$4,$0
$L124:
	lw	$2,%lo(udp_cache_table)($18)
	addiu	$17,$17,1
	slt	$3,$17,$19
	lw	$2,0($2)
	addu	$2,$4,$2
	sw	$0,8($2)
	lw	$2,%lo(udp_cache_table)($18)
	lw	$2,0($2)
	addu	$2,$4,$2
	sw	$0,0($2)
	lw	$2,%lo(udp_cache_table)($18)
	lw	$2,0($2)
	addu	$2,$4,$2
	addiu	$4,$4,12
	bne	$3,$0,$L124
	sw	$2,4($2)

	move	$17,$0
$L184:
	beq	$5,$0,$L182
	lui	$2,%hi(malloc_sizes+12)

$L132:
$L173:
	li	$5,32			# 0x20
	lw	$4,%lo(malloc_sizes+12)($2)
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	move	$5,$0
	move	$16,$2
	addiu	$4,$2,16
	beq	$2,$0,$L183
	move	$6,$0

	jal	init_timer_key
	nop

	lui	$2,%hi(cache_timeout)
	addiu	$2,$2,%lo(cache_timeout)
	addiu	$3,$20,%lo(udp_cache_list_free)
	sw	$2,28($16)
	addiu	$4,$16,48
	sw	$16,32($16)
	slt	$5,$17,$19
	sw	$0,48($16)
	lw	$2,4($3)
	sw	$2,52($16)
	lw	$2,4($3)
	sw	$16,0($2)
	lw	$2,8($3)
	sw	$4,4($3)
	addiu	$2,$2,1
	j	$L184
	sw	$2,8($3)

$L182:
	li	$3,1			# 0x1
$L20:
	lw	$31,52($sp)
	move	$2,$3
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
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
	lui	$2,%hi(napt_table_list_max)
	lhu	$4,18($sp)
	xor	$3,$3,$5
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
	li	$2,-4			# 0xfffffffc

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
	li	$2,-4			# 0xfffffffc

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
	move	$8,$0
	lw	$3,%lo(route_list_inuse)($2)
	move	$7,$0
$L202:
	beq	$3,$0,$L201
	nop

	lw	$5,8($3)
	sltu	$2,$5,$7
	bne	$2,$0,$L197
	and	$6,$4,$5

	lw	$2,4($3)
	bne	$6,$2,$L197
	nop

	move	$7,$5
	addiu	$8,$3,16
$L197:
	lw	$3,44($3)
	j	$L202
	nop

$L201:
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
	li	$2,-1			# 0xffffffff
	sw	$23,140($sp)
	move	$23,$7
	sw	$18,120($sp)
	move	$18,$5
	sw	$17,116($sp)
	move	$17,$4
	sw	$31,148($sp)
	sw	$fp,144($sp)
	sw	$22,136($sp)
	sw	$21,132($sp)
	sw	$20,128($sp)
	sw	$19,124($sp)
	sw	$16,112($sp)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L203
	sw	$6,160($sp)
	.set	macro
	.set	reorder

	lw	$3,0($4)
	li	$2,17			# 0x11
	lw	$4,20($4)
	li	$5,6			# 0x6
	xori	$3,$3,0x2
	movn	$5,$2,$3		#conditional move  
	.set	noreorder
	.set	nomacro
	jal	filter_addconnect
	sw	$5,72($sp)
	.set	macro
	.set	reorder

	lw	$22,0($17)
	lw	$21,4($17)
	lw	$20,8($17)
	andi	$2,$22,0xffff
	lw	$19,20($17)
	move	$4,$21
	lw	$16,24($17)
	move	$5,$20
	sw	$2,16($sp)
	move	$6,$19
	.set	noreorder
	.set	nomacro
	jal	FastPath_Hash_NAPT_Entry
	move	$7,$16
	.set	macro
	.set	reorder

	lui	$9,%hi(table_napt)
	lw	$3,%lo(table_napt)($9)
	sll	$7,$2,1
	move	$fp,$2
	addu	$2,$7,$2
	lw	$3,0($3)
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$4,0($2)
$L333:
	beq	$4,$0,$L331
	lw	$2,4($4)
	bne	$2,$22,$L209
	lw	$2,8($4)
	bne	$2,$21,$L209
	lw	$2,12($4)
	bne	$2,$20,$L209
	lw	$3,16($4)
	lw	$2,12($17)
	bne	$3,$2,$L209
	lw	$3,20($4)
	lw	$2,16($17)
	bne	$3,$2,$L209
	lw	$2,24($4)
	bne	$2,$19,$L209
	lw	$2,28($4)
	.set	noreorder
	.set	nomacro
	beq	$2,$16,$L323
	move	$2,$0
	.set	macro
	.set	reorder

$L209:
	lw	$4,36($4)
	j	$L333
$L331:
#APP
	raw_local_irq_save	$2
#NO_APP
	lui	$5,%hi(napt_list_free)
	lw	$8,%lo(napt_list_free)($5)
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L213
	sw	$2,76($sp)
	.set	macro
	.set	reorder

	lw	$2,0($17)
	lw	$4,44($8)
	sw	$2,4($8)
	lw	$2,4($17)
	lw	$3,48($8)
	sw	$2,8($8)
	lw	$2,8($17)
	sw	$2,12($8)
	lw	$2,12($17)
	sw	$2,16($8)
	lw	$2,16($17)
	sw	$2,20($8)
	lw	$2,20($17)
	sw	$2,24($8)
	lw	$2,24($17)
	sw	$23,32($8)
	sw	$2,28($8)
	li	$2,-1
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L215
	sb	$2,0($8)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L216
	sw	$3,48($4)
	.set	macro
	.set	reorder

$L215:
	addiu	$2,$5,%lo(napt_list_free)
	sw	$3,4($2)
$L216:
	lw	$4,48($8)
	addiu	$5,$5,%lo(napt_list_free)
	lw	$2,44($8)
	lui	$3,%hi(napt_list_inuse)
	addiu	$3,$3,%lo(napt_list_inuse)
	addiu	$6,$8,44
	sw	$2,0($4)
	addu	$4,$7,$fp
	lw	$2,8($5)
	sll	$4,$4,2
	addiu	$7,$8,36
	addiu	$2,$2,-1
	sw	$2,8($5)
	sw	$0,44($8)
	lw	$2,4($3)
	sw	$2,48($8)
	lw	$2,4($3)
	sw	$8,0($2)
	lw	$2,8($3)
	sw	$6,4($3)
	addiu	$2,$2,1
	sw	$2,8($3)
	sw	$0,36($8)
	lw	$2,%lo(table_napt)($9)
	lw	$2,0($2)
	addu	$2,$4,$2
	lw	$2,4($2)
	sw	$2,40($8)
	lw	$2,%lo(table_napt)($9)
	lw	$2,0($2)
	addu	$2,$4,$2
	lw	$2,4($2)
	sw	$8,0($2)
	lw	$2,%lo(table_napt)($9)
	lw	$2,0($2)
	addu	$2,$4,$2
	sw	$7,4($2)
	lw	$2,%lo(table_napt)($9)
	lw	$2,0($2)
	addu	$4,$4,$2
	lw	$2,8($4)
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
	move	$19,$2
	.set	macro
	.set	reorder

	addiu	$4,$sp,24
	lw	$22,152($18)
	move	$20,$2
	lw	$5,156($18)
	li	$6,6			# 0x6
	lbu	$2,0($22)
	lhu	$3,106($18)
	addiu	$5,$5,6
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$23,$22,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,100($sp)
	.set	macro
	.set	reorder

	addiu	$4,$sp,32
	lw	$5,156($18)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$4,12($22)
	sw	$4,80($sp)
	lhu	$5,0($23)
	sw	$5,88($sp)
	lw	$2,16($22)
	sw	$2,84($sp)
	lhu	$3,2($23)
	sw	$3,92($sp)
	lw	$4,28($18)
	sw	$4,96($sp)
	sw	$0,28($18)
	lw	$5,4($17)
	lw	$6,20($17)
	lw	$4,8($17)
	srl	$2,$5,8
	srl	$3,$6,16
	xor	$2,$2,$5
	xor	$3,$3,$6
	srl	$4,$4,4
	lw	$5,24($17)
	xor	$2,$2,$3
	xor	$2,$2,$4
	lui	$4,%hi(path_table_list_max)
	xor	$2,$2,$5
	lw	$5,72($sp)
	lw	$3,%lo(path_table_list_max)($4)
	xor	$2,$2,$5
	lui	$5,%hi(path_list_free)
	srl	$4,$2,12
	addiu	$3,$3,-1
	xor	$2,$2,$4
	and	$fp,$3,$2
	lw	$16,%lo(path_list_free)($5)
	lw	$2,140($18)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L226
	sw	$2,104($sp)
	.set	macro
	.set	reorder

	lw	$2,0($17)
	sw	$2,4($16)
	lw	$2,4($17)
	sw	$2,16($16)
	lw	$2,8($17)
	sw	$2,12($16)
	lw	$2,20($17)
	sw	$2,20($16)
	lw	$2,24($17)
	sw	$2,8($16)
	lw	$2,12($17)
	sw	$2,24($16)
	lw	$2,16($17)
	sw	$2,28($16)
	lw	$4,20($17)
	sw	$4,32($16)
	lw	$2,24($17)
	.set	noreorder
	.set	nomacro
	jal	FastPath_Route
	sw	$2,36($16)
	.set	macro
	.set	reorder

	sw	$2,44($16)
	li	$2,1
	lw	$4,16($16)
	lw	$3,24($16)
	sb	$2,2($16)
	li	$2,-1
	sb	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$4,$3,$L227
	sb	$0,1($16)
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	sb	$2,1($16)
$L227:
	lw	$3,12($16)
	lw	$2,28($16)
	beq	$3,$2,$L228
	lbu	$2,1($16)
	ori	$2,$2,0x2
	sb	$2,1($16)
$L228:
	sw	$0,48($16)
	addiu	$21,$sp,40
	sw	$0,52($16)
	move	$4,$21
	lw	$2,4($17)
	move	$5,$0
	li	$6,14			# 0xe
	sw	$2,12($22)
	lw	$2,8($17)
	sh	$2,0($23)
	lw	$2,20($17)
	sw	$2,16($22)
	lw	$2,24($17)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($23)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L229
	move	$5,$19
	.set	macro
	.set	reorder

	lw	$4,4($17)
	.set	noreorder
	.set	nomacro
	jal	arp_req_get_ha
	move	$6,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L229
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

	bne	$2,$0,$L230
	lw	$3,156($18)
	beq	$3,$0,$L230
	lbu	$2,12($3)
	lbu	$3,13($3)
	sll	$2,$2,8
	or	$3,$3,$2
	li	$2,2048			# 0x800
	beq	$3,$2,$L230
	lw	$2,172($18)
	addiu	$2,$2,-22
	sw	$2,156($18)
$L230:
	lw	$4,156($18)
	move	$5,$21
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,6
	.set	macro
	.set	reorder

	addiu	$5,$19,320
	lw	$4,156($18)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

$L229:
	sw	$0,140($18)
	li	$2,2048			# 0x800
	lw	$3,100($sp)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L239
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L239
	lui	$2,%hi(init_net+204)
	.set	macro
	.set	reorder

	move	$4,$18
	lw	$2,%lo(init_net+204)($2)
	move	$5,$0
	move	$6,$19
	move	$7,$20
	.set	noreorder
	.set	nomacro
	jal	ipt_do_table
	sw	$2,16($sp)
	.set	macro
	.set	reorder

$L239:
	lw	$2,140($18)
	addiu	$21,$sp,40
	move	$4,$21
	move	$5,$0
	sw	$2,48($16)
	li	$6,14			# 0xe
	lw	$2,12($17)
	sw	$2,12($22)
	lw	$2,16($17)
	sh	$2,0($23)
	lw	$2,20($17)
	sw	$2,16($22)
	lw	$2,24($17)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($23)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L244
	move	$5,$20
	.set	macro
	.set	reorder

	lw	$4,20($17)
	.set	noreorder
	.set	nomacro
	jal	arp_req_get_ha
	move	$6,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L244
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

	bne	$2,$0,$L245
	lw	$3,156($18)
	beq	$3,$0,$L245
	lbu	$2,12($3)
	lbu	$3,13($3)
	sll	$2,$2,8
	or	$3,$3,$2
	li	$2,2048			# 0x800
	beq	$3,$2,$L245
	lw	$2,172($18)
	addiu	$2,$2,-22
	sw	$2,156($18)
$L245:
	lw	$4,156($18)
	move	$5,$21
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	addiu	$5,$20,320
	lw	$4,156($18)
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,6
	.set	macro
	.set	reorder

$L244:
	sw	$0,140($18)
	li	$2,2048			# 0x800
	lw	$4,100($sp)
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L254
	lui	$2,%hi(nf_hooks+160)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L254
	lui	$2,%hi(init_net+204)
	.set	macro
	.set	reorder

	move	$4,$18
	lw	$2,%lo(init_net+204)($2)
	li	$5,4			# 0x4
	move	$6,$19
	move	$7,$20
	.set	noreorder
	.set	nomacro
	jal	ipt_do_table
	sw	$2,16($sp)
	.set	macro
	.set	reorder

$L254:
	lw	$2,140($18)
	sw	$2,52($16)
	li	$2,6			# 0x6
	lw	$5,72($sp)
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L259
	lui	$2,%hi(nf_ct_udp_timeout_stream)
	.set	macro
	.set	reorder

	lw	$2,160($sp)
	lbu	$4,176($2)
	jal	tcp_get_timeouts_by_state
	.set	noreorder
	.set	nomacro
	j	$L338
	sw	$2,60($16)
	.set	macro
	.set	reorder

$L259:
	lw	$2,%lo(nf_ct_udp_timeout_stream)($2)
	sw	$2,60($16)
$L338:
	lui	$2,%hi(jiffies)
	lw	$3,160($sp)
	lw	$4,76($16)
	sw	$3,56($16)
	lw	$2,%lo(jiffies)($2)
	lw	$3,72($16)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L262
	sw	$2,40($16)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L263
	sw	$4,76($3)
	.set	macro
	.set	reorder

$L262:
	lui	$5,%hi(path_list_free)
	addiu	$2,$5,%lo(path_list_free)
	sw	$4,4($2)
$L263:
	lw	$3,76($16)
	lui	$5,%hi(path_list_free)
	lw	$2,72($16)
	addiu	$5,$5,%lo(path_list_free)
	addiu	$7,$16,72
	lui	$6,%hi(table_path)
	sw	$2,0($3)
	lui	$3,%hi(path_list_inuse)
	lw	$2,8($5)
	addiu	$3,$3,%lo(path_list_inuse)
	sll	$4,$fp,1
	addiu	$8,$16,64
	addiu	$2,$2,-1
	addu	$4,$4,$fp
	sw	$2,8($5)
	sll	$4,$4,2
	sw	$0,72($16)
	lw	$2,4($3)
	sw	$2,76($16)
	lw	$2,4($3)
	sw	$16,0($2)
	lw	$2,8($3)
	sw	$7,4($3)
	addiu	$2,$2,1
	sw	$2,8($3)
	sw	$0,64($16)
	lw	$2,%lo(table_path)($6)
	lw	$2,0($2)
	addu	$2,$4,$2
	lw	$2,4($2)
	sw	$2,68($16)
	lw	$2,%lo(table_path)($6)
	lw	$2,0($2)
	addu	$2,$4,$2
	lw	$2,4($2)
	sw	$16,0($2)
	lw	$2,%lo(table_path)($6)
	lw	$2,0($2)
	addu	$2,$4,$2
	sw	$8,4($2)
	lw	$2,%lo(table_path)($6)
	lw	$2,0($2)
	addu	$4,$4,$2
	lw	$2,8($4)
	addiu	$2,$2,1
	sw	$2,8($4)
$L226:
	lw	$5,12($17)
	lw	$6,20($17)
	srl	$3,$5,16
	lw	$4,24($17)
	srl	$2,$6,8
	xor	$3,$3,$5
	xor	$2,$2,$6
	srl	$4,$4,4
	xor	$2,$2,$3
	lw	$5,16($17)
	xor	$2,$2,$4
	lui	$4,%hi(path_table_list_max)
	xor	$2,$2,$5
	lw	$5,72($sp)
	lw	$3,%lo(path_table_list_max)($4)
	xor	$2,$2,$5
	lui	$5,%hi(path_list_free)
	srl	$4,$2,12
	addiu	$3,$3,-1
	lw	$16,%lo(path_list_free)($5)
	xor	$2,$2,$4
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L267
	and	$fp,$3,$2
	.set	macro
	.set	reorder

	lw	$2,0($17)
	sw	$2,4($16)
	lw	$2,20($17)
	sw	$2,16($16)
	lw	$2,24($17)
	sw	$2,12($16)
	lw	$2,12($17)
	sw	$2,20($16)
	lw	$2,16($17)
	sw	$2,8($16)
	lw	$2,20($17)
	sw	$2,24($16)
	lw	$2,24($17)
	sw	$2,28($16)
	lw	$4,4($17)
	sw	$4,32($16)
	lw	$2,8($17)
	.set	noreorder
	.set	nomacro
	jal	FastPath_Route
	sw	$2,36($16)
	.set	macro
	.set	reorder

	sw	$2,44($16)
	li	$2,2
	lw	$4,20($16)
	lw	$3,32($16)
	sb	$2,2($16)
	li	$2,-1
	sb	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$4,$3,$L268
	sb	$0,1($16)
	.set	macro
	.set	reorder

	li	$2,4			# 0x4
	sb	$2,1($16)
$L268:
	lw	$3,8($16)
	lw	$2,36($16)
	beq	$3,$2,$L269
	lbu	$2,1($16)
	ori	$2,$2,0x8
	sb	$2,1($16)
$L269:
	sw	$0,48($16)
	addiu	$21,$sp,40
	sw	$0,52($16)
	move	$4,$21
	lw	$2,20($17)
	move	$5,$0
	li	$6,14			# 0xe
	sw	$2,12($22)
	lw	$2,24($17)
	sh	$2,0($23)
	lw	$2,12($17)
	sw	$2,16($22)
	lw	$2,16($17)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($23)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L270
	move	$5,$20
	.set	macro
	.set	reorder

	lw	$4,20($17)
	.set	noreorder
	.set	nomacro
	jal	arp_req_get_ha
	move	$6,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L270
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

	bne	$2,$0,$L271
	lw	$3,156($18)
	beq	$3,$0,$L271
	lbu	$2,12($3)
	lbu	$3,13($3)
	sll	$2,$2,8
	or	$3,$3,$2
	li	$2,2048			# 0x800
	beq	$3,$2,$L271
	lw	$2,172($18)
	addiu	$2,$2,-22
	sw	$2,156($18)
$L271:
	lw	$4,156($18)
	move	$5,$21
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,6
	.set	macro
	.set	reorder

	addiu	$5,$20,320
	lw	$4,156($18)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

$L270:
	sw	$0,140($18)
	li	$2,2048			# 0x800
	lw	$3,100($sp)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L280
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L280
	lui	$2,%hi(init_net+204)
	.set	macro
	.set	reorder

	move	$4,$18
	lw	$2,%lo(init_net+204)($2)
	move	$5,$0
	move	$6,$20
	move	$7,$19
	.set	noreorder
	.set	nomacro
	jal	ipt_do_table
	sw	$2,16($sp)
	.set	macro
	.set	reorder

$L280:
	lw	$2,140($18)
	addiu	$21,$sp,40
	move	$4,$21
	move	$5,$0
	sw	$2,48($16)
	li	$6,14			# 0xe
	lw	$2,20($17)
	sw	$2,12($22)
	lw	$2,24($17)
	sh	$2,0($23)
	lw	$2,4($17)
	sw	$2,16($22)
	lw	$2,8($17)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($23)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L285
	move	$5,$19
	.set	macro
	.set	reorder

	lw	$4,4($17)
	.set	noreorder
	.set	nomacro
	jal	arp_req_get_ha
	move	$6,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L285
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

	bne	$2,$0,$L286
	lw	$3,156($18)
	beq	$3,$0,$L286
	lbu	$2,12($3)
	lbu	$3,13($3)
	sll	$2,$2,8
	or	$3,$3,$2
	li	$2,2048			# 0x800
	beq	$3,$2,$L286
	lw	$2,172($18)
	addiu	$2,$2,-22
	sw	$2,156($18)
$L286:
	lw	$4,156($18)
	move	$5,$21
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	addiu	$5,$19,320
	lw	$4,156($18)
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,6
	.set	macro
	.set	reorder

$L285:
	sw	$0,140($18)
	li	$2,2048			# 0x800
	lw	$4,100($sp)
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L295
	lui	$2,%hi(nf_hooks+160)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L295
	lui	$2,%hi(init_net+204)
	.set	macro
	.set	reorder

	move	$4,$18
	lw	$2,%lo(init_net+204)($2)
	li	$5,4			# 0x4
	move	$6,$20
	move	$7,$19
	.set	noreorder
	.set	nomacro
	jal	ipt_do_table
	sw	$2,16($sp)
	.set	macro
	.set	reorder

$L295:
	lw	$2,140($18)
	sw	$2,52($16)
	li	$2,6			# 0x6
	lw	$5,72($sp)
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L300
	lui	$2,%hi(nf_ct_udp_timeout_stream)
	.set	macro
	.set	reorder

	lw	$2,160($sp)
	lbu	$4,176($2)
	jal	tcp_get_timeouts_by_state
	.set	noreorder
	.set	nomacro
	j	$L339
	sw	$2,60($16)
	.set	macro
	.set	reorder

$L300:
	lw	$2,%lo(nf_ct_udp_timeout_stream)($2)
	sw	$2,60($16)
$L339:
	lui	$2,%hi(jiffies)
	lw	$3,160($sp)
	lw	$4,76($16)
	sw	$3,56($16)
	lw	$2,%lo(jiffies)($2)
	lw	$3,72($16)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L303
	sw	$2,40($16)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L304
	sw	$4,76($3)
	.set	macro
	.set	reorder

$L303:
	lui	$5,%hi(path_list_free)
	addiu	$2,$5,%lo(path_list_free)
	sw	$4,4($2)
$L304:
	lw	$3,76($16)
	lui	$5,%hi(path_list_free)
	lw	$2,72($16)
	addiu	$5,$5,%lo(path_list_free)
	addiu	$7,$16,72
	lui	$6,%hi(table_path)
	sw	$2,0($3)
	lui	$3,%hi(path_list_inuse)
	lw	$2,8($5)
	addiu	$3,$3,%lo(path_list_inuse)
	sll	$4,$fp,1
	addiu	$8,$16,64
	addiu	$2,$2,-1
	addu	$4,$4,$fp
	sw	$2,8($5)
	sll	$4,$4,2
	sw	$0,72($16)
	lw	$2,4($3)
	sw	$2,76($16)
	lw	$2,4($3)
	sw	$16,0($2)
	lw	$2,8($3)
	sw	$7,4($3)
	addiu	$2,$2,1
	sw	$2,8($3)
	sw	$0,64($16)
	lw	$2,%lo(table_path)($6)
	lw	$2,0($2)
	addu	$2,$4,$2
	lw	$2,4($2)
	sw	$2,68($16)
	lw	$2,%lo(table_path)($6)
	lw	$2,0($2)
	addu	$2,$4,$2
	lw	$2,4($2)
	sw	$16,0($2)
	lw	$2,%lo(table_path)($6)
	lw	$2,0($2)
	addu	$2,$4,$2
	sw	$8,4($2)
	lw	$2,%lo(table_path)($6)
	lw	$2,0($2)
	addu	$4,$4,$2
	lw	$2,8($4)
	addiu	$2,$2,1
	sw	$2,8($4)
$L267:
	lw	$4,156($18)
	addiu	$5,$sp,24
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,6
	.set	macro
	.set	reorder

	addiu	$5,$sp,32
	lw	$4,156($18)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$2,104($sp)
	sw	$2,140($18)
	lw	$3,80($sp)
	sw	$3,12($22)
	lw	$4,88($sp)
	sh	$4,0($23)
	lw	$5,84($sp)
	sw	$5,16($22)
	lw	$2,92($sp)
	sh	$2,2($23)
	lw	$3,96($sp)
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L311
	sw	$3,28($18)
	.set	macro
	.set	reorder

#APP
	   

	1:	ll	    $2, 452($19)		# atomic_sub		
       nop                                             
	subu	$2, 1					
	sc	    $2, 452($19)					
	beqz	$2, 2f					
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
$L311:
	beq	$20,$0,$L314
#APP
	   

	1:	ll	    $2, 452($20)		# atomic_sub		
       nop                                             
	subu	$2, 1					
	sc	    $2, 452($20)					
	beqz	$2, 2f					
	.subsection 2					
2:	b	1b					
	.previous					

	   

#NO_APP
$L314:
	lw	$4,76($sp)
	xori	$2,$4,0x1
	andi	$2,$2,0x1
	beq	$2,$0,$L318
	.set	noreorder
	.set	nomacro
	j	$L337
	move	$2,$4
	.set	macro
	.set	reorder

$L318:
	lw	$2,76($sp)
$L337:
#APP
	raw_local_irq_restore	$2
#NO_APP
	.set	noreorder
	.set	nomacro
	j	$L203
	move	$2,$0
	.set	macro
	.set	reorder

$L213:
	lw	$5,76($sp)
	xori	$2,$5,0x1
	andi	$2,$2,0x1
	beq	$2,$0,$L325
	.set	noreorder
	.set	nomacro
	j	$L336
	move	$2,$5
	.set	macro
	.set	reorder

$L325:
	lw	$2,76($sp)
$L336:
#APP
	raw_local_irq_restore	$2
#NO_APP
	li	$2,-1			# 0xffffffff
$L323:
$L203:
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
	li	$3,17			# 0x11
	sw	$23,52($sp)
	li	$23,6			# 0x6
	sw	$16,24($sp)
	move	$16,$4
	sw	$31,56($sp)
	sw	$22,48($sp)
	sw	$21,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	lw	$2,0($4)
	xori	$2,$2,0x2
	movn	$23,$3,$2		#conditional move  
#APP
	raw_local_irq_save	$17
#NO_APP
	lw	$4,20($4)
	jal	filter_delconnect
	lw	$22,0($16)
	lw	$21,4($16)
	lw	$20,8($16)
	andi	$2,$22,0xffff
	lw	$19,20($16)
	move	$4,$21
	lw	$18,24($16)
	move	$5,$20
	move	$6,$19
	sw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	jal	FastPath_Hash_NAPT_Entry
	move	$7,$18
	.set	macro
	.set	reorder

	lui	$10,%hi(table_napt)
	lw	$3,%lo(table_napt)($10)
	sll	$8,$2,1
	move	$6,$2
	addu	$2,$8,$2
	lw	$3,0($3)
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$5,0($2)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L395
	lui	$2,%hi(napt_list_inuse)
	.set	macro
	.set	reorder

	lui	$24,%hi(path_list_inuse)
	addiu	$9,$2,%lo(napt_list_inuse)
	lui	$2,%hi(napt_list_free)
	addiu	$7,$2,%lo(napt_list_free)
$L387:
	lw	$2,4($5)
	bne	$2,$22,$L346
	lw	$2,8($5)
	bne	$2,$21,$L346
	lw	$2,12($5)
	bne	$2,$20,$L346
	lw	$3,16($5)
	lw	$2,12($16)
	bne	$3,$2,$L346
	lw	$3,20($5)
	lw	$2,16($16)
	bne	$3,$2,$L346
	lw	$2,24($5)
	bne	$2,$19,$L346
	lw	$2,28($5)
	bne	$2,$18,$L346
	lw	$2,36($5)
	sb	$0,0($5)
	lw	$4,40($5)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L349
	addu	$3,$8,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L350
	sw	$4,40($2)
	.set	macro
	.set	reorder

$L349:
	lw	$2,%lo(table_napt)($10)
	sll	$3,$3,2
	lw	$2,0($2)
	addu	$3,$3,$2
	sw	$4,4($3)
$L350:
	lw	$2,36($5)
	sll	$3,$6,1
	lw	$4,40($5)
	addu	$3,$3,$6
	sll	$3,$3,2
	sw	$2,0($4)
	lui	$2,%hi(table_napt)
	lw	$2,%lo(table_napt)($2)
	lw	$2,0($2)
	addu	$3,$3,$2
	lw	$2,8($3)
	addiu	$2,$2,-1
	sw	$2,8($3)
	lw	$2,44($5)
	lw	$3,48($5)
	beq	$2,$0,$L352
	.set	noreorder
	.set	nomacro
	j	$L353
	sw	$3,48($2)
	.set	macro
	.set	reorder

$L352:
	sw	$3,4($9)
$L353:
	lw	$3,48($5)
	addiu	$4,$5,44
	lw	$2,44($5)
	lui	$18,%hi(path_table_list_max)
	lui	$12,%hi(table_path)
	sw	$2,0($3)
	lw	$2,8($9)
	addiu	$2,$2,-1
	sw	$2,8($9)
	sw	$0,44($5)
	lw	$2,4($7)
	sw	$2,48($5)
	lw	$2,4($7)
	sw	$5,0($2)
	lw	$2,8($7)
	sw	$4,4($7)
	addiu	$2,$2,1
	sw	$2,8($7)
	lw	$8,4($16)
	lw	$7,20($16)
	lw	$11,8($16)
	srl	$2,$8,8
	srl	$3,$7,16
	xor	$2,$2,$8
	xor	$3,$3,$7
	srl	$4,$11,4
	xor	$2,$2,$3
	lw	$10,24($16)
	xor	$2,$2,$4
	lw	$4,%lo(path_table_list_max)($18)
	xor	$2,$2,$10
	xor	$2,$2,$23
	addiu	$4,$4,-1
	srl	$3,$2,12
	xor	$2,$2,$3
	and	$6,$4,$2
	lw	$3,%lo(table_path)($12)
	sll	$2,$6,1
	addu	$2,$2,$6
	lw	$3,0($3)
	sll	$2,$2,2
	addu	$3,$2,$3
	lw	$5,0($3)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L357
	move	$4,$2
	.set	macro
	.set	reorder

	lui	$2,%hi(path_list_free)
	addiu	$6,$2,%lo(path_list_free)
	move	$13,$4
	addiu	$9,$24,%lo(path_list_inuse)
	li	$14,1			# 0x1
	move	$15,$12
$L367:
	lw	$3,4($5)
	lw	$2,0($16)
	bne	$3,$2,$L358
	lbu	$2,2($5)
	bne	$2,$14,$L358
	lw	$2,16($5)
	bne	$2,$8,$L358
	lw	$2,12($5)
	bne	$2,$11,$L358
	lw	$3,24($5)
	lw	$2,12($16)
	bne	$3,$2,$L358
	lw	$3,28($5)
	lw	$2,16($16)
	bne	$3,$2,$L358
	lw	$2,20($5)
	bne	$2,$7,$L358
	lw	$2,8($5)
	bne	$2,$10,$L358
	lw	$2,64($5)
	sb	$0,0($5)
	lw	$3,68($5)
	beq	$2,$0,$L361
	.set	noreorder
	.set	nomacro
	j	$L362
	sw	$3,68($2)
	.set	macro
	.set	reorder

$L361:
	lw	$2,%lo(table_path)($12)
	lw	$2,0($2)
	addu	$2,$4,$2
	sw	$3,4($2)
$L362:
	lw	$3,68($5)
	lw	$2,64($5)
	sw	$2,0($3)
	lw	$2,%lo(table_path)($15)
	lw	$3,0($2)
	addu	$3,$13,$3
	lw	$2,8($3)
	addiu	$2,$2,-1
	sw	$2,8($3)
	lw	$3,72($5)
	lw	$2,76($5)
	beq	$3,$0,$L364
	.set	noreorder
	.set	nomacro
	j	$L365
	sw	$2,76($3)
	.set	macro
	.set	reorder

$L364:
	sw	$2,4($9)
$L365:
	lw	$2,72($5)
	addiu	$4,$5,72
	lw	$3,76($5)
	sw	$2,0($3)
	lw	$2,8($9)
	addiu	$2,$2,-1
	sw	$2,8($9)
	sw	$0,72($5)
	lw	$2,4($6)
	sw	$2,76($5)
	lw	$2,4($6)
	sw	$5,0($2)
	lw	$2,8($6)
	sw	$4,4($6)
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	$L357
	sw	$2,8($6)
	.set	macro
	.set	reorder

$L358:
	lw	$5,64($5)
	bne	$5,$0,$L367
$L357:
	lw	$8,20($16)
	lui	$10,%hi(table_path)
	lw	$7,12($16)
	lw	$12,24($16)
	srl	$2,$8,8
	srl	$3,$7,16
	xor	$2,$2,$8
	xor	$3,$3,$7
	srl	$4,$12,4
	xor	$2,$2,$3
	lw	$11,16($16)
	xor	$2,$2,$4
	lw	$4,%lo(path_table_list_max)($18)
	xor	$2,$2,$11
	xor	$2,$2,$23
	addiu	$4,$4,-1
	srl	$3,$2,12
	xor	$2,$2,$3
	and	$6,$4,$2
	lw	$3,%lo(table_path)($10)
	sll	$2,$6,1
	addu	$2,$2,$6
	lw	$3,0($3)
	sll	$2,$2,2
	addu	$3,$2,$3
	lw	$5,0($3)
	beq	$5,$0,$L370
	move	$4,$2
	lui	$2,%hi(path_list_free)
	addiu	$9,$24,%lo(path_list_inuse)
	addiu	$6,$2,%lo(path_list_free)
	move	$13,$4
	li	$14,2			# 0x2
	move	$15,$10
$L380:
	lw	$3,4($5)
	lw	$2,0($16)
	bne	$3,$2,$L371
	lbu	$2,2($5)
	bne	$2,$14,$L371
	lw	$2,20($5)
	bne	$2,$7,$L371
	lw	$2,8($5)
	bne	$2,$11,$L371
	lw	$2,24($5)
	bne	$2,$8,$L371
	lw	$2,28($5)
	bne	$2,$12,$L371
	lw	$3,32($5)
	lw	$2,4($16)
	bne	$3,$2,$L371
	lw	$3,36($5)
	lw	$2,8($16)
	bne	$3,$2,$L371
	lw	$2,64($5)
	sb	$0,0($5)
	lw	$3,68($5)
	beq	$2,$0,$L374
	.set	noreorder
	.set	nomacro
	j	$L375
	sw	$3,68($2)
	.set	macro
	.set	reorder

$L374:
	lw	$2,%lo(table_path)($10)
	lw	$2,0($2)
	addu	$2,$4,$2
	sw	$3,4($2)
$L375:
	lw	$3,68($5)
	lw	$2,64($5)
	sw	$2,0($3)
	lw	$2,%lo(table_path)($15)
	lw	$3,0($2)
	addu	$3,$13,$3
	lw	$2,8($3)
	addiu	$2,$2,-1
	sw	$2,8($3)
	lw	$3,72($5)
	lw	$2,76($5)
	beq	$3,$0,$L377
	.set	noreorder
	.set	nomacro
	j	$L378
	sw	$2,76($3)
	.set	macro
	.set	reorder

$L377:
	sw	$2,4($9)
$L378:
	lw	$2,72($5)
	addiu	$4,$5,72
	lw	$3,76($5)
	sw	$2,0($3)
	lw	$2,8($9)
	addiu	$2,$2,-1
	sw	$2,8($9)
	sw	$0,72($5)
	lw	$2,4($6)
	sw	$2,76($5)
	lw	$2,4($6)
	sw	$5,0($2)
	lw	$2,8($6)
	sw	$4,4($6)
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	$L370
	sw	$2,8($6)
	.set	macro
	.set	reorder

$L371:
	lw	$5,64($5)
	bne	$5,$0,$L380
$L370:
#APP
	raw_local_irq_restore	$17
#NO_APP
	.set	noreorder
	.set	nomacro
	j	$L340
	move	$2,$0
	.set	macro
	.set	reorder

$L346:
	lw	$5,36($5)
	bne	$5,$0,$L387
$L395:
#APP
	raw_local_irq_restore	$17
#NO_APP
	li	$2,-4			# 0xfffffffc
$L340:
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
	move	$7,$4
	sw	$16,0($sp)
	li	$11,6			# 0x6
	lw	$8,4($4)
	lui	$16,%hi(path_table_list_max)
	lw	$6,20($4)
	lui	$25,%hi(table_path)
	lw	$13,8($4)
	srl	$2,$8,8
	srl	$3,$6,16
	xor	$2,$2,$8
	xor	$3,$3,$6
	srl	$4,$13,4
	lw	$12,0($7)
	xor	$2,$2,$3
	lw	$10,24($7)
	xor	$2,$2,$4
	li	$3,17			# 0x11
	xori	$4,$12,0x2
	movn	$11,$3,$4		#conditional move  
	xor	$2,$2,$10
	xor	$2,$2,$11
	lw	$4,%lo(path_table_list_max)($16)
	srl	$3,$2,12
	xor	$2,$2,$3
	addiu	$4,$4,-1
	and	$4,$4,$2
	lw	$3,%lo(table_path)($25)
	sll	$2,$4,1
	addu	$2,$2,$4
	lw	$4,0($3)
	sll	$2,$2,2
	lui	$3,%hi(jiffies)
	addu	$2,$2,$4
	lw	$4,0($2)
	lw	$14,%lo(jiffies)($3)
	beq	$4,$0,$L403
	li	$2,-16777216			# 0xff000000

	subu	$9,$14,$5
	ori	$24,$2,0x100
	ori	$15,$2,0xff00
$L407:
	lw	$2,4($4)
	bne	$2,$12,$L404
	nop

	lw	$2,0($4)
	and	$2,$2,$15
	bne	$2,$24,$L404
	nop

	lw	$2,16($4)
	bne	$2,$8,$L404
	nop

	lw	$2,12($4)
	bne	$2,$13,$L404
	nop

	lw	$3,24($4)
	lw	$2,12($7)
	bne	$3,$2,$L404
	nop

	lw	$3,28($4)
	lw	$2,16($7)
	bne	$3,$2,$L404
	nop

	lw	$2,20($4)
	bne	$2,$6,$L404
	nop

	lw	$2,8($4)
	bne	$2,$10,$L404
	nop

	lw	$6,40($4)
	subu	$2,$9,$6
	bgez	$2,$L403
	nop

	j	$L417
	nop

$L404:
	lw	$4,64($4)
	bne	$4,$0,$L407
	nop

$L403:
	lw	$8,20($7)
	lw	$6,12($7)
	lw	$12,24($7)
	srl	$2,$8,8
	srl	$3,$6,16
	xor	$2,$2,$8
	xor	$3,$3,$6
	srl	$4,$12,4
	xor	$2,$2,$3
	lw	$10,16($7)
	xor	$2,$2,$4
	lw	$4,%lo(path_table_list_max)($16)
	xor	$2,$2,$10
	xor	$2,$2,$11
	addiu	$4,$4,-1
	srl	$3,$2,12
	xor	$2,$2,$3
	and	$4,$4,$2
	lw	$3,%lo(table_path)($25)
	sll	$2,$4,1
	addu	$2,$2,$4
	lw	$3,0($3)
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$4,0($2)
	beq	$4,$0,$L410
	li	$2,-16777216			# 0xff000000

	subu	$9,$14,$5
	ori	$13,$2,0x200
	ori	$11,$2,0xff00
$L414:
	lw	$3,4($4)
	lw	$2,0($7)
	bne	$3,$2,$L411
	nop

	lw	$2,0($4)
	and	$2,$2,$11
	bne	$2,$13,$L411
	nop

	lw	$2,20($4)
	bne	$2,$6,$L411
	nop

	lw	$2,8($4)
	bne	$2,$10,$L411
	nop

	lw	$2,24($4)
	bne	$2,$8,$L411
	nop

	lw	$2,28($4)
	bne	$2,$12,$L411
	nop

	lw	$3,32($4)
	lw	$2,4($7)
	bne	$3,$2,$L411
	nop

	lw	$3,36($4)
	lw	$2,8($7)
	bne	$3,$2,$L411
	nop

	lw	$6,40($4)
	subu	$2,$9,$6
	bgez	$2,$L410
	nop

$L417:
	lw	$3,56($4)
	addu	$2,$6,$5
	li	$4,-1			# 0xffffffff
	j	$L398
	sw	$2,116($3)

$L411:
	lw	$4,64($4)
	bne	$4,$0,$L414
	nop

$L410:
	move	$4,$0
$L398:
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
	.frame	$sp,72,$31		# vars= 32, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	lui	$2,%hi(jiffies)
	sw	$16,48($sp)
	move	$16,$4
	sw	$31,64($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	lbu	$3,50($4)
	lw	$19,%lo(jiffies)($2)
	li	$2,17			# 0x11
	bne	$3,$2,$L419
	li	$2,6			# 0x6

	lw	$2,100($4)
	andi	$2,$2,0x2
	beq	$2,$0,$L420
	li	$18,1			# 0x1

	lui	$2,%hi(nf_ct_udp_timeout_stream)
	lw	$17,%lo(nf_ct_udp_timeout_stream)($2)
	j	$L422
	nop

$L420:
	lui	$2,%hi(nf_ct_udp_timeout)
	lw	$17,%lo(nf_ct_udp_timeout)($2)
	j	$L422
	nop

$L419:
	bne	$3,$2,$L418
	li	$2,-1			# 0xffffffff

	lbu	$4,176($4)
	sltu	$2,$4,6
	beq	$2,$0,$L418
	li	$2,-1			# 0xffffffff

	jal	tcp_get_timeouts_by_state
	li	$18,2			# 0x2

	move	$17,$2
$L422:
	lw	$10,12($16)
	lw	$8,80($16)
	lw	$9,60($16)
	lw	$7,32($16)
	bne	$10,$8,$L425
	addiu	$4,$sp,16

	lhu	$2,76($16)
	lhu	$3,48($16)
	move	$5,$17
	lhu	$6,28($16)
	sw	$18,16($sp)
	sw	$9,20($sp)
	sw	$2,24($sp)
	sw	$7,28($sp)
	sw	$3,32($sp)
	j	$L433
	sw	$8,36($sp)

$L425:
	bne	$7,$9,$L418
	li	$2,-1			# 0xffffffff

	lhu	$2,28($16)
	addiu	$4,$sp,16
	lhu	$3,96($16)
	move	$5,$17
	lhu	$6,76($16)
	sw	$18,16($sp)
	sw	$10,20($sp)
	sw	$2,24($sp)
	sw	$8,28($sp)
	sw	$3,32($sp)
	sw	$7,36($sp)
$L433:
	jal	rtk_idleNaptConnection
	sw	$6,40($sp)

	beq	$2,$0,$L430
	addu	$5,$19,$17

	jal	rtl_check_for_acc
	move	$4,$16

	j	$L418
	move	$2,$0

$L430:
	li	$2,-1			# 0xffffffff
$L418:
	lw	$31,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,72

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
	move	$4,$0
	sw	$16,24($sp)
	move	$16,$5
	move	$5,$6
	li	$6,2			# 0x2
	sw	$31,32($sp)
	jal	rtl_get_hw_fdb_age
	sw	$17,28($sp)

	move	$3,$2
	li	$2,300			# 0x12c
	beq	$3,$2,$L437
	lui	$2,%hi(jiffies)

	sltu	$2,$3,301
	beq	$2,$0,$L440
	li	$2,450			# 0x1c2

	li	$2,150			# 0x96
	beq	$3,$2,$L438
	lui	$2,%hi(jiffies)

	j	$L435
	nop

$L440:
	bne	$3,$2,$L435
	lui	$2,%hi(jiffies)

	lw	$2,%lo(jiffies)($2)
	j	$L435
	sw	$2,24($16)

$L437:
	lw	$2,%lo(jiffies)($2)
	j	$L445
	addiu	$2,$2,-15000

$L438:
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,-30000
$L445:
	sw	$2,24($16)
$L435:
	lbu	$2,1112($17)
	beq	$2,$0,$L442
	nop

	lw	$5,1080($17)
	j	$L444
	nop

$L442:
	lw	$5,1088($17)
$L444:
	lw	$2,24($16)
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	addu	$2,$2,$5
	subu	$3,$3,$2
	li	$2,-1			# 0xffffffff
	slt	$3,$3,0
	lw	$31,32($sp)
	movn	$2,$0,$3		#conditional move
	lw	$17,28($sp)
	lw	$16,24($sp)
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
	li	$2,1			# 0x1
	sw	$23,76($sp)
	move	$23,$0
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
	sw	$2,32($sp)
	li	$2,6			# 0x6
	sw	$0,36($sp)
	sw	$0,40($sp)
	lbu	$20,9($18)
	.set	noreorder
	.set	nomacro
	beq	$20,$2,$L449
	move	$21,$4
	.set	macro
	.set	reorder

	li	$2,17			# 0x11
	.set	noreorder
	.set	nomacro
	bne	$20,$2,$L446
	move	$4,$0
	.set	macro
	.set	reorder

$L449:
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
	bne	$5,$2,$L446
	lhu	$3,0($22)
	lhu	$16,6($18)
	lw	$19,12($18)
	lw	$fp,16($18)
	sw	$3,24($sp)
	lhu	$7,2($22)
	sw	$7,28($sp)
	lw	$3,24($21)
	.set	noreorder
	.set	nomacro
	bne	$20,$5,$L451
	lui	$4,%hi($LC0)
	.set	macro
	.set	reorder

	addiu	$2,$4,%lo($LC0)
	move	$8,$3
#APP
	.set	noreorder
	.set	noat
	lbu	$3,($8)
1:	lbu	$1,($2)
	addiu	$8,1
	bne	$1,$3,2f
	addiu	$2,1
	bnez	$3,1b
	lbu	$3,($8)
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L446
	move	$4,$0
	.set	macro
	.set	reorder

	andi	$2,$16,0x3fff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L457
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	xori	$2,$16,0x2000
	.set	noreorder
	.set	nomacro
	j	$L457
	sltu	$3,$2,1
	.set	macro
	.set	reorder

$L627:
	lw	$5,24($4)
	lhu	$7,30($4)
	j	$L462
$L457:
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L458
	li	$2,65536			# 0x10000
	.set	macro
	.set	reorder

	lw	$3,12($22)
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L459
	li	$2,393216			# 0x60000
	.set	macro
	.set	reorder

	lw	$4,24($sp)
	srl	$2,$19,8
	srl	$3,$fp,16
	xor	$2,$2,$19
	xor	$3,$3,$fp
	andi	$9,$4,0xffff
	lw	$5,28($sp)
	srl	$4,$9,4
	xor	$2,$2,$3
	andi	$6,$20,0xffff
	xor	$2,$2,$4
	andi	$8,$5,0xffff
	xor	$2,$2,$8
	lui	$10,%hi(path_table_list_max)
	xor	$2,$2,$6
	lui	$11,%hi(table_path)
	lw	$3,%lo(path_table_list_max)($10)
	srl	$4,$2,12
	xor	$2,$2,$4
	addiu	$3,$3,-1
	and	$4,$3,$2
	lw	$3,%lo(table_path)($11)
	sll	$2,$4,1
	addu	$2,$2,$4
	lw	$3,0($3)
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$4,0($2)
	beq	$4,$0,$L469
	li	$3,255			# 0xff
$L467:
	lw	$2,12($4)
	bne	$2,$9,$L463
	lw	$2,8($4)
	bne	$2,$8,$L463
	lw	$2,16($4)
	bne	$2,$19,$L463
	lw	$2,20($4)
	bne	$2,$fp,$L463
	lbu	$2,0($4)
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L463
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	lbu	$3,2($4)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L627
	sb	$0,0($4)
	.set	macro
	.set	reorder

	lw	$5,32($4)
	lhu	$7,38($4)
	j	$L462
$L463:
	lw	$4,64($4)
	bne	$4,$0,$L467
	.set	noreorder
	.set	nomacro
	j	$L446
	move	$4,$0
	.set	macro
	.set	reorder

$L462:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L469
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	lbu	$3,2($4)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L470
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
	j	$L633
	xor	$2,$2,$7
	.set	macro
	.set	reorder

$L470:
	srl	$2,$5,8
	xor	$3,$3,$19
	xor	$2,$2,$5
	xor	$2,$2,$3
	srl	$3,$7,4
	xor	$2,$2,$3
	xor	$2,$2,$9
$L633:
	xor	$2,$2,$6
	lw	$3,%lo(path_table_list_max)($10)
	srl	$4,$2,12
	xor	$2,$2,$4
	addiu	$3,$3,-1
	and	$4,$3,$2
	lw	$2,%lo(table_path)($11)
	sll	$3,$4,1
	addu	$3,$3,$4
	lw	$2,0($2)
	sll	$3,$3,2
	addu	$3,$3,$2
	lw	$4,0($3)
	beq	$4,$0,$L469
	li	$3,255			# 0xff
$L478:
	lw	$2,24($4)
	bne	$2,$fp,$L476
	lw	$2,28($4)
	bne	$2,$8,$L476
	lw	$2,32($4)
	bne	$2,$19,$L476
	lw	$2,36($4)
	bne	$2,$9,$L476
	lbu	$2,0($4)
	bne	$2,$3,$L476
	sb	$0,0($4)
$L476:
	lw	$4,64($4)
	bne	$4,$0,$L478
$L469:
	.set	noreorder
	.set	nomacro
	j	$L446
	move	$4,$0
	.set	macro
	.set	reorder

$L459:
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L446
	move	$4,$0
	.set	macro
	.set	reorder

$L458:
	lw	$2,12($22)
	li	$3,1048576			# 0x100000
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L481
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$3,2($18)
	bne	$3,$2,$L481
	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

	j	$L481
$L451:
	addiu	$2,$4,%lo($LC0)
	move	$7,$3
#APP
	.set	noreorder
	.set	noat
	lbu	$3,($7)
1:	lbu	$1,($2)
	addiu	$7,1
	bne	$1,$3,2f
	addiu	$2,1
	bnez	$3,1b
	lbu	$3,($7)
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L446
	move	$4,$0
	.set	macro
	.set	reorder

$L481:
	lhu	$2,6($18)
	andi	$2,$2,0x3fff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L647
	srl	$3,$fp,16
	.set	macro
	.set	reorder

	li	$2,8192			# 0x2000
	.set	noreorder
	.set	nomacro
	bne	$16,$2,$L485
	andi	$2,$16,0x2000
	.set	macro
	.set	reorder

	li	$8,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L486
	sw	$8,36($sp)
	.set	macro
	.set	reorder

$L485:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L486
	andi	$3,$16,0x1fff
	.set	macro
	.set	reorder

	lw	$4,40($sp)
	li	$2,1			# 0x1
	movn	$4,$2,$3		#conditional move
	sw	$4,40($sp)
$L486:
	lw	$8,12($18)
	andi	$6,$20,0x00ff
	lw	$7,16($18)
	lhu	$5,4($18)
	xor	$3,$8,$7
	srl	$2,$3,16
	xor	$2,$2,$5
	xor	$3,$3,$2
	lui	$2,%hi(max_udp_frag_entry)
	lw	$4,%lo(max_udp_frag_entry)($2)
	srl	$2,$3,8
	xor	$2,$2,$6
	xor	$3,$3,$2
	addiu	$4,$4,-1
	lui	$2,%hi(udp_cache_table)
	and	$3,$3,$4
	lw	$4,%lo(udp_cache_table)($2)
	sll	$2,$3,1
	addu	$2,$2,$3
	lw	$3,0($4)
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$4,0($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L495
	move	$23,$0
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
$L494:
	lhu	$2,2($4)
	bne	$2,$5,$L491
	lw	$2,4($4)
	bne	$2,$8,$L491
	lw	$2,8($4)
	bne	$2,$7,$L491
	lbu	$2,1($4)
	bne	$2,$6,$L491
	lbu	$2,0($4)
	beq	$2,$3,$L628
$L491:
	lw	$4,40($4)
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L494
	move	$23,$0
	.set	macro
	.set	reorder

	j	$L495
$L628:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L495
	move	$23,$4
	.set	macro
	.set	reorder

	lhu	$5,12($4)
	lw	$8,36($sp)
	sw	$5,24($sp)
	lhu	$7,14($4)
	.set	noreorder
	.set	nomacro
	bne	$8,$0,$L484
	sw	$7,28($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L484
	sw	$0,32($sp)
	.set	macro
	.set	reorder

$L495:
	lw	$2,36($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L446
	move	$4,$0
	.set	macro
	.set	reorder

$L484:
	srl	$3,$fp,16
$L647:
	srl	$2,$19,8
	lw	$4,24($sp)
	xor	$2,$2,$19
	sw	$3,44($sp)
	xor	$3,$3,$fp
	xor	$2,$2,$3
	srl	$3,$4,4
	lw	$5,28($sp)
	xor	$2,$2,$3
	lui	$3,%hi(path_table_list_max)
	xor	$2,$2,$5
	xor	$2,$2,$20
	lw	$4,%lo(path_table_list_max)($3)
	srl	$3,$2,12
	xor	$2,$2,$3
	addiu	$4,$4,-1
	and	$4,$4,$2
	lui	$2,%hi(table_path)
	lw	$3,%lo(table_path)($2)
	sll	$2,$4,1
	addu	$2,$2,$4
	lw	$3,0($3)
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$16,0($2)
$L634:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L626
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$2,12($16)
	lw	$7,24($sp)
	bne	$2,$7,$L502
	lw	$2,8($16)
	lw	$8,28($sp)
	bne	$2,$8,$L502
	lw	$2,16($16)
	bne	$2,$19,$L502
	lw	$2,20($16)
	.set	noreorder
	.set	nomacro
	bne	$2,$fp,$L502
	li	$2,255			# 0xff
	.set	macro
	.set	reorder

	lbu	$3,0($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L502
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

	lw	$3,4($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L648
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$20,$2,$L504
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L648:
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L502
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L502
$L504:
	lw	$5,32($16)
	move	$4,$21
	lbu	$7,1($18)
	move	$6,$19
	lw	$2,24($21)
	.set	noreorder
	.set	nomacro
	jal	ip_route_input
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L446
	move	$4,$0
	.set	macro
	.set	reorder

	lw	$3,28($21)
	lw	$2,48($3)
	bne	$2,$0,$L509
	lw	$2,44($3)
	beq	$2,$0,$L641
$L509:
	lw	$2,72($3)
	lw	$3,84($21)
	sltu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L641
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$20,$2,$L513
	addiu	$17,$22,16
	.set	macro
	.set	reorder

	lhu	$3,6($22)
	addiu	$2,$22,6
	move	$17,$0
	movn	$17,$2,$3		#conditional move  
$L513:
	lw	$2,36($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L516
	xor	$3,$19,$fp
	.set	macro
	.set	reorder

	lui	$2,%hi(max_udp_frag_entry)
	lhu	$10,4($18)
	srl	$4,$3,16
	lw	$7,24($sp)
	andi	$9,$20,0x00ff
	xor	$4,$4,$10
	lui	$6,%hi(udp_cache_list_free)
	xor	$3,$3,$4
	andi	$4,$7,0xffff
	lw	$8,28($sp)
	lw	$5,%lo(max_udp_frag_entry)($2)
	srl	$2,$3,8
	xor	$2,$2,$9
	andi	$7,$8,0xffff
	lw	$8,%lo(udp_cache_list_free)($6)
	addiu	$5,$5,-1
	xor	$3,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$8,$0,$L519
	and	$11,$3,$5
	.set	macro
	.set	reorder

$L641:
	lw	$4,28($21)
	jal	dst_release
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L446
	sw	$0,28($21)
	.set	macro
	.set	reorder

$L519:
	lw	$5,48($8)
	li	$2,1
	sh	$10,2($8)
	sb	$9,1($8)
	sh	$4,12($8)
	sh	$7,14($8)
	sb	$2,0($8)
	sw	$19,4($8)
	sw	$fp,8($8)
	lw	$3,52($8)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L522
	addiu	$2,$6,%lo(udp_cache_list_free)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L523
	sw	$3,52($5)
	.set	macro
	.set	reorder

$L522:
	sw	$3,4($2)
$L523:
	lw	$4,52($8)
	addiu	$6,$6,%lo(udp_cache_list_free)
	lw	$2,48($8)
	lui	$5,%hi(udp_cache_table)
	sll	$3,$11,1
	addiu	$7,$8,40
	sw	$2,0($4)
	addu	$3,$3,$11
	lw	$2,8($6)
	sll	$3,$3,2
	addiu	$4,$8,16
	addiu	$2,$2,-1
	sw	$2,8($6)
	sw	$0,40($8)
	lw	$2,%lo(udp_cache_table)($5)
	lw	$2,0($2)
	addu	$2,$3,$2
	lw	$2,4($2)
	sw	$2,44($8)
	lw	$2,%lo(udp_cache_table)($5)
	lw	$2,0($2)
	addu	$2,$3,$2
	lw	$2,4($2)
	sw	$8,0($2)
	lw	$2,%lo(udp_cache_table)($5)
	lw	$2,0($2)
	addu	$2,$3,$2
	sw	$7,4($2)
	lw	$2,%lo(udp_cache_table)($5)
	lw	$2,0($2)
	addu	$3,$3,$2
	lw	$2,8($3)
	addiu	$2,$2,1
	sw	$2,8($3)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,1000
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$2,24($8)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L646
	move	$4,$21
	.set	macro
	.set	reorder

$L516:
	lw	$2,40($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L646
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
	xor	$3,$3,$2
	srl	$2,$3,16
	xor	$2,$2,$4
	xor	$3,$3,$2
	lui	$2,%hi(max_udp_frag_entry)
	lbu	$5,1($23)
	srl	$4,$3,8
	lw	$2,%lo(max_udp_frag_entry)($2)
	xor	$4,$4,$5
	lw	$6,40($23)
	xor	$3,$3,$4
	addiu	$2,$2,-1
	sb	$0,0($23)
	lw	$4,44($23)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L529
	and	$5,$3,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L530
	sw	$4,44($6)
	.set	macro
	.set	reorder

$L529:
	lui	$3,%hi(udp_cache_table)
	sll	$2,$5,1
	lw	$3,%lo(udp_cache_table)($3)
	addu	$2,$2,$5
	sll	$2,$2,2
	lw	$3,0($3)
	addu	$2,$2,$3
	sw	$4,4($2)
$L530:
	lw	$4,44($23)
	sll	$3,$5,1
	lw	$2,40($23)
	addu	$3,$3,$5
	sll	$3,$3,2
	addiu	$5,$23,48
	sw	$2,0($4)
	lui	$2,%hi(udp_cache_table)
	lw	$2,%lo(udp_cache_table)($2)
	lui	$4,%hi(udp_cache_list_free)
	addiu	$4,$4,%lo(udp_cache_list_free)
	lw	$2,0($2)
	addu	$3,$3,$2
	lw	$2,8($3)
	addiu	$2,$2,-1
	sw	$2,8($3)
	sw	$0,48($23)
	lw	$2,4($4)
	sw	$2,52($23)
	lw	$2,4($4)
	sw	$23,0($2)
	lw	$2,8($4)
	sw	$5,4($4)
	addiu	$2,$2,1
	sw	$2,8($4)
	move	$4,$21
$L646:
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
	bne	$5,$2,$L446
	lbu	$3,1($16)
	sltu	$2,$3,13
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L595
	sll	$2,$3,2
	.set	macro
	.set	reorder

	lui	$3,%hi($L618)
	addiu	$3,$3,%lo($L618)
	addu	$2,$2,$3
	lw	$2,0($2)
	j	$2
	.rdata
	.align	2
$L618:
	.word	$L535
	.word	$L537
	.word	$L552
	.word	$L552
	.word	$L566
	.word	$L595
	.word	$L595
	.word	$L595
	.word	$L581
	.word	$L595
	.word	$L595
	.word	$L595
	.word	$L581
	.section	.text.enter_fast_path
$L537:
	lw	$3,24($16)
	srl	$2,$19,16
	andi	$6,$19,0xffff
	addu	$6,$6,$2
	andi	$2,$3,0xffff
	subu	$6,$6,$2
	srl	$3,$3,16
	lhu	$2,10($18)
	subu	$6,$6,$3
	addu	$6,$6,$2
	.set	noreorder
	.set	nomacro
	bgez	$6,$L539
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
	j	$L538
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L539:
	sra	$3,$6,16
	addu	$6,$3,$2
	sra	$2,$6,16
	addu	$6,$6,$2
	sh	$6,10($18)
$L538:
	lw	$3,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L556
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L542
	beq	$17,$0,$L556
	lhu	$6,0($17)
	beq	$6,$0,$L556
	lw	$2,12($18)
	lw	$3,24($16)
	srl	$4,$2,16
	andi	$5,$2,0xffff
	addu	$5,$5,$4
	andi	$2,$3,0xffff
	subu	$5,$5,$2
	srl	$3,$3,16
	subu	$5,$5,$3
	.set	noreorder
	.set	nomacro
	j	$L642
	addu	$5,$5,$6
	.set	macro
	.set	reorder

$L542:
	lw	$2,12($18)
	lw	$3,24($16)
	srl	$4,$2,16
	andi	$5,$2,0xffff
	addu	$5,$5,$4
	andi	$2,$3,0xffff
	subu	$5,$5,$2
	srl	$3,$3,16
	lhu	$2,0($17)
	subu	$5,$5,$3
	addu	$5,$5,$2
$L642:
	.set	noreorder
	.set	nomacro
	bgez	$5,$L549
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
	j	$L556
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L549:
	sra	$3,$5,16
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	j	$L556
	sh	$5,0($17)
	.set	macro
	.set	reorder

$L552:
	lw	$3,24($16)
	srl	$2,$19,16
	andi	$6,$19,0xffff
	addu	$6,$6,$2
	andi	$2,$3,0xffff
	subu	$6,$6,$2
	srl	$3,$3,16
	lhu	$2,10($18)
	subu	$6,$6,$3
	addu	$6,$6,$2
	.set	noreorder
	.set	nomacro
	bgez	$6,$L554
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
	j	$L553
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L554:
	sra	$3,$6,16
	addu	$6,$3,$2
	sra	$2,$6,16
	addu	$6,$6,$2
	sh	$6,10($18)
$L553:
	lw	$4,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L556
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L557
	beq	$17,$0,$L562
	lhu	$6,0($17)
	beq	$6,$0,$L562
	lw	$2,12($18)
	lw	$3,24($16)
	srl	$4,$2,16
	andi	$5,$2,0xffff
	addu	$5,$5,$4
	andi	$2,$3,0xffff
	subu	$5,$5,$2
	srl	$3,$3,16
	lhu	$4,0($22)
	subu	$5,$5,$3
	lw	$2,28($16)
	addu	$5,$5,$4
	subu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	j	$L643
	addu	$5,$5,$6
	.set	macro
	.set	reorder

$L557:
	lw	$2,12($18)
	lw	$3,24($16)
	srl	$4,$2,16
	andi	$5,$2,0xffff
	addu	$5,$5,$4
	andi	$2,$3,0xffff
	subu	$5,$5,$2
	srl	$3,$3,16
	lhu	$2,0($22)
	subu	$5,$5,$3
	lw	$3,28($16)
	addu	$5,$5,$2
	lhu	$2,0($17)
	subu	$5,$5,$3
	addu	$5,$5,$2
$L643:
	.set	noreorder
	.set	nomacro
	bgez	$5,$L564
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
	j	$L562
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L564:
	sra	$3,$5,16
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	sh	$5,0($17)
$L562:
	lw	$2,28($16)
	sh	$2,0($22)
$L556:
	lw	$2,24($16)
	.set	noreorder
	.set	nomacro
	j	$L535
	sw	$2,12($18)
	.set	macro
	.set	reorder

$L566:
	lw	$2,32($16)
	andi	$6,$fp,0xffff
	lw	$5,44($sp)
	andi	$3,$2,0xffff
	srl	$2,$2,16
	addu	$4,$6,$5
	subu	$4,$4,$3
	lhu	$3,10($18)
	subu	$4,$4,$2
	addu	$4,$4,$3
	.set	noreorder
	.set	nomacro
	bgez	$4,$L568
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
	.set	noreorder
	.set	nomacro
	j	$L567
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L568:
	sra	$3,$4,16
	addu	$4,$3,$2
	sra	$2,$4,16
	addu	$4,$4,$2
	sh	$4,10($18)
$L567:
	lw	$7,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L640
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L571
	beq	$17,$0,$L640
	lhu	$5,0($17)
	beq	$5,$0,$L640
	lw	$2,32($16)
	lw	$8,44($sp)
	andi	$3,$2,0xffff
	srl	$2,$2,16
	addu	$4,$6,$8
	subu	$4,$4,$3
	subu	$4,$4,$2
	addu	$4,$4,$5
	.set	noreorder
	.set	nomacro
	bgez	$4,$L574
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
	j	$L637
	nor	$2,$0,$4
	.set	macro
	.set	reorder

$L574:
	andi	$2,$4,0xffff
	addu	$4,$3,$2
	sra	$2,$4,16
	addu	$4,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L640
	sh	$4,0($17)
	.set	macro
	.set	reorder

$L571:
	lw	$2,32($16)
	lw	$3,44($sp)
	lhu	$4,0($17)
	addu	$5,$6,$3
	andi	$3,$2,0xffff
	subu	$5,$5,$3
	srl	$2,$2,16
	subu	$5,$5,$2
	addu	$5,$5,$4
	.set	noreorder
	.set	nomacro
	bgez	$5,$L578
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
$L637:
	.set	noreorder
	.set	nomacro
	j	$L640
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L578:
	sra	$3,$5,16
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	j	$L640
	sh	$5,0($17)
	.set	macro
	.set	reorder

$L581:
	lw	$2,32($16)
	andi	$5,$fp,0xffff
	lw	$7,44($sp)
	andi	$3,$2,0xffff
	srl	$2,$2,16
	addu	$4,$5,$7
	subu	$4,$4,$3
	lhu	$3,10($18)
	subu	$4,$4,$2
	addu	$4,$4,$3
	.set	noreorder
	.set	nomacro
	bgez	$4,$L583
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
	.set	noreorder
	.set	nomacro
	j	$L582
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L583:
	sra	$3,$4,16
	addu	$4,$3,$2
	sra	$2,$4,16
	addu	$4,$4,$2
	sh	$4,10($18)
$L582:
	lw	$8,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L640
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L586
	beq	$17,$0,$L591
	lhu	$6,0($17)
	beq	$6,$0,$L591
	lw	$2,32($16)
	lw	$3,44($sp)
	lhu	$4,2($22)
	addu	$5,$5,$3
	andi	$3,$2,0xffff
	subu	$5,$5,$3
	srl	$2,$2,16
	subu	$5,$5,$2
	lw	$2,36($16)
	addu	$5,$5,$4
	subu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	j	$L644
	addu	$5,$5,$6
	.set	macro
	.set	reorder

$L586:
	lw	$2,32($16)
	lw	$4,44($sp)
	andi	$3,$2,0xffff
	srl	$2,$2,16
	addu	$5,$5,$4
	subu	$5,$5,$3
	lhu	$4,2($22)
	subu	$5,$5,$2
	lw	$3,36($16)
	addu	$5,$5,$4
	lhu	$2,0($17)
	subu	$5,$5,$3
	addu	$5,$5,$2
$L644:
	.set	noreorder
	.set	nomacro
	bgez	$5,$L593
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
	j	$L591
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L593:
	sra	$3,$5,16
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	sh	$5,0($17)
$L591:
	lw	$2,36($16)
	.set	noreorder
	.set	nomacro
	j	$L640
	sh	$2,2($22)
	.set	macro
	.set	reorder

$L595:
	lw	$2,24($16)
	srl	$7,$19,16
	andi	$6,$19,0xffff
	andi	$3,$2,0xffff
	addu	$4,$6,$7
	subu	$4,$4,$3
	srl	$2,$2,16
	lhu	$3,10($18)
	subu	$4,$4,$2
	addu	$4,$4,$3
	.set	noreorder
	.set	nomacro
	bgez	$4,$L597
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
	.set	noreorder
	.set	nomacro
	j	$L596
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L597:
	sra	$3,$4,16
	addu	$4,$3,$2
	sra	$2,$4,16
	addu	$4,$4,$2
	sh	$4,10($18)
$L596:
	lw	$2,32($16)
	andi	$8,$fp,0xffff
	lw	$5,44($sp)
	andi	$3,$2,0xffff
	srl	$2,$2,16
	addu	$4,$8,$5
	subu	$4,$4,$3
	lhu	$3,10($18)
	subu	$4,$4,$2
	addu	$4,$4,$3
	.set	noreorder
	.set	nomacro
	bgez	$4,$L600
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
	.set	noreorder
	.set	nomacro
	j	$L599
	sh	$2,10($18)
	.set	macro
	.set	reorder

$L600:
	sra	$3,$4,16
	addu	$4,$3,$2
	sra	$2,$4,16
	addu	$4,$4,$2
	sh	$4,10($18)
$L599:
	lw	$2,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L602
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L603
	beq	$17,$0,$L611
	lhu	$5,0($17)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L611
	addu	$6,$6,$7
	.set	macro
	.set	reorder

	lw	$2,24($16)
	lhu	$4,0($22)
	andi	$3,$2,0xffff
	srl	$2,$2,16
	subu	$6,$6,$3
	subu	$6,$6,$2
	lw	$2,28($16)
	addu	$6,$6,$4
	subu	$6,$6,$2
	addu	$6,$6,$5
	.set	noreorder
	.set	nomacro
	bgez	$6,$L606
	sra	$3,$6,16
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
	j	$L605
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L606:
	andi	$2,$6,0xffff
	addu	$6,$3,$2
	sra	$2,$6,16
	addu	$6,$6,$2
	sh	$6,0($17)
$L605:
	lw	$3,44($sp)
	lw	$2,32($16)
	.set	noreorder
	.set	nomacro
	j	$L645
	addu	$5,$8,$3
	.set	macro
	.set	reorder

$L603:
	lw	$2,24($16)
	addu	$6,$6,$7
	lhu	$4,0($22)
	andi	$3,$2,0xffff
	srl	$2,$2,16
	subu	$6,$6,$3
	subu	$6,$6,$2
	lw	$3,28($16)
	addu	$6,$6,$4
	lhu	$2,0($17)
	subu	$6,$6,$3
	addu	$6,$6,$2
	.set	noreorder
	.set	nomacro
	bgez	$6,$L613
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
	j	$L612
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L613:
	sra	$3,$6,16
	addu	$6,$3,$2
	sra	$2,$6,16
	addu	$6,$6,$2
	sh	$6,0($17)
$L612:
	lw	$4,44($sp)
	lw	$2,32($16)
	addu	$5,$8,$4
$L645:
	andi	$3,$2,0xffff
	srl	$2,$2,16
	subu	$5,$5,$3
	lhu	$4,2($22)
	subu	$5,$5,$2
	lw	$3,36($16)
	addu	$5,$5,$4
	lhu	$2,0($17)
	subu	$5,$5,$3
	addu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	bgez	$5,$L616
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
	j	$L611
	sh	$2,0($17)
	.set	macro
	.set	reorder

$L616:
	sra	$3,$5,16
	addu	$5,$3,$2
	sra	$2,$5,16
	addu	$5,$5,$2
	sh	$5,0($17)
$L611:
	lw	$2,28($16)
	sh	$2,0($22)
	lw	$2,36($16)
	sh	$2,2($22)
$L602:
	lw	$2,24($16)
	sw	$2,12($18)
$L640:
	lw	$2,32($16)
	sw	$2,16($18)
$L535:
	lw	$3,104($21)
	li	$2,-805371904			# 0xcfff0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	lw	$2,28($21)
	sw	$3,104($21)
	lw	$2,12($2)
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
	move	$6,$21
	.set	noreorder
	.set	nomacro
	jal	fast_path_post_process_xmit_check
	sw	$2,40($16)
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L446
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
	j	$L446
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

$L502:
	lw	$16,64($16)
	j	$L634
$L626:
	lw	$2,24($21)
	move	$5,$22
	move	$6,$21
	sw	$2,136($21)
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

$L446:
	lw	$31,84($sp)
	move	$2,$4
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
	lui	$2,%hi(path_list_inuse)
	sw	$16,16($sp)
	move	$7,$0
	sw	$31,20($sp)
	move	$16,$5
	lw	$2,%lo(path_list_inuse)($2)
$L662:
	beq	$2,$0,$L659
	move	$6,$0

	lw	$2,72($2)
	j	$L662
	addiu	$7,$7,1

$L659:
	lui	$2,%hi(napt_list_inuse)
	lw	$2,%lo(napt_list_inuse)($2)
$L663:
	beq	$2,$0,$L661
	lui	$5,%hi($LC1)

	lw	$2,44($2)
	j	$L663
	addiu	$6,$6,1

$L661:
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
	.frame	$sp,64,$31		# vars= 16, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L669:
$L710:
	addiu	$sp,$sp,-64
	lui	$2,%hi(malloc_sizes+4)
	sw	$21,52($sp)
	move	$21,$5
	sw	$19,44($sp)
	li	$5,32			# 0x20
	sw	$18,40($sp)
	move	$18,$4
	sw	$17,36($sp)
	sw	$16,32($sp)
	lui	$16,%hi(table_napt)
	sw	$31,56($sp)
	sw	$20,48($sp)
	lw	$4,%lo(malloc_sizes+4)($2)
	jal	kmem_cache_alloc
	move	$19,$16

	li	$3,-1			# 0xffffffff
	move	$17,$2
	beq	$2,$0,$L664
	sw	$2,%lo(table_napt)($16)

$L720:
$L761:
	lui	$2,%hi(napt_list_inuse)
	lui	$20,%hi(napt_list_free)
	sll	$4,$18,1
	addiu	$3,$2,%lo(napt_list_inuse)
	addiu	$6,$20,%lo(napt_list_free)
	addu	$4,$4,$18
	sw	$0,%lo(napt_list_inuse)($2)
	sll	$4,$4,2
	sw	$3,4($3)
	lui	$2,%hi(napt_table_list_max)
	sw	$0,8($3)
	li	$5,32			# 0x20
	sw	$6,4($6)
	sw	$0,8($6)
	sw	$18,%lo(napt_table_list_max)($2)
	jal	__kmalloc
	sw	$0,%lo(napt_list_free)($20)

	sw	$2,0($17)
	lw	$2,%lo(table_napt)($16)
	lw	$2,0($2)
	beq	$2,$0,$L664
	li	$3,-1			# 0xffffffff

	j	$L764
	move	$16,$0

$L828:
	j	$L664
	li	$3,-2			# 0xfffffffe

$L764:
	blez	$18,$L825
	nop

	move	$4,$0
$L769:
	lw	$2,%lo(table_napt)($19)
	addiu	$16,$16,1
	slt	$3,$16,$18
	lw	$2,0($2)
	addu	$2,$4,$2
	sw	$0,8($2)
	lw	$2,%lo(table_napt)($19)
	lw	$2,0($2)
	addu	$2,$4,$2
	sw	$0,0($2)
	lw	$2,%lo(table_napt)($19)
	lw	$2,0($2)
	addu	$2,$4,$2
	addiu	$4,$4,12
	bne	$3,$0,$L769
	sw	$2,4($2)

$L825:
	blez	$21,$L827
	move	$16,$0

$L823:
$L777:
$L818:
	lui	$2,%hi(malloc_sizes+12)
	li	$5,32			# 0x20
	lw	$4,%lo(malloc_sizes+12)($2)
	jal	kmem_cache_alloc
	addiu	$16,$16,1

	addiu	$4,$20,%lo(napt_list_free)
	move	$3,$2
	addiu	$5,$2,44
	beq	$2,$0,$L828
	slt	$6,$16,$21

	sw	$0,44($2)
	lw	$2,4($4)
	sw	$2,48($3)
	lw	$2,4($4)
	sw	$3,0($2)
	lw	$2,8($4)
	sw	$5,4($4)
	addiu	$2,$2,1
	bne	$6,$0,$L823
	sw	$2,8($4)

$L827:
	move	$3,$0
$L664:
	lw	$31,56($sp)
	move	$2,$3
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
	.end	init_table_napt
	.section	.text.init_table_path,"ax",@progbits
	.align	2
	.globl	init_table_path
	.ent	init_table_path
	.type	init_table_path, @function
init_table_path:
	.set	nomips16
	.frame	$sp,64,$31		# vars= 16, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L834:
$L875:
	addiu	$sp,$sp,-64
	lui	$2,%hi(malloc_sizes+4)
	sw	$21,52($sp)
	move	$21,$5
	sw	$19,44($sp)
	li	$5,32			# 0x20
	sw	$18,40($sp)
	move	$18,$4
	sw	$17,36($sp)
	sw	$16,32($sp)
	lui	$16,%hi(table_path)
	sw	$31,56($sp)
	sw	$20,48($sp)
	lw	$4,%lo(malloc_sizes+4)($2)
	jal	kmem_cache_alloc
	move	$19,$16

	li	$3,-1			# 0xffffffff
	move	$17,$2
	beq	$2,$0,$L829
	sw	$2,%lo(table_path)($16)

$L885:
$L926:
	lui	$2,%hi(path_list_inuse)
	lui	$20,%hi(path_list_free)
	sll	$4,$18,1
	addiu	$3,$2,%lo(path_list_inuse)
	addiu	$6,$20,%lo(path_list_free)
	addu	$4,$4,$18
	sw	$0,%lo(path_list_inuse)($2)
	sll	$4,$4,2
	sw	$3,4($3)
	lui	$2,%hi(path_table_list_max)
	sw	$0,8($3)
	li	$5,32			# 0x20
	sw	$6,4($6)
	sw	$0,8($6)
	sw	$18,%lo(path_table_list_max)($2)
	jal	__kmalloc
	sw	$0,%lo(path_list_free)($20)

	sw	$2,0($17)
	lw	$2,%lo(table_path)($16)
	lw	$2,0($2)
	beq	$2,$0,$L829
	li	$3,-1			# 0xffffffff

	j	$L929
	move	$16,$0

$L993:
	j	$L829
	li	$3,-2			# 0xfffffffe

$L929:
	blez	$18,$L990
	nop

	move	$4,$0
$L934:
	lw	$2,%lo(table_path)($19)
	addiu	$16,$16,1
	slt	$3,$16,$18
	lw	$2,0($2)
	addu	$2,$4,$2
	sw	$0,8($2)
	lw	$2,%lo(table_path)($19)
	lw	$2,0($2)
	addu	$2,$4,$2
	sw	$0,0($2)
	lw	$2,%lo(table_path)($19)
	lw	$2,0($2)
	addu	$2,$4,$2
	addiu	$4,$4,12
	bne	$3,$0,$L934
	sw	$2,4($2)

$L990:
	blez	$21,$L992
	move	$16,$0

$L988:
$L942:
$L983:
	lui	$2,%hi(malloc_sizes+20)
	li	$5,32			# 0x20
	lw	$4,%lo(malloc_sizes+20)($2)
	jal	kmem_cache_alloc
	addiu	$16,$16,1

	addiu	$4,$20,%lo(path_list_free)
	move	$3,$2
	addiu	$5,$2,72
	beq	$2,$0,$L993
	slt	$6,$16,$21

	sw	$0,72($2)
	lw	$2,4($4)
	sw	$2,76($3)
	lw	$2,4($4)
	sw	$3,0($2)
	lw	$2,8($4)
	sw	$5,4($4)
	addiu	$2,$2,1
	bne	$6,$0,$L988
	sw	$2,8($4)

$L992:
	move	$3,$0
$L829:
	lw	$31,56($sp)
	move	$2,$3
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
