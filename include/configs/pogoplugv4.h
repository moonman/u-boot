/*
 * Copyright (C) 2015 Oleg Rakhmanov <oleg@archlinuxarm.org>
 *
 * Based on
 * Copyright (C) 2012
 * David Purdy <david.c.purdy@gmail.com>
 *
 * Based on Kirkwood support:
 * (C) Copyright 2009
 * Marvell Semiconductor <www.marvell.com>
 * Written-by: Prafulla Wadaskar <prafulla@marvell.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CONFIG_POGOPLUGV4_H
#define _CONFIG_POGOPLUGV4_H

/*
 * Machine type definition and ID
 */
#define MACH_TYPE_POGOPLUGV4		3960
#define CONFIG_MACH_TYPE		MACH_TYPE_POGOPLUGV4
#define CONFIG_IDENT_STRING		" Arch Linux ARM\nPogoplug V4"

/*
 * High Level Configuration Options (easy to change)
 */
#define CONFIG_FEROCEON_88FR131		/* #define CPU Core subversion */
#define CONFIG_KIRKWOOD			/* SOC Family Name */
#define CONFIG_KW88F6192		/* SOC Name */
#define CONFIG_SKIP_LOWLEVEL_INIT	/* disable board lowlevel_init */

/*
 * Commands configuration
 */
#define CONFIG_SYS_NO_FLASH		/* Declare no flash (NOR/SPI) */
#define CONFIG_CONSOLE_MUX
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

#include <config_cmd_default.h>
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_ENV
#define CONFIG_CMD_MII
#define CONFIG_CMD_MMC
#define CONFIG_CMD_NAND
#define CONFIG_CMD_PING
#define CONFIG_CMD_USB
#define CONFIG_SYS_MVFS /* Picks up Filesystem from mv-common.h */
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_SETEXPR
#define CONFIG_SUPPORT_RAW_INITRD
#define CONFIG_OF_LIBFDT

#define CONFIG_CMD_DATE
#define CONFIG_CMD_IDE
#define CONFIG_SYS_LONGHELP
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_PREBOOT

/*
 * mv-common.h should be defined after CMD configs since it used them
 * to enable certain macros
 */
#include "mv-common.h"

/* Remove or override few declarations from mv-common.h */
#undef CONFIG_SYS_PROMPT	/* previously defined in mv-common.h */
#define CONFIG_SYS_PROMPT	"Pogoplugv4> "
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2 "> "


/*
 *  Environment variables configurations
 */
#ifdef CONFIG_CMD_NAND
#define CONFIG_ENV_IS_IN_NAND		1
#define CONFIG_ENV_SECT_SIZE		0x20000	/* 128K */
#else
#define CONFIG_ENV_IS_NOWHERE		1
#endif

#define CONFIG_ENV_SIZE			0x20000	/* 128k */
#define CONFIG_ENV_ADDR			0xC0000
#define CONFIG_ENV_OFFSET		0xC0000 /* env starts here */
#define CONFIG_LOADADDR			0x810000

/*
 * Default environment variables
 */
#define CONFIG_MTDPARTS \
	"mtdparts=orion_nand:1M(u-boot),-(rootfs)\0"

#define CONFIG_EXTRA_ENV_SETTINGS \

	"console=ttyS0\0" \
	"mtdids=nand0=orion_nand\0" \
	"mtdparts="CONFIG_MTDPARTS \
	"zimage=/boot/zImage\0" \
	"uimage=/boot/uImage\0" \
	"fdt_file=/boot/dtbs/kirkwood-pogoplugv4.dtb\0" \
	"fdt_addr=0x800000\0" \
	"setargs=setenv bootargs console=${console},${baudrate} " \
		"${optargs} " \
		"root=/dev/sd${letter}1 rw rootwait " \
		"${mtdparts}\0" \
	"loadbootenv=load ${type} ${disk}:1 ${loadaddr} /boot/uEnv.txt\0" \
	"importbootenv=echo Importing environment (uEnv.txt)...; " \
		"env import -t $loadaddr $filesize\0" \
	"loaduimage=load ${type} ${disk}:1 ${loadaddr} ${uimage}\0" \
	"loadzimage=load ${type} ${disk}:1 ${loadaddr} ${zimage}\0" \
	"loadfdt=load ${type} ${disk}:1 ${fdt_addr} ${fdt_file}\0" \
	"bootz=echo Booting from ${disk} ...; " \
		"run setargs; " \
		"bootz ${loadaddr} - ${fdt_addr};\0" \
	"bootm=echo Booting from ${disk} ...; " \
		"run setargs; " \
		"bootm ${loadaddr};\0" \
	"load=echo Attempting to boot from ${type} ${disk}:1...;" \
		"if run loadbootenv; then " \
			"run importbootenv;" \
		"fi;" \
		"echo Checking if uenvcmd is set ...;" \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...;" \
			"run uenvcmd;" \
		"fi;" \
		"echo Running default loadzimage ...;" \
		"if run loadzimage; then " \
			"run loadfdt;" \
			"run bootz;" \
		"fi;" \
		"echo Running default loaduimage ...;" \
		"if run loaduimage; then " \
			"run bootm;" \
		"fi;\0"

#define CONFIG_BOOTCOMMAND \
	"ide reset; usb start; setenv letter 9;" \
	"for type in mmc ide usb; do " \
		"for disk in 0 1; do " \
			"if ${type} part ${disk};then " \
				"setexpr letter $letter + 1;" \
				"run load;" \
			"fi;" \
		"done;" \
	"done;"

/*
 * Ethernet Driver configuration
 */
#ifdef CONFIG_CMD_NET
#define CONFIG_MVGBE_PORTS	{1, 0}	/* enable port 0 only */
#define CONFIG_PHY_BASE_ADR	0
#endif /* CONFIG_CMD_NET */

/*
 * SATA 
 */
#ifdef CONFIG_MVSATA_IDE
#define CONFIG_SYS_ATA_IDE0_OFFSET	MV_SATA_PORT0_OFFSET
#endif

/*
 * Device Tree
 */

#define CONFIG_OF_LIBFDT

/*
 * EFI partition
 */

#define CONFIG_EFI_PARTITION

/*
 *  Date Time
 *   */
#ifdef CONFIG_CMD_DATE
#define CONFIG_RTC_MV
#define CONFIG_CMD_SNTP
#define CONFIG_CMD_DNS
#endif /* CONFIG_CMD_DATE */

/*
 *
 */

#define CONFIG_KIRKWOOD_GPIO

#endif /* _CONFIG_POGOPLUGV4_H */
