#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>

long int affichetemps (int fois){
	struct timeval time;
	gettimeofday(&time,NULL);
	printf("***********c'est %d fois***********\n",fois );
	printf("tv_usec : %d , tv_sec : %ld \n", time.tv_usec,time.tv_sec);
	printf("%jd,%jd\n",(intmax_t)time.tv_usec,(intmax_t)time.tv_sec );
	long int temps=time.tv_sec+time.tv_usec;
	return temps;
}

void appel_ls(){
	execlp("/bin/ls", "/bin/ls", "-l",(char *)0);
}
char *prog;
int main(int argc, char *argv[])
{
	//clock_t starttime, endtime;
	//double totaltime;
	long int start,end,totalTime;
	prog=argv[0];
	if(argc < 1){perror("mal entrer");}
	start=affichetemps(1);

	pid_t pid;
	pid=fork();
	if(pid < 0){perror("ForkError");}
	if(pid > 0){wait(NULL);}
	else {appel_ls();}
	sleep(3);
	end=affichetemps(2);
	totalTime=end - start;
    
	printf("time end: %jd ,  time start: %jd\n", (intmax_t)end ,(intmax_t)start);
	
	printf("time diff: %jd\n",totalTime);
	
	execlp("/bin/ls", "/bin/ls", argv[1],(char *)0);
	return 0;
}