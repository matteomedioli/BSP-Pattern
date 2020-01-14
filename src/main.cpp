#include <algorithm>
#include <random>
#include "../include/utimer.hpp"
#include "../include/superstep.hpp"
#include "../include/worker.hpp"
#include "../include/sharedvect.hpp"

std::vector<int> generate_data(int n)
{   
    Utimer t("RANDOM VECTOR GENERATION: ");
    std::vector<int> data;
    for (unsigned i = 1; i < n+1; ++i)
    {
        unsigned j = rand() % (i + 1);

        if (j < i)
        {
            data.push_back(data[j]);
            data[j] = i;
        }
        else
            data.push_back(i);
    }
    return data;
}
 
int main(int argc, char * argv[])
{
    if(argc!=3)
    {
        std::cout<<"WRONG ARGUMENT EXCEPT: must give n (number of elements) and nw (parallel activities)"<<std::endl;
        return 0;
    }



/* GET ARGS */
    int n=atoi(argv[1]);
    int nw=atoi(argv[2]);


/* GENERATE DATA VECTOR */
    std::vector<int> data_vector = generate_data(n);
    /*std::cout<<"INPUT VECTOR: ";
    for(auto v : data_vector)
        std::cout<<v<<" ";
    std::cout<<std::endl<<std::endl;
    */


/* COMPUTE TSEQ */
    {
        std::vector<int> seq=data_vector;
        /*std::cout<<"INPUT VECTOR SEQ: ";
        for(auto v : data_vector)
            std::cout<<v<<" ";
        std::cout<<std::endl<<std::endl;*/
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
    std::function<std::vector<int>(std::vector<int>, int id, int dest)> void_comm = [&](std::vector<int> data,int id, int dest)
    {
            return data;
    };

    std::mutex print;
    std::function<std::vector<int>(std::vector<int>,int,int)> distribute_by_bound = [sort_and_separators,nw,&print,&data_vector](std::vector<int> output, int id, int dest)
    {
        std::unique_lock<std::mutex> lock(print);
        std::vector<int> boundaries=sort_and_separators(output);
        std::vector<int> filtered;
        // CHUNCK DISTRIBUTION
        int delta = data_vector.size()/nw;
        auto first = data_vector.begin() + id*delta;
        auto last = data_vector.begin() + ((id+1)*delta);
        std::vector<int> data(first,last);
    
        std::cout<<"1) ANALIZE INPUT FROM "<<id<<" TO "<<dest<<": ";
        for(auto e : data)
            std::cout<<e<<" ";
        std::cout<<std::endl;

        int infer = boundaries[dest];
        int super = boundaries[dest+1];

        std::cout<<"2) RANGE: ";
        std::cout<<"["<<infer<<" , "<<super<<"]"<<std::endl;

        if (dest!=nw-1)
            std::copy_if (data.begin(), data.end(), std::back_inserter(filtered),[infer,super](int i){return  infer<=i && i<super;});
        else
            std::copy_if (data.begin(), data.end(), std::back_inserter(filtered),[infer,super](int i){return  infer<=i && i<=super;});

        std::cout<<"3) FILTERED OUTPUT: ";
        for(auto e : filtered)
            std::cout<<e<<" ";
        
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
    

/* SUPERSTEP 1 */
    std::cout<<std::endl;
    SuperStep<int> s1(nw, data_vector);
        //S1 COMPUTATION PHASE
        s1.reset_barrier();
        {
            Utimer t("COMP_S1:");
            s1.computation(sort_and_separators,true);
        }

        //S1 COMMUNICATION PHASE
        s1.reset_barrier();
        {
            Utimer t("COMM_S1:");
            s1.communication(void_comm,to_itself);
        }

    for(auto a:s1.get_results())
    std::cout<<a<<" ";
    std::cout<<std::endl;


/* SUPERSTEP 2 */
    std::cout<<std::endl;
    SuperStep<int> s2(nw, s1.get_results());
        //S1 COMPUTATION PHASE
        s2.reset_barrier();
        {
            Utimer t("COMP_S2:");
            s2.computation(void_comp,false);
        }

        //S1 COMMUNICATION PHASE
        s2.reset_barrier();
        {
            Utimer t("COMM_S2:");
            s2.communication(distribute_by_bound,to_all);
        }


    for(auto a:s2.get_results())
    std::cout<<a<<" ";
    std::cout<<std::endl;

/* SUPERSTEP 3 */
    std::cout<<std::endl;
    SuperStep<int> s3(nw, s2.get_results());
        //S1 COMPUTATION PHASE
        s3.reset_barrier();
        {
            Utimer t("COMP_S3:");
            s3.computation(sort,true);
        }

        //S1 COMMUNICATION PHASE
        s3.reset_barrier();
        {
            Utimer t("COMM_S3:");
            s3.communication(void_comm,to_itself);
        }

    std::vector<int> result = s3.get_results();
    
    for(auto a:result)
    std::cout<<a<<" ";
    std::cout<<std::endl;

}
