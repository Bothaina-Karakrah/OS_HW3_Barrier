#include "Barrier.h"
#include <semaphore.h>

Barrier::Barrier(unsigned int num_of_threads): threadCounter(num_of_threads), currThreads(0){
    sem_init(&mutex, 0, 1);
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
}

void Barrier::wait() {
    sem_wait(&mutex);
    currThreads++;
    if(currThreads == threadCounter){
        for (unsigned int i = 0; i < threadCounter; ++i) {
            sem_post(&sem1);
        }
    }
    sem_post(&mutex);
    sem_wait(&sem1);


    sem_wait(&mutex);
    currThreads--;
    if(currThreads == 0){
        for (unsigned int i = 0; i < threadCounter; ++i) {
            sem_post(&sem2);
        }
    }
    sem_post(&mutex);
    sem_wait(&sem2);
    return;
}


Barrier::~Barrier() {
    sem_destroy(&mutex);
    sem_destroy(&sem1);
    sem_destroy(&sem2);
}


/////fot test
unsigned int Barrier::waitingThreads() {
    return this->currThreads;
}
