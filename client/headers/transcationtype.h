#pragma once

#include <string>

enum class TranscationType {
    SAVE_CASH,
    WITHDRAW_CASH,
    TRANSFER
};

class TranscationCode{
    public:
        static std::string GetCode(TranscationType type){
            switch(type){
                case TranscationType::SAVE_CASH:
                    return "SAVE_CASH";
                case TranscationType::WITHDRAW_CASH:
                    return "WITHDRAW_CASH";
                case TranscationType::TRANSFER:
                    return "TRANSFER";
                default:
                    return "UNKNOWN";
            }
        }
};

