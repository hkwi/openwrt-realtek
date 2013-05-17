/* Driver for Realtek RTS51xx USB card reader
 *
 * Copyright(c) 2009 Realtek Semiconductor Corp. All rights reserved.  
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http:
 *
 * Author:
 *   wwang (wei_wang@realsil.com.cn)
 *   No. 450, Shenhu Road, Suzhou Industry Park, Suzhou, China
 */

#include <linux/blkdev.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/workqueue.h>

#include "timestamp.h"
#include "rts51x.h"
#include "rts51x_chip.h"

/***********************************************************************
 * Host functions 
 ***********************************************************************/

const char* host_info(struct Scsi_Host *host)
{
	return "SCSI emulation for RTS51xx USB driver-based card reader";
}

int slave_alloc (struct scsi_device *sdev)
{
	/*
	 * Set the INQUIRY transfer length to 36.  We don't use any of
	 * the extra data and many devices choke if asked for more or
	 * less than 36 bytes.
	 */
	sdev->inquiry_len = 36;
	return 0;
}

int slave_configure(struct scsi_device *sdev)
{
	/* Scatter-gather buffers (all but the last) must have a length
	 * divisible by the bulk maxpacket size.  Otherwise a data packet
	 * would end up being short, causing a premature end to the data
	 * transfer.  Since high-speed bulk pipes have a maxpacket size
	 * of 512, we'll use that as the scsi device queue's DMA alignment
	 * mask.  Guaranteeing proper alignment of the first buffer will
	 * have the desired effect because, except at the beginning and
	 * the end, scatter-gather buffers follow page boundaries. */
	blk_queue_dma_alignment(sdev->request_queue, (512 - 1));

	/* Set the SCSI level to at least 2.  We'll leave it at 3 if that's
	 * what is originally reported.  We need this to avoid confusing
	 * the SCSI layer with devices that report 0 or 1, but need 10-byte
	 * commands (ala ATAPI devices behind certain bridges, or devices
	 * which simply have broken INQUIRY data).
	 *
	 * NOTE: This means /dev/sg programs (ala cdrecord) will get the
	 * actual information.  This seems to be the preference for
	 * programs like that.
	 *
	 * NOTE: This also means that /proc/scsi/scsi and sysfs may report
	 * the actual value or the modified one, depending on where the
	 * data comes from.
	 */
	if (sdev->scsi_level < SCSI_2)
		sdev->scsi_level = sdev->sdev_target->scsi_level = SCSI_2;

	return 0;
}


/***********************************************************************
 * /proc/scsi/ functions
 ***********************************************************************/


#undef SPRINTF
#define SPRINTF(args...) \
	do { if (pos < buffer+length) pos += sprintf(pos, ## args); } while (0)

int proc_info (struct Scsi_Host *host, char *buffer,
		char **start, off_t offset, int length, int inout)
{
	char *pos = buffer;

	
	if (inout)
		return length;

	
	SPRINTF("   Host scsi%d: %s\n", host->host_no, RTS51X_NAME);

	
	SPRINTF("       Vendor: Realtek Corp.\n");
	SPRINTF("      Product: RTS51xx USB Card Reader\n");
	SPRINTF("      Version: %s\n", DRIVER_VERSION);
	SPRINTF("        Build: %s\n", DRIVER_MAKE_TIME);

	/*
	 * Calculate start of next buffer, and return value.
	 */
	*start = buffer + offset;

	if ((pos - buffer) < offset)
		return (0);
	else if ((pos - buffer - offset) < length)
		return (pos - buffer - offset);
	else
		return (length);
}



int queuecommand_lck(struct scsi_cmnd *srb,
			void (*done)(struct scsi_cmnd *))
{
	struct rts51x_chip *chip = host_to_rts51x(srb->device->host);

	
	if (chip->srb != NULL) {
		RTS51X_DEBUGP(("Error in %s: chip->srb = %p\n",
			__FUNCTION__, chip->srb));
		return SCSI_MLQUEUE_HOST_BUSY;
	}

	
	if (test_bit(FLIDX_DISCONNECTING, &chip->usb->dflags)) {
		RTS51X_DEBUGP(("Fail command during disconnect\n"));
		srb->result = DID_NO_CONNECT << 16;
		done(srb);
		return 0;
	}

	
	srb->scsi_done = done;
	chip->srb = srb;
	complete(&chip->usb->cmnd_ready);

	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 37)
int queuecommand(struct scsi_cmnd *srb,
			void (*done)(struct scsi_cmnd *))
{
	return queuecommand_lck(srb, done);
}
#else
DEF_SCSI_QCMD(queuecommand)
#endif

/***********************************************************************
 * Error handling functions
 ***********************************************************************/


int command_abort(struct scsi_cmnd *srb)
{
	struct rts51x_chip *chip = host_to_rts51x(srb->device->host);

	RTS51X_DEBUGP(("%s called\n", __func__));

	/* us->srb together with the TIMED_OUT, RESETTING, and ABORTING
	 * bits are protected by the host lock. */
	scsi_lock(rts51x_to_host(chip));

	
	if (chip->srb != srb) {
		scsi_unlock(rts51x_to_host(chip));
		RTS51X_DEBUGP(("-- nothing to abort\n"));
		return FAILED;
	}

	/* Set the TIMED_OUT bit.  Also set the ABORTING bit, but only if
	 * a device reset isn't already in progress (to avoid interfering
	 * with the reset).  Note that we must retain the host lock while
	 * calling usb_stor_stop_transport(); otherwise it might interfere
	 * with an auto-reset that begins as soon as we release the lock. */
	set_bit(FLIDX_TIMED_OUT, &chip->usb->dflags);
	if (!test_bit(FLIDX_RESETTING, &chip->usb->dflags)) {
		set_bit(FLIDX_ABORTING, &chip->usb->dflags);
	}
	scsi_unlock(rts51x_to_host(chip));

	
	wait_for_completion(&chip->usb->notify);
	return SUCCESS;
}

/* This invokes the transport reset mechanism to reset the state of the
 * device */
int device_reset(struct scsi_cmnd *srb)
{
	int result = 0;

	RTS51X_DEBUGP(("%s called\n", __FUNCTION__));

	return result < 0 ? FAILED : SUCCESS;
}


int bus_reset(struct scsi_cmnd *srb)
{
	int result = 0;

	RTS51X_DEBUGP(("%s called\n", __FUNCTION__));
	
	return result < 0 ? FAILED : SUCCESS;
}
