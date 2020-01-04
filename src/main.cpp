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


int compare(const void* a, const void* b)   // comparison function
{
    int arg1 = *reinterpret_cast<const int*>(a);
    int arg2 = *reinterpret_cast<const int*>(b);
    if(arg1 < arg2) return -1;
    if(arg1 > arg2) return 1;
    return 0;
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
    int n=262144;
    int nw=32;
    std::vector<int> data_vector = generate_data(n);
    std::function<std::vector<int>(std::vector<int>)> sort_and_separators = [](std::vector<int> data)
    {
        std::sort(data.begin(), data.end(), std::less<int>());

        return data;
    };

    {
        Utimer t("SEQ:");
        std::sort(data_vector.begin(),data_vector.end(),std::less<int>());
    }

    {
        Utimer t("Superstep:");
        SuperStep<int,std::vector<int>, std::vector<int>> s1(nw, data_vector);
        s1.setThreadBody(sort_and_separators);
        s1.computation(true);
    }

}
