/**
 * \file barrier.hpp
 * \author Matteo Medioli
 */

#include "../include/barrier.hpp"


Barrier::Barrier(int workers, std::string n):active_workers(workers),name(n)
{
    assert(0 != active_workers);
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

std::string Barrier::get_name()
{
    return name;
}
