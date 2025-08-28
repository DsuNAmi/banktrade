#include "ui.h"
#include "customer.h"
#include "net.h"

int main(){
    
    std::shared_ptr<Client> sp_client = std::make_shared<Client>("localhost", 12345);
    sp_client->Run();
    return EXIT_SUCCESS;
}