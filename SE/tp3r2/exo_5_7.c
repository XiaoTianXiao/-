#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

void raler(const char *msg){
	perror(msg);
	exit(1);
}

void usage ()
{
    fprintf (stderr, "usage:[-l][fichier]\n") ;
    exit (1) ;
}


void affichetype(char *file) {
	struct stat info;
	if (stat(file , &info) == -1){
		raler("StatReadError");
	} 
		//printf("The file %s has premission :%d \n",file,info.st_mode)
	int type=info.st_mode &S_IFMT;
	switch(type){
		case S_IFREG:
			printf("the fichier %s is a ordinary file\n",file );
			break;
		case S_IFDIR:
			printf("the fichier %s is a directory\n",file );
			break;
		case S_IFBLK:
			printf("the fichier %s is peripheral file (mode block)\n",file );
			break;
		case S_IFCHR:
			printf("the fichier %s is peripheral file (mode character)\n",file );
			break;	
		case S_IFIFO:
			printf("the fichier %s is tube or named tube\n",file );
			break;
		case S_IFLNK:
			printf("the fichier %s is symbolic link\n",file );
			break;	
		case S_IFSOCK:
			printf("the fichier %s is socket\n",file );
			break;
		default:
			printf("problem\n");

		}
	}

void affichedrote(char *file){
	char droit[9+1];//on a besoin de +1 pour le caractere terminal qui est \0.
	int masque=0400;
	int i=0;
	strcpy(droit,"rwxrwxrwx");
	struct stat info;
	if (stat(file , &info) == -1){
		raler("StatReadError");
	} 
	while( masque != 0){
		if((info.st_mode & masque) == 0){
			droit[i] = '-';
		}
		i++;
		masque >>= 1;
	}
	printf("%s\n",droit);
}

int main(int argc, char *argv[])
{
	int opt;
	int optl=0;

	if (argc != 2)
	{
		usage();
	}
	affichetype(argv[1]);
	affichedrote(argv[1]);

	return 0;
}