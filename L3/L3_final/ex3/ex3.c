/*************************************
* Lab 3 Exercise 3
* Name: Xia Liyi
* Student Id: A0177873L
* Lab Group: B19
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/*************************************
* Lab 3 Exercise 3
* Name: Zhang Yunjie
* Student Id: A0204208U
* Lab Group: B19
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/************************************

You should use ex2 solution as the starting point.

Copy over the solution and modify as needed.
************************************/

#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_init(&(lock->reader_mutex), NULL);
  pthread_mutex_init(&(lock->writer_mutex), NULL);
  pthread_mutex_init(&(lock->mutex), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;

}

void writer_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->reader_mutex));
  pthread_mutex_lock(&(lock->writer_mutex));
  //pthread_mutex_lock(&(lock->reader_mutex));
  lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
  //TODO: modify as needed
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->writer_mutex)); 
  pthread_mutex_unlock(&(lock->reader_mutex));
  
}

void reader_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->reader_mutex));
  pthread_mutex_unlock(&(lock->reader_mutex));
  pthread_mutex_lock(&(lock->mutex));
  if (lock->reader_count == 0) {
    pthread_mutex_lock(&(lock->writer_mutex));
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->mutex));
  
}

void reader_release(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->mutex));
  lock->reader_count--;
  
  if (lock->reader_count == 0) {
    pthread_mutex_unlock(&(lock->writer_mutex)); 
  }
  pthread_mutex_unlock(&(lock->mutex));
  
}

void cleanup(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_destroy(&(lock->writer_mutex));
  pthread_mutex_destroy(&(lock->reader_mutex));
  pthread_mutex_destroy(&(lock->mutex));
}
