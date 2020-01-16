/**
 * \file barrier.cpp
 * \author Matteo Medioli
 */

#include "../include/barrier.hpp"


Barrier::Barrier()
{
}


Barrier::~Barrier() noexcept
{
    barrier_cv.notify_all();
    active_workers=0;
}


void Barrier::wait()
{
    std::unique_lock< std::mutex > lock(barrier_mutex);
    assert(0 != active_workers);
    if (0 == --active_workers)
    {
        barrier_cv.notify_all();
    }
    else
    {
        barrier_cv.wait(lock, [this]() { return 0 == active_workers; });
    }
    
}

void Barrier::reset(int n)
{
    active_workers=n;
    barrier_mutex.unlock();
    barrier_cv.notify_all();
}


int Barrier::get_active_workers()
{
    return int(active_workers);
}