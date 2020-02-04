#include "client_transport.h"

Client_transport::Client_transport()
{
    buffer = std::make_shared<ShmCircularBuffer>(TypeOfBuffer::ClientBuffer);
}

Client_transport::~Client_transport()
{
    //close();
}

void Client_transport::create_concrete_clients()
{
   int numb_of_clients = config_conationer.files_count();
   for(int client_cnt = 0; client_cnt< numb_of_clients; ++client_cnt){
       auto tmp = std::make_shared<concrete_client_sender>(&shared_mtx,
                                                           buffer.get(),
                                                           config_conationer.client_id());


       tmp->set_path_to_file(config_conationer.file(client_cnt));
       clients.push_back(tmp);
   }

}

void Client_transport::send_data_from_config(const char *path_to_file)
{
    if(0 == config_conationer.load(path_to_file))
        create_concrete_clients();
    else
        throw "Error in opening input file";

}


void Client_transport::open()
{
    buffer->open();
}

void Client_transport::close()
{
    for(auto& client:clients)
        client->set_force_quit(true);

    buffer->close();
}

