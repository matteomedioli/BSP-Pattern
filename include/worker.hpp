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
#include "sharedvect.hpp"
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
                        // CHUNCK DISTRIBUTIONworkers
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
        void send(std::function<F(Args...)> body, std::vector<std::pair<int,std::vector<int>>> protocol, Barrier* barrier, const std::vector<std::shared_ptr<SharedVector<T>>>& shared_vector)
        {
            thread = std::thread{[this,body,protocol,barrier,shared_vector]()
            { 
                auto it = std::find_if( protocol.begin(), protocol.end(),[this](const std::pair<int, std::vector<int>>& element){return element.first == id;});
                std::vector<int> destination = it->second;
                for(auto dest : destination)
                    {
                        shared_vector[dest]->append(body(output,id,dest));
                        std::cout<<" send FILTERED from "<<id<<" TO "<<shared_vector[dest]->get_id()<<std::endl<<std::endl;
                    }
                barrier->wait();
            }};
        }
};

#endif
