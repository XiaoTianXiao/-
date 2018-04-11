/**
 * author xiaotian xiao
 * date   20/20/2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#define Taille_Char 512
#define Taille_File 1024

typedef struct file
{
	char *nom;
	unsigned long int oct;
}file;

void raler(const char *msg){1
	perror(msg);
	exit(1);
}

char *prog;
void usage (void)
{
    fprintf (stderr, "usage: %s [Directory]\n",prog);
    exit (1);
}

unsigned long int c=0;
void Recursive(file *rep,file *order )
{
    struct stat buf;
    DIR *dir;
    struct dirent *ptr;
    char temp[Taille_Char];
    if ((dir = opendir( rep->nom )) == NULL){
    	raler("opdenDir Error\n");
    	}	
    while( ( ptr = readdir( dir )) != NULL ) {
    	strcpy(temp,rep->nom);
    	strcat(temp,"/");
    	strcat(temp,ptr->d_name);
    	file *temprep=malloc(sizeof(file));
    	temprep->nom=temp;
    	file aaa;
    	if(stat(temp,&buf) == -1)
    		raler(ptr->d_name);
    	int type=buf.st_mode &S_IFMT;
    	switch(type){
    		case S_IFDIR:
    			if(strcmp(ptr->d_name,"..")!=0 && strcmp(ptr->d_name,".")!=0)
				Recursive(temprep,order);  	  		
    	  		break;
    	  	case S_IFREG:
    	  		aaa.nom=malloc(128*sizeof(char));
    	  		aaa.oct=buf.st_size;
    	  		strcpy(aaa.nom,temp);
    	  		order[c]=aaa;
    	  		c++;
				break;
			default:
				printf("probleme\n");				
 		} 
    }
}    

void affiche(file *afiche){
	unsigned long int i;
	for(i=0;i<c;i++){
	printf("%ld \t %s\n",afiche[i].oct,afiche[i].nom);
	}
	free(afiche);
}

static int cmp(const void *a, const void *b ){
	const file *c1 = a;
	const file *c2 = b;
	return c1->oct - c2->oct;
}

int main(int argc, char *argv[])
{
	file *d=malloc(sizeof(file));
	d->nom=malloc(Taille_Char*sizeof(char));
	d->nom=argv[1];
	file *tab=malloc(Taille_File*sizeof(file));


	prog=argv[0];
	if (argc != 2){
		usage();
	}
	Recursive(d,tab);
	qsort(tab,c,sizeof(tab[0]),cmp);
	affiche(tab);
	
}