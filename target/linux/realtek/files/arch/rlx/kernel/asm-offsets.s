	.file	1 "asm-offsets.c"
	.section .mdebug.abi32
	.previous

 # -G value = 0, Arch = 4181, ISA = 1
 # GNU C version 3.4.6-1.3.6 (mips-linux)
 #	compiled by GNU C version 3.4.6.
 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed:  -nostdinc -Iinclude
 # -I/home/roman/dev/rsdk/rtl819x/linux-2.6.30/arch/rlx/include
 # -Iinclude/asm-rlx -Iarch/rlx/bsp/
 # -I/home/roman/dev/rsdk/rtl819x/linux-2.6.30/arch/rlx/include/asm/mach-generic
 # -iprefix -U__PIC__ -U__pic__ -D__KERNEL__ -UMIPSEB -U_MIPSEB -U__MIPSEB
 # -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEB
 # -D_MIPSEB -D__MIPSEB -D__MIPSEB__ -DVMLINUX_LOAD_ADDRESS=0x80000000
 # -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(asm_offsets)
 # -DKBUILD_MODNAME=KBUILD_STR(asm_offsets) -isystem -include -MD -G -meb
 # -march=4181 -mno-check-zero-division -mabi=32 -mno-abicalls -msoft-float
 # -auxbase-strip -Os -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs
 # -Werror-implicit-function-declaration -Wdeclaration-after-statement
 # -fno-strict-aliasing -fno-common -fno-delete-null-pointer-checks
 # -ffunction-sections -fno-pic -ffreestanding -fomit-frame-pointer
 # -fverbose-asm
 # options enabled:  -feliminate-unused-debug-types -fdefer-pop
 # -fomit-frame-pointer -foptimize-sibling-calls -funit-at-a-time
 # -fcse-follow-jumps -fcse-skip-blocks -fexpensive-optimizations
 # -fthread-jumps -fstrength-reduce -fpeephole -fforce-mem -ffunction-cse
 # -fkeep-static-consts -fcaller-saves -fpcc-struct-return -fgcse -fgcse-lm
 # -fgcse-sm -fgcse-las -floop-optimize -fcrossjumping -fif-conversion
 # -fif-conversion2 -frerun-cse-after-loop -frerun-loop-opt
 # -fschedule-insns -fschedule-insns2 -fsched-interblock -fsched-spec
 # -fsched-stalled-insns -fsched-stalled-insns-dep -fbranch-count-reg
 # -freorder-functions -fcprop-registers -ffunction-sections -fverbose-asm
 # -fregmove -foptimize-register-move -fargument-alias -fident -fpeephole2
 # -fguess-branch-probability -fmath-errno -ftrapping-math -mgas
 # -msoft-float -meb -mno-check-zero-division -mexplicit-relocs -march=4181
 # -mabi=32 -mno-flush-func_flush_cache -mflush-func=_flush_cache

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
	.section	.text.output_ptreg_defines,"ax",@progbits
	.align	2
	.globl	output_ptreg_defines
	.ent	output_ptreg_defines
	.type	output_ptreg_defines, @function
output_ptreg_defines:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
->#MIPS pt_regs offsets.
	
->PT_R0 24 offsetof(struct pt_regs, regs[0])	 #
	
->PT_R1 28 offsetof(struct pt_regs, regs[1])	 #
	
->PT_R2 32 offsetof(struct pt_regs, regs[2])	 #
	
->PT_R3 36 offsetof(struct pt_regs, regs[3])	 #
	
->PT_R4 40 offsetof(struct pt_regs, regs[4])	 #
	
->PT_R5 44 offsetof(struct pt_regs, regs[5])	 #
	
->PT_R6 48 offsetof(struct pt_regs, regs[6])	 #
	
->PT_R7 52 offsetof(struct pt_regs, regs[7])	 #
	
->PT_R8 56 offsetof(struct pt_regs, regs[8])	 #
	
->PT_R9 60 offsetof(struct pt_regs, regs[9])	 #
	
->PT_R10 64 offsetof(struct pt_regs, regs[10])	 #
	
->PT_R11 68 offsetof(struct pt_regs, regs[11])	 #
	
->PT_R12 72 offsetof(struct pt_regs, regs[12])	 #
	
->PT_R13 76 offsetof(struct pt_regs, regs[13])	 #
	
->PT_R14 80 offsetof(struct pt_regs, regs[14])	 #
	
->PT_R15 84 offsetof(struct pt_regs, regs[15])	 #
	
->PT_R16 88 offsetof(struct pt_regs, regs[16])	 #
	
->PT_R17 92 offsetof(struct pt_regs, regs[17])	 #
	
->PT_R18 96 offsetof(struct pt_regs, regs[18])	 #
	
->PT_R19 100 offsetof(struct pt_regs, regs[19])	 #
	
->PT_R20 104 offsetof(struct pt_regs, regs[20])	 #
	
->PT_R21 108 offsetof(struct pt_regs, regs[21])	 #
	
->PT_R22 112 offsetof(struct pt_regs, regs[22])	 #
	
->PT_R23 116 offsetof(struct pt_regs, regs[23])	 #
	
->PT_R24 120 offsetof(struct pt_regs, regs[24])	 #
	
->PT_R25 124 offsetof(struct pt_regs, regs[25])	 #
	
->PT_R26 128 offsetof(struct pt_regs, regs[26])	 #
	
->PT_R27 132 offsetof(struct pt_regs, regs[27])	 #
	
->PT_R28 136 offsetof(struct pt_regs, regs[28])	 #
	
->PT_R29 140 offsetof(struct pt_regs, regs[29])	 #
	
->PT_R30 144 offsetof(struct pt_regs, regs[30])	 #
	
->PT_R31 148 offsetof(struct pt_regs, regs[31])	 #
	
->PT_HI 152 offsetof(struct pt_regs, hi)	 #
	
->PT_LO 156 offsetof(struct pt_regs, lo)	 #
	
->PT_EPC 160 offsetof(struct pt_regs, cp0_epc)	 #
	
->PT_BVADDR 164 offsetof(struct pt_regs, cp0_badvaddr)	 #
	
->PT_STATUS 168 offsetof(struct pt_regs, cp0_status)	 #
	
->PT_CAUSE 172 offsetof(struct pt_regs, cp0_cause)	 #
	
->PT_SIZE 176 sizeof(struct pt_regs)	 #
	
->
#NO_APP
	j	$31
	.end	output_ptreg_defines
	.section	.text.output_task_defines,"ax",@progbits
	.align	2
	.globl	output_task_defines
	.ent	output_task_defines
	.type	output_task_defines, @function
output_task_defines:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
->#MIPS task_struct offsets.
	
->TASK_STATE 0 offsetof(struct task_struct, state)	 #
	
->TASK_THREAD_INFO 4 offsetof(struct task_struct, stack)	 #
	
->TASK_FLAGS 12 offsetof(struct task_struct, flags)	 #
	
->TASK_MM 216 offsetof(struct task_struct, mm)	 #
	
->TASK_PID 252 offsetof(struct task_struct, pid)	 #
	
->TASK_STRUCT_SIZE 784 sizeof(struct task_struct)	 #
	
->
#NO_APP
	j	$31
	.end	output_task_defines
	.section	.text.output_thread_info_defines,"ax",@progbits
	.align	2
	.globl	output_thread_info_defines
	.ent	output_thread_info_defines
	.type	output_thread_info_defines, @function
output_thread_info_defines:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
->#MIPS thread_info offsets.
	
->TI_TASK 0 offsetof(struct thread_info, task)	 #
	
->TI_EXEC_DOMAIN 4 offsetof(struct thread_info, exec_domain)	 #
	
->TI_FLAGS 8 offsetof(struct thread_info, flags)	 #
	
->TI_TP_VALUE 12 offsetof(struct thread_info, tp_value)	 #
	
->TI_CPU 16 offsetof(struct thread_info, cpu)	 #
	
->TI_PRE_COUNT 20 offsetof(struct thread_info, preempt_count)	 #
	
->TI_ADDR_LIMIT 24 offsetof(struct thread_info, addr_limit)	 #
	
->TI_RESTART_BLOCK 32 offsetof(struct thread_info, restart_block)	 #
	
->TI_REGS 64 offsetof(struct thread_info, regs)	 #
	
->_THREAD_SIZE 32768 THREAD_SIZE	 #
	
->_THREAD_MASK 32767 THREAD_MASK	 #
	
->
#NO_APP
	j	$31
	.end	output_thread_info_defines
	.section	.text.output_thread_defines,"ax",@progbits
	.align	2
	.globl	output_thread_defines
	.ent	output_thread_defines
	.type	output_thread_defines, @function
output_thread_defines:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
->#MIPS specific thread_struct offsets.
	
->THREAD_REG16 512 offsetof(struct task_struct, thread.reg16)	 #
	
->THREAD_REG17 516 offsetof(struct task_struct, thread.reg17)	 #
	
->THREAD_REG18 520 offsetof(struct task_struct, thread.reg18)	 #
	
->THREAD_REG19 524 offsetof(struct task_struct, thread.reg19)	 #
	
->THREAD_REG20 528 offsetof(struct task_struct, thread.reg20)	 #
	
->THREAD_REG21 532 offsetof(struct task_struct, thread.reg21)	 #
	
->THREAD_REG22 536 offsetof(struct task_struct, thread.reg22)	 #
	
->THREAD_REG23 540 offsetof(struct task_struct, thread.reg23)	 #
	
->THREAD_REG29 544 offsetof(struct task_struct, thread.reg29)	 #
	
->THREAD_REG30 548 offsetof(struct task_struct, thread.reg30)	 #
	
->THREAD_REG31 552 offsetof(struct task_struct, thread.reg31)	 #
	
->THREAD_STATUS 556 offsetof(struct task_struct, thread.cp0_status)	 #
	
->THREAD_BVADDR 560 offsetof(struct task_struct, thread.cp0_badvaddr)	 #
	
->THREAD_BUADDR 564 offsetof(struct task_struct, thread.cp0_baduaddr)	 #
	
->THREAD_ECODE 568 offsetof(struct task_struct, thread.error_code)	 #
	
->THREAD_TRAPNO 572 offsetof(struct task_struct, thread.trap_no)	 #
	
->
#NO_APP
	j	$31
	.end	output_thread_defines
	.section	.text.output_mm_defines,"ax",@progbits
	.align	2
	.globl	output_mm_defines
	.ent	output_mm_defines
	.type	output_mm_defines, @function
output_mm_defines:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
->#Size of struct page
	
->STRUCT_PAGE_SIZE 32 sizeof(struct page)	 #
	
->
	
->#Linux mm_struct offsets.
	
->MM_USERS 40 offsetof(struct mm_struct, mm_users)	 #
	
->MM_PGD 36 offsetof(struct mm_struct, pgd)	 #
	
->MM_CONTEXT 328 offsetof(struct mm_struct, context)	 #
	
->
	
->_PAGE_SIZE 4096 PAGE_SIZE	 #
	
->_PAGE_SHIFT 12 PAGE_SHIFT	 #
	
->
	
->_PGD_T_SIZE 4 sizeof(pgd_t)	 #
	
->_PMD_T_SIZE 4 sizeof(pmd_t)	 #
	
->_PTE_T_SIZE 4 sizeof(pte_t)	 #
	
->
	
->_PGD_T_LOG2 2 PGD_T_LOG2	 #
	
->_PMD_T_LOG2 2 PMD_T_LOG2	 #
	
->_PTE_T_LOG2 2 PTE_T_LOG2	 #
	
->
	
->_PGD_ORDER 0 PGD_ORDER	 #
	
->_PMD_ORDER 1 PMD_ORDER	 #
	
->_PTE_ORDER 0 PTE_ORDER	 #
	
->
	
->_PMD_SHIFT 22 PMD_SHIFT	 #
	
->_PGDIR_SHIFT 22 PGDIR_SHIFT	 #
	
->
	
->_PTRS_PER_PGD 1024 PTRS_PER_PGD	 #
	
->_PTRS_PER_PMD 1 PTRS_PER_PMD	 #
	
->_PTRS_PER_PTE 1024 PTRS_PER_PTE	 #
	
->
#NO_APP
	j	$31
	.end	output_mm_defines
	.section	.text.output_sc_defines,"ax",@progbits
	.align	2
	.globl	output_sc_defines
	.ent	output_sc_defines
	.type	output_sc_defines, @function
output_sc_defines:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
->#Linux sigcontext offsets.
	
->SC_REGS 16 offsetof(struct sigcontext, sc_regs)	 #
	
->SC_MDHI 272 offsetof(struct sigcontext, sc_mdhi)	 #
	
->SC_MDLO 280 offsetof(struct sigcontext, sc_mdlo)	 #
	
->SC_PC 8 offsetof(struct sigcontext, sc_pc)	 #
	
->
#NO_APP
	j	$31
	.end	output_sc_defines
	.section	.text.output_signal_defined,"ax",@progbits
	.align	2
	.globl	output_signal_defined
	.ent	output_signal_defined
	.type	output_signal_defined, @function
output_signal_defined:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
->#Linux signal numbers.
	
->_SIGHUP 1 SIGHUP	 #
	
->_SIGINT 2 SIGINT	 #
	
->_SIGQUIT 3 SIGQUIT	 #
	
->_SIGILL 4 SIGILL	 #
	
->_SIGTRAP 5 SIGTRAP	 #
	
->_SIGIOT 6 SIGIOT	 #
	
->_SIGABRT 6 SIGABRT	 #
	
->_SIGEMT 7 SIGEMT	 #
	
->_SIGFPE 8 SIGFPE	 #
	
->_SIGKILL 9 SIGKILL	 #
	
->_SIGBUS 10 SIGBUS	 #
	
->_SIGSEGV 11 SIGSEGV	 #
	
->_SIGSYS 12 SIGSYS	 #
	
->_SIGPIPE 13 SIGPIPE	 #
	
->_SIGALRM 14 SIGALRM	 #
	
->_SIGTERM 15 SIGTERM	 #
	
->_SIGUSR1 16 SIGUSR1	 #
	
->_SIGUSR2 17 SIGUSR2	 #
	
->_SIGCHLD 18 SIGCHLD	 #
	
->_SIGPWR 19 SIGPWR	 #
	
->_SIGWINCH 20 SIGWINCH	 #
	
->_SIGURG 21 SIGURG	 #
	
->_SIGIO 22 SIGIO	 #
	
->_SIGSTOP 23 SIGSTOP	 #
	
->_SIGTSTP 24 SIGTSTP	 #
	
->_SIGCONT 25 SIGCONT	 #
	
->_SIGTTIN 26 SIGTTIN	 #
	
->_SIGTTOU 27 SIGTTOU	 #
	
->_SIGVTALRM 28 SIGVTALRM	 #
	
->_SIGPROF 29 SIGPROF	 #
	
->_SIGXCPU 30 SIGXCPU	 #
	
->_SIGXFSZ 31 SIGXFSZ	 #
	
->
#NO_APP
	j	$31
	.end	output_signal_defined
	.section	.text.output_irq_cpustat_t_defines,"ax",@progbits
	.align	2
	.globl	output_irq_cpustat_t_defines
	.ent	output_irq_cpustat_t_defines
	.type	output_irq_cpustat_t_defines, @function
output_irq_cpustat_t_defines:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
->#Linux irq_cpustat_t offsets.
	
->IC_SOFTIRQ_PENDING 0 offsetof(irq_cpustat_t, __softirq_pending)	 #
	
->IC_IRQ_CPUSTAT_T 32 sizeof(irq_cpustat_t)	 #
	
->
#NO_APP
	j	$31
	.end	output_irq_cpustat_t_defines
	.ident	"GCC: (GNU) 3.4.6-1.3.6"
