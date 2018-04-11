#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>    
 /*
 *   写一个能查看其他文件的功能 
 *       cat 
 */  
         
  
#define LINELEN  256  
  
int main(int ac,char **av)  
{  
     int fd = -1 ;     //文件描述符  
     int rdsize = 0;  
     char buf[LINELEN+1];    //定义一个临时缓存区  
  
     if(1 >= ac){  
         printf("file name empty\n");  
         return 1;  
     }  
  
     fd = open(av[1],O_RDWR|O_CREAT|O_APPEND,0644);     //可读的写，wr-w--w--  
     if(0 > fd){  
         printf("open error [%d]\n",errno);  
         return 2;  
     }
     buf[LINELEN] = '\0';  
     memset(buf,'\0',LINELEN);      //'\0'   == 0  
     while(0 < (rdsize = read(fd,buf,LINELEN))){  //每次读（缓存区大小-1）的字节，直至读的值为0  
         printf("%s",buf);                
         memset(buf,'\0',LINELEN);    //每次都得进行清除操作  
     }  
     printf("\n");  
  
     close(fd);    //关闭描述符  
     return 0;  
}  
