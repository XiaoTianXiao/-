#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>          
#include <stdnoreturn.h>        
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <limits.h>         
#include <inttypes.h>

char *prog
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

int main(int argc, char *argv[]){

    prog=argv[0];

    int fd;
    int new_fd;

    if((fd = open("argv[1]", O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1)
        raler("openError");
    printf("fd = %d\n", fd);

    new_fd = dup(fd);    //复制文件描述符,把fd复制给new_fd
    printf("new_fd = %d\n", new_fd);
    write(new_fd, "hello", 5);

    close(fd);
    close(new_fd);

    return 0;
}