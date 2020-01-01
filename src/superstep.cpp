#include "../include/superstep.hpp"
#include<iostream>

template<typename T>

SuperStep<T>::SuperStep(int n, std::vector<int> destinations)
{
    std::cout<<"Init Superstep"<<std::endl;
    nw=n;
    destination_index=destinations;
    chunks.reserve(nw);
    workers.reserve(nw);
    output.reserve(nw);
}

template<typename T>
SuperStep<T>::~SuperStep()
{}

template<typename T>
template <typename F, typename ...Args>
int SuperStep<T>::computation(std::function<F(Args...)> body)
{
    body();
}
