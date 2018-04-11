#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX 1024

void raler(const char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]){

if(argc != 3){
	raler("USAGE: ./coreection5_3.c fichier1 ficher2");
}

int fd1, fd2;

char bloc[MAX];

ssize_t nblus;

fd1=open(argv[1],O_RDONLY);

if(fd1 == -1)
{
	raler("openficher1");
}

fd2=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0666);
if(fd2 == -1){
	raler("openfichier2");
}

while ((nblus = read(fd1,bloc,sizeof bloc)) > 0)
{
	if (write(fd2,bloc,nblus) == -1){
		raler("writefichier2");
	}
}

if(nblus == -1)
{
	raler("readfichier1");
}

if(close(fd1) == -1)
{
	raler("closefichier1");
}

if(close(fd2) == -1)
{
	raler("closefichier2");
}

exit(0);
}