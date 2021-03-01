/*************************************
* Lab 2 Exercise 3
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
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
//#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()


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


void printUnwaitedPid(int pids[]) {
    printf("Unwaited Child Process:\n");
    for (int i = 0; i < 10; i++) {
        if (pids[i] != 0) {
            printf("%i\n",pids[i]);
        }
    }
}

void addUnwaitedPid(int pids[], int pid) {
    for (int i = 0; i < 10; i++) {
        if (pids[i] == 0) {
            pids[i] = pid;
            break;
        }
    }
}

void removeUnwaitedPid(int pids[], int pid) {
    for (int i = 0; i < 10; i++) {
        if (pids[i] == pid) {
            pids[i] = 0;
            break;
        }
    }
}

int isUnwaited(int pids[], int pid) {
    for (int i = 0; i < 10; i++) {
        if (pids[i] == pid) {
            return 1;
        }
    }
    return 0;
}

void makeArgsList(char *argsList[], char **cmdLineArgs, int num) {
    for (int i = 0; i < num; i++) {
        if (strcmp(cmdLineArgs[i], "&") != 0) {
            argsList[i] = cmdLineArgs[i];
        }
    }
    argsList[num] = NULL;
}
    


int main()
{
    char **cmdLineArgs;
    char path[20] = ".";  //default search path
    char userInput[121];

    int tokenNum;

    int pids[10] = {0}; // function scope, enough to set {0}
    int status; // status of child process termination


    //read user input
    printf("YWIMC > ");

    //read up to 120 characters from user input
    fgets(userInput, 120, stdin);

    //split is a helper function to "tokenize" the input
    //see "stringTokenizer.c" program for more sample usage 
    cmdLineArgs = split( userInput, " \n", 7, &tokenNum );

    //At this point you have the user input split neatly into token in cmdLineArg[]

    while ( strcmp( cmdLineArgs[0], "quit") != 0 ){

        //Figure out which command the user want and implement below
        if (strcmp(cmdLineArgs[0], "showpath") == 0) {
            printf("%s\n", path);
        } else if (strcmp(cmdLineArgs[0], "setpath") == 0) {
            strcpy(path, cmdLineArgs[1]);
        } else if (strcmp(cmdLineArgs[0], "wait") == 0) {
            int pid = atoi(cmdLineArgs[1]);
            if (isUnwaited(pids,pid)) {
                waitpid(pid,&status,0);
                removeUnwaitedPid(pids,pid);
            } else {
                printf("%i not a valid child pid\n",atoi(cmdLineArgs[1]));
            }
        } else if (strcmp(cmdLineArgs[0], "pc") == 0) {
            printUnwaitedPid(pids);
        } else if (strcmp(cmdLineArgs[0], "result") == 0) {
            printf("%i\n",WEXITSTATUS(status));
        } else {
            char filename[40];
            strcpy(filename,path);
            strcat(filename,"/");
            strcat(filename,cmdLineArgs[0]);

            struct stat buffer;
            int exist = stat(filename,&buffer);
            if (exist == -1) {
                printf("\"%s\" not found\n", filename);
            } else {
                int pid =  fork();
                if (strcmp(cmdLineArgs[tokenNum - 1], "&") == 0) { // run command in the background
                    if (pid == 0) {
                        addUnwaitedPid(pids, getpid());
                        printf("Child %i in background\n", getpid());
                        char *argsList[tokenNum];
                        makeArgsList(argsList, cmdLineArgs, tokenNum);
                        execv(filename, argsList);
                    }
                } else { // run command 
                    if (pid == 0) {
                        char *argsList[tokenNum + 1];
                        makeArgsList(argsList, cmdLineArgs, tokenNum);
                        execv(filename, argsList);
                    } else {
                        waitpid(pid,&status,0);
                    }
                }
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
