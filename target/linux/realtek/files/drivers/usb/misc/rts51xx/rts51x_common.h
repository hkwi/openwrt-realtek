/* Driver for Realtek RTS51xx USB card reader
 * Header file
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

#ifndef __REALTEK_RTS51X_COMMON_H
#define __REALTEK_RTS51X_COMMON_H

struct Scsi_Host;
struct scsi_device;
struct scsi_cmnd;

const char* host_info(struct Scsi_Host *host);
int slave_alloc (struct scsi_device *sdev);
int slave_configure(struct scsi_device *sdev);
int proc_info (struct Scsi_Host *host, char *buffer,
		char **start, off_t offset, int length, int inout);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 37)
int queuecommand(struct scsi_cmnd *srb,
			void (*done)(struct scsi_cmnd *));
#else
int queuecommand(struct Scsi_Host *, struct scsi_cmnd *);
#endif
int command_abort(struct scsi_cmnd *srb);
int device_reset(struct scsi_cmnd *srb);
int bus_reset(struct scsi_cmnd *srb);

#endif 
