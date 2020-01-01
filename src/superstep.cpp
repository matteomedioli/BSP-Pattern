/**
 * \file superstep.cpp
 * \author Matteo Medioli
 */
#include "../include/superstep.hpp"


template<typename T, typename F, typename ...Args>
SuperStep<T,F,Args...>::SuperStep(int n, std::function<F(Args...)> body)
{
    std::cout<<"Init Superstep"<<std::endl;
    nw=n;
    for (int i=0; i<n; i++)
    {
        Worker<int,void> * w = new Worker<int,void>(i,body);
        workers.insert(workers.begin()+i, w);
    }
    output.reserve(nw);

}

template<typename T, typename F, typename ...Args>
SuperStep<T,F,Args...>::~SuperStep()
{}

template<typename T, typename F, typename ...Args>
int SuperStep<T,F,Args...>::computation()
{

}
