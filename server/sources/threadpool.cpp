#include "threadpool.h"


Threadpool::Threadpool(std::size_t size)
: tp_size(size), tp_stop(false)
{
    if(0 == tp_size){
        throw std::invalid_argument("ThreadPool must have it's size\n");
    }
    for(size_t i = 0; i < tp_size; ++i){
        tp_pool.emplace_back([this]{
            while(true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tp_basic_mutex);
                    tp_codition.wait(lock, [this]{
                        return tp_stop.load() ||
                            !tp_tasks.empty();
                    });

                    if(tp_stop.load() && tp_tasks.empty()){
                        return;
                    }

                    task = std::move(tp_tasks.front());
                    tp_tasks.pop();
                }
                task();
            }
        });
    }
}

bool Threadpool::Full(bool added){
    if(tp_size >= tp_max_size){
        return true;
    }
    if(added){
        std::size_t remain = tp_max_size - tp_size;
        AddVolum(remain);
        return true;
    }
    return false;
}


bool Threadpool::AutoAddVolum(){
    if(tp_size >= tp_max_size){
        return false;
    }
    std::size_t add_size = tp_size / 2;
    if(add_size <= 0){
        add_size = 1;
    }
    return AddVolum(add_size);
}


bool Threadpool::AddVolum(std::size_t volum){
    if(volum <= 0){
        return false;
    }
    if(tp_size + volum > tp_max_size){
        throw std::length_error(std::format("ThreadPool size exceed max limit {}\n",tp_max_size));
    }
    for(size_t i = 0; i < volum; ++i){
        tp_pool.emplace_back([this]{
            while(true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tp_basic_mutex);
                    tp_codition.wait(lock,[this]{
                        return tp_stop.load() ||
                            !tp_tasks.empty();
                    });

                    if(tp_stop.load() && tp_tasks.empty()){
                        return;
                    }

                    task = std::move(tp_tasks.front());
                    tp_tasks.pop();
                }
                task();
            }
        });
    }
    tp_size += volum;
    return true;
}


void Threadpool::Stop(){
    {
        std::unique_lock<std::mutex> lock(tp_basic_mutex);
        tp_stop.store(true);
    }

    tp_codition.notify_all();
    for(auto & th : tp_pool){
        if(th.joinable()){
            th.join();
        }
    }
}


Threadpool::~Threadpool(){
    Stop();
}