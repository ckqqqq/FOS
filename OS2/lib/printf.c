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
/*****************************************************************************
 *                                printf
 *****************************************************************************/

PUBLIC int printf(const char *fmt, ...)
{
	int i;
	char buf[STR_DEFAULT_LEN];

	va_list arg = (va_list)((char*)(&fmt) + 4);        /* 4 是参数 fmt 所占堆栈中的大小 */
	i = vsprintf(buf, fmt, arg);
	int c = write(1, buf, i);

	assert(c == i);

	return i;
}

/*****************************************************************************
 *                                printl
 *****************************************************************************/

PUBLIC int printl(const char *fmt, ...)
{
	int i;
	char buf[STR_DEFAULT_LEN/2];

	va_list arg = (va_list)((char*)(&fmt) + 4); 
	i = vsprintf(buf, fmt, arg);
	printx(buf);

	return i;
}

