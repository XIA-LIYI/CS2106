#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

int main(int argc, char** argv) {
    int id = fork();
    char *command[] = {"return", "123", NULL};
    int result;
    if (id == 0) {
        execv("./return", command);
    } else {
        
        waitpid(id, &result, 0);
        printf("%i", WEXITSTATUS(result));
    }
}