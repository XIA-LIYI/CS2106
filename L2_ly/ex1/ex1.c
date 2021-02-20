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

int main()
{
    int nChild;

    //Read the number of child
    scanf("%d", &nChild);

    //Spawn child processes
    for (int i = 1; i <= nChild; i++) {
    	int id;
	id = fork();
	if (id == 0 ) {
	    printf("Child %i[%i]: Hello!\n", i, getpid());
	    sleep(1);
	    exit(0);
	} else {
	    int pr;
	    pr = waitpid(id, NULL, 0);
   	    printf("Parent: Child %i[%i] done.\n", i, id);
	}
    }
    printf("Parent: Exiting.");
    //Wait on child processes in order
     
    return 0;
}
