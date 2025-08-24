#pragma once 


#include <boost/asio.hpp>
#include <string>

class Net {
public:
    Net(const std::string& host, unsigned short port);
    void Send(const std::string& message);
    std::string Receive();

private:
    boost::asio::io_context n_io_context;
    boost::asio::ip::tcp::socket n_socket;
};