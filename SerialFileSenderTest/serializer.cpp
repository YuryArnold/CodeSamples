#include "serializer.h"
#include "smartbuffer.h"

Serializer::Serializer(QObject *parent) : QObject(parent)
{

}

Serializer::~Serializer(){
    if(file.isOpen())
        file.close();
}

void Serializer::setSmartBuffer(SmartBuffer* _buffer)
{
    buffer = _buffer;
    sizeOfBurst = buffer->getSizeOfBurst();
    connect(buffer,&SmartBuffer::startWrite,
            this,&Serializer::burstLoop);
}

/**
 * @brief Serializer::burstLoop
 * this method is analogue of
 * cv for writing.
 */
void Serializer::burstLoop()
{   
    QByteArray burstBuffer;
    while(buffer->canWrite() && file.isOpen() && !file.atEnd()){
       burstBuffer = file.read(sizeOfBurst);
       buffer->setBurst(burstBuffer);
       burstBuffer.clear();
    }
}


/**
 * @brief Serializer::setPathToSendedFileSlot
 * This method work on it's own thread and
 * prepare
 * @param _pathTofile
 */
void Serializer::setPathToSendedFileSlot(QString _pathTofile)
{
    if(file.isOpen())
        file.close();

    file.setFileName(_pathTofile);
    if(!file.open(QIODevice::ReadOnly)){
        emit errorOpenFileString("Error in opening file for transfering!");
        return;
    }
    buffer->setSizeOfFile(file.size());
}

void Serializer::closeFileWithPort()
{
    if(file.isOpen())
        file.close();
}

