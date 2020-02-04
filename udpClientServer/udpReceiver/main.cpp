#include <iostream>
#include <vector>
#include <thread>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "udpreceiver.h"


int main(int argc, char *argv[])
{

    UdpReceiver udpRec("127.0.0.1",5557);

    return 0;
}
