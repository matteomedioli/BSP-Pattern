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
        int id;
        std::vector<T> vector;
        std::mutex vector_mutex;
    public:
        SharedVector(int);
        SharedVector(std::vector<T>);
        ~SharedVector();
        int get_id();
        void append(std::vector<T>);
        std::vector<T>& get_vector();
};

template<typename T>
SharedVector<T>::SharedVector(int id):vector(), id(id)
{}

template<typename T>
SharedVector<T>::SharedVector(std::vector<T> v):vector(v)
{}

template<typename T>
SharedVector<T>::~SharedVector()
{
}

template<typename T>
int SharedVector<T>::get_id()
{
    return id;
}

template<typename T>
void SharedVector<T>::append(std::vector<T> v)
{
    std::unique_lock<std::mutex> lock (vector_mutex);
    //TODO ERROR: QUI ARRIVA UNA COPIA DI V, CERCA GLI ITERATOR DELA COPIA E FA ANDARE TUTTO A TROIE
    //TODO: PASSARE BY REFRENCE VETTORE DI RITORNO DELLA SEND
    vector.insert(std::end(vector), std::begin(v), std::end(v));
}

template<typename T>
std::vector<T>& SharedVector<T>::get_vector()
{
    return vector;
}

#endif