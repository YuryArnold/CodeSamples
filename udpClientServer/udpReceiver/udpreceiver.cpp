#include "udpreceiver.h"
#include <cstring>
#include <chrono>
#include <numeric>

UdpReceiver::UdpReceiver(const std::string ipAddr, const uint16_t port):canListenChannel(false),threadForSocket(&UdpReceiver::slotForReceveingData,this)
{
    canListenChannel = true;
    loopForReceiver = true;
    cv.notify_one();

    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    servAddr.sin_port = htons(port);
    servAddr.sin_family = AF_INET;

    // create datagram socket
    if ((sockFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        std::cerr<<"Error: Socket creation failed!"<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    // bind server address to socket descriptor
    if (bind(sockFd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0){
        std::cerr<<"Error : Bind Failed!"<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    findMinMaxInArrayPart = [&](std::promise<std::pair<elemAndIndex,elemAndIndex>>&& pairPromise,int startInd, int endInd){
        auto maxIndIter = std::max_element((receivedData.begin() + startInd), (receivedData.begin() + endInd));
        int elem = *maxIndIter;
        int ind = maxIndIter - receivedData.begin();
        elemAndIndex maxElem(elem,ind);

        auto minIndIter = std::min_element((receivedData.begin() + startInd), (receivedData.begin() + endInd));
        elem = *minIndIter;
        ind = minIndIter - receivedData.begin();
        elemAndIndex minElem(elem,ind);

        pairPromise.set_value(std::make_pair(maxElem,minElem));
    };
     std::cout<<"Receiving data from sender..."<<std::endl;

}

UdpReceiver::~UdpReceiver()
{
    /*All action happends in here because I put thread into condition variable,
     * notified different thread and after notify I passed to destructor!*/
    if(threadForSocket.joinable())
        threadForSocket.join();
    close(sockFd);
}

void UdpReceiver::slotForReceveingData()
{
    /*Number of integers transfered via datagram */
    int portion = 10000;
    /*size of recieved datagram in bytes/*/
    int datagramSize = 40004;
    /*transfer data with 10000 element portions almost 40 kB*/
    char buffer[datagramSize];
    //Wait until socket won't be initialized!
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck,[&](){ return (canListenChannel);});
    while (loopForReceiver){
    // waiting for response
        int numBytesReceived = recvfrom(sockFd, buffer, sizeof(buffer),
                                   MSG_WAITALL, (struct sockaddr*)nullptr, nullptr);


        if(!checkForCorrectEndHeader(buffer) && !checkForCorrectPacketHeader(buffer))
            continue;
        int* bytesAsIntArr = reinterpret_cast<int*>(&buffer[4]);
        /*May be this part could be faster with move constructor!*/
        mtxForContainer.lock();
        receivedData.insert(receivedData.end(),bytesAsIntArr,bytesAsIntArr+portion);
        mtxForContainer.unlock();

        if(checkForCorrectEndHeader(buffer)){
                std::thread(&UdpReceiver::calculateReceivedData,this).detach();
                threadsForMinMax.clear();
                futuresForMinMax.clear();
        }
    }
}

bool UdpReceiver::checkForCorrectPacketHeader(const char *data)
{
    const char* pattern = "mess";
    int lenOfHeader = 4;
    return !strncmp(data,pattern,lenOfHeader);
}

bool UdpReceiver::checkForCorrectEndHeader(const char *data)
{
    const char* pattern = "endm";
    int lenOfHeader = 4;
    return !strncmp(data,pattern,4);
}

float UdpReceiver::calculateMean(std::vector<int> &data)
{
    return float(std::accumulate(data.begin(), data.end(), 0))/data.size();
}

float UdpReceiver::calculateVariance(std::vector<int> &data, const float mean)
{
    float variance = 0.0;
    for(auto& item : data)
        variance += (float)item - mean;
    return variance/=data.size();
}

std::pair<int, int> UdpReceiver::findMinElemAndItsIndex(std::map<int, int> &minInParths)
{
    return *std::min_element(minInParths.begin(),
                                       minInParths.end(),
                                       [](const std::pair<int, int>& p1,
                                          const std::pair<int, int>& p2) {
                                            return p1.first < p2.first; });

}


std::pair<int, int> UdpReceiver::findMaxElemAndItsIndex(std::map<int, int> &maxInParths)
{
    return *std::max_element(maxInParths.begin(),
                                       maxInParths.end(),
                                       [](const std::pair<int, int>& p1,
                                          const std::pair<int, int>& p2) {
                                            return p1.first < p2.first; });

}

void UdpReceiver::calculateReceivedData()
{
    std::lock_guard<std::mutex> lck(mtxForContainer);

    int stepOfInformArray = receivedData.size()/4;

    //Collect all threads and futures in container
    for(int indOfThread = 0; indOfThread < 4; ++indOfThread){
        std::promise<std::pair<elemAndIndex,elemAndIndex>> maxPromise;
        std::future<std::pair<elemAndIndex,elemAndIndex>> maxResult = maxPromise.get_future();
        threadsForMinMax.push_back(std::move(std::thread(findMinMaxInArrayPart,
                                                      std::move(maxPromise),
                                                      indOfThread*stepOfInformArray, //start index of portion
                                                      (indOfThread + 1)*stepOfInformArray - 1) // end index of portion
                                          ));
        threadsForMinMax[indOfThread].detach();
        futuresForMinMax.push_back(std::move(maxResult));
    }

    std::map<int,int>resultMax;
    std::map<int,int>resultMin;
    /*get all answers from every thread! */
    for(auto& futureItem:futuresForMinMax){
        auto minAndMax = futureItem.get();
        resultMax.insert(std::make_pair<int,int>(std::move(minAndMax.first.elem),
                                                 std::move(minAndMax.first.ind)));
        resultMin.insert(std::make_pair<int,int>(std::move(minAndMax.second.elem),
                                                 std::move(minAndMax.second.ind)));
    }

    std::pair<int,int> maxMax = findMaxElemAndItsIndex(resultMax);
    std::cout<<"Maximum element in received array is = "
            <<maxMax.first<<" and it's index = "
            <<maxMax.second<<std::endl;


    std::pair<int,int> minMin = findMinElemAndItsIndex(resultMin);
    std::cout<<"Minimum element in received array is = "
            <<minMin.first<<" and it's index = "
            <<minMin.second<<std::endl;

    /*Sorting array*/
    std::sort(receivedData.begin(), receivedData.end(), [](const int& a, const int& b){
        return a < b;
    });

    float mean = calculateMean(receivedData);
    std::cout<<"Mean of received array is = "<<mean<<std::endl;

    float variance = calculateVariance(receivedData,mean);
    std::cout<<"Variance of received array is = "<<variance<<std::endl;
    /*clearing received data for another time of receveing!*/
    receivedData.clear();
}

