#include "crossitem.h"

CrossItem::CrossItem()
{
    initCross();
}

CrossItem::CrossItem(double _xCoord, double _yCoord)
{
    setPos(_xCoord,_yCoord);
    initCross();
}

void CrossItem::initCross()
{
    leftUpCornerX = -1;
    leftUpCornerY = -1;
    width = 4;
    height = 4;

    QLineF first(0,-leftUpCornerY,
                 0,leftUpCornerY);
    QLineF second(leftUpCornerX,0,
                  -leftUpCornerX,0);


    //QLineF first(leftUpCornerX,-leftUpCornerY,
    //             -leftUpCornerX,leftUpCornerY);
    //QLineF second(leftUpCornerX,leftUpCornerY,
    //              -leftUpCornerX,-leftUpCornerY);
    lines.append(first);
    lines.append(second);
}

CrossItem::~CrossItem()
{

}

/**
 * @brief CrossItem::boundingRect
 * All incoming data are in pixels
 * Here's have to be no magick digits
 * @return
 */
QRectF CrossItem::boundingRect() const
{
    return QRectF(leftUpCornerX,leftUpCornerY,
                  width,height);
}


/**
 * @brief CrossItem::paint
 * Have to Create Cross in here
 * @param painter
 * @param option
 * @param widget
 */
void CrossItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::magenta));
    painter->drawLines(lines);
}
