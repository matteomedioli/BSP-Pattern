/**
 * \file superstep.hpp
 * \author Matteo Medioli
 */

#ifndef SUPERSTEP_H
#define SUPERSTEP_H

#include <functional>
#include <vector>
#include <iostream>
#include "./worker.hpp"


template <typename T, typename F, typename ... Args>
class SuperStep {
    private:
        int nw;
        std::vector<T> input;
        std::vector<std::vector<T>> chunks;
        std::vector<Worker<T,F,Args...>*> workers;              //vector of pointers
        std::vector<std::pair<int, std::vector<T>>> output;

    public:
        SuperStep(int n, std::vector<T> input);
        ~SuperStep();
        int computation();
        std::vector<T> get_input();
        void setThreadBody(std::function<F(Args...)> body);
};

#endif
