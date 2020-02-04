#ifndef SENDER_H
#define SENDER_H
#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMutex>
#include <QThread>
#include <atomic>
#include "settingsstructure.h"

class SmartBuffer;
class Serializer;

/**
 * @brief The Sender class
 * Class that incapsulates
 * QSerial port for sending
 * data, also contains loop
 * for writinfg data into port.
 */
class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(QObject *parent = nullptr);
    ~Sender();
    void setSettings(Settings settings);
    void setSerializerObject(Serializer* _serializer);
    bool isOpen();
    void setSizeOfFile(int);
    void setSmartBuffer(SmartBuffer* _buffrer);

signals:
    void duringSendProgress(int);
    void stringMessage(QString _message);
    void connectPortSignal(bool);
    void transferAccomplished();


public slots:
    void handleError(QSerialPort::SerialPortError error);
    void connectPort();
    void disconnectPort();
    void senderLoop();

private:
    QSerialPort* serialPort;
    QString errOutput;
    Settings settings;
    Serializer* serializer = nullptr;
    std::atomic<int> sizeOfFile;
    int sendedBytes;
    SmartBuffer* buffer = nullptr;

};

#endif // RECIERVERANDPARSER_H
