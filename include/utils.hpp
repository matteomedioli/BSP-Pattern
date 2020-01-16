/**
 * \file utils.hpp
 * \author Matteo Medioli
 */

#ifndef UTILS_H
#define UTILS_H


#include <iterator>
#include <fstream>
#include <vector>
#include <random>
#include <numeric>
/* FLATTEN FUNCTION */
template<typename T, typename = typename T::value_type>
T flatten(const std::vector<T>& v) {
    return std::accumulate(v.begin(), v.end(), T{}, [](auto& dest, auto& src) {
        dest.insert(dest.end(), src.begin(), src.end());
        return dest;
    });
}

/* GENERATE RANDOM UNIQUE VECTOR OF SIZE N */
std::vector<int> generate_data(int n)
{   
    int num_values = n;
    std::vector<int> rand_values(num_values);
    std::mt19937 eng{std::random_device{}()};

    for(int i = 0; i < num_values; ++i) {
        rand_values[i] = i+1;
    }
    std::shuffle(rand_values.begin(), rand_values.end(), eng);
    return rand_values;
}

/* READ EXIST VECTOR FOR USE SAME VECTOR FOR A SPECIFIC RUN TEST
   OTHERWISE, GENERATE ONE */
std::vector<int> get_data(int n)
{
    std::vector<int> v;
    v.resize(n,1);
    auto file = std::ifstream("data/"+std::to_string(n)+".txt");
    std::string str;
    if (file)
    {
        //EXIST -> READ FROM FILE
        int i=0;
        while (std::getline(file, str))
        {
            v[i]=std::stoi(str);
            i++;
        }

    }
    else
    {
        //NOT EXIST -> GENERATE AND WRITE
        std::ofstream file("data/"+std::to_string(n)+".txt");
        v=generate_data(n);
        for(int i=0; i<n; i++)
            file<<v[i]<<"\n";
    }  
    return v; 
}

#endif