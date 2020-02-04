#include "smartbuffer.h"

SmartBuffer::SmartBuffer(QObject *parent) : QObject(parent)
{
    bufferSize = 100;
    numbOfUsedBursts = 0;
    sizeOfBurst = 30;
}

SmartBuffer::~SmartBuffer()
{

}

void SmartBuffer::getBurst(QByteArray& _burst)
{
    QMutexLocker lkc(&mtx);
    numbOfUsedBursts--;
    _burst =  messages.first();
    messages.pop_front();
    if((numbOfUsedBursts == 0) || messages.isEmpty()){
        emit startWrite();
    }
}

void SmartBuffer::setBurst(QByteArray& _burst)
{
    QMutexLocker lkc(&mtx);
    numbOfUsedBursts++;
    messages.push_back(_burst);
    if(numbOfUsedBursts == 1){
        emit startRead();
    }
}

bool SmartBuffer::canRead()
{
    return (numbOfUsedBursts > 0);
}

int SmartBuffer::getSizeOfFile()
{
    return sizeOfFile;
}

void SmartBuffer::setSizeOfBurst(int _sizeOfBurst)
{
    sizeOfBurst = _sizeOfBurst;
}

int SmartBuffer::getSizeOfBurst()
{
    return  sizeOfBurst;
}

void SmartBuffer::setSizeOfFile(int _sizeOfFile)
{
    sizeOfFile = _sizeOfFile;
}


bool SmartBuffer::canWrite()
{
    return numbOfUsedBursts <= bufferSize;
}

bool SmartBuffer::isEmpty()
{
    return messages.isEmpty();
}


void SmartBuffer::reset()
{
    bufferSize = 100;
    numbOfUsedBursts = 0;
}
