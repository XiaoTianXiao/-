#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc !=3)
	{
		printf("usage : ./copy filename1 filename2\n");
	}
	
	char buffer[1024] = {0};
	
	int fich1 = open(argv[1], O_RDONLY);
	
	if (fich1 == -1)
	{
		perror("openfich1");
		return 1;
	}

	int fich2 = open(argv[2], O_RDONLY|O_CREAT|O_TRUNC,0666);

	if(fich2 == -1)
	{
		perror("openfich2");
		return 2;
	}

	int count = 0;

	while (count = read(fd1, buffer, 1024)){
		if (count == -1){
			perror("readfich1");
			close(fich1);
			close(fich2);
			return 3;
		}

		int count2 = write (fich2, buffer, count);

		if (count == -1){
			perror("writefich1")
		}
	}

}

///////////////////////////////////////////////////////////

#define MAX 1024

void raler(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(){
int fd1, fd2;
char bloc[MAX];
ssize_t nblus;
fd1=open("toto",O_RDONLY);
if(fd1 == -1)
{
	raler("open toto");
}

fd2=open("titi",O_RDONLY|O_CREAT|O_TRUNC,0666);
if(fd2 == -1){

}

while ((nblus = read(fd1,bloc,sizeof bloc)) > 0)
{
	if (write(fd2,bloc,nblus) == -1){
		raler("writetiti");
	}
}

if(nblus == -1)
{
	raler("read toto");
}

if(close(fd1) == -1)
{
	raler("close toto");
}

if(close(fd2) == -1)
{
	raler("close titi");
}

exit(0);
}