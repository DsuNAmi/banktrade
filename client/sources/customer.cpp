#include "customer.h"


int Customer::c_id_counter = 0;

std::string generate_random_id_by_curtime(){
    auto cur_time = std::chrono::system_clock::now();
    auto ct = std::chrono::system_clock::to_time_t(cur_time);
    std::tm tm;
    localtime_r(&ct,&tm);
    std::stringstream oss;
    oss << std::put_time(&tm, "%Y%m%d%H%M%S");
    std::string time_str = oss.str();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(1,100);
    int random_number = dis(gen);

    return std::to_string(random_number) + time_str;
}

Customer::Customer(
    int age,
    double deposit,
    std::string id,
    std::wstring name,
    std::string foreign_name,
    std::string id_number,
    std::string card_number,
    std::wstring localtion
)
: c_age(age), c_deposit(deposit),
  c_id(std::move(id)), c_name(std::move(name)),
  c_foreign_name(std::move(foreign_name)),
  c_id_number(std::move(id_number)),
  c_card_number(std::move(card_number)),
  c_localtion(std::move(localtion))
{
    ++c_id_counter;
    if(c_id.empty()) {
        c_id = generate_random_id_by_curtime() + std::to_string(c_id_counter);
        std::cout << "Generated ID Randomly!" << std::endl;
    }else{
        
    }
}


Customer::Customer(Customer &&other)
: c_age(other.c_age), c_deposit(other.c_deposit),
  c_id(std::move(other.c_id)), c_name(std::move(other.c_name)),
    c_foreign_name(std::move(other.c_foreign_name)),
    c_id_number(std::move(other.c_id_number)),
    c_card_number(std::move(other.c_card_number)),
    c_localtion(std::move(other.c_localtion))
{
    // Reset the moved-from object
    other.c_age = 0;
    other.c_deposit = 0.0;
    other.c_id.clear();
    other.c_name.clear();
    other.c_foreign_name.clear();
    other.c_id_number.clear();
    other.c_card_number.clear();
    other.c_localtion.clear();
}

Customer & Customer::operator=(Customer &&other) {
    if (this != &other) {
        c_age = other.c_age;
        c_deposit = other.c_deposit;
        c_id = std::move(other.c_id);
        c_name = std::move(other.c_name);
        c_foreign_name = std::move(other.c_foreign_name);
        c_id_number = std::move(other.c_id_number);
        c_card_number = std::move(other.c_card_number);
        c_localtion = std::move(other.c_localtion);

        // Reset the moved-from object
        other.c_age = 0;
        other.c_deposit = 0.0;
        other.c_id.clear();
        other.c_name.clear();
        other.c_foreign_name.clear();
        other.c_id_number.clear();
        other.c_card_number.clear();
        other.c_localtion.clear();
    }
    return *this;
}


std::string Customer::serialize() const {
    boost::property_tree::ptree pt;
    pt.put("age", c_age);
    pt.put("deposit", c_deposit);
    pt.put("id", c_id);
    pt.put("name", std::string(c_name.begin(), c_name.end()));
    pt.put("foreign_name", c_foreign_name);
    pt.put("id_number", c_id_number);
    pt.put("card_number", c_card_number);
    pt.put("localtion", std::string(c_localtion.begin(), c_localtion.end()));

    std::ostringstream buff;
    boost::property_tree::write_json(buff, pt, false);
    return buff.str();
}

void Customer::deserialize(const std::string & jsonStr){
    std::istringstream buff(jsonStr);
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(buff, pt);

    c_age = pt.get<int>("age",0);
    c_deposit = pt.get<double>("deposit",0.0);
    c_id = pt.get<std::string>("id","");
    c_name = std::wstring(pt.get<std::string>("name","").begin(), pt.get<std::string>("name","").end());
    c_foreign_name = pt.get<std::string>("foreign_name","");
    c_id_number = pt.get<std::string>("id_number","");
    c_card_number = pt.get<std::string>("card_number","");
    c_localtion = std::wstring(pt.get<std::string>("localtion","").begin(), pt.get<std::string>("localtion","").end());
}