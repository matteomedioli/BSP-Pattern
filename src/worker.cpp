/**
 * \file worker.cpp
 * \author Matteo Medioli
 */

#include "../include/worker.hpp"
#include "../include/superstep.hpp"

template<typename T>
Worker<T>::Worker(int i, SuperStep<T> *s)
{
    id=i;
    ss=s;
    ss_input=s->get_input();
}

template<typename T>
Worker<T>::Worker(Worker<T> &&other)
    :   thread(std::move(other.thread)),
        id(std::move(other.id)),
        input(std::move(other.input)),
        ss(std::move(other.ss)),
        ss_input(std::move(other.ss_input))
{}

template<typename T>
Worker<T>::~Worker()
{
    if(thread.joinable())
        thread.join();
}

template<typename T>
int Worker<T>::get_id() 
{
    return id;    
}


template<typename T>
template<typename F, typename ...Args>
void Worker<T>::work(std::function<F(Args...)> body, bool chunk)
{
    thread = std::thread{[this,body,chunk]()
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
            output = body(input);
            
            //PRINT BLOCKS
            /*
            {
          
                std::unique_lock<std::mutex> lock(ss->ss_mutex);
                std::cout<<"WORKER"<<id<<" input:";
                for(T i:input)
                    std::cout<<i<<" ";
                std::cout<<std::endl;

                std::cout<<"WORKER"<<id<<" output:";
                for(T i:output)
                    std::cout<<i<<" ";
                std::cout<<std::endl<<std::endl;
            }
            */
        }
    };
}
