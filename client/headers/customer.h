#pragma once

#include <string>
#include <utility>
#include <iostream>
#include <format>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <chrono>
#include <ctime>
#include <random>

class Customer{
    public:
        Customer(
            int age,
            double deposit,
            std::string id,
            std::wstring name,
            std::string foreign_name,
            std::string id_number,
            std::string card_number,
            std::wstring localtion
        );
        Customer() = default;
        Customer(const Customer &) = default;
        Customer & operator=(const Customer &) = default;


        Customer (Customer &&);
        Customer & operator=(Customer &&);

        
        ~Customer() = default;

        std::string serialize() const;
        void deserialize(const std::string & jsonStr);

        
    private:
        int c_age;
        double c_deposit;
        std::string c_id;
        std::wstring c_name;
        std::string c_foreign_name;
        std::string c_id_number;
        std::string c_card_number;
        std::wstring c_localtion;

        static int c_id_counter;

};
