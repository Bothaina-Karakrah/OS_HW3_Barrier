#ifndef BARRIER_H_
#define BARRIER_H_

#include <semaphore.h>

class Barrier {
public:
    Barrier(unsigned int num_of_threads);
    void wait();
    ~Barrier();

	// TODO: define the member variables
	// Remember: you can only use semaphores!
    sem_t mutex;
    sem_t sem1;
    sem_t sem2;
    unsigned int threadsNum;
    unsigned int currCounter;
    bool state;
};

#endif // BARRIER_H_
