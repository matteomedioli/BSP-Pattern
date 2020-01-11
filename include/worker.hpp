/**
 * \file worker.hpp
 * \author Matteo Medioli
 */

#ifndef WORKER_H
#define WORKER_H

#include <functional>
#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>
#include "superstep.hpp"
#include "barrier.hpp"

template <typename T>
class SuperStep;

template <typename T>
class Worker {
    private:
        int id;
        SuperStep<T> *ss;
        Barrier *barrier;
        std::vector<T> ss_input;
        std::thread thread;
        std::vector<T> input;
        std::vector<T> output;


    public:
        Worker(int id, SuperStep<T> *s);
        Worker(const Worker&) = delete;
        Worker(Worker &&other);
        Worker& operator=(const Worker&) = delete; 
        Worker& operator=(Worker&&) = delete;
        ~Worker();
        int get_id();
        void join();
        template<typename F,typename ...Args>
        void work(std::function<F(Args...)> body, bool chunk);
        template<typename F,typename ...Args>
        void send(std::function<F(Args...)> body, std::vector<std::pair<int,std::vector<int>>> protocol);
};


#endif