#include "saveloaddata.h"

SaveLoadData::SaveLoadData(QObject *parent):QObject(parent)
{

}

SaveLoadData::~SaveLoadData()
{
    emit finished();
}

void SaveLoadData::setLoadedParametersSlot(FunctionParameters _parameters)
{
    parameters = _parameters;
}

void SaveLoadData::setPointerOfDataContainer(vectorOfPoints *_data)
{
    data = _data;
}

void SaveLoadData::packToJson(QJsonDocument& doc)
{
     QJsonArray arrayOfFunction;
     QJsonArray arrayOfArguments;
     for(auto& item : *data){
        arrayOfArguments.append(item.first);
        arrayOfFunction.append(item.second);
     }
     QJsonObject jsonObj;
     jsonObj.insert("A", parameters.A);
     jsonObj.insert("B", parameters.B);
     jsonObj.insert("C",parameters.C);
     jsonObj.insert("Start", parameters.start);
     jsonObj.insert("Stop", parameters.stop);
     jsonObj.insert("Step", parameters.step);
     jsonObj.insert("State", parameters.state);
     jsonObj.insert("StartInd",parameters.startInd);
     jsonObj.insert("EndInd",parameters.endInd);
     jsonObj.insert("function",parameters.function);
     jsonObj.insert("argArray",arrayOfArguments);
     jsonObj.insert("funcArray",arrayOfFunction);
     doc.setObject(jsonObj);
}

void SaveLoadData::repackFromJson(QJsonDocument &doc)
{
    data->clear();
    QJsonObject obj = doc.object();
    parameters.A = obj["A"].toDouble();
    parameters.B = obj["B"].toDouble();
    parameters.C = obj["C"].toDouble();
    parameters.start = obj["Start"].toDouble();
    parameters.stop = obj["Stop"].toDouble();
    parameters.step = obj["Step"].toDouble();
    parameters.startInd = obj["StartInd"].toDouble();
    parameters.endInd = obj["EndInd"].toDouble();
    parameters.function = obj["function"].toString();
    parameters.state = stateOfCalculation(obj["State"].toInt());
    /*I need to read array from json!*/
    /*They are the same size!*/
    QJsonArray arg = obj["argArray"].toArray();
    QJsonArray func = obj["funcArray"].toArray();
    for(int ind = 0; ind < arg.size(); ++ind)
        data->append(std::make_pair(arg.at(ind).toDouble(),
                                    func.at(ind).toDouble()));

    emit getLoadedParametersSignal(parameters);
}

void SaveLoadData::readDataFromFile(QString _pathToFile)
{
   QJsonDocument doc;
   QFile file(_pathToFile);
       if(!file.open(QFile::ReadOnly | QFile::Text)){
           QMessageBox::warning(nullptr,tr("Plot."),tr("Error in load file\nTry another directory."));
           file.close();
           return;
       } else {

           doc = QJsonDocument().fromJson(file.readAll());
       }
       file.close();
       repackFromJson(doc);
}

void SaveLoadData::writeDataToFile(QString _pathToFile)
{
    QFile file(_pathToFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(nullptr,tr("Plot."),tr("Error in save file\nTry another directory."));
        file.close();
        return;
    } else {
        QJsonDocument doc;
        packToJson(doc);
        file.write(doc.toJson());
    }
    file.close();
}


