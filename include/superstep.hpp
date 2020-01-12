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
#include "sharedvect.hpp"
#include "utimer.hpp"

template <typename T>
class Worker;

template <typename T>
class SharedVector;

template <typename T>
class SuperStep {
    private:
        int nw;
        std::vector<T> input;
        std::vector<std::unique_ptr<Worker<T>>> workers;     //vector of pointers
        std::vector<std::shared_ptr<SharedVector<T>>> output;
        std::shared_ptr<Barrier> barrier;


    public:
        SuperStep(int n, std::vector<T> input);
        ~SuperStep();
        std::vector<T> get_input();
        std::shared_ptr<Barrier>& get_barrier();
        std::shared_ptr<SharedVector<T>>& get_output(int i);
        int get_parallel_degree();
        template<typename F,typename ...Args>
        int computation(std::function<F(Args...)> b, bool chunk);
        template<typename F,typename ...Args>
        void communication(std::function<F(Args...)> b, std::vector<std::pair<int,std::vector<int>>> protocol);
        void set_barrier(std::shared_ptr<Barrier> b);
        void sync();
        std::vector<T> get_results();
};

#endif
