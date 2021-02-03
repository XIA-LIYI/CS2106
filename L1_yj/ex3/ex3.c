/*************************************
* Lab 1 Exercise 3
* Name     :
* Matric No:
* Lab Group:
*************************************/

#include <stdio.h>
#include <math.h>

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

    arithFuncPtr ptr1, ptr2, ptr3, ptr4;

    //ptr points to the function add
    ptr1 = ap;
    ptr2 = gcd;
    ptr3 = removeFactor;
    ptr4 = power;

    arithFuncPtr ptrArray[4] = {ptr1,ptr2,ptr3,ptr4};
    char printArray[4][15] = {"lumos", "alohomora", "expelliarmus", "sonorus"};
    
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
    return pow(base,power);
}

int ap(int firstNum, int lastNum) { //arithmetic progression
    int n = lastNum - firstNum;
    return (n + 1) * firstNum + (n + 1) * n / 2;
} 

int removeFactor(int num, int factor) {
    while (num % factor == 0) {
        num /= factor;
    }
    return num;
}
