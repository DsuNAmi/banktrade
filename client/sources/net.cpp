#include "net.h"


Net::Net(const std::string & host, unsigned short port)
: n_io_context(), n_socket(n_io_context)
{
    boost::asio::ip::tcp::resolver resolver(n_io_context);
    auto endpoints = resolver.resolve(host,std::to_string(port));
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


