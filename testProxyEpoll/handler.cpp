#include "handler.h"
#include <string.h>
#include <errno.h>
#include "common.h"

Handler::Handler(int _epfd,
                 int _clientId,
                 uint32_t _port,
                 std::string _ip) noexcept :
    m_idClient(_clientId),
    m_epfd(_epfd),
    clientEvent(EPOLLIN),
    serverEvent(EPOLLIN),
    totalRecFromClient(0),
    totalRecFromDb(0)

{
    regEpollFd(m_epfd,m_idClient);
    serverAddrDb.sin_family = PF_INET;
    serverAddrDb.sin_port = htons(_port);
    serverAddrDb.sin_addr.s_addr = inet_addr(_ip.c_str());
    connectToDdServer();
}

Handler::~Handler()
{
    remove();
}

int Handler::getClientId() const
{
    return m_idClient;
}

int Handler::getServerId() const
{
    return  m_idServer;
}

void Handler::remove()
{
    remEpollFd(m_idClient,m_epfd);
    remEpollFd(m_idServer,m_epfd);
    close(m_idClient);
    close(m_idServer);
}

void Handler::connectToDdServer()
{

    m_idServer = socket (AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (m_idServer == -1) {
        std::cerr<<"Socket to DB creation error"<<std::endl;
        return;
    }
    if (connect(m_idServer, (struct sockaddr*) &serverAddrDb, sizeof(serverAddrDb)) == -1) {
        std::cerr<<"Connection error"<<std::endl;
        close(m_idServer);
        return;
    }
    regEpollFd(m_epfd, m_idServer);
}


int Handler::receiveData(char* _buffer,
                         int _sockFdIN,
                         uint32_t& _inEvent,
                         int _sockFdOut,
                         uint32_t& _outEvent,
                         int& _totalLen,
                         std::function<void (char* _buf, int len)> _logCallBack)
{
    if(_totalLen > 0) {
        clearPollEvent(_sockFdIN,EPOLLIN,_inEvent);
        return 1;
    } else {
        setPollEvent(_sockFdOut,EPOLLOUT,_outEvent);
        //bzero(_buffer,BUF_SIZE);
        int len = 0;

        len = recv(_sockFdIN, _buffer, BUF_SIZE, 0);
        //std::cerr<<"recv id: "<<_sockFdIN<<" len "<<len<<std::endl;
        //std::cerr<<"rec message "<<_buffer<<std::endl;

        if(_logCallBack)
            _logCallBack(_buffer,len);

        if(!len)
            return 0;

        if (len == -1 && errno != EAGAIN)
        {
            std::cerr<<"Error in sending socket"<<errno<<std::endl;
            return 0;
        }

        if(len == -1 && errno == EAGAIN)
            return 1;

        _totalLen+=len;
    }

    return _totalLen;
}

int Handler::transmitData(char* _buffer,
                          int _sockFdIN,
                          uint32_t& _inEvent,
                          int _sockFdOut,
                          uint32_t& _outEvent,
                          int& _totalLen)
{
    int len = 0;
    if(!_totalLen){
        clearPollEvent(_sockFdOut,EPOLLOUT,_outEvent);
        return 1;

    } else {
        len=send(_sockFdOut,_buffer, _totalLen, 0);
        std::cerr<<"send id: "<<_sockFdOut<<" len "<<len<<std::endl;


        if(!len)
            return 0;

        if (len ==-1 && errno != EAGAIN){
            std::cerr<<"Error in sended message errno is"<<errno<<std::endl;
            return 0;
        }

        if(len ==-1 && errno == EAGAIN)
            return 1;

        _totalLen -=len;

        if(!_totalLen)
            setPollEvent(_sockFdIN,EPOLLIN,_inEvent);
        else {
            memmove(_buffer,_buffer+len,_totalLen);
        }

    }
    return len;
}

void Handler::setPollEvent(int _fd, uint32_t _event, uint32_t& _state)
{
    _state |= _event;
    changeFdMod(m_epfd, _fd, _state);
}

void Handler::clearPollEvent(int _fd, uint32_t _event, uint32_t& _state)
{
    _state &= ~_event;
    changeFdMod(m_epfd, _fd, _state);
}

int Handler::receiveClientMessage()
{
    return receiveData(m_recv_buf_cl,
                       m_idClient,
                       clientEvent,
                       m_idServer,
                       serverEvent,
                       totalRecFromClient,
                       std::bind(&Parser::parseMessage,m_Prs,
                                 std::placeholders::_1,
                                 std::placeholders::_2));
}

int Handler::receiveDbMessage()
{
    return receiveData(m_recv_buf_db,
                       m_idServer,
                       serverEvent,
                       m_idClient,
                       clientEvent,
                       totalRecFromDb);
}

int Handler::sendMessageToDb()
{
    return transmitData(m_recv_buf_cl,
                        m_idClient,
                        clientEvent,
                        m_idServer,
                        serverEvent,
                        totalRecFromClient);
}

int Handler::sendMessageToClient()
{
    return transmitData(m_recv_buf_db,
                        m_idServer,
                        serverEvent,
                        m_idClient,
                        clientEvent,
                        totalRecFromDb);
}
