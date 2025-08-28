#pragma once 

#include "net.h"
#include "ui.h"
#include "threadpool.h"

#include <string>
#include <exception>
#include <chrono>
#include <atomic>

class Client : std::enable_shared_from_this<Client>{
    public:
        Client(std::string host, unsigned short port, std::size_t t_number = 10);
        std::string GetName() const {return c_username;}

        void Run();

        
        const std::weak_ptr<Net> & GetNetptr() const {return c_sp_net;}
        
    private:
        std::string c_username;
        std::shared_ptr<Net> c_sp_net;
        Threadpool c_pools;
        std::atomic<bool> c_connect_status;
        std::atomic<int> c_connect_overtime;
        std::mutex c_mutex;
        
        std::size_t CalTreadNumber(std::size_t t_number){
            if(t_number < c_defalut_t_number) return c_defalut_t_number;
            else if(t_number > c_max_t_number) return c_max_t_number;
            return t_number;
        }
        void ConnectServer();
        void Disconnect();

        static constexpr std::size_t c_defalut_t_number = 10;
        static constexpr std::size_t c_max_t_number = 100;
        static constexpr std::size_t c_reconnect_times = 5;

};
