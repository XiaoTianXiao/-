#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>			// pour les msg d'erreur uniquement
#include <stdnoreturn.h>		
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>		
#include <inttypes.h>
#define	CODE_ERREUR	255	


char *prog ;				// nom du programme pour les erreurs

noreturn void raler (int syserr, const char *fmt, ...)
{
    va_list ap ;

    va_start (ap, fmt) ;
    fprintf (stderr, "%s: ", prog) ;
    vfprintf (stderr, fmt, ap) ;
    fprintf (stderr, "\n") ;
    va_end (ap) ;

    if (syserr)
	perror ("") ;

    exit (CODE_ERREUR) ;
}

noreturn void usage (void)
{
    fprintf (stderr, "usage: %s [fichier ou repetoire]\n", prog) ;
    exit (1) ;
}

char * concat(char * a, char * b){
    char * dest = (char *) malloc(strlen(a) + strlen(b) + 1);
    strcpy(dest, a);
    strcat(dest, b);
    return dest;
}


int main(int argc, char *argv[])
{
	prog=argv[0];
	DIR *dp ;
    struct dirent *d ;


	if(argc != 2){
		usage();
	}
	dp=opendir(argv[1]);
	
	if (dp == NULL){
		raler (1, "cannot open directory %s", argv[1]) ;
	}

	while ((d = readdir (dp)) != NULL){
		if(strcmp (d -> d_name, ".") != 0 && strcmp(d -> d_name, "..") != 0){
			char * slash = concat(argv[1], "");
            char * nom = concat(slash, d->d_name);
            printf("%s\n",nom );
            //printf("\n1\n ");
            //pid_t pid = fork();
			//execlp("/bin/wc", "/bin/wc", "-l",nom,NULL);
			
			//if(pid > 0){wait(NULL);}
			//}
		}


















}
	return 0;

}