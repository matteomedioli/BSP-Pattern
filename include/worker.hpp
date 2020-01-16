/**
 * \file worker.hpp
 * \author Matteo Medioli
 */

#ifndef WORKER_H
#define WORKER_H

#include <functional>
#include <thread>
#include <algorithm>
#include "barrier.hpp"
#include "sharedvect.hpp"

using ComputationFunction = std::function<std::vector<int>(std::vector<int>)>;
using CommunicationFunction = std::function<std::vector<int>(std::vector<int>, int id, int dest)>;
using CommunicationProtocol = std::vector<std::pair<int,std::vector<int>>>;

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
        void work(std::function<F(Args...)> body, Barrier* barrier)
        {
            thread = std::thread{[this,body,barrier]()
                { 
                    output = body(input);
                    barrier->wait();
                }
            };

        }
        
        template<typename F, typename ...Args>
        void send(std::function<F(Args...)> body, CommunicationProtocol protocol, Barrier* barrier, const std::vector<std::shared_ptr<SharedVector<T>>>& shared_vector)
        {
            thread = std::thread{[this,body,protocol,barrier,shared_vector]()
            { 
                auto it = std::find_if( protocol.begin(), protocol.end(),[this](const std::pair<int, std::vector<int>>& element){return element.first == id;});
                std::vector<int> destination = it->second;
                for(auto dest : destination)
                    {
                        shared_vector[dest]->append(body(output,id,dest));
                    }
                barrier->wait();
            }};
        }
};

#endif
