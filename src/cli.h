/*********************************************************************************
 ** Copyright © 2011 - 2020 Petros Koutoupis
 ** All rights reserved.
 **
 ** This file is part of RapidDisk.
 **
 ** RapidDisk is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 2 of the License, or
 ** (at your option) any later version.
 **
 ** RapidDisk is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with RapidDisk.  If not, see <http://www.gnu.org/licenses/>.
 **
 ** SPDX-License-Identifier: GPL-2.0-or-later
 **
 ** @project: rapiddisk
 **
 ** @filename: main.c
 ** @description: This is the main file for the RapidDisk userland tool.
 **
 ** @date: 26Aug20, petros@petroskoutoupis.com
 ********************************************************************************/

#ifndef CLI_H
#define CLI_H

#include <malloc.h>

#define PROC_MODULES			"/proc/modules"
#define SYS_RDSK			"/sys/kernel/rapiddisk/mgmt"
#define SYS_BLOCK			"/sys/block"
#define ETC_MTAB			"/etc/mtab"
#define DEV_MAPPER			"/dev/mapper"

#define ACTION_NONE			0x0
#define ACTION_ATTACH			0x1
#define ACTION_DETACH			0x2
#define ACTION_FLUSH			0x3
#define ACTION_LIST			0x4
#define ACTION_CACHE_MAP		0x5
#define ACTION_RESIZE			0x6
#define ACTION_CACHE_STATS		0x7
#define ACTION_CACHE_UNMAP		0x8
#define ACTION_QUERY_RESOURCES		0x9

#define NAMELEN				0x200
#define FILEDATA			0x40
#define BYTES_PER_SECTOR		0x200

typedef struct RD_PROFILE {      /* For RapidDisk device list     */
	unsigned char device[0xf];
	unsigned long long size;
	struct RD_PROFILE *next;
} RD_PROFILE;

typedef struct RC_PROFILE {      /* For RapidDisk-Cache node list */
	unsigned char device[NAMELEN];
	unsigned char cache[0x20];
	unsigned char source[NAMELEN];
	struct RC_PROFILE *next;
} RC_PROFILE;

typedef struct RC_STATS {
	unsigned char device[NAMELEN];
	unsigned int reads;
	unsigned int writes;
	unsigned int cache_hits;
	unsigned int replacement;
	unsigned int write_replacement;
	unsigned int read_invalidates;
	unsigned int write_invalidates;
	unsigned int uncached_reads;
	unsigned int uncached_writes;
	unsigned int disk_reads;
	unsigned int disk_writes;
	unsigned int cache_reads;
	unsigned int cache_writes;

	/* Unsupported in this release */
	unsigned int read_ops;
	unsigned int write_ops;
} RC_STATS;

typedef struct MEM_PROFILE {
        unsigned long long mem_total;
        unsigned long long mem_free;
} MEM_PROFILE;

typedef struct VOLUME_PROFILE {
	unsigned char device[0x20];
	unsigned long long size;
	unsigned char vendor[FILEDATA];
	unsigned char model[FILEDATA];
	struct VOLUME_PROFILE *next;
} VOLUME_PROFILE;

struct RD_PROFILE *search_rdsk_targets(void);
struct RC_PROFILE *search_cache_targets(void);
unsigned char *read_info(unsigned char *, unsigned char *);
int mem_device_list(struct RD_PROFILE *, struct RC_PROFILE *);
int mem_device_attach(struct RD_PROFILE *, unsigned long);
int mem_device_detach(struct RD_PROFILE *, struct RC_PROFILE *, unsigned char *);
int mem_device_resize(struct RD_PROFILE *, unsigned char *, unsigned long);
int mem_device_flush(struct RD_PROFILE *, RC_PROFILE *, unsigned char *);
int cache_device_map(struct RD_PROFILE *, struct RC_PROFILE *, unsigned char *, unsigned char *, int);
int cache_device_unmap(struct RC_PROFILE *, unsigned char *);
int cache_device_stat(struct RC_PROFILE *, unsigned char *);
int cache_device_stat_json(struct RC_PROFILE *, unsigned char *);
int json_device_list(struct RD_PROFILE *, struct RC_PROFILE *);
int json_resources_list(struct MEM_PROFILE *, struct VOLUME_PROFILE *);
int json_cache_statistics(struct RC_STATS *);
int json_status_return(int);
int get_memory_usage(struct MEM_PROFILE *);
struct VOLUME_PROFILE *search_volumes_targets(void);
int resources_list(struct MEM_PROFILE *, struct VOLUME_PROFILE *);

#endif
