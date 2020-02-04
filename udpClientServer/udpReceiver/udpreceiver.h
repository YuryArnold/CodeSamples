#ifndef UDPRESEIVER_H
#define UDPRESEIVER_H
#include <thread>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <future>
#include <condition_variable>
#include <mutex>
#include <map>

struct elemAndIndex{
    elemAndIndex(int _elem, int _ind):
        elem(_elem),ind(_ind){

    }
    int elem;
    int ind;
};

class UdpReceiver
{
public:
    UdpReceiver(const std::string ipAddr,const uint16_t port);
    ~UdpReceiver();
    void slotForReceveingData();
    bool checkForCorrectPacketHeader(const char* data);
    bool checkForCorrectEndHeader(const char* data);
    float calculateMean(std::vector<int>& data);
    float calculateVariance(std::vector<int>& data, const float mean);
    std::pair<int,int> findMaxElemAndItsIndex(std::map<int,int>& maxInParths);
    std::pair<int,int> findMinElemAndItsIndex(std::map<int,int>& maxInParths);
    void calculateReceivedData();

private:
    std::mutex mtxForContainer;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread threadForSocket;
    std::vector<int> receivedData;
    std::vector<std::thread> threadsForMinMax;
    std::vector<std::future<std::pair<elemAndIndex,elemAndIndex>>> futuresForMinMax;
    std::function<void(std::promise<std::pair<elemAndIndex,elemAndIndex>>&& pairPromise,int startEnd, int endInd)> findMinMaxInArrayPart;
    struct sockaddr_in servAddr;
    int sockFd;
    bool canListenChannel;
    bool loopForReceiver;
};

#endif // UDPRESEIVER_H
