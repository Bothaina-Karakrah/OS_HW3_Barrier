#include "Barrier.h"
#include <semaphore.h>

Barrier::Barrier(unsigned int NumThreads): threadsNum(NumThreads), currCounter(0), state(true) {
    sem_init(&mutex, 0, 1);
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
}

void Barrier::wait() {
    if(this->state){
        sem_wait(&this->mutex);
        this->currCounter++;
        if(this->currCounter == this->threadsNum){
            for (int i = 0; i < this->threadsNum; ++i) {
                sem_post(&sem1);
                this->state = false;
            }
        }
        sem_post(&mutex);
        sem_wait(&sem1);
    }
    else if(!this->state){
        sem_wait(&mutex);
        this->currCounter--;
        if(this->currCounter == 0){
            for (int i = 0; i < this->threadsNum; ++i) {
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
