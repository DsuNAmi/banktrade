#pragma once


#include <mutex>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include <queue>
#include <exception>
#include <utility>


class Threadpool{
    public:
        Threadpool(size_t size);
        ~Threadpool();


        template<typename Func, typename... Args>
        void AddTask(Func && function, Args && ... args){
            auto task_wrapper = [
                function_copy = std::forward<Func>(function),
                ...args_copy = std::forward<Args>(args)
            ]() mutable {
                function_copy(args_copy...);
            };

            {
                std::unique_lock<std::mutex> lock(tp_basic_mutex);
                tp_tasks.emplace(std::move(task_wrapper));
            }
            tp_codition.notify_one();
        }

        bool IsStop() const {
            return tp_stop.load();
        }
        

        std::size_t Size() const {
            return tp_size;
        }

        bool Full(bool added = false);

        bool AutoAddVolum();

        bool AddVolum(std::size_t volum);

        void Stop();

    private:
        std::mutex tp_basic_mutex;
        std::vector<std::thread> tp_pool;
        std::queue<std::function<void()>> tp_tasks;
        std::condition_variable tp_codition;
        std::size_t tp_size;
        std::atomic<bool> tp_stop;
        static constexpr std::size_t tp_max_size = 2000;
};