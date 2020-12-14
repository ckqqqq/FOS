
#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

/* #define __TTY_DEBUG__ */

/* local routines */
PRIVATE void	set_cursor(unsigned int position);
PRIVATE void	set_video_start_addr(u32 addr);
PRIVATE void	flush(CONSOLE* con);
PRIVATE	void	w_copy(unsigned int dst, const unsigned int src, int size);
PRIVATE void	clear_screen(int pos, int len);

/*****************************************************************************
 *                                init_screen
 *****************************************************************************/

PUBLIC void init_screen(TTY* tty)
{
	int nr_tty = tty - tty_table;
	tty->console = console_table + nr_tty;

	int v_mem_size = V_MEM_SIZE >> 1; /* size of Video Memory */
	int size_per_con = v_mem_size / NR_CONSOLES;
	tty->console->orig = nr_tty * size_per_con;
	tty->console->con_size = size_per_con / SCR_WIDTH * SCR_WIDTH;
	tty->console->cursor = tty->console->crtc_start = tty->console->orig;
	tty->console->is_full = 0;

	if (nr_tty == 0) {
		tty->console->cursor = disp_pos / 2;
		disp_pos = 0;
	}
	else {
		const char prompt[] = "[TTY #?]\n";

		const char * p = prompt;
		for (; *p; p++)
			out_char(tty->console, *p == '?' ? nr_tty + '0' : *p);
	}

	set_cursor(tty->console->cursor);
}


/*****************************************************************************
 *                                out_char
 *****************************************************************************/

PUBLIC void out_char(CONSOLE* con, char ch)
{
	u8* pch = (u8*)(V_MEM_BASE + con->cursor * 2);

	assert(con->cursor - con->orig < con->con_size);

	int cursor_x = (con->cursor - con->orig) % SCR_WIDTH;
	int cursor_y = (con->cursor - con->orig) / SCR_WIDTH;

	switch(ch) {
	case '\n':
		con->cursor = con->orig + SCR_WIDTH * (cursor_y + 1);
		break;
	case '\b':
		if (con->cursor > con->orig) {
			con->cursor--;
			*(pch - 2) = ' ';
			*(pch - 1) = DEFAULT_CHAR_COLOR;
		}
		break;
	default:
		*pch++ = ch;
		*pch++ = DEFAULT_CHAR_COLOR;
		con->cursor++;
		break;
	}

	if (con->cursor - con->orig >= con->con_size) {
		cursor_x = (con->cursor - con->orig) % SCR_WIDTH;
		cursor_y = (con->cursor - con->orig) / SCR_WIDTH;
		int cp_orig = con->orig + (cursor_y + 1) * SCR_WIDTH - SCR_SIZE;
		w_copy(con->orig, cp_orig, SCR_SIZE - SCR_WIDTH);
		con->crtc_start = con->orig;
		con->cursor = con->orig + (SCR_SIZE - SCR_WIDTH) + cursor_x;
		clear_screen(con->cursor, SCR_WIDTH);
		if (!con->is_full)
			con->is_full = 1;
	}

	assert(con->cursor - con->orig < con->con_size);

	while (con->cursor >= con->crtc_start + SCR_SIZE ||
	       con->cursor < con->crtc_start) {
		scroll_screen(con, SCR_UP);

		clear_screen(con->cursor, SCR_WIDTH);
	}

	flush(con);
}

/*****************************************************************************
 *                                clear_screen
 *****************************************************************************/

PRIVATE void clear_screen(int pos, int len)
{
	u8 * pch = (u8*)(V_MEM_BASE + pos * 2);
	while (--len >= 0) {
		*pch++ = ' ';
		*pch++ = DEFAULT_CHAR_COLOR;
	}
}


/*****************************************************************************
 *                            is_current_console
 *****************************************************************************/

PUBLIC int is_current_console(CONSOLE* con)
{
	return (con == &console_table[current_console]);
}


/*****************************************************************************
 *                                set_cursor
 *****************************************************************************/

PRIVATE void set_cursor(unsigned int position)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, CURSOR_H);
	out_byte(CRTC_DATA_REG, (position >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CURSOR_L);
	out_byte(CRTC_DATA_REG, position & 0xFF);
	enable_int();
}


/*****************************************************************************
 *                                set_video_start_addr
 *****************************************************************************/

PRIVATE void set_video_start_addr(u32 addr)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, START_ADDR_H);
	out_byte(CRTC_DATA_REG, (addr >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, START_ADDR_L);
	out_byte(CRTC_DATA_REG, addr & 0xFF);
	enable_int();
}


/*****************************************************************************
 *                                select_console
 *****************************************************************************/
PUBLIC void select_console(int nr_console)
{
	if ((nr_console < 0) || (nr_console >= NR_CONSOLES)) return;

	flush(&console_table[current_console = nr_console]);
}


/*****************************************************************************
 *                                scroll_screen
 *****************************************************************************/

PUBLIC void scroll_screen(CONSOLE* con, int dir)
{

	int oldest; 
	int newest; 
	int scr_top;

	newest = (con->cursor - con->orig) / SCR_WIDTH * SCR_WIDTH;
	oldest = con->is_full ? (newest + SCR_WIDTH) % con->con_size : 0;
	scr_top = con->crtc_start - con->orig;

	if (dir == SCR_DN) {
		if (!con->is_full && scr_top > 0) {
			con->crtc_start -= SCR_WIDTH;
		}
		else if (con->is_full && scr_top != oldest) {
			if (con->cursor - con->orig >= con->con_size - SCR_SIZE) {
				if (con->crtc_start != con->orig)
					con->crtc_start -= SCR_WIDTH;
			}
			else if (con->crtc_start == con->orig) {
				scr_top = con->con_size - SCR_SIZE;
				con->crtc_start = con->orig + scr_top;
			}
			else {
				con->crtc_start -= SCR_WIDTH;
			}
		}
	}
	else if (dir == SCR_UP) {
		if (!con->is_full && newest >= scr_top + SCR_SIZE) {
			con->crtc_start += SCR_WIDTH;
		}
		else if (con->is_full && scr_top + SCR_SIZE - SCR_WIDTH != newest) {
			if (scr_top + SCR_SIZE == con->con_size)
				con->crtc_start = con->orig;
			else
				con->crtc_start += SCR_WIDTH;
		}
	}
	else {
		assert(dir == SCR_DN || dir == SCR_UP);
	}

	flush(con);
}


/*****************************************************************************
 *                                flush
 *****************************************************************************/

PRIVATE void flush(CONSOLE* con)
{
	if (is_current_console(con)) {
		set_cursor(con->cursor);
		set_video_start_addr(con->crtc_start);
	}

#ifdef __TTY_DEBUG__
	int lineno = 0;
	for (lineno = 0; lineno < con->con_size / SCR_WIDTH; lineno++) {
		u8 * pch = (u8*)(V_MEM_BASE +
				   (con->orig + (lineno + 1) * SCR_WIDTH) * 2
				   - 4);
		*pch++ = lineno / 10 + '0';
		*pch++ = RED_CHAR;
		*pch++ = lineno % 10 + '0';
		*pch++ = RED_CHAR;
	}
#endif
}

/*****************************************************************************
 *                                w_copy
 *****************************************************************************/

PRIVATE	void w_copy(unsigned int dst, const unsigned int src, int size)
{
	phys_copy((void*)(V_MEM_BASE + (dst << 1)),
		  (void*)(V_MEM_BASE + (src << 1)),
		  size << 1);
}

