#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>



#define ETHER_TYPE	0x0800
#define DEFAULT_IF	"eth5"
#define BUF_SIZ		1024


enum LoopState{
    repeat,
    done
};

struct dataForChannel{
    u_int8_t macSrc[8];
    u_int8_t macDst[8];
    double timer;
    char* ethId;
};

dataForChannel inputData;
std::ofstream logFile;
std::vector<std::string> allArgs;

/**
 * @brief arraysAreEqual
 * @param first
 * @param second
 * @param len
 * @return
 */
bool arraysAreEqual(u_int8_t* first, u_int8_t* second, int len){
    while(len){
        if(first[len] != second[len])
            return false;
        len--;
    }
        return true;
}


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


void writeDataToFile(uint64_t& milliseconds){

    if(!logFile.is_open()){
         logFile.open(allArgs[6]);
         logFile<<"Creating file for writing cahnnel delay\n";
         logFile<<milliseconds<<endl;
    } else {
        logFile<<"delay for"<<milliseconds<<endl;
    }
}


/**
 * @brief checkForCorrectMac
 * @param lineForParsing
 * @param input
 * @return
 */
bool checkForCorrectMac(std::string& lineForParsing,dataForChannel& input, macType mac){
    //For checking have to be regular expression and convertion data to array
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


bool checkForCorrectInputString(int argc, char *argv[],dataForChannel& parsedData){
    if(argc != 6){
        std::cerr<<"Wrong number of Input parameters"<<std::endl;
    }

    //set all lines into one string array
    allArgs.assign(argv + 1, argv + argc);


    if(!checkForCorrectMac(allArgs[1],parsedData,macType::localMac))
        perror("Wrong mac address for sending or receveing!");

    if(!checkForCorrectMac(allArgs[1],parsedData,macType::remoteMac))
        perror("Wrong mac address for sending or receveing!");


    //Check for every string correct input
    return true;
}



int main(int argc, char *argv[])

{

    char sender[INET6_ADDRSTRLEN];
    int sockfd, ret, i;
    int sockopt;
    ssize_t numbytes;
    ifreq ifopts;	/* set promiscuous mode */
    ifreq if_ip;	/* get ip addr */
    sockaddr_storage their_addr;
    uint8_t buf[BUF_SIZ];
    char ifName[IFNAMSIZ];
    bool canLoop = true;
    LoopState insideState = repeat;

    if (!checkForCorrectInputString(argc,argv,inputData)){
        perror("Parameter is passed with mistake");
        return 1;
    }


    /* Header structures */
    struct ether_header *eh = (struct ether_header *) buf;
    struct iphdr *iph = (struct iphdr *) (buf + sizeof(struct ether_header));
    struct udphdr *udph = (struct udphdr *) (buf + sizeof(struct iphdr) + sizeof(struct ether_header));

    memset(&if_ip, 0, sizeof(struct ifreq));

    /* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
    if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1) {
        perror("listener: socket");
        return -1;

    }

    /* Set interface to promiscuous mode - do we need to do this every time? */
    strncpy(ifopts.ifr_name, ifName, IFNAMSIZ-1);
    ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
    ifopts.ifr_flags |= IFF_PROMISC;
    ioctl(sockfd, SIOCSIFFLAGS, &ifopts);
    /* Allow the socket to be reused - incase connection is closed prematurely */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) {
        perror("setsockopt");
       // close(sockfd);
        exit(EXIT_FAILURE);
    }

    /* Bind to device */
    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)	{
        perror("SO_BINDTODEVICE");
       // close(sockfd);
        exit(EXIT_FAILURE);
    }

      size_t indTmp = sizeof(struct ether_header);

      while(canLoop){
          switch(insideState){
                case repeat:{

                  std::cout<<("listener: Waiting to recvfrom...\n");
                  numbytes = recvfrom(sockfd, buf, BUF_SIZ, 0, NULL, NULL);
                  printf("listener: got packet %lu bytes\n", numbytes);

                  /* Check the packet is for me */

                  if (arraysAreEqual(eh->ether_dhost,inputData.macDst,6)){
                      std::cout<<("Correct destination MAC address\n");

                  } else {

                      printf("Wrong destination MAC: %x:%x:%x:%x:%x:%x\n",
                                      eh->ether_dhost[0],
                                      eh->ether_dhost[1],
                                      eh->ether_dhost[2],
                                      eh->ether_dhost[3],
                                      eh->ether_dhost[4],
                                      eh->ether_dhost[5]);
                      ret = -1;
                      insideState = done;
                      /*Have to be stopen instead of*/
                      break;
                  }

                  /* Get source IP */
                  ((struct sockaddr_in *)&their_addr)->sin_addr.s_addr = iph->saddr;
                  inet_ntop(AF_INET, &((struct sockaddr_in*)&their_addr)->sin_addr, sender, sizeof sender);


                  /* Look up my device IP addr if possible */
                  strncpy(if_ip.ifr_name, ifName, IFNAMSIZ-1);
                  if (ioctl(sockfd, SIOCGIFADDR, &if_ip) >= 0) { /* if we can't check then don't */
                      printf("Source IP: %s\n My IP: %s\n", sender,
                              inet_ntoa(((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr));

                      /* ignore if I sent it */
                      if (strcmp(sender, inet_ntoa(((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr)) == 0)	{
                          printf("but I sent it :(\n");
                          ret = -1;
                          insideState = done;
                          break;

                      }

                  }

                  /* UDP payload length */
                  ret = ntohs(udph->len) - sizeof(struct udphdr);
                  timeval tv;
                  tv = *(reinterpret_cast<timeval*>(&buf[indTmp]));
                  uint64_t timeOfSending = (tv.tv_sec*(uint64_t)1000) + (tv.tv_usec/1000);
                  gettimeofday(&tv,nullptr);
                  uint64_t timeOfReceveing = (tv.tv_sec*(uint64_t)1000) + (tv.tv_usec/1000);


                  insideState = done;
                  break;
                }

                case done:{
                    /*Sometime I have to close socket*/
                    insideState = repeat;
                    //copy this from example
                    return ret;
                    break;
                }
          }
      }
}
