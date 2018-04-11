#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>

int main(int argc, char const *argv[])
{
	struct stat buf;
	stat(argv[1],&buf);
	printf("%lld\n",buf.st_size );
	return 0;
}