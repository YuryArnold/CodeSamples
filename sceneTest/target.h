#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QTimer>
#include <cmath>

class Target : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Target();
    virtual ~Target();
    double d2r(double d);
    void SetAzEl(double , double);
    QString GetName();
    void SetName(QString );

protected:
    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private slots:
    void RecalculatePos();

protected:
    double azRad;
    double elRad;
    double azDeg;
    double Radius;
    QString nameOfSat;
};

