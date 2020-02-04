#ifndef BACKENDWRAPPER_H
#define BACKENDWRAPPER_H
#include <QObject>
#include <QUdpSocket>
#include <QSet>
#include <QString>

class BackEndWrapper : public QObject
{
    Q_OBJECT
public:
    explicit BackEndWrapper(QString _ipAddr, quint16 _port,QObject* parent = nullptr);
    ~BackEndWrapper();
    void setDataContainer(QSet<int>* _data);
    void addDatagramHeader(QByteArray& data);
    void addFinishDatagramHeader(QByteArray& data);

public slots:
    void startTransferingSlot();
    void createSocket();
    void loadFileSlot(QString _pathToFile);
    void saveFileSlot(QString _pathToFile);
    void generateDataSlot();

signals:
    void refreshWidgetSignal();
    void finishTransfering();
    void finished();

private:
    QUdpSocket* senderSocket;
    QSet<int>* data;
    QString ipAddr;
    quint16 port;
    QByteArray dataAsByteStream;
    int numberOfElemnts;

};

#endif // BACKENDWRAPPER_H
