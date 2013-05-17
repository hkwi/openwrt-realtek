	.file	1 "bounds.c"
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
 # -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(bounds)
 # -DKBUILD_MODNAME=KBUILD_STR(bounds) -isystem -include -MD -G -meb
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

	.section	.text.foo,"ax",@progbits
	.align	2
	.globl	foo
	.ent	foo
	.type	foo, @function
foo:
	.set	nomips16
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
->NR_PAGEFLAGS 23 __NR_PAGEFLAGS	 #
	
->MAX_NR_ZONES 2 __MAX_NR_ZONES	 #
#NO_APP
	j	$31
	.end	foo
	.ident	"GCC: (GNU) 3.4.6-1.3.6"
