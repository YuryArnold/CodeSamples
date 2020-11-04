#include "server_transport.h"

int main()
{
    Server_transport server;
    try {
       server.open();
    } catch (std::string& str) {
        std::cout<<str<<std::endl;
    }

    return 0;
}
