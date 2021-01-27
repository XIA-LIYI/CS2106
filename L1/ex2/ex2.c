/*************************************
* Lab 1 Exercise 1
* Name:
* Matric No:
* Lab Group:
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
    	printf("%i %i %i", position, input, copy);
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
    
    node* ptr = head;
    int index = 0;
    if (position == 0 || head == NULL) {
    	int copy = 0;
	while (copy < copies) {
	    copy++;
	    node* now = malloc(sizeof(node));
	    now->data = newValue;
	    now->next = ptr;
	    ptr = now;
	}
	return ptr;
    }

    node* prev = ptr;
    while (index < position - 1 && ptr != NULL) {
    	index++;
	prev = ptr;
	ptr = ptr->next;
    }
    node* last = ptr;
    if (ptr == NULL) {
    	int copy = 0;
	ptr = NULL;
	while (copy < copies) {
	    copy++;
	    node* now = malloc(sizeof(node));
	    now->data = newValue;
	    now->next = ptr;
	    ptr = now;
	}
	prev->next = ptr;
	return head;
    }
    
    ptr = ptr->next;
    int copy = 0;
    while (copy < copies) {
	copy++;
	node* now = malloc(sizeof(node));
	now->data = newValue;
	now->next = ptr;
	ptr = now;
    }
    last->next = ptr;
    
    return head;    //change this!
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
    //You can use the same implementation as in exercise 1
}
