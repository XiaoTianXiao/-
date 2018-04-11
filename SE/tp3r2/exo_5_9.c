#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

void raler(const char *msg){
	perror(msg);
	exit(1);
}

char *prog;
void usage (void)
{
    fprintf (stderr, "usage: %s [fichier]\n",prog) ;
    exit (1) ;
}

int main(int argc, char *argv[]){
	struct dirent *entry;
	prog = argv[0];
	DIR *od;
	if(argc != 2){
		usage();
	} 
	if((od=opendir(argv[1])) == NULL){
		raler("OpendirError");
	}
	while ((entry=readdir(od)) != NULL){
		if(strcmp(entry->d_name,"..") != 0 && strcmp(entry->d_name,".") != 0)
		printf("%s\n",entry->d_name);
	}
	
}