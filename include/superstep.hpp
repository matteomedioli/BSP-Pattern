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
#include "barrier.hpp"
#include "../include/utimer.hpp"

template <typename T>
class Worker;

template <typename T>
class SuperStep {
    private:
        int nw;
        std::vector<T> input;
        std::vector<std::unique_ptr<Worker<T>>> workers;     //vector of pointers
        std::vector<std::vector<T>> output;
        std::unique_ptr<Barrier> barrier;


    public:
        SuperStep(int n, std::vector<T> input);
        ~SuperStep();
        std::vector<T> get_input();
        std::vector<std::vector<T>>& get_output();
        int get_parallel_degree();
        template<typename F,typename ...Args>
        int computation(std::function<F(Args...)> b, bool chunk);
        template<typename F,typename ...Args>
        void communication(std::function<F(Args...)> b, std::vector<std::pair<int,int>> protocol);
        void set_barrier(Barrier *b);
        void sync();      
        std::mutex output_mutex;  
};

#endif
