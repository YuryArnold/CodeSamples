#include <iostream>
#include <cstdint>
#include "server.h"
#include "handler.h"


Server::Server(uint32_t _port,
               std::string _Ip,
               uint32_t _portHand,
               std::string _IpHand) noexcept :
    m_Listener(0),
    m_epFd(0),
    m_handPort(_portHand),
    m_handIp(_IpHand)
{
    
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(_port);
    serverAddr.sin_addr.s_addr = inet_addr(_Ip.c_str());
    m_localIp = _Ip;
    m_Listener = 0;
    m_epFd = 0;

}

void Server::Init() {
    std::cout << "Init Server..." << std::endl;
    
    m_Listener = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
    if(m_Listener < 0)  {
        std::cerr<<" Cant listener open socket for listening"<<std::endl;
        exit(-1);
    }
    std::cerr<<"Listen id "<<m_Listener<<std::endl;

    int flag = 1;
    if(setsockopt(m_Listener, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1) {
            std::cerr<< "ERROR: Can't set SO_REUSEADDR"<<std::endl;
            exit(EXIT_FAILURE);
    }


    if( bind(m_Listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind error");
        exit(-1);
    }

    int ret = listen(m_Listener, 5);
    if(ret < 0) {
        std::cerr<<"listen error"<<std::endl;
        exit(-1);
    }

    std::cout << "Start to listen: " << m_localIp << std::endl;
    m_epFd = epoll_create (EPOLL_SIZE);
    
    if(m_epFd < 0) {
        std::cerr<<"epfd error"<<std::endl;
        exit(-1);
    }

    regEpollFd(m_epFd, m_Listener);
}

void Server::Close() {
    close(m_Listener);
    close(m_epFd);
}

// Start the server
void Server::Start() {

    static struct epoll_event events[EPOLL_SIZE];

    Init();

    while(1)
    {
        //epoll_events_count indicates the number of ready events
        int epoll_events_count = epoll_wait(m_epFd, events, EPOLL_SIZE, -1);

        if(epoll_events_count < 0) {
            std::cerr<<"epoll failure"<<std::endl;
            break;
        }

        //Deal with this epoll_events_count ready events
        for(int i = 0; i < epoll_events_count; ++i)
        {
            int sockfd = events[i].data.fd;
            uint32_t event = events[i].events;

            if(sockfd == m_Listener && event&EPOLLIN)
            {
                addHandler(sockfd);

            } else {


                if(event&EPOLLIN) {

                    handleEpollEventIN(sockfd);

                }

                if(event&EPOLLOUT) {

                    handleEpollEventOUT(sockfd);

                }

                if(!(event&(EPOLLIN|EPOLLOUT))) {
                    std::cerr<<"Unknown event"<<std::endl;
                }

            }
        }
    }


    // Shut down the service
    Close();
}

void Server::addHandler(int _sockfd)
{
    int clientfd;
    struct sockaddr_in client_address;
    socklen_t client_addrLength = sizeof(struct sockaddr_in);
    while ((clientfd = accept(_sockfd,(struct sockaddr *) &client_address,
                              (socklen_t *)&client_addrLength)) > 0) {
        auto handler = std::make_shared<Handler>(m_epFd,
                                                 clientfd,
                                                 m_handPort,
                                                 m_handIp);
        m_handlersClient[handler->getClientId()] = handler;
        m_handlersServer[handler->getServerId()] = handler;
        std::cerr<<"server id "<<handler->getServerId()<<" cient id "<<handler->getClientId()<<std::endl;
        std::cout << "client connection from: "
             << inet_ntoa(client_address.sin_addr) << ":"
             << ntohs(client_address.sin_port) << ", clientfd = "
             << clientfd << std::endl;
    }
}

void Server::handleEpollEventIN(int sockfd)
{
    if(m_handlersClient.find(sockfd) != m_handlersClient.end()){
        int resultLen = m_handlersClient[sockfd]->receiveClientMessage();
        if(!resultLen){
            std::cerr<<"Close socket for in"<<std::endl;
            m_handlersServer.erase(m_handlersClient[sockfd]->getServerId());
            m_handlersClient.erase(sockfd);
        }

    } else if(m_handlersServer.find(sockfd) != m_handlersServer.end()){
        int resultLen = m_handlersServer[sockfd]->receiveDbMessage();
        if(!resultLen){
            std::cerr<<"Close socket for in"<<std::endl;
            m_handlersClient.erase(m_handlersServer[sockfd]->getClientId());
            m_handlersServer.erase(sockfd);
        }

    } else {
        std::cerr<<" No such Socket fd "<<sockfd<<" for incomming event"<<std::endl;

    }
}

void Server::handleEpollEventOUT(int sockfd)
{
    if(m_handlersClient.find(sockfd) != m_handlersClient.end()){
        int resultLen = m_handlersClient[sockfd]->sendMessageToClient();//sendMessageToDb();
        if(!resultLen){
            std::cerr<<"Close socket for out"<<std::endl;
            m_handlersServer.erase(m_handlersClient[sockfd]->getServerId());
            m_handlersClient.erase(sockfd);
        }

    } else if(m_handlersServer.find(sockfd) != m_handlersServer.end()){
        int resultLen = m_handlersServer[sockfd]->sendMessageToDb();//sendMessageToClient();
        if(!resultLen){
            std::cerr<<"Close socket for out"<<std::endl;
            m_handlersClient.erase(m_handlersServer[sockfd]->getClientId());
            m_handlersServer.erase(sockfd);
        }

    } else {
        std::cerr<<" No such Socket fd "<<sockfd<<" for incomming event"<<std::endl;
    }
}
