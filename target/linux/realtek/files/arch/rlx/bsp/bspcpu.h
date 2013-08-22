/*
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 * 
 * Copyright (C) 2006, Realtek Semiconductor Corp.
 * Copyright (C) 2013, Artur Artamonov (artur@advem.lv)
 */

#ifndef __LEXRA_BSPCPU_H
#define __LEXRA_BSPCPU_H

#if defined(CONFIG_RTL_8196C)

#define cpu_scache_size		0
#define cpu_dcache_size		( 8 << 10)
#define cpu_icache_size		(16 << 10)
#define cpu_scache_line		0
#define cpu_dcache_line		16
#define cpu_icache_line		16
#define cpu_dcache_line_mask	0xF  /*cpu_dcache_line-1*/
#define cpu_icache_line_mask	0xF  /*cpu_icache_line-1*/
#define cpu_tlb_entry		32
//#define cpu_mem_size        (64 << 20)
#define cpu_mem_size		(32 << 20)
//#define cpu_mem_size        ((7 << 20)-16) //reserve 16 byte for firmware header

#define cpu_imem_size		0
#define cpu_dmem_size		0
#define cpu_smem_size		0

#elif defined(CONFIG_RTL_819XD)

#define cpu_scache_size		0
#define cpu_dcache_size		( 32 << 10)
#define cpu_icache_size		(64 << 10)
#define cpu_scache_line		0
#define cpu_dcache_line		32
#define cpu_icache_line		32
#define cpu_dcache_line_mask	0x1F /*cpu_dcache_line-1*/ 
#define cpu_icache_line_mask	0x1F /*cpu_icache_line-1*/
#define cpu_tlb_entry		32

//#define cpu_mem_size        (64 << 20)
//#define cpu_mem_size        (16 << 20)

#define cpu_imem_size		0
#define cpu_dmem_size		0
#define cpu_smem_size		0

#endif

#endif
