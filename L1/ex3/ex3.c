/*************************************
* Lab 1 Exercise 3
* Name     : Xia Liyi
* Matric No: A0177873L
* Lab Group: B19
*************************************/

/*************************************
* Lab 1 Exercise 3
* Name     : Zhang Yunjie
* Matric No: A0204208U
* Lab Group: B09
*************************************/


#include <stdio.h>

//Datatype Declarations
typedef int (*arithFuncPtr)(int, int);


//Function Prototypes
int ap(int x, int y);
int gcd(int x, int y);
int removeFactor(int x, int y);
int power(int x, int y);

int main() {
    int a, b, optype, res;

    //arithFuncPtr is a function pointer datatype
    //   - it points to afunction that takes (int, int) as parameter
    //   - and returns int as return result

    arithFuncPtr ptrArray[4] = {ap,gcd,removeFactor,power};
    char printArray[4][13] = {"lumos", "alohomora", "expelliarmus", "sonorus"};
    
    while (scanf("%i %i %i", &optype, &a, &b) == 3) {
        res = (*ptrArray[optype - 1])(a, b);
        printf("%s %i\n", printArray[optype - 1], res);
    }
    
    return 0;
}


int gcd(int x, int y) {
    if (y != 0) {
        return gcd(y, x % y);
    } else {
        return x;
    }
}

int power(int base, int power) {
    int res = 1;
    for (int i = 0; i < y; i++) {
    	res = res * x;
    }
    return res;
}

int ap(int x, int y) { //arithmetic progression
    return (x + y) * (y - x + 1) / 2;
} 

int removeFactor(int num, int factor) {
    while (num % factor == 0) {
        num /= factor;
    }
    return num;
}
