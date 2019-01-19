#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>



#define set_flag 355

int main(int argc, char **argv)
{
	int flag = atoi(argv[1]);
	int pid = atoi(argv[2]);
	int setter = syscall(set_flag, pid,flag);
	if(setter !=0)
	{
			printf("error code : %s ",strerror(setter));
	}

	printf("Please give letter to kill given pid !");
	getchar();
	kill(pid,9);
	return 0;

}
