#include "./superstep.cpp"
#include "./utimer.cpp"
#include "./worker.cpp"
#include <algorithm>
#include <random>
#include <thread>
#include <mutex>


std::vector<int> generate_data(int n)
{   
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


template<typename T>
void print_vector(std::vector<T> data)
{
    for ( T i:data)
        std::cout<<i<<" ";
        std::cout<<std::endl;
}
 
int main()
{
    int n=21;
    int nw=2;
    //std::vector<int> data_vector = generate_data(n);
    std::vector<int> data_vector{21,18,16,1,3,20,2,10,15,4,17,5,9,19,6,11,14,7,12,8,13};   
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
    {
        Utimer t("Superstep:");
        SuperStep<int> s1(nw, data_vector);
        s1.computation(sort_and_separators,true);
    }
}
