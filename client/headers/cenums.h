#pragma once


enum class ErrorType{
    ERROR = 1,
    WARNNING,
    INFO
};

enum class TranscationType{
    COMMIT = 1,
    WRONG,
    STORE,
    ROLLBACK
};

enum class LoginType{
    INPUT = 1,
    SCANFACE,
    FINGER,
    WRONG,
    QUIT
};