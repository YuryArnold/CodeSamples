#include "squareitem.h"

SquareItem::SquareItem() : QGraphicsRectItem ()
{
    setState(Empty);
    setPen(QPen(Qt::black));
}

SquareItem::SquareItem(double _xCoord, double _yCoord)
{
    setState(Empty);
    setPen(QPen(Qt::black));
    setPos(_xCoord,_yCoord);
}

SquareItem::~SquareItem()
{

}

void SquareItem::setState(State _state)
{
    state = _state;
    switch (state) {

        case Start:
        setBrush(QBrush(Qt::green));
        break;

    case End:
        setBrush(QBrush(Qt::yellow));
        break;


    case Empty:
        setBrush(QBrush(Qt::white));
        break;


    case Colored:
        setBrush(QBrush(Qt::black));
        break;


    case Path:
        setBrush(QBrush(Qt::blue));
        break;
    }
}

State SquareItem::getState() const
{
    return state;
}


void SquareItem::setGridCoord(int _x, int _y)
{
    xCell = _x;
    yCell = _y;
}

Point SquareItem::getGridCoord()
{
    return qMakePair(xCell,yCell);
}
