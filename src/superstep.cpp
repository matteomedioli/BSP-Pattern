/**
 * \file superstep.cpp
 * \author Matteo Medioli
 */
#include "../include/superstep.hpp"

template<typename T>
SuperStep<T>::SuperStep(int n, std::vector<T> data): nw(n), input(data)
{
    Utimer t("T_GEN_SUPERSTEP: ");
    workers.reserve(nw);
    for (int i=0; i<n; i++)
    {
        std::unique_ptr<Worker<T>> w_ptr(new Worker<T>(i,this));
        workers.emplace_back(std::move(w_ptr));
    }
}

template<typename T>
SuperStep<T>::~SuperStep()
{
}

template<typename T>
std::vector<T> SuperStep<T>::get_input()
{
    return input;
}

template<typename T>
void SuperStep<T>::set_barrier(Barrier* b)
{
    barrier.reset(b);
}

template<typename T>
std::vector<std::vector<T>> SuperStep<T>::get_output()
{
    return output;
}


template<typename T>
int SuperStep<T>::get_parallel_degree()
{
    return nw;
}

template<typename T>
template<typename F,typename ...Args>
int SuperStep<T>::computation(std::function<F(Args...)> body, bool chunk)
{
    for (auto &w: workers)
    {   
        w->work(body,chunk);
    }
}

template<typename T>
template<typename F,typename ...Args>
void SuperStep<T>::communication(std::function<F(Args...)> body)
{
    for (auto &w: workers)
            w->send(body);
}

template<typename T>
void SuperStep<T>::sync()
{
    barrier->wait();
    for (auto &w: workers)
    {   
        w->join();
    }
}
