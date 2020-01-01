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
        Worker(int id, std::function<F(Args...)> body);
        ~Worker();

};


#endif