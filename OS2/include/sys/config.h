/**
 * 保留一些扇区供我们复制tar文件
 *保存在那里，供操作系统提取并使用。
 */
#define	INSTALL_START_SECT		0x17000
#define	INSTALL_NR_SECTS		0x800

/**
 *引导参数由加载程序存储，它们应该是
 *内核正在运行时不应该被覆盖
 *由于内核可能随时使用它们。
 */
#define	BOOT_PARAM_ADDR	0x900  /*物理地址*/
#define	BOOT_PARAM_MAGIC 0xB007 /*魔法数*/
#define	BI_MAG 0
#define	BI_MEM_SIZE	1
#define	BI_KERNEL_FILE 2

#define	MINOR_BOOT			MINOR_hd2c

/*
 * 磁盘日志
 */
#define ENABLE_DISK_LOG
#define SET_LOG_SECT_SMAP_AT_STARTUP
#define MEMSET_LOG_SECTS
#define	NR_SECTS_FOR_LOG		NR_DEFAULT_FILE_SECTS
