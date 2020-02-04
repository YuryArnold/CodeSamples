#include "backendwrapper.h"
#include <QtXml>
#include <QMessageBox>
#include <QTextStream>

BackEndWrapper::BackEndWrapper(QString _ipAddr, quint16 _port, QObject *parent) : QObject(parent)
{
    ipAddr = _ipAddr;
    port = _port;
    numberOfElemnts = int(1e6);
}

BackEndWrapper::~BackEndWrapper()
{
    senderSocket->close();
    delete senderSocket;
    emit finished();
}

void BackEndWrapper::setDataContainer(QSet<int>* _data)
{
    data = _data;
}

void BackEndWrapper::addDatagramHeader(QByteArray &data)
{
    const char* head = "mess";
    data.insert(0,head,4);
}

void BackEndWrapper::addFinishDatagramHeader(QByteArray &data)
{
    const char* head = "endm";
    data.insert(0,head,4);
}


void BackEndWrapper::startTransferingSlot()
{
    /*First of all deserialize data and move it to my container*/
    /*Prepare data and send it into socket*/
    for(auto& itemOfSet: *data){
        dataAsByteStream.append(reinterpret_cast<const char*>(&itemOfSet),4);
    }

    /*number of informational bytes in one datagram*/
    int portion = 40000;
    QByteArray datagram;
    /*After it i should transfer datagram*/
    while(!dataAsByteStream.isEmpty()){
        std::copy(dataAsByteStream.begin(),
                  dataAsByteStream.begin() + portion,
                  std::back_inserter(datagram));

        (dataAsByteStream.length() <= portion)?(addFinishDatagramHeader(datagram)):(addDatagramHeader(datagram));

        senderSocket->writeDatagram(datagram,QHostAddress(ipAddr),port);
        dataAsByteStream.remove(0,portion);
        datagram.clear();
    }
    emit finishTransfering();
}

void BackEndWrapper::loadFileSlot(QString _pathToFile)
{
    data->clear();
    QFile file(_pathToFile);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(nullptr,tr("Udp Sender."),tr("Error in load file\nTry another directory/"));
        file.close();
        return;
    }

    QXmlStreamReader xmlDoc(&file);

    while(!xmlDoc.atEnd() && !xmlDoc.hasError()){
        QXmlStreamReader::TokenType token = xmlDoc.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        if(token == QXmlStreamReader::StartElement) {
            if(xmlDoc.name() == "Digits")
                continue;
            if(xmlDoc.name() == "Digit"){
                QXmlStreamAttributes attributes = xmlDoc.attributes();
                QString value = attributes.value("Value").toString();
                data->insert(value.toInt());
            }

        }
    }
    file.close();
    emit refreshWidgetSignal();
}

void BackEndWrapper::saveFileSlot(QString _pathToFile)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("Digits");
    QFile file(_pathToFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(nullptr,tr("Udp Sender."),tr("Error in save file\nTry another directory/"));
        file.close();
        return;
    } else {
        doc.appendChild(root);
        // Adding more elements
        for(auto& item: *data){
            QDomElement valueNode = doc.createElement("Digit");
            valueNode.setAttribute("Value",QString::number(item));
            root.appendChild(valueNode);
        }
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

void BackEndWrapper::generateDataSlot()
{
    data->clear();
    while(data->size() < numberOfElemnts){
        int value = ((rand()%(2*numberOfElemnts)) - numberOfElemnts);
        data->insert(value);
    }
    emit refreshWidgetSignal();
}

void BackEndWrapper::createSocket()
{
    senderSocket = new QUdpSocket;
}
