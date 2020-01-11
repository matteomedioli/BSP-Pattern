/**
 * \file shared.cpp
 * \author Matteo Medioli
 */
#include "../include/sharedvect.hpp"

template<typename T>
SharedVector<T>::SharedVector():vector()
{}

template<typename T>
SharedVector<T>::SharedVector(std::vector<T> v):vector(v)
{}

template<typename T>
SharedVector<T>::~SharedVector()
{
}

template<typename T>
void SharedVector<T>::append(std::vector<T> v)
{
    std::unique_lock<std::mutex> lock (vector_mutex);
    vector.insert(std::end(vector), std::begin(v), std::end(v));
}

template<typename T>
std::vector<T>& SharedVector<T>::get_vector()
{
    return vector;
}