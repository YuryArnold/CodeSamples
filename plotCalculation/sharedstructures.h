#ifndef SHAREDSTRUCTURES_H
#define SHAREDSTRUCTURES_H
#include <QVector>
#include <map>
#include <QMetaType>
using vectorOfPoints = QVector<std::pair<double,double>>;

enum stateOfCalculation{
    pause,
    over
};

struct FunctionParameters{

    FunctionParameters(){
        A = B= C = 0;
        start = step = stop = 0;
        startInd = endInd = 0;
        state = pause;
    }

    double A;
    double B;
    double C;
    double step;
    double start;
    double stop;
    int startInd;
    int endInd;
    stateOfCalculation state;
    QString function;
};


Q_DECLARE_METATYPE(FunctionParameters)

#endif // SHAREDSTRUCTURES_H
