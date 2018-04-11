#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>

void appel_ls(char *dir，char *des){
	execlp("/bin/ls", "/bin/ls", dir,">",des,(char *)0);
}

int main(int argc, char *argv[])
{
	if (argc < 2 ){perror("Mal entrer");}
	appel_ls(argv[1],"~/dev/null");
	return 0;
}