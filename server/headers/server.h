#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <string>
#include "threadpool.h"
#include <iostream>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <boost/asio.hpp>



class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(boost::asio::ip::tcp::socket socket, std::unordered_map<std::string, std::shared_ptr<Session>> & clients);
        ~Session() = default; 


        void Start(){DoRead();}

        std::string GetClientId() const;

        void SendMessage(const std::string & message){DoWrite(message);}


    private:
        boost::asio::ip::tcp::socket s_s_socket;
        enum {max_length = 1024};
        char s_s_data[max_length];
        std::unordered_map<std::string, std::shared_ptr<Session>> & s_s_clients;
        std::string s_s_client_id;
        mutable std::mutex s_s_mutex;



        void DoRead();
        void DoWrite(const std::string & message);


};




class Server {
    public:
        Server(unsigned int port_num, std::string host = "localhost", int connect_num = 5);
        ~Server();

        void UIShow();
        void Run();
        void Stop();
        void Restart();
        

        std::size_t ClientCount() const;
        void SendClientByClientId(const std::string & client_id, const std::string & message);
        void Boardcast(const std::string & message);
        
        

    private:
        unsigned int s_port_num;
        std::string s_host;
        int s_connect_num;
        boost::asio::io_context s_io_context;
        boost::asio::ip::tcp::acceptor s_acceptor;
        std::unordered_map<std::string, std::shared_ptr<Session>> s_clients;
        std::atomic<bool> s_running;
        mutable std::mutex s_server_mutex;

        boost::asio::ip::tcp::endpoint MakeEndpoint();
        int judgeConnectNum(int connect_num);

        void DoAccept();


        static constexpr int s_max_connect_num = 100;
        static constexpr int s_min_connect_num = 5;
        static constexpr int s_defalut_connect_num = 1;
};