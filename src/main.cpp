
#include <algorithm>
#include <random>
#include "../include/utimer.hpp"
#include "../include/superstep.hpp"
#include "superstep.cpp"
#include "../include/worker.hpp"
#include "worker.cpp"

template <template<typename...> class R=std::vector, typename Top, typename Sub = typename Top::value_type> 
R<typename Sub::value_type> flatten(Top const& all)
{
    using std::begin;
    using std::end;

    R<typename Sub::value_type> accum;

    for(auto& sub : all)
        accum.insert(end(accum), begin(sub), end(sub));

    return accum;
}

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
    int n=21;
    int nw=3;
    //std::vector<int> data_vector = generate_data(n);
    std::vector<int> data_vector{21,18,16,1,3,20,2,10,15,4,17,5,9,19,6,11,14,7,12,8,13}; 

/* DEFINE COMPUTATION BARRIER AND BODY COMPUTATION THREAD */
    std::shared_ptr<Barrier> computation(new Barrier(nw+1,"COMPUTATION_1"));
    std::function<std::vector<int>(std::vector<int>)> sort_and_separators = [nw,&computation](std::vector<int> data)
    {
            std::sort(data.begin(), data.end(), std::less<int>());
            std::vector<int> sample(nw-1);
            sample.insert(sample.begin(),*data.begin());
            sample.insert(sample.end(), *std::prev(data.end()));
            int span = data.size()/nw;
            std::vector<int>::iterator it = data.begin()+span;
            for(int i=1; i<nw; i++,it+=span)
                sample[i]=*it;
            computation->wait();
            return sample;
    };

/* DEFINE COMMUNICATION BARRIER AND BODY COMMUNICATION THREAD */
    std::shared_ptr<Barrier> communication(new Barrier(nw+1, "COMMUNICATION_1"));
    std::function<std::vector<int>(std::vector<int>)> send = [&communication](std::vector<int> data)
    {
            communication->wait();
            return data;
    };
    

/* COMPUTE TSEQ */
    {
        std::vector<int> seq=generate_data(n);
        Utimer t("T_SEQ: ");
        std::sort(seq.begin(), seq.end(), std::less<int>());
    }

/* SUPERSTEP 1 */
    std::cout<<std::endl;
    SuperStep<int> s1(nw, data_vector);
        //S1 COMPUTATION PHASE
        s1.set_barrier(computation);
        {
            Utimer t("COMP_S1:");
            s1.computation(sort_and_separators,true);
            s1.sync();
        }
        //S1 COMMUNICATION PHASE
        s1.set_barrier(communication);
        std::vector<std::pair<int,int>> protocol;
        for(int i=0; i<nw; i++)
            protocol.push_back(std::make_pair(i,i));

        {
            Utimer t("COMM_S1:");
            s1.communication(send,protocol);
            s1.sync();
        }

std::vector<int> output_s1 = flatten(s1.get_output());
computation.reset(new Barrier(nw+1,"COMPUTATION_2"));
communication.reset(new Barrier(nw+1,"COMMUNICATION_2"));

/* SUPERSTEP 2 */
    std::cout<<std::endl;
    SuperStep<int> s2(nw, output_s1);
        //S1 COMPUTATION PHASE
        s2.set_barrier(computation);
        {
            Utimer t("COMP_S2:");
            s2.computation(sort_and_separators,false);
            s2.sync();
        }
        //S1 COMMUNICATION PHASE
        s2.set_barrier(communication);
        {
            Utimer t("COMM_S2:");
            s2.communication(send,protocol);
            s2.sync();
        }
std::vector<int> output_s2 = flatten(s2.get_output());
for(auto i:output_s2)
std::cout<<i<<" ";
std::cout<<std::endl;
}