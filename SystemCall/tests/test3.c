#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define set_myFlag 355
int main(int argc, char** argv)
{
	if(argc<2 || argc>3)
	{
		printf("Give exact two arguments first niceValue Second Flag\n");
		return 0;
	}

	int  niceValue=atoi(argv[1]);
	int flag=atoi(argv[2]);

	//try pid manuelly for getting ESRCH error uncomment these lines
	//int pid;
	//printf("RANDOM PID: ");
	//scanf("%d",&pid);
	//int ERR=syscall(set_myFlag,pid,flag);
	//after that comment the other error part 

	int niceRet	=nice(niceValue);      
	int ERR=syscall(set_myFlag,getpid(),flag);
	if(ERR!=0){
		printf("Error Control statement:%s\n",strerror(ERR));
		return 0;
	}
	int f=fork();

	if(f==0)
	{
		printf("CHild pid:%d ---- Parent pid:%d  niceValue:%d\n",getpid(),getppid(),niceRet);
		
	}	
	else
	{
		printf("Error Control statement:%s \n",strerror(ERR));
		printf("Fork return %s created\n",strerror(f));
		
	}
	sleep(1);
	return 0;
}
