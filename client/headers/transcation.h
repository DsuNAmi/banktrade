#pragma once

#include "customer.h"



#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class Transcation {
public:
    virtual ~Transcation() = default;

    // 父类声明虚的序列化接口
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& jsonStr) = 0;
};

class saveCash : Transcation{
    public:
        std::string serialize() const override;
        void deserialize(const std::string& jsonStr) override;
        saveCash() = default;
        saveCash(const Customer& customer, const std::string& transcation_id, int amount, const std::string& date);
        virtual ~saveCash() = default;

        saveCash(const saveCash &) = delete;
        saveCash & operator=(const saveCash &) = delete;
    private:
        Customer c_transcation_customer;
        std::string s_transcation_id;
        int s_amount;
        std::string s_date;
};