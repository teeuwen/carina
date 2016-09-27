/*
 *
 * Carina
 * src/kernel/lib/stdio.c
 *
 * Copyright (C) 2016 Bastiaan Teeuwen <bastiaan.teeuwen170@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 */

#include <kbd/kbd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <system.h>
#include <video/vga.h>
#include <kernel/print.h>

/*
 * TODO This has to be safer
 * Don't use kprintf, switch to VGA if double fault, etc...
 * TODO Also relocate in other file
 * TODO Seperate panic for isrs
 * TODO Dump registers (at least rip/eip)
 */
void panic(char *reason, u64 err_code)
{
	asm volatile ("cli");

	//TODO Hide cursor

	kprintf(KP_CRIT, "panic", "%s", reason); //TODO First do check for specific err

	if ((err_code >> 1) & 0x00)
		prints(" in GDT\n");
	else if ((err_code >> 1) & 0x01)
		prints(" in IDT\n");
	else if ((err_code >> 1) & 0x02)
		prints(" in LDT\n");
	else if ((err_code >> 1) & 0x03)
		prints(" in IDT\n");
	else
		printc('\n');

	/* TODO Don't always print error code */
	kprintf(KP_CRIT, "panic", "Error code: %#x\n", err_code);

	prints("The system has been halted.");

	for (;;)
		asm volatile ("hlt");
}

void printc(char c)
{
	printcc(c, vga_fgcolor);
}

void printcc(char c, u8 color)
{
	vga_putch(c, color);
}

void prints(char *str)
{
	u32 length = 0;
	while (str[length])
		printc(str[length++]);
}