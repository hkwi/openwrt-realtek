#include <linux/kernel.h>
#include <linux/init.h>

int scrlog_printk(const char *fmt, ...) {
	
	va_list args;
	int r;
	va_start(args, fmt);
	r = vprintk(fmt, args);
        va_end(args);
	return r;		
}

int panic_printk(const char *fmt, ...) {
	va_list args;
	int r;
	va_start(args, fmt);
	r = vprintk(fmt, args);
        va_end(args);
	return r;
}