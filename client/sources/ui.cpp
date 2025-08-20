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
}