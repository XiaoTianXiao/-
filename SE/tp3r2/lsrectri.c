#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#define MAXBUF 200
#include <ctype.h>
#include <sys/stat.h>


//Structure contenant les données sur les Répertoires
 struct Rep { 
	char chemin[MAXBUF];
	long int taille;
	
};

//Fonction pour qsort
int cmp (const void * a, const void * b)
{
	struct Rep const * p = a;
    struct Rep const * p2 = b;
    return p2->taille - p->taille;
}

//Fonction principale
struct Rep * mon_ls( char * path, struct Rep *t)
{
	//Définition d'objet de type Rep pour créer un élément
	struct Rep r;
	char chemin[MAXBUF];
	
	//Définitions de type répertoire
    struct dirent *dp;
    DIR *rep;
    //Définitions de type stat pour récupérer les informations de rép.
    struct stat *stbuf=malloc(sizeof(struct stat));
			
    //Ouverture du répertoire passé en paramètre  
    rep = opendir(path);
    if (rep == NULL)
     { 
		fprintf(stderr, "ERROR OPEN");
		exit(-1);
	}
	
	
    printf("\n");
    printf("Répertoire %s:\n", path);
      
	int  i=0;
   
	//Lecture du répertoire
    while ((dp= readdir(rep))!=NULL)
    {
		
		//Cas d'un répertoire
        if (dp->d_type == DT_DIR)
        {
		  
          if (strcmp(dp->d_name, ".")!=0 && strcmp(dp->d_name, "..")!=0)
          { 
            strcpy(chemin, path);
            strcat(chemin,"/");
            strcat(chemin, dp->d_name);
            mon_ls(chemin,t);
           }
       }
      
		//Cas d'un fichier
		else if ((dp->d_type) == DT_REG) 
		{ 
			stat(dp->d_name,stbuf);  
			printf("%lld", stbuf->st_size);       
			printf("%s/%s",path,dp->d_name);
			printf("\n");
			strcpy(r.chemin,path);
			strcat(r.chemin,"/");
			strcat(r.chemin,dp->d_name);
			r.taille=stbuf->st_size;
			t[i]=r;
	
			
		}  
         i++;
         
    }

    closedir(rep);
    printf("\n");
    return t;
    
}

//Fonction utilisée pour afficher les données après tri
int pourtri(struct Rep *t) {
	//Initialisation de valeurs
	int i=0;
	char valeur[0];
	
	for (i=0;i<MAXBUF;i++)
	 {
		 if (strcmp(t[i].chemin,valeur)==0) 
		 { 
			 exit (2);
		 }
	
		 printf("%ld \t %s \n", t[i].taille,t[i].chemin);
		 
	}
	 
	 return i;
	
}
		 
int main(int argc, char *argv[]) 
{
	if (argc==2) { 
	//Définition du tableau contenant les fichiers à triés
	struct Rep *t;
    t=malloc(MAXBUF*sizeof(struct Rep));
    t=mon_ls(argv[1],t);
    
    //Fonction qsort pour trier
	qsort(t,MAXBUF,sizeof(struct Rep),cmp);
	printf("------Fichiers Triés-------\n");
	pourtri(t);
}
    return 0;
}
