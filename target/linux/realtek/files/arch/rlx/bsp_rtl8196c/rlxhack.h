#ifndef __RLXHACK_H
#define __RLXHACK_H

/*
 * Register access macro
 */
#ifndef REG32
#define REG32(reg)		(*(volatile unsigned int   *)((unsigned int)reg))
#define REG16(reg)		(*(volatile unsigned short *)((unsigned int)reg))
#define REG08(reg)		(*(volatile unsigned char  *)((unsigned int)reg))
#define REG8(reg)		(*(volatile unsigned char  *)((unsigned int)reg))

#define WRITE_MEM32(addr, val)	(*(volatile unsigned int *)   (addr)) = (val)
#define READ_MEM32(addr)	(*(volatile unsigned int *)   (addr))
#define WRITE_MEM16(addr, val)	(*(volatile unsigned short *) (addr)) = (val)
#define READ_MEM16(addr)	(*(volatile unsigned short *) (addr))
#define WRITE_MEM8(addr, val)	(*(volatile unsigned char *)  (addr)) = (val)
#define READ_MEM8(addr)		(*(volatile unsigned char *)  (addr))

#endif

//config options
#define PRINTF printk
#define COLORIZE
#define PRINT_LINENUM
#define PRINT_FILENAME
#define PRINT_DEBUG


//use color
#ifdef COLORIZE
	#define COLOR "1;32m"
	#define COLOR_S "\033[" COLOR
	#define COLOR_E "\033[0m"
#else
	#define COLOR
	#define COLOR_S
	#define COLOR_E
#endif

//print debug line
#ifdef PRINT_LINENUM
	#define PRINT_LINE_F "LINE:%d "
	#define PRINT_LINE_D __LINE__
#else
	#define PRINT_LINE_F ""
	#define PRINT_LINE_D ""
#endif

//print
#ifdef PRINT_FILENAME
	#define PRINT_FILE_F "FILE:%s "
	#define PRINT_FILE_D __FILE__
#else
	#define PRINT_FILE_F ""
	#define PRINT_FILE_D ""
#endif

//print debug string
#ifdef PRINT_DEBUG
	#define PRINT_DEBUG_F "Debug: "
#else
	#define PRINT_DEBUG_F ""
#endif

#define PRINT( format, args ... ) PRINTF( COLOR_S PRINT_DEBUG_F \
	PRINT_FILE_F PRINT_LINE_F format COLOR_E, PRINT_FILE_D, \
	PRINT_LINE_D, ##args);

#endif
