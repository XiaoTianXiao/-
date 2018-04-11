/**
 * @author XIAOTIAN
 * @description: This projet for the tp_rendre of first TP. Require: creat a mycat by operating system
 * @date 09/02/2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#define Taille_Buffer 1024

//#define Taille_Buffer 1024
//By defaut, the size of Taille_Buffer is 8192

/**
 * @author xiaotian
 * @dateTime 2018-02-09
 * @return   EOF
 */
int mygetchar(){
	static unsigned char buf[Taille_Buffer]; //We can change Taille_Buffer with Taille_Buffer, anyway, here just is the size of buffer
	static unsigned char *bufp = buf; //we creat a point *bufp for pointting to the buffer
	static ssize_t n = 0; 			//Here we use 3 static variable to get a stat , so they can't be return to original by each circle
	if(n == 0){
		n = read(0,buf,sizeof buf);
		bufp = buf;
	}
	if(--n >= 0){     // It is same with if(n > 0) {n--; return .......} ;
		return (unsigned char) *bufp++;
	}
	return EOF;
}
/**
 * @author xiaotian
 * @dateTime 2018-02-09
 * @param    msg        This is a message to print when we have error.
 */
void raler(const char *msg){
	perror(msg);
	exit(1);
}

/**
 * @author xiaotian
 * @dateTime 2018-02-10
 * @param    argc       number of option
 * @param    argv       a pointer to an "option" array data structure   
 * @return              nothing is important for return
 */

int main(int argc,char *argv[]){
	
	if(argc <= 1){
		printf("The Filename if empty");
		exit(1);
	}

	ssize_t fd1 = -1;
	fd1 = open(argv[1],O_RDWR|O_CREAT|O_APPEND,0644);
	int rdsize = 0;
	char buf[Taille_Buffer+1];
	char *bufp = buf; 
	
	if(fd1 < 0){
		raler("openfd1");
	}

	buf[Taille_Buffer] = '\0';
	memset(buf,'\0',Taille_Buffer);
	while((rdsize = read(fd1,buf,Taille_Buffer)) > 0){
		for(int w = 0; 0 < Taille_Buffer; w++ ){
		putchar(*bufp++);
		//memset(buf,'\0',Taille_Buffer);
	}
		//memset(buf,'\0',Taille_Buffer);
	}
	printf("\n");
	close(fd1);
	
	// while((fd1 = read()) != EOF){
	// 	putchar(fd1);
	// }
	// return 0;

	int c;
	//int opterr= 0 ;
	while((c = getopt(argc,argv,"nE")) != EOF){
		printf("Your option char: %c \n",c);
		switch(c){
			case'n':
				printf("putout line number !\n");
				break;
			case 'E':
				printf("putout a sign & in each end of a line \n.");
				break;
			case '?':
				fprintf(stderr, "Usage: %s [-n] [-E] file\n",argv[0]);
				break;
		}
	}
	

}