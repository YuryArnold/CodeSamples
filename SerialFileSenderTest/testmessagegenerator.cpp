#include "testmessagegenerator.h"

TestMessageGenerator::TestMessageGenerator(QObject *parent) : QObject(parent)
{
    sendMessageTimer.setInterval(3000);

    message.clear();
    generateMessage(message);
    QString serialPortName = "/dev/tnt0";
    generator.setPortName(serialPortName);
    generator.open(QSerialPort::WriteOnly);

    connect(&sendMessageTimer,&QTimer::timeout,this,[&](){
            generator.write(message);
            generator.waitForBytesWritten(2000);
    });

}

TestMessageGenerator::~TestMessageGenerator()
{
    if(generator.isOpen())
        generator.close();
}

void TestMessageGenerator::generateMessage(QByteArray &data)
{
    data.append(0x40);
    float digit = 3.45;
    data.append(reinterpret_cast<const char*>(&digit),4);
    digit = 5.8;
    data.append(reinterpret_cast<const char*>(&digit),4);
    digit = 12.28;
    data.append(reinterpret_cast<const char*>(&digit),4);
    digit = -6.3;
    data.append(reinterpret_cast<const char*>(&digit),4);
}

void TestMessageGenerator::start()
{
    if(sendMessageTimer.isActive())
        sendMessageTimer.stop();
    else
        sendMessageTimer.start();
}
