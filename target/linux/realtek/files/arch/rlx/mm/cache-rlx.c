/*
 * cache-rlx.c: RLX specific mmu/cache code.
 * Realtek Semiconductor Corp.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>

#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/mmu_context.h>
#include <asm/system.h>
#include <asm/isadep.h>
#include <asm/io.h>
#include <asm/bootinfo.h>
#include <asm/cpu.h>
#include <asm/cpu-features.h>
#include <asm/wbflush.h>

#include <asm/rlxbsp.h>

/*
 * Determine whether CPU has CACHE OP
 */
#if defined(CONFIG_CPU_RLX4181) || defined(CONFIG_CPU_RLX5181) || \
    defined(CONFIG_CPU_RLX4281) || defined(CONFIG_CPU_RLX5281)
#define CONFIG_CPU_HAS_CACHE_OP
#else
#undef CONFIG_CPU_HAS_CACHE_OP
#endif

/*
 * DCACHE part 
 */
#if defined(CONFIG_CPU_HAS_WBC) || defined(CONFIG_CPU_HAS_L2C)
static void inline rlx_dcache_flush_all(void)
{
    __asm__ __volatile__(
       ".set\tpush\n"
       ".set\tnoreorder\n"
       "\tmtc0\t$0, $20\n"
       "\tli\t$8, 0x200\n"
       "\tmtc0\t$8, $20\n"
       ".set\tpop\n");
}

__attribute__  ((section(".iram-gen")))
static void rlx_dcache_flush_range(unsigned long start, unsigned long end)
{
#ifdef CONFIG_CPU_HAS_CACHE_OP
    unsigned long size, i, flags;
    volatile unsigned char *p;

    start &= ~cpu_dcache_line_mask;
    size = end - start;
    if (size >= cpu_dcache_size * 2)
      {
        rlx_dcache_flush_all();
        return;
      }

    p = (char *)start;
    flags = read_c0_status();

    /* disable interrupt */
    write_c0_status(flags & ~ST0_IEC);

    /* 0x10 = IInval   */
    /* 0x11 = DInval   */
    /* 0x15 = DWBInval */
    /* 0x19 = DWB      */

    for (i = 0; i < size; i += 0x080) {
        asm (
#if (cpu_dcache_line == 16)
             "cache 0x15, 0x000(%0)\n\t"
             "cache 0x15, 0x010(%0)\n\t"
             "cache 0x15, 0x020(%0)\n\t"
             "cache 0x15, 0x030(%0)\n\t"
             "cache 0x15, 0x040(%0)\n\t"
             "cache 0x15, 0x050(%0)\n\t"
             "cache 0x15, 0x060(%0)\n\t"
             "cache 0x15, 0x070(%0)\n\t"
#else
             "cache 0x15, 0x000(%0)\n\t"
             "cache 0x15, 0x020(%0)\n\t"
             "cache 0x15, 0x040(%0)\n\t"
             "cache 0x15, 0x060(%0)\n\t"
#endif
             : : "r" (p) );
        p += 0x080;
    }

    /* restore interrupt */
    write_c0_status(flags);
#else
    rlx_dcache_flush_all();
#endif
}

void rlx_dcache_wb_all(void)
{
    __asm__ __volatile__(
       ".set\tpush\n"
       ".set\tnoreorder\n"
       "\tmtc0\t$0, $20\n"
       "\tli\t$8, 0x100\n"
       "\tmtc0\t$8, $20\n"
       ".set\tpop\n");
}

static void rlx_dcache_wb_range(unsigned long start, unsigned long end)
{
 #ifdef CONFIG_CPU_HAS_CACHE_OP
    unsigned long size, i, flags;
    volatile unsigned char *p;

    start &= ~cpu_dcache_line_mask;
    size = end - start;
    if (size >= cpu_dcache_size * 2)
      {
        rlx_dcache_wb_all();
        return;
      }

    p = (char *)start;
    flags = read_c0_status();

    /* disable interrupt */
    write_c0_status(flags & ~ST0_IEC);

    /* 0x10 = IInval   */
    /* 0x11 = DInval   */
    /* 0x15 = DWBInval */
    /* 0x19 = DWB      */
    for (i = 0; i < size; i += 0x080) {
        asm (
#if (cpu_dcache_line == 16)
             "cache 0x19, 0x000(%0)\n\t"
             "cache 0x19, 0x010(%0)\n\t"
             "cache 0x19, 0x020(%0)\n\t"
             "cache 0x19, 0x030(%0)\n\t"
             "cache 0x19, 0x040(%0)\n\t"
             "cache 0x19, 0x050(%0)\n\t"
             "cache 0x19, 0x060(%0)\n\t"
             "cache 0x19, 0x070(%0)\n\t"
#else
             "cache 0x19, 0x000(%0)\n\t"
             "cache 0x19, 0x020(%0)\n\t"
             "cache 0x19, 0x040(%0)\n\t"
             "cache 0x19, 0x060(%0)\n\t"
#endif
             : : "r" (p) );
        p += 0x080;
    }

    /* restore interrupt */
    write_c0_status(flags);
 #else
    rlx_dcache_wb_all();
 #endif
}
#else /* not CONFIG_CPU_HAS_WBC and not CONFIG_CPU_HAS_L2C */
static void rlx_dcache_flush_all(void)
{
    __asm__ __volatile__(
       ".set\tpush\n"
       ".set\tnoreorder\n"
       "\tmtc0\t$0, $20\n"
       "\tli\t$8, 0x1\n"
       "\tmtc0\t$8, $20\n"
       ".set\tpop\n");

    return;
}

static void rlx_dcache_flush_range(unsigned long start, unsigned long end)
{
#ifdef CONFIG_CPU_HAS_CACHE_OP
    unsigned long size, i, flags;
    volatile unsigned char *p;

    start &= ~cpu_dcache_line_mask;
    size = end - start;
    if (size >= cpu_dcache_size * 2)
      {
        rlx_dcache_flush_all();
        return;
      }

    p = (char *)start;
    flags = read_c0_status();

    /* disable interrupt */
    write_c0_status(flags &~ ST0_IEC);

    /* 0x10 = IInval   */
    /* 0x11 = DInval   */
    /* 0x15 = DWBInval */
    /* 0x19 = DWB      */
    for (i = 0; i < size; i += 0x080) {
        asm (
#if (cpu_dcache_line == 16)
             "cache 0x11, 0x000(%0)\n\t"
             "cache 0x11, 0x010(%0)\n\t"
             "cache 0x11, 0x020(%0)\n\t"
             "cache 0x11, 0x030(%0)\n\t"
             "cache 0x11, 0x040(%0)\n\t"
             "cache 0x11, 0x050(%0)\n\t"
             "cache 0x11, 0x060(%0)\n\t"
             "cache 0x11, 0x070(%0)\n\t"
#else
             "cache 0x11, 0x000(%0)\n\t"
             "cache 0x11, 0x020(%0)\n\t"
             "cache 0x11, 0x040(%0)\n\t"
             "cache 0x11, 0x060(%0)\n\t"
#endif
             : : "r" (p) );
        p += 0x080;
    }

    /* restore interrupt */
    write_c0_status(flags);
#else
    rlx_dcache_flush_all();
#endif
}

void rlx_dcache_wb_all(void)
{
}

static void rlx_dcache_wb_range(unsigned long start, unsigned long end)
{
}
#endif /* CONFIG_CPU_HAS_WBC or CONFIG_CPU_HAS_L2C */

/*
 * ICACHE part
 */
static void rlx_icache_flush_all(void)
{
    __asm__ __volatile__(
       ".set\tpush\n"
       ".set\tnoreorder\n"
       "\tmtc0\t$0, $20\n"
       "\tli\t$8, 0x2\n"
       "\tmtc0\t$8, $20\n"
       ".set\tpop\n");
}

static void rlx_icache_flush_range(unsigned long start, unsigned long end)
{
#if defined(CONFIG_CPU_RLX4281) || defined(CONFIG_CPU_RLX5281)
    unsigned long size, i, flags;
    volatile unsigned char *p;

    rlx_dcache_wb_range(start, end);

    start &= ~cpu_icache_line_mask;
    size = end - start;
    if (size >= cpu_icache_size * 2)
      {
        rlx_icache_flush_all();
        return;
      }

    p = (char *)start;
    flags = read_c0_status();

    /* disable interrupt */
    write_c0_status(flags &~ ST0_IEC);

    /* 0x10 = IInval   */
    /* 0x11 = DInval   */
    /* 0x15 = DWBInval */
    /* 0x19 = DWB      */
    for (i = 0; i < size; i += 0x080) {
        asm (
#if (cpu_icache_line == 16)
             "cache 0x10, 0x000(%0)\n\t"
             "cache 0x10, 0x010(%0)\n\t"
             "cache 0x10, 0x020(%0)\n\t"
             "cache 0x10, 0x030(%0)\n\t"
             "cache 0x10, 0x040(%0)\n\t"
             "cache 0x10, 0x050(%0)\n\t"
             "cache 0x10, 0x060(%0)\n\t"
             "cache 0x10, 0x070(%0)\n\t"
#else
             "cache 0x10, 0x000(%0)\n\t"
             "cache 0x10, 0x020(%0)\n\t"
             "cache 0x10, 0x040(%0)\n\t"
             "cache 0x10, 0x060(%0)\n\t"
#endif
             : : "r" (p) );
        p += 0x080;
    }

    /* restore interrupt */
    write_c0_status(flags);
#else
    rlx_dcache_wb_range(start, end);
    rlx_icache_flush_all();
#endif
}

static inline void rlx_cache_flush_all(void)
{
}

static inline void __rlx_cache_flush_all(void)
{
	rlx_dcache_flush_all();
	rlx_icache_flush_all();
}

static void rlx_cache_flush_mm(struct mm_struct *mm)
{
}

static void rlx_cache_flush_range(struct vm_area_struct *vma,
				  unsigned long start, unsigned long end)
{
}

static void rlx_cache_flush_page(struct vm_area_struct *vma,
				 unsigned long addr, unsigned long pfn)
{
	unsigned long kaddr = KSEG0ADDR(pfn << PAGE_SHIFT);
	int exec = vma->vm_flags & VM_EXEC;
	struct mm_struct *mm = vma->vm_mm;
	pgd_t *pgdp;
	pud_t *pudp;
	pmd_t *pmdp;
	pte_t *ptep;

	pr_debug("cpage[%08lx,%08lx]\n",
		 cpu_context(smp_processor_id(), mm), addr);

	/* No ASID => no such page in the cache.  */
	if (cpu_context(smp_processor_id(), mm) == 0)
		return;

	pgdp = pgd_offset(mm, addr);
	pudp = pud_offset(pgdp, addr);
	pmdp = pmd_offset(pudp, addr);
	ptep = pte_offset(pmdp, addr);

	/* Invalid => no such page in the cache.  */
	if (!(pte_val(*ptep) & _PAGE_PRESENT))
		return;

	rlx_dcache_flush_range(kaddr, kaddr + PAGE_SIZE);
	if (exec)
      rlx_icache_flush_range(kaddr, kaddr + PAGE_SIZE);
}

static void local_rlx_dcache_flush_page(void *addr)
{
}

static void rlx_dcache_flush_page(unsigned long addr)
{
}

static void rlx_cache_flush_sigtramp(unsigned long addr)
{
    unsigned long flags;

    pr_debug("csigtramp[%08lx]\n", addr);

    flags = read_c0_status();

    /* disable interrupt */
    write_c0_status(flags&~ST0_IEC);

#if defined(CONFIG_CPU_HAS_WBC) || defined(CONFIG_CPU_HAS_L2C)
 #ifndef CONFIG_CPU_HAS_CACHE_OP
    rlx_dcache_flush_all();
 #else
    asm (   "cache\t0x19, 0x000(%0)\n\t" : : "r" (addr) );
 #endif
#endif

#if defined(CONFIG_CPU_RLX4281) || defined(CONFIG_CPU_RLX5281)
    asm (   "cache\t0x10, 0x000(%0)\n\t" : : "r" (addr) );
#else
    rlx_icache_flush_all();
#endif

    /* restore interrupt */
    write_c0_status(flags);
}

static void rlx_dma_cache_wback_inv(unsigned long start, unsigned long size)
{
	/* Catch bad driver code */
	BUG_ON(size == 0);

	iob();
    rlx_dcache_flush_range(start, start + size);
}

void __cpuinit rlx_cache_init(void)
{
	extern void build_clear_page(void);
	extern void build_copy_page(void);

          flush_cache_all     =   rlx_cache_flush_all;
        __flush_cache_all     = __rlx_cache_flush_all;
          flush_cache_mm      =   rlx_cache_flush_mm;
          flush_cache_range   =   rlx_cache_flush_range;
          flush_cache_page    =   rlx_cache_flush_page;
          flush_icache_range  =   rlx_icache_flush_range;
    local_flush_icache_range  =   rlx_icache_flush_range;
	local_flush_data_cache_page = local_rlx_dcache_flush_page;
	      flush_data_cache_page = rlx_dcache_flush_page;
	      flush_cache_sigtramp  = rlx_cache_flush_sigtramp;

	_dma_cache_wback_inv = rlx_dma_cache_wback_inv;
	_dma_cache_wback = rlx_dma_cache_wback_inv;
	_dma_cache_inv = rlx_dma_cache_wback_inv;

    printk("icache: %dkB/%dB, dcache: %dkB/%dB, scache: %dkB/%dB\n",
           cpu_icache_size >> 10, cpu_icache_line,
           cpu_dcache_size >> 10, cpu_dcache_line,
           cpu_scache_size >> 10, cpu_scache_line);

	build_clear_page();
	build_copy_page();
}
