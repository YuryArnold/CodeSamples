#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QGraphicsScene>
#include <QPoint>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <squareitem.h>
#include "typedefs.h"

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
     explicit GraphicsScene(QObject *parent = 0);
    ~GraphicsScene();
    void resetScene();

signals:
    void calculatePathSignal(Point _first,
                             Point _second);

    // QGraphicsScene interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    bool isHead;
    bool isTail;
    SquareItem* head = nullptr;
    SquareItem* tail = nullptr;
};

#endif // GRAPHICSCENE_H


