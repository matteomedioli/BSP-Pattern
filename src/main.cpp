
#include <algorithm>
#include <random>
#include "../include/utimer.hpp"
#include "../include/superstep.hpp"
#include "superstep.cpp"
#include "../include/worker.hpp"
#include "worker.cpp"

std::vector<int> generate_data(int n)
{   
    Utimer t("RANDOM VECTOR GENERATION: ");
    std::vector<int> data;
    for(int i=0; i<n; i++)
    {   
        std::random_device random_device;
        std::mt19937 random_engine(random_device());
        std::uniform_int_distribution<int> distribution(0, n);
        auto const randomNumber = distribution(random_engine);
        data.insert(data.begin()+i, randomNumber);
        
    }
    return data;
}
 
int main()
{
    int n=1117649;
    int nw=7;
    std::vector<int> data_vector = generate_data(n);
    //std::vector<int> data_vector{21,18,16,1,3,20,2,10,15,4,17,5,9,19,6,11,14,7,12,8,13}; 

/* DEFINE COMPUTATION BARRIER AND BODY COMPUTATION THREAD */
    std::unique_ptr<Barrier> comp_barrier(new Barrier(nw+1));
    std::function<std::vector<int>(std::vector<int>)> sort_and_separators = [nw,&comp_barrier](std::vector<int> data)
    {
            std::sort(data.begin(), data.end(), std::less<int>());
            std::vector<int> sample(nw-1);
            sample.insert(sample.begin(),*data.begin());
            sample.insert(sample.end(), *std::prev(data.end()));
            int span = data.size()/nw;
            std::vector<int>::iterator it = data.begin()+span;
            for(int i=1; i<nw; i++,it+=span)
                sample[i]=*it;
            comp_barrier->wait();
            return sample;
    };

/* DEFINE COMMUNICATION BARRIER AND BODY COMMUNICATION THREAD */
    std::unique_ptr<Barrier> comm_barrier(new Barrier(nw+1));
    std::function<void(std::vector<int>)> send = [&comm_barrier](std::vector<int> data)
    {

            comm_barrier->wait();
    };


/* COMPUTE TSEQ */
    {
        std::vector<int> seq=generate_data(n);
        Utimer t("T_SEQ: ");
        std::sort(seq.begin(), seq.end(), std::less<int>());
    }

/* SUPERSTEP 1 */
    {
        Utimer t_s1("T_S1: ");
        SuperStep<int> s1(nw, data_vector);
        s1.set_barrier(comp_barrier.get());
        {
            Utimer t("COMP_S1:");
            s1.computation(sort_and_separators,true);
            s1.sync();
        }
        s1.set_barrier(comm_barrier.get());
        {
            Utimer t("COMM_S1:");
            s1.communication(send);
            s1.sync();
        }
    }
}
