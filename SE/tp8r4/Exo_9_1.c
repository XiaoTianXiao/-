#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>			// pour les msg d'erreur uniquement
#include <stdnoreturn.h>		// C norme 2011
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>

#define	CODE_ERREUR	255

/******************************************************************************
 * Gestion des erreurs
 */

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
    fprintf (stderr, "usage: %s [-d|-v] [fichier-spec]\n", prog) ;
    exit (1) ;
}






/***************************************************************************
*/

#include <signal.h>
int comp=1;
void catch(int sig){
    (void) sig;
    printf("trouver un signal SIGINT\n");
    printf("%d\n",comp );
    comp++;
}


int main(int argc, char *argv[])
{
    (void) *argv;   
    if(argc > 1){usage();}

    if(signal(SIGINT,catch) == SIG_ERR){raler(1,"problem with signal");}
    


    while(1)
    {
        if(comp > 5){
            exit(0);
        }
        pause();
    }
        
    exit(0);  
    
    
    

}