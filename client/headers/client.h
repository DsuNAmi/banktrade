#pragma once 

#include "net.h"
#include "ui.h";

#include <string>

class Client{
    public:
        Client();
        Client(std::string host, unsigned short port);
        std::string GetName() const {return c_username;}
    private:
        std::string c_username;
        Net net;
};