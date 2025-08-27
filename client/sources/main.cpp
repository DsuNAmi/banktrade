#include "ui.h"
#include "customer.h"
#include "net.h"

int main(){
    
    auto clinet_net = std::make_shared<Net>("localhost",12341);
    UI::Setup();


    std::cout << static_cast<int>(UI::Login(clinet_net)) << std::endl;
    


    return EXIT_SUCCESS;
}