#pragma once
#include <vector>

class Parser
{
    enum States {
        Head,
        Length,
        Message
    };

public:
    Parser() = default;
    ~Parser() = default;
    void parseMessage(char* _messsage, size_t _len);
private:
    States m_state;
};
