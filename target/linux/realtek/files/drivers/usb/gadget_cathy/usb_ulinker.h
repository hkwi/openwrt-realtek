#ifndef __LINUX_USB_ULINKER_H
#define __LINUX_USB_ULINKER_H

#if defined(CONFIG_RTL_ULINKER)

#define ULINKER_DEVINIT
#define ULINKER_DEVINITDATA
#define ULINKER_DEVINITCONST
#define ULINKER_DEVEXIT
#define ULINKER_DEVEXITDATA
#define ULINKER_DEVEXITCONST

#else

#define ULINKER_DEVINIT		__devinit
#define ULINKER_DEVINITDATA	__devinitdata
#define ULINKER_DEVINITCONST	__devinitconst
#define ULINKER_DEVEXIT		__devexit
#define ULINKER_DEVEXITDATA	__devexitdata
#define ULINKER_DEVEXITCONST	__devexitconst

#endif

#endif	/* __LINUX_USB_ULINKER_H */
