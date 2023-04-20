#include "graphicsscene.h"


GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene (parent)
{
    isHead = false;
    isTail = false;
}

GraphicsScene::~GraphicsScene()
{

}

void GraphicsScene::resetScene()
{
    isHead = false;
    isTail = false;
    head = nullptr;
    tail = nullptr;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        SquareItem *square = qgraphicsitem_cast<SquareItem *>(item);

        if(!square)
            return;

        if(square->getState() == State::Colored)
            return;

        if(head)
            head->setState(State::Empty);


        square->setState(State::Start);
        isHead = true;
        head = square;


    } else if (event->button() == Qt::RightButton) {
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        SquareItem *square = qgraphicsitem_cast<SquareItem *>(item);

        if(!square)
            return;

        if(square->getState() == State::Colored)
            return;

        if(tail)
            tail->setState(State::Empty);


        square->setState(State::End);
        isTail = true;
        tail = square;
    }

    if(isHead && isTail)
        emit calculatePathSignal(head->getGridCoord(),
                                 tail->getGridCoord());
}
