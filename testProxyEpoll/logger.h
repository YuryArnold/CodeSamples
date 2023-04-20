#pragma once
#include <fstream>

class Logger {

    explicit Logger (const char* _fname = "log.txt") {
        m_myFile.open (_fname);
        if (m_myFile.is_open()) {
            m_myFile << "Log file" << std::endl << std::endl;
        }
    }

    ~Logger () {
        if (m_myFile.is_open()) {
            m_myFile << std::endl << std::endl;
            m_myFile.close();
        }
    }

public:
    friend Logger& operator << (Logger &logger, const char *text) {
        logger.m_myFile << text << std::endl;
        return logger;
    }

    static Logger& getLogger(){
        static Logger lg;
        return lg;
    }

    Logger (const Logger &) = delete;
    Logger &operator= (const Logger &) = delete;

private:
    std::ofstream m_myFile;
};


Logger& LOG();
