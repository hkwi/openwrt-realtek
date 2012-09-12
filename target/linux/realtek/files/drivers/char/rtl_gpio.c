/*
 * FILE NAME rtl_gpio.c
 *
 * BRIEF MODULE DESCRIPTION
 *  GPIO For Flash Reload Default
 *
 *  Author: jimmylin@realtek.com.tw
 *
 * Copyright 2005 Realtek Semiconductor Corp.
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE	LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

//#define CONFIG_USING_JTAG 1

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/reboot.h>
#include <linux/kmod.h>
#include <linux/proc_fs.h>
//#include  "bspchip.h"
#define AUTO_CONFIG

// 2009-0414
//#define	DET_WPS_SPEC
#ifndef CONFIG_RTK_VOIP_DRIVERS_ATA_DECT //DECT SPI use GPIO E interrupt, need refine code to share irq.
#ifndef CONFIG_SERIAL_SC16IS7X0 //SC16IS7x0 use GPIO E interrupt, too.  
#define USE_INTERRUPT_GPIO
#endif
#endif

#if defined(CONFIG_RTL_8196C) || defined(CONFIG_RTL_8198)
#ifndef CONFIG_RTK_VOIP
	#define READ_RF_SWITCH_GPIO
#endif
#endif

#if defined(CONFIG_RTL_8196C) || defined(CONFIG_RTL_8198)
	#include "drivers/net/rtl819x/AsicDriver/rtl865xc_asicregs.h"
/*define the GPIO physical address to customer_gpio.h*/
#ifdef CONFIG_RTK_VOIP
	#if defined (CONFIG_RTK_VOIP_GPIO_8954C_V100) || \
		defined (CONFIG_RTK_VOIP_GPIO_8964C_QA)
	

		// GPIO E7
		#define RESET_PIN_IOBASE PEFGH_CNR 		//RTL_GPIO_PEFGH_CNR
		#define RESET_PIN_DIRBASE PEFGH_DIR		//RTL_GPIO_PEFGH_DIR 
		#define RESET_PIN_DATABASE PEFGH_DAT 	//RTL_GPIO_PEFGH_DATA
		#define RESET_PIN_NO 7 					//pin number of the EFGH

		// GPIO G0
		#define RESET_LED_IOBASE PEFCH_CNR 		//RTL_GPIO_PEFGH_CNR
		#define RESET_LED_DIRBASE PEFGH_DIR		//RTL_GPIO_PEFGH_DIR 
		#define RESET_LED_DATABASE PEFGH_DAT 	//RTL_GPIO_PEFGH_DATA
		#define RESET_LED_NO 16 				//pin number of the EFGH
		
		// GPIO G1
		#define AUTOCFG_LED_IOBASE PEFGH_CNR 	//RTL_GPIO_PEFGH_CNR
		#define AUTOCFG_LED_DIRBASE PEFGH_DIR	//RTL_GPIO_PEFGH_DIR 
		#define AUTOCFG_LED_DATABASE PEFGH_DAT 	//RTL_GPIO_PEFGH_DATA
		#define AUTOCFG_LED_NO 17 				//pin number of the EFGH

		// GPIO E2
		#define AUTOCFG_PIN_IOBASE PEFGH_CNR 	//RTL_GPIO_PEFGH_CNR
		#define AUTOCFG_PIN_DIRBASE PEFGH_DIR	//RTL_GPIO_PEFGH_DIR 
		#define AUTOCFG_PIN_DATABASE PEFGH_DAT  //RTL_GPIO_PEFGH_DATA
		#define AUTOCFG_PIN_NO 2 				//pin number of the EFGH)

		#define AUTOCFG_PIN_IMR PEF_IMR
		#define RTL_GPIO_MUX_DATA 0x00004300 	//MUX for GPIO

	#elif defined(CONFIG_RTK_VOIP_GPIO_8954C_V200) || \
	 	  defined(CONFIG_RTK_VOIP_GPIO_8954C_V400)

		// GPIO F4 DEFAULT_Button
		#define RESET_PIN_IOBASE PEFGH_CNR 		//RTL_GPIO_PEFGH_CNR
		#define RESET_PIN_DIRBASE PEFGH_DIR		//RTL_GPIO_PEFGH_DIR 
		#define RESET_PIN_DATABASE PEFGH_DAT 	//RTL_GPIO_PEFGH_DATA
		#define RESET_PIN_NO 12 				//pin number of the EFGH

		// GPIO E7 SYS LED
		#define RESET_LED_IOBASE PEFGH_CNR 		//RTL_GPIO_PEFGH_CNR
		#define RESET_LED_DIRBASE PEFGH_DIR		//RTL_GPIO_PEFGH_DIR 
		#define RESET_LED_DATABASE PEFGH_DAT 	//RTL_GPIO_PEFGH_DATA
		#define RESET_LED_NO 7 					//number of the EFGH
		
		// GPIO F0 WPS LED
		#define AUTOCFG_LED_IOBASE PEFGH_CNR 	//RTL_GPIO_PEFGH_CNR
		#define AUTOCFG_LED_DIRBASE PEFGH_DIR	//RTL_GPIO_PEFGH_DIR 
		#define AUTOCFG_LED_DATABASE PEFGH_DAT  //RTL_GPIO_PEFGH_DATA
		#define AUTOCFG_LED_NO 8 				//pin number of the EFGH

		// GPIO F3 WPS Button
		#define AUTOCFG_PIN_IOBASE PEFGH_CNR 	//RTL_GPIO_PEFGH_CNR
		#define AUTOCFG_PIN_DIRBASE PEFGH_DIR	//RTL_GPIO_PEFGH_DIR 
		#define AUTOCFG_PIN_DATABASE PEFGH_DAT  //RTL_GPIO_PEFGH_DATA
		#define AUTOCFG_PIN_NO 11 				//pin number of the EFGH
		#define AUTOCFG_PIN_IMR PEF_IMR
		#define RTL_GPIO_MUX_DATA 0x00000300 	//MUX for GPIO
    #endif
	
	#define RTL_GPIO_MUX 0xB8000040

	//#define RTL_GPIO_WIFI_ONOFF     19
	#define AUTOCFG_BTN_PIN         AUTOCFG_PIN_NO
	#define AUTOCFG_LED_PIN         AUTOCFG_LED_NO
	#define RESET_LED_PIN           RESET_LED_NO
	#define RESET_BTN_PIN           RESET_PIN_NO
	
#else
#if defined(CONFIG_RTL_8196C)	
   #if defined(CONFIG_RTL_8196CS)
	#define PCIE0_BASE 0xb9000000
	#define PCIE_BASE_OFFSET (PCIE0_BASE+0x40)
	#define PCIE_PIN_MUX PCIE_BASE_OFFSET
	#define RESET_PIN_IOBASE (PCIE_BASE_OFFSET+4)
	#define WPS_PIN_IOBASE (PCIE_BASE_OFFSET+4)
	#define WPS_LED_IOBASE (PCIE_BASE_OFFSET)
	#define PCIE_GPIO_IMR (PCIE_BASE_OFFSET+8)
	#define MODE_MARSK 24
	#define MODE_MARSK1 28
	#define DIR_MASK 16
	#define DIR_MASK1 24
	#define IN_MASK 0
	#define OUT_MASK 8
	#define OUT_MASK1 20
	
	// GPIO A0
	//#define RESET_PIN_IOBASE RESET_PIN_IOBASE 	//RTL_GPIO_PABCD_CNR
	#define RESET_PIN_DIRBASE RESET_PIN_IOBASE	//RTL_GPIO_PABCD_DIR 
	#define RESET_PIN_DATABASE RESET_PIN_IOBASE //RTL_GPIO_PABCD_DATA
	#define RESET_PIN_NO 0 /*number of the ABCD*/

	// GPIO C2
	#define RESET_LED_IOBASE PABCD_CNR 	//RTL_GPIO_PABCD_CNR
	#define RESET_LED_DIRBASE PABCD_DIR	//RTL_GPIO_PABCD_DIR 
	#define RESET_LED_DATABASE PABCD_DAT //RTL_GPIO_PABCD_DATA
	#define RESET_LED_NO 2 /*number of the ABCD*/

	// GPIO C4
	#define AUTOCFG_LED_IOBASE WPS_LED_IOBASE 	//RTL_GPIO_PABCD_CNR
	#define AUTOCFG_LED_DIRBASE WPS_LED_IOBASE	//RTL_GPIO_PABCD_DIR 
	#define AUTOCFG_LED_DATABASE WPS_LED_IOBASE //RTL_GPIO_PABCD_DATA
	#define AUTOCFG_LED_NO 20 /*number of the ABCD*/

	// GPIO A1
	#define AUTOCFG_PIN_IOBASE WPS_PIN_IOBASE 	//RTL_GPIO_PABCD_CNR
	#define AUTOCFG_PIN_DIRBASE WPS_PIN_IOBASE	//RTL_GPIO_PABCD_DIR 
	#define AUTOCFG_PIN_DATABASE WPS_PIN_IOBASE //RTL_GPIO_PABCD_DATA
	#define AUTOCFG_PIN_NO 2 /*number of the ABCD)*/
	#define AUTOCFG_PIN_IMR PCIE_GPIO_IMR
	#else
	// GPIO A0
	#define RESET_PIN_IOBASE PABCD_CNR 	//RTL_GPIO_PABCD_CNR
	#define RESET_PIN_DIRBASE PABCD_DIR	//RTL_GPIO_PABCD_DIR 
	#define RESET_PIN_DATABASE PABCD_DAT //RTL_GPIO_PABCD_DATA
	#define RESET_PIN_NO 0 /*number of the ABCD*/

	// GPIO C2
	#define RESET_LED_IOBASE PABCD_CNR 	//RTL_GPIO_PABCD_CNR
	#define RESET_LED_DIRBASE PABCD_DIR	//RTL_GPIO_PABCD_DIR 
	#define RESET_LED_DATABASE PABCD_DAT //RTL_GPIO_PABCD_DATA
	#define RESET_LED_NO 18 /*number of the ABCD*/

	// GPIO C4
	#define AUTOCFG_LED_IOBASE PABCD_CNR 	//RTL_GPIO_PABCD_CNR
	#define AUTOCFG_LED_DIRBASE PABCD_DIR	//RTL_GPIO_PABCD_DIR 
	#define AUTOCFG_LED_DATABASE PABCD_DAT //RTL_GPIO_PABCD_DATA
	#define AUTOCFG_LED_NO 20 /*number of the ABCD*/

	// GPIO A1
	#define AUTOCFG_PIN_IOBASE PABCD_CNR 	//RTL_GPIO_PABCD_CNR
	#define AUTOCFG_PIN_DIRBASE PABCD_DIR	//RTL_GPIO_PABCD_DIR 
	#define AUTOCFG_PIN_DATABASE PABCD_DAT //RTL_GPIO_PABCD_DATA
	#define AUTOCFG_PIN_NO 1 /*number of the ABCD)*/
	#define AUTOCFG_PIN_IMR PAB_IMR
	#endif
#ifdef CONFIG_POCKET_ROUTER_SUPPORT
	#define RTL_GPIO_MUX_GPIOA2	(3<<20)
	#define RTL_GPIO_MUX_GPIOB3	(3<<2)
	#define RTL_GPIO_MUX_GPIOB2	(3<<0)	
	#define RTL_GPIO_MUX_GPIOC0	(3<<12)
	#define RTL_GPIO_MUX_POCKETAP_DATA	(RTL_GPIO_MUX_GPIOA2 | RTL_GPIO_MUX_GPIOB3 | RTL_GPIO_MUX_GPIOB2 | RTL_GPIO_MUX_GPIOC0)

	#define RTL_GPIO_CNR_GPIOA2	(1<<2)
	#define RTL_GPIO_CNR_GPIOB3	(1<<11)
	#define RTL_GPIO_CNR_GPIOB2	(1<<10)	
	#define RTL_GPIO_CNR_GPIOC0	(1<<16)
	#define RTL_GPIO_CNR_POCKETAP_DATA	(RTL_GPIO_CNR_GPIOA2 | RTL_GPIO_CNR_GPIOB3 | RTL_GPIO_CNR_GPIOB2 | RTL_GPIO_CNR_GPIOC0)

	#define RTL_GPIO_DIR_GPIOA2	(1<<2) /* &- */
	#define RTL_GPIO_DIR_GPIOB3	(1<<11) /* &- */
	#define RTL_GPIO_DIR_GPIOB2	(1<<10) /* |*/	
	#define RTL_GPIO_DIR_GPIOC0	(1<<16) /* &- */

	#define RTL_GPIO_DAT_GPIOA2	(1<<2) 
	#define RTL_GPIO_DAT_GPIOB3	(1<<11) 
	#define RTL_GPIO_DAT_GPIOB2	(1<<10) 	
	#define RTL_GPIO_DAT_GPIOC0	(1<<16) 

	static int ap_cli_rou_time_state[2] = {0};
	static char ap_cli_rou_state = 0;
	static char ap_cli_rou_idx=0;
	static char pocketAP_hw_set_flag='0';

	static char dc_pwr_plugged_time_state = 0;
	static char dc_pwr_plugged_state = 0;
	static char dc_pwr_plugged_flag = '0';

	static int pwr_saving_state=0;
	static char pwr_saving_led_toggle = 0;
#endif

	
#elif defined(CONFIG_RTL_8198)
	// GPIO H1
	#define RESET_PIN_IOBASE PEFGH_CNR 	//RTL_GPIO_PABCD_CNR
	#define RESET_PIN_DIRBASE PEFGH_DIR	//RTL_GPIO_PABCD_DIR 
	#define RESET_PIN_DATABASE PEFGH_DAT //RTL_GPIO_PABCD_DATA
	#define RESET_PIN_NO 25 /*number of the ABCD*/

	// GPIO H3
	#define RESET_LED_IOBASE PEFGH_CNR 	//RTL_GPIO_PABCD_CNR
	#define RESET_LED_DIRBASE PEFGH_DIR	//RTL_GPIO_PABCD_DIR 
	#define RESET_LED_DATABASE PEFGH_DAT //RTL_GPIO_PABCD_DATA
	#define RESET_LED_NO 27 /*number of the ABCD*/

	// GPIO G4
	#define AUTOCFG_LED_IOBASE PEFGH_CNR 	//RTL_GPIO_PABCD_CNR
	#define AUTOCFG_LED_DIRBASE PEFGH_DIR	//RTL_GPIO_PABCD_DIR 
	#define AUTOCFG_LED_DATABASE PEFGH_DAT //RTL_GPIO_PABCD_DATA
	#define AUTOCFG_LED_NO 20 /*number of the ABCD*/

	// GPIO E1
	#define AUTOCFG_PIN_IOBASE PEFGH_CNR 	//RTL_GPIO_PABCD_CNR
	#define AUTOCFG_PIN_DIRBASE PEFGH_DIR	//RTL_GPIO_PABCD_DIR 
	#define AUTOCFG_PIN_DATABASE PEFGH_DAT //RTL_GPIO_PABCD_DATA
	#define AUTOCFG_PIN_NO 1 /*number of the ABCD)*/
	#define AUTOCFG_PIN_IMR PGH_IMR
	
#endif
	// GPIO C3
	#define WIFI_ONOFF_PIN_IOBASE PABCD_CNR 	//RTL_GPIO_PABCD_CNR
	#define WIFI_ONOFF_PIN_DIRBASE PABCD_DIR	//RTL_GPIO_PABCD_DIR 
	#define WIFI_ONOFF_PIN_DATABASE PABCD_DAT //RTL_GPIO_PABCD_DATA
	#define WIFI_ONOFF_PIN_NO 19/*umber of the ABCD)*/
	#define WIFI_ONOFF_PIN_IMR PAB_IMR

/*
	#define RTL_GPIO_MUX 0xB8000030
	#define RTL_GPIO_MUX_DATA 0x0FC00380//for WIFI ON/OFF and GPIO

	

	#define AUTOCFG_BTN_PIN	AUTOCFG_PIN_NO	// 1
	#define AUTOCFG_LED_PIN		AUTOCFG_LED_NO//20
	#define RESET_LED_PIN		RESET_LED_NO //18
	#define RESET_BTN_PIN		RESET_PIN_NO //0
	#define RTL_GPIO_WIFI_ONOFF	WIFI_ONOFF_PIN_NO
*/
	 #define RTL_GPIO_MUX 0xB8000040
	#ifdef CONFIG_8198_PORT5_GMII
		#define RTL_GPIO_MUX_DATA 0x00340000
	#else 
		#define RTL_GPIO_MUX_DATA 0x00340C00//for WIFI ON/OFF and GPIO
	#endif 
	#define RTL_GPIO_WIFI_ONOFF     19

#if defined(CONFIG_RTL_8196C)	 
	#if  defined(CONFIG_RTL_8196CS)
	 #define AUTOCFG_BTN_PIN         2
	 #define AUTOCFG_LED_PIN         1
	 #define RESET_LED_PIN           2
	 #define RESET_BTN_PIN           0
	#else
	 #define AUTOCFG_BTN_PIN         3
	 #define AUTOCFG_LED_PIN         4
	 #define RESET_LED_PIN           6
	 #define RESET_BTN_PIN           5
	#endif
	
#elif defined(CONFIG_RTL_8198)
	 #define AUTOCFG_BTN_PIN         24
	 #define AUTOCFG_LED_PIN         26
	 #define RESET_LED_PIN           27
	 #define RESET_BTN_PIN           25
#endif	 

#endif // CONFIG_RTK_VOIP
#endif // CONFIG_RTL_8196C || CONFIG_RTL_8198

// 2009-0414
#ifdef USE_INTERRUPT_GPIO
#ifdef CONFIG_RTK_VOIP
	#define GPIO_IRQ_NUM		(16+17)	// GPIO_EFGH
#else
	#define GPIO_IRQ_NUM		1	
#endif
#endif

	#define PROBE_TIME	5


#define PROBE_NULL		0
#define PROBE_ACTIVE	1
#define PROBE_RESET		2
#define PROBE_RELOAD	3
#define RTL_R32(addr)		(*(volatile unsigned long *)(addr))
#define RTL_W32(addr, l)	((*(volatile unsigned long*)(addr)) = (l))
#define RTL_R8(addr)		(*(volatile unsigned char*)(addr))
#define RTL_W8(addr, l)		((*(volatile unsigned char*)(addr)) = (l))

//#define  GPIO_DEBUG
#ifdef GPIO_DEBUG
/* note: prints function name for you */
#  define DPRINTK(fmt, args...) printk("%s: " fmt, __FUNCTION__ , ## args)
#else
#  define DPRINTK(fmt, args...)
#endif

static struct timer_list probe_timer;
static unsigned int    probe_counter;
static unsigned int    probe_state;

static char default_flag='0';
//Brad add for update flash check 20080711
int start_count_time=0;
int Reboot_Wait=0;

static int get_dc_pwr_plugged_state();



//#ifdef CONFIG_RTL865X_AC

#ifdef CONFIG_POCKET_ROUTER_SUPPORT
static struct timer_list pocket_ap_timer;
#endif

#ifdef	USE_INTERRUPT_GPIO
static int wps_button_push = 0;

#endif
#if defined(CONFIG_RTL_8196CS)
void update_pcie_status(void)
{
	unsigned int temp;
	temp=RTL_R32(0xb8b00728);
	temp=RTL_R32(PCIE_PIN_MUX);
	temp=RTL_R32(RESET_PIN_DATABASE);
	
	
	//printk("LINE: %x d:%x *  %x****R:%x\n",__LINE__,RTL_R32(0xb8b00728),RTL_R32(PCIE_PIN_MUX),RTL_R32(RESET_PIN_DATABASE));
}
#endif

#ifdef AUTO_CONFIG
static unsigned int		AutoCfg_LED_Blink;
static unsigned int		AutoCfg_LED_Toggle;
static unsigned int		AutoCfg_LED_Slow_Blink;
static unsigned int		AutoCfg_LED_Slow_Toggle;

void autoconfig_gpio_init(void)
{
#if defined(CONFIG_RTL_8196CS)
	RTL_W32(AUTOCFG_PIN_IOBASE,(RTL_R32(AUTOCFG_PIN_IOBASE)&(~((1 << AUTOCFG_BTN_PIN)<<MODE_MARSK))));
	RTL_W32(AUTOCFG_LED_IOBASE,(RTL_R32(AUTOCFG_LED_IOBASE)&(~((1 << AUTOCFG_LED_PIN)<<MODE_MARSK1))));

	// Set GPIOA pin 1 as input pin for auto config button
	RTL_W32(AUTOCFG_PIN_DIRBASE, (RTL_R32(AUTOCFG_PIN_DIRBASE) & (~(((1 << AUTOCFG_BTN_PIN))<<DIR_MASK1))));

	// Set GPIOA ping 3 as output pin for auto config led
	RTL_W32(AUTOCFG_LED_DIRBASE, (RTL_R32(AUTOCFG_LED_DIRBASE) | ((1 << AUTOCFG_LED_PIN)<<DIR_MASK1)));

	// turn off auto config led in the beginning
	RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) | ((1 << AUTOCFG_LED_PIN)<<OUT_MASK1)));
	update_pcie_status();
	//printk("LINE: %x d:%x *  %x****R:%x\n",__LINE__,RTL_R32(0xb8b00728),RTL_R32(PCIE_PIN_MUX),RTL_R32(RESET_PIN_DATABASE));
#else
	RTL_W32(AUTOCFG_PIN_IOBASE,(RTL_R32(AUTOCFG_PIN_IOBASE)&(~(1 << AUTOCFG_BTN_PIN))));
	RTL_W32(AUTOCFG_LED_IOBASE,(RTL_R32(AUTOCFG_LED_IOBASE)&(~(1 << AUTOCFG_LED_PIN))));

	// Set GPIOA pin 1 as input pin for auto config button
	RTL_W32(AUTOCFG_PIN_DIRBASE, (RTL_R32(AUTOCFG_PIN_DIRBASE) & (~(1 << AUTOCFG_BTN_PIN))));

	// Set GPIOA ping 3 as output pin for auto config led
	RTL_W32(AUTOCFG_LED_DIRBASE, (RTL_R32(AUTOCFG_LED_DIRBASE) | (1 << AUTOCFG_LED_PIN)));

	// turn off auto config led in the beginning
	RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) | (1 << AUTOCFG_LED_PIN)));
#endif
}
#ifdef CONFIG_RTL_8196C_GW_MP

void all_led_on(void)
{
	//printk("Into MP GPIO");
	 #ifdef CONFIG_RTL_8196C_GW_MP
	RTL_W32(0xB8000030, (RTL_R32(0xB8000030) | 0x00F00F80 ));
	RTL_W32(PABCD_CNR, (RTL_R32(PABCD_CNR) & (~(1 << PS_LED_GREEN_PIN))));
	RTL_W32(PABCD_CNR, (RTL_R32(PABCD_CNR) & (~(1 << PS_LED_ORANGE_PIN))));

	RTL_W32(PABCD_DIR, (RTL_R32(PABCD_DIR) | (1 << PS_LED_GREEN_PIN)));
	RTL_W32(PABCD_DIR, (RTL_R32(PABCD_DIR) | (1 << PS_LED_ORANGE_PIN)));
	
	RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) | (1 << PS_LED_GREEN_PIN)));
	RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) & (~(1 << PS_LED_ORANGE_PIN))));

	/* inet_led init setting */
	RTL_W32(PABCD_CNR, (RTL_R32(PABCD_CNR) & (~(1 << INET_LED_GREEN_PIN))));
	RTL_W32(PABCD_CNR, (RTL_R32(PABCD_CNR) & (~(1 << INET_LED_ORANGE_PIN))));

	RTL_W32(PABCD_DIR, (RTL_R32(PABCD_DIR) | (1 << INET_LED_GREEN_PIN)));
	RTL_W32(PABCD_DIR, (RTL_R32(PABCD_DIR) | (1 << INET_LED_ORANGE_PIN)));
	
	RTL_W32(AUTOCFG_LED_DIRBASE, (RTL_R32(AUTOCFG_LED_DIRBASE) & (~(1 << AUTOCFG_LED_PIN_MP))));

	RTL_W32(PABCD_DIR, (RTL_R32(PABCD_DIR) | (1 << AUTOCFG_LED_PIN_MP)));

	//RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) & (~(1 << INET_LED_GREEN_PIN))));
	//RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) | (1 << INET_LED_ORANGE_PIN)));
	RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) & (~(1 << PS_LED_GREEN_PIN))));	
	RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) & (~(1 << PS_LED_ORANGE_PIN))));
	RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) & (~(1 << INET_LED_GREEN_PIN))));
	RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) & (~(1 << INET_LED_ORANGE_PIN))));
	RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) & (~(1 << AUTOCFG_LED_PIN_MP))));
	RTL_W32(RESET_LED_DATABASE, (RTL_R32(RESET_LED_DATABASE) & (~(1 << RESET_LED_PIN_MP))));
	#endif

}
#endif
#if defined(CONFIG_RTL_8196CS)

void autoconfig_gpio_off(void)
{
	RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) | ((1 << AUTOCFG_LED_PIN)<<OUT_MASK1)));
	AutoCfg_LED_Blink = 0;
	update_pcie_status();
	//printk("LINE: %x d:%x *  %x****R:%x\n",__LINE__,RTL_R32(0xb8b00728),RTL_R32(PCIE_PIN_MUX),RTL_R32(RESET_PIN_DATABASE));
}


void autoconfig_gpio_on(void)
{
	RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) & (~((1 << AUTOCFG_LED_PIN)<<OUT_MASK1))));
	//printk("LINE: %x d:%x *  %x****R:%x\n",__LINE__,RTL_R32(0xb8b00728),RTL_R32(PCIE_PIN_MUX),RTL_R32(RESET_PIN_DATABASE));
	update_pcie_status();
	AutoCfg_LED_Blink = 0;
}


void autoconfig_gpio_blink(void)
{
	RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) & (~((1 << AUTOCFG_LED_PIN)<<OUT_MASK1))));
	//printk("LINE: %x d:%x *  %x****R:%x\n",__LINE__,RTL_R32(0xb8b00728),RTL_R32(PCIE_PIN_MUX),RTL_R32(RESET_PIN_DATABASE));
	update_pcie_status();
	AutoCfg_LED_Blink = 1;
	AutoCfg_LED_Toggle = 1;

}



#else
void autoconfig_gpio_off(void)
{
	RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) | (1 << AUTOCFG_LED_PIN)));
	AutoCfg_LED_Blink = 0;
}


void autoconfig_gpio_on(void)
{
	RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) & (~(1 << AUTOCFG_LED_PIN))));
	AutoCfg_LED_Blink = 0;
}


void autoconfig_gpio_blink(void)
{
	RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) & (~(1 << AUTOCFG_LED_PIN))));

	AutoCfg_LED_Blink = 1;
	AutoCfg_LED_Toggle = 1;
	AutoCfg_LED_Slow_Blink = 0;

}

void autoconfig_gpio_slow_blink(void)
{
	RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) & (~(1 << AUTOCFG_LED_PIN))));
	
	AutoCfg_LED_Blink = 1;
	AutoCfg_LED_Toggle = 1;
	AutoCfg_LED_Slow_Blink = 1;
	AutoCfg_LED_Slow_Toggle = 1;

}

#endif


#endif // AUTO_CONFIG




static void rtl_gpio_timer(unsigned long data)
{
	unsigned int pressed=1;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
	struct pid *pid;
#endif
#if defined(CONFIG_RTL_8196CS)
	update_pcie_status();
#endif
#if  defined(CONFIG_RTL_8196C) || defined(CONFIG_RTL_8198)

	if ((RTL_R32(RESET_PIN_DATABASE) & (1 << RESET_BTN_PIN)))

#endif
	{
		pressed = 0;

		//turn off LED0
		#ifndef CONFIG_RTL_8196C_GW_MP
		RTL_W32(RESET_LED_DATABASE, (RTL_R32(RESET_LED_DATABASE) | ((1 << RESET_LED_PIN))));
		#endif
	}
	else
	{
		DPRINTK("Key pressed %d!\n", probe_counter+1);
	}

	if (RTL_R32(AUTOCFG_PIN_DATABASE) & (1 << AUTOCFG_BTN_PIN)){
		wps_button_push = 0;
	}else{
		wps_button_push++;
	}

	if (probe_state == PROBE_NULL)
	{
		if (pressed)
		{
			probe_state = PROBE_ACTIVE;
			probe_counter++;
		}
		else
			probe_counter = 0;
	}
	else if (probe_state == PROBE_ACTIVE)
	{
		if (pressed)
		{
			probe_counter++;

			if ((probe_counter >=2 ) && (probe_counter <=PROBE_TIME))
			{
				DPRINTK("2-5 turn on led\n");
				//turn on LED0
				RTL_W32(RESET_LED_DATABASE, (RTL_R32(RESET_LED_DATABASE) & (~(1 << RESET_LED_PIN))));
			}
			else if (probe_counter >= PROBE_TIME)
			{
				// sparkling LED0
				DPRINTK(">5 \n");

				if (probe_counter & 1)
					RTL_W32(RESET_LED_DATABASE, (RTL_R32(RESET_LED_DATABASE) | ((1 << RESET_LED_PIN))));
				else
					RTL_W32(RESET_LED_DATABASE, (RTL_R32(RESET_LED_DATABASE) & (~(1 << RESET_LED_PIN))));
			}
		}
		else
		{
			#if defined(CONFIG_RTL865X_SC)
			if (probe_counter < 5)
			#else
			if (probe_counter < 2)
			#endif
			{
				probe_state = PROBE_NULL;
				probe_counter = 0;
				DPRINTK("<2 \n");
				#if defined(CONFIG_RTL865X_SC)
					ResetToAutoCfgBtn = 1;
				#endif
			}
			else if (probe_counter >= PROBE_TIME)
			{


				//reload default
				default_flag = '1';

				//kernel_thread(reset_flash_default, (void *)1, SIGCHLD);
				return;

			}
			else
			{
				DPRINTK("2-5 reset 1\n");
			#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
				kill_proc(1,SIGTERM,1);
			#else
				pid = get_pid(find_vpid(1));
				kill_pid(pid,SIGTERM,1);
			#endif
				DPRINTK("2-5 reset 2\n");
				//kernel_thread(reset_flash_default, 0, SIGCHLD);
				return;
			}
		}
	}

#ifdef AUTO_CONFIG
	if (AutoCfg_LED_Blink==1)
	{
		if (AutoCfg_LED_Toggle) {
		#if defined(CONFIG_RTL_8196CS)
			RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) | ((1 << AUTOCFG_LED_PIN)<<OUT_MASK1)));
		#else
			RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) | (1 << AUTOCFG_LED_PIN)));
		#endif
		}
		else {
			#if defined(CONFIG_RTL_8196CS)
			RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) & (~((1 << AUTOCFG_LED_PIN)<<OUT_MASK1))));
			#else
			 RTL_W32(AUTOCFG_LED_DATABASE, (RTL_R32(AUTOCFG_LED_DATABASE) & (~(1 << AUTOCFG_LED_PIN))));
			#endif
			
		}
				
		if(AutoCfg_LED_Slow_Blink)
		{
			if(AutoCfg_LED_Slow_Toggle)
				AutoCfg_LED_Toggle = AutoCfg_LED_Toggle;
			else
				AutoCfg_LED_Toggle = AutoCfg_LED_Toggle? 0 : 1;
			
			AutoCfg_LED_Slow_Toggle = AutoCfg_LED_Slow_Toggle? 0 : 1;
		}
		else
			AutoCfg_LED_Toggle = AutoCfg_LED_Toggle? 0 : 1;
		
	}
#endif


	mod_timer(&probe_timer, jiffies + HZ);

}

#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE

#define SYSTEM_CONTRL_DUMMY_REG 0xb8000068

int is_bank2_root()
{
	//boot code will steal System's dummy register bit0 (set to 1 ---> bank2 booting
	//for 8198 formal chip 
	if ((RTL_R32(SYSTEM_CONTRL_DUMMY_REG)) & (0x00000001))  // steal for boot bank idenfy
		return 1;

	return 0;
}
static int read_bootbank_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;
	char flag='1';

	if (is_bank2_root())  // steal for boot bank idenfy
		flag='2';
		
	len = sprintf(page, "%c\n", flag);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;
	return len;
}
#endif

#ifdef AUTO_CONFIG
#if defined(USE_INTERRUPT_GPIO)
static irqreturn_t gpio_interrupt_isr(int irq, void *dev_instance, struct pt_regs *regs)
{
	wps_button_push = 1;   	  	
#ifdef CONFIG_RTK_VOIP
	RTL_W32(PEFGH_ISR, RTL_R32(PEFGH_ISR)); 	
#else
  	RTL_W32(PABCD_ISR, RTL_R32(PABCD_ISR)); 	
#endif
	return IRQ_HANDLED;
}
#endif

static int read_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;
	char flag;

#if  defined(USE_INTERRUPT_GPIO)
// 2009-0414		
	if (wps_button_push) {
		flag = '1';
		//wps_button_push = 0; //mark it for select wlan interface by button pressed time		
	}
	else{
		if (RTL_R32(AUTOCFG_PIN_DATABASE) & (1 << AUTOCFG_BTN_PIN)){
			flag = '0';
		}else{
			//printk("wps button be held \n");
			flag = '1';
		}

	}
// 2009-0414		
#else

	if (RTL_R32(AUTOCFG_PIN_DATABASE) & (1 << AUTOCFG_BTN_PIN))
		flag = '0';
	else {
		flag = '1';
	}
#endif // CONFIG_RTL865X_KLD		
		
	len = sprintf(page, "%c\n", flag);


	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;
	return len;
}



#ifdef CONFIG_RTL_KERNEL_MIPS16_CHAR
__NOMIPS16
#endif 
static int write_proc(struct file *file, const char *buffer,
				unsigned long count, void *data)
{
	char flag[20];
//Brad add for update flash check 20080711

	char start_count[10], wait[10];

	if (count < 2)
		return -EFAULT;

	DPRINTK("file: %08x, buffer: %s, count: %lu, data: %08x\n",
		(unsigned int)file, buffer, count, (unsigned int)data);

	if (buffer && !copy_from_user(&flag, buffer, 1)) {
		if (flag[0] == 'E') {
			autoconfig_gpio_init();
			#ifdef CONFIG_RTL865X_CMO
			extra_led_gpio_init();
			#endif
		}
		else if (flag[0] == '0')
			autoconfig_gpio_off();
		else if (flag[0] == '1')
			autoconfig_gpio_on();
		else if (flag[0] == '2')
			autoconfig_gpio_blink();
#ifndef CONFIG_RTL_8196CS
		else if (flag[0] == '3')
			autoconfig_gpio_slow_blink();
#endif

#ifdef CONFIG_RTL_8196C_GW_MP
		else if (flag[0] == '9') // disable system led
                {
			all_led_on();
		}
#endif	

#ifdef CONFIG_RTL865X_CMO
		else if (flag[0] == '3')
			wep_wpa_led_on();
		else if (flag[0] == '5')
			wep_wpa_led_off();
		else if (flag[0] == '6')
			mac_ctl_led_on();
		else if (flag[0] == '7')
			mac_ctl_led_off();
		else if (flag[0] == '8')
			bridge_repeater_led_on();
		else if (flag[0] == '9')
			bridge_repeater_led_off();
		else if (flag[0] == 'A')
			system_led_on();
//		else if (flag[0] == 'B')
//			system_led_off();
		else if (flag[0] == 'C')
			lan_led_on();
		else if (flag[0] == 'D')
			lan_led_off();
		else if (flag[0] == 'Z')
			printk("gpio test test\n");
//		else if (flag[0] == '9')
//			system_led_blink = 2;
#endif

//Brad add for update flash check 20080711

		else if (flag[0] == '4'){
			start_count_time= 1;
			sscanf(buffer, "%s %s", start_count, wait);
			Reboot_Wait = (simple_strtol(wait,NULL,0))*100;
		}


		else
			{}

		return count;
	}
	else
		return -EFAULT;
}
#endif // AUTO_CONFIG

static int default_read_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%c\n", default_flag);
	if (len <= off+count) *eof = 1;
	  *start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	  return len;
}

#ifdef CONFIG_RTL_KERNEL_MIPS16_CHAR
__NOMIPS16
#endif 
static int default_write_proc(struct file *file, const char *buffer,
				unsigned long count, void *data)
{
	if (count < 2)
		return -EFAULT;
	if (buffer && !copy_from_user(&default_flag, buffer, 1)) {
		return count;
	}
	return -EFAULT;
}

#ifdef READ_RF_SWITCH_GPIO
static int rf_switch_read_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;
	char flag;

	if (RTL_R32(WIFI_ONOFF_PIN_DATABASE) & (1<<WIFI_ONOFF_PIN_NO)){
		flag = '1';
	}else{
		flag = '0';
	}
	len = sprintf(page, "%c\n", flag);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;
	return len;
}
#endif

#ifdef CONFIG_POCKET_ROUTER_SUPPORT
static int write_pocketAP_hw_set_flag_proc(struct file *file, const char *buffer,
				unsigned long count, void *data)
{
	unsigned int reg_cnr, reg_dir;

	if (count != 2)
		return -EFAULT;
	if (buffer && !copy_from_user(&pocketAP_hw_set_flag, buffer, 1)) {

	}
	return -EFAULT;
}

static int read_pocketAP_hw_set_flag_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;
	
	len = sprintf(page, "%c\n", pocketAP_hw_set_flag);
	
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;

//panic_printk("\r\n __[%s-%u]",__FILE__,__LINE__);	
	return len;

}



static int read_ap_client_rou_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;
	char flag;
	int gpioA2_flag,gpioB3_flag;
	
	
	if(ap_cli_rou_state == 2)
	{
		len = sprintf(page, "%c\n", '2'); // AP
	}
	else if(ap_cli_rou_state == 1)
	{
		len = sprintf(page, "%c\n", '1'); // Client
	}
	else if(ap_cli_rou_state == 3)
	{
		len = sprintf(page, "%c\n", '3'); // Router
	}
	else
	{
		len = sprintf(page, "%c\n", '0'); 
	}
	
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;

//panic_printk("\r\n __[%s-%u]",__FILE__,__LINE__);	
	return len;
}

static int read_dc_pwr_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;
	char flag;
	int pluged_state=0;
//panic_printk("\r\n 0x%x__[%s-%u]",RTL_R32(RESET_PIN_DATABASE),__FILE__,__LINE__);		

	pluged_state = get_dc_pwr_plugged_state();
	if(pluged_state == 1)
	{
		len = sprintf(page, "%c\n", '1');
	}
	else if(pluged_state == 2)
	{
		len = sprintf(page, "%c\n", '2');
	}
	else
	{
		len = sprintf(page, "%c\n", '0');
	}		

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;

//panic_printk("\r\n len=[%u]__[%s-%u]",len,__FILE__,__LINE__);	
	return len;
}

static int read_dc_pwr_plugged_flag_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;
	
	len = sprintf(page, "%c\n", dc_pwr_plugged_flag);
	
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;

//panic_printk("\r\n __[%s-%u]",__FILE__,__LINE__);
	dc_pwr_plugged_flag = '0';
	return len;

}
static int read_EnablePHYIf_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;
	char flag;

//panic_printk("\r\n 0x%x__[%s-%u]",RTL_R32(RESET_PIN_DATABASE),__FILE__,__LINE__);		

	if(RTL_R32(0xBB804114) & (0x01))
	{
		flag = '1';
	}
	else
	{
		flag = '0';
	}
		
	len = sprintf(page, "%c\n", flag);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;

//panic_printk("\r\n len=[%u]__[%s-%u]",len,__FILE__,__LINE__);	
	return len;
}

static int get_pocketAP_ap_cli_rou_state()
{
	int gpioA2_flag,gpioB3_flag;
	
	if(RTL_R32(RESET_PIN_DATABASE) & (RTL_GPIO_DAT_GPIOA2))
	{
		gpioA2_flag = 1;
	}
	else
	{
		gpioA2_flag = 0;
	}

	if(RTL_R32(RESET_PIN_DATABASE) & (RTL_GPIO_DAT_GPIOB3))
	{
		gpioB3_flag = 1;
	}
	else
	{
		gpioB3_flag = 0;
	}

	return ((1<<gpioA2_flag)|gpioB3_flag);
}

static int get_dc_pwr_plugged_state()
{
	
	if(RTL_R32(RESET_PIN_DATABASE) & (RTL_GPIO_DAT_GPIOC0))
	{
		return 1; //plugged
	}
	else
	{
		return 2; //unplugged
	}

}

static int check_EnablePHYIf()
{
	if(RTL_R32(0xBB804114) & (0x01))
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

static void pocket_ap_timer_func(unsigned long data)
{
//panic_printk("\r\n __[%s-%u]",__FILE__,__LINE__);

	if(ap_cli_rou_idx >= 1)
		ap_cli_rou_idx = 0;
	else
		ap_cli_rou_idx++;

	ap_cli_rou_time_state[ap_cli_rou_idx]=get_pocketAP_ap_cli_rou_state();
	dc_pwr_plugged_time_state = get_dc_pwr_plugged_state();

	if(ap_cli_rou_time_state[0] == ap_cli_rou_time_state[1] )
	{
		if(ap_cli_rou_state != ap_cli_rou_time_state[0])
		{
			ap_cli_rou_state = ap_cli_rou_time_state[0];
			pocketAP_hw_set_flag = '0';
		}
	}

	if(dc_pwr_plugged_state == 0)
	{
		dc_pwr_plugged_state = dc_pwr_plugged_time_state;
	}
	else if(dc_pwr_plugged_state != dc_pwr_plugged_time_state)
	{
		dc_pwr_plugged_state = dc_pwr_plugged_time_state;
		dc_pwr_plugged_flag = '1';
	}
	
//B8b00728 & 0x1F 0x11:L0 0x14:L1  
//panic_printk("\r\n [%d-%d-%d-%d],__[%s-%u]",ap_cli_rou_time_state[0],ap_cli_rou_time_state[1],ap_cli_rou_state,__FILE__,__LINE__);		

//panic_printk("\r\n [0x%x]",RTL_R32(0xB8b00728) & (0x1F));
	pwr_saving_state=(RTL_R32(0xB8b00728) & (0x1F));
//panic_printk("\r\n pwr_saving_state = [0x%x]",pwr_saving_state);

	if(pwr_saving_state == 0x14) // L1 state, in low speed
	{
		if (pwr_saving_led_toggle < 2) {
			RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) | (RTL_GPIO_DAT_GPIOB2)));
			pwr_saving_led_toggle++;
		}
		else if (pwr_saving_led_toggle < 4){
			RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) & (~(RTL_GPIO_DAT_GPIOB2))));
			pwr_saving_led_toggle++;
			if(pwr_saving_led_toggle == 4)
				pwr_saving_led_toggle = 0;
		}
		else
		{
			pwr_saving_led_toggle = 0;
		}
	}
	else // L0 state, always on
	{
		RTL_W32(PABCD_DAT, (RTL_R32(PABCD_DAT) & (~(RTL_GPIO_DAT_GPIOB2))));
	}


	mod_timer(&pocket_ap_timer, jiffies + HZ/2);
}
#endif
int __init rtl_gpio_init(void)
{
	struct proc_dir_entry *res=NULL;

	printk("Realtek GPIO Driver for Flash Reload Default\n");

	// Set GPIOA pin 10(8181)/0(8186) as input pin for reset button

#if  defined(CONFIG_RTL_8196C) || defined(CONFIG_RTL_8198)
#ifndef CONFIG_USING_JTAG
	RTL_W32(RTL_GPIO_MUX, (RTL_R32(RTL_GPIO_MUX) | (RTL_GPIO_MUX_DATA)));
#endif	
#if defined(CONFIG_RTL_8198)
	RTL_W32(RTL_GPIO_MUX, (RTL_R32(RTL_GPIO_MUX) | 0xf));
#endif
#if defined(CONFIG_RTL_8196CS)
extern int PCIE_reset_procedure(int PCIE_Port0and1_8196B_208pin, int Use_External_PCIE_CLK, int mdio_reset,unsigned long conf_addr);
#define CLK_MANAGE	0xb8000010
#define PCI_CONFIG_COMMAND		(0xB8B10000+4)
#define PCI_CONFIG_LATENCY			(0xB8B10000+0x0c)
#define PCI_CONFIG_BASE0			(0xB8B10000+0x10)
#define PCI_CONFIG_BASE1			(0xB8B10000+0x18)
	extern void setBaseAddressRegister(void);
		RTL_W32(CLK_MANAGE, (RTL_R32(CLK_MANAGE) | BIT(11)));
		mdelay(10);
		PCIE_reset_procedure(0, 0, 1, 0xb8b10000);
		setBaseAddressRegister();	
		{

			int i=0;
			*((volatile unsigned long *)PCI_CONFIG_BASE1) = 0x19000000;
			//DEBUG_INFO("...config_base1 = 0x%08lx\n", *((volatile unsigned long *)PCI_CONFIG_BASE1));
			for(i=0; i<1000000; i++);
			*((volatile unsigned char *)PCI_CONFIG_COMMAND) = 0x07;
			//DEBUG_INFO("...command = 0x%08lx\n", *((volatile unsigned long *)PCI_CONFIG_COMMAND));
			for(i=0; i<1000000; i++);
			*((volatile unsigned short *)PCI_CONFIG_LATENCY) = 0x2000;
			for(i=0; i<1000000; i++);
			//DEBUG_INFO("...latency = 0x%08lx\n", *((volatile unsigned long *)PCI_CONFIG_LATENCY));
		}
		
#endif
#ifdef CONFIG_POCKET_ROUTER_SUPPORT

//panic_printk("\r\n 0x%x__[%s-%u]",RTL_R32(RTL_GPIO_MUX),__FILE__,__LINE__);	
#ifndef CONFIG_USING_JTAG
	RTL_W32(RTL_GPIO_MUX, (RTL_R32(RTL_GPIO_MUX) | (RTL_GPIO_MUX_POCKETAP_DATA)));
#endif	
//panic_printk("\r\n 0x%x__[%s-%u]",RTL_R32(RTL_GPIO_MUX),__FILE__,__LINE__);	
	RTL_W32(PABCD_CNR, (RTL_R32(PABCD_CNR) & (~(RTL_GPIO_CNR_POCKETAP_DATA))));





	RTL_W32(PABCD_DIR, (RTL_R32(PABCD_DIR) & (~(RTL_GPIO_DIR_GPIOA2))));

	RTL_W32(PABCD_DIR, (RTL_R32(PABCD_DIR) & (~(RTL_GPIO_DIR_GPIOB3))));

	RTL_W32(PABCD_DIR, (RTL_R32(PABCD_DIR) | ((RTL_GPIO_DIR_GPIOB2))));

	RTL_W32(PABCD_DIR, (RTL_R32(PABCD_DIR) & (~(RTL_GPIO_DIR_GPIOC0))));	

#endif //end of CONFIG_POCKET_ROUTER_SUPPORT 
#if defined(CONFIG_RTL_8196CS)
	RTL_W32(PCIE_PIN_MUX,(RTL_R32(PCIE_PIN_MUX)&(~(1<<29)))|(1<<13) );
	RTL_W32(RESET_PIN_IOBASE, (RTL_R32(RESET_PIN_IOBASE) & (~((1 << RESET_BTN_PIN)<<MODE_MARSK))));
	RTL_W32(RESET_PIN_DIRBASE, (RTL_R32(RESET_PIN_DIRBASE) & (~((1 << RESET_BTN_PIN)<<DIR_MASK  ))));

#else
	RTL_W32(RESET_PIN_IOBASE, (RTL_R32(RESET_PIN_IOBASE) & (~(1 << RESET_BTN_PIN))));
	RTL_W32(RESET_PIN_DIRBASE, (RTL_R32(RESET_PIN_DIRBASE) & (~(1 << RESET_BTN_PIN))));
#endif
#if defined(READ_RF_SWITCH_GPIO)
	RTL_W32(WIFI_ONOFF_PIN_IOBASE, (RTL_R32(WIFI_ONOFF_PIN_DIRBASE) & ( ~(1<<RTL_GPIO_WIFI_ONOFF))));
	RTL_W32(WIFI_ONOFF_PIN_DIRBASE, (RTL_R32(WIFI_ONOFF_PIN_DIRBASE) & (~(1<<RTL_GPIO_WIFI_ONOFF))));
	RTL_W32(WIFI_ONOFF_PIN_DATABASE, (RTL_R32(WIFI_ONOFF_PIN_DATABASE) & (~(1<<RTL_GPIO_WIFI_ONOFF))));

#endif // #if defined(READ_RF_SWITCH_GPIO)
#endif // #if defined(CONFIG_RTL865X)
#if  defined(CONFIG_RTL_8196CS)
	RTL_W32(RESET_LED_IOBASE, (RTL_R32(RESET_LED_IOBASE) | (((1 << RESET_LED_PIN)))));
	RTL_W32(RESET_LED_DIRBASE, (RTL_R32(RESET_LED_DIRBASE) | ((1 << RESET_LED_PIN))));
#else
	// Set GPIOA ping 2 as output pin for reset led
	RTL_W32(RESET_LED_DIRBASE, (RTL_R32(RESET_LED_DIRBASE) | ((1 << RESET_LED_PIN))));
#endif
#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE	
	res = create_proc_entry("bootbank", 0, NULL);
	if (res) {
		res->read_proc = read_bootbank_proc;
		//res->write_proc = write_bootbank_proc;
	}
	else {
		printk("read bootbank, create proc failed!\n");
	}
#endif
#ifdef AUTO_CONFIG
	res = create_proc_entry("gpio", 0, NULL);
	if (res) {
		res->read_proc = read_proc;
		res->write_proc = write_proc;
	}
	else {
		printk("Realtek GPIO Driver, create proc failed!\n");
	}

// 2009-0414		
#if defined(USE_INTERRUPT_GPIO)
#if defined(CONFIG_RTL_8198) && !defined(CONFIG_RTK_VOIP)
	RTL_R32(AUTOCFG_PIN_IMR) |= (0x01 << (AUTOCFG_BTN_PIN-16)*2); // enable interrupt in falling-edge		
#else
	RTL_R32(AUTOCFG_PIN_IMR) |= (0x01 << AUTOCFG_BTN_PIN*2); // enable interrupt in falling-edge	
#endif
	if (request_irq(GPIO_IRQ_NUM, gpio_interrupt_isr, IRQF_DISABLED, "rtl_gpio", NULL)) {
		//panic_printk("gpio request_irq(%d) error!\n", GPIO_IRQ_NUM);		
   	}
#endif
// 2009-0414		
#endif

	res = create_proc_entry("load_default", 0, NULL);
	if (res) {
		res->read_proc = default_read_proc;
		res->write_proc = default_write_proc;
	}

#ifdef READ_RF_SWITCH_GPIO
	res = create_proc_entry("rf_switch", 0, NULL);
	if (res) {
		res->read_proc = rf_switch_read_proc;
		res->write_proc = NULL;
	}
#endif



#ifdef CONFIG_POCKET_ROUTER_SUPPORT

	res = create_proc_entry("dc_pwr", 0, NULL);
	if (res)
		res->read_proc = read_dc_pwr_proc;
	else
		printk("create read_dc_pwr_proc failed!\n");

	res = create_proc_entry("dc_pwr_plugged_flag", 0, NULL);
	if (res)
	{
		res->read_proc = read_dc_pwr_plugged_flag_proc;
	}
	else
		printk("create read_pocketAP_hw_set_flag_proc failed!\n");
	
	res = create_proc_entry("ap_client_rou", 0, NULL);
	if (res)
		res->read_proc = read_ap_client_rou_proc;
	else
		printk("create read_ap_client_rou_proc failed!\n");

	res = create_proc_entry("pocketAP_hw_set_flag", 0, NULL);
	if (res)
	{
		res->read_proc = read_pocketAP_hw_set_flag_proc;
		res->write_proc = write_pocketAP_hw_set_flag_proc;
	}
	else
		printk("create read_pocketAP_hw_set_flag_proc failed!\n");

	res = create_proc_entry("phyif", 0, NULL);
	if (res)
		res->read_proc = read_EnablePHYIf_proc;
	else
		printk("create read_EnablePHYIf_proc failed!\n");
				
	init_timer(&pocket_ap_timer);
	pocket_ap_timer.data = (unsigned long)NULL;
	pocket_ap_timer.function = &pocket_ap_timer_func;
	mod_timer(&pocket_ap_timer, jiffies + HZ);
#endif

	init_timer(&probe_timer);
	probe_counter = 0;
	probe_state = PROBE_NULL;
	probe_timer.expires = jiffies + HZ;
	probe_timer.data = (unsigned long)NULL;
	probe_timer.function = &rtl_gpio_timer;
	mod_timer(&probe_timer, jiffies + HZ);

#ifdef CONFIG_RTL865X_CMO
	extra_led_gpio_init();
#endif
	return 0;
}


static void __exit rtl_gpio_exit(void)
{
	printk("Unload Realtek GPIO Driver \n");
	del_timer_sync(&probe_timer);
}


module_exit(rtl_gpio_exit);
module_init(rtl_gpio_init);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO driver for Reload default");
