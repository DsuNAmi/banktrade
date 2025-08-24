#include " ui.h"
#include "customer.h"
#include "net.h"

int main(){

    UI::Setup();
    UI::Title();

    char option;
    while(option != 'q'){
        std::cin >> option;
        Customer cust(30, 1000.0, "", L"John Doe", "John", "ID123456", "CARD123456", L"123 Main St");
        auto client_net = std::make_unique<Net>("localhost", 12345);
        client_net->Send("ID: 4320598#\n");
        std::cout << "Sent client ID to server." << std::endl;
        std::cout << "Server response: " << client_net->Receive() << std::endl;
        client_net->Send(cust.serialize() + "\n");
        std::cout << "Sent customer data to server." << std::endl;
        std::cout << "Server response: " << client_net->Receive() << std::endl;
    }


    return EXIT_SUCCESS;
}