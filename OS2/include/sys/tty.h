#ifndef _ORANGES_TTY_H_
#define _ORANGES_TTY_H_


#define TTY_IN_BYTES		256
#define TTY_OUT_BUF_LEN		2	

struct s_tty;
struct s_console;

/* TTY */
typedef struct s_tty
{
	u32	ibuf[TTY_IN_BYTES];	
	u32*	ibuf_head;		
	u32*	ibuf_tail;		
	int	ibuf_cnt;

	int	tty_caller;
	int	tty_procnr;
	void*	tty_req_buf;
	int	tty_left_cnt;
	int	tty_trans_cnt;

	struct s_console *	console;
}TTY;

#endif /* _ORANGES_TTY_H_ */
