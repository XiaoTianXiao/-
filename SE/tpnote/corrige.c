#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>			// pour les msg d'erreur uniquement
#include <stdnoreturn.h>		// C norme 2011
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <limits.h>			// pour PATH_MAX
#include <inttypes.h>			// pour intmax_t


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

    exit (1) ;
}

noreturn void usage (void)
{
    fprintf (stderr, "usage: %s répertoire\n", prog) ;
    exit (1) ;
}


/******************************************************************************
 * Parcours du répertoire et exécution de wc
 */

void executer (const char *fichier)
{
    int fd ;

    switch (fork ())
    {
	case -1 :
	    raler (1, "cannot fork") ;

	case 0 :
	    /*
	     * Le sujet demande explicitement de faire une
	     * redirection de l'entrée de wc. Celle-ci doit
	     * être bien sûr réalisée dans le fils.
	     */
	    fd = open (fichier, O_RDONLY) ;
	    if (fd == -1)
		raler (1, "cannot open %s", fichier) ;
	    if (dup2 (fd, 0) == -1)
		raler (1, "cannot dup %d (file %s)", fd, fichier) ;
	    if (close (fd) == -1)
		raler (1, "cannot close %d (file %s)", fd, fichier) ;

	    // execlp est assimilé à une primitive pour cette UE
	    execlp ("wc", "wc", "-l", NULL) ;
	    raler (1, "cannot exec wc for %s", fichier) ;

	default :
	    /*
	     * Il ne faut surtout pas faire wait ici car sinon les
	     * fils ne seraient pas exécutés en parallèle (comme demandé
	     * dans l'énoncé
	     */
	    break ;
    }
}

void parcours (const char *chemin)
{
    DIR *dp ;
    struct dirent *d ;
    struct stat stbuf ;
    int i, nfils ;

    dp = opendir (chemin) ;
    if (dp == NULL)
	raler (1, "cannot open directory %s", chemin) ;

    nfils = 0 ;
    while ((d = readdir (dp)) != NULL)
    {
	/*
	 * Pas besoin de traitement particulier pour . et .. puisqu'on
	 * ne fait pas d'exploration récursive et qu'on ignore les
	 * répertoires.
	 */

	char nc [PATH_MAX] ;	// nouveau chemin construit
	int taille ;		// taille (idéale) du nouveau chemin

	/*
	 * Il est très important de vérifier qu'on ne déborde
	 * pas du tableau déclaré. Snprintf renvoie le nombre
	 * de caractères qui aurait été placés dans nc, s'il
	 * y avait eu suffisamment de place (sans compter le
	 * '\0'). Il suffit de vérifier que cette taille n'est
	 * pas supérieure à la taille de la variable.
	 * L'utilisation de snprintf n'est pas forcément la
	 * méthode la plus efficace, mais elle est rigoureuse
	 * tout en conservant une lisibilité correcte.
	 */

	taille = snprintf (nc, sizeof nc, "%s/%s", chemin, d->d_name) ;
	if (taille < 0)
	    raler (0, "erreur de conversion snprintf pour '%s/%s'",
			    chemin, d->d_name) ;
	if ((unsigned) taille >= sizeof nc)
	    raler (0, "chemin '%s/%s' trop long (> %zd)",
			    chemin, d->d_name, PATH_MAX) ;

	/*
	 * Le sujet dit qu'on écarte les liens symboliques.
	 * Il faut donc ne pas les "suivre" de manière transparente.
	 * (l'utilisation de stat n'a toutefois pas été sanctionnée)
	 */

	if (lstat (nc, &stbuf) == -1)
	    raler (1, "cannot stat %s", nc) ;

	/*
	 * Fichier trouvé : on lance l'exécution de wc (on espère
	 * qu'on n'atteint pas le nombre max de processus par
	 * utilisateur, mais le test de fork le détectera).
	 */

	if (S_ISREG (stbuf.st_mode))
	{
	    executer (nc) ;
	    nfils++ ;		// compter le nombre de fils lancés
	}
    }

    if (closedir (dp) == -1)
	raler (1, "cannot close directory %s", chemin) ;

    /*
     * Attendre les fils, et vérifier qu'ils se sont bien terminés
     * comme demandé dans l'énoncé.
     */

    for (i = 0 ; i < nfils ; i++)
    {
	int status ;
	pid_t fils ;

	if ((fils = wait (&status)) == -1)
	    raler (1, "cannot wait child %d", i) ;
	if (! (WIFEXITED (status) && WEXITSTATUS (status) == 0))
	    raler (0, "child %d (pid=%d) exited abnormally", i, fils) ;
    }
}

/******************************************************************************
 * Main
 */

int main (int argc, char *argv [])
{
    prog = argv [0] ;
    if (argc != 2)
	usage () ;

    parcours (argv [1]) ;
    exit (0) ;				// si on arrive là, tout est ok
}