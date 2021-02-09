/*************************************
* Lab 1 Exercise 2
* Name: Zhang Yunjie
* Matric No: A0204208U
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
node* addToHead(node* head, int newData);
node* getNode(int data);
node* createLL(node* head, int copies, int value);


int main()
{
    node* myList = NULL;    //Empty List
    int position, input, copies;

    //Fill in code for input and processing
    while (scanf("%i %i %i", &position, &input, &copies) == 3) {
       // if (position < 0 || copies < 1) {
       //     printf("Invalid inputs.\n");
       //     return 0;
       // }

        myList = insertAt(myList, position, copies, input);
    }

    //Output code coded for you
    printf("My List:\n");
    printList(myList);
    
    destroyList(myList);
    myList = NULL; // set pointer to null to prevent memory leak

    
    printf("My List After Destroy:\n");
    printList(myList);
 
    return 0;
}

//Actual Function Implementations
node* insertAt(node* head, int position, int copies, int newValue) {
    node* firstNode = getNode(newValue);
    node* lastNode = createLL(firstNode, copies, newValue);
    node* temp; //node after the inserted nodes
        
    if (position == 0) {
        lastNode -> next = head;
        head = firstNode;
    } else {
        // traverse the linked list until the indicated position
        int count = 1;
        node* curr = head;
        while (count < position) {
            if (curr -> next == NULL) {
                break;
            }
            curr = curr -> next;
            count ++;
        }
        node* temp = curr -> next;
        curr -> next = firstNode;
        lastNode -> next = temp;
    }
    return head;
}

    
// Function to creare a linked list of the same value
node* createLL(node* head,int copies, int value) {
    node* curr = head;
    while (copies > 1) {
        node* newNode = getNode(value);
        curr -> next = newNode;
        curr = newNode;
        copies --;
    }
    return curr; //return the pointer to tail node
}


// Function to create a linked list from scratch
node* addToHead(node* head, int newData) {
    node* newNode;
    newNode = (node*)malloc(sizeof(node));
    newNode->data = newData;
    newNode->next = head;

    return newNode;
}

// Function to create a new node with a given data
node* getNode(int data) {
    // allocating space
    node* newNode = (node*)malloc(sizeof(node));
 
    // inserting the required data
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
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

void destroyList(node* head) {
    node *toFree,*cur; // use two pointers for buffer
    cur = head;
    while (cur != NULL) {
        toFree = cur;
        cur = cur->next;
        free(toFree);
    }
}
