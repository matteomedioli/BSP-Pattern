/**
 * \file superstep.hpp
 * \author Matteo Medioli
 */

#ifndef SUPERSTEP_H
#define SUPERSTEP_H

#include <functional>
#include <vector>
#include <iostream>
#include "../include/worker.hpp"

template <typename T, typename F, typename ... Args>
class SuperStep {
    private:
        int nw;
        std::vector<T> input;
        std::vector<Worker<T,F,Args...>*> workers;              //vector of pointers
        std::vector<std::pair<int, std::vector<T>>> output;

    public:

        SuperStep(int n, std::function<F(Args...)> body);
        ~SuperStep();
        int computation();

};

#endif
