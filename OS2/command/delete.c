#include "type.h"
#include "stdio.h"
#include "string.h"
int main(int argc, char* argv[])
{
	char* filename;
	int i;
	for (i = 1; i < argc; i++)
	{
		filename = argv[i];
		if (unlink(filename) == 0) {
			printf("%s deleted\n", filename);
		}
		else {
			printf("Faild to delete\n");
		}
	}
}