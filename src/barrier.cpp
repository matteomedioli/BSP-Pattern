/**
 * \file barrier.hpp
 * \author Matteo Medioli
 */

#include "../include/barrier.hpp"


Barrier::Barrier(int workers):active_workers(workers)
{
    assert(0 != active_workers);
}

Barrier::~Barrier() noexcept
{
    barrier_cv.notify_all();
    assert(0 == active_workers);
}


void Barrier::wait()
{
    std::unique_lock< std::mutex > lock(barrier_mutex);
    assert(0 != active_workers);
    if (0 == --active_workers)
    {
        std::cout<<"DELETE LAST"<<std::endl;
        std::cout<<"RELEASE"<<std::endl;
        barrier_cv.notify_all();
    }
    else
    {
        std::cout<<"DELETE & WAIT"<<std::endl;
        barrier_cv.wait(lock, [this]() { return 0 == active_workers; });
    }
}
