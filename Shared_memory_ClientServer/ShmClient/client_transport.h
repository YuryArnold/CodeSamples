#ifndef CLIENT_TRANSPORT_H
#define CLIENT_TRANSPORT_H
#include "config.h"
#include "shmcircularbuffer.h"
#include "concrete_client_sender.h"
#include <memory>
#include <mutex>


class Client_transport
{
public:
    Client_transport();
    ~Client_transport();
    void create_concrete_clients();
    void send_data_from_config(const char* path_to_file);
    void open();
    void close();

private:
    std::shared_ptr<ShmCircularBuffer> buffer;
    config config_conationer;
    std::vector<std::shared_ptr<concrete_client_sender>> clients;
    std::mutex shared_mtx;
};

#endif // CLIENT_TRANSPORT_H
