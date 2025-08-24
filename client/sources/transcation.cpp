#include "transcation.h"

std::string saveCash::serialize() const {
    boost::property_tree::ptree pt;
    pt.put("customer", c_transcation_customer.serialize());
    pt.put("transcation_id", s_transcation_id);
    pt.put("amount",s_amount);
    pt.put("date",s_date);

    std::ostringstream buff;
    boost::property_tree::write_json(buff, pt, false);
    return buff.str();
}

void saveCash::deserialize(const std::string & jsonStr){
    std::istringstream buff(jsonStr);
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(buff, pt);
    
    Customer temp_customer;
    temp_customer.deserialize(pt.get<std::string>("customer",""));
    c_transcation_customer = std::move(temp_customer);
    s_transcation_id = pt.get<std::string>("transcation_id","");
    s_amount = pt.get<int>("amount",0);
    s_date = pt.get<std::string>("date","");
}

saveCash::saveCash(const Customer& customer, const std::string& transcation_id, int amount, const std::string& date)
    : c_transcation_customer(customer), s_transcation_id(transcation_id), s_amount(amount), s_date(date)
{
    
}