#pragma once 

#include "net.h"
#include "ui.h"
#include "threadpool.h"
#include "cenums.h"


#include <string>
#include <exception>
#include <chrono>
#include <atomic>
#include <functional>



class Client : std::enable_shared_from_this<Client>{
    public:
        Client(std::string host, unsigned short port, std::size_t t_number = 10);
        std::string GetName() const {return c_username;}

        void Run();
        void Send(const std::string & message){
            c_sp_net->Send(message);
        }
        std::string Receive(){
            return c_sp_net->Receive();
        }
        void SetName(const std::string & username){
            if(c_username.empty()){
                c_username = username;
            }
            return;
        }
        const std::weak_ptr<Net> & GetNetPtr() const {return c_sp_net;}

        LoginType Login(std::string && username, std::string && password, LoginType login_type = LoginType::INPUT);
        TranscationType Transcation(std::function<TranscationType(const boost::property_tree::ptree & pt)> func,
                                    const boost::property_tree::ptree & pt);
        //Transcation Function
        // TranscationType SaveCash(const boost::property_tree::ptree & pt);
        // TranscationType Transfer(const boost::property_tree::ptree & pt);
        // TranscationType WithdrawCash(const boost::property_tree::ptree & pt);


        

        //protocol
        std::string ConstructProtocol(int protocol_type, const boost::property_tree::ptree & pt){
            if(protocol_type == 1){return this->LoginProtocol(pt);}
            else return "";
        }

        std::string LoginProtocol(const boost::property_tree::ptree & pt){
            try{
                std::cout << "Fix the pt_json to str" << std::endl;
                std::string res = "fitprotocol";
                EncryptStr(res);
                return "fitprotocol";
            }catch(const std::exception & e){
                std::cerr << e.what() << std::endl;
                show_error(ErrorType::ERROR, "Construction LoginProtocol Wrong.");
                return "";
            }
        }

    private:
        std::string c_username;
        std::shared_ptr<Net> c_sp_net;
        Threadpool c_pools;
        std::atomic<bool> c_connect_status;
        std::atomic<int> c_connect_overtime;
        std::mutex c_mutex;
        std::function<TranscationType(const boost::property_tree::ptree & pt)> c_transcation_func;
        
        std::size_t CalTreadNumber(std::size_t t_number){
            if(t_number < c_defalut_t_number) return c_defalut_t_number;
            else if(t_number > c_max_t_number) return c_max_t_number;
            return t_number;
        }
        void ConnectServer();

        void EncryptStr(std::string & plain_text);



        void show_error(ErrorType e_type,const char * message){
            switch (e_type)
            {
            case ErrorType::ERROR:
                std::cerr << "[<ERROR>]:: " << message << std::endl;
                return;
            case ErrorType::WARNNING:
                std::cerr << "[<WARNNING>]:: " << message << std::endl;
                return;
            case ErrorType::INFO:
                std::cerr << "[<INFO>]:: " << message << std::endl;
                return;
            default:
                std::cerr << "[<UNDEFINE>]:: " << message << std::endl;
                return;
            }
        }

        static constexpr std::size_t c_defalut_t_number = 10;
        static constexpr std::size_t c_max_t_number = 100;
        static constexpr std::size_t c_reconnect_times = 5;

};
