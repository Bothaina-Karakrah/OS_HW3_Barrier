#include "Barrier.h"
#include <semaphore.h>

Barrier::Barrier(unsigned int num_of_threads): threadCounter(num_of_threads), currThreads(0), state(true) {
    sem_init(&mutex, 0, 1);
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
}

void Barrier::wait() {
    if(this->state){
        sem_wait(&this->mutex);
        this->currThreads++;
        if(this->currThreads == this->threadCounter){
            for (unsigned int i = 0; i < this->threadCounter; ++i) {
                sem_post(&sem1);
                this->state = false;
            }
        }
        sem_post(&mutex);
        sem_wait(&sem1);
    }
    else if(!this->state){
        sem_wait(&mutex);
        this->currThreads--;
        if(this->currThreads == 0){
            for (unsigned int i = 0; i < this->threadCounter; ++i) {
                sem_post(&sem2);
                this->state = true;
            }
        }
        sem_post(&mutex);
        sem_wait(&sem2);
    }
    return;
}


Barrier::~Barrier() {
    sem_destroy(&mutex);
    sem_destroy(&sem1);
    sem_destroy(&sem2);
}


/////fot test
unsigned int Barrier::waitingThreads() {
    if(!this->state){
        return (this->threadCounter - this->currThreads);
    }
    return this->currThreads;
}
