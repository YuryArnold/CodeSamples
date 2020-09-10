#ifndef SQUAREITEM_H
#define SQUAREITEM_H


#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QLineF>
#include <QVector>
#include <QGraphicsRectItem>
#include "typedefs.h"

enum State{
    Start,
    End,
    Empty,
    Path,
    Colored
};

class SquareItem : public QGraphicsRectItem
{
public:
    SquareItem();
    SquareItem(double _xCoord,double _yCoord);
    void setState(State _state);
    State getState () const;
    virtual ~SquareItem();
    void setGridCoord(int _x, int _y);
    Point getGridCoord();

private:
    State state;
    int xCell;
    int yCell;
};

#endif // SQUAREITEM_H
