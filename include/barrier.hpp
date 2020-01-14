/**
 * \file barrier.hpp
 * \author Matteo Medioli
 */

#ifndef BARRIER_H
#define BARRIER_H
#include <iostream>
#include <cassert>
#include <mutex>
#include <condition_variable>
#include <atomic>

class Barrier
{
    private:
        std::atomic_int	active_workers;
        std::condition_variable barrier_cv ;
        std::mutex barrier_mutex;
    public:
        Barrier();
        ~Barrier() noexcept;
        void wait();
        void reset(int n);
        int get_active_workers();
};

#endif