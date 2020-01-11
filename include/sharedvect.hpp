/**
 * \file sharedvector.hpp
 * \author Matteo Medioli
 */

#ifndef SHAREDVECTOR_H
#define SHAREDVECTOR_H
#include <iostream>
#include<vector>
#include <mutex>
#include <condition_variable>

template <typename T>
class SuperStep;

template<typename T>
class SharedVector
{
    private:
        std::vector<T> vector;
        std::mutex vector_mutex;
    public:
        SharedVector();
        SharedVector(std::vector<T>);
        ~SharedVector();
        void append(std::vector<T>);
        std::vector<T>& get_vector();
};


#endif