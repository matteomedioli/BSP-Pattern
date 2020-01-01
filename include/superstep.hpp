/**
 * \file superstep.cpp
 * \author Matteo Medioli
 */

#ifndef SUPERSTEP_H
#define SUPERSTEP_H

#include <functional>
#include <vector>
#include <thread>

template <typename T>
class SuperStep {
    private:
        int nw;
        std::vector<std::vector<T>> chunks;
        std::vector<std::thread> workers;
        std::vector<int> destination_index;
        std::vector<std::pair<int, std::vector<T>>> output;

    public:
        SuperStep(int n, std::vector<int> destinations);
        ~SuperStep();
        template<typename F, typename ... Args>
        int computation(std::function<F(Args...)> body);

};

#endif
