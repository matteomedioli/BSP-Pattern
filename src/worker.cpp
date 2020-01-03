/**
 * \file worker.cpp
 * \author Matteo Medioli
 */

#include "../include/worker.hpp"
#include "../include/superstep.hpp"

template<typename T, typename F, typename ...Args>
Worker<T,F,Args...>::Worker(int i, SuperStep<T,F,Args...> *s)
{
    id=i;
    // CHUNCK DISTRIBUTION
    int delta = s->get_input().size()/s->get_parallel_degree();
    auto first = s->get_input().begin() + id*delta;
    auto last = s->get_input().begin() + (id+1)*delta;
    std::vector<T> c(first,last);
    chunk=c;
}

template<typename T, typename F, typename ...Args>
Worker<T,F,Args...>::~Worker()
{
    if(thread.joinable())
        thread.join();
}

template<typename T, typename F, typename ...Args>
int Worker<T,F,Args...>::get_id() 
{
    return id;    
}

template<typename T, typename F, typename ...Args>
void Worker<T,F,Args...>::setThreadBody(std::function<F(Args...)> b) 
{
    body=b; 
}

template<typename T, typename F, typename ...Args>
void Worker<T,F,Args...>::work()
{
     thread = std::thread{[this](){ this->body(); } };
     thread.detach();
}