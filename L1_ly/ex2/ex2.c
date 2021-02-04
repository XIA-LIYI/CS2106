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
#include <stdlib.h> //for malloc() and free()

//Declaration of a Linked List Node

typedef struct NODE{
    int data;
    struct NODE* next;
} node;

//Function Prototypes
node* insertAt(node*, int, int, int);

void printList(node*);
void destroyList(node*);



int main()
{
    node* myList = NULL;    //Empty List
    int position, input, copy;

    //Fill in code for input and processing
    while (scanf("%i %i %i", &position, &input, &copy) == 3) {
	myList = insertAt(myList, position, copy, input);
    }  

    //Output code coded for you
    printf("My List:\n");
    printList(myList);
    
    destroyList(myList);
    myList = NULL;

    
    printf("My List After Destroy:\n");
    printList(myList);
 
    return 0;
}

//Actual Function Implementations
node* insertAt(node* head, int position, int copies, int newValue)
{
    //Fill in your code here
    
    node* curr = head;
    node* prev = head;
    int index = 0;
    while (index < position && curr != NULL) {
    	index++;
	prev = curr;
	curr = curr->next;
    }
    for (int i = 0; i < copies; i++) {
        node* new = malloc(sizeof(node));
        new -> data = newValue;
        new -> next = curr;
        curr = new;
    }
    if (prev == NULL || position == 0) {
	return curr;
    } else {
	prev -> next = curr;
	return head;
    }
}	
 
void printList(node* head)
//Purpose: Print out the linked list content
//Assumption: the list is properly null terminated
{
    //This function has been implemented for you
    node* ptr = head;

    while (ptr != NULL)  {    //or you can write while(ptr)
        printf("%i ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}


void destroyList(node* head)
{
    //Fill in your code here
    
    node* ptr = head;
    while (ptr != NULL) {
    	node* now = ptr;
	ptr = ptr->next;
	free(now);
    }
    free(ptr);
}
