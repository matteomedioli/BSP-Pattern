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


template<typename T, typename = typename T::value_type>
T flatten(const std::vector<T>& v) {
    return std::accumulate(v.begin(), v.end(), T{}, [](auto& dest, auto& src) {
        dest.insert(dest.end(), src.begin(), src.end());
        return dest;
    });
}

template <typename T>
class SuperStep {
    private:
        int nw;
        std::vector<std::vector<T>> input;
        std::vector<std::unique_ptr<Worker<T>>> workers;     //vector of pointers
        Barrier barrier;
        std::vector<std::shared_ptr<SharedVector<T>>> output;


    public:
        SuperStep(int n, std::vector<T> input, bool chunk);
        SuperStep(int n, std::vector<std::vector<T>> input, bool merge);
        ~SuperStep();
        void reset_barrier();
        void sync();
        std::vector<std::vector<T>>& get_results(std::vector<std::vector<T>>&);
    template<typename F,typename ...Args>
        int computation(std::function<F(Args...)> b);
    template<typename F,typename ...Args>
        void communication(std::function<F(Args...)> b, CommunicationProtocol protocol);
    
};

template<typename T>
SuperStep<T>::SuperStep(int n, std::vector<T> data, bool chunk): nw(n)
{
    workers.reserve(nw);
    input.reserve(nw);
    for (int id=0; id<n; id++)
    {
        if(chunk)
        {
            int delta = data.size()/nw;
            auto first = data.begin() + id*delta;
            auto last = data.begin() + ((id+1)*delta);
            std::vector<int> chunk(first,last);
            input[id]=chunk;
        }
        else
        {
            input[id]=data;
        }
        std::unique_ptr<Worker<T>> w_ptr(new Worker<T>(id,input[id]));
        workers.emplace_back(std::move(w_ptr));
        std::unique_ptr<SharedVector<T>> sh(new SharedVector<T>(id));
        output.emplace_back(std::move(sh));
    }
}

template<typename T>
SuperStep<T>::SuperStep(int n, std::vector<std::vector<T>> data, bool merge) : nw(n)
{
    workers.reserve(nw);
    input.reserve(nw);
    for (int id=0; id<n; id++)
    {
        if(merge)
        {
            input.insert(input.begin()+id, flatten(data));
        }
        else
        {
            input.insert(input.begin()+id, data[id]);
        }
        std::unique_ptr<Worker<T>> w_ptr(new Worker<T>(id,input[id]));
        workers.emplace_back(std::move(w_ptr));
        std::unique_ptr<SharedVector<T>> sh(new SharedVector<T>(id));
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
int SuperStep<T>::computation(std::function<F(Args...)> body)
{
    for (int id=0; id<nw; id++)
    {   
        workers[id]->work(nw,body,&barrier);
    }
    sync();
}

template<typename T>
template<typename F,typename ...Args>
void SuperStep<T>::communication(std::function<F(Args...)> body, CommunicationProtocol protocol)
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
std::vector<std::vector<T>>& SuperStep<T>::get_results(std::vector<std::vector<T>>& results)
{
    results.clear();
    results.reserve(nw);
    for(int id=0; id<nw; id++)
        results.insert(results.begin()+id, output[id]->get_vector());
    return results;
}

#endif
