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
#include "./superstep.hpp"

template <typename T, typename F, typename ... Args>
class SuperStep;

template <typename T, typename F, typename ... Args>
class Worker {
    private:
        int id;
        std::thread thread;
        std::function<F(Args...)> body;
        std::vector<T> chunk;
        std::vector<T> sample;
        std::pair<T,T> range;

    public:
        Worker(int id, SuperStep<T,F,Args...> *s);
        Worker(const Worker& w);
        Worker& operator=(Worker&& w);
        ~Worker();
        int get_id();
        void setThreadBody(std::function<F(Args...)> body);
        void work();
};


#endif