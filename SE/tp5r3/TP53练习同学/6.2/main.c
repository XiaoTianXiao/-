#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void fils()
{
	//malloc... SIGSEV
	exit(0);
}


int main(int argc, char ** argv)
{
	int n = atoi(argv[1]);

	int i;
	for (i = 0; i < n; i++)
	{
		switch(fork())
		{
			case -1:
				perror("");
				exit(EXIT_FAILURE);

			case 0:
				printf("start\n");
				fils();
				break;
		}
	}

	for (i = 0; i < n; i++)
	{
		int status;
		
		pid_t pid;
		if((pid = wait(&status)) == -1)
		{
			perror("");
			exit(1); 
		}

		if (WIFEXITED(status))
		{
			printf("terminé %d avec status %d\n", 
			pid,
			WEXITSTATUS(status));
		}
		else
		{
			printf("ps %d a rencontré des erreurs\n",
			pid);
		}
	}

	
	
	return 0;
}
