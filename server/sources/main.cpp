#include "server.h"


int main(){

    auto bank_server = std::make_unique<Server>(12345);

    bank_server->UIShow();
    bank_server->Run();

    return EXIT_SUCCESS;
}