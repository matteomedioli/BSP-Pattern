/**
 * \file superstep.cpp
 * \author Matteo Medioli
 */
#include "../include/superstep.hpp"

template<typename T>
SuperStep<T>::SuperStep(int n, std::vector<T> data)
{
    nw=n;
    input=data;
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
int SuperStep<T>::get_parallel_degree()
{
    return nw;
}

template<typename T>
template<typename F,typename ...Args>
int SuperStep<T>::computation(std::function<F(Args...)> f, bool chunk)
{
    for (auto &w: workers)
            w->work(f, chunk);
}


