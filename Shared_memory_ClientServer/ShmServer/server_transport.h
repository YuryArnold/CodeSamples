#ifndef SERVER_TRANSPORT_H
#define SERVER_TRANSPORT_H
#include "shmcircularbuffer.h"
#include "messagestructures.h"
#include <memory>
#include "concrete_client_receiver.h"
#include <unordered_map>
#include <atomic>
#include <memory>

class Server_transport
{
public:
    Server_transport();
    ~Server_transport();
    void open();
    void close();
    
private:
    void get_message();
    void start_server();
    void stop_server();
    std::shared_ptr<concrete_client_receiver> create_concrete_client(const message&);

private:
    std::shared_ptr<ShmCircularBuffer> buffer;
    std::map<std::pair<uint32_t,uint32_t>,std::shared_ptr<concrete_client_receiver>> clients;
    std::thread thread_for_clients;
    std::atomic<bool> start_server_flag;
    int m_sleep_period;
};
#endif // SERVER_TRANSPORT_H
