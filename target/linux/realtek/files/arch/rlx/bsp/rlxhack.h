#ifndef __RLXHACK_H
#define __RLXHACK_H

/*
 * Register access macro
 */

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
