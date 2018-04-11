#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#define TAILLE_BUF 256
typedef struct _IO_FICHIER {
	char *filename;
	int desc;
	unsigned char buf[TAILLE_BUF];
	unsigned char *p;
	int nb_octet;
} FICHIER;
FICHIER* my_fopen(char *chemin,const char *mode){
	int desc;
	switch(*mode){
		case 'r':
			desc=open(chemin,O_RDONLY);
			break;
		case 'w':
			desc=open(chemin,O_WRONLY|O_CREAT|O_TRUNC,0666);
			break;
		default:
			return NULL;
	}
	if(desc==-1)
		return NULL;
	FICHIER *f=malloc(sizeof(FICHIER));
	f->desc=desc;
	f->filename=malloc(sizeof(char)*TAILLE_BUF);
	strcpy(f->filename,chemin);
	f->nb_octet=0;
	return f;
}
int my_getc(FICHIER *fp){
	if(fp->nb_octet==0){
		fp->nb_octet=read(fp->desc,fp->buf,sizeof TAILLE_BUF);
		fp->p=fp->buf;
	}
	if (fp->nb_octet-->0){
		return *(fp->p++);
	}
	else{
		fp->nb_octet=0;
		return EOF;
	}
}
void raler(const char *msg){
	perror(msg);
	exit(1);
}
int main(){
	FICHIER *fp1;
	fp1=my_fopen("toto","r");
	if(fp1==NULL)
		raler("toto n'existe pas");
	FICHIER *fp2;
	fp2=my_fopen("titi","r");
	if (fp2==NULL)
		raler("tata n'existe pas");
	int c1,c2;
	while((c1=my_getc(fp1))!=EOF && (c2=my_getc(fp2))!=EOF)
		printf("%c%c\n",c1,c2);
	//fclose(fp1);
	//fclose(fp2);
	return 0;
}