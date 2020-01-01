/**
 * \file worker.cpp
 * \author Matteo Medioli
 */
#include "../include/worker.hpp"

template<typename T, typename F, typename ...Args>
Worker<T,F,Args...>::Worker(int i, std::function<F(Args...)> b)
{
    id=i;
    body=b;
    std::cout<<"Init WORKER "<<i<<std::endl;
}

template<typename T, typename F, typename ...Args>
Worker<T,F,Args...>::~Worker()
{
    std::cout<<"Destroy Worker"<<std::endl;
}