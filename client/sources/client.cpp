#include "client.h"


Client::Client(std::string host, unsigned short port, std::size_t t_number)
: c_sp_net(std::make_shared<Net>(std::move(host), port)),
c_pools(CalTreadNumber(t_number)),
c_connect_status(false), c_connect_overtime(0)
{

}


void Client::ConnectServer(){
    while(c_connect_overtime < c_reconnect_times){
        try{
            c_sp_net->Connect();
            c_connect_status.store(true);
            return;
        }catch(const std::exception & e){
            std::cerr << e.what() << std::endl;
            std::cerr << "Ready to reconnceting... after 10s" << std::endl;
            for(int i = 10; i > 0; --i){
                std::cout << "\rwait for " << i << "s" << std::flush;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            ++c_connect_overtime;
        }
    }

    c_connect_status.store(false);
    return;

}

void Client::Disconnect(){
    
}


void Client::Run(){
    //steup for bank trade
    UI::Setup();

    //net connection
    c_pools.AddTask([this](){
        this->ConnectServer();
    });
    while(!c_connect_status.load()
            && c_connect_overtime < c_reconnect_times){
                //actually, You should be sppoused to give something to do.
                //Just like play an offline game.
                //Give me a another think.
                //Yes, a game, you R right.
            };
    c_connect_overtime = 0;
    if(c_connect_status.load()){
        UI::Login(c_sp_net);
        std::cout << "GetName from server : \n";
        c_username = "Server::GetName()";
        UI::Title(shared_from_this());
        UI::ClearScreen();
        //add a thread
        c_pools.AddTask(UI::MainWindow);
        //and you can do otherthings
        //check the connection
        
    }else{
        std::cout << std::endl;
        std::cerr << "Failed to connect to server." << std::endl;
    }
}