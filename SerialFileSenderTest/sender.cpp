#include "sender.h"
#include "smartbuffer.h"

Sender::Sender(QObject *parent) : QObject(parent)
{
    serialPort = new QSerialPort(this);
    connect(serialPort, &QSerialPort::errorOccurred, this, &Sender::handleError);
    sizeOfFile = 0;
    sendedBytes = 0;
}

Sender::~Sender()
{
    if(serialPort->isOpen())
        serialPort->close();
}

void Sender::setSettings(Settings _settings)
{
    settings = _settings;
}

void Sender::setSerializerObject(Serializer* _serializer)
{
    serializer = _serializer;
}

bool Sender::isOpen()
{
    return serialPort->isOpen();
}

void Sender::setSizeOfFile(int _size)
{
    sizeOfFile = _size;
}

void Sender::setSmartBuffer(SmartBuffer* _buffrer)
{
    buffer = _buffrer;
    connect(buffer,&SmartBuffer::startRead,
                this,&Sender::senderLoop);
}


void Sender::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::WriteError) {
        disconnectPort();
        emit stringMessage(serialPort->errorString());
    }
}

void Sender::connectPort()
{
    QString portNamePrefix = "";
#ifdef Q_OS_LINUX
    portNamePrefix = "/dev/";
#endif

    serialPort->setPortName(portNamePrefix + settings.name);
    if (serialPort->open(QIODevice::WriteOnly)) {
        if (serialPort->setBaudRate(settings.baudRate)
                && serialPort->setDataBits(settings.dataBits)
                && serialPort->setParity(settings.parity)
                && serialPort->setStopBits(settings.stopBits)
                && serialPort->setFlowControl(settings.flowControl))
        {
            if (serialPort->isOpen()){
                emit stringMessage((settings.name+ " >> Opened!\r"));
                emit connectPortSignal(true);
            }
        } else {
            serialPort->close();
            emit stringMessage(serialPort->errorString());
            emit connectPortSignal(false);
          }
    } else {
        serialPort->close();
        emit stringMessage(serialPort->errorString());
        emit connectPortSignal(false);
    }
}

void Sender::disconnectPort()
{
    if(serialPort->isOpen())
        serialPort->close();
    emit stringMessage("Port closed!");
    emit connectPortSignal(false);
}


/**
 * @brief Sender::senderLoop
 * this method is cv analogue
 * for sendong data in the loop,
 * while smart buffer isn't empty.
 */
void Sender::senderLoop()
{
    QByteArray sendBurst;
    int sizeOfFile = buffer->getSizeOfFile();
    while(buffer->canRead()){
        buffer->getBurst(sendBurst);
        int transferedSizeOfBurts = serialPort->write(sendBurst);
        serialPort->flush();
        sendBurst.clear();
        /*Here have to be emmit on transfering persent for transfering line*/
        sendedBytes += transferedSizeOfBurts;
        emit duringSendProgress((sendedBytes*100)/sizeOfFile);
        if(float(sendedBytes/sizeOfFile) > 0.98){
            emit transferAccomplished();
            sendedBytes = 0;
        }
    }
}

