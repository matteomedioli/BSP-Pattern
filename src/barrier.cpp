/**
 * \file barrier.hpp
 * \author Matteo Medioli
 */

#include "../include/barrier.hpp"


Barrier::Barrier()
{
}

Barrier::Barrier(int workers):active_workers(workers)
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
    std::cout<<"[BARRIER]: exec "<<4-(active_workers)<<" workers."<<std::endl;
    if (0 == --active_workers)
    {
        std::cout<<"[BARRIER]: reach "<<active_workers<<" workers, RELEASE ALL."<<std::endl;
        barrier_cv.notify_all();
    }
    else
    {
        std::cout<<"[BARRIER]: waiting "<<active_workers<<" workers."<<std::endl;
        barrier_cv.wait(lock, [this]() { return 0 == active_workers; });
    }
    
}

void Barrier::reset(int n)
{
    active_workers=n;
}


int Barrier::get_active_workers()
{
    return int(active_workers);
}