#include "client_transport.h"

int main(int argc, char *argv[])
{
    Client_transport client;
    client.open();
    try {
        client.send_data_from_config("/home/user/Desktop/RTI_TEST/ShmClient/test.xml");
    } catch (...) {
        std::cout<<"Error in config file, please fix it, or use another file"<<std::endl;
    }

    client.close();
    return 0;
}
