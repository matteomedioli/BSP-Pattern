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
}

template<typename T, typename F, typename ...Args>
Worker<T,F,Args...>::Worker(Worker<T,F,Args...> &&other)
    :   thread(std::move(other.thread)),
        id(std::move(other.id)),
        body(std::move(other.body)),
        input(std::move(other.input))
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
            if(chunk)
            {
                // CHUNCK DISTRIBUTION
                int delta = ss->get_input().size()/ss->get_parallel_degree();
                auto first = ss->get_input().begin() + id*delta;
                auto last = ss->get_input().begin() + (id+1)*delta;
                std::vector<T> c(first,last);
                input=c;
            }
            else
            {
                auto first = ss->get_input().begin();
                auto last = ss->get_input().end();
                std::vector<T> c(first,last);
                input=c;
            }

            //WORK ON INPUT (CHUNKED OR NOT)
            body(&input);
            {
                std::unique_lock<std::mutex> lock(ss->global_mutex);
                std::cout<<"WORKER "<<std::this_thread::get_id()<<std::endl;
                for(T i:input)
                    std::cout<<i<<" ";
                std::cout<<std::endl;

            }  
        }
    };
}
