#include <unistd.h>
#include <stdio.h>

 int main()
 {
   /*
   int oldfd;

   oldfd = open("mytest2",O_RDWR|O_CREAT,0644);
   dup2(oldfd,1);   //复制oldfd到文件描述符1（stdout标准输出）
   close(oldfd);    //关闭文件描述符oldfd
   printf("ddd");  //在标准输出上打印出ddd，这时由于标准输出已经被oldfd文件描述符代替
   return 0;       //打印到标准输出上的内容就全部打印到了文件mytest2中
 	*/
 
 	/*indication:	
 		fork
 		fils -> execlp("ps")
 		pere -> attend fin du fils
 		->fork
 			fils->execlp("grep")
 			pere->attend fin du fils
 			->si fils terminé par 0, alors printf

 	POUR REALISER COMMANDE : ps eaux >toto

 	*/
 	// fork();
 	// execlp("ps","ps","ps")
 	execlp("ps","ps",NULL);
 	return 0;
 }
