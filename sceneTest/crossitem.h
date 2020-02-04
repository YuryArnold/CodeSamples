#ifndef CROSSITEM_H
#define CROSSITEM_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QLineF>
#include <QVector>

class CrossItem : public QGraphicsItem
{
public:
    CrossItem();
    CrossItem(double _xCoord,double _yCoord);
    void initCross();
    virtual ~CrossItem();
protected:
    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QVector<QLineF> lines;
    int width;
    int height;
    int leftUpCornerX;
    int leftUpCornerY;
};

#endif // CROSSITEM_H
