#ifndef	_ORANGES_FS_H_
#define	_ORANGES_FS_H_

struct dev_drv_map {
	int driver_nr; 
};

#define	MAGIC_V1	0x111

struct super_block {
	u32	magic;		  /*魔法数（什么东西不懂）*/
	u32	nr_inodes;	  /*保存inodes数量*/
	u32	nr_sects;	  /*扇区数量*/
	u32	nr_imap_sects;	  /*inode-map扇区数量*/
	u32	nr_smap_sects;	  /*sector-map数量*/
	u32	n_1st_sect;	  
	u32	nr_inode_sects;   /*inode扇区数量*/
	u32	root_inode;
	u32	inode_size;       /**< INODE大小*/
	u32	inode_isize_off; 
	u32	inode_start_off; 
	u32	dir_ent_size;   
	u32	dir_ent_inode_off;
	u32	dir_ent_fname_off;

	int	sb_dev; 
};

#define	SUPER_BLOCK_SIZE	56

struct inode {
	u32	i_mode;		/*存储模式*/
	u32	i_size;		/*文件大小*/
	u32	i_start_sect;	/*第一个扇区*/
	u32	i_nr_sects;
	u8	_unused[16];

	int	i_dev;
	int	i_cnt;		/*当前共享inode的进程的数量  */
	int	i_num;	
};

#define	INODE_SIZE	32

#define	MAX_FILENAME_LEN	12

struct dir_entry {
	int	inode_nr;		
	char	name[MAX_FILENAME_LEN];	
};

#define	DIR_ENTRY_SIZE	sizeof(struct dir_entry)
struct file_desc {
	int		fd_mode;	
	int		fd_pos;		
	int		fd_cnt;		
	struct inode*	fd_inode;	
};

#define RD_SECT(dev,sect_nr) rw_sector(DEV_READ, \
				       dev,				\
				       (sect_nr) * SECTOR_SIZE,		\
				       SECTOR_SIZE,\
				       TASK_FS,				\
				       fsbuf);
#define WR_SECT(dev,sect_nr) rw_sector(DEV_WRITE, \
				       dev,				\
				       (sect_nr) * SECTOR_SIZE,		\
				       SECTOR_SIZE,\
				       TASK_FS,				\
				       fsbuf);

	
#endif /* _ORANGES_FS_H_ */
