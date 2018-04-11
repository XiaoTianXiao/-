#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>


void fils()
{
	pid_t mon_pid = getpid();

	printf("\tJe suis le processus fils,\n\tMon pid est %d et le pid de mon pere est %d\n",
		mon_pid, getppid());

	exit(mon_pid % 10);
}


int main()
{
	pid_t pid_fils;
	switch(pid_fils = fork())
	{
		case -1:
			perror("");
			exit(1);
			break;

		case 0:
			fils();
			break;

		default:
			printf("Je suis le père, le pid de mon fils est %d\n",
				pid_fils);
			printf("J'attends que mon fils fini ...\n");
			int status;
			wait(&status);

			if (WIFEXITED(status))
			{
				printf("code retour du fils: %d\n", 
					WEXITSTATUS(status));
			}
			else
			{
				perror("");
				exit(1);
			}
	}

	return 0;
}
