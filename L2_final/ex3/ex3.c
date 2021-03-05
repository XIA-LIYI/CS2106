/*************************************
 * * Lab 2 Exercise 3 with Bonus
 * * Name     : Xia Liyi
 * * Matric No: A0177873L
 * * Lab Group: B19
 * *************************************/

/*************************************
 * * Lab 2 Exercise 3 with Bonus
 * * Name     : Zhang Yunjie
 * * Matric No: A0204208U
 * * Lab Group: B09
 * *************************************/

/*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
*************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()
#include <signal.h>


int waiting_pid = 0;
int is_running = 0;

char** split( char* input, char* delimiter, int maxTokenNum, int* readTokenNum )
//Assumptions:
//  - the input line is a string (i.e. with NULL character at the end)
//  - the delimiter is a string of possible delimiters, each delimiter is single chracter
//Behavior:
//  - Split up to and include maxTokenNum from the input string
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - Must use the freeTokenArray to free memory after use!
{
    char** tokenStrArr;
    char* tStart;   //start of token
    int i, strSize;      

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum );

    //Nullify all entries
    for (i = 0; i < maxTokenNum; i++){
        tokenStrArr[i] = NULL;
    }

    //Look for next token as separated by the delimiter
    tStart = strtok(input, delimiter); 

    i = 0;
    while (i < maxTokenNum && tStart){

        strSize = strlen(tStart);

        //Allocate space for token string. +1 for null terminator
        tokenStrArr[i] = (char*) malloc(sizeof(char) * (strSize + 1) );

        strcpy(tokenStrArr[i], tStart);    

        i++;
        tStart = strtok(NULL, delimiter); 
    }

    //return number of token read
    *readTokenNum = i;

    return tokenStrArr;
}

void freeTokenArray(char** strArr, int size)
{
    int i;

    //Free each string first
    for (i = 0; i < size; i++){
        if (strArr[i])
            free( strArr[i] );
        strArr[i] = NULL;
    }

    //Free the entire array
    free( strArr );

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}

void makeArgsList(char *argsList[], char **cmdLineArgs, int num) {
    for (int i = 0; i < num; i++) {
        if (strcmp(cmdLineArgs[i], "&") != 0) {
            argsList[i] = cmdLineArgs[i];
        }
    }
    argsList[num] = NULL;
}

void pc(int pids[10]) {
    printf("Unwaited Child Processes:\n");
    for (int i = 0; i < 10; i++) {
        if (pids[i] != 0) {
            printf("%i\n", pids[i]);
        }
    }
}

void add_pid(int pids[10], int pid) {
    for (int i = 0; i< 10; i++) {
        if (pids[i] == 0) {
            pids[i] = pid;
            break;
        }
    }
}

void remove_pid(int pids[10], int pid) {
    for (int i = 0; i < 10; i++) {
        if (pids[i] == pid) {
            pids[i] = 0;
        }
    }
}

int check_pid(int pids[10], int pid) {
    for (int i = 0; i < 10; i++) {
        if (pids[i] == pid) {
            return 1;
        }
    }
    return 0;
}

void myOwnHandler(int signo)
{
    if (signo == SIGINT){
        if (waiting_pid != 0) {
            kill(waiting_pid, 2);
        } else if (is_running == 0) {
            printf("Nothing to kill.\n");
        }
    }

}

int main()
{
    char **cmdLineArgs;
    char path[20] = ".";  //default search path
    char userInput[121];

    int tokenNum;

    signal(SIGINT, myOwnHandler);

    //read user input
    printf("YWIMC > ");

    //read up to 120 characters from user input
    fgets(userInput, 120, stdin);

    //split is a helper function to "tokenize" the input
    //see "stringTokenizer.c" program for more sample usage 
    cmdLineArgs = split( userInput, " \n", 7, &tokenNum );

    //At this point you have the user input split neatly into token in cmdLineArg[]

    int result;
    int pids[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    while ( strcmp( cmdLineArgs[0], "quit") != 0 ){


        //Figure out which command the user want and implement below
        if (strcmp(cmdLineArgs[0], "showpath") == 0) {
            printf("%s\n", path);
        } else if (strcmp(cmdLineArgs[0], "setpath") == 0) {
            strcpy(path, cmdLineArgs[1]);
        } else if (strcmp(cmdLineArgs[0], "wait") == 0) {
            int pid = atoi(cmdLineArgs[1]);
            if (check_pid(pids, pid) == 1) {
                remove_pid(pids, pid);
                waiting_pid = pid;
                waitpid(pid, &result, 0);
                waiting_pid = 0;
            } else {
                printf("%i not a valid child pid\n", pid);
            }
        } else if (strcmp(cmdLineArgs[0], "pc") == 0) {
            pc(pids);
        } else if (strcmp(cmdLineArgs[0], "result") == 0) {
            printf("%i\n", WEXITSTATUS(result));   
        } else if (strcmp(cmdLineArgs[tokenNum - 1], "&") == 0) {
            char runCommand[100] = "";
            struct stat buf;

            strcat(runCommand, path);
            strcat(runCommand, "/");
            strcat(runCommand, cmdLineArgs[0]);

            int fileExsits = stat(runCommand, &buf);
            if (fileExsits == 0) {
                int pid = fork();
                if (pid == 0) {
                    // setsid();
                    setpgid(0, 0);
                    char *argsList[tokenNum];
                    makeArgsList(argsList, cmdLineArgs, tokenNum - 1);
                    int res = execv(runCommand, argsList);
                    if (res == -1) {
                        printf("The command fails to run due to some unknown reasons.\n");    
                    }
                    exit(1);
                } else {
                    add_pid(pids, pid);
                    printf("Child %i in background\n", pid);
                    
                }
            } else {
                printf("\"%s\" not found\n", runCommand);
            }


        } else {
            char runCommand[100] = "";
            struct stat buf;

            strcat(runCommand, path);
            strcat(runCommand, "/");
            strcat(runCommand, cmdLineArgs[0]);

            int fileExsits = stat(runCommand, &buf);
            if (fileExsits == 0) {
                int pid = fork();
                if (pid == 0) {
                    char *argsList[tokenNum + 1];
                    makeArgsList(argsList, cmdLineArgs, tokenNum);

                    int res = execv(runCommand, argsList);

                    if (res == -1) {
                        printf("The command fails to run due to some unknown reasons.");
                    }
                } else {
                    is_running = 1;
                    waitpid(pid, &result, 0);
                    is_running= 0;
                }
            } else {
                printf("\"%s\" not found\n", runCommand);
            }
        }

        //Prepare for next round input

        //Clean up the token array as it is dynamically allocated
        freeTokenArray(cmdLineArgs, tokenNum);

        printf("YWIMC > ");
        fgets(userInput, 120, stdin);
        cmdLineArgs = split( userInput, " \n", 7, &tokenNum );
    }
    

    printf("Goodbye!\n");

    //Clean up the token array as it is dynamically allocated
    freeTokenArray(cmdLineArgs, tokenNum);


    return 0;

}
