#include "client_transport.h"

int main(int argc, char *argv[])
{
     Client_transport client;
     client.open();
     client.send_data_from_config("/home/user/Desktop/RTI_TEST/ShmClient/test.xml");
     /*Let stop this after translation!*/
     //client.close();
     //client.start_client();

    return 0;
}
