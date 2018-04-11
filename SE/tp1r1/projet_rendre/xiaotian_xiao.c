/**
 * author xiaotian xiao
 * date   09/02/2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#define Taille_Buffer 1024
/**
 * @author xiaotian
 * @dateTime 2018-02-09
 * @param    desc       when we cat a file, a decriptor for the getchar
 * @return   it is depend on conditions.
 */
int mygetchar(int desc){
    static unsigned char buf[Taille_Buffer]; //We can change Taille_Buffer with Taille_Buffer, anyway, here just is the size of buffer
    static unsigned char *bufp = buf; //we creat a point *bufp for pointting to the buffer
    static ssize_t n = 0;           //Here we use 3 static variable to get a stat , so they can't be return to original by each circle
    if(n == 0){
        n = read(desc,buf,sizeof buf);
        bufp = buf;
    }
    if(--n >= 0){     // It is same with if(n > 0) {n--; return .......} ;
        return (unsigned char) *bufp++;
    }
    n=0;
    return EOF;
}
/**
 * @author xiaotian
 * @dateTime 2018-02-11
 * @param    argc       number of arguments
 * @param    argv       array of arguments
 * @return   0           
 */
int main(int argc, char *argv[]) {
    int c=0;       // for getchar 
    int Eflag=0;   // for -E
    int nflag=0;   // for -n
    extern int optind; // the indice of arguments
    int desc=0;    //initialization of descripter
    int nbl=1;     // lines number
    int control=0; // control for give number 1 to first line
    

    while((c = getopt(argc,argv,"nE")) != EOF){
        //printf("Your option char: %c \n",c);
        switch(c){
            case'n':
                nflag++;
                break;
            case 'E':
                Eflag++;
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-n] [-E] file\n",argv[0]);
                exit(1);
                break;  
        }
    }
    //condition: no objetive file and no option -E neither option -n

    if(argc == optind && nflag == 0 && Eflag == 0){
        for(;;){
            if ((c = mygetchar(desc)) != EOF)putchar(c);
        }
    }
    //condition: no objetive file and no option -E but with option -n

    if(argc == optind && nflag != 0 && Eflag == 0){
        for(;;){
            if ((c = mygetchar(desc)) != EOF){
                if(control == 0){
                printf("1");
                putchar(c);
                control=2;
                } else {
                    putchar(c);
                }
                if(c == '\n'){
                    nbl++;
                    printf("%d",nbl);
                }
            }
        }
    }
    //condition: no objetive file and with option -E but no option -n

    if(argc == optind && nflag == 0 && Eflag != 0){
        for(;;){
            if ((c = mygetchar(desc)) != EOF){
                if(c == '\n'){putchar('$');}
                putchar(c);
            }
        }
    }
    //condition: no objetive file and with option -E and option -n

    if(argc == optind && nflag == 1 && Eflag == 1){
        for(;;){
            if ((c = mygetchar(desc)) != EOF){
                if(control == 0){
                printf("1");
                putchar(c);
                control=2;
                } 
                if(control == 2 && c == '\n')
                    putchar('$');
                    putchar(c);
                }
                if(c == '\n'){
                    nbl++;
                    printf("%d",nbl);
                }
            }
        }
    //condition: with objetive file and no option -E neither option -n    
    
    if(!Eflag  && !nflag ){
        for(;optind<argc;optind++){
            desc=open(argv[optind],O_RDONLY);
            while((c=mygetchar(desc)) !=EOF ){
                putchar(c);
            }
            close(desc);
        }
    } 
    //condition: with objetive file and with option -E but no option -n

    else if( !nflag && Eflag ){
        for(;optind<argc;optind++){
            desc=open(argv[optind],O_RDONLY);
            while((c=mygetchar(desc)) !=EOF ){
                if(c == '\n'){putchar('$');}
                putchar(c);
            }
            close(desc);
        }
    } 
    //condition: with objetive file and no option -E but with option -n

    else if( nflag && !Eflag  ){
        for(;optind<argc;optind++){
            desc=open(argv[optind],O_RDONLY);
            while((c=mygetchar(desc)) !=EOF){
                if(control == 0){
                printf("1");
                putchar(c);
                control=2;
            } else {
                putchar(c);
            }
                if(c == '\n'){
                    nbl++;
                    printf("%d",nbl);
                }
            }
            close(desc);
        }
    } 
    //condition: with objetive file and with option -E and option -n

    else if( nflag !=0 && Eflag != 0){
        for(;optind<argc;optind++){
            desc=open(argv[optind],O_RDONLY);
            while((c=mygetchar(desc)) !=EOF){
                if(control == 0){
                printf("1");
                putchar(c);
                control=2;
            } else {
                if(c == '\n'){putchar('$');}
                putchar(c);
            }
                if(c == '\n'){
                    nbl++;
                    printf("%d",nbl);
                }
            }
            close(desc);
        }
    }
    return 0;
}