#include "client_transport.h"

int main(int argc, char *argv[])
{
    Client_transport client;

    try {
        client.open();
        client.send_data_from_config("../.../test.xml");
    }  catch (std::string errStr) {
        std::cout<<errStr<<std::endl;
    }   catch (...) {
        std::cout<<"Error in config file, please fix it, or use another file"<<std::endl;
    }

    client.close();
    return 0;
}
