#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

#define READ_END 0
#define WRITE_END 1
int main()
{
	int pipeFd[2], pid, len;
	char buf[100], *str = "Hello There!";
	pipe( pipeFd );
	if ((pid = fork()) > 0) { /* parent */
		close(pipeFd[READ_END]);
		write(pipeFd[WRITE_END], str, strlen(str)+1);
		close(pipeFd[WRITE_END]);
	} else { /* child */
		close(pipeFd[WRITE_END]);
		len = read(pipeFd[READ_END], buf, sizeof buf);
		printf("Proc %d read: %s\n", pid, buf);
		close(pipeFd[READ_END]);
	}
}
