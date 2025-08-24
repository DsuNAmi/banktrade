#pragma once


#include "customer.h"
#include "net.h"

#include <iostream>



class UI{
    public:
        static void Setup();
        static void Title();
        static void Run();
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