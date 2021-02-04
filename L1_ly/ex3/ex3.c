/*************************************
* Lab 1 Exercise 3
* Name     :
* Matric No:
* Lab Group:
*************************************/

#include <stdio.h>

//Datatype Declarations
typedef int (*arithFuncPtr)(int, int);


//Function Prototypes
int add(int x, int y);
int spell1(int x, int y);
int spell2(int x, int y);
int spell3(int x, int y);
int spell4(int x, int y);

int main()
{
    int a, b, optype, res;

    //The code below is to show the usage of a function pointer only
    //You can remove/comment them off 

    //arithFuncPtr is a function pointer datatype
    //   - it points to afunction that takes (int, int) as parameter
    //   - and returns int as return result
    
    arithFuncPtr actionList[4] = {spell1, spell2, spell3, spell4}; 
    char nameList[4][13] = {"lumos", "alohomora", "expelliarmus", "sonorus"};

    while (scanf("%i", &optype) == 1) {
    	scanf("%i %i", &a, &b);
	res = actionList[optype - 1](a, b);
    	printf("%s %i\n", nameList[optype - 1], res);
    }
   
    return 0;
}

int spell1(int x, int y) {
    return (x + y) * (y - x + 1) / 2;
}

int spell2(int x, int y) {
    int gcd = 1;
    for (int i = 1; i <= x && i <= y; i++) {
    	if (x % i == 0 && y % i == 0) {
	    gcd = i;
	}
    }
    return gcd;
}

int spell3(int x, int y) {
    while (x % y == 0) {
    	x = x / 3;
    }
    return x;
}

int spell4(int x, int y) {
    int res = 1;
    for (int i = 1; i < y + 1; i++) {
    	res = res * x;
    }
    return res;
}
