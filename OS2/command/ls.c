#include "type.h"
#include "stdio.h"
#include "string.h"
int main(int argc, char* argv[])
{
	//初始化变量
	char* filename;
	char* constname[] = { "/","dev_tty0","dev_tty1","dev_tty2","cmd.tar","kernel.bin","echo","pwd","hdldr.bin","create","cat","delete","ls" };
	struct stat* buf;
	int j = 0;
	filename = argv[1];
	//获取文件信息
	if (argc==1)
	{
		printf("I-node     Size           File name\n");
		for (; j < 13; j++)
		{
			if (stat(constname[j], buf) != -1) {
				printf("%d         %d Bytes       %s\n",buf->st_ino, buf->st_size, constname[j]);
			}
		}
	}
	else if (stat(filename, buf)!=-1) {
		printf("I-node     Size           File name\n");
		printf("%d         %d Bytes       %s\n", buf->st_ino, buf->st_size, filename);
	}
	else {
		printf("file doesn't exist\n");
	}
}