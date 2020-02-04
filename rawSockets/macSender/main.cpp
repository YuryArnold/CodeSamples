#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>


#define BUF_SIZ		1024

enum macType{
    localMac,
    remoteMac,
};

enum timerType{
    timerForWriting,
    timerForSending,
};

/**
 * @brief The dataForChannel struct
 * This structure was created for
 *  transfering interesting data
 * in it
 */
struct dataForChannel{
    u_int8_t macSrc[8];
    u_int8_t macDst[8];
    double timerForWriting;
    double timerForSending;
    char* ethId;
};

/**
 * @brief allArgs
 * vector of input arguments as string
 * it easy to parse it.
 */

dataForChannel inputData;
int lengthOfMessageHeader = 0;
int sockfd;
sockaddr_ll socket_address;
char sendbuf[BUF_SIZ];


/**
 * @brief split
 * @param line
 * @param delim
 * @return
 */
std::vector<std::string> split(std::string& line, char delim){

    std::vector<std::string> splitted;
    std::stringstream tmpString(line);
    std::string item;
    while(std::getline(tmpString,item,delim)){
        splitted.push_back(item);
    }
    return splitted;
}



/**
 * @brief checkForCorrectMac
 * @param lineForParsing
 * @param input
 * @return
 */
bool checkForCorrectMac(std::string& lineForParsing,dataForChannel& input,macType mac){
    //For checking have to be regular expression and convertion data to array
    std::regex macRegex("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$");
    if(!std::regex_match(lineForParsing,macRegex))
        return false;

    char delim = ':';
    std::vector<std::string> splitted = split(lineForParsing,delim);
    std::vector<u_int8_t> macAsDigits;


    for(auto elem: splitted)
        macAsDigits.push_back(std::stoi(elem,nullptr,16));


    switch(mac){
        case localMac:{
                std::copy(macAsDigits.begin(),macAsDigits.end(),inputData.macSrc);
                break;
            }

        case remoteMac:{
            std::copy(macAsDigits.begin(),macAsDigits.end(),inputData.macDst);
            break;
        }
    }

    return true;
}

/**
 * @brief getTimeFromString
 * @param lineForParsing
 * @param input
 * @param tmType
 * @return
 */
bool getTimeFromString(std::string& lineForParsing,dataForChannel& input,timerType tmType)
{
    std::regex macRegex("\\d+(\\.\\d+)?");
    if(!std::regex_match(lineForParsing,macRegex))
        return false;

    switch(tmType){
        case timerForWriting:{
                inputData.timerForWriting = std::stol(lineForParsing);
                break;
            }

        case timerForSending:{
                inputData.timerForSending = std::stol(lineForParsing);
                break;
        }
    }

    return true;

}




bool checkForCorrectInputString(int argc, char *argv[],dataForChannel& parsedData)
{
    if(argc != 8){
        std::cerr<<"Wrong number of Input parameters"<<std::endl;
    }

    std::vector<std::string> allArgs;
    //set all lines into one string array
    allArgs.assign(argv + 1, argv + argc);


    if(!checkForCorrectMac(allArgs[2],parsedData,macType::localMac))
        perror("Wrong mac address for sending or receveing!");

    if(!checkForCorrectMac(allArgs[3],parsedData,macType::remoteMac))
        perror("Wrong mac address for sending or receveing!");

    if(!)

    //Check for every string correct input
    return true;
}

/**
 * @brief writeToFile
 * Here have to be writing data
 * to file function, just isn't realized
 *
 *
 */
void writeToFile()
{

}

/**
 * @brief setDataToChanel
 * This function called with a timer
 * to transfer data through channel
 */
void setDataToChanel()
{

    timeval tv;
    gettimeofday(&tv,NULL);
    int tx_lenSend = lengthOfMessageHeader;
    /*Sending and rewriting data, to informational array */
    memcpy((&sendbuf[tx_lenSend]),(&tv),sizeof(timeval));
    tx_lenSend += sizeof(timeval);

    if (sendto(sockfd, sendbuf, tx_lenSend, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
        printf("Send failed\n");
}



/**
 * @brief startTimer
 * This function initialize
 * the timer for sending
 * message to receiver, it initialize
 * the signal when the time comes.
 * @param seconds - double that represent
 * floating point seconds in here.
 * @return
 */
bool startTimer(double seconds)
{
    itimerval it_val;
    double integer, fractional;

    integer = (int)seconds;
    fractional = seconds - integer;

    it_val.it_value.tv_sec = integer;
    it_val.it_value.tv_usec = fractional * 1000000;
    it_val.it_interval = it_val.it_value;

    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1)
        return false;

    return true;
}



int main(int argc, char *argv[])
{
    //if(argc != 6){
    //    perror("Wrong number of Input parameters");
    //    return 1;
    //}

    ifreq if_idx;
    ifreq if_mac;
    char sendbuf[BUF_SIZ];
    ether_header *eh = (struct ether_header *) sendbuf;
    struct iphdr *iph = (struct iphdr* ) (sendbuf + sizeof(struct ether_header));

    char ifName[IFNAMSIZ];
    if (!checkForCorrectInputString(argc,argv,inputData)){
        std::cerr<<"Parameter is passed with mistake"<<endl;
        std::exit(1);

    }


    /* Open RAW socket to send on */
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1){
        std::cerr<<"socket"<<endl;
    }

    /* Get the index of the interface to send on */
    memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
       perror("SIOCGIFINDEX");

    /* Get the MAC address of the interface to send on */
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
       perror("SIOCGIFHWADDR");

    /* Construct the Ethernet header */
    memset(sendbuf, 0, BUF_SIZ);

    /* Ethernet header */
    memcpy(eh->ether_shost,&(inputData.macSrc),6);
    memcpy(eh->ether_dhost,&(inputData.macDst),6);


    /* Ethertype field */
    eh->ether_type = htons(ETH_P_IP);
    lengthOfMessageHeader += sizeof(struct ether_header);

    /* Index of the network device */
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    /* Address length*/
    socket_address.sll_halen = ETH_ALEN;
    /* Destination MAC */
    memcpy(socket_address.sll_addr,&(inputData.macDst),6);

    if (signal(SIGALRM, (void(*)(int))setDataToChanel) == SIG_ERR){
        perror("Unable to catch SIGALRM");
        return 1;
    }

    //Transfer time for timer for asking
    startTimer(std::stol(allArgs[5]));


    return 0;
}
