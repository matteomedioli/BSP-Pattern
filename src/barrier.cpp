/**
 * \file barrier.hpp
 * \author Matteo Medioli
 */

#include "../include/barrier.hpp"

Barrier::Barrier()
{
    active_workers=0;
}

Barrier::~Barrier()
{
    cv_barrier.notify_all ();
}

int Barrier::increase()
{
    active_workers+=1;
    return active_workers;
}

int Barrier::decrease()
{
    std::unique_lock<std::mutex> lock(mutex_barrier);
	if (active_workers == 0)
        return 0;

    active_workers-=1;
	if (active_workers == 0)
        cv_barrier.notify_all();
	else
		cv_barrier.wait(lock);

    return active_workers;
}
