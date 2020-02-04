#ifndef SETTINGSSTRUCTURE_H
#define SETTINGSSTRUCTURE_H
#include <QSerialPort>
#include <QSerialPortInfo>

struct Settings {
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};

#endif // SETTINGSSTRUCTURE_H
