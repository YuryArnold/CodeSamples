#ifndef SNAKEITEM_H
#define SNAKEITEM_H

#include <QObject>
#include <QVector>
#include "squareitem.h"

class SnakeItem : public QObject
{
    Q_OBJECT
public:
    explicit SnakeItem(QObject* parent = nullptr);
    ~SnakeItem();
    void setScene();

public slots:
    void printSnakeSlot(PointsContainer points);


private:
    QVector<SquareItem> snakeBody;
    PointsContainer duringPoints;

};

#endif // SNAKEITEM_H
