#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define set_flag 355

int main(int argc, char ** argv)
{
	int first = fork();
	if(first == 0)
	{
		printf("Child = %d, parent = %d\n", getpid(),getppid());
		int second = fork();
		if(second == 0)
		{
			printf("Child %d , parent %d\n", getpid(),getppid());
			while(1);
		}
		else
		{
			int retval = nice(11);
			int third = fork();
			if(third == 0)
			{
				printf("Child %d , parent %d\n", getpid(),getppid());
				while(1);
			}
			else
			{
				int fifth = fork();
				if(fifth == 0)
				{
					printf("Child %d , parent %d\n", getpid(),getppid());
					while(1);
				}
				while(1);
			}
			while(1);
		}		
	}
	else
	{
		printf("I am the main process  %d\n",getpid() );
		while(1);
	}
	sleep(1);
	return 0;
}
