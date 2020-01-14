/**
 * \file superstep.hpp
 * \author Matteo Medioli
 */

#ifndef SUPERSTEP_H
#define SUPERSTEP_H

#include <functional>
#include <vector>
#include <mutex>
#include <iostream>
#include "worker.hpp"
#include "utimer.hpp"


template <typename T>
class SuperStep {
    private:
        int nw;
        std::vector<T> input;
        std::vector<std::unique_ptr<Worker<T>>> workers;     //vector of pointers
        Barrier barrier;
        std::vector<std::shared_ptr<SharedVector<T>>> output;


    public:
        SuperStep(int n, std::vector<T> input);
        ~SuperStep();
        template<typename F,typename ...Args>
        int computation(std::function<F(Args...)> b, bool chunk);
        template<typename F,typename ...Args>
        void communication(std::function<F(Args...)> b, std::vector<std::pair<int,std::vector<int>>> protocol);
        void reset_barrier();
        void sync();
        std::vector<T> get_results();
};

template<typename T>
SuperStep<T>::SuperStep(int n, std::vector<T> data): nw(n), input(data)
{
    Utimer t("T_GEN_SUPERSTEP: ");
    workers.reserve(nw);

    for (int i=0; i<n; i++)
    {
        std::unique_ptr<Worker<T>> w_ptr(new Worker<T>(i,input));
        workers.emplace_back(std::move(w_ptr));
        std::unique_ptr<SharedVector<T>> sh(new SharedVector<T>(i));
        output.emplace_back(std::move(sh));
    }
}

template<typename T>
SuperStep<T>::~SuperStep()
{}


template<typename T>
void SuperStep<T>::reset_barrier()
{
    barrier.reset(nw+1);
}

template<typename T>
template<typename F,typename ...Args>
int SuperStep<T>::computation(std::function<F(Args...)> body, bool chunk)
{
    for (int id=0; id<nw; id++)
    {   
        workers[id]->work(nw,body,chunk,&barrier);
    }
    sync();
}

template<typename T>
template<typename F,typename ...Args>
void SuperStep<T>::communication(std::function<F(Args...)> body, std::vector<std::pair<int,std::vector<int>>> protocol)
{
    for (int id=0; id<nw; id++)
    {
       workers[id]->send(body,protocol,&barrier,output);
    }
    sync();
}

template<typename T>
void SuperStep<T>::sync()
{
    barrier.wait();
    for (auto &w: workers)
    {   
        w->join();
    }
}


template<typename T>
std::vector<T> SuperStep<T>::get_results()
{
    std::vector<T> result;
    for(int i=0; i<nw; i++)
        result.insert(std::end(result), std::begin(output[i]->get_vector()), std::end(output[i]->get_vector()));
    return result;
}

#endif
