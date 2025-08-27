#include "net.h"


Net::Net(std::string host, unsigned short port)
: n_io_context(), n_socket(n_io_context),
n_host(std::move(host)),n_port(port)
{
    boost::asio::ip::tcp::resolver resolver(n_io_context);
    auto endpoints = resolver.resolve(n_host,std::to_string(n_port));
    boost::asio::connect(n_socket, endpoints);   
}


void Net::Send(const std::string & message){
    boost::asio::write(n_socket,boost::asio::buffer(message));
}

std::string Net::Receive(){
    boost::asio::streambuf buf;
    boost::asio::read_until(n_socket, buf, "\n");
    std::string data{boost::asio::buffers_begin(buf.data()), boost::asio::buffers_end(buf.data())};
    return data;
}


