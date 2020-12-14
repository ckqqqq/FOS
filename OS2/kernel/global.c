
#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "fs.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "global.h"
#include "proto.h"


PUBLIC	struct proc proc_table[NR_TASKS + NR_PROCS];

/* 注意下面的 TASK 的顺序要与 const.h 中对应 */
PUBLIC	struct task	task_table[NR_TASKS] = {
	{task_tty,      STACK_SIZE_TTY,   "TTY"       },
	{task_sys,      STACK_SIZE_SYS,   "SYS"       },
	{task_hd,       STACK_SIZE_HD,    "HD"        },
	{task_fs,       STACK_SIZE_FS,    "FS"        },
	{task_mm,       STACK_SIZE_MM,    "MM"        }};

PUBLIC	struct task	user_proc_table[NR_NATIVE_PROCS] = {
	/* entry    stack size     proc name */
	/* -----    ----------     --------- */
	{Init,   STACK_SIZE_INIT,  "INIT" },
	{TestA,  STACK_SIZE_TESTA, "TestA"},
	{TestB,  STACK_SIZE_TESTB, "TestB"},
	{TestC,  STACK_SIZE_TESTC, "TestC"}};


PUBLIC	char		task_stack[STACK_SIZE_TOTAL];

PUBLIC	TTY		tty_table[NR_CONSOLES];
PUBLIC	CONSOLE		console_table[NR_CONSOLES];

PUBLIC	irq_handler	irq_table[NR_IRQ];

PUBLIC	system_call	sys_call_table[NR_SYS_CALL] = {sys_printx,sys_sendrec};

struct dev_drv_map dd_map[] = {

	{INVALID_DRIVER},	
	{INVALID_DRIVER},	
	{INVALID_DRIVER},
	{TASK_HD},
	{TASK_TTY},	
	{INVALID_DRIVER}
};

PUBLIC	u8 *		fsbuf		= (u8*)0x600000;
PUBLIC	const int	FSBUF_SIZE	= 0x100000;


PUBLIC	u8 *		mmbuf		= (u8*)0x700000;
PUBLIC	const int	MMBUF_SIZE	= 0x100000;


PUBLIC	char *		logbuf		= (char*)0x800000;
PUBLIC	const int	LOGBUF_SIZE	= 0x100000;
PUBLIC	char *		logdiskbuf	= (char*)0x900000;
PUBLIC	const int	LOGDISKBUF_SIZE	= 0x100000;

