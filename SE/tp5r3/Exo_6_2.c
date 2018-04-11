#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	if (argc != 2){
		perror("[Usage:] -option specification");
	}
	
	int fois=atoi(argv[1]);

	pid_t pid;

	int i;
	for(i=0;i<fois;i++){
		switch(fork()){
			case -1:
				perror("ForkError");
				exit(EXIT_FAILURE);

			case 0:
				printf("Commencer\n");
				sleep(1);
				exit(0);
				break;

		}
	}


	for(i=0;i<fois;i++){
		int status;

		pid_t pid;

		if((pid = wait(&status)) == -1){
			perror("WaitError");
			exit(1);
		}

		if(WIFEXITED(status)){
			printf("terminé %d avec status %d\n",pid,WEXITSTATUS(status));
		}
		else {
			printf("processus rencontre l'erruer : %d\n", pid );
			
		}
	}

}