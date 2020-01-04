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
    ss=s;
    ss_input=s->get_input();
}

template<typename T, typename F, typename ...Args>
Worker<T,F,Args...>::Worker(Worker<T,F,Args...> &&other)
    :   thread(std::move(other.thread)),
        id(std::move(other.id)),
        body(std::move(other.body)),
        input(std::move(other.input)),
        ss(std::move(other.ss)),
        ss_input(std::move(other.ss_input))
{}

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
void Worker<T,F,Args...>::work(bool chunk)
{
    thread = std::thread{[this,chunk]()
        { 
            //Prepare input of worker: chunk or full
            if(chunk)
            {
                // CHUNCK DISTRIBUTION
                int delta = ss_input.size()/ss->get_parallel_degree();
                auto first = ss_input.begin() + id*delta;
                auto last = ss_input.begin() + ((id+1)*delta);
                std::vector<T> c(first,last);
                input=c;
            }
            else
                input=ss_input;

            //WORK ON INPUT (CHUNKED OR NOT)
            body(&input);
        }
    };
}
