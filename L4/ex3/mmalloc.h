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

#ifndef __MMALLOC_H_	//standard protection against multiple includes
#define __MMALLOC_H_

#include <unistd.h>	//for brk, sbrk

#define FREE 0
#define OCCUPIED 1

/**********************************************************
 * You may need to change the partInfo and / or heapMetaInfo 
 *   structure IF NEEDED
 * We have added changes that should be sufficient for your 
 *   buddy system implementation
 * SUBMIT this file regardless of whether you modify it to
 *   ease the marking process
 *********************************************************/

typedef struct PARTINFO {
    unsigned int offset;
	struct PARTINFO *nextPart;

    //Buddy system's partition has an implicit partition size
	//int	size;

    //Buddy system's partition is implicitly free
	//int status;
} partInfo;

typedef struct {
	int totalSize;
    void* base;
	partInfo** A; //The array of partition lists in buddy system
    int maxIdx;   //For convenience. Keep the max index for A[].
    int internalFragTotal;  //Keep track of internal fragmentation.
} heapMetaInfo;


/**********************************************************
 * Do NOT modify any of the following functions as they 
 *   are used by the test drivers.
 *********************************************************/
int setupHeap(int);
void printHeapMetaInfo();
void printHeapStatistic();
void printHeap();

void* mymalloc(int);
//For simplicity, the free() takes in the size of partition
void myfree(void*, int);

#endif
