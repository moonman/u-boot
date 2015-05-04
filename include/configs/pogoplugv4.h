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
	"bootfilez=zImage\0" \
	"bootfilem=uImage\0" \
	"bootdir=/boot\0" \
	"console=ttyS0,115200\0" \
	"ethact=egiga0\0" \
	"rdaddr=0x1100000\0" \
	"loadaddr=0x810000\0" \
	"rdfile=initramfs-linux.img\0" \
	"fdtaddr=0x800000\0" \
	"fdtfile=kirkwood-pogoplugv4.dtb\0" \
	"fdtdir=/boot/dtbs\0" \
	"optargs=\0" \
	"mtdparts="CONFIG_MTDPARTS \
	"mtdids=nand0=orion_nand\0" \
	"mainargs=setenv bootargs console=${console} ${mtdparts} " \
		"root=${root} rw rootwait " \
		"${optargs}\0 " \
	"loadimage=load ${devtype} ${bootpart} ${loadaddr} ${bootdir}/${bootfilez} || load ${devtype} ${bootpart} ${loadaddr} ${bootdir}/${bootfilem}\0" \
	"loadrd=load ${devtype} ${bootpart} ${rdaddr} ${bootdir}/${rdfile}\0" \
	"loadfdt=echo loading ${fdtdir}/${fdtfile} ...; load ${devtype} ${bootpart} ${fdtaddr} ${fdtdir}/${fdtfile}\0" \
	"mountubi=ubi part rootfs; ubifsmount ubi0:rootfs\0" \
	"startboot=usb start; ide reset; " \
		"for devtype in mmc usb ide; do " \
			"setenv devnum 0; " \
			"while ${devtype} dev ${devnum}; do " \
				"echo ${devtype} found on device ${devnum}; " \
				"setenv bootpart ${devnum}:1; " \
				"echo Checking for: ${bootdir}/uEnv.txt ...; " \
				"if test -e ${devtype} ${bootpart} ${bootdir}/uEnv.txt; then " \
					"load ${devtype} ${bootpart} ${loadaddr} ${bootdir}/uEnv.txt; " \
					"env import -t ${loadaddr} ${filesize}; " \
					"echo Loaded environment from ${bootdir}/uEnv.txt; " \
					"echo Checking if uenvcmd is set ...; " \
					"if test -n ${uenvcmd}; then " \
						"echo Running uenvcmd ...; " \
						"run uenvcmd; " \
					"fi; " \
				"fi; " \
				"if run loadimage; then " \
					"if env exists root; then " \
						"echo root has been defined by user; " \ 
					"else " \
						"part uuid ${devtype} ${bootpart} uuid; " \
						"setenv root PARTUUID=${uuid}; " \
                                        "fi; " \
					"run mainargs; " \
					"if run loadfdt; then " \
						"if run loadrd; then " \
							"bootz ${loadaddr} ${rdaddr}:${filesize} ${fdtaddr}; " \
						"else " \
							"bootz ${loadaddr} - ${fdtaddr}; " \
						"fi; " \
					"else " \
						"if run loadrd; then " \
							"bootm ${loadaddr} ${rdaddr}:${filesize}; " \
						"else " \
							"bootm ${loadaddr}; " \
						"fi; " \
					"fi; " \
				"else " \
					"echo No kernel found; " \
				"fi; " \
				"setexpr devnum ${devnum} + 1; " \
			"done; " \
		"done;\0" \
		"bootubi=echo Trying to boot from NAND ...; " \
		"if run mountubi; then " \
			"ubifsload ${loadaddr} /boot/zImage;ubifsload ${fdtaddr} /boot/dtbs/${fdtfile}; " \
			"ubifsumount; " \
			"setenv bootargs console=${console} ubi.mtd=1 root=ubi0:rootfs ro rootfstype=ubifs  rootwait ${mtdparts}; " \
			"bootz ${loadaddr} - ${fdtaddr}; " \
                "fi\0"

#define CONFIG_BOOTCOMMAND \
        "run startboot; run bootubi"
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
