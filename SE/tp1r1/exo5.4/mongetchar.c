/**
 * @file mongetchar non_buffer
 * @author xiaotian xiao
 */
#include <stdio.h>
#include <unistd.h>
/**
 * @brief The fonction mygetchar will read the first caracter on the standard screen
 * @return If the read is finished, will return EOF to n, and return caracter's number  
 */
int mygetchar(){
	unsigned char c;	
	ssize_t n;
	n = read(0, &c, 1);
	if(n<=0){
		return EOF;
	}
	return c;
}
/**
 * @author xiaotian
 * @dateTime 2018-02-08
 * @description: use a while circle for putchar each caracter to the screen	
 */
int main(){
	int c;
	while((c = mygetchar()) != EOF){
		putchar(c);
	}
	return 0;
}	