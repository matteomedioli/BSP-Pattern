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
#include <string>

class Barrier
{
    private:
        std::string name;
        std::atomic_int	active_workers;
        std::condition_variable barrier_cv ;
        std::mutex barrier_mutex;
    public:
        Barrier(int workers);
        ~Barrier() noexcept;
        void wait();
};

#endif