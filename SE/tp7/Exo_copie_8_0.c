/*
	essayez de programmer un fontion void copier(int fdsrc, int fddst)
	copier l'entree standare -> sortie standare
	TESTER:
		a.out < /usr/bin/doxygen > toto
		cmp /usr/bin/doxygen toto
	
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX 8092

void raler(const char *msg){
	perror(msg);
	exit(1);
}


void copier(int fdsrc, int fddst){


	int nblus;
	char bloc[MAX];

	while ((nblus = read(fdsrc,bloc,sizeof bloc)) > 0){
		if (write(fddst,bloc,nblus) == -1){
			raler("writefichier2");
		}
	}

	if(nblus == -1){raler("readfichier1");}

	if(close(fdsrc) == -1){raler("closefichier1");}

	if(close(fddst) == -1){raler("closefichier2");}
	
}



















int main(){


	copier(0,1);






















// int fd1, fd2;

// fd1=open(argv[1],O_RDONLY);

// if(fd1 == -1)
// {
// 	raler("openficher1");
// }

// fd2=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0666);
// if(fd2 == -1){
// 	raler("openfichier2");
// }

return 0;
}