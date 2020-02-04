#ifndef LOGMANAGER_H
#define LOGMANAGER_H
#include <mutex>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iostream>
#include <condition_variable>
#include <queue>
#include <thread>
#include <string>
#include <memory>
#include <algorithm>
#include <codecvt>
#include <locale>

enum LevelOfId{
    LOG_DEBUG,
    LOG_INFO,
    LOG_MESSAGE,
    LOG_MAJOR,
    LOG_ERROR,
};

enum GroupId{
    LOG_MEMORY_MONITORING,
    LOG_SIGNAL_ANALISYS
};

class bufferForMessage;

bufferForMessage LOG(GroupId id, LevelOfId levelId);

class logManager
{

private:

    logManager():threadForWriting(&logManager::writeDataIntoFile, this){
        canWriteLog = true;
        timeOfStartWritingInFile = std::time(nullptr);
        lz_timeOfStartWritingIntoFile = std::localtime(&timeOfStartWritingInFile);
        setBomHeader = false;
    }

    ~logManager(){
        std::unique_lock<std::mutex>(mtx);
        canWriteLog = false;
        m_QueueCheck.notify_one();
        if(threadForWriting.joinable())
            threadForWriting.join();
        if(m_LogFile.is_open())
            m_LogFile.close();
    }

    void createNameOfFile();
    logManager(const logManager&) = delete;
    logManager& operator=(logManager&) = delete;
    void writeDataIntoFile();
    friend class bufferForMessage;
    void checkForNextDay();
public:
    static logManager& getInstance(){
        static logManager instance;
        return instance;
    }
private:
    std::mutex mtx;
    std::string m_FileName;
    std::stringstream informString;
    std::ofstream m_LogFile;
    std::condition_variable m_QueueCheck;
    std::queue<std::shared_ptr<std::stringstream>> m_MessageContainer;
    std::thread threadForWriting;
    bool canWriteLog;
    std::time_t timeOfStartWritingInFile;
    std::tm * lz_timeOfStartWritingIntoFile;
    bool setBomHeader;

};


class bufferForMessage{
public:

    bufferForMessage(GroupId _id, LevelOfId _levelId, logManager& loger ) : logManagerInst(loger),id(_id),levelId(_levelId),ptrDuringString(std::make_shared<std::stringstream>()){

        firstMessage = false;
        auto getDuringTimeAsString = [&] () {
            using namespace std::chrono;
            auto time = system_clock::now();
            auto t_time = system_clock::to_time_t(time);
            system_clock::duration fraction = time.time_since_epoch();
            fraction -= duration_cast<seconds>(fraction);
            std::string tmpString = std::ctime(&t_time);
            tmpString.pop_back();
            std::string tmpString2 = std::to_string(static_cast<unsigned>(fraction / milliseconds(1)));
            tmpString2.insert(0,":");
            tmpString.insert(19,tmpString2);
            tmpString.push_back(' ');
            return tmpString;
        };

        (*ptrDuringString)<<getDuringTimeAsString()<<" @ ";
        std::thread::id this_id = std::this_thread::get_id();
        (*ptrDuringString)<<"Thread id is: "<<this_id<<" @ ";
    }

    ~bufferForMessage(){
        (*ptrDuringString)<<"\r\n";
        std::unique_lock<std::mutex> guard(logManagerInst.mtx);
        logManagerInst.m_MessageContainer.push(ptrDuringString);
        logManagerInst.m_QueueCheck.notify_one();
    }

    void setLogID(std::string& message){

            switch (id){
                case LOG_MEMORY_MONITORING:
                    (*ptrDuringString)<<std::string("Memory monitoring type: ");
                    break;

                case LOG_SIGNAL_ANALISYS:
                    (*ptrDuringString)<<std::string("Signal analisys type: ");
                    break;

                default:
                    throw "Error in incoming message, incorrect type of Group Id!";
            };

            (*ptrDuringString)<< message <<" @ "<<"Type of message: ";

            switch (levelId){
                case LOG_DEBUG:
                    (*ptrDuringString)<<std::string("debug");
                    break;

                case LOG_INFO:
                    (*ptrDuringString)<<std::string("info");
                    break;

                case LOG_MESSAGE:
                    (*ptrDuringString)<<std::string("message");
                    break;

                case LOG_MAJOR:
                    (*ptrDuringString)<<std::string("major");
                    break;

                case LOG_ERROR:
                    (*ptrDuringString)<<std::string("error");
                    break;

            default:
                throw "Error in incoming message, incorrect type of Level Id!";
            }

            (*ptrDuringString)<<" @ ";

    }


    /**
     * @brief operator << This method takes utf-16 symbol for input from the box
     * and automatically convert them into utf-8, after it ,
     * method use this data for writing into log.
     * @param message - std::u16string incoming string
     * @return
     */
    bufferForMessage& operator<<(std::u16string message){
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> conversion;
        std::string utf8message = conversion.to_bytes( message );

        if(!firstMessage){
            setLogID(utf8message);
            firstMessage = true;
        } else {
            (*ptrDuringString)<<utf8message<<" ";
        }
        return *this;
    }

private:
    std::shared_ptr<std::stringstream> ptrDuringString;
    logManager& logManagerInst;
    GroupId id;
    LevelOfId levelId;
    bool firstMessage;
};



#endif // LOGMANAGER_H
