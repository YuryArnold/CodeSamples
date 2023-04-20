#include "server.h"
 
//Set Parameters to argc,argv, first for server, second to postgres database
int main(int argc, char *argv[]) {

    uint32_t portServer,portDb;
    std::string ipServer,ipDb;
    if(argc < 5){
        std::cout<<"You have to set parameters to progrm: "<<std::endl;
        std::cout<<" -Proxy ip\n -proxy port\n -Postgres server IP\n -Postgres server Port\n"<<std::endl;

    } else {
        ipServer = std::string(argv[1]);
        portServer = std::stoi(argv[2]);
        ipDb = std::string(argv[3]);
        portDb = std::stoi(argv[4]);

        Server server(portServer,
                      ipServer,
                      portDb,
                      ipDb);
        server.Start();
    }

    return 0;
}
