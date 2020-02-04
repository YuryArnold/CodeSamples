#include "pointitem.h"

PointItem::PointItem()
{
    radius = 1;
}

PointItem::~PointItem()
{

}

void PointItem::setPointRadius(float _radius)
{

}

QRectF PointItem::boundingRect() const
{
    return QRectF(-radius/2,-radius/2,
                  radius,radius);
}

void PointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::yellow));


}
