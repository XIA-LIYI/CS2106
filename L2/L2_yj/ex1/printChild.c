#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
//#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()



int main(int argc, char* argv[]) {
     
    int index = atoi(argv[1]);
    printf("Child %i[%i]: Hello!\n", index, getpid());

    return 0;
}
