#include "logmanager.h"

void logManager::writeDataIntoFile(){
    while(canWriteLog){
        {
            std::unique_lock<std::mutex> lck(mtx);
            m_QueueCheck.wait(lck,[&](){
                return (!m_MessageContainer.empty() && canWriteLog);
            });

            /*If container was opened so I can wright data into file, without copying std::stringstream */
            if(m_LogFile.is_open()){
                m_LogFile<<m_MessageContainer.front()->str()<<std::flush;
                m_MessageContainer.pop();
            }
        }
        if(m_FileName.empty())
            createNameOfFile();

        if(!m_LogFile.is_open())
            m_LogFile.open(m_FileName,std::ios_base::app | std::ios_base::out);

        checkForNextDay();
        if(m_LogFile.fail()){
            mtx.unlock();
            std::cerr<<"Log manger error - fail to create memory file!"<<std::endl;
            canWriteLog = false;
            std::exit(1);
        }

        /*Set BOM header for new created text File!*/
        if(!setBomHeader){
            unsigned char bom[] = { 0xEF,0xBB,0xBF };
            m_LogFile.write((char*)bom, sizeof(bom));
            setBomHeader = true;
        }
    }
    m_LogFile.close();
}

void logManager::checkForNextDay(){
    std::time_t tmpTimeOfStartWritingInFile = std::time(nullptr);
    std::tm * tmpLzTs_TimeOfStart = std::localtime(&tmpTimeOfStartWritingInFile);
    if((lz_timeOfStartWritingIntoFile->tm_mday - tmpLzTs_TimeOfStart->tm_mday) > 0){
        timeOfStartWritingInFile = tmpTimeOfStartWritingInFile;
        lz_timeOfStartWritingIntoFile = std::localtime(&timeOfStartWritingInFile);
        m_LogFile.close();
        createNameOfFile();
        m_LogFile.open(m_FileName.c_str());
        setBomHeader = false;
    }
}

bufferForMessage LOG(GroupId id, LevelOfId levelId){
    logManager& log = logManager::getInstance();
    bufferForMessage tmpBuf(id,levelId,log);
    return tmpBuf;
}

void logManager::createNameOfFile(){
    auto time = std::chrono::system_clock::now();
    auto t_time = std::chrono::system_clock::to_time_t(time);
    std::string tmpTimeString =  std::string(std::ctime(&t_time));
    tmpTimeString.pop_back();
    std::replace( tmpTimeString.begin(), tmpTimeString.end(), ' ', '_');
    std::replace( tmpTimeString.begin(), tmpTimeString.end(), ':', '^');
    std::string tmpStringAll = "log_" + tmpTimeString +  ".txt";
    m_FileName.append(tmpStringAll);
}

