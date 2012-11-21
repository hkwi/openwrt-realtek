/*
 * RTL819X gpio driver
 *
 * Copyright 2012 Andrew 'Necromant' Andrianov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 *
 * Please note, that this driver is VERY hacky due to f*cked up nature of
 * realtek's kernel. Say thanks to code monkeys from realtek 
 * (see arch/rlx for emailsand names) and never hire any of them ;)
 * This driver should be properly rewritten once 3.x kernels for rlx arrive
 * Till then it's a placeholder for a proper driver.
 * Current Issues: 
 *                 * No interrupt support whatsoever.
 *                 * No proper probing & platform_device defs.
 *                 * Many things hardcoded. Sorry
 * 
 * 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/spinlock.h>
#include <asm/io.h>

/* Register Offset Definitions */ 
#define RTLGPIO_CNTRL      (0)
#define RTLGPIO_DIR        (2)
#define RTLGPIO_DATA       (3)

/* rlx missing sizes.h ? */
#define SZ_4K 4096

#define RTL_MUX_ADDR 0xb8000040


static int g_muxdummy;
static uint32_t g_mux_value = 0x340FFF;

static int param_set_mux(const char *val, struct kernel_param *kp)
{
	volatile uint32_t* mux  = (uint32_t*) RTL_MUX_ADDR;
        if (!val) 
		return -EINVAL;
	sscanf(val, "0x%x", &g_mux_value);
	*mux = g_mux_value; 
	printk("rtl-gpio: setting mux to 0x%x\n", g_mux_value);
	return 0;
}

static int param_get_mux(char *buffer, struct kernel_param *kp)
{
	volatile uint32_t* mux  = (uint32_t*) RTL_MUX_ADDR;
	sprintf(buffer, "0x%x", *mux); 
	return strlen(buffer);
}

#define param_check_mux(a,b)  ;;

module_param_named(mux, g_muxdummy, mux, S_IRUGO | S_IWUSR)

struct rtl_gpio_chip {
	struct gpio_chip chip;
	uint32_t gpio_state;
	uint32_t gpio_dir;
	volatile uint32_t* regs;	
	spinlock_t lock;	/* Lock used for synchronization */
};

static inline struct rtl_gpio_chip *to_rtl_gpio_chip(struct gpio_chip *gc)
{
        return container_of(gc, struct rtl_gpio_chip, chip);
}


/* Dummy to make gpiolib happy */
int gpio_to_irq(unsigned g) 
{
	return 0;
}

EXPORT_SYMBOL(gpio_to_irq);

/**
 * rtlgpio_get - Read the specified signal of the GPIO device.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 *
 * This function reads the specified signal of the GPIO device. It returns 0 if
 * the signal clear, 1 if signal is set or negative value on error.
 */
static int rtlgpio_get(struct gpio_chip *gc, unsigned int gpio)
{
	struct rtl_gpio_chip *rgc = to_rtl_gpio_chip(gc);
	return (*(rgc->regs + RTLGPIO_DATA) >> gpio) & 1;
}

/**
 * rtlgpio_set - Write the specified signal of the GPIO device.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 * @val:    Value to be written to specified signal.
 *
 * This function writes the specified value in to the specified signal of the
 * GPIO device.
 */
static void rtlgpio_set(struct gpio_chip *gc, unsigned int gpio, int val)
{
	unsigned long flags;
	struct rtl_gpio_chip *rgc = to_rtl_gpio_chip(gc);

	spin_lock_irqsave(&rgc->lock, flags);
	/* Write to GPIO signal and set its direction to output */
	if (val)
		rgc->gpio_state |= 1 << gpio;
	else
		rgc->gpio_state &= ~(1 << gpio);
	*(rgc->regs + RTLGPIO_DATA) = rgc->gpio_state;
	spin_unlock_irqrestore(&rgc->lock, flags);
}

/**
 * rtlgpio_dir_in - Set the direction of the specified GPIO signal as input.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 *
 * This function sets the direction of specified GPIO signal as input.
 * It returns 0 if direction of GPIO signals is set as input otherwise it
 * returns negative error value.
 */
static int rtlgpio_dir_in(struct gpio_chip *gc, unsigned int gpio)
{

	unsigned long flags;
	struct rtl_gpio_chip *rgc = to_rtl_gpio_chip(gc);

	spin_lock_irqsave(&rgc->lock, flags);
	/* Set the GPIO bit in shadow register and set direction as input */
	rgc->gpio_dir &= ~(1 << gpio);
	*(rgc->regs + RTLGPIO_DIR) = rgc->gpio_dir;
	spin_unlock_irqrestore(&rgc->lock, flags);
	return 0;
}

/**
 * rtlgpio_dir_out - Set the direction of the specified GPIO signal as output.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 * @val:    Value to be written to specified signal.
 *
 * This function sets the direction of specified GPIO signal as output. If all
 * GPIO signals of GPIO chip is configured as input then it returns
 * error otherwise it returns 0.
 */
static int rtlgpio_dir_out(struct gpio_chip *gc, unsigned int gpio, int val)
{
	unsigned long flags;
	struct rtl_gpio_chip *rgc = to_rtl_gpio_chip(gc);
	spin_lock_irqsave(&rgc->lock, flags);
	/* Set the GPIO bit in shadow register and set direction as input */
	rgc->gpio_dir |= (1 << gpio);
	*(rgc->regs + RTLGPIO_DIR) = rgc->gpio_dir;
	spin_unlock_irqrestore(&rgc->lock, flags);
	return 0;
}


static struct rtl_gpio_chip rgpio;
static int __init rtlgpio_init(void)
{
/*	volatile uint32_t* regs = ioremap_nocache(0xb8003500, 32);
	volatile uint32_t* mux = ioremap_nocache(0xB8000040, 4);
*/
	/* 
	 * Something is definetely not right here. 
	 * Theory says that ioremap is the way, practice says the opposite
	 * Must be some dark realtek's magic coming from arch/rlx
	 */

	volatile uint32_t* regs = (uint32_t*) 0xb8003500;
	volatile uint32_t* mux  = (uint32_t*) 0xb8000040;
	*mux = g_mux_value; 
	printk("rtl-gpio: Hacky Realtek 819x GPIO Driver (c) Andrew 'Necromant' Andrianov 2012\n");
	rgpio.chip.direction_input = rtlgpio_dir_in;
	rgpio.chip.direction_output = rtlgpio_dir_out;
	rgpio.chip.get = rtlgpio_get;
	rgpio.chip.label = "rtl-gpio";
	rgpio.chip.set = rtlgpio_set;
	rgpio.chip.base = 0;
	rgpio.chip.ngpio = 32;
	spin_lock_init(&rgpio.lock);
	rgpio.regs = regs;
	rgpio.gpio_state = *(rgpio.regs + RTLGPIO_DATA);
	rgpio.gpio_dir = *(rgpio.regs + RTLGPIO_DIR);
	printk("rtl-gpio: remaped io regs: 0x%p, mux: 0x%p\n", 
	       regs,
	       mux);
	printk("rtl-gpio: mux:0x%x\n", 
	       *mux);

	printk("rtl-gpio: initial dir: 0x%x, value: 0x%x\n", 
	       rgpio.gpio_state,
	       rgpio.gpio_dir);
	gpiochip_add(&rgpio.chip);
	return 0;
}

/* Make sure we get initialized before anyone else tries to use us */
subsys_initcall(rtlgpio_init);
/* No exit call at the moment as we cannot unregister of GPIO chips */



MODULE_AUTHOR("Andrew 'Necromant' Andrianov");
MODULE_DESCRIPTION("Hacky RTL819x GPIO Driver");
MODULE_LICENSE("GPL");
