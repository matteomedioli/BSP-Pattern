#include "./superstep.cpp"
#include "./utimer.cpp"
#include "./worker.cpp"
#include <algorithm>
#include <random>
#include <thread>

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
    std::vector<int> data_vector = generate_data(10000);
    int nw=16;
    std::function<void(int)> hello = [](int i){std::cout<<"CIAO da WORKER "<<i<<std::endl;};

    //  T_SEQ
    Utimer *timer = new Utimer("std::sort:");
    std::sort(data_vector.begin(), data_vector.end(), std::greater<int>());
    timer->~Utimer();
    //
    Utimer *timer_ss = new Utimer("Superstep Generation:");
    SuperStep<int,void,int> *s = new SuperStep<int,void,int>(nw, data_vector);
    s->setThreadBody(hello);
    s->computation();
    timer_ss->~Utimer();

}
