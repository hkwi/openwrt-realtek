#ifndef __BOOTCODE__ 
#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>
#include <platform.h>		// GPIO_ABCD_IRQ, PABCD_ISR
#include <asm/delay.h>
#endif

#ifdef __BOOTCODE__
#define ENODEV		19      /* No such device */
#define __init
#endif

#if defined( LINUX_VERSION_CODE ) && (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,30))
#define LINUX_KERNEL_VERSION_2_6_30		1
#endif

#ifdef LINUX_KERNEL_VERSION_2_6_30
#include "bspchip.h"		// BSP_GPIO_EFGH_IRQ
#endif


// ------------------------------------------------------------------
// ------------------------------------------------------------------
// Basic setup information 


// ------------------------------------------------------------------
// ------------------------------------------------------------------
// I2C level function 

#ifndef __BOOTCODE__
#include "i2c.h"
#else
#include "i2c.h"
#endif

#define I2C_GPIO_ID( port, pin )		( ( ( port - 'A' ) << 16 ) | ( pin ) )

#ifdef CONFIG_SND_RTL8197D_SOC_ALC5628
#define ALC5628_SCL 		I2C_GPIO_ID( 'G', 4 )	// SCL = G4
#define ALC5628_SDA 		I2C_GPIO_ID( 'G', 6 )	// SDA = G6
#endif

#define ALC5628_I2C_ADDR	0x30
#define ALC5628_I2C_WRITE	0x00
#define ALC5628_I2C_READ	0x01

static i2c_dev_t alc5628_i2c_dev = {
	.sclk	= ALC5628_SCL,
	.sdio	= ALC5628_SDA,
};

// Register address byte
//  7  : not used
//  6~3: A[3:0] UART's internal register select
//  2~1: channel select: CH1 = 0?!, CH0 = 0
//  0  : not used 
#define MK_SC16IS7X0_REG_ADDR( uart_reg )	( ( uart_reg & 0x0F ) << 3 )

unsigned int serial_in_i2c(unsigned int addr, int offset)
{
	unsigned short int data_hibyte=0;
	unsigned short int data_lowbyte=0;
	unsigned short int data;
	
	//printk( "serial_in_i2c(%X):%X\n", addr, offset );
	
	if( addr != ALC5628_I2C_ADDR )
		return 0;
	
	// start 
	i2c_start_condition( &alc5628_i2c_dev );
	
	// addr + write
	i2c_serial_write_byte( &alc5628_i2c_dev, ALC5628_I2C_ADDR | 
											   ALC5628_I2C_WRITE );
	
	// read ACK 
	if( i2c_ACK( &alc5628_i2c_dev ) != 0 )
		return 0;
	
	// write register address 
	i2c_serial_write_byte( &alc5628_i2c_dev, offset );
	
	// read ACK 
	if( i2c_ACK( &alc5628_i2c_dev ) != 0 )
		return 0;
	
	// start 
	i2c_start_condition( &alc5628_i2c_dev );

	// addr + read
	i2c_serial_write_byte( &alc5628_i2c_dev, ALC5628_I2C_ADDR | 
											   ALC5628_I2C_READ );
	
	// read ACK 
	if( i2c_ACK( &alc5628_i2c_dev ) != 0 )
		return 0;
		
	// read data_hibyte
	i2c_serial_read( &alc5628_i2c_dev, &data_hibyte );

	//write ACK
	i2c_ACK_w(&alc5628_i2c_dev, 0);

	// read data_lowbyte
	i2c_serial_read( &alc5628_i2c_dev, &data_lowbyte );

	data = (data_hibyte<<8) | data_lowbyte;

	// write negative-ACK
	i2c_ACK_w( &alc5628_i2c_dev, 1 );
	
	// stop
	i2c_stop_condition( &alc5628_i2c_dev );
	
	//printk( "in[%X]\n", data );
	
	return data;
}
EXPORT_SYMBOL_GPL(serial_in_i2c);

unsigned int serial_out_i2c(unsigned int addr, int offset, int value)
{
	//printk( "serial_out_i2c(%X):%X,%X\n", addr, offset, value );
	unsigned short int data_hibyte;
	unsigned short int data_lowbyte;

	data_hibyte =(unsigned char) (value>>8);
	data_lowbyte =(unsigned char) (value & 0xff);

	if( addr != ALC5628_I2C_ADDR )
		return 0;
	//printk("(%d)", __LINE__);
	// start 
	i2c_start_condition( &alc5628_i2c_dev );
	
	// addr + write
	i2c_serial_write_byte( &alc5628_i2c_dev, ALC5628_I2C_ADDR | ALC5628_I2C_WRITE );
	
	// read ACK 
	if( i2c_ACK( &alc5628_i2c_dev ) != 0 )
		return 0;
	//printk("(%d)", __LINE__);
	// write register address 
	i2c_serial_write_byte( &alc5628_i2c_dev, offset );
	
	// read ACK 
	if( i2c_ACK( &alc5628_i2c_dev ) != 0 )
		return 0;
	//printk("(%d)", __LINE__);
	// write data hibyte
	i2c_serial_write_byte( &alc5628_i2c_dev, data_hibyte );

	// read ACK 
	if( i2c_ACK( &alc5628_i2c_dev ) != 0 )
		return 0;
	//printk("(%d)", __LINE__);
	// write data lowbyte
	i2c_serial_write_byte( &alc5628_i2c_dev, data_lowbyte );

	// read ACK 
	if( i2c_ACK( &alc5628_i2c_dev ) != 0 )
		return 0;
	//printk("(%d)", __LINE__);
	// stop
	i2c_stop_condition( &alc5628_i2c_dev );
	
	return 0;
}
EXPORT_SYMBOL_GPL(serial_out_i2c);

static void __init alc5628_init_i2c( void )
{
	int temp;
	// init SCL / SDA 
	i2c_init_SCL_SDA( &alc5628_i2c_dev );
	
	serial_in_i2c( ALC5628_I2C_ADDR, 0 );		// avoid NO ACK at first time access

	serial_out_i2c( ALC5628_I2C_ADDR, 0x0, 0x707 );
	serial_out_i2c( ALC5628_I2C_ADDR, 0x0, 0x707 );
	
	serial_in_i2c( ALC5628_I2C_ADDR, 0 );
	serial_in_i2c( ALC5628_I2C_ADDR, 0 );


// enable playback
	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x3E );
	serial_out_i2c( ALC5628_I2C_ADDR, 0x3E, (temp) | 0x8000 );

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x3C );
	serial_out_i2c( ALC5628_I2C_ADDR, 0x3C, (temp) | 0x2000 );

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x3C );
	serial_out_i2c( ALC5628_I2C_ADDR, 0x3C, (temp) | 0x7F0 );

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x3A );
	serial_out_i2c( ALC5628_I2C_ADDR, 0x3A, (temp) | 0x8030 );

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x3E );
	serial_out_i2c( ALC5628_I2C_ADDR, 0x3E, (temp) | 0x600 );

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x0C );
	serial_out_i2c( ALC5628_I2C_ADDR, 0x0C, (temp&(~0xBFBF)) | 0x1010 );


	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x1C );
	serial_out_i2c( ALC5628_I2C_ADDR, 0x1C, (temp) | 0x300 );

//	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x0C );
//	serial_out_i2c( ALC5628_I2C_ADDR, 0x0C, (temp&(~0xBFBF)) | 0x1010 );


	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0 );
	//printk("reg0=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x2 );
	//printk("reg2=%x\n", temp);

	serial_out_i2c( ALC5628_I2C_ADDR, 0x2, 0x0 );

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x2 );
	//printk("reg2=%x\n", temp);

	serial_out_i2c( ALC5628_I2C_ADDR, 0x2, 0xFFFF );

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x2 );
	//printk("reg2=%x\n", temp);



	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x4 );
	//printk("reg4=%x\n", temp);

	serial_out_i2c( ALC5628_I2C_ADDR, 0x4, 0x0 );

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x4 );
	//printk("reg4=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0xA );
	//printk("regA=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0xC );
	//printk("regC=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x16 );
	//printk("reg16=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x1C );
	//printk("reg1C=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x34 );
	//printk("reg34=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x38 );
	//printk("reg38=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x3A );
	//printk("reg3A=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x3C );
	//printk("reg3C=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x3E );
	//printk("reg3E=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x40 );
	//printk("reg40=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x42 );
	//printk("reg42=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x44 );
	//printk("reg44=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x5A );
	//printk("reg5A=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x5C );
	//printk("reg5C=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x5E );
	//printk("reg5E=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x68 );
	//printk("reg68=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x6A );
	//printk("reg6A=%x\n", temp);

	temp = serial_in_i2c( ALC5628_I2C_ADDR, 0x6C );
	//printk("reg6C=%x\n", temp);
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------
// Early printk or bootcode 

#define rtlRegRead(addr)        \
        (*(volatile u32 *)addr)

#define rtlRegWrite(addr, val)  \
        ((*(volatile u32 *)addr) = (val))

static inline u32 rtlRegMask(u32 addr, u32 mask, u32 value)
{
	u32 reg;

	reg = rtlRegRead(addr);
	reg &= ~mask;
	reg |= value & mask;
	rtlRegWrite(addr, reg);
	reg = rtlRegRead(addr); /* flush write to the hardware */

	return reg;
}

static int __init alc5628_init(void)
{
	int ret;
	rtlRegMask(0xb8000010, 0x03DCB000, 0x01DCB000);//enable iis controller clock
	rtlRegMask(0xb8000058, 0x00000001, 0x00000001);//enable 24p576mHz clock

	/* Configure the I2S pins in correct mode */
#if 1 // set the jtag as iis-audio
	rtlRegMask(0xb8000040, 0x00000007, 0x00000003);//change pin mux to iis-voice pin
#else // set the led-phase or lec-sig as iis-audio
	rtlRegMask(0xb8000044, 0x001F80DB, 0x00000049);//change pin mux to iis-voice pin
#endif

	alc5628_init_i2c();

	return 0;
}

#ifndef __BOOTCODE__
module_init(alc5628_init);

MODULE_AUTHOR("Realtek");
MODULE_DESCRIPTION("i2c driver for ALC5628");
MODULE_LICENSE("GPL");
#endif