/*************************************
* Lab 2 Exercise 1
* Name: Zhang Yunjie
* Student Id: A0204208U
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
int main() {
    int nChild;
    int pid, i, pID[9];
    char index[4];
    char spid[10];


    //Read the number of child
    scanf("%d", &nChild);

    //Spawn child processes
    for(int i = 0; i < nChild; i++) {
        pID[i] = fork(); 

        if (pID[i] == 0) { // child process
            sprintf(index, "%i", i + 1);
            execl("./printChild","printChild",index,NULL);
            sleep(4);
        }
    }

    //Wait on child processes in order
    for (int i = 0; i < nChild; i++) {
         pid = waitpid(pID[i],0,0);
         printf("Parent: Child %i[%i] done.\n",i + 1, pid);
    }
  
     
    return 0;
}
