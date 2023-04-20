#pragma once
 
#include <string>
#include <memory>
#include <unordered_map>
#include "common.h"

class Handler;
  
// Server class, used to handle client requests
class Server { 
public:
    Server(uint32_t _port, std::string _Ip,
           uint32_t _portHand, std::string _IpHand) noexcept;
    Server (const Server &) = delete;
    Server &operator= (const Server &) = delete;
    void Init(); 
    void Close(); 
    void Start();
    void addHandler(int _sockfd);
    void handleEpollEventIN(int _sockfd);
    void handleEpollEventOUT(int _sockfd);

private:
    struct sockaddr_in serverAddr;
    int m_Listener;
    int m_epFd;
    uint32_t m_handPort;
    std::string m_handIp;
    std::string m_localIp;
    std::unordered_map<int, std::shared_ptr<Handler>> m_handlersClient;
    std::unordered_map<int, std::shared_ptr<Handler>> m_handlersServer;
};

