#include "type.h"
#include "stdio.h"
#include "string.h"
int main(int argc, char* argv[])
{
	//初始化变量
	char* filename;
	char* bufr;
	int i;
	filename= argv[1];
	//获取文件内容
	int fd;
	int n;
	fd = open(filename, O_RDWR);
	if (fd != -1)
	{
		n = read(fd, bufr, 512);
		i = strlen(bufr);
		bufr[i] = 0;
		printf("%d bytes read:%s\n", i, bufr);
	}
	else {
		printf("file doesn't exist\n");
		return 0;
	}
	//关闭
	close(fd);
}