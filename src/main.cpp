
#include <algorithm>
#include <random>
#include "../include/utimer.hpp"
#include "../include/superstep.hpp"
#include "superstep.cpp"
#include "../include/worker.hpp"
#include "worker.cpp"
#include "../include/sharedvect.hpp"
#include "sharedvect.cpp"

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

/* COMPUTE TSEQ */
    {
        std::vector<int> seq=generate_data(n);
        Utimer t("T_SEQ: ");
        std::sort(seq.begin(), seq.end(), std::less<int>());
    }


/* DEFINE COMPUTATION BODY THREAD */
    std::function<std::vector<int>(std::vector<int>)> sort_and_separators = [nw](std::vector<int> data)
    {
            std::sort(data.begin(), data.end(), std::less<int>());
            std::vector<int> sample(nw-1);
            sample.insert(sample.begin(),*data.begin());
            sample.insert(sample.end(), *std::prev(data.end()));
            int span = data.size()/nw;
            std::vector<int>::iterator it = data.begin()+span;
            for(int i=1; i<nw; i++,it+=span)
                sample[i]=*it;
            return sample;
    };

/* DEFINE COMPUTATION BODY THREAD */
    std::function<std::vector<int>(std::vector<int>)> sort = [](std::vector<int> data)
    {
            std::sort(data.begin(), data.end(), std::less<int>());
            return data;
    };

/* DEFINE COMMUNICATION BODY THREAD */
    std::function<std::vector<int>(std::vector<int>)> void_comp = [](std::vector<int> data)
    {
            return data;
    };
/* DEFINE COMMUNICATION BODY THREAD */
    std::function<std::vector<int>(std::vector<int>, int dest)> void_comm = [](std::vector<int> data, int dest)
    {
            return data;
    };

    std::function<std::vector<int>(std::vector<int>,int)> distribute_by_bound = [sort_and_separators,nw](std::vector<int> data, int dest)
    {
        std::vector<int> filtered;
        std::vector<int> boundaries=sort_and_separators(data);
        int infer = boundaries[dest];
        int super = boundaries[dest+1];
        if (dest!=nw-1)
            std::copy_if (data.begin(), data.end(), std::back_inserter(filtered),[infer,super](int i){return  infer<=i && i<super;});
        else
            std::copy_if (data.begin(), data.end(), std::back_inserter(filtered),[infer,super](int i){return  infer<=i && i<=super;});
        return filtered;
    };

/* COMMUNICATION PROTOCOLS */
    std::vector<std::pair<int,std::vector<int>>> to_itself;
    for(int i=0; i<nw; i++)
    {
        std::vector<int> d{i};
        to_itself.push_back(std::make_pair(i,d));
    }


    /* COMMUNICATION PROTOCOLS */
    std::vector<std::pair<int,std::vector<int>>> to_all;
    std::vector<int> d;
    for(int i=0; i<nw; i++)
        d.push_back(i);
    for(int i=0; i<nw; i++)
        to_all.push_back(std::make_pair(i,d));
    

/* BARRIER */
    std::shared_ptr<Barrier> barrier(new Barrier(nw+1));


/* SUPERSTEP 1 */
    std::cout<<std::endl;
    SuperStep<int> s1(nw, data_vector);
        //S1 COMPUTATION PHASE
        s1.set_barrier(barrier);
        {
            Utimer t("COMP_S1:");
            s1.computation(sort_and_separators,true);
            s1.sync();
        }

        //RESET BARRIER FOR COMMUNICATION
        barrier.reset(new Barrier(nw+1));
        //S1 COMMUNICATION PHASE
        s1.set_barrier(barrier);
        {
            Utimer t("COMM_S1:");
            s1.communication(void_comm,to_itself);
            s1.sync();
        }
    barrier.reset(new Barrier(nw+1));

    for(auto a:s1.get_results())
    std::cout<<a<<" ";
    std::cout<<std::endl;


/* SUPERSTEP 2 */
    std::cout<<std::endl;
    SuperStep<int> s2(nw, s1.get_results());
        //S1 COMPUTATION PHASE
        s2.set_barrier(barrier);
        {
            Utimer t("COMP_S2:");
            s2.computation(void_comp,false);
            s2.sync();
        }

        //RESET BARRIER FOR COMMUNICATION
        barrier.reset(new Barrier(nw+1));
        //S1 COMMUNICATION PHASE
        s2.set_barrier(barrier);
        {
            Utimer t("COMM_S2:");
            s2.communication(distribute_by_bound,to_itself);
            s2.sync();
        }

    barrier.reset(new Barrier(nw+1));

    for(auto a:s2.get_results())
    std::cout<<a<<" ";
    std::cout<<std::endl;

/* SUPERSTEP 3 */
    std::cout<<std::endl;
    SuperStep<int> s3(nw, s2.get_results());
        //S1 COMPUTATION PHASE
        s3.set_barrier(barrier);
        {
            Utimer t("COMP_S3:");
            s3.computation(sort,true);
            s3.sync();
        }

        //RESET BARRIER FOR COMMUNICATION
        barrier.reset(new Barrier(nw+1));
        //S1 COMMUNICATION PHASE
        s3.set_barrier(barrier);
        {
            Utimer t("COMM_S3:");
            s3.communication(void_comm,to_itself);
            s3.sync();
        }

    barrier.reset(new Barrier(nw+1));
    std::vector<int> result = s3.get_results();
    
    for(auto a:result)
    std::cout<<a<<" ";
    std::cout<<std::endl;

}

