#include "server.h"



Session::Session(
    boost::asio::ip::tcp::socket socket,
    std::unordered_map<std::string, std::shared_ptr<Session>> & clients
)
: s_s_socket(std::move(socket)),
  s_s_clients(clients)
{    

}


void Session::DoRead(){
    auto self(shared_from_this());
    s_s_socket.async_read_some(
        boost::asio::buffer(s_s_data, max_length),
        [this, self](boost::system::error_code ec, std::size_t lenght){
            if(!ec){
                std::string recv_msg(s_s_data, max_length);
                std::cout << "Received message: " << recv_msg << std::endl;
                //read the 
                // Handle the packet
                DoWrite("Message received\n");
                DoRead();
            }else{
                std::cerr << "client disconnection." << std::endl;
                if(!s_s_client_id.empty()){
                    s_s_clients.erase(s_s_client_id);
                }
            }
        }  
    );
}

void Session::DoWrite(const std::string & message){
    auto self(shared_from_this());
    boost::asio::async_write(s_s_socket, boost::asio::buffer(message),
        [this, self](boost::system::error_code ec, std::size_t lenght){
            if(!ec){
                std::cout << "Sent message: " << std::endl;
            }else{
                std::cerr << "Error sending message: " << ec.message() << std::endl;
            }
        }
    );
}




Server::Server(
    unsigned int port_num,
    std::string host,
    int connect_num
)
: 
  s_port_num(port_num),
  s_host(std::move(host)),
  s_connect_num(judgeConnectNum(connect_num)),
  s_running(false),
  s_io_context(),
  s_acceptor(s_io_context, MakeEndpoint()),
  s_threadpool(s_connect_num)
{
    
}

int Server::judgeConnectNum(int connect_num){
    int res;;
    if(connect_num < 5){res = 5;}
    else if(connect_num > 100){res = 100;}
    else{res = connect_num;}
    return res;
}


boost::asio::ip::tcp::endpoint Server::MakeEndpoint(){
    if(s_host == "localhost"){
        return boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), s_port_num);
    }
    return boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(s_host), s_port_num);
}


void Server::UIShow(){
    std::cout << R"(   ____              _       _______              _      
  |  _ \            | |     |__   __|            | |     
  | |_) | __ _ _ __ | | __     | | __ _ _ __   __| | ___ 
  |  _ < / _` | '_ \| |/ /     | |/ _` | '_ \ / _` |/ _ \
  | |_) | (_| | | | |   <      | | (_| | | | | (_| |  __/
  |____/ \__,_|_| |_|_|\_\     |_|\__,_|_| |_|\__,_|\___|
)" << std::endl;
    std::cout << "Welcome to bank Trade Sever!" << std::endl; 
}



void Server::DoAccept(){
    s_acceptor.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket t_socket){
            if(!ec){
                std::make_shared<Session>(std::move(t_socket), s_clients)->Start();
                DoAccept();
            }else{
                std::cerr << "Accept error: " << ec.message() << std::endl;
            }
        }

    );   
}

void Server::Run(){
    s_running.store(true);
    std::cout << "Server is running on " << s_host << ":" << s_port_num << std::endl;
    DoAccept();
    for(int i = 0; i < s_connect_num; ++i){
        s_threadpool.AddTask([this]{
            s_io_context.run();
        });
    }
    //手动阻塞主线程，防止程序退出
    while(s_running.load()){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(s_threadpool.IsStop()){
            s_running.store(false);
        }
    }
}

void Server::Stop(){
    s_io_context.stop();
}


Server::~Server(){
    Stop();
}
