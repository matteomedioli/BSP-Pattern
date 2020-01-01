#include "./superstep.cpp"
#include "./utimer.cpp"


int main()
{
    auto foo = [&] () {
        std::cout<<"BODY"<<std::endl;
    };
    std::function<void()> body = foo;
    int nw=64;
    std::vector<int> dst {0,0,0,0};
    SuperStep<int> * s1 = new SuperStep<int>(nw, dst);
    s1->computation(body);
}