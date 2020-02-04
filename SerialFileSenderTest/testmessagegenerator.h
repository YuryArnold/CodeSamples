#ifndef TESTMESSAGEGENERATOR_H
#define TESTMESSAGEGENERATOR_H
#include <QSerialPort>
#include <QObject>
#include <QTimer>

class TestMessageGenerator : public QObject
{
    Q_OBJECT
public:
    explicit TestMessageGenerator(QObject *parent = nullptr);
    ~TestMessageGenerator();
    void generateMessage(QByteArray& data);
    void start();

private:
    QSerialPort generator;
    QTimer sendMessageTimer;
    QByteArray message;
};

#endif // TESTMESSAGEGENERATOR_H
