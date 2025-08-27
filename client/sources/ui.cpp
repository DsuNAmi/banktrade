#include "ui.h"


void UI::Setup(){
    std::cout << R"(   ____              _       _______              _      
  |  _ \            | |     |__   __|            | |     
  | |_) | __ _ _ __ | | __     | | __ _ _ __   __| | ___ 
  |  _ < / _` | '_ \| |/ /     | |/ _` | '_ \ / _` |/ _ \
  | |_) | (_| | | | |   <      | | (_| | | | | (_| |  __/
  |____/ \__,_|_| |_|_|\_\     |_|\__,_|_| |_|\__,_|\___|
)" << std::endl;
    std::cout << "Welcome to bank Trade!" << std::endl; 
}

LoginType UI::Login(const std::shared_ptr<Net> & client_net){
    auto changeLoginType = [&client_net]()->LoginType{
        std::cout << "input type (1 - 3) to change your login type" << std::endl;
        auto inputWay = [&]()->LoginType{
            std::string username;
            std::string password;
            std::string r_password;
            bool input_flag = false;
            bool quit_button = false;
            while(!input_flag && !quit_button){
                std::cout << "Enter username(Input <no-name> to quit) : \n"; 
                std::getline(std::cin, username);
                if(username == "<no-name>"){
                    quit_button = true;
                    break;
                }
                std::cout << "Enter password : \n";
                std::getline(std::cin, password);
                std::cout << "Again password : \n";
                std::getline(std::cin, r_password);
                if(password.empty() || password != r_password){
                    std::cout << "password is not same";
                    username.clear();
                    password.clear();
                    r_password.clear();
                    continue;
                }
                std::string login_info = std::format("username[{}]password[{}]",username, r_password);
                client_net->Send(login_info);
                // actually, need to judge the server responed, but no server now
                std::cout << "Login Successfully!" <<client_net->Receive() << std::endl;
                input_flag = true;
            }
            return quit_button ? LoginType::QUIT : LoginType::INPUT;
        };
        auto faceWay = []()->LoginType{
            std::cout << "scan your face." << std::endl;
            return LoginType::SCANFACE; 
        };
        auto fingerWay = []()->LoginType{
            std::cout << "put your finger." << std::endl;
            return LoginType::FINGER;
        };
        int login_type = 1;
        std::cin >> login_type;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        if(1 == login_type){
            return inputWay();
        }else{
            switch (login_type)
            {
            case 2:
                return faceWay();
            case 3:
                return fingerWay();
            default:
                return LoginType::WRONG;
            }
        }

        return LoginType::QUIT;
    };

    LoginType res_type = LoginType::QUIT;

    while(static_cast<bool>(res_type = changeLoginType()) && res_type == LoginType::QUIT);

    return res_type;
}


void UI::Title(const std::shared_ptr<Client> & client){
    std::cout << "Welcome " << client->GetName() << std::endl;
}


void UI::Run(){
    while(true){
        int transcation_type;
        std::cout << "Enter the number of your transcation type (1 - 5)" << std::endl;
        std::cout << "Enter 0 to exit." << std::endl;
        std::cin >> transcation_type;
        if(0 == transcation_type){
            Exit();
            break;
        }
        switch(transcation_type){
            // case 1:
            //     Transfer();
            //     break;
            case 2:
                SaveCash();
                break;
            // case 3:
            //     WithdrawCash();
            //     break;
            default:
                std::cout << "Invalid input. Please try again." << std::endl;
                continue;
        }
    }
}

void UI::SaveCash(){
    std::cout << "You selected Save Cash." << std::endl;
    int input_type;
    std::cin >> input_type;
    std::cout << "Input 1 represent scan Id card, 2 represent input by basic information." << std::endl;
    Customer temp_customer;
    std::string input_id;
    if(1 == input_type){
        std::cout << "Please scan your Id card (input your Id): ";
        std::getline(std::cin, input_id);
    }else if(2 == input_type){
        // input by basic information
    }else{
        std::cout << "Invalid input. Returning to main menu." << std::endl;
        return;
    }
    //send customer id to server check if exist or not
    //if exist, proceed to next step
    //if not exist, prompt to create new customer or return to main menu

}

Customer UI::InputCustomerInfo(){
    int age;
    double deposit;
    std::string id;
    std::wstring name;
    std::string foreign_name;
    std::string id_number;
    std::string card_number;
    std::wstring localtion;

    std::cout << "Please enter customer information." << std::endl;

    std::cout << "Age: ";
    std::cin >> age;

    std::cout << "Deposit: ";
    std::cin >> deposit;

    std::cout << "ID (leave empty to auto-generate): ";
    std::cin.ignore(); // Clear the newline character from the input buffer
    std::getline(std::cin, id);

    std::wcout << "Name: ";
    std::getline(std::wcin, name);

    std::cout << "Foreign Name: ";
    std::getline(std::cin, foreign_name);

    std::cout << "ID Number: ";
    std::getline(std::cin, id_number);

    std::cout << "Card Number: ";
    std::getline(std::cin, card_number);

    std::wcout << "Location: ";
    std::getline(std::wcin, localtion);

    Customer customer(age, deposit, id, name, foreign_name, id_number, card_number, localtion);
    return customer;
}


void UI::ClearScreen(){
    system("clear");
}

void UI::Exit(){
    std::cout << "Exiting the program. See you next time!"  << std::endl;
    ClearScreen();
}