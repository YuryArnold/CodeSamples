#include "target.h"
#include <QDebug>
#include <cmath>
///Warning For Correct sending

Target::Target()
{

    azRad = 0;
    azDeg = 0;
    elRad = 0;
    Radius = 180;
}

Target::~Target()
{

}

double Target::d2r(double d)
{
    return (d / 180.0) * ((double) M_PI);
}

void Target::SetAzEl(double _azDeg, double _elDeg)
{
    azRad = d2r(_azDeg);
    elRad = d2r(_elDeg);
    this->RecalculatePos();
}

QString Target::GetName()
{
    return nameOfSat;
}

void Target::SetName(QString _name)
{
    nameOfSat = _name;
}

/**
 * @brief Target::boundingRect
 * @return
 */
QRectF Target::boundingRect() const
{
    return QRectF(-70,-35,140,70);
}

/**
 * @brief Target::paint
 * @param painter
 * @param option
 * @param widget
 */
void Target::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QBrush(Qt::red));
    painter->setPen(QPen(Qt::black));
    painter->drawEllipse(-5,-5,10,10);
    painter->drawText(5,5,nameOfSat);

}

/**
 * @brief Target::timerHandler
 */
void Target::RecalculatePos()
{
    QPointF newPos;
    azRad += d2r(1.0);
    double len = Radius * cos(elRad);
    double y = -len * cos(azRad) ;
    double x = (len*sin(azRad));//(azDeg > 180)?(len*sin(azRad)):
    newPos = QPointF(x, y);
    this->setPos(newPos);
    return;
}


