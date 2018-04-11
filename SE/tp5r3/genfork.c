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
#define size 1024

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

/******************************************************************************
 * Transformation de la spécification en structure d'arbre
 * => utiliser la fonction analyser_spec()
 */

struct sommet
{
    int val ;					// étiquette du sommet
    struct sommet *frere_suivant ;		// liste des frères
    struct sommet *premier_fils ;		// premier des fils
    /*
     * Ce dernier champ (dernier_fils) n'est utilisé que pendant la
     * construction de l'arbre, pour insérer en queue et ainsi
     * respecter l'ordre des processus dans la spécification.
     */
    struct sommet *dernier_fils ;		// pour insertion en queue
} ;

/*
 * Fonction interne pour l'analyse de la spécification : analyse une
 * ligne de la spécification (la ligne pointée par *spec), et
 * retourne la profondeur, la valeur, et la position suivante (via
 * *spec) et un code pour dire si on a rencontré une erreur, une fin
 * de spéc ou si tout baigne.
 */

enum ret_al { AL_ERR, AL_FIN, AL_OK } ;		// retour de analyser_ligne

enum ret_al analyser_ligne (char **spec, int *prof, int *val)
{
    *prof = *val = 0 ;				// mettre les compteurs à 0

    if (**spec == '\0')				// spec terminée
	return AL_FIN ;

    /*
     * Compter les espaces pour avoir la profondeur
     */

    while (**spec == ' ')
    {
	(*spec)++ ;
	(*prof)++ ;
    }

    /*
     * Récupérer la valeur
     */

    char *debut = *spec ;			// vérifier au moins un chiffre
    while (isdigit (**spec))
    {
	*val = *val * 10 + (**spec - '0') ;
	(*spec)++ ;
    }
    if (debut == *spec)				// on n'a pas avancé => pb
	return AL_ERR ;

    /*
     * Vérifier qu'on est bien à la fin d'une ligne
     */

    if (**spec != '\n')
	return AL_ERR ;
    (*spec)++ ;					// passer au sommet suivant

    return AL_OK ;
}

/*
 * Fonction interne pour l'analyse de la spécification : crée
 * un sommet isolé avec l'étiquette indiquée.
 */

struct sommet *creer_sommet (int val)
{
    struct sommet *s ;

    s = malloc (sizeof *s) ;
    if (s == NULL)
	raler (1, "cannot malloc %d bytes", sizeof *s) ;

    s->frere_suivant = NULL ;
    s->premier_fils = NULL ;
    s->dernier_fils = NULL ;
    s->val = val ;
    return s ;
}

/*
 * Fonction interne pour l'analyse de la spécification : analyse la
 * spécification à partir de la profondeur donnée.
 * Pré-conditions :
 *   - numl pointe sur le num de la ligne courante de la spéc, pour les msg err
 *   - spec pointe sur la spéc de la racine du sous-arbre à étudier
 *	qui doit exister et avoir la profondeur attendue
 *   - prof donne la profondeur attendue pour cette racine
 * Post-conditions :
 *   - valeur de retour : racine du sous-arbre étudié
 *   - spec pointe sur la première ligne suivant le sous-arbre
 *   - numl pointe sur le numéro de de la première ligne suivant le sous-arbre
 */

struct sommet *analyser_spec_profondeur (int *numl, char **spec, int prof)
{
    char *nspec ;
    int nprof, val ;
    struct sommet *s ;
    enum ret_al r ;

    /*
     * Analyser la racine du sous-arbre
     */

    nspec = *spec ;				// ne pas avancer si pas ok
    r = analyser_ligne (&nspec, &nprof, &val) ;	// lire la ligne numl
    switch (r)
    {
	case AL_ERR :
	    raler (0, "cannot parse spec at line %d", *numl) ;
	case AL_FIN :
	    return NULL ;
	case AL_OK :				// on continue en séquence
	    break ;
	default :				// programmation défensive
	    raler (0, "internal error at line %d", *numl) ;
    }
    if (nprof != prof)
	raler (0, "unexpected depth (%d) at line %d", nprof, *numl) ;

    s = creer_sommet (val) ;
    *spec = nspec ;
    (*numl)++ ;

    /*
     * Chercher les fils directs de la racine
     */

    while ((r = analyser_ligne (&nspec, &nprof, &val)) == AL_OK && nprof > prof)
    {
	struct sommet *f ;			// sous-arbre fils trouvé

	nspec = *spec ;
	f = analyser_spec_profondeur (numl, &nspec, prof + 1) ;

	// Est-ce le premier fils ou bien y-a-t'il déjà au moins un ?
	if (s->dernier_fils == NULL)
	{
	    s->premier_fils = s->dernier_fils = f ;
	}
	else
	{
	    s->dernier_fils->frere_suivant = f ;
	    s->dernier_fils = f ;
	}
	*spec = nspec ;
    }

    if (r == AL_ERR)
	raler (0, "cannot parse spec at line %d", *numl) ;
    // rien à faire si r == AL_FIN

    return s ;
}

/*
 * Fonction à utiliser : analyse une spécification donnée sous forme
 * d'une chaîne de caractères contenant le texte complet de la
 * spécification (cf énoncé). Ce texte est une succession de lignes
 * terminée par '\0' pour la fin de chaîne. Par exemple :
 * spec = "200\n 500\n  200\n   300\n  600\n 100\n  0\n\0"
 */

struct sommet *analyser_spec (char *spec)
{
    struct sommet *s ;				// arbre obtenu
    int numl ;					// numéro de ligne pour msg err

    numl = 1 ;
    s = analyser_spec_profondeur (&numl, &spec, 0) ;
    if (*spec != '\0')				// est-on bien à la fin ?
	raler (0, "invalid spec at line %d", numl) ;
    return s ;
}

/******************************************************************************
 * Ajoutez vos fonctions à la suite
 */

void pro(struct sommet *a){
    pid_t pid;
    
    int status;
    switch(pid=fork()){
        case -1:
                perror("cannot fork fils");
                exit(1);
        case  0:
                pid=getpid();
                printf("pid %d -> %d\n", getppid(),pid);
                //printf("pid %d : aatente %d ms\n",ppid,a->val );
                if(a->premier_fils != NULL ){
                    pro(a->premier_fils);
                }
                exit(0);
}
                usleep(a->val);
                // printf("pid %d : attente %d ms\n", getpid(),a->val);
                if((pid = wait(&status)) == -1){
                    perror("WaitError");
                    exit(1);
                }
                printf("pid %d : attente %d ms\n", getpid(),a->val);
                if(a->frere_suivant){
                    pro(a->frere_suivant);
                }
                exit(0);
                //break;

    
}




int main(int argc, char *argv[])
{
    int c=0;
    int vflag=0;
    int dflag=0;
    
    int fd=-1;
    int r;
    char buf[size];
    while((c=getopt(argc,argv,"vd")) != -1){
        switch(c){
            case 'v':
                vflag++;
                break;
            case 'd':
                dflag++;
                break;
            default:
                 exit(0);

        }
    }
   if((fd=open(argv[2],O_RDONLY)) == -1){
            perror("OpenError");
        }

        if ((r=read(fd,buf,size)) == -1){
            perror("ReadError");
        }

    if (dflag){
     
        printf("%s",buf );;
}

    if(vflag){
        struct sommet *a;
        a=analyser_spec(buf);
        while(a->premier_fils != NULL){
            pro(a);
        }
    }




    return 0;
}