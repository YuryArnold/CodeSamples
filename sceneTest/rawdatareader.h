#ifndef RAWDATAREADER_H
#define RAWDATAREADER_H
#include <QObject>
#include <QVector>
#include <QDataStream>
#include <QFile>
#include <QThread>
#include "typedefs.h"

class RawDataReader : public QObject
{
    Q_OBJECT
public:
    explicit RawDataReader(QObject* parent = nullptr);
    virtual ~RawDataReader();
    void parseDataFromFile();
    void setNumberOfPointsPerTransfer(int _numbOfPointsPortion);

signals:
    void parsedData(rawPointsContainer);

public slots:
    void setPathToFile(QString _pathToFile);

private:
    rawPointsContainer points;
    QString pathToRawFile;
    int numbOfPointsPortion;
};

#endif // RAWDATAREADER_H
