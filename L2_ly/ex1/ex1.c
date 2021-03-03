/*************************************
* Lab 2 Exercise 1
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
*************************************/

#include <stdio.h>
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>     //for fork()
#include <sys/wait.h>   //for wait()
#include <stdlib.h>

int main()
{
    int nChild;

    //Read the number of child
    scanf("%d", &nChild);
	int childID[nChild];

    //Spawn child processes
    for (int i = 0; i < nChild; i++) {
    	int id;
		childID[i] = fork();
		if (childID[i] == 0 ) {
			sleep(4);
			printf("Child %i[%i]: Hello!\n", i + 1, getpid());
			
			exit(0);
		}
    }
	for (int i = 0; i < nChild; i++) {
		int pid = waitpid(childID[i], 0, 0);
        printf("Parent: Child %i[%i] done.\n",i + 1, pid);
	}
    printf("Parent: Exiting.\n");
    //Wait on child processes in order
     
    return 0;
}
