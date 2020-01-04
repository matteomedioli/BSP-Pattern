/**
 * \file superstep.cpp
 * \author Matteo Medioli
 */
#include "../include/superstep.hpp"

template<typename T, typename F, typename ...Args>
SuperStep<T,F,Args...>::SuperStep(int n, std::vector<T> data)
{
    nw=n;
    input=data;
    workers.reserve(nw);
    for (int i=0; i<n; i++)
    {
        std::unique_ptr<Worker<T,F,Args...>> w_ptr(new Worker<T,F,Args...>(i,this));
        workers.push_back(std::move(w_ptr));
    }
}

template<typename T, typename F, typename ...Args>
SuperStep<T,F,Args...>::~SuperStep()
{
}

template<typename T, typename F, typename ...Args>
std::vector<T> SuperStep<T,F,Args...>::get_input()
{
    return input;
}

template<typename T, typename F, typename ...Args>
int SuperStep<T,F,Args...>::get_parallel_degree()
{
    return nw;
}

template<typename T, typename F, typename ...Args>
void SuperStep<T,F,Args...>::setThreadBody(std::function<F(Args...)> b) 
{
    for (auto &w:workers)
    {
        w->setThreadBody(b);
    }

}

template<typename T, typename F, typename ...Args>
int SuperStep<T,F,Args...>::computation(bool chunk)
{
    for (auto &w: workers)
        w->work(chunk);
}


