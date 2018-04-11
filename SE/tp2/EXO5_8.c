#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define TAILLE_BUFFER 1024

typedef struct FICHIER{
	int fd;
	char *nom;
	int nb_oct;
	unsigned char buf[TAILLE_BUFFER];
	unsigned char *p;
}FICHIER;

void raler(const char *msg){
	perror(msg);
	exit(1);
}

FICHIER *myfopen (char *file,const char *mode){
	int desc;
	switch(*mode){
		case 'r':
			desc=open(file,O_RDONLY);
			break;
		case 'w':
			desc=open(file,O_WRONLY|O_CREAT|O_TRUNC,0666);
			break;
		default:
			return NULL;
	}
	if (desc == EOF){
		raler("open_error:");
	}
	FICHIER *fnouv=malloc(sizeof(FICHIER));
	fnouv->fd=desc;
	fnouv->nom=malloc(sizeof(char)*1024);
	strcpy(fnouv->nom,file);
	fnouv->nb_oct=0;
	return fnouv;
}

int mygetchar(FICHIER * flux){
	if(flux->nb_oct == 0){
		flux->nb_oct=read(flux->fd,flux->buf,sizeof TAILLE_BUFFER);
		flux->p=flux->buf;
	}
	if(--flux->nb_oct >= 0){
		return *(flux->p++);
	} else {
		flux->nb_oct=0;
		return EOF;
	}
}
	// static unsigned char buf[BUFSIZ]; 
	// static unsigned char *bufp = buf; 
	// static ssize_t n = 0; 			
	// if(n == 0){
	// 	n = read(0,buf,sizeof buf);
	// 	bufp = buf;
	// }
	// if(--n >= 0){     
	// 	return *bufp++;
	// }
	// return EOF;


int main(int argc, char *argv[])
{
	if (argc < 3){printf("[USAGE]: ./main [fichier1] [fichier2]");}
	FICHIER *f1;
	FICHIER *f2;
	f1=myfopen(argv[1],"r");
	f2=myfopen(argv[2],"r");
	int c1,c2;
	while ((c1 = mygetchar(f1)) != EOF) {
		//printf("%c",c1);
		putchar(c1);
	}
	while ((c2 = mygetchar(f2)) != EOF) {
		//printf("%c",c2 );
		putchar(c2);
	}
	close(f1->fd);
	close(f2->fd);
	return 0;
	
}

