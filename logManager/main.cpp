#include "logmanager.h"

void writeAnotherLog1(){
    while(true){
        LOG(GroupId::LOG_MEMORY_MONITORING,LevelOfId::LOG_DEBUG)<<std::u16string(u"Memory leackage")<<std::u16string(u"Some message!");
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void writeAnotherLog2(){
    while(true){
         LOG(GroupId::LOG_SIGNAL_ANALISYS,LevelOfId::LOG_DEBUG)<<std::u16string(u"Error signal")<<std::u16string(u"Some message!");
         std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}



int main(int argc, char *argv[])
{

    std::thread th1(writeAnotherLog1);
    std::thread th2(writeAnotherLog2);

    th1.join();
    th2.join();

    return 0;
}
