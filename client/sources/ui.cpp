#include " ui.h"


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

void UI::Title(){
    std::cout << "Please select your transcation:" << std::endl;
    std::cout << std::format("{:-^30}\n","");
    std::cout << std::format("{: <10}1.Transfer\n","");
    std::cout << std::format("{: <10}2.Save Cash\n","");
    std::cout << std::format("{: <10}3.Withdraw Cash\n","");
    std::cout << std::format("{:-^30}\n","");
}


void UI::Run(){
    while(true){
        int transcation_type;
        std::cout << "Enter the number of your transcation type (1 - 3)" << std::endl;
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