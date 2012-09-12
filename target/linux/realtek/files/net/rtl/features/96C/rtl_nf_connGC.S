	.file	1 "rtl_nf_connGC.c"
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
	.globl	drop_priority
	.data
	.align	2
	.type	drop_priority, @object
	.size	drop_priority, 40
drop_priority:
	.byte	8
	.space	1
	.half	60
	.byte	6
	.space	1
	.half	30
	.byte	5
	.space	1
	.half	60
	.byte	7
	.space	1
	.half	120
	.byte	4
	.space	1
	.half	120
	.byte	11
	.space	1
	.half	60
	.byte	1
	.space	1
	.half	90
	.byte	2
	.space	1
	.half	30
	.byte	12
	.space	1
	.half	20
	.byte	3
	.space	1
	.half	120
	.globl	Udp_State_Hash_Head
	.align	2
	.type	Udp_State_Hash_Head, @object
	.size	Udp_State_Hash_Head, 16
Udp_State_Hash_Head:
	.byte	11
	.space	3
	.word	udp_unreply_list
	.byte	12
	.space	3
	.word	udp_assured_list
	.align	2
	.type	udp_assured_list, @object
	.size	udp_assured_list, 8
udp_assured_list:
	.word	udp_assured_list
	.word	udp_assured_list
	.align	2
	.type	udp_unreply_list, @object
	.size	udp_unreply_list, 8
udp_unreply_list:
	.word	udp_unreply_list
	.word	udp_unreply_list
	.globl	Tcp_State_Hash_Head
	.align	2
	.type	Tcp_State_Hash_Head, @object
	.size	Tcp_State_Hash_Head, 88
Tcp_State_Hash_Head:
	.word	0
	.word	0
	.word	1
	.word	syn_sent_list
	.word	2
	.word	syn_recv_list
	.word	3
	.word	established_list
	.word	4
	.word	fin_wait_list
	.word	5
	.word	close_wait_list
	.word	6
	.word	last_ack_list
	.word	7
	.word	time_wait_list
	.word	8
	.word	close_list
	.word	9
	.word	listen_list
	.word	10
	.word	0
	.align	2
	.type	listen_list, @object
	.size	listen_list, 8
listen_list:
	.word	listen_list
	.word	listen_list
	.align	2
	.type	close_list, @object
	.size	close_list, 8
close_list:
	.word	close_list
	.word	close_list
	.align	2
	.type	time_wait_list, @object
	.size	time_wait_list, 8
time_wait_list:
	.word	time_wait_list
	.word	time_wait_list
	.align	2
	.type	last_ack_list, @object
	.size	last_ack_list, 8
last_ack_list:
	.word	last_ack_list
	.word	last_ack_list
	.align	2
	.type	close_wait_list, @object
	.size	close_wait_list, 8
close_wait_list:
	.word	close_wait_list
	.word	close_wait_list
	.align	2
	.type	fin_wait_list, @object
	.size	fin_wait_list, 8
fin_wait_list:
	.word	fin_wait_list
	.word	fin_wait_list
	.align	2
	.type	established_list, @object
	.size	established_list, 8
established_list:
	.word	established_list
	.word	established_list
	.align	2
	.type	syn_recv_list, @object
	.size	syn_recv_list, 8
syn_recv_list:
	.word	syn_recv_list
	.word	syn_recv_list
	.align	2
	.type	syn_sent_list, @object
	.size	syn_sent_list, 8
syn_sent_list:
	.word	syn_sent_list
	.word	syn_sent_list
	.globl	conntrack_max
	.align	2
	.type	conntrack_max, @object
	.size	conntrack_max, 4
conntrack_max:
	.word	100
	.globl	conntrack_min
	.align	2
	.type	conntrack_min, @object
	.size	conntrack_min, 4
conntrack_min:
	.word	0
	.section	.text.conntrack_dointvec,"ax",@progbits
	.align	2
	.globl	conntrack_dointvec
	.ent	conntrack_dointvec
	.type	conntrack_dointvec, @function
conntrack_dointvec:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	lw	$2,48($sp)
	sw	$16,24($sp)
	sw	$31,28($sp)
	sw	$2,16($sp)
	lw	$2,52($sp)
	move	$16,$5
	jal	proc_dointvec
	sw	$2,20($sp)

	bne	$2,$0,$L1
	move	$3,$2

	beq	$16,$0,$L1
	move	$3,$0

	lui	$2,%hi(nf_conntrack_max)
	lw	$6,%lo(nf_conntrack_max)($2)
	lui	$2,%hi(_prot_limit)
	addiu	$9,$2,%lo(_prot_limit)
	lui	$2,%hi(prot_limit)
	addiu	$8,$2,%lo(prot_limit)
	li	$2,1374355456			# 0x51eb0000
	ori	$7,$2,0x851f
	move	$5,$0
	sll	$3,$5,2
$L15:
	addu	$2,$3,$8
	lw	$2,0($2)
	addiu	$5,$5,1
	addu	$3,$3,$9
	mult	$6,$2
	slt	$4,$5,3
	mflo	$2
	multu	$2,$7
	mfhi	$2
	srl	$2,$2,5
	sw	$2,0($3)
	bne	$4,$0,$L15
	sll	$3,$5,2

	li	$3,-859045888			# 0xffffffffcccc0000
	sll	$2,$6,2
	ori	$3,$3,0xcccd
	multu	$2,$3
	lui	$4,%hi(rtl_nf_conntrack_threshold)
	mfhi	$2
	srl	$2,$2,2
	subu	$3,$6,$2
	sltu	$3,$3,65
	bne	$3,$0,$L8
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)

	addiu	$2,$6,-64
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)
$L8:
	lui	$2,%hi(nf_conntrack_max)
	lw	$2,%lo(nf_conntrack_max)($2)
	nop
	beq	$2,$0,$L16
	lui	$2,%hi(rtl_newGC_session_status_flags)

	lui	$2,%hi(init_net+468)
	lw	$3,%lo(init_net+468)($2)
	lw	$2,%lo(rtl_nf_conntrack_threshold)($4)
	nop
	slt	$2,$2,$3
	beq	$2,$0,$L9
	lui	$4,%hi(rtl_newGC_session_status_flags)

	lw	$2,%lo(rtl_newGC_session_status_flags)($4)
	li	$3,3			# 0x3
	beq	$2,$3,$L3
	lui	$2,%hi(jiffies)

	lw	$2,%lo(jiffies)($2)
	sw	$3,%lo(rtl_newGC_session_status_flags)($4)
	lui	$3,%hi(rtl_newGC_session_status_time)
	addiu	$2,$2,1
	j	$L3
	sw	$2,%lo(rtl_newGC_session_status_time)($3)

$L9:
	lui	$2,%hi(rtl_newGC_session_status_flags)
$L16:
	sw	$0,%lo(rtl_newGC_session_status_flags)($2)
$L3:
	move	$3,$0
$L1:
	lw	$31,28($sp)
	lw	$16,24($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	conntrack_dointvec
	.section	.text.conntrack_dointvec_minmax,"ax",@progbits
	.align	2
	.globl	conntrack_dointvec_minmax
	.ent	conntrack_dointvec_minmax
	.type	conntrack_dointvec_minmax, @function
conntrack_dointvec_minmax:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	lw	$2,48($sp)
	sw	$16,24($sp)
	sw	$31,28($sp)
	sw	$2,16($sp)
	lw	$2,52($sp)
	move	$16,$5
	jal	proc_dointvec_minmax
	sw	$2,20($sp)

	bne	$2,$0,$L17
	move	$3,$2

	beq	$16,$0,$L17
	move	$3,$0

	lui	$2,%hi(nf_conntrack_max)
	lw	$6,%lo(nf_conntrack_max)($2)
	lui	$2,%hi(_prot_limit)
	addiu	$9,$2,%lo(_prot_limit)
	lui	$2,%hi(prot_limit)
	addiu	$8,$2,%lo(prot_limit)
	li	$2,1374355456			# 0x51eb0000
	ori	$7,$2,0x851f
	move	$5,$0
	sll	$3,$5,2
$L31:
	addu	$2,$3,$8
	lw	$2,0($2)
	addiu	$5,$5,1
	addu	$3,$3,$9
	mult	$6,$2
	slt	$4,$5,3
	mflo	$2
	multu	$2,$7
	mfhi	$2
	srl	$2,$2,5
	sw	$2,0($3)
	bne	$4,$0,$L31
	sll	$3,$5,2

	li	$3,-859045888			# 0xffffffffcccc0000
	sll	$2,$6,2
	ori	$3,$3,0xcccd
	multu	$2,$3
	lui	$4,%hi(rtl_nf_conntrack_threshold)
	mfhi	$2
	srl	$2,$2,2
	subu	$3,$6,$2
	sltu	$3,$3,65
	bne	$3,$0,$L24
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)

	addiu	$2,$6,-64
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)
$L24:
	lui	$2,%hi(nf_conntrack_max)
	lw	$2,%lo(nf_conntrack_max)($2)
	nop
	beq	$2,$0,$L32
	lui	$2,%hi(rtl_newGC_session_status_flags)

	lui	$2,%hi(init_net+468)
	lw	$3,%lo(init_net+468)($2)
	lw	$2,%lo(rtl_nf_conntrack_threshold)($4)
	nop
	slt	$2,$2,$3
	beq	$2,$0,$L25
	lui	$4,%hi(rtl_newGC_session_status_flags)

	lw	$2,%lo(rtl_newGC_session_status_flags)($4)
	li	$3,3			# 0x3
	beq	$2,$3,$L19
	lui	$2,%hi(jiffies)

	lw	$2,%lo(jiffies)($2)
	sw	$3,%lo(rtl_newGC_session_status_flags)($4)
	lui	$3,%hi(rtl_newGC_session_status_time)
	addiu	$2,$2,1
	j	$L19
	sw	$2,%lo(rtl_newGC_session_status_time)($3)

$L25:
	lui	$2,%hi(rtl_newGC_session_status_flags)
$L32:
	sw	$0,%lo(rtl_newGC_session_status_flags)($2)
$L19:
	move	$3,$0
$L17:
	lw	$31,28($sp)
	lw	$16,24($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	conntrack_dointvec_minmax
	.section	.text.clean_from_lists,"ax",@progbits
	.align	2
	.globl	clean_from_lists
	.ent	clean_from_lists
	.type	clean_from_lists, @function
clean_from_lists:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$17,20($sp)
	sw	$16,16($sp)
	sw	$31,24($sp)
	lw	$3,4($4)
	addiu	$6,$4,4
	move	$16,$4
	lw	$4,4($6)
	andi	$2,$3,0x1
	move	$17,$5
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L38
	sw	$3,0($4)
	.set	macro
	.set	reorder

	sw	$4,4($3)
$L38:
	li	$2,2097152			# 0x200000
	ori	$7,$2,0x200
	sw	$7,4($6)
	lw	$3,52($16)
	addiu	$5,$16,52
	lw	$4,4($5)
	andi	$2,$3,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L42
	sw	$3,0($4)
	.set	macro
	.set	reorder

	sw	$4,4($3)
$L42:
	.set	noreorder
	.set	nomacro
	jal	local_bh_disable
	sw	$7,4($5)
	.set	macro
	.set	reorder

	lbu	$3,50($16)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L48
	slt	$2,$3,7
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L57
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L54
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	j	$L68
$L57:
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L51
	lui	$4,%hi(prot_counters+8)
	.set	macro
	.set	reorder

	j	$L47
$L48:
	lui	$4,%hi(prot_counters+4)
#APP
	raw_local_irq_save	$3
#NO_APP
	lw	$2,%lo(prot_counters+4)($4)
	#nop
	addiu	$2,$2,-1
	sw	$2,%lo(prot_counters+4)($4)
	j	$L67
$L51:
#APP
	raw_local_irq_save	$3
#NO_APP
	lw	$2,%lo(prot_counters+8)($4)
	#nop
	addiu	$2,$2,-1
	sw	$2,%lo(prot_counters+8)($4)
	j	$L67
$L54:
	lui	$4,%hi(prot_counters)
#APP
	raw_local_irq_save	$3
#NO_APP
	lw	$2,%lo(prot_counters)($4)
	#nop
	addiu	$2,$2,-1
	sw	$2,%lo(prot_counters)($4)
$L67:
#APP
	raw_local_irq_restore	$3
#NO_APP
$L47:
	lbu	$3,50($16)
	li	$2,6			# 0x6
$L68:
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L59
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$3,$2,$L66
$L59:
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	clean_from_lists_hooks
	move	$5,$17
	.set	macro
	.set	reorder

	addiu	$5,$16,204
	lw	$4,4($5)
	lw	$3,204($16)
	li	$2,1048576			# 0x100000
	ori	$2,$2,0x100
	sw	$3,0($4)
	sw	$2,204($16)
	li	$2,2097152			# 0x200000
	ori	$2,$2,0x200
	sw	$4,4($3)
	sw	$2,4($5)
	lui	$2,%hi(nf_conntrack_max)
	lw	$2,%lo(nf_conntrack_max)($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L69
	lui	$2,%hi(rtl_newGC_session_status_flags)
	.set	macro
	.set	reorder

	lui	$2,%hi(rtl_nf_conntrack_threshold)
	lw	$3,468($17)
	lw	$2,%lo(rtl_nf_conntrack_threshold)($2)
	#nop
	slt	$2,$2,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L62
	lui	$4,%hi(rtl_newGC_session_status_flags)
	.set	macro
	.set	reorder

	lw	$2,%lo(rtl_newGC_session_status_flags)($4)
	li	$3,3			# 0x3
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L66
	lui	$2,%hi(jiffies)
	.set	macro
	.set	reorder

	lw	$2,%lo(jiffies)($2)
	sw	$3,%lo(rtl_newGC_session_status_flags)($4)
	lui	$3,%hi(rtl_newGC_session_status_time)
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	$L66
	sw	$2,%lo(rtl_newGC_session_status_time)($3)
	.set	macro
	.set	reorder

$L62:
	lui	$2,%hi(rtl_newGC_session_status_flags)
$L69:
	sw	$0,%lo(rtl_newGC_session_status_flags)($2)
$L66:
	jal	local_bh_enable
	move	$4,$16
	lw	$31,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	nf_ct_remove_expectations
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	clean_from_lists
	.section	.text.rtl_death_action,"ax",@progbits
	.align	2
	.globl	rtl_death_action
	.ent	rtl_death_action
	.type	rtl_death_action, @function
rtl_death_action:
	.set	nomips16
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$17,20($sp)
	sw	$16,16($sp)
	sw	$31,24($sp)
	lw	$3,200($4)
	lui	$2,%hi(init_net)
	move	$16,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L77
	addiu	$17,$2,%lo(init_net)
	.set	macro
	.set	reorder

	lbu	$2,8($3)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L77
	addu	$3,$3,$2
	.set	macro
	.set	reorder

	beq	$3,$0,$L77
	lw	$2,0($3)
	#nop
	beq	$2,$0,$L77
	lw	$2,64($2)
	#nop
	beq	$2,$0,$L77
	jal	$2
$L77:
	jal	local_bh_disable
	lw	$3,488($17)
	move	$5,$17
	move	$4,$16
	lw	$2,24($3)
	#nop
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	clean_from_lists
	sw	$2,24($3)
	.set	macro
	.set	reorder

	jal	local_bh_enable
	beq	$16,$0,$L70
#APP
	raw_local_irq_save	$3
#NO_APP
	lw	$2,0($16)
	#nop
	addiu	$2,$2,-1
	sw	$2,0($16)
#APP
	raw_local_irq_restore	$3
#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L70
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$31,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	nf_conntrack_destroy
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L70:
	lw	$31,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	rtl_death_action
	.section	.text.rtl_connGC_addList,"ax",@progbits
	.align	2
	.globl	rtl_connGC_addList
	.ent	rtl_connGC_addList
	.type	rtl_connGC_addList, @function
rtl_connGC_addList:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lw	$2,156($4)
	#nop
	beq	$2,$0,$L98
	lbu	$3,9($2)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L103
	slt	$2,$3,7
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L123
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L118
	lui	$4,%hi(prot_counters)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$0
	.set	macro
	.set	reorder

$L123:
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L109
	move	$2,$0
	.set	macro
	.set	reorder

	j	$L126
$L103:
	lbu	$3,176($5)
	lui	$2,%hi(Tcp_State_Hash_Head)
	addiu	$2,$2,%lo(Tcp_State_Hash_Head)
	sll	$3,$3,3
	addu	$3,$3,$2
	lw	$4,4($3)
	#nop
	beq	$4,$0,$L104
	lw	$3,4($4)
	addiu	$2,$5,204
	sw	$4,204($5)
	sw	$2,4($4)
	sw	$2,0($3)
	sw	$3,4($2)
$L104:
	lui	$4,%hi(prot_counters+4)
#APP
	raw_local_irq_save	$3
#NO_APP
	lw	$2,%lo(prot_counters+4)($4)
	#nop
	addiu	$2,$2,1
	sw	$2,%lo(prot_counters+4)($4)
	j	$L125
$L109:
	lw	$2,100($5)
	addiu	$4,$5,204
	andi	$2,$2,0x2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L110
	lui	$3,%hi(Udp_State_Hash_Head)
	.set	macro
	.set	reorder

	addiu	$2,$3,%lo(Udp_State_Hash_Head)
	lw	$2,12($2)
	j	$L124
$L110:
	addiu	$2,$3,%lo(Udp_State_Hash_Head)
	lw	$2,4($2)
	#nop
$L124:
	lw	$3,4($2)
	sw	$2,204($5)
	sw	$4,4($2)
	sw	$4,0($3)
	sw	$3,4($4)
	lui	$4,%hi(prot_counters+8)
#APP
	raw_local_irq_save	$3
#NO_APP
	lw	$2,%lo(prot_counters+8)($4)
	#nop
	addiu	$2,$2,1
	sw	$2,%lo(prot_counters+8)($4)
	j	$L125
$L118:
#APP
	raw_local_irq_save	$3
#NO_APP
	lw	$2,%lo(prot_counters)($4)
	#nop
	addiu	$2,$2,1
	sw	$2,%lo(prot_counters)($4)
$L125:
#APP
	raw_local_irq_restore	$3
#NO_APP
$L98:
	move	$2,$0
$L126:
	j	$31
	.end	rtl_connGC_addList
	.section	.text.__nf_ct_refresh_acct_proto,"ax",@progbits
	.align	2
	.globl	__nf_ct_refresh_acct_proto
	.ent	__nf_ct_refresh_acct_proto
	.type	__nf_ct_refresh_acct_proto, @function
__nf_ct_refresh_acct_proto:
	.set	nomips16
	.frame	$sp,64,$31		# vars= 8, regs= 7/0, args= 24, gp= 0
	.mask	0x803f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	sw	$17,36($sp)
	move	$17,$4
	sw	$21,52($sp)
	sw	$20,48($sp)
	sw	$19,44($sp)
	sw	$18,40($sp)
	sw	$31,56($sp)
	sw	$16,32($sp)
	move	$20,$5
	move	$21,$6
	sw	$0,24($sp)
	lbu	$18,87($sp)
	jal	local_bh_disable
	move	$19,$7

	lw	$2,100($17)
	nop
	srl	$2,$2,10
	andi	$2,$2,0x1
	bne	$2,$0,$L163
	nop

	lw	$2,100($17)
	nop
	srl	$2,$2,3
	andi	$2,$2,0x1
	bne	$2,$0,$L136
	li	$2,8			# 0x8

	sw	$19,116($17)
	j	$L139
	sw	$2,24($sp)

$L136:
	addiu	$16,$17,108
	jal	del_timer
	move	$4,$16

	beq	$2,$0,$L139
	lui	$2,%hi(jiffies)

	lw	$2,%lo(jiffies)($2)
	move	$4,$16
	addu	$2,$2,$19
	jal	add_timer
	sw	$2,116($17)

	li	$2,8			# 0x8
	sw	$2,24($sp)
	li	$2,6			# 0x6
	beq	$18,$2,$L142
	li	$2,17			# 0x11

	beq	$18,$2,$L147
	nop

	j	$L139
	nop

$L142:
	lw	$3,92($sp)
	lui	$2,%hi(Tcp_State_Hash_Head)
	addiu	$4,$17,204
	addiu	$2,$2,%lo(Tcp_State_Hash_Head)
	sll	$3,$3,3
	lw	$6,4($4)
	lw	$5,204($17)
	addu	$3,$3,$2
	lw	$2,4($3)
	sw	$6,4($5)
	sw	$5,0($6)
	lw	$3,4($2)
	sw	$2,204($17)
	sw	$4,4($2)
	sw	$4,0($3)
	j	$L139
	sw	$3,4($4)

$L147:
	lw	$2,100($17)
	lw	$6,204($17)
	andi	$2,$2,0x2
	beq	$2,$0,$L148
	addiu	$5,$17,204

	lw	$2,4($5)
	lui	$3,%hi(udp_assured_list)
	j	$L162
	addiu	$3,$3,%lo(udp_assured_list)

$L148:
	lw	$2,4($5)
	lui	$3,%hi(udp_unreply_list)
	addiu	$3,$3,%lo(udp_unreply_list)
$L162:
	sw	$2,4($6)
	lw	$4,4($3)
	sw	$6,0($2)
	sw	$5,4($3)
	sw	$3,204($17)
	sw	$4,4($5)
	sw	$5,0($4)
$L139:
	lw	$7,80($sp)
	addiu	$2,$sp,24
	move	$4,$17
	move	$5,$20
	move	$6,$21
	jal	__nf_ct_refresh_acct_proto_hooks
	sw	$2,16($sp)

$L163:
	jal	local_bh_enable
	nop

	lw	$31,56($sp)
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
	.end	__nf_ct_refresh_acct_proto
	.section	.text.drop_one_conntrack,"ax",@progbits
	.align	2
	.globl	drop_one_conntrack
	.ent	drop_one_conntrack
	.type	drop_one_conntrack, @function
drop_one_conntrack:
	.set	nomips16
	.frame	$sp,64,$31		# vars= 0, regs= 9/0, args= 24, gp= 0
	.mask	0x80ff0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	sw	$31,56($sp)
	sw	$23,52($sp)
	sw	$22,48($sp)
	sw	$21,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	lhu	$3,16($4)
	nop
	sltu	$2,$3,1024
	bne	$2,$0,$L164
	li	$2,1			# 0x1

	li	$6,8080			# 0x1f90
	beq	$3,$6,$L164
	nop

	lhu	$3,36($4)
	nop
	sltu	$2,$3,1024
	bne	$2,$0,$L164
	li	$2,1			# 0x1

	beq	$3,$6,$L164
	nop

	lhu	$3,16($5)
	nop
	sltu	$2,$3,1024
	bne	$2,$0,$L164
	li	$2,1			# 0x1

	beq	$3,$6,$L164
	nop

	lhu	$3,36($5)
	nop
	sltu	$2,$3,1024
	bne	$2,$0,$L164
	li	$2,1			# 0x1

	beq	$3,$6,$L164
	li	$7,-268435456			# 0xfffffffff0000000

	lw	$3,20($4)
	li	$8,-536870912			# 0xffffffffe0000000
	and	$2,$3,$7
	lw	$4,0($4)
	beq	$2,$8,$L401
	li	$6,-1			# 0xffffffffffffffff

	beq	$3,$6,$L164
	li	$2,1			# 0x1

	beq	$4,$0,$L164
	nop

	lw	$3,20($5)
	lw	$5,0($5)
	and	$2,$3,$7
	beq	$2,$8,$L164
	li	$2,1			# 0x1

	beq	$3,$6,$L164
	nop

	beq	$5,$0,$L164
	lui	$3,%hi(prot_counters+4)

	lui	$2,%hi(_prot_limit+4)
	lw	$2,%lo(_prot_limit+4)($2)
	lw	$3,%lo(prot_counters+4)($3)
	nop
	slt	$2,$2,$3
	beq	$2,$0,$L454
	lui	$2,%hi(_prot_limit+8)

	jal	local_bh_disable
	move	$19,$0

	lui	$3,%hi(drop_priority)
$L456:
	addiu	$3,$3,%lo(drop_priority)
	sll	$2,$19,2
	addu	$2,$2,$3
	lbu	$3,0($2)
	nop
	sltu	$2,$3,10
	beq	$2,$0,$L193
	lui	$2,%hi(Tcp_State_Hash_Head)

	sll	$3,$3,3
	addiu	$2,$2,%lo(Tcp_State_Hash_Head)
	addu	$3,$3,$2
	lw	$18,4($3)
	nop
	lw	$4,0($18)
	nop
	beq	$4,$18,$L193
	move	$17,$4

$L444:
	beq	$17,$18,$L193
	addiu	$16,$17,-204

	lw	$6,32($16)
	li	$11,-268435456			# 0xfffffffff0000000
	li	$12,-536870912			# 0xffffffffe0000000
	and	$2,$6,$11
	lhu	$7,28($16)
	lhu	$8,48($16)
	lhu	$9,76($16)
	lhu	$10,96($16)
	lw	$4,12($16)
	lw	$5,60($16)
	lw	$3,80($16)
	beq	$2,$12,$L199
	and	$2,$3,$11

	beq	$2,$12,$L199
	nop

	bne	$4,$3,$L455
	sltu	$2,$7,1024

	beq	$6,$5,$L199
	nop

$L455:
	bne	$2,$0,$L199
	sltu	$2,$8,1024

	bne	$2,$0,$L199
	sltu	$2,$9,1024

	bne	$2,$0,$L199
	sltu	$2,$10,1024

	bne	$2,$0,$L199
	li	$2,8080			# 0x1f90

	beq	$7,$2,$L199
	nop

	beq	$8,$2,$L199
	nop

	beq	$9,$2,$L199
	nop

	beq	$10,$2,$L199
	nop

	jal	del_timer
	addiu	$4,$17,-96

	bne	$2,$0,$L419
	nop

$L199:
	lw	$17,0($17)
	j	$L444
	nop

$L193:
	addiu	$19,$19,1
	sltu	$2,$19,10
	bne	$2,$0,$L456
	lui	$3,%hi(drop_priority)

	jal	local_bh_enable
	nop

	lui	$2,%hi(_prot_limit+8)
$L454:
	lui	$3,%hi(prot_counters+8)
	lw	$2,%lo(_prot_limit+8)($2)
	lw	$3,%lo(prot_counters+8)($3)
	nop
	slt	$2,$2,$3
	beq	$2,$0,$L223
	nop

	jal	local_bh_disable
	move	$19,$0

	lui	$3,%hi(drop_priority)
$L458:
	addiu	$3,$3,%lo(drop_priority)
	sll	$2,$19,2
	addu	$2,$2,$3
	lbu	$3,0($2)
	nop
	sltu	$2,$3,11
	bne	$2,$0,$L229
	lui	$2,%hi(Udp_State_Hash_Head)

	sll	$3,$3,3
	addiu	$2,$2,%lo(Udp_State_Hash_Head)
	addu	$3,$3,$2
	lw	$18,-84($3)
	nop
	lw	$4,0($18)
	nop
	beq	$4,$18,$L229
	move	$17,$4

$L445:
	beq	$17,$18,$L229
	addiu	$16,$17,-204

	lw	$6,32($16)
	li	$11,-268435456			# 0xfffffffff0000000
	li	$12,-536870912			# 0xffffffffe0000000
	and	$2,$6,$11
	lhu	$7,28($16)
	lhu	$8,48($16)
	lhu	$9,76($16)
	lhu	$10,96($16)
	lw	$4,12($16)
	lw	$5,60($16)
	lw	$3,80($16)
	beq	$2,$12,$L235
	and	$2,$3,$11

	beq	$2,$12,$L235
	nop

	bne	$4,$3,$L457
	sltu	$2,$7,1024

	beq	$6,$5,$L235
	nop

$L457:
	bne	$2,$0,$L235
	sltu	$2,$8,1024

	bne	$2,$0,$L235
	sltu	$2,$9,1024

	bne	$2,$0,$L235
	sltu	$2,$10,1024

	bne	$2,$0,$L235
	li	$2,8080			# 0x1f90

	beq	$7,$2,$L235
	nop

	beq	$8,$2,$L235
	nop

	beq	$9,$2,$L235
	nop

	beq	$10,$2,$L235
	nop

	jal	del_timer
	addiu	$4,$17,-96

	bne	$2,$0,$L419
	nop

$L235:
	lw	$17,0($17)
	j	$L445
	nop

$L229:
	addiu	$19,$19,1
	sltu	$2,$19,10
	bne	$2,$0,$L458
	lui	$3,%hi(drop_priority)

	jal	local_bh_enable
	nop

	j	$L223
	nop

$L419:
	jal	local_bh_enable
	nop

	jal	rtl_death_action
	move	$4,$16

	j	$L453
	lui	$4,%hi(rtl_newGC_session_status_flags)

$L223:
	jal	local_bh_disable
	move	$19,$0

	lui	$22,%hi(drop_priority)
$L463:
	sll	$21,$19,2
	addiu	$2,$22,%lo(drop_priority)
	addu	$2,$21,$2
	lbu	$2,0($2)
	nop
	sltu	$3,$2,10
	beq	$3,$0,$L267
	sll	$4,$2,3

	lui	$2,%hi(Tcp_State_Hash_Head)
	addiu	$2,$2,%lo(Tcp_State_Hash_Head)
	addu	$2,$4,$2
	lw	$18,4($2)
	nop
	lw	$4,0($18)
	nop
	beq	$4,$18,$L300
	li	$23,3			# 0x3

	move	$17,$4
	move	$20,$0
$L446:
	beq	$17,$18,$L300
	addiu	$16,$17,-204

	lw	$6,32($16)
	li	$11,-268435456			# 0xfffffffff0000000
	li	$12,-536870912			# 0xffffffffe0000000
	and	$2,$6,$11
	lhu	$7,28($16)
	lhu	$8,48($16)
	lhu	$9,76($16)
	lhu	$10,96($16)
	lw	$4,12($16)
	lw	$5,60($16)
	lw	$3,80($16)
	beq	$2,$12,$L273
	addiu	$20,$20,1

	and	$2,$3,$11
	beq	$2,$12,$L273
	nop

	bne	$4,$3,$L459
	sltu	$2,$7,1024

	beq	$6,$5,$L273
	nop

$L459:
	bne	$2,$0,$L273
	sltu	$2,$8,1024

	bne	$2,$0,$L273
	sltu	$2,$9,1024

	bne	$2,$0,$L273
	sltu	$2,$10,1024

	bne	$2,$0,$L273
	li	$2,8080			# 0x1f90

	beq	$7,$2,$L273
	nop

	beq	$8,$2,$L273
	nop

	beq	$9,$2,$L273
	nop

	beq	$10,$2,$L273
	lui	$2,%hi(jiffies)

	lw	$4,%lo(jiffies)($2)
	lw	$3,116($16)
	addiu	$2,$22,%lo(drop_priority)
	addu	$2,$21,$2
	lhu	$2,2($2)
	subu	$3,$3,$4
	srl	$3,$3,7
	sltu	$2,$2,$3
	bne	$2,$0,$L460
	slt	$2,$20,1025

	jal	del_timer
	addiu	$4,$17,-96

	beq	$2,$0,$L460
	slt	$2,$20,1025

	move	$4,$16
	move	$5,$19
	move	$6,$0
	li	$7,1			# 0x1
	jal	__drop_one_conntrack_process_hooks1
	sw	$23,16($sp)

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L420
	li	$2,2			# 0x2

	beq	$3,$2,$L447
	move	$5,$19

	move	$4,$16
	move	$6,$0
	li	$7,1			# 0x1
	jal	__drop_one_conntrack_process_hooks2
	sw	$23,16($sp)

	slt	$2,$20,1025
$L460:
	beq	$2,$0,$L323
	move	$3,$0

$L273:
	lw	$17,0($17)
	j	$L446
	nop

$L420:
	jal	local_bh_enable
	nop

	jal	rtl_death_action
	move	$4,$16

$L447:
	j	$L323
	li	$3,1			# 0x1

$L267:
	lui	$2,%hi(Udp_State_Hash_Head)
	addiu	$2,$2,%lo(Udp_State_Hash_Head)
	addu	$2,$4,$2
	lw	$18,-84($2)
	nop
	lw	$4,0($18)
	nop
	beq	$4,$18,$L300
	li	$23,12			# 0xc

	move	$17,$4
	move	$20,$0
$L448:
	beq	$17,$18,$L300
	addiu	$16,$17,-204

	lw	$6,32($16)
	li	$11,-268435456			# 0xfffffffff0000000
	li	$12,-536870912			# 0xffffffffe0000000
	and	$2,$6,$11
	lhu	$7,28($16)
	lhu	$8,48($16)
	lhu	$9,76($16)
	lhu	$10,96($16)
	lw	$4,12($16)
	lw	$5,60($16)
	lw	$3,80($16)
	beq	$2,$12,$L304
	addiu	$20,$20,1

	and	$2,$3,$11
	beq	$2,$12,$L304
	nop

	bne	$4,$3,$L461
	sltu	$2,$7,1024

	beq	$6,$5,$L304
	nop

$L461:
	bne	$2,$0,$L304
	sltu	$2,$8,1024

	bne	$2,$0,$L304
	sltu	$2,$9,1024

	bne	$2,$0,$L304
	sltu	$2,$10,1024

	bne	$2,$0,$L304
	li	$2,8080			# 0x1f90

	beq	$7,$2,$L304
	nop

	beq	$8,$2,$L304
	nop

	beq	$9,$2,$L304
	nop

	beq	$10,$2,$L304
	lui	$2,%hi(jiffies)

	lw	$4,%lo(jiffies)($2)
	lw	$3,116($16)
	addiu	$2,$22,%lo(drop_priority)
	addu	$2,$21,$2
	lhu	$2,2($2)
	subu	$3,$3,$4
	srl	$3,$3,7
	sltu	$2,$2,$3
	bne	$2,$0,$L462
	slt	$2,$20,1025

	jal	del_timer
	addiu	$4,$17,-96

	beq	$2,$0,$L462
	slt	$2,$20,1025

	move	$4,$16
	move	$5,$19
	move	$6,$0
	li	$7,1			# 0x1
	jal	__drop_one_conntrack_process_hooks1
	sw	$23,16($sp)

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L420
	li	$2,2			# 0x2

	beq	$3,$2,$L447
	move	$5,$19

	move	$4,$16
	move	$6,$0
	li	$7,1			# 0x1
	jal	__drop_one_conntrack_process_hooks2
	sw	$23,16($sp)

	slt	$2,$20,1025
$L462:
	beq	$2,$0,$L323
	move	$3,$0

$L304:
	lw	$17,0($17)
	j	$L448
	nop

$L300:
	move	$3,$0
$L323:
	li	$2,1			# 0x1
	beq	$3,$2,$L260
	addiu	$19,$19,1

	sltu	$2,$19,10
	bne	$2,$0,$L463
	lui	$22,%hi(drop_priority)

	jal	local_bh_enable
	move	$19,$0

	jal	local_bh_disable
	lui	$22,%hi(drop_priority)

$L468:
	sll	$21,$19,2
	addiu	$2,$22,%lo(drop_priority)
	addu	$2,$21,$2
	lbu	$2,0($2)
	nop
	sltu	$3,$2,10
	beq	$3,$0,$L337
	sll	$4,$2,3

	lui	$2,%hi(Tcp_State_Hash_Head)
	addiu	$2,$2,%lo(Tcp_State_Hash_Head)
	addu	$2,$4,$2
	lw	$18,4($2)
	nop
	lw	$4,0($18)
	nop
	beq	$4,$18,$L370
	li	$23,3			# 0x3

	move	$17,$4
	move	$20,$0
$L449:
	beq	$17,$18,$L370
	addiu	$16,$17,-204

	lw	$6,32($16)
	li	$11,-268435456			# 0xfffffffff0000000
	li	$12,-536870912			# 0xffffffffe0000000
	and	$2,$6,$11
	lhu	$7,28($16)
	lhu	$8,48($16)
	lhu	$9,76($16)
	lhu	$10,96($16)
	lw	$4,12($16)
	lw	$5,60($16)
	lw	$3,80($16)
	beq	$2,$12,$L343
	addiu	$20,$20,1

	and	$2,$3,$11
	beq	$2,$12,$L343
	nop

	bne	$4,$3,$L464
	sltu	$2,$7,1024

	beq	$6,$5,$L343
	nop

$L464:
	bne	$2,$0,$L343
	sltu	$2,$8,1024

	bne	$2,$0,$L343
	sltu	$2,$9,1024

	bne	$2,$0,$L343
	sltu	$2,$10,1024

	bne	$2,$0,$L343
	li	$2,8080			# 0x1f90

	beq	$7,$2,$L343
	nop

	beq	$8,$2,$L343
	nop

	beq	$9,$2,$L343
	nop

	beq	$10,$2,$L343
	lui	$2,%hi(jiffies)

	lw	$4,%lo(jiffies)($2)
	lw	$3,116($16)
	addiu	$2,$22,%lo(drop_priority)
	addu	$2,$21,$2
	lhu	$2,2($2)
	subu	$3,$3,$4
	srl	$3,$3,9
	sltu	$2,$2,$3
	bne	$2,$0,$L465
	slt	$2,$20,1025

	jal	del_timer
	addiu	$4,$17,-96

	beq	$2,$0,$L465
	slt	$2,$20,1025

	move	$4,$16
	move	$5,$19
	li	$6,2			# 0x2
	move	$7,$0
	jal	__drop_one_conntrack_process_hooks1
	sw	$23,16($sp)

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L425
	li	$2,2			# 0x2

	beq	$3,$2,$L450
	move	$5,$19

	move	$4,$16
	li	$6,2			# 0x2
	move	$7,$0
	jal	__drop_one_conntrack_process_hooks2
	sw	$23,16($sp)

	slt	$2,$20,1025
$L465:
	beq	$2,$0,$L393
	move	$3,$0

$L343:
	lw	$17,0($17)
	j	$L449
	nop

$L425:
	jal	local_bh_enable
	nop

	jal	rtl_death_action
	move	$4,$16

$L450:
	j	$L393
	li	$3,1			# 0x1

$L337:
	lui	$2,%hi(Udp_State_Hash_Head)
	addiu	$2,$2,%lo(Udp_State_Hash_Head)
	addu	$2,$4,$2
	lw	$18,-84($2)
	nop
	lw	$4,0($18)
	nop
	beq	$4,$18,$L370
	li	$23,12			# 0xc

	move	$17,$4
	move	$20,$0
$L451:
	beq	$17,$18,$L370
	addiu	$16,$17,-204

	lw	$6,32($16)
	li	$11,-268435456			# 0xfffffffff0000000
	li	$12,-536870912			# 0xffffffffe0000000
	and	$2,$6,$11
	lhu	$7,28($16)
	lhu	$8,48($16)
	lhu	$9,76($16)
	lhu	$10,96($16)
	lw	$4,12($16)
	lw	$5,60($16)
	lw	$3,80($16)
	beq	$2,$12,$L374
	addiu	$20,$20,1

	and	$2,$3,$11
	beq	$2,$12,$L374
	nop

	bne	$4,$3,$L466
	sltu	$2,$7,1024

	beq	$6,$5,$L374
	nop

$L466:
	bne	$2,$0,$L374
	sltu	$2,$8,1024

	bne	$2,$0,$L374
	sltu	$2,$9,1024

	bne	$2,$0,$L374
	sltu	$2,$10,1024

	bne	$2,$0,$L374
	li	$2,8080			# 0x1f90

	beq	$7,$2,$L374
	nop

	beq	$8,$2,$L374
	nop

	beq	$9,$2,$L374
	nop

	beq	$10,$2,$L374
	lui	$2,%hi(jiffies)

	lw	$4,%lo(jiffies)($2)
	lw	$3,116($16)
	addiu	$2,$22,%lo(drop_priority)
	addu	$2,$21,$2
	lhu	$2,2($2)
	subu	$3,$3,$4
	srl	$3,$3,9
	sltu	$2,$2,$3
	bne	$2,$0,$L467
	slt	$2,$20,1025

	jal	del_timer
	addiu	$4,$17,-96

	beq	$2,$0,$L467
	slt	$2,$20,1025

	move	$4,$16
	move	$5,$19
	li	$6,2			# 0x2
	move	$7,$0
	jal	__drop_one_conntrack_process_hooks1
	sw	$23,16($sp)

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L425
	li	$2,2			# 0x2

	beq	$3,$2,$L450
	move	$5,$19

	move	$4,$16
	li	$6,2			# 0x2
	move	$7,$0
	jal	__drop_one_conntrack_process_hooks2
	sw	$23,16($sp)

	slt	$2,$20,1025
$L467:
	beq	$2,$0,$L393
	move	$3,$0

$L374:
	lw	$17,0($17)
	j	$L451
	nop

$L370:
	move	$3,$0
$L393:
	li	$2,1			# 0x1
	beq	$3,$2,$L260
	addiu	$19,$19,1

	sltu	$2,$19,10
	bne	$2,$0,$L468
	lui	$22,%hi(drop_priority)

	j	$L443
	nop

$L260:
	lui	$4,%hi(rtl_newGC_session_status_flags)
$L453:
	lw	$2,%lo(rtl_newGC_session_status_flags)($4)
	nop
	bne	$2,$0,$L401
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	li	$2,3			# 0x3
	sw	$2,%lo(rtl_newGC_session_status_flags)($4)
	addiu	$3,$3,1
	lui	$2,%hi(rtl_newGC_session_status_time)
	sw	$3,%lo(rtl_newGC_session_status_time)($2)
$L401:
	j	$L164
	li	$2,1			# 0x1

$L443:
	jal	local_bh_enable
	nop

	lui	$4,%hi(rtl_newGC_session_status_flags)
	lw	$2,%lo(rtl_newGC_session_status_flags)($4)
	li	$3,1			# 0x1
	beq	$2,$3,$L403
	lui	$2,%hi(jiffies)

	lw	$2,%lo(jiffies)($2)
	sw	$3,%lo(rtl_newGC_session_status_flags)($4)
	lui	$3,%hi(rtl_newGC_session_status_time)
	addiu	$2,$2,200
	sw	$2,%lo(rtl_newGC_session_status_time)($3)
$L403:
	move	$2,$0
$L164:
	lw	$31,56($sp)
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
	.end	drop_one_conntrack
	.section	.text.rtl_nf_conn_GC_init,"ax",@progbits
	.align	2
	.globl	rtl_nf_conn_GC_init
	.ent	rtl_nf_conn_GC_init
	.type	rtl_nf_conn_GC_init, @function
rtl_nf_conn_GC_init:
	.set	nomips16
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(Tcp_State_Hash_Head)
	addiu	$sp,$sp,-24
	addiu	$2,$2,%lo(Tcp_State_Hash_Head)
	sw	$31,20($sp)
	sw	$16,16($sp)
	addiu	$2,$2,4
	li	$5,10			# 0xa
$L475:
	lw	$3,0($2)
	addiu	$5,$5,-1
	beq	$3,$0,$L472
	addiu	$2,$2,8

	sw	$3,4($3)
	sw	$3,0($3)
$L472:
	bgez	$5,$L475
	nop

	lui	$2,%hi(Udp_State_Hash_Head)
	addiu	$2,$2,%lo(Udp_State_Hash_Head)
	addiu	$2,$2,4
	li	$5,1			# 0x1
$L481:
	lw	$3,0($2)
	addiu	$5,$5,-1
	beq	$3,$0,$L478
	addiu	$2,$2,8

	sw	$3,4($3)
	sw	$3,0($3)
$L478:
	bgez	$5,$L481
	lui	$16,%hi(prot_limit)

	move	$5,$0
	li	$6,12			# 0xc
	jal	memset
	addiu	$4,$16,%lo(prot_limit)

	lui	$2,%hi(prot_counters)
	addiu	$7,$2,%lo(prot_counters)
	lui	$2,%hi(_prot_limit)
	addiu	$6,$2,%lo(_prot_limit)
	move	$5,$0
	sll	$2,$5,2
$L504:
	addiu	$5,$5,1
	addu	$4,$2,$6
	slt	$3,$5,3
	addu	$2,$2,$7
	sw	$0,0($2)
	sw	$0,0($4)
	bne	$3,$0,$L504
	sll	$2,$5,2

	lui	$2,%hi(nf_conntrack_max)
	lw	$6,%lo(nf_conntrack_max)($2)
	li	$2,2			# 0x2
	addiu	$3,$16,%lo(prot_limit)
	sw	$2,%lo(prot_limit)($16)
	li	$2,90			# 0x5a
	sw	$2,4($3)
	li	$2,60			# 0x3c
	sw	$2,8($3)
	lui	$2,%hi(_prot_limit)
	lui	$7,%hi(rtl_newGC_session_status_flags)
	addiu	$10,$2,%lo(_prot_limit)
	li	$2,1374355456			# 0x51eb0000
	sw	$0,%lo(rtl_newGC_session_status_flags)($7)
	move	$9,$3
	ori	$8,$2,0x851f
	move	$5,$0
	sll	$3,$5,2
$L505:
	addu	$2,$3,$9
	lw	$2,0($2)
	addiu	$5,$5,1
	addu	$3,$3,$10
	mult	$6,$2
	slt	$4,$5,3
	mflo	$2
	multu	$2,$8
	mfhi	$2
	srl	$2,$2,5
	sw	$2,0($3)
	bne	$4,$0,$L505
	sll	$3,$5,2

	li	$3,-859045888			# 0xffffffffcccc0000
	sll	$2,$6,2
	ori	$3,$3,0xcccd
	multu	$2,$3
	lui	$4,%hi(rtl_nf_conntrack_threshold)
	mfhi	$2
	srl	$2,$2,2
	subu	$3,$6,$2
	sltu	$3,$3,65
	bne	$3,$0,$L491
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)

	addiu	$2,$6,-64
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)
$L491:
	lui	$2,%hi(nf_conntrack_max)
	lw	$2,%lo(nf_conntrack_max)($2)
	nop
	beq	$2,$0,$L492
	lui	$2,%hi(init_net+468)

	lw	$3,%lo(init_net+468)($2)
	lw	$2,%lo(rtl_nf_conntrack_threshold)($4)
	nop
	slt	$2,$2,$3
	beq	$2,$0,$L492
	li	$3,3			# 0x3

	lw	$2,%lo(rtl_newGC_session_status_flags)($7)
	nop
	beq	$2,$3,$L495
	lui	$2,%hi(jiffies)

	lw	$2,%lo(jiffies)($2)
	sw	$3,%lo(rtl_newGC_session_status_flags)($7)
	lui	$3,%hi(rtl_newGC_session_status_time)
	addiu	$2,$2,1
	j	$L495
	sw	$2,%lo(rtl_newGC_session_status_time)($3)

$L492:
	sw	$0,%lo(rtl_newGC_session_status_flags)($7)
$L495:
	jal	rtl_nf_conn_GC_init_hooks
	nop

	lw	$31,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	rtl_nf_conn_GC_init
	.globl	rtl_nf_conntrack_threshold
	.section	.bss
	.align	2
	.type	rtl_nf_conntrack_threshold, @object
	.size	rtl_nf_conntrack_threshold, 4
rtl_nf_conntrack_threshold:
	.space	4
	.globl	rtl_newGC_session_status_flags
	.section	.dram-fwd,"aw",@progbits
	.align	2
	.type	rtl_newGC_session_status_flags, @object
	.size	rtl_newGC_session_status_flags, 4
rtl_newGC_session_status_flags:
	.space	4
	.globl	rtl_newGC_session_status_time
	.align	2
	.type	rtl_newGC_session_status_time, @object
	.size	rtl_newGC_session_status_time, 4
rtl_newGC_session_status_time:
	.space	4
	.globl	prot_limit
	.section	.bss
	.align	2
	.type	prot_limit, @object
	.size	prot_limit, 12
prot_limit:
	.space	12
	.local	prot_counters
	.comm	prot_counters,12,4
	.local	_prot_limit
	.comm	_prot_limit,12,4
	.ident	"GCC: (GNU) 3.4.6-1.3.6"
