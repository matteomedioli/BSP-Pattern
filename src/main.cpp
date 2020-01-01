#include "./superstep.cpp"
#include "./utimer.cpp"
#include "./worker.cpp"
#include <random>

std::vector<int> generate_data(int n, int min, int max)
{   
    std::vector<int> data;
    for(int i=0; i<n; i++)
    {
        std::random_device random_device;
        std::mt19937 random_engine(random_device());
        std::uniform_int_distribution<int> distribution(min, max);
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


int main()
{
    std::vector<int> data_vector = generate_data(21,0,21);
    int* data = &data_vector[0];

    int nw=3;

    constexpr std::size_t size = sizeof(data)/sizeof(int);
    std::function<void()> body = std::bind(std::qsort, data, size, sizeof(int), compare);
    SuperStep<int,void>* s1 = new SuperStep<int,void>(nw, body);
}
