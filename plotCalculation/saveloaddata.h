#ifndef SAVELOADDATA_H
#define SAVELOADDATA_H
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include "sharedstructures.h"

/*This class work after calculating,
 * so I don't need to synchronize data with mutexes*/
class SaveLoadData : public QObject
{
    Q_OBJECT
public:
    explicit SaveLoadData(QObject* parent = nullptr);
    virtual ~SaveLoadData();
    void setPointerOfDataContainer(vectorOfPoints* _data);

private:
    void packToJson(QJsonDocument& doc);
    void repackFromJson(QJsonDocument& doc);

signals:
    void getLoadedParametersSignal(FunctionParameters _parameters);
    void finished();

public slots:
    void readDataFromFile(QString _path);
    void writeDataToFile(QString _path);
    void setLoadedParametersSlot(FunctionParameters _parameters);

private:
    vectorOfPoints* data;
    FunctionParameters parameters;
};

#endif // SAVELOADDATA_H
