/*************************************
* Lab 4 Exercise 2
* Name: Xia Liyi
* Student Id: A0177873L
* Lab Group: B19
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/*************************************
* Lab 4 Exercise 2
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

int setupHeap(int initialSize, int minPartSize, int maxPartSize)
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

    hmi.internalFragTotal = 0;
	
    //TODO: Task 1. Setup the rest of the bookkeeping info:
    //       hmi.A <= an array of partition linked list
    //       hmi.maxIdx <= the largest index for hmi.A[]
    //       

    int maxLevel = log2Ceiling(maxPartSize);
    int minLevel = log2Floor(minPartSize);
    hmi.minIdx = minLevel;

    int numOfLevels = log2Floor(initialSize);
    int totalSize = 0;


    if (numOfLevels > maxLevel) {
            numOfLevels = maxLevel;
    }

    if (numOfLevels < minLevel) {
            numOfLevels = minLevel;
    }

    hmi.maxIdx = numOfLevels;


    hmi.A = (partInfo**) malloc((numOfLevels + 1) * sizeof(partInfo*));
    for (int i = 0; i < numOfLevels + 1; i++) {
        hmi.A[i] = NULL;
    }

    hmi.A[numOfLevels] = buildPartitionInfo(0);
    totalSize += power(2,numOfLevels);
    
    if (initialSize > power(2,numOfLevels)) {  //non-power-of-2 memory
        int diff = initialSize - power(2,numOfLevels);
        int level;
        int offset = power(2,numOfLevels);
        
            while (diff >= minPartSize) {
                    level = log2Floor(diff);                    
                    if (level > maxLevel) {
                            level = maxLevel;
                    }
 
                    if (hmi.A[level] == NULL) {
                        hmi.A[level] = buildPartitionInfo(offset);
                    } else {
                        partInfo *curr = hmi.A[level];
                        while (curr->nextPart != NULL) {
                            curr = curr->nextPart;
                        }
                        curr->nextPart = buildPartitionInfo(offset);
                    }
                    totalSize += power(2,level);
                    offset += power(2,level);
                    diff -= power(2,level);
            }
    }
    hmi.totalSize = totalSize;
    
    return 1;
}


void allocateMemory(int lvl) {
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

    int minLevel = hmi.minIdx;
    int maxLevel = hmi.maxIdx;
    int minPartSize = power(2,minLevel);
    int maxPartSize = power(2,maxLevel);
    int lvl;

    if (size < minPartSize) {
        lvl = minLevel;
    } else if (size > maxPartSize) {
        return NULL;
    } else {
        lvl = log2Ceiling(size);
    }

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
    int minLevel = hmi.minIdx;
    int maxLevel = hmi.maxIdx;
    int minPartSize = power(2,minLevel);
    int maxPartSize = power(2,maxLevel);
    int lvl;

    if (size < minPartSize) {
        lvl = minLevel;
    } else if (size > maxPartSize) {
        
    } else {
        lvl = log2Ceiling(size);
    }
    
    int offset = address - hmi.base;
    freeMemory(lvl, offset);
    hmi.internalFragTotal = hmi.internalFragTotal - (power(2, lvl) - size);

}
