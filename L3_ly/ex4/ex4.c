/*************************************
* Lab 3 Exercise 4
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from 
// ex4_driver.c in your code.
extern int num_of_cars;
extern int num_of_segments;
sem_t *mutex;
sem_t sync; // This is to control the number of cars in the roundabout.


void initialise()
{
    //TODO: Your code here
    sem_init(&sync, 0, num_of_segments - 1);
    mutex = malloc(sizeof(sem_t) * num_of_segments);
    for (int i = 0; i < num_of_segments; i++) {
        sem_init(&mutex[i], 0, 1);
    }

}

void cleanup()
{
    //TODO: Your code here
    for (int i = 0; i < num_of_segments; i++) {
        sem_destroy(&mutex[i]);
    }
    sem_destroy(&sync);
    free(mutex);
}

void* car(void* car)
{
    //TODO: Your code here, see the general steps below

    //This function modeles a thread 
    //A car: 
    //   1. should call enter_roundabout (...)
    //   2. followed by some calls to move_to_next_segment (...)
    //   3. finally call exit_roundabout (...)
    sem_wait(&sync);
    car_struct* self = (car_struct *) car;

    sem_wait(&mutex[self->entry_seg]);
    enter_roundabout(self);

    while (self->current_seg != self->exit_seg) {
        int next_pos = NEXT(self->current_seg, num_of_segments);
        sem_wait(&mutex[next_pos]);
        move_to_next_segment(self);
        int prev_pos = PREV(self->current_seg, num_of_segments);
        sem_post(&mutex[prev_pos]);
    }

    exit_roundabout(self);
    sem_post(&mutex[self->exit_seg]);
    sem_post(&sync);
    pthread_exit(NULL);
}
