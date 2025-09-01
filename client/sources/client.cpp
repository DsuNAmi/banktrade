#include "client.h"


Client::Client(std::string host, unsigned short port, std::size_t t_number)
: c_sp_net(std::make_shared<Net>(std::move(host), port)),
c_pools(CalTreadNumber(t_number)),
c_connect_status(false), c_connect_overtime(0)
{

}


void Client::ConnectServer(){
    while(c_connect_overtime < c_reconnect_times){
        try{
            c_sp_net->Connect();
            c_connect_status.store(true);
            return;
        }catch(const std::exception & e){
            std::cerr << e.what() << std::endl;
            std::cerr << "Ready to reconnceting... after 10s" << std::endl;
            for(int i = 10; i > 0; --i){
                std::cout << "\rwait for " << i << "s" << std::flush;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            ++c_connect_overtime;
        }
    }

    c_connect_status.store(false);
    return;

}


LoginType Client::Login(std::string && username, std::string && password, LoginType login_type){
    if(login_type == LoginType::INPUT){
        if(username.empty() || password.empty()){
            show_error(ErrorType::WARNNING,"No value in username or password");
            //remain somethings
            return LoginType::QUIT;
        }
        
        //con -> login -> protocol
        std::string loginInfo = std::format("username[{}]password[{}]",username, password);
        c_sp_net->Send(loginInfo);
        std::string server_res = c_sp_net->Receive();
        if(server_res.empty()){
            //unkonwn error
            show_error(ErrorType::ERROR, "unkonwn_error");
            return LoginType::WRONG;
        }else if(server_res == "100"){
            show_error(ErrorType::ERROR, "Wrong Password");
            return LoginType::QUIT;
        }else if(server_res == "101"){
            show_error(ErrorType::ERROR, "username is not existed.");
            return LoginType::QUIT;
        }else{
            //deparse the packet
            //code = 200
            show_error(ErrorType::INFO, "Login successful!");
            c_username = std::move(username);
            return LoginType::INPUT;
        }
    }else{
        switch (login_type)
        {
        case LoginType::FINGER:
            show_error(ErrorType::INFO, "Finger is right.");
            return login_type;
        case LoginType::SCANFACE:
            show_error(ErrorType::INFO, "face is right.");
            return login_type;
        default:
            show_error(ErrorType::WARNNING, "unkown wrong.");
            return LoginType::WRONG;
        }
    }
}


void Client::EncryptStr(std::string & plain_text){
    //do nothing for now 
}


TranscationType Client::Transcation(std::function<TranscationType(const boost::property_tree::ptree & pt)> func,
                                    const boost::property_tree::ptree & pt){
    TranscationType res_type;
    std::ostringstream transcation_information;
    boost::property_tree::write_json(transcation_information, pt, false);
    c_sp_net->Send(transcation_information.str());
    std::string res_server = c_sp_net->Receive();
    // deseralize the res_server
    return res_type;
}



void Client::Run(){
    //steup for bank trade
    UI::Setup();

    //net connection
    c_pools.AddTask([this](){
        this->ConnectServer();
    });
    while(!c_connect_status.load()
            && c_connect_overtime < c_reconnect_times){
                //actually, You should be sppoused to give something to do.
                //Just like play an offline game.
                //Give me a another think.
                //Yes, a game, you R right.
            };
    c_connect_overtime = 0;
    if(c_connect_status.load()){
        UI::Login(shared_from_this());
        UI::Title(shared_from_this());
        //add a thread
        // c_pools.AddTask(UI::MainWindow);
        //and you can do otherthings
        //check the connection
        //Keep Connection
        //snneze the user selection
    }else{
        std::cout << std::endl;
        std::cerr << "Failed to connect to server." << std::endl;
    }
}