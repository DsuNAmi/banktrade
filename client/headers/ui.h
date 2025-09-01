#pragma once


#include "customer.h"
#include "net.h"
#include "client.h"

#include <iostream>
#include <memory>
#include <limits>




class UI{
    public:
        static void Setup();

        static LoginType Login(const std::shared_ptr<Client> & client);

        static void Title(const std::shared_ptr<Client> & client);
        static void MainWindow();
        static void ClearScreen();
        static void Exit();


        //transcation interfaces
        static void SaveCash();
        // static void WithdrawCash();
        // static void Transfer();

        //information input interfaces
        static Customer InputCustomerInfo();

    private:
};