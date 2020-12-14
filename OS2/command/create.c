#include "type.h"
#include "stdio.h"
#include "string.h"
int main(int argc, char* argv[])
{
	//初始化变量
	char* filename;
	char* bufw;
	int i;
	//获取文件名和文件内容
	for (i = 1; i < argc; i++)
	{
		//获取文件名
		if (i == 1)
		{
			filename = argv[i];
		}
		//获取文件内容
		if (i == 2)
		{
			bufw = argv[i];
		}
	}
	//开始创建文件
	int fd;
	fd = open(filename, O_CREAT | O_RDWR);
	//assert(fd != -1);
	if (fd != -1)
	{
		int n;
		n = write(fd, bufw, strlen(bufw));
		printf("File:%s created.\n", filename);
	}
	else {
		printf("Fail to create\n");
		return 0;
	}
	//关闭
	close(fd);

}