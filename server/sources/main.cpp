#include "server.h"


int main(int argc, char * argv[]){

    int port = argv[1] ? std::atoi(argv[1]) : 12345;
    std::string host = argv[2] ? argv[2] : "localhost";
    int connect_num = argv[3] ? std::atoi(argv[3]) : 5;

    auto bank_server = std::make_unique<Server>(port, host, connect_num);

    bank_server->UIShow();
    bank_server->Run();

    return EXIT_SUCCESS;
}