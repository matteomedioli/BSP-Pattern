/**
 * \file barrier.hpp
 * \author Matteo Medioli
 */

#ifndef BARRIER_H
#define BARRIER_H

#include <mutex>
#include <condition_variable>
#include <atomic>

class Barrier
{
    private:
        std::atomic_int	active_workers;
        std::mutex mutex_barrier;
        std::condition_variable cv_barrier ;

    public:
        Barrier();
        ~Barrier ();
        int increase();
        int decrease();
};

#endif