/**
 * \file worker.hpp
 * \author Matteo Medioli
 */

#ifndef WORKER_H
#define WORKER_H

#include <functional>
#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>
#include "barrier.hpp"

template <typename T>
class Worker {
    private:
        int id;
        std::thread thread;
        std::vector<T> input;
        std::vector<T> output;


    public:
        Worker& operator=(const Worker&) = delete; 
        Worker& operator=(Worker&&) = delete;
        Worker(const Worker&) = delete;

        Worker(int id, std::vector<T> input) : id(id), input(input)
        {}


        Worker(Worker &&other) : thread(std::move(other.thread)),
                                 id(std::move(other.id)),
                                 input(std::move(other.input)),
                                 output(std::move(other.output))
        {}

        
        ~Worker()
        {
            join();
        }


        int get_id()
        {
            return id;    
        }


        void join()
        {
            if(thread.joinable())
            {
                thread.join();
            }
        }


        template<typename F, typename ...Args>
        void work(int nw, std::function<F(Args...)> body, bool chunk, Barrier* barrier)
        {
            thread = std::thread{[this,body,chunk,nw, barrier]()
                { 
                    if(chunk)
                    {
                        // CHUNCK DISTRIBUTION
                        int delta = input.size()/nw;
                        auto first = input.begin() + id*delta;
                        auto last = input.begin() + ((id+1)*delta);
                        std::vector<T> c(first,last);
                        input=c;
                    }
                    output = body(input);
                    barrier->wait();
                }
            };

        }

        template<typename F, typename ...Args>
        std::vector<T>& send(int dest,std::function<F(Args...)> body, Barrier* barrier)
        {
            thread = std::thread{[this,body, dest,barrier]()
                { 
                    barrier->wait();
                    std::vector<T> res;
                    return body(output,id,dest);
                }
            };
        }
};

#endif
