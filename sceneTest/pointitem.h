#ifndef POINTMARKER_H
#define POINTMARKER_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QLineF>
#include <QVector>


/**
 * @brief The PointItem class
 * This class creates point item
 * for Graphics Scene
 */
class PointItem : public QGraphicsItem
{
public:
    PointItem();
    virtual ~PointItem();
    void setPointRadius(float _radius);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    int radius;

};

#endif // POINTMARKER_H
