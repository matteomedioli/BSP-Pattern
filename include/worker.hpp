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
        SuperStep<T,F,Args...> *ss;
        std::thread thread;
        std::function<F(Args...)> body;
        std::vector<T> input;
        std::vector<T> output;


    public:
        Worker(int id, SuperStep<T,F,Args...> *s);
        Worker(const Worker&) = delete;
        Worker(Worker &&other);
        Worker& operator=(const Worker&) = delete; 
        Worker& operator=(Worker&&) = delete;
        ~Worker();
        int get_id();
        void setThreadBody(std::function<F(Args...)> body);
        void work(bool chunk);
};


#endif