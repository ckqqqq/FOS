#ifndef	_ORANGES_STDIO_H_
#define	_ORANGES_STDIO_H_

#include "type.h"

/*assert函数*/
#define ASSERT
#ifdef ASSERT
void assertion_failure(char *exp, char *file, char *base_file, int line);
#define assert(exp)  if (exp) ; \
        else assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)
#else
#define assert(exp)
#endif

#define	EXTERN	extern

#define	STR_DEFAULT_LEN	1024

#define	O_CREAT		1
#define	O_RDWR		2
#define	O_TRUNC		4

#define SEEK_SET	1
#define SEEK_CUR	2
#define SEEK_END	3

#define	MAX_PATH	128

struct stat {
	int st_dev;	//文件设备号		
	int st_ino;	//节点
	int st_mode;	//文件的类型和存储的权限
	int st_rdev;	//若此文件为设备文件，则为其设备编号
	int st_size;	//文件字节数
};

struct time {
	u32 year;
	u32 month;
	u32 day;
	u32 hour;
	u32 minute;
	u32 second;
};

#define  BCD_TO_DEC(x)      ( (x >> 4) * 10 + (x & 0x0f) )

/* printf.c */
PUBLIC  int     printf(const char *fmt, ...);
PUBLIC  int     printl(const char *fmt, ...);

/* vsprintf.c */
PUBLIC  int     vsprintf(char *buf, const char *fmt, va_list args);
PUBLIC	int	sprintf(char *buf, const char *fmt, ...);

/*--------*/
/* 库函数 */
/*--------*/

#ifdef ENABLE_DISK_LOG
#define SYSLOG syslog
#endif

/* lib/open.c */
PUBLIC	int	open(const char *pathname, int flags);

/* lib/close.c */
PUBLIC	int	close(int fd);

/* lib/read.c */
PUBLIC int	read(int fd, void *buf, int count);

/* lib/write.c */
PUBLIC int	write(int fd, const void *buf, int count);

/* lib/lseek.c */
PUBLIC	int	lseek(int fd, int offset, int whence);

/* lib/unlink.c */
PUBLIC	int	unlink(const char *pathname);

/* lib/getpid.c */
PUBLIC int	getpid();

/* lib/fork.c */
PUBLIC int	fork();

/* lib/exit.c */
PUBLIC void	exit(int status);

/* lib/wait.c */
PUBLIC int	wait(int * status);

/* lib/exec.c */
PUBLIC int	exec(const char * path);
PUBLIC int	execl(const char * path, const char *arg, ...);
PUBLIC int	execv(const char * path, char * argv[]);

/* lib/stat.c */
PUBLIC int	stat(const char *path, struct stat *buf);

/* lib/syslog.c */
PUBLIC	int	syslog(const char *fmt, ...);


#endif /* _ORANGES_STDIO_H_ */

