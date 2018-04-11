#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>



static char pathname[_POSIX_PATH_MAX + 1];

char *
searchdir( char *dirname, struct stat *fdstatp)
{
    int         len;    
    DIR     *dp;
    struct dirent   *dirp;
    struct stat statbuf;

    strcpy(pathname,dirname);
    strcat(pathname,"/");
    len = strlen(pathname);

    if((dp = opendir(dirname)) == NULL)
        return NULL;
    while((dirp = readdir(dp)) != NULL){
        strncpy(pathname +len,dirp->d_name,_POSIX_PATH_MAX - len);
        if ( strcmp(pathname + len,".") == 0 || strcmp(pathname+len+1,".") == 0) 
            continue;
        if (lstat(pathname,&statbuf) < 0)
            return NULL;
        if (S_ISDIR(statbuf.st_mode)){
            if (searchdir(pathname,fdstatp) != NULL)
                return pathname;
        } else if ( S_ISCHR(statbuf.st_mode)){
               if (statbuf.st_dev == fdstatp->st_dev && 
                    statbuf.st_ino == fdstatp->st_ino ){

                closedir(dp);
                return pathname;
               }
        }
    }
    closedir(dp);
    return NULL;
}
char *
my_ttyname( int fd)
{
    struct stat     statbuf;
    char        *term§pathname;

    if (isatty(fd) == 0)
        return NULL;

    if (fstat(fd,&statbuf) < 0)
        return NULL;
    termpathname = searchdir("/dev",&statbuf);

    return termpathname;
}

int main(int argc, char *argv[])
{
  int fd;
  char * file = argv[1];

  if ((fd = open (file, O_RDONLY)) != -1)
      printf("OpenError\n");;
  if(isatty(fd))
  {
   printf("ttyname = %s \n", my_ttyname(fd));
  }
  else
   printf(" is not a tty\n");
  close(fd);
}