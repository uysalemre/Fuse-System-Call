#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/errno.h>

asmlinkage long sys_set_myFlag(pid_t pid, int flag)
{
	if(capable(CAP_SYS_ADMIN)) //needs root  privalage
	{
		struct task_struct *process;
		process =  find_task_by_vpid(pid); //find given pid 
		if(process == NULL)
		{
			return ESRCH; //no such process
		}
		else
		{
			if(flag == 1 || flag == 0)
			{
				process->myFlag = flag; //set flag value into task_struct
				return 0; //return success after setting the flag
			}
			else
			{
				return EINVAL; //invalid argument error
			}
		}
	}
	else
	{
		return EPERM;//operation not permitted error
	}
}
