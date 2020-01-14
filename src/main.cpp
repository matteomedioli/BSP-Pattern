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
    std::cout<<"INPUT VECTOR: ";
    for(auto v : data_vector)
        std::cout<<v<<" ";
    std::cout<<std::endl<<std::endl;
    

/* COMPUTE TSEQ */
    {
        std::vector<int> seq=data_vector;
        Utimer t("T_SEQ: ");
        std::sort(seq.begin(), seq.end(), std::less<int>());
    }


/* DEFINE COMPUTATION BODY THREAD */
    ComputationFunction sort_and_separators = [nw](std::vector<int> data)
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

    ComputationFunction sort = [](std::vector<int> data)
    {
            std::sort(data.begin(), data.end(), std::less<int>());
            return data;
    };


    ComputationFunction void_comp = [](std::vector<int> data)
    {
            return data;
    };


/* DEFINE COMMUNICATION BODY THREAD */
    CommunicationFunction void_comm = [](std::vector<int> data,int id, int dest)
    {
            return data;
    };

    CommunicationFunction distribute_by_bound = [sort_and_separators,nw,&data_vector](std::vector<int> output, int id, int dest)
    {
        std::vector<int> boundaries=sort_and_separators(output);
        std::vector<int> filtered;
        // CHUNCK DISTRIBUTION
        int delta = data_vector.size()/nw;
        auto first = data_vector.begin() + id*delta;
        auto last = data_vector.begin() + ((id+1)*delta);
        std::vector<int> data(first,last);

        int inf = boundaries[dest];
        int sup = boundaries[dest+1];

        if (dest!=nw-1)
            std::copy_if (data.begin(), data.end(), std::back_inserter(filtered),[inf,sup](int i){return  inf<=i && i<sup;});
        else
            std::copy_if (data.begin(), data.end(), std::back_inserter(filtered),[inf,sup](int i){return  inf<=i && i<=sup;});
        return filtered;
    };

/* COMMUNICATION PROTOCOLS */
    CommunicationProtocol to_itself;
    for(int i=0; i<nw; i++)
    {
        std::vector<int> d{i};
        to_itself.push_back(std::make_pair(i,d));
    }


    /* COMMUNICATION PROTOCOLS */
    CommunicationProtocol to_all;

    std::vector<int> d;
    for(int i=0; i<nw; i++)
        d.push_back(i);
    for(int i=0; i<nw; i++)
        to_all.push_back(std::make_pair(i,d));


std::vector<std::vector<int>> output;

/* SUPERSTEP 1 */
    std::cout<<std::endl;
    SuperStep<int> s1(nw,data_vector,true);
        //S1 COMPUTATION PHASE
        s1.reset_barrier();
        {
            Utimer t("COMP_S1:");
            s1.computation(sort_and_separators);
        }

        //S1 COMMUNICATION PHASE
        s1.reset_barrier();

        {
            Utimer t("COMM_S1:");
            s1.communication(void_comm,to_itself);
        }

    output=s1.get_results(output);
    std::cout<<std::endl;

/* SUPERSTEP 2 */
    SuperStep<int> s2(nw, output, true);
        //S1 COMPUTATION PHASE
        s2.reset_barrier();
        {
            Utimer t("COMP_S2:");
            s2.computation(void_comp);
        }

        //S1 COMMUNICATION PHASE
        s2.reset_barrier();
        {
            Utimer t("COMM_S2:");
            s2.communication(distribute_by_bound,to_all);
        }
    
    output=s2.get_results(output);
    std::cout<<std::endl;

/* SUPERSTEP 3 */
    SuperStep<int> s3(nw, s2.get_results(output), false);
        //S1 COMPUTATION PHASE
        s3.reset_barrier();
        {
            Utimer t("COMP_S3:");
            s3.computation(sort);
        }
        //S1 COMMUNICATION PHASE
        s3.reset_barrier();
        {
            Utimer t("COMM_S3:");
            s3.communication(void_comm,to_itself);
        }

    output=s3.get_results(output);
    std::vector<int> result = flatten(output);
    for(auto el : result)
        std::cout<<el<<" ";
    std::cout<<std::endl;
}
