/*
 * 
 *
 * Copyright (C)2006-2008 by 
 * All rights reserved.
 *
 */
 
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/stat.h>
#include <linux/moduleparam.h>
#include <linux/irq.h>

////////////////////////////////////////////////////////////////////////
//DEBUG macroses
#define MOD_NAME "gpio_rtl8196c"

#ifdef DEBUG
	#define PRINT( format, args ...) printk( "%15s:%3d = " format, MOD_NAME,__LINE__, ##args)
#else
	#define PRINT( format, args ...) ;
#endif

////////////////////////////////////////////////////////////////////////
//SAME DEFINE FOR 8196C/8196E/8196D/8198
#define RTL819X_BASE_ADDR		0xb8000000
#define RTL819X_GPIO_ADDR		(RTL819X_BASE_ADDR + 0x3500)
#define RTL819X_GPIO_PABCD_CNT_ADDR	(RTL819X_GPIO_ADDR + 0x00)
#define RTL819X_GPIO_PABCD_TYPE_ADDR	(RTL819X_GPIO_ADDR + 0x00)
#define RTL819X_GPIO_PABCD_DIR_ADDR	(RTL819X_GPIO_ADDR + 0x08)
#define RTL819X_GPIO_PABCD_DAT_ADDR	(RTL819X_GPIO_ADDR + 0x0c)
#define RTL819X_GPIO_PABCD_ISR_ADDR	(RTL819X_GPIO_ADDR + 0x10)
#define RTL819X_GPIO_PAB_IMR_ADDR	(RTL819X_GPIO_ADDR + 0x14)
#define RTL819X_GPIO_PCD_IMR_ADDR	(RTL819X_GPIO_ADDR + 0x18)

#define RTL819X_MUX_ADDR0		(RTL819X_BASE_ADDR + 0x40)
#define RTL819X_MUX_ADDR1		(RTL819X_MUX_ADDR  + 0x4)

#if defined(CONFIG_RTL_8196C)
#define RTL8196X_MUX_DEF_VALUE		0x340FFF
#elif defined(CONFIG_RTL_8196D)
#define RTL8196X_MUX_DEF_VALUE		0x0
#elif defined(CONFIG_RTL_8196E)
#define RTL8196X_MUX_DEF_VALUE		0x0
#endif

static int g_muxdummy;

//gpio shared pin mapping list
#define RTL8196C_GPIO_SPML_LED_S0	//LED_PORT0,GPIOB2
#define RTL8196C_GPIO_SPML_LED_S1	//LED_PORT1,GPIOB3
#define RTL8196C_GPIO_SPML_LED_S2	//LED_PORT2,GPIOB4
#define RTL8196C_GPIO_SPML_LED_S3	//LED_PORT3,GPIOB5
#define RTL8196C_GPIO_SPML_LED_P0	//LED_PORT4,GPIOB6
#define RTL8196C_GPIO_SPML_LED_P1	//GPIOB7
#define RTL8196C_GPIO_SPML_LED_P2	//GPIOC0
#define RTL8196C_GPIO_SPML_MEM		//NF_CS1#MCS1, DRAM_CKE, GPIOA[1:0]
#define RTL8196C_GPIO_SPML_JTAG		//JTAG, GPIOA[6:2]
#define RTL8196C_GPIO_SPML_UART		
#define RTL8196C_GPIO_SPML_PCIE		

static uint32_t rtl819x_mux_value	=	RTL8196X_MUX_DEF_VALUE;

struct rtl_gpio_chip 
{
	struct gpio_chip	chip;
	uint32_t		gpio_cnt;
	uint32_t		gpio_data;
	uint32_t		gpio_dir;
	volatile uint8_t*	regs;
	spinlock_t		lock;/* Lock used for synchronization */
};

extern struct rtl_gpio_chip gpio_rtl819x;

static inline struct rtl_gpio_chip *to_rtl_gpio_chip(struct gpio_chip *gc)
{
	return container_of(gc, struct rtl_gpio_chip, chip);
}

////////////////////////////////////////////////////////////////////////
//configure ports as GPIO
static int param_set_rtl819x_mux( const char *val, struct kernel_param *kp )
{
	volatile uint32_t *mux = (uint32_t *)RTL819X_MUX_ADDR0;
	unsigned long flags;
	
	PRINT("param_set_rtl819x_mux\n");
	if ( !val )
	{
		return -EINVAL;
	}
	if ( sscanf( val, "0x%x", &rtl819x_mux_value ) < 0 )
	{
		PRINT("err val\n");
		return -EINVAL;
	}
	spin_lock_irqsave(&gpio_rtl819x.lock, flags);
	PRINT("mux set 0x%08x\n",rtl819x_mux_value);
	*mux = rtl819x_mux_value;
	spin_unlock_irqrestore(&gpio_rtl819x.lock, flags);
	return 0;
}

#define param_check_rtl819x_mux(a,b)  ;;

////////////////////////////////////////////////////////////////////////
//get current configured port value
static int param_get_rtl819x_mux( char *buf, struct kernel_param *kp )
{
	volatile uint32_t *mux = (uint32_t*)RTL819X_MUX_ADDR0;
	unsigned long flags;
	
	PRINT("param_get_rtl819x_mux\n");
	spin_lock_irqsave(&gpio_rtl819x.lock, flags);
	sprintf( buf, "0x%x", *mux );
	spin_unlock_irqrestore(&gpio_rtl819x.lock, flags);
	return strlen( buf );
}

module_param_named(rtl819x_mux, g_muxdummy, rtl819x_mux, S_IRUGO | S_IWUSR);

////////////////////////////////////////////////////////////////////////
static int gpio_rtl819x_dir_in( struct gpio_chip *chip, unsigned int gpio )
{
	volatile uint32_t *dir = (uint32_t*)RTL819X_GPIO_PABCD_DIR_ADDR;
	unsigned long flags;
	struct rtl_gpio_chip *rgc = to_rtl_gpio_chip( chip );
	
	PRINT("gpio_rtl819x_dir_in\n");
	PRINT("offset = %d\n", gpio);
	
	spin_lock_irqsave(rgc->lock, flags);
	rgc->gpio_dir &= ~(1 << gpio);
	*dir = rgc->gpio_dir;
	spin_unlock_irqrestore(rgc->lock, flags);
	
	return 0;
}


////////////////////////////////////////////////////////////////////////
static int gpio_rtl819x_dir_out( struct gpio_chip *chip, unsigned int gpio )
{
	volatile uint32_t *dir = (uint32_t*)RTL819X_GPIO_PABCD_DIR_ADDR;
	unsigned long flags;
	struct rtl_gpio_chip *rgc = to_rtl_gpio_chip( chip );
	
	PRINT("gpio_rtl819x_dir_out\n");
	PRINT("offset = %d\n", gpio);
	
	spin_lock_irqsave(rgc->lock, flags);
	rgc->gpio_dir |= (1 << gpio);
	*dir = rgc->gpio_dir;
	spin_unlock_irqrestore(rgc->lock, flags);
	
	return 0;
}


////////////////////////////////////////////////////////////////////////
static int gpio_rtl819x_get( struct gpio_chip *chip, unsigned int gpio )
{
	volatile uint32_t *dat = (uint32_t*)RTL819X_GPIO_PABCD_DAT_ADDR;
	int res=0;
	unsigned long flags;
	struct rtl_gpio_chip *rgc = to_rtl_gpio_chip( chip );
	
	PRINT("gpio_rtl819x_get\n");
	PRINT("offset = %d\n", gpio);
	
	spin_lock_irqsave(rgc->lock, flags);
	//or get it from data register?
	res = ((*dat)>>gpio)&0x1;
	spin_unlock_irqrestore(rgc->lock, flags);
	return res;
}


////////////////////////////////////////////////////////////////////////
static void gpio_rtl819x_set( struct gpio_chip *chip, unsigned int gpio, int value )
{
	volatile uint32_t *dat = (uint32_t*)RTL819X_GPIO_PABCD_DAT_ADDR;
	int res=0;
	unsigned long flags;
	struct rtl_gpio_chip *rgc = to_rtl_gpio_chip( chip );
	
	PRINT("gpio_rtl819x_set\n");
	PRINT("offset = %d = %d\n", gpio, value);
	
	spin_lock_irqsave(rgc->lock, flags);
	if ( value )
		rgc->gpio_data |= (1 << gpio);
	else
		rgc->gpio_data &= ~(1 << gpio);
	*dat = rgc->gpio_data;
	spin_unlock_irqrestore(rgc->lock, flags);
	
}

////////////////////////////////////////////////////////////////////////
static int gpio_rtl819x_to_irq(struct gpio_chip *chip, unsigned offset)
{
	return -EINVAL;
}

////////////////////////////////////////////////////////////////////////
//init module
struct rtl_gpio_chip gpio_rtl819x = 
{
	.chip = {
		.label =		"gpio_rtl819x",
		.direction_input =	gpio_rtl819x_dir_in,
		.get =			gpio_rtl819x_get,
		.direction_output =	gpio_rtl819x_dir_out,
		.set =			gpio_rtl819x_set,
		.to_irq =		gpio_rtl819x_to_irq,
		.base =			0,
		.ngpio =		32,
	},
	.gpio_dir = 0x0,
};

static int __init gpio_rtl819x_init( void )
{
	PRINT("Start\n");
	gpiochip_add( &gpio_rtl819x.chip );
	return 0;
}

////////////////////////////////////////////////////////////////////////
//exit from module
static int gpio_rtl819x_exit( void )
{
	PRINT("End\n");
	gpiochip_remove( &gpio_rtl819x.chip );
	return 0;
}



MODULE_AUTHOR("Artur Artamonov");
MODULE_DESCRIPTION("gpio_rtl8196c");
MODULE_LICENSE("GPL");

module_init( gpio_rtl819x_init );
module_exit( gpio_rtl819x_exit );
