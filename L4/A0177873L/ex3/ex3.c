/*************************************
* Lab 4 Exercise 3
* Name: Xia Liyi
* Student Id: A0177873L
* Lab Group: B19
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/*************************************
* Lab 4 Exercise 3
* Name: Zhang Yunjie
* Student Id: A0204208U
* Lab Group: B19
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmalloc.h"

/**********************************************************
 * This is a "global" structure storing heap information
 * The "static" modifier restrict the structure to be
 *  visible only to functions in this file
 *********************************************************/
static heapMetaInfo hmi;


/**********************************************************
 * Quality of life helper functions / macros
 *********************************************************/
#define powOf2(E) (1 << E)

unsigned int log2Ceiling( unsigned int N )
/**********************************************************
 * Find the smallest S, such that 2^S >= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 < N){
        pOf2 <<= 1;
        s++;
    }

    return s;
}


unsigned int log2Floor( unsigned int N )
/**********************************************************
 * Find the largest S, such that 2^S <= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 <= N){
        pOf2 <<= 1;
        s++;
    }

    return s-1;
}

unsigned int buddyOf( unsigned int addr, unsigned int lvl )
/**********************************************************
 * Return the buddy address of address (addr) at level (lvl)
 *********************************************************/
{
    unsigned int mask = 0xFFFFFFFF << lvl;
    unsigned int buddyBit = 0x0001 << lvl;

    return (addr & mask) ^ buddyBit;
}

partInfo* buildPartitionInfo(unsigned int offset)
/**********************************************************
 * Allocate a new partInfo structure and initialize the fields
 *********************************************************/
{
    partInfo *piPtr;

    piPtr = (partInfo*) malloc(sizeof(partInfo));

    piPtr->offset = offset;
	piPtr->nextPart = NULL;

    //Buddy system's partition size is implicit
	//piPtr->size = size;

    //All available partition in buddy system is implicitly free
	//piPtr->status = FREE;

    return piPtr;
}

void printPartitionList(partInfo* piPtr)
/**********************************************************
 * Print a partition linked list
 *********************************************************/
{
	partInfo* current;
    int count = 1;
	
	for ( current = piPtr; current != NULL; 
		current = current->nextPart){
        if (count % 8 == 0){
            printf("\t");
        }
		printf("[+%5d] ", current->offset);
        count++;
        if (count % 8 == 0){
            printf("\n");
        }
	}
    printf("\n");
}

void printHeapMetaInfo()
/**********************************************************
 * Print Heap Internal Bookkeeping Information
 *********************************************************/
{
    int i;

	printf("\nHeap Meta Info:\n");
	printf("===============\n");
	printf("Total Size = %d bytes\n", hmi.totalSize);
	printf("Start Address = %p\n", hmi.base);

    for (i = hmi.maxIdx; i >=0; i--){
        printf("A[%d]: ", i);
        printPartitionList(hmi.A[i] );
    }

}

void printHeap()
/**********************************************************
 * Print the content of the entire Heap 
 *********************************************************/
{
    //Included as last debugging mechanism.
    //Print the entire heap regions as integer values.

    int* array;
    int size, i;
    
    size = hmi.totalSize / sizeof(int);
    array = (int*)hmi.base;

    for ( i = 0; i < size; i++){
        if (i % 4 == 0){
            printf("[+%5d] |", i);
        }
        printf("%8d",array[i]);
        if ((i+1) % 4 == 0){
            printf("\n");
        }
    }
}

int power(int base, int power) {
    int res = 1;
    for (int i = 0; i < power; i++) {
        res = res * base;
    }
    return res;
}

void printHeapStatistic()
/**********************************************************
 * Print Heap Usage Statistics
 *********************************************************/
{
    //TODO: Task 4. Calculate and report the various statistics

    printf("\nHeap Usage Statistics:\n");
    printf("======================\n");

    printf("Total Space: %d bytes\n", hmi.totalSize);

   //Remember to preserve the message format!
    int numOfFreePartition = 0;
    int totalFreeSize = 0;
    for (int i = 0; i <= hmi.maxIdx; i++) {
        partInfo* curr = hmi.A[i];
        if (curr == NULL) {
            continue;
        }
        while (curr != NULL) {
            numOfFreePartition++;
            totalFreeSize += power(2, i);
            curr = curr -> nextPart;
        }
    }
    
    printf("Total Free Partitions: %d\n", numOfFreePartition);
    printf("Total Free Size: %d bytes\n", totalFreeSize);

    printf("Total Internal Fragmentation: %d bytes\n", hmi.internalFragTotal);
}

void addPartitionAtLevel( unsigned int lvl, unsigned int offset )
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function adds a new free partition with "offset" at hmi.A[lvl]
 *    If buddy is found, recursively (or repeatedly) perform merging and insert
 *      at higher level
 *********************************************************/
{
    // My implementation is to add a partition with offset sorted.
    partInfo* curr = hmi.A[lvl];
    if (curr == NULL) {
        hmi.A[lvl] = buildPartitionInfo(offset);
        return;
    }
    if (curr -> offset > offset) {
        hmi.A[lvl] = buildPartitionInfo(offset);
        hmi.A[lvl] -> nextPart = curr;
        return;
    }
    while (curr -> nextPart != NULL) {
        if (offset < curr -> nextPart -> offset) {
            break;
        }
        curr = curr -> nextPart;
    }
    partInfo* next = curr -> nextPart;
    curr -> nextPart = buildPartitionInfo(offset);
    curr -> nextPart -> nextPart = next;
}

partInfo* removePartitionAtLevel(unsigned int lvl)
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function remove a free partition at hmi.A[lvl]
 *    Perform the "upstream" search if this lvl is empty AND perform
 *      the repeated split from higher level back to this level.
 * 
 * Return NULL if cannot find such partition (i.e. cannot sastify request)
 * Return the Partition Structure if found.
 *********************************************************/
{
    // My implementation is only about remove the first element from current level
    // because it is sorted by offset when adding the partition.
    if (hmi.A[lvl] == NULL) {
        return NULL;
    }
    if (hmi.A[lvl] -> nextPart == NULL) {
        partInfo* best = hmi.A[lvl];
        hmi.A[lvl] = NULL;
        return best;
    }
    partInfo* res = hmi.A[lvl];
    hmi.A[lvl] = hmi.A[lvl] -> nextPart;
    return res;
}

int setupHeap(int initialSize)
/**********************************************************
 * Setup a heap with "initialSize" bytes
 *********************************************************/
{
	void* base;

	base = sbrk(0);
	if(	sbrk(initialSize) == (void*)-1){
		printf("Cannot set break! Behavior undefined!\n");
		return 0;
	}

    hmi.base = base;

	hmi.totalSize = initialSize;
    hmi.internalFragTotal = 0;
	
    //TODO: Task 1. Setup the rest of the bookkeeping info:
    //       hmi.A <= an array of partition linked list
    //       hmi.maxIdx <= the largest index for hmi.A[]
    //       

    int numOfLevels = log2Ceiling(initialSize);
    hmi.A = (partInfo**) malloc((numOfLevels + 1) * sizeof(partInfo*));
    for (int i = 0; i < numOfLevels + 1; i++) {
        hmi.A[i] = NULL;
    }
    hmi.maxIdx = numOfLevels;
    hmi.A[numOfLevels] = buildPartitionInfo(0);

    return 1;
}

void allocateMemory(int lvl) {
    // My idea is to check whether current level has accessible memory.
    // If yes, break it into two parts and give it to lower level.
    // If no, ask for higher level.
    partInfo* removed = removePartitionAtLevel(lvl);
    if (removed == NULL) {
        allocateMemory(lvl + 1);
        removed = removePartitionAtLevel(lvl);
    }

    addPartitionAtLevel(lvl - 1, removed -> offset);
    addPartitionAtLevel(lvl - 1, removed -> offset + power(2, lvl - 1));

}

void* mymalloc(int size)
/**********************************************************
 * Mimic the normal "malloc()":
 *    Attempt to allocate a piece of free heap of (size) bytes
 *    Return the memory addres of this free memory if successful
 *    Return NULL otherwise 
 *********************************************************/
{
    //TODO: Task 2. Implement the allocation using buddy allocator

    int lvl = log2Ceiling(size);
    partInfo* best = removePartitionAtLevel(lvl);
    if (best == NULL) {
        allocateMemory(lvl + 1);
        best = removePartitionAtLevel(lvl);
    }
    
    if (best == NULL) {
        return NULL;
    } else {
        hmi.internalFragTotal = hmi.internalFragTotal + (power(2, lvl) - size);
        return (void*) hmi.base + best -> offset;
    }
}

int checkPartitionAtLevel(int lvl, int offset) {
    // This is to check whether level lvl has a partition with offset.
    partInfo* curr = hmi.A[lvl];
    while (curr != NULL) {
        if (curr -> offset == offset) {
            return 1;
        }
        curr = curr -> nextPart;
    }
    return 0;
}

void removePartitionAtLevelWithOffset(int lvl, int offset) {
    // This is to remove the partition with offset in level lvl.
    partInfo* curr = hmi.A[lvl];
    if (curr -> offset == offset) {
        hmi.A[lvl] = curr -> nextPart;
        return;
    }
    partInfo* before = curr;
    curr = curr -> nextPart;
    while (curr != NULL) {
        if (curr -> offset == offset) {
            before -> nextPart = curr -> nextPart;
        }
        before = curr;
        curr = curr -> nextPart;
    }
}

int getMinOffset(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

void freeMemory(int lvl, int offset) {
    // The idea is to check whether the offset has a buddy in this level.
    // If yes, remove both of them and add a new bigger partition to higher level and call recursion.
    // If no, add this free partition in current level.
    // If it has been added before, finish and return nothing.
    int buddyOffset = buddyOf((unsigned int) hmi.base + offset, lvl) - (unsigned int) hmi.base;
    
    if (checkPartitionAtLevel(lvl, buddyOffset) == 0) {
        if (checkPartitionAtLevel(lvl, offset) == 0) {
            addPartitionAtLevel(lvl, offset);
        }
        return;
    }

    int minOffset = getMinOffset(offset, buddyOffset);
    removePartitionAtLevelWithOffset(lvl, offset);
    removePartitionAtLevelWithOffset(lvl, buddyOffset);
    addPartitionAtLevel(lvl + 1, minOffset);
    freeMemory(lvl + 1, minOffset);
    
}

void myfree(void* address, int size)
/**********************************************************
 * Mimic the normal "free()":
 *    Attempt to free a previously allocated memory space
 *********************************************************/
{
    //TODO: Task 3. Implement the de allocation using buddy allocator
    int lvl = log2Ceiling(size);
    int offset = address - hmi.base;
    freeMemory(lvl, offset);
    hmi.internalFragTotal = hmi.internalFragTotal - (power(2, lvl) - size);

}
