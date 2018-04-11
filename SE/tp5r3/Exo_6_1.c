#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	pid_t aff;
	int status;
	printf("*******************************************************\nBefore use fork, first call processus pid : %d\n*******************************************************\n",getpid() );	
	
	switch (pid = fork()){
		case -1:
				perror("ForkError");
				exit(1);
				//break;
		case  0:
				printf("i'm child and my Pid is : %d \nand my parent's pid : %d\n" , getpid() , getppid() );
				//sleep(3);
				exit(getpid() % 10);
				//break;
		default:
				printf("i'm waitting my child to be finished, my child's pid is %d\n", pid );
				aff=wait(&status);
				if (aff == -1 ){
					perror("WaitError");
					exit(1);
				} else{
					printf("code de retour de comande WAIT is :%d\n", aff);
					printf("%d\n",WEXITSTATUS(status));
				}

	}
	//int code = getpid() % 10;
	//printf("%d\n",code);
}	