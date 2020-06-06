#ifndef BARRIER_H_
#define BARRIER_H_

#include <semaphore.h>

class Barrier {
public:
    Barrier(unsigned int num_of_threads);
    void wait();
    ~Barrier();

    ///just for test
    unsigned int waitingThreads();

    sem_t mutex;
    sem_t sem1;
    sem_t sem2;
    unsigned int threadCounter;
    unsigned int currThreads;
};

#endif // BARRIER_H_
