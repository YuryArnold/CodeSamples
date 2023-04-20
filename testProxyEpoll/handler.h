#pragma once
#include <vector>
#include <functional>
#include "parser.h"
#include "common.h"

class Handler
{
public:
    Handler(int _epfd,int _clientId, uint32_t _port, std::string _ip) noexcept;
    virtual ~Handler();
    Handler (const Handler &) = delete;
    Handler &operator= (const Handler &) = delete;

public:
    int getClientId()const;
    int getServerId()const;
    void remove();
    int receiveClientMessage();
    int receiveDbMessage();
    int sendMessageToDb();
    int sendMessageToClient();

private:
    void connectToDdServer();
    void setPollEvent(int _fd, uint32_t _event, uint32_t& _state );
    void clearPollEvent(int _fd, uint32_t _event,  uint32_t& _state);


    int receiveData(char* _buffer,
                    int _sockFdIN,
                    uint32_t& _inEvent,
                    int _sockFdOut,
                    uint32_t& _outEvent,
                    int& _totalLen,
                    std::function<void (char* _buf, int len)> _logCallBack = nullptr);

    int transmitData(char* _buffer,
                      int _sockFdIN,
                      uint32_t& _inEvent,
                      int _sockFdOut,
                      uint32_t& _outEvent,
                      int& _totalLen);



private:
    struct sockaddr_in serverAddrDb;
    int m_idClient;
    int m_idServer;
    int m_epfd;
    uint32_t clientEvent;
    uint32_t serverEvent;
    char m_recv_buf_cl[BUF_SIZE];
    char m_recv_buf_db[BUF_SIZE];
    int totalRecFromClient;
    int totalRecFromDb;
    Parser m_Prs;
};

