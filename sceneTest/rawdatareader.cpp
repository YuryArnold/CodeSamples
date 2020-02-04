#include "rawdatareader.h"
#include <QDataStream>
#include <QDebug>

RawDataReader::RawDataReader(QObject *parent) : QObject (parent)
{
    numbOfPointsPortion = 1000;
}

RawDataReader::~RawDataReader()
{

}

void RawDataReader::parseDataFromFile()
{
    int cntOfPoints = 0;
    QPair<int16_t,int16_t> data;
    QFile file(pathToRawFile);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in.setByteOrder(in.LittleEndian);
    while(!in.atEnd()){
        if(cntOfPoints == numbOfPointsPortion){
            emit parsedData(points);
            //just sleep for some time
            QThread::msleep(200);
            cntOfPoints = 0;
            points.clear();
        }
        in>>data;
        points.append(data);
        cntOfPoints++;
    }

    if(!points.isEmpty()){
        emit parsedData(points);
        points.clear();
    }

    file.close();
    qDebug()<<"Reading of points Are finished!";
}

void RawDataReader::setNumberOfPointsPerTransfer(int _numbOfPointsPortion)
{
    numbOfPointsPortion = _numbOfPointsPortion;
}

void RawDataReader::setPathToFile(QString _pathToRawFile)
{
    pathToRawFile = _pathToRawFile;
    parseDataFromFile();
}
