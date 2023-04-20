#ifndef BFSFINDER_H
#define BFSFINDER_H
#include <QObject>
#include <memory>
#include <QVector>
#include <QSet>
#include <queue>
#include <QMutex>
#include "typedefs.h"


struct bfsNode
{
    Point pt;
    PointsContainer pathPoint;
    bfsNode(Point _pt){
        pathPoint.append(_pt);
        pt= _pt;
    }
};


class BfsFinder : public QObject
{
    Q_OBJECT
public:
    BfsFinder(QObject* parent = nullptr);
    ~BfsFinder();
    void setStateArray(stateArr& _states);
    bool BFSCalculate(Point _src, Point _dest,PointsContainer& _path);

private:
    bool isValid(int row, int col){
        return (row >= 0) && (row < rows) &&
               (col >= 0) && (col < colums);
    }

    stateArr states;
    int rows;
    int colums;
    int rowNum[4] = {-1, 0, 0, 1};
    int colNum[4] = {0, -1, 1, 0};

};

#endif // BFSFINDER_H
