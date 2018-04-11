#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

void execute_grep(char * motif, char * fichier);

void traite(char * motif, char * nom);

void ouvre_repertoire(char * motif, char * repertoire);

void attend(int compteur);

int compt = 0;

char * concat(char * a, char * b){
    char * dest = (char *) malloc(strlen(a) + strlen(b) + 1);
    strcpy(dest, a);
    strcat(dest, b);
    return dest;
}
void execute_grep(char * motif, char * fichier){
    execlp("wc", "wc", "-l", motif, fichier, NULL);
}

void traite(char * motif, char * nom){

    // printf("Motif : %s, nom : %s\n", motif, nom);
    pid_t pid = fork();
    if(pid == -1){
        perror("fork");
        exit(1);
    }
    if(pid == 0){
        struct stat stbuf;
        if(stat(nom, &stbuf) < 0){
            perror("stat");
            exit(1);
        }

        if(S_ISREG(stbuf.st_mode)){
            compt++;
            execute_grep(motif, nom);
        }
        else if (S_ISDIR(stbuf.st_mode)){
            ouvre_repertoire(motif, nom);
        }
    }
    else{
        attend(compt);
    }
}



void ouvre_repertoire(char * motif, char * repertoire){
    DIR * dp = NULL;
    struct dirent *d;
    char * nom = NULL;
    char * slash = NULL;
    dp = opendir(repertoire);
    if(dp == NULL){
        perror("opendir");
        exit(1);
    }
    while((d = readdir(dp)) != NULL){
        if(strcmp (d -> d_name, ".") != 0 && strcmp(d -> d_name, "..") != 0){
            char * slash = concat(repertoire, "/");
            char * nom = concat(slash, d->d_name);
            compt++;
            traite(motif, nom);
        }
    }
    free(nom);
    free(slash);
    closedir(dp);
}




void attend(int compteur){
    int i = compteur;
    while(i > 0){
        wait(NULL);
        i--;
    }
}

int main(int argc, char * argv[]){
    if(argc == 1){
        fprintf(stderr, "The program takes at least one argument.\nUsage: ./main motif <rep1 ... repn>\n");
        exit(1);
    }

    // par défaut on le fait dans le répertoire courant
    if(argc == 2){
        traite(argv[1], ".");
    }

    // Sinon on le fait pour tous les répertoires donnés
    else{
        int i;
        for(i = 2; i < argc; ++i){
            if(argv)
            traite(argv[1], argv[i]);
        }
    }
}