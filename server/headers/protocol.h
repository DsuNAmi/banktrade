#pragma once




#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


enum class ProtocolType{
    CONNECT = 1,
    DISCONNECT,
    CUSTOMER,
    TRANSCTION,
    BOARDCAST,
    ERROR,
    UNKNOWN
};


class Protocol{
    public:

        Protocol() = default;

        template<typename BodyType>
        void ConstructMessage(const boost::property_tree::ptree & protocol_tree){
            p_type = protocol_tree.get<ProtocolType>("type",ProtocolType::UNKNOWN);
            if(p_type == ProtocolType::UNKNOWN){return;}
            SetHeader(protocol_tree.get<int>("sub_type", -1));
            SetBody(protocol_tree.get<BodyType>("body").Seralize());
            SetTail(protocol_tree.get<std::string>("client_id",""), 
                    protocol_tree.get<std::string>("extra",""));
            AssembleMessage();
        };


        void SetHeader(int sub_type);
        void SetBody(std::string && body_serailize);
        void SetTail(const std::string & client_id, const std::string & extra);
        void AssembleMessage();
        
        template<typename BodyType>
        void DeconstructMessage(const std::string & message, boost::property_tree::ptree & protocol_tree){
            if(!ParseMessage(message)){
                p_type = ProtocolType::ERROR;
                p_header.clear();
                p_body.clear();
                p_tail.clear();
                return;
            }

            GetHeader(protocol_tree);
            GetBody(protocol_tree);
            GetTail(protocol_tree);

            p_header.clear();
            p_body.clear();
            p_tail.clear();
        }

        bool ParseMessage(const std::string & message);
        void GetHeader(boost::property_tree::ptree & protocol_tree);
        void GetBody(boost::property_tree::ptree & protocol_tree);
        void GetTail(boost::property_tree::ptree & protocol_tree);

        std::string GetMessage() const;

        std::size_t GetLength() const{
            return p_length;
        }

        ProtocolType GetType() const{
            return p_type;
        }
        
    private:
        std::string p_header;
        std::string p_body;
        std::string p_tail;

        std::string p_message;
        
        ProtocolType p_type;
        std::size_t p_length;
};