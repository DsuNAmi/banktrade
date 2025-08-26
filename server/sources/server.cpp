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
                std::lock_guard<std::mutex> lock(s_s_mutex);
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

std::string Session::GetClientId() const{
    return s_s_client_id;
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
  s_acceptor(s_io_context, MakeEndpoint())
{
    
}

int Server::judgeConnectNum(int connect_num){
    if(connect_num < s_min_connect_num){return s_defalut_connect_num;}
    else if(connect_num > s_max_connect_num){return s_max_connect_num;}
    return connect_num;
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
    if (s_clients.size() >= static_cast<size_t>(s_connect_num)) {
        std::cout << "Maximum number of clients reached. New connections will not be accepted." << std::endl;
        return;
    }
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


void Server::SendClientByClientId(const std::string & client_id, const std::string & message){
    if(!s_running.load()){
        std::cerr << "Sever is not running." << std::endl;
    }
    {
        std::lock_guard<std::mutex> lock(s_server_mutex);
        auto it = s_clients.find(client_id);
        if(it != s_clients.end()){
            it->second->SendMessage(message);
        }else{
            std::cerr << "Client:" << client_id << "is not exsiting." << std::endl;
        }
    }
}

void Server::Boardcast(const std::string & message){
    if(!s_running.load()){
        std::cerr << "Sever is not running." << std::endl;
    }
    std::unique_lock<std::mutex> lock(s_server_mutex, std::defer_lock);
    lock.lock();
    if(s_clients.empty()){
        std::cerr << "No clients needs to send message: " << message << std::endl; 
        return;
    }
    lock.unlock();
    lock.lock();
    for(const auto & [c_id, c_seesion] : s_clients){
        c_seesion->SendMessage(message);
    }
    lock.unlock();
}


std::size_t Server::ClientCount() const {
    std::lock_guard<std::mutex> lock(s_server_mutex);
    return s_clients.size();
}

void Server::Run(){
    s_running.store(true);
    std::cout << "Server is running on " << s_host << ":" << s_port_num << std::endl;
    DoAccept();
    s_io_context.run();
}

void Server::Stop(){
    s_running.store(false);
    std::cout << "Server is stopping..." << std::endl;
    s_io_context.stop();
}

void Server::Restart(){
    s_running.store(false);
    std::cout << "Server is restarting..." << std::endl;
    s_io_context.restart();
    Run();
}


Server::~Server(){
    Stop();
}
