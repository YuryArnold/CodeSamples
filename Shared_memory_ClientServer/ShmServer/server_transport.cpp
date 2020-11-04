#include "server_transport.h"

Server_transport::Server_transport()
{
     buffer = std::make_shared<ShmCircularBuffer>(TypeOfBuffer::ServerBuffer);
     start_server_flag = false;
     m_sleep_period = 200;
}

Server_transport::~Server_transport()
{
    stop_server();
}

void Server_transport::open()
{
    buffer->open();
    start_server();
}

void Server_transport::close()
{
    buffer->close();
    stop_server();
}

void Server_transport::get_message()
{
    while(start_server_flag){
        message mes = buffer->get_message();
        auto key = std::make_pair(mes.id,mes.client_run_time);
        if(clients.end() == clients.find(key)){
                clients[key] = create_concrete_client(mes);
        }
        clients[key]->set_message_to_container(mes);
    }
}

void Server_transport::start_server()
{
     start_server_flag = true;
     thread_for_clients = std::thread(&Server_transport::get_message,this);
     std::this_thread::sleep_for(std::chrono::milliseconds());
}

void Server_transport::stop_server()
{
    start_server_flag = false;
    if(thread_for_clients.joinable())
            thread_for_clients.join();

    for(auto& client : clients)
        client.second->set_force_quit(true);
}


std::shared_ptr<concrete_client_receiver> Server_transport::create_concrete_client(const message& input)
{
    auto tmp = std::make_shared<concrete_client_receiver>();
    std::string path_to_file = std::to_string(input.id) + "_"
            + std::to_string(input.client_run_time) + ".bin";
    tmp->set_path_to_file(path_to_file.c_str());
    return tmp;
}

