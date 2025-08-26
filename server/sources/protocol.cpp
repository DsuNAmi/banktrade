#include "protocol.h"






std::string Protocol::GetMessage() const{
    if(p_message.empty()){
        return "";
    }
    return p_message;
}

void Protocol::SetHeader(int sub_type){
    boost::property_tree::ptree tpt;
    tpt.put("protocol_type",p_type);
    tpt.put("sub_type",sub_type);
    std::ostringstream buff;
    boost::property_tree::write_json(buff, tpt, false);
    p_header = buff.str();
}

void Protocol::SetBody(std::string && body_serailize){
    p_body = std::move(body_serailize);
}

void Protocol::SetTail(const std::string & client_id, const std::string & extra){
    boost::property_tree::ptree tpt;
    tpt.put("client_id",client_id);
    tpt.put("extra",extra);
    std::ostringstream buff;
    boost::property_tree::write_json(buff, tpt, false);
    p_tail = buff.str();
}

void Protocol::AssembleMessage(){
    p_message = std::format("Header[{}]Body[{}]Tail[{}]", p_header, p_body, p_tail);
    p_length = p_message.length();
}


bool Protocol::ParseMessage(const std::string & message){
    p_message.clear();
    if(message.empty()){
        return false;
    }

    auto header_start = message.find("Header[");
    auto header_end = message.find("]Body[");
    auto body_end = message.find("]Tail[");
    auto tail_end = message.find("]", body_end);

    if(header_start == std::string::npos 
        || header_end == std::string::npos 
        || body_end == std::string::npos 
        || tail_end == std::string::npos){
        return false;
    }

    p_header = message.substr(header_start + 7, header_end - (header_start + 7));
    p_body = message.substr(header_end + 6, body_end - (header_end + 6));
    p_tail = message.substr(body_end + 6, tail_end - (body_end + 6));

    return true;
}

void Protocol::GetHeader(boost::property_tree::ptree & protocol_tree){
    std::istringstream buff(p_header);
    boost::property_tree::ptree npt;
    boost::property_tree::read_json(buff, npt);
    p_type = npt.get<ProtocolType>("protocol_type");
    protocol_tree.put("protocol_type",p_type);
    protocol_tree.put("sub_type",npt.get<ProtocolType>("sub_type"));
}

void Protocol::GetBody(boost::property_tree::ptree & protocol_tree){
    if(p_type == ProtocolType::CUSTOMER){
        //Customer 
    }else if(p_type == ProtocolType::TRANSCTION){
        //Transcation
    }else if(p_type == ProtocolType::CONNECT){
        //string Defalut
    }else if(p_type == ProtocolType::DISCONNECT){
        //string Defalut
    }
}

void Protocol::GetTail(boost::property_tree::ptree & protocol_tree){
    std::istringstream buff(p_header);
    boost::property_tree::ptree npt;
    boost::property_tree::read_json(buff, npt);
    protocol_tree.put("client_id",npt.get<std::string>("client_id"));
    protocol_tree.put("extra",npt.get<std::string>("extra"));
}

