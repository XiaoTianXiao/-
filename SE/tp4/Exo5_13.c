#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdnoreturn.h>        // C norme 2011

// define à choisir (main/pas main)
#define DEBUG
//#undef DEBUG

#define INCREMENT_REALLOC   10

char *prog ;                // nom du programme pour les erreurs
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

/** fonction qui sépare la chaine de caractères 'chaine'
 *  avec tous les séparateurs présents dans 'separateurs'
 *  renvoie un tableau NULL-terminated de mots
 *  utilise la mémoire de chaine pour cela (modifie la chaine) !
 */

char **separe (char *chaine, const char *separateurs)
{
    char **tab ;
    int i, s, m, size = INCREMENT_REALLOC ;

    tab = malloc (size * sizeof (char *)) ;
    if (tab == NULL)
        raler (1, "cannot malloc %d entries", size) ;
    m = 0 ;
    i = 0 ;
    while (chaine [i] != 0)
    {
        // saute un séparateur
        for (s = 0 ; separateurs [s] != '\0' ; s++)
            if (chaine [i] == separateurs [s])
                break ;
        if (separateurs [s] != '\0')
        {
        // met une fin de chaine à la place du séparateur et avance
            chaine [i++] = '\0' ;
            continue ;            // les séparateurs n'ont pas été épuisés
        }

        if (chaine [i] != 0)
            tab [m++] = chaine + i ;
        if (m == size)
        {
            // si j'atteins la limite de mon tableau, je l'agrandis.
            size += INCREMENT_REALLOC ;
            tab = realloc (tab, size * sizeof (char *)) ;
        if (tab == NULL)
        raler (1, "cannot realloc %d entries", size) ;
        }
        // saute les caractères non séparateurs
        for ( ; chaine [i] != '\0' ; i++)
        {
            for (s = 0 ; separateurs [s] != '\0' ; s++)
                if (chaine [i] == separateurs [s])
                    break ;
            if (separateurs [s] != '\0')
                break ; // trouvé un caractère séparateur : passer au suivant
        }
    }
    tab [m] = NULL ;
    return tab ;
}

#ifdef DEBUG

/*char cherche(char *tab, char comande){                 
    DIR *dir;                                           mauvais methode pour chercher le comande dans chaque repétoire,c mieux de chercher /bin/ls
    struct dirent *ptr;                                 est éxecutable ou /usr/bin/ls est executable etc            
    if ((dir = opendir(tab)) == NULL){
        raler(1,"cannot open the directory");
    }
    while((ptr = readdir(dir)) != NULL){}
}
*/

void witch(char *tab, char *commande){
    strcat(tab,"/");
    strcat(tab,commande);
    if((access(tab,X_OK)) == 0)
        printf("%s\n",tab );
}
int main (int argc, char *argv[])
{
    prog = argv[0];
    if(argc != 2){
        raler(0,"[Usage] %s comande",prog);
    }
    int i ;
    char *ge_path, *path, **tab ;

    // récupère la valeur de $PATH
    ge_path = getenv ("PATH") ;
    if (ge_path == NULL)
    raler (0, "cannot find PATH variable") ;
    // crée une copie locale modifiable
    // (on ne doit pas modifier la zone mémoire renvoyée par getenv)
    path = malloc (strlen (ge_path) + 1) ;
    if (path == NULL)
    raler (1, "cannot malloc string for %s", ge_path) ;

    strcpy (path, ge_path) ;

    tab = separe (path, ":") ;

    for (i = 0 ; tab [i] != NULL ; i++)
    {
        witch(tab[i],argv[1]);
        //printf ("%s\n", tab [i]) ;
    }

    free (tab) ;
    free (path) ;

    exit (0) ;
}
#endif