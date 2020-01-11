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
    output.reserve(nw);
    for (int i=0; i<n; i++)
    {
        std::unique_ptr<Worker<T>> w_ptr(new Worker<T>(i,this));
        workers.emplace_back(std::move(w_ptr));
        std::shared_ptr<SharedVector<T>> sh(new SharedVector<T>());
        output.emplace_back(std::move(sh));
    }
}

template<typename T>
SuperStep<T>::~SuperStep()
{}

template<typename T>
std::vector<T> SuperStep<T>::get_input()
{
    return input;
}

template<typename T>
void SuperStep<T>::set_barrier(std::shared_ptr<Barrier> b)
{
    
    barrier=b;
}

template<typename T>
std::shared_ptr<SharedVector<T>>& SuperStep<T>::get_output(int i)
{
    return output[i];
}

template<typename T>
std::shared_ptr<Barrier>& SuperStep<T>::get_barrier()
{
    return barrier;
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
void SuperStep<T>::communication(std::function<F(Args...)> body, std::vector<std::pair<int,std::vector<int>>> protocol)
{
    for (auto &w: workers)
        w->send(body, protocol);
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


