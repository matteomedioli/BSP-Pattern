/**
 * \file worker.cpp
 * \author Matteo Medioli
 */

#include "../include/worker.hpp"
#include "../include/superstep.hpp"

template<typename T, typename F, typename ...Args>
Worker<T,F,Args...>::Worker(int i, SuperStep<T,F,Args...> *s, int nw)
{
    id=i;
    // CHUNCK DISTRIBUTION
    int delta = s->get_input().size()/nw;
    auto first = s->get_input().begin() + id*delta;
    auto last = s->get_input().begin() + (id+1)*delta;
    std::vector<T> c(first,last);
    chunk=c;
}

template<typename T, typename F, typename ...Args>
Worker<T,F,Args...>::~Worker()
{
    std::cout<<"Destroy Worker"<<std::endl;
}

template<typename T, typename F, typename ...Args>
void Worker<T,F,Args...>::setThreadBody(std::function<F(Args...)> b) 
{
    body=b;
}

template<typename T, typename F, typename ...Args>
void Worker<T,F,Args...>::work()
{
    //thread(body);
}