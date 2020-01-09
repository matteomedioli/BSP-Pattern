/**
 * \file worker.cpp
 * \author Matteo Medioli
 */

#include "../include/worker.hpp"

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
    join();
}

template<typename T>
void Worker<T>::join()
{
    if(thread.joinable())
    {
        thread.join();
    }
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
            if(input[0]==10)
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            output = body(input);
        }
    };

}

template<typename T>
template<typename F,typename ...Args>
void Worker<T>::send(std::function<F(Args...)> body) //ALREADY OPEN COMPUTATION THREAD. TO KILL COMPUTATION THREAD BEFORE START COMMUNICATION THREAD
{
    thread = std::thread{[this,body]()
        { 
            body(output);
        }
    };
}

