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
char *buf;

void signal1(int sig)
{
    switch(sig)
    {
        case SIGINT:
            printf("get a signal SIGINT\n");
            break;
        case SIGQUIT:
            printf("get a signal SIGQUIT\n");
            break; 
    }
    return;
}





void signal2(int sig) {  
     
    psignal(sig,buf);   
   
}   
  
  
int main() {   
    printf("process id is %d \n ",getpid());
    printf("input your signal:\n");
    for(int i=1;i<32;i++)
    {
        signal(i, signal2); 
    }
    // signal(SIGHUP,siganl1)
    // signal(SIGINT, signal1);   
    // signal(SIGQUIT,signal1);    较笨拙的方案定义接受范畴和选项
    //   
    while(1)
    {
        pause();
    }   
}   