/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>

/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int main(int argc, char *argv[])
{
	pid_t child;
	int val = 0;
	int count = 0;
	struct user_regs_struct regs;

	//示例1：类似
	/* child = fork(); //创建一个子进程
	if (child == 0) //子进程
	{
		ptrace(PTRACE_TRACEME, 0, NULL, NULL); //表示当前进程进入被追踪状态
		execl("/bin/ls", "ls", NULL);
	}
	else //父进程
	{
		wait(NULL); //等待子进程发送一个SIGCHILD信号
		ptrace(PTRACE_GETREGS, child, NULL, regs);
		printf("Register: rdi[%ld], rsi[%ld], rdx[%ld], rax[%ld], orig_rax[%ld]\n",
                regs.rdi, regs.rsi, regs.rdx,regs.rax, regs.orig_rax); // 打印寄存器的值
		ptrace(PTRACE_CONT, child, NULL, NULL); //继续运行子进程
		sleep(1);
	} */

	//示例2
	child = fork();
	if (child == 0) //子进程
	{
		execl("/home/dong/Project/Practice/test/ptrace_test/bin/test", "test", NULL);
	}
	else //父进程
	{
		ptrace(PTRACE_ATTACH, child, NULL, NULL); //建立与子进程的跟踪关系
		while(1)
		{
			wait(&val);
			if (WIFEXITED(val))
				break;
			count++;
			ptrace(PTRACE_SINGLESTEP, child, NULL, NULL); //单步跟踪，子进程每执行一条指令就触发一个SIGTRAP信号
		}
		printf("Total Instraction number: %d\n", count);
	}

	return 0;
}
