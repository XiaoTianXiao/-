/**
 * @author xiaotian
 * @description My getchar with buffer which size is 1024 o
 */
#include <stdio.h>
#include <unistd.h>
//#define Taille_Buffer 1024
//By defaut, the size of BUFSIZ is 8192

int mygetchar(){
	static unsigned char buf[BUFSIZ]; 
	static unsigned char *bufp = buf; 
	static ssize_t n = 0; 			
	if(n == 0){
		n = read(0,buf,sizeof buf);
		bufp = buf;
	}
	if(--n >= 0){     
		return *bufp++;
	}
	return EOF;
}

int main(){
	int c;
	while((c = mygetchar()) != EOF){
		putchar(c);
	}
	return 0;
}